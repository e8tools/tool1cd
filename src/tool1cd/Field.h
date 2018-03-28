/*
 * Field.h
 *
 *  Created on: 24 июн. 2017 г.
 *      Author: andreevlex
 */

#ifndef SRC_CTOOL1CD_FIELD_H_
#define SRC_CTOOL1CD_FIELD_H_

#include "Common.h"
#include "Class_1CD.h"
#include "Constants.h"
#include "DetailedException.h"
#include "FieldType.h"

class Table;
class T_1CD;
class TableRecord;

class Field
{
public:

	explicit Field(Table* _parent);

	int32_t get_size() const; // возвращает длину поля в байтах

	std::string get_name() const;
	void set_name(const std::string &value);

	std::string get_presentation(
			const char *rec,
			bool EmptyNull = false,
			wchar_t Delimiter = 0,
			bool ignore_showGUID = false,
			bool detailed = false) const;

	std::string get_XML_presentation(const char *rec, bool ignore_showGUID = false) const;

	bool get_binary_value(char *buf, bool null, const std::string &value) const;
	type_fields gettype() const;
	Table* getparent() const;
	bool getnull_exists() const;
	int32_t getlength() const;
	int32_t getprecision() const;
	bool getcase_sensitive() const;
	std::string get_presentation_type() const;
	bool save_blob_to_file(const TableRecord *rec, const std::string &filename, bool unpack) const;
	uint32_t getSortKey(const char* rec, unsigned char* SortKey, int32_t maxlen) const;

	FieldType *get_type_manager() const;
	void set_type_manager(FieldType *value);

	int32_t get_offset() const;
	void set_offset(const int32_t value);
private:
	std::string name;
	type_fields type;
	bool null_exists {false};
	FieldType *type_manager;

	Table* parent {nullptr};
	int32_t offset {0}; // смещение поля в записи
	static char buf[];
	static char null_index[];
	static bool null_index_initialized;

public:
	static Field *field_from_tree(tree *field_tree, bool &has_version, Table *parent);
};

#endif /* SRC_CTOOL1CD_FIELD_H_ */
