#include "vendor_configurations_table_model.h"
#include <Class_1CD.h>
#include <QString>

VendorConfigurationsTableModel::VendorConfigurationsTableModel(T_1CD *db):
    db(db)
{

}

int VendorConfigurationsTableModel::rowCount(const QModelIndex &parent) const
{
	return db->supplier_configs().size();
}

int VendorConfigurationsTableModel::columnCount(const QModelIndex &parent) const
{
	return 3;
}

QModelIndex VendorConfigurationsTableModel::index(int row, int column, const QModelIndex &parent) const
{
	return QAbstractItemModel::createIndex(row, column);
}

QModelIndex VendorConfigurationsTableModel::parent(const QModelIndex &child) const
{
	return QModelIndex();
}

QVariant VendorConfigurationsTableModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}
	if (role == Qt::DisplayRole) {
		auto config = db->supplier_configs().at(index.row());
		switch (index.column()) {
		case 0:
			return QString::fromStdString(config->name());
		case 1:
			return QString::fromStdString(config->supplier());
		case 2:
			return QString::fromStdString(config->version());
		} // switch
	}

	return QVariant();
}

QVariant VendorConfigurationsTableModel::headerData(int section, Qt::Orientation orientation, int role) const
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

