#ifndef TABLEDATAMODEL_H
#define TABLEDATAMODEL_H

#include <QAbstractItemModel>
#include <Table.h>
#include <TableIterator.h>

class TableDataModel : public QAbstractItemModel
{
public:
	TableDataModel(Table *table, Index *index = nullptr)
	    : table(table), _index(index) {}

	int rowCount(const QModelIndex &parent) const override
	{
		if (_index == nullptr) {
			return table->get_phys_numrecords();
		}
		return _index->get_numrecords();
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
		TableRecord *record = _index == nullptr
		        ? table->getrecord(index.row())
		        : table->getrecord(_index->get_numrec(index.row()));
		if (role == Qt::DisplayRole) {
			Field *f = table->getfield(index.column());
			if (record->is_null_value(f)) {
				return QString("{NULL}");
			}
			return QString::fromStdString(static_cast<std::string>(record->get_string(f)));
		}
		if (role == Qt::FontRole) {
			if (record->is_removed()) {
				QFont italic;
				italic.setItalic(true);
				return italic;
			}
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
	Index *_index;
};


#endif // TABLEDATAMODEL_H
