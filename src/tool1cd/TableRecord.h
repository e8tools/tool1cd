//
// Created by dmpas on 20.01.18.
//

#ifndef TOOL1CD_PROJECT_TABLERECORD_H
#define TOOL1CD_PROJECT_TABLERECORD_H

#include <vector>
#include <System.Classes.hpp>

class Table;
class Field;

class TableRecord {
public:

	// owns data
	TableRecord(const Table *parent, char *data, int data_size);

	String get_string(const Field *field) const;
	String get_string(const String &field_name) const;

	~TableRecord();

private:
	const Table *table;
	char *data;
	int data_size;
};


#endif //TOOL1CD_PROJECT_TABLERECORD_H
