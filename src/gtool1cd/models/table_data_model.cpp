#include "table_data_model.h"
#include <QFont>
#include "stream_device.h"

TableDataModel::TableDataModel(Table *table, Index *index)
    : table(table), _index(index) {}

int TableDataModel::rowCount(const QModelIndex &parent) const
{
	if (_index == nullptr) {
		return table->get_phys_numrecords();
	}
	return _index->get_numrecords();
}

int TableDataModel::columnCount(const QModelIndex &parent) const
{
	return table->get_numfields();
}

QModelIndex TableDataModel::index(int row, int column, const QModelIndex &parent) const
{
	return QAbstractItemModel::createIndex(row, column);
}

QModelIndex TableDataModel::parent(const QModelIndex &child) const
{
	return QModelIndex();
}


QVariant TableDataModel::data(const QModelIndex &index, int role) const
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
		return QString::fromStdString(record->get_string(f));
	}
	if (role == Qt::EditRole) {
		Field *f = table->getfield(index.column());
		if (record->is_null_value(f)) {
			return QString("");
		}

		bool is_blob = false;
		if (f->gettype() == type_fields::tf_string) {
			is_blob = true;
		}
		if (f->gettype() == type_fields::tf_text) {
			is_blob = true;
		}
		if (f->gettype() == type_fields::tf_image) {
			is_blob = true;
		}

		if (is_blob) {
			TStream *data_stream;

			if (!record->try_store_blob_data(f, data_stream, true)) {
				return QString(tr("{Error getting BLOB}"));
			}

			V8Catalog cat(data_stream, true, false);
			if (cat.isOpen() && cat.IsCatalog()) {
				QString result("{catalog}");
			}
			TMemoryStream *mem = new TMemoryStream;
			mem->CopyFrom(data_stream, 0);
			if (f->gettype() == type_fields::tf_image) {
				return QString::fromStdString(TEncoding::UTF8->toUtf8(mem->GetBytes()));
			} else {
				return QString::fromStdString(TEncoding::Unicode->toUtf8(mem->GetBytes()));
			}
			delete mem;
			delete data_stream;
		}

		return QString::fromStdString(record->get_string(f));
	}
	if (role == Qt::FontRole) {
		if (record->is_removed()) {
			QFont italic;
			italic.setItalic(true);
			return italic;
		}
	}
	if (role == Qt::TextAlignmentRole) {
		Field *f = table->getfield(index.column());
		if (f->gettype() == type_fields::tf_numeric) {
			return Qt::AlignRight + Qt::AlignVCenter;
		}
	}

	return QVariant();
}

QVariant TableDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		Field *f = table->getfield(section);
		return QString::fromStdString(f->get_name());
	}
	return QVariant();
}

void TableDataModel::dumpBlob(const QModelIndex &index, const QString &filename) const
{
	Field *f = table->getfield(index.column());
	TableRecord *record = _index == nullptr
	        ? table->getrecord(index.row())
	        : table->getrecord(_index->get_numrec(index.row()));

	TStream *out;
	if (!record->try_store_blob_data(f, out, true)) {
		// TODO: ругнуться
		return;
	}
	TFileStream fs(filename.toStdString(), fmCreate);
	fs.CopyFrom(out, 0);
	delete out;
}

bool TableDataModel::isBlobValue(const QModelIndex &index) const
{
	Field *f = table->getfield(index.column());
	return f->gettype() == type_fields::tf_image;
}

bool TableDataModel::isClobValue(const QModelIndex &index) const
{
	Field *f = table->getfield(index.column());
	return f->gettype() == type_fields::tf_string
	        || f->gettype() == type_fields::tf_text;
}

V8Catalog *TableDataModel::getCatalog(const QModelIndex &index) const
{
	Field *f = table->getfield(index.column());
	TableRecord *record = _index == nullptr
	        ? table->getrecord(index.row())
	        : table->getrecord(_index->get_numrec(index.row()));

	TStream *data_stream;

	if (!record->try_store_blob_data(f, data_stream, true)) {
		if (!record->try_store_blob_data(f, data_stream, false)){
			return nullptr;
		}
	}

	{
		auto *cat = new V8Catalog(data_stream, false, false);
		if (cat->isOpen() && cat->IsCatalog()) {
			return cat;
		}
		delete cat;
	}

	return nullptr;
}

const TableRecord *TableDataModel::getRecord(const QModelIndex &index) const
{
	return _index == nullptr
	        ? table->getrecord(index.row())
	        : table->getrecord(_index->get_numrec(index.row()));

}

QIODevice *TableDataModel::getBlobStream(const QModelIndex &index) const
{
	Field *f = table->getfield(index.column());
	TableRecord *record = _index == nullptr
	        ? table->getrecord(index.row())
	        : table->getrecord(_index->get_numrec(index.row()));

	TStream *out;
	if (!record->try_store_blob_data(f, out, true)) {
		return nullptr;
	}
	return new StreamDevice(out);
}
