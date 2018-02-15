#ifndef TABLEDATAMODEL_H
#define TABLEDATAMODEL_H

#include <QAbstractItemModel>
#include <Table.h>
#include <TableIterator.h>

class TableDataModel : public QAbstractItemModel
{
public:
	TableDataModel(Table *table)
	    : table(table) {}

	int rowCount(const QModelIndex &parent) const override
	{
		return table->get_phys_numrecords();
	}

	int columnCount(const QModelIndex &parent) const override
	{
		return table->get_numfields();
	}

	QModelIndex index(int row, int column, const QModelIndex &parent) const override
	{
		return QAbstractItemModel::createIndex(row, column);
	}

	QModelIndex parent(const QModelIndex &child) const override
	{
		return QModelIndex();
	}


	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override
	{
		if (!index.isValid()) {
			return QVariant();
		}
		if (role == Qt::DisplayRole) {
			Field *f = table->getfield(index.column());
			TableRecord *record = table->getrecord(index.row());
			if (record->is_null_value(f)) {
				return QString("{NULL}");
			}
			return QString::fromStdString(static_cast<std::string>(record->get_string(f)));
		}

		return QVariant();
	}

	QVariant headerData(int section, Qt::Orientation orientation, int role) const override
	{
		if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
			Field *f = table->getfield(section);
			return QString::fromStdString(static_cast<std::string>(f->getname()));
		}
		return QVariant();
	}

	Table *table;
};


#endif // TABLEDATAMODEL_H
