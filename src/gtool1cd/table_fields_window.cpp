/*
    GTool1CD provides GUI front end to Tool1CD library
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of GTool1CD.

    GTool1CD is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GTool1CD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GTool1CD.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "table_fields_window.h"
#include "ui_table_fields_window.h"
#include <Table.h>
#include "models/fields_list_model.h"

TableFieldsWindow::TableFieldsWindow(QWidget *parent, Table *table)
    : QMainWindow(parent), table(table),
      ui(new Ui::TableFieldsWindow)
{
	ui->setupUi(this);
	setWindowTitle(QString::fromStdString(table->get_name()));
	ui->fieldsView->setModel(new FieldsListModel(table));
}

TableFieldsWindow::~TableFieldsWindow()
{

}
