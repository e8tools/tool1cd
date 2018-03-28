#include "table_data_window.h"
#include "ui_table_data_window.h"
#include "table_fields_window.h"
#include <Table.h>
#include "skobkatextwindow.h"
#include "models/table_data_model.h"
#include "export_table_to_xml_dialog.h"
#include <QDebug>
#include <QItemSelection>
#include "models/skobka_tree_model.h"
#include <QFileDialog>
#include "models/v8catalog_tree_model.h"
#include "QHexEdit/qhexedit.h"

QString index_presentation(Index *index)
{
	QString presentation = QString::fromStdString(index->get_name());
	presentation += QString(": ");
	for (int i = 0; i < index->get_num_records(); i++) {
		if (i != 0) {
			presentation += QString(", ");
		}
		presentation += QString::fromStdString(index->get_records()[i].field->get_name());
	}
	return presentation;
}

TableDataWindow::TableDataWindow(QWidget *parent, Table *table)
    : QMainWindow(parent), table(table),
      ui(new Ui::TableDataWindow),
      tableWindow(nullptr)
{
	ui->setupUi(this);
	setWindowTitle(QString::fromStdString(table->getname()));
	ui->dataView->setModel(new TableDataModel(table));

	hexedit = new QHexEdit();
	auto layout = new QVBoxLayout();
	layout->addWidget(hexedit);
	ui->hexeditFrame->setLayout(layout);

	QVector<Index*> indexes;
	indexes.push_back(nullptr); // PK

	for (int i = 0; i < table->get_numindexes(); i++) {
		Index *index = table->getindex(i);
		if (index->is_primary() && indexes[0] == nullptr) {
			indexes[0] = index;
		} else {
			indexes.push_back(index);
		}
	}

	ui->indexChooseBox->addItem(tr("<без индекса>"));
	for (Index *index : indexes) {
		if (index == nullptr) {
			continue;
		}
		ui->indexChooseBox->addItem(index_presentation(index), QString::fromStdString(index->get_name()));
	}

	connect(ui->dataView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
	        this, SLOT(dataView_selection_changed(QItemSelection)));
}

TableDataWindow::~TableDataWindow()
{

}


void TableDataWindow::on_descriptionButton_clicked()
{
	SkobkaTextWindow *w = new SkobkaTextWindow(this);
	w->setText(QString::fromStdString(table->getdescription()),
	           QString::fromStdString(table->getname()));
	w->show();
}

void TableDataWindow::on_fieldsButton_clicked()
{
	if (tableWindow == nullptr) {
		tableWindow = new TableFieldsWindow(this, table);
	}
	tableWindow->show();
	tableWindow->activateWindow();
}

void TableDataWindow::on_exportToXmlButton_clicked()
{
	ExportTableToXmlDialog *export_dialog = new ExportTableToXmlDialog(table, this);
	export_dialog->show_with_file_dialog();
}

void TableDataWindow::on_indexChooseBox_activated(int index)
{
	if (index == 0) {
		// без индекса
		ui->dataView->setModel(new TableDataModel(table));
		return;
	}
	QString index_name = ui->indexChooseBox->itemData(index).toString();
	Index *table_index = table->get_index(index_name.toStdString());
	ui->dataView->setModel(new TableDataModel(table, table_index));

	connect(ui->dataView->selectionModel(), SIGNAL(selectionChanged(QItemSelection,QItemSelection)),
	        this, SLOT(dataView_selection_changed(QItemSelection)));
}

void TableDataWindow::on_dataView_activated(const QModelIndex &index)
{
}

tree *try_parse_tree(const QVariant &data)
{
	std::string string_data = data.toString().toStdString();
	if (string_data.substr(0, 1) != "{") {
		return nullptr;
	}
	try {
		tree *data_tree = parse_1Ctext(string_data, "");
		return data_tree;
	} catch (...) {
		return nullptr;
	}
}

void TableDataWindow::dataView_selection_changed(const QItemSelection &selection)
{
	if (selection.empty()) {
		return;
	}
	auto range = selection.at(0);
	auto column = range.left();
	auto row = range.top();
	auto index = ui->dataView->model()->index(row, column);
	auto model = static_cast<TableDataModel*>(ui->dataView->model());

	auto data = ui->dataView->model()->data(index, Qt::EditRole);

	QTextDocument *qd = new QTextDocument(data.toString());
	qd->setDocumentLayout(new QPlainTextDocumentLayout(qd));
	ui->plainTextEdit->setDocument(qd);
	ui->plainTextEdit->setVisible(true);

	ui->treeView->setVisible(false);
	hexedit->setVisible(false);

	if (model->isBlobValue(index)) {

		auto cat = model->getCatalog(index);
		if (cat != nullptr) {
			QString catalog_name("{catalog}");

			if (table->find_field("FILENAME") != nullptr) {

				auto rec = model->getRecord(index);
				catalog_name = QString::fromStdString(rec->get_string("FILENAME"));

			} else if (table->find_field("EXTNAME") != nullptr) {

				auto rec = model->getRecord(index);
				catalog_name = QString::fromStdString(rec->get_string("EXTNAME"));

			}
			ui->plainTextEdit->setVisible(false);
			ui->treeView->setModel(new V8CatalogTreeModel(cat, catalog_name));
			ui->treeView->setVisible(true);
			ui->treeView->expandAll();
			return;
		}

		auto stream = model->getBlobStream(index);
		if (stream != nullptr) {
			auto doc = QHexDocument::fromDevice(stream);
			hexedit->setDocument(doc);
			hexedit->setVisible(true);
			ui->plainTextEdit->setVisible(false);
			return;
		}
	}

	tree *data_tree = try_parse_tree(data);
	if (data_tree != nullptr) {
		ui->treeView->setModel(new SkobkaTreeModel(data_tree));
		ui->treeView->setVisible(true);
		ui->treeView->expandAll();
		return;
	}

}

void TableDataWindow::on_saveBlobButton_clicked()
{
	auto index = ui->dataView->currentIndex();
	if (!index.isValid()) {
		return;
	}
	QString filename = QFileDialog::getSaveFileName(this, tr("Сохранение..."), "");
	if (filename.isNull()) {
		return;
	}
	auto model = static_cast<TableDataModel*>(ui->dataView->model());
	model->dumpBlob(index, filename);
}
