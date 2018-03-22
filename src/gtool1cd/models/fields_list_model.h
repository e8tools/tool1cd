#ifndef FIELDSMODEL_H
#define FIELDSMODEL_H

#include <QAbstractItemModel>
#include <Table.h>

class FieldsListModel : public QAbstractItemModel
{
public:
	explicit FieldsListModel(Table *table);

	int rowCount(const QModelIndex &parent) const override;

	int columnCount(const QModelIndex &parent) const override;

	QModelIndex index(int row, int column, const QModelIndex &parent) const override;

	QModelIndex parent(const QModelIndex &child) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

	Table *table;
};


#endif // FIELDSMODEL_H
