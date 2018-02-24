#ifndef SKOBKA_TREE_MODEL_H
#define SKOBKA_TREE_MODEL_H

#include <Parse_tree.h>
#include <QAbstractItemModel>

class SkobkaTreeModel : public QAbstractItemModel
{
public:
	SkobkaTreeModel(tree *data_tree);


	virtual int rowCount(const QModelIndex &parent) const override;

	virtual int columnCount(const QModelIndex &parent) const override;

	virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;

	virtual QModelIndex parent(const QModelIndex &child) const override;

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
	tree* data_tree;
};

#endif // SKOBKA_TREE_MODEL_H
