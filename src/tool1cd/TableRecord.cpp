//
// Created by dmpas on 20.01.18.
//

#include "TableRecord.h"
#include "Field.h"
#include "Table.h"
#include "TempStream.h"
#include "UZLib.h"

NullValueException::NullValueException(const Field *field)
	: DetailedException("Запрошенное значение равно NULL")
{
	add_detail("Поле", field->getname());
	add_detail("Таблица", field->getparent()->getname());
}

FieldCannotBeNullException::FieldCannotBeNullException(const Field *field)
	: DetailedException("Поле не может быть NULL")
{
	add_detail("Поле", field->getname());
	add_detail("Таблица", field->getparent()->getname());
}

TableRecord::TableRecord(const Table *parent, char *data, int data_size)
	: data(data == nullptr ? new char [parent->get_recordlen()] : data),
	  table(parent),
	  data_size(data_size == -1 ? parent->get_recordlen() : data_size)
{

}

TableRecord::TableRecord(const TableRecord &another)
: data(new char [another.get_table()->get_recordlen()]),
  table(another.get_table()),
  data_size(another.data_size)
{
	auto i = data_size;
	while (i--) {
		data[i] = another.data[i];
	}
}

TableRecord::~TableRecord()
{
	if (data) {
		// delete[] data; // TODO: утечка данных
		data = nullptr;
	}
}

String TableRecord::get_string(const Field *field) const
{
	if (is_null_value(field)) {
		throw NullValueException(field);
	}
	return field->get_presentation(data);
}

String TableRecord::get_string(const String &field_name) const
{
	return get_string(table->get_field(field_name));
}

bool TableRecord::is_null_value(const Field *field) const
{
	if (!field->getnull_exists()) {
		return false;
	}
	return data[field->getoffset()] == '\0';
}

bool TableRecord::is_null_value(const String &field_name) const
{
	return is_null_value(table->get_field(field_name));
}

bool TableRecord::is_removed() const
{
	return data[0] != '\0';
}

BinaryGuid TableRecord::get_guid(const Field *field) const
{
	if (is_null_value(field)) {
		throw NullValueException(field);
	}
	const char *field_raw_data = &data[field->getoffset()];
	if (field->getnull_exists()) {
		field_raw_data++;
	}
	return BinaryGuid(field_raw_data);
}

BinaryGuid TableRecord::get_guid(const String &field_name) const
{
	return get_guid(table->get_field(field_name));
}

const char *TableRecord::get_raw(const Field *field) const
{
	return &data[field->getoffset()];
}

const char *TableRecord::get_raw(const String &field_name) const
{
	return get_raw(table->get_field(field_name));
}


const char *TableRecord::get_data(const Field *field) const
{
	return &data[field->getoffset() + (field->getnull_exists() ? 1 : 0)];
}

const char *TableRecord::get_data(const String &field_name) const
{
	return get_data(table->get_field(field_name));
}

bool TableRecord::get_bool(const Field *field) const
{
	if (is_null_value(field)) {
		throw NullValueException(field);
	}
	const char *field_raw_data = &data[field->getoffset()];
	if (field->getnull_exists()) {
		field_raw_data++;
	}
	return *field_raw_data != '\0';
}

bool TableRecord::get_bool(const String &field_name) const
{
	return get_bool(table->get_field(field_name));
}

void TableRecord::Assign(const TableRecord *another_record)
{
	if (table != nullptr) {
		if (another_record->data_size != data_size || another_record->table != table) {
			throw DetailedException("Попытка передать данные между записями разных таблиц!")
					.add_detail("Таблица-приёмник", table->getname())
					.add_detail("Таблица-источник", another_record->table->getname());
		}
	}
	auto i = data_size;
	while (i--) {
		data[i] = another_record->data[i];
	}
}

void TableRecord::set_null(const Field *field)
{
	if (!field->getnull_exists()) {
		throw FieldCannotBeNullException(field);
	}
	data[field->getoffset()] = '\0';
}

void TableRecord::set_data(const Field *field, const void *new_data)
{
	char *data_start = &data[field->getoffset()];
	if (field->getnull_exists()) {
		data_start[0] = '\001';
		data_start++;
	}
	memcpy(data_start, new_data, field->getlen());
}

String TableRecord::get_xml_string(const Field *field) const
{
	if (is_null_value(field)) {
		throw NullValueException(field);
	}
	return field->get_XML_presentation(data);
}

String TableRecord::get_xml_string(const String &field_name) const
{
	return get_xml_string(table->get_field(field_name));
}

bool TableRecord::try_store_blob_data(const Field *field, TStream *&out, bool inflate_stream) const
{
	if (is_null_value(field)) {
		return false;
	}
	auto *b = (const table_blob_file *)get_data(field);
	if (b->blob_start == 0 && b->blob_length == 0) {
		return false;
	}

	out = new TTempStream;
	if (inflate_stream) {
		TMemoryStream in;
		table->readBlob(&in, b->blob_start, b->blob_length);
		in.Seek(0, soFromBeginning);
		ZInflateStream(&in, out);
	}
	else {
		table->readBlob(out, b->blob_start, b->blob_length);
	}
	out->Close();
	return true;
}
