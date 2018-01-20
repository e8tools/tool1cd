//
// Created by dmpas on 20.01.18.
//

#include "TableRecord.h"
#include "Field.h"
#include "Table.h"

TableRecord::TableRecord(const Table *parent, char *data, int data_size)
	: table(parent), data(data), data_size(data_size)
{

}

TableRecord::~TableRecord()
{
	if (data) {
		delete[] data;
		data = nullptr;
	}
}

String TableRecord::get_string(const Field *field) const
{

}

String TableRecord::get_string(const String &field_name) const
{
	return get_string(table->get_field(field_name));
}
