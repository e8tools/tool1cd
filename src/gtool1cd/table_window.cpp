#include "table_window.h"
#include "ui_table_window.h"
#include <Table.h>

TableWindow::TableWindow(QWidget *parent, Table *table)
    : QMainWindow(parent), table(table),
      ui(new Ui::TableWindow)
{
	ui->setupUi(this);
	setWindowTitle(QString::fromStdString(static_cast<std::string>(table->getname())));
	ui->fieldsView->setModel(new FieldsListModel(table));
}

TableWindow::~TableWindow()
{

}
