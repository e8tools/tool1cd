#ifndef EXTENSIONS_TABLE_MODEL_H
#define EXTENSIONS_TABLE_MODEL_H

#include <QAbstractItemModel>

class T_1CD;

class ExtensionsTableModel : public QAbstractItemModel
{
public:
	explicit ExtensionsTableModel(T_1CD *db);

	virtual int rowCount(const QModelIndex &parent) const override;

	virtual int columnCount(const QModelIndex &parent) const override;

	virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;

	virtual QModelIndex parent(const QModelIndex &child) const override;

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
private:
	T_1CD *db;
};

#endif // EXTENSIONS_TABLE_MODEL_H
