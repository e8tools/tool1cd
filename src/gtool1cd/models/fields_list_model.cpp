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
#include "fields_list_model.h"


FieldsListModel::FieldsListModel(Table *table)
    : table(table) {}

int FieldsListModel::rowCount(const QModelIndex &parent) const
{
	return table->get_num_fields();
}

int FieldsListModel::columnCount(const QModelIndex &parent) const
{
	return 6;
}

QModelIndex FieldsListModel::index(int row, int column, const QModelIndex &parent) const
{
	return QAbstractItemModel::createIndex(row, column);
}

QModelIndex FieldsListModel::parent(const QModelIndex &child) const
{
	return QModelIndex();
}

QVariant FieldsListModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}
	if (role == Qt::DisplayRole) {
		Field *f = table->get_field(index.row());
		switch (index.column()) {
		case 0:
			return QString::fromStdString(f->get_name());

		case 1:
			return QString::fromStdString(f->get_presentation_type());
		case 2:
			return QVariant::fromValue(f->get_length());
		case 3:
			return QVariant::fromValue(f->get_precision());
		case 4:
			return f->get_null_exists() ? QString("NULL") : QString("NOT NULL");
		case 5:
			return f->get_case_sensitive() ? QString("CS") : QString("CI");
		} // switch
	}

	return QVariant();
}

QVariant FieldsListModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (section) {
		case 0:
			return tr("Имя");
		case 1:
			return tr("Тип");
		case 2:
			return tr("Длина");
		case 3:
			return tr("Точность");
		case 4:
			return tr("м.б. NULL");
		case 5:
			return tr("РегистроЧувствительность");
		}
	}
	return QVariant();
}
