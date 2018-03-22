#ifndef TABLESLISTMODEL_H
#define TABLESLISTMODEL_H

#include <Class_1CD.h>
#include <QAbstractItemModel>

class TablesListModel : public QAbstractItemModel
{
public:
	explicit TablesListModel(T_1CD *db);

	int rowCount(const QModelIndex &parent) const override;

	int columnCount(const QModelIndex &parent) const override;

	QModelIndex index(int row, int column, const QModelIndex &parent) const override;

	QModelIndex parent(const QModelIndex &child) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
	T_1CD *db;
};

#endif // TABLESLISTMODEL_H
