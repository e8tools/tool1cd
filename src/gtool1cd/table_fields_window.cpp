#include "table_fields_window.h"
#include "ui_table_fields_window.h"
#include <Table.h>
#include "models/fields_list_model.h"

TableFieldsWindow::TableFieldsWindow(QWidget *parent, Table *table)
    : QMainWindow(parent), table(table),
      ui(new Ui::TableFieldsWindow)
{
	ui->setupUi(this);
	setWindowTitle(QString::fromStdString(static_cast<std::string>(table->getname())));
	ui->fieldsView->setModel(new FieldsListModel(table));
}

TableFieldsWindow::~TableFieldsWindow()
{

}
