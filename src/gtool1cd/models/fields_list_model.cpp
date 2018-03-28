#include "fields_list_model.h"


FieldsListModel::FieldsListModel(Table *table)
    : table(table) {}

int FieldsListModel::rowCount(const QModelIndex &parent) const
{
	return table->get_numfields();
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
		Field *f = table->getfield(index.row());
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
