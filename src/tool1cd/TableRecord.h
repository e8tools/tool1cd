//
// Created by dmpas on 20.01.18.
//

#ifndef TOOL1CD_PROJECT_TABLERECORD_H
#define TOOL1CD_PROJECT_TABLERECORD_H

#include <vector>
#include <System.Classes.hpp>
#include "DetailedException.h"
#include "BinaryGuid.h"

class Table;
class Field;

class NullValueException : public DetailedException {
public:
	NullValueException(const Field *field);
};

class FieldCannotBeNullException : public DetailedException {
public:
	FieldCannotBeNullException(const Field *field);
};

class TableRecord {
public:

	// owns data
	explicit TableRecord(const Table *parent, char *data = nullptr, int data_size = -1);
	TableRecord(const TableRecord &another);

	String get_string(const Field *field) const;
	String get_string(const String &field_name) const;

	String get_xml_string(const Field *field) const;
	String get_xml_string(const String &field_name) const;

	bool is_null_value(const Field *field) const;
	bool is_null_value(const String &field_name) const;

	void set_null(const Field *field);
	void set_data(const Field *field, const void *data);

	BinaryGuid get_guid(const Field *field) const;
	BinaryGuid get_guid(const String &field_name) const;

	bool get_bool(const Field *field) const;
	bool get_bool(const String &field_name) const;

	// TODO: get_raw не нужен
	const char *get_raw(const Field *field) const;
	const char *get_raw(const String &field_name) const;

	// TODO: get_data не нужен
	// в случае null_exists get_data вернёт get_raw + 1, иначе get_raw
	const char *get_data(const Field *field) const;
	const char *get_data(const String &field_name) const;

	bool is_removed() const;

	void Assign(const TableRecord *another_record);

	const Table *get_table() const { return table; }

	bool try_store_blob_data(const Field *field, TStream* &out, bool inflate_stream = false) const;

	~TableRecord();

private:
	char *data;
	const Table *table;
	int data_size;
};


#endif //TOOL1CD_PROJECT_TABLERECORD_H
