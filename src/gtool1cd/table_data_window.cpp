#include "table_data_window.h"
#include "ui_table_data_window.h"
#include "table_fields_window.h"
#include <Table.h>
#include "skobkatextwindow.h"
#include "models/table_data_model.h"
#include "export_table_to_xml_dialog.h"

QString index_presentation(Index *index)
{
	QString presentation = QString::fromStdString(index->getname());
	presentation += QString(": ");
	for (int i = 0; i < index->get_num_records(); i++) {
		if (i != 0) {
			presentation += QString(", ");
		}
		presentation += QString::fromStdString(index->get_records()[i].field->getname());
	}
	return presentation;
}

TableDataWindow::TableDataWindow(QWidget *parent, Table *table)
    : QMainWindow(parent), table(table),
      ui(new Ui::TableDataWindow),
      tableWindow(nullptr)
{
	ui->setupUi(this);
	setWindowTitle(QString::fromStdString(static_cast<std::string>(table->getname())));
	ui->dataView->setModel(new TableDataModel(table));

	QVector<Index*> indexes;
	indexes.push_back(nullptr); // PK

	for (int i = 0; i < table->get_numindexes(); i++) {
		Index *index = table->getindex(i);
		if (index->get_is_primary() && indexes[0] == nullptr) {
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
		ui->indexChooseBox->addItem(index_presentation(index), QString::fromStdString(index->getname()));
	}
}

TableDataWindow::~TableDataWindow()
{

}


void TableDataWindow::on_descriptionButton_clicked()
{
	SkobkaTextWindow *w = new SkobkaTextWindow(this);
	w->setText(QString::fromStdString(static_cast<std::string>(table->getdescription())),
	           QString::fromStdString(static_cast<std::string>(table->getname())));
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
}
