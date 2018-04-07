#include "skobka_tree_model.h"
#include <QBuffer>
#include "stream_device.h"

const int PATH_COLUMN = 0;
const int DATA_COLUMN = 1;

SkobkaTreeModel::SkobkaTreeModel(unique_ptr<Tree> data_tree)
	: data_tree(std::move(data_tree))
{

}

int SkobkaTreeModel::rowCount(const QModelIndex &parent) const
{
	if (parent.column() > 0) {
		return 0;
	}

	int result = 0;

	if (parent.isValid())
	{
		Tree *parentItem = static_cast<Tree*>(parent.internalPointer());
		result = parentItem->get_num_subnode();
	}
	else
	{
		result = data_tree->get_num_subnode();
	}

	return result;
}

int SkobkaTreeModel::columnCount(const QModelIndex &parent) const
{
	return 2;
}

static int index_of(Tree *parent, Tree *child)
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

	Tree * childItem = nullptr;
	if(parent.isValid())
	{
		Tree *parentItem = static_cast<Tree*>(parent.internalPointer());
		childItem = parentItem->get_subnode(row);
	}
	else
	{
		childItem = data_tree->get_subnode(row);
	}

	if (childItem == nullptr) {
		return QModelIndex();
	}
	return QAbstractItemModel::createIndex(row, column, childItem);
}

QModelIndex SkobkaTreeModel::parent(const QModelIndex &child) const
{
	if (child.isValid()) {
		Tree *childItem = static_cast<Tree*>(child.internalPointer());
		Tree *parentItem = childItem->get_parent();
		if (parentItem != nullptr && parentItem != data_tree.get()) {
			int parent_index = index_of(parentItem->get_parent(), parentItem);
			return createIndex(parent_index, 0, parentItem);
		}
	}
	return QModelIndex();
}

QString trim_item_presentation(const QString &full_presentation)
{
	const int MAX_PRESENTATION_LEN = 40;
	QString result = full_presentation;
	{
		int k = result.indexOf('\n');
		if (k != -1) {
			result = result.left(k - 1);
		}
	}
	if (result.size() > MAX_PRESENTATION_LEN) {
		result = result.left(MAX_PRESENTATION_LEN - 3) + QString("...");
	}
	return result;
}

QVariant SkobkaTreeModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid()) {
		return QVariant();
	}

	Tree *item = static_cast<Tree*>(index.internalPointer());
	if (role == Qt::DisplayRole
	        || role == Qt::EditRole
	        || role == Qt::ToolTipRole) {
		switch (index.column()) {
		case DATA_COLUMN: {
			if (item->get_type() == node_type::nd_list) {
				QString result = QString(tr("{Элементов: %1}")).arg(item->get_num_subnode());
				return result;
			}
			std::string presentation;
			item->outtext(presentation);
			QString qp = QString::fromStdString(presentation);
			if (role == Qt::DisplayRole) {
				return trim_item_presentation(qp);
			}

			if (role == Qt::EditRole) {
				if (qp.startsWith("#base64:")) {
					qp = qp.right(qp.size() - QString("#base64:").size());
					auto byteArray = QByteArray::fromBase64(qp.toUtf8());
					TMemoryStream *mems = new TMemoryStream();
					mems->WriteBuffer(byteArray.data(), byteArray.size());
					auto device = new StreamDevice(mems);

					return QVariant::fromValue(device);
				}
			}

			return qp;
		}
		case PATH_COLUMN:
			return QString::fromStdString(item->path());
		}
	}

	return QVariant();
}

QVariant SkobkaTreeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		switch (section) {
		case DATA_COLUMN:
			return tr("Значение");
		case PATH_COLUMN:
			return tr("Путь");
		}
	}
	return QVariant();
}
