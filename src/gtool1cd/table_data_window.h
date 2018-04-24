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
#ifndef TABLEDATAWINDOW_H
#define TABLEDATAWINDOW_H

#include <QMainWindow>
#include <QAbstractItemModel>
#include <Table.h>
#include <TableIterator.h>
#include <QItemSelection>
#include "QHexEdit/qhexedit.h"

namespace Ui {
class TableDataWindow;
}

class TableDataWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit TableDataWindow(QWidget *parent, Table *table);
	~TableDataWindow();

private slots:

	void on_descriptionButton_clicked();

	void on_fieldsButton_clicked();

	void on_exportToXmlButton_clicked();

	void on_indexChooseBox_activated(int index);

	void on_dataView_activated(const QModelIndex &index);

	void dataView_selection_changed(const QItemSelection &selection);

	void on_saveBlobButton_clicked();

private:
	Table *table;
	Ui::TableDataWindow *ui;
	QWidget *tableWindow;
	QHexEdit *hexedit;
};

#endif // TABLEDATAWINDOW_H
