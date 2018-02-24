#ifndef TABLEDATAMODEL_H
#define TABLEDATAMODEL_H

#include <QAbstractItemModel>
#include <Table.h>
#include <TableIterator.h>

class TableDataModel : public QAbstractItemModel
{
public:
	TableDataModel(Table *table, Index *index = nullptr);

	int rowCount(const QModelIndex &parent) const override;

	int columnCount(const QModelIndex &parent) const override;

	QModelIndex index(int row, int column, const QModelIndex &parent) const override;

	QModelIndex parent(const QModelIndex &child) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

	void dumpBlob(const QModelIndex &index, const QString &filename) const;

	bool isBlobValue(const QModelIndex &index) const;

	bool isClobValue(const QModelIndex &index) const;

	V8Catalog *getCatalog(const QModelIndex &index) const;

	const TableRecord *getRecord(const QModelIndex &index) const;

private:
	Table *table;
	Index *_index;
};


#endif // TABLEDATAMODEL_H
