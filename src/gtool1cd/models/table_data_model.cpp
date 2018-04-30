/*
    GTool1CD provides GUI front end to Tool1CD library
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of GTool1CD.

    GTool1CD is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GTool1CD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GTool1CD.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "table_data_model.h"
#include <QFont>
#include "stream_device.h"
#include <TempStream.h>
#include <UZLib.h>
#include <QDebug>
#include "SystemClasses/String.hpp"

using namespace System;

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
	return table->get_num_fields();
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
	        ? table->get_record(index.row())
	        : table->get_record(_index->get_numrec(index.row()));
	if (role == Qt::DisplayRole) {
		Field *f = table->get_field(index.column());
		if (record->is_null_value(f)) {
			return QString("{NULL}");
		}
		return QString::fromStdString(record->get_string(f));
	}
	if (role == Qt::EditRole) {
		Field *f = table->get_field(index.column());
		if (record->is_null_value(f)) {
			return QString("");
		}

		bool is_blob = false;
		if (f->get_type() == type_fields::tf_string) {
			is_blob = true;
		}
		if (f->get_type() == type_fields::tf_text) {
			is_blob = true;
		}
		if (f->get_type() == type_fields::tf_image) {
			is_blob = true;
		}

		if (is_blob) {
			TStream *data_stream;

			if (!record->try_store_blob_data(f, data_stream, true)) {
				return QString(tr("{Error getting BLOB}"));
			}

			V8Catalog cat(data_stream, true, false);
			if (cat.isOpen() && cat.is_catalog()) {
				QString result("{catalog}");
			}
			TMemoryStream *mem = new TMemoryStream;
			mem->CopyFrom(data_stream, 0);
			if (f->get_type() == type_fields::tf_image) {
				return QString::fromStdString(TEncoding::UTF8->toUtf8(mem->GetBytes()));
			} else {
				return QString::fromStdString(TEncoding::Unicode->toUtf8(mem->GetBytes()));
			}
			delete mem;
			delete data_stream;
		}

		if (f->get_type() == type_fields::tf_binary
		        && f->get_length() == 16
		        && EndsWithIC(f->get_name(),"REF")) {
			auto value = record->get<BinaryGuid>(f);
			return QString::fromStdString(value.as_1C());
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
		Field *f = table->get_field(index.column());
		if (f->get_type() == type_fields::tf_numeric) {
			return Qt::AlignRight + Qt::AlignVCenter;
		}
	}

	return QVariant();
}

QVariant TableDataModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
		Field *f = table->get_field(section);
		return QString::fromStdString(f->get_name());
	}
	return QVariant();
}

void TableDataModel::dumpBlob(const QModelIndex &index, const QString &filename) const
{
	Field *f = table->get_field(index.column());
	TableRecord *record = _index == nullptr
	        ? table->get_record(index.row())
	        : table->get_record(_index->get_numrec(index.row()));

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
	Field *f = table->get_field(index.column());
	return f->get_type() == type_fields::tf_image;
}

bool TableDataModel::isClobValue(const QModelIndex &index) const
{
	Field *f = table->get_field(index.column());
	return f->get_type() == type_fields::tf_string
	        || f->get_type() == type_fields::tf_text;
}

TStream *new_stream(int64_t needed_size)
{
	const int threshold = 10 * 1024 * 1024;
	if (needed_size < threshold) {
		return new TMemoryStream();
	}
	return new TTempStream();
}

int inflate_stream(TStream* &stream)
{
	int counter = 0;
	while (true) {
		TStream *unpacked_stream = new_stream(stream->GetSize());
		stream->SetPosition(0);
		try {
			ZInflateStream(stream, unpacked_stream);
		} catch (ZError &) {
			return counter;
		}
		stream = unpacked_stream;
		if (counter++ > 10) {
			return counter;
		}
	}
}

V8Catalog *TableDataModel::getCatalog(const QModelIndex &index) const
{
	TStream *data_stream = getBlobStream(index);

	auto *cat = new V8Catalog(data_stream, false, false);
	if (cat->isOpen() && cat->is_catalog()) {
		return cat;
	}
	delete cat;

	return nullptr;
}

const TableRecord *TableDataModel::getRecord(const QModelIndex &index) const
{
	return _index == nullptr
	        ? table->get_record(index.row())
	        : table->get_record(_index->get_numrec(index.row()));

}

TStream *TableDataModel::getBlobStream(const QModelIndex &index) const
{
	Field *f = table->get_field(index.column());
	TableRecord *record = _index == nullptr
	        ? table->get_record(index.row())
	        : table->get_record(_index->get_numrec(index.row()));

	TStream *out;
	if (!record->try_store_blob_data(f, out, false)) {
		return nullptr;
	}

	int count = inflate_stream(out);
	qDebug() << "Deflate count: " << count;

	return out;
}
