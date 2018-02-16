#include "table_data_window.h"
#include "ui_table_data_window.h"
#include "table_fields_window.h"
#include <Table.h>
#include "skobkatextwindow.h"
#include "models/table_data_model.h"
#include <QFileDialog>

TableDataWindow::TableDataWindow(QWidget *parent, Table *table)
    : QMainWindow(parent), table(table),
      ui(new Ui::TableDataWindow),
      tableWindow(nullptr)
{
	ui->setupUi(this);
	setWindowTitle(QString::fromStdString(static_cast<std::string>(table->getname())));
	ui->dataView->setModel(new TableDataModel(table));
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
	QString exportFileName = QFileDialog::getSaveFileName(this,
	                             tr("Экспорт таблицы"),
	                             "", "*.xml");
	if (exportFileName.isNull()) {
		return;
	}
	table->fillrecordsindex();
	table->export_to_xml(exportFileName.toStdString(), true, false);
}
