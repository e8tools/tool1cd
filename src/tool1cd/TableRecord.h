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

#ifndef TOOL1CD_PROJECT_TABLERECORD_H
#define TOOL1CD_PROJECT_TABLERECORD_H

#include <vector>
#include "SystemClasses/TStream.hpp"
#include "DetailedException.h"
#include "BinaryGuid.h"

class Field;
class Table;

class NullValueException : public DetailedException {
public:
	NullValueException(const Field *field);
};

class FieldCannotBeNullException : public DetailedException {
public:
	FieldCannotBeNullException(const Field *field);
};

namespace RecordConverters {

	template <typename T>
	void convert(const char *data, T &result)
	{
		result = *(reinterpret_cast<const T*>(data));
	}

	void convert(const char *data, BinaryGuid &result);

	template <typename T>
	void put(char *data, const T value)
	{
		*(reinterpret_cast<T*>(data)) = value;
	}
}

class TableRecord {
public:

	// owns data
	explicit TableRecord(const Table *parent, char *data = nullptr, int data_size = -1);
	TableRecord(const TableRecord &another);

	std::string get_string(const Field *field) const;
	std::string get_string(const std::string &field_name) const;

	std::string get_xml_string(const Field *field) const;
	std::string get_xml_string(const std::string &field_name) const;

	bool is_null_value(const Field *field) const;
	bool is_null_value(const std::string &field_name) const;

	void set_null(const Field *field);
	void set_data(const Field *field, const void *data);

	template <typename T>
	T get (const Field *f, const T default_value = T()) const
	{
		if (is_null_value(f)) {
			return default_value;
		}

		T result;
		RecordConverters::convert(get_data(f), result);
		return result;
	}

	template <typename T>
	T get(const std::string &field_name, const T default_value = T()) const
	{
		return get(get_field(field_name), default_value);
	}

	template <typename T>
	void set(const Field *f, const T value)
	{
		RecordConverters::put(__get_data(f), value);
	}

	template <typename T>
	void set(const std::string &field_name, const T value)
	{
		set(get_field(field_name), value);
	}

	const char *get_raw(const Field *field) const;
	const char *get_raw(const std::string &field_name) const;

	// в случае null_exists get_data вернёт get_raw + 1, иначе get_raw
	const char *get_data(const Field *field) const;
	const char *get_data(const std::string &field_name) const;

	bool is_removed() const;

	void Assign(const TableRecord *another_record);

	const Table *get_table() const { return table; }

	bool try_store_blob_data(const Field *field, TStream* &out, bool inflate_stream = false) const;

	~TableRecord();

private:
	const Field *get_field(const std::string &field_name) const;
	char *__get_data(const Field *field);

	char *data;
	const Table *table;
	int data_size;
};


#endif //TOOL1CD_PROJECT_TABLERECORD_H
