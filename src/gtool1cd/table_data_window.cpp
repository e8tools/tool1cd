#include "table_data_window.h"
#include "ui_table_data_window.h"
#include "table_window.h"
#include <Table.h>

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

}

void TableDataWindow::on_fieldsButton_clicked()
{
	if (tableWindow == nullptr) {
		tableWindow = new TableWindow(this, table);
	}
	tableWindow->show();
	tableWindow->activateWindow();
}
