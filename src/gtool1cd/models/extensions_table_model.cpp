#include "extensions_table_model.h"
#include <Class_1CD.h>

ExtensionsTableModel::ExtensionsTableModel(T_1CD *db):
    db(db)
{

}


int ExtensionsTableModel::rowCount(const QModelIndex &parent) const
{
	return db->supplier_configs().size();
}

int ExtensionsTableModel::columnCount(const QModelIndex &parent) const
{
	return 3;
}

QModelIndex ExtensionsTableModel::index(int row, int column, const QModelIndex &parent) const
{
	return QAbstractItemModel::createIndex(row, column);
}

QModelIndex ExtensionsTableModel::parent(const QModelIndex &child) const
{
	return QModelIndex();
}

QVariant ExtensionsTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}
	if (role == Qt::DisplayRole) {
	}

	return QVariant();
}

QVariant ExtensionsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (section) {
		case 0:
			return tr("Конфигурация");
		case 1:
			return tr("Поставщик");
		case 2:
			return tr("Версия");
		}
	}
	return QVariant();
}
