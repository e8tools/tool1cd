/*
    Tool1CD library provides access to 1CD database files.
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of Tool1CD Library.

    Tool1CD Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tool1CD Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Tool1CD Library.  If not, see <http://www.gnu.org/licenses/>.
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
	static Field *field_from_tree(Tree *field_tree, bool &has_version, Table *parent);

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
	type_fields get_type() const;
	Table* get_parent() const;
	bool get_null_exists() const;
	int32_t get_length() const;
	int32_t get_precision() const;
	std::string get_presentation_type() const;
	bool get_case_sensitive() const;
	bool save_blob_to_file(const TableRecord *rec, const std::string &filename, bool unpack) const;
	uint32_t get_sort_key(const char* rec, unsigned char* SortKey, int32_t maxlen) const;

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
};

#endif /* SRC_CTOOL1CD_FIELD_H_ */
