#ifndef V8CATALOG_TREE_MODEL_H
#define V8CATALOG_TREE_MODEL_H

#include <cfapi/V8Catalog.h>
#include <cfapi/V8File.h>
#include <QAbstractItemModel>

class V8CatalogTreeModel : public QAbstractItemModel
{
public:
	V8CatalogTreeModel(V8Catalog *catalog, const QString &catalogName);

	virtual int rowCount(const QModelIndex &parent) const override;

	virtual int columnCount(const QModelIndex &parent) const override;

	virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;

	virtual QModelIndex parent(const QModelIndex &child) const override;

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
	V8Catalog* catalog;
	QString catalogName;
};


#endif // V8CATALOG_TREE_MODEL_H
