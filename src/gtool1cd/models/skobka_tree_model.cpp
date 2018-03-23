#include "skobka_tree_model.h"

SkobkaTreeModel::SkobkaTreeModel(tree *data_tree)
    : data_tree(data_tree)
{

}

int SkobkaTreeModel::rowCount(const QModelIndex &parent) const
{
	if (parent.column() > 0) {
		return 0;
	}

	tree *parentItem = parent.isValid() ? static_cast<tree*>(parent.internalPointer()) : data_tree;
	return parentItem->get_num_subnode();
}

int SkobkaTreeModel::columnCount(const QModelIndex &parent) const
{
	return 2;
}

static int index_of(tree *parent, tree *child)
{
	if (parent == nullptr) {
		return -1;
	}
	for (int i = 0; i < parent->get_num_subnode(); i++) {
		if (parent->get_subnode(i) == child) {
			return i;
		}
	}
	return -1;
}

QModelIndex SkobkaTreeModel::index(int row, int column, const QModelIndex &parent) const
{
	if (!hasIndex(row, column, parent)) {
		return QModelIndex();
	}

	tree *parentItem = parent.isValid() ? static_cast<tree*>(parent.internalPointer()) : data_tree;
	tree *childItem = parentItem->get_subnode(row);
	if (childItem == nullptr) {
		return QModelIndex();
	}
	return QAbstractItemModel::createIndex(row, column, childItem);
}

QModelIndex SkobkaTreeModel::parent(const QModelIndex &child) const
{
	if (child.isValid()) {
		tree *childItem = static_cast<tree*>(child.internalPointer());
		tree *parentItem = childItem->get_parent();
		if (parentItem != nullptr && parentItem != data_tree) {
			int parent_index = index_of(parentItem->get_parent(), parentItem);
			return createIndex(parent_index, 0, parentItem);
		}
	}
	return QModelIndex();
}

QVariant SkobkaTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}
	if (role == Qt::DisplayRole) {
		tree *item = static_cast<tree*>(index.internalPointer());
		switch (index.column()) {
		case 0: {
			if (item->get_type() == node_type::nd_list) {
				QString result = QString(tr("{Элементов: %1}")).arg(item->get_num_subnode());
				return result;
			}
			std::string presentation;
			item->outtext(presentation);
			return QString::fromStdString(presentation);
		}
		case 1:
			return QString::fromStdString(item->path());
		}
	}

	return QVariant();
}

QVariant SkobkaTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (section) {
		case 0:
			return tr("Значение");
		case 1:
			return tr("Путь");
		}
	}
	return QVariant();
}
