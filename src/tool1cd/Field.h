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

class Table;
class T_1CD;

class Field
{
friend Table;
friend T_1CD;
public:
	static bool showGUID;

	Field(Table* _parent);

	int32_t getlen() const; // возвращает длину поля в байтах
	String getname() const;

	String get_presentation(
			const char* rec,
			bool EmptyNull = false,
			wchar_t Delimiter = 0,
			bool ignore_showGUID = false,
			bool detailed = false) const;

	String get_XML_presentation(char* rec, bool ignore_showGUID = false) const;

	bool get_bynary_value(char* buf, bool null, String& value) const;
	type_fields gettype() const;
	Table* getparent() const;
	bool getnull_exists() const;
	int32_t getlength() const;
	int32_t getprecision() const;
	bool getcase_sensitive() const;
	int32_t getoffset() const;
	String get_presentation_type() const;
	bool save_blob_to_file(char* rec, String filename, bool unpack) const;
	uint32_t getSortKey(const char* rec, unsigned char* SortKey, int32_t maxlen) const;
private:
	String name;
	type_fields type;
	bool null_exists;
	int32_t length;
	int32_t precision;
	bool case_sensitive;

	Table* parent;
	mutable int32_t len; // длина поля в байтах
	int32_t offset; // смещение поля в записи
	static bool showGUIDasMS; // Признак, что GUID надо преобразовывать по стилю MS (иначе по стилю 1С)
	static char buf[];
	static char null_index[];
	static bool null_index_initialized;
};

#endif /* SRC_CTOOL1CD_FIELD_H_ */
