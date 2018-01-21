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

class TableRecord {
public:

	// owns data
	TableRecord(const Table *parent, char *data, int data_size);

	String get_string(const Field *field) const;
	String get_string(const String &field_name) const;

	bool is_null_value(const Field *field) const;
	bool is_null_value(const String &field_name) const;

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

	~TableRecord();

private:
	const Table *table;
	char *data;
	int data_size;
};


#endif //TOOL1CD_PROJECT_TABLERECORD_H
