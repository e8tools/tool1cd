//
// Created by dmpas on 07.01.18.
//

#ifndef TOOL1CD_PROJECT_FIELDTYPE_H
#define TOOL1CD_PROJECT_FIELDTYPE_H

#include "Constants.h"
#include "Parse_tree.h"

class Table;

class FieldType {
public:
	virtual type_fields gettype() const = 0;
	virtual bool getnull_exists() const = 0;
	virtual int32_t getlength() const = 0;
	virtual int32_t getlen() const = 0;
	virtual int32_t getprecision() const = 0;
	virtual bool getcase_sensitive() const = 0;
	virtual String get_presentation_type() const = 0;

	virtual String get_presentation(
			const char* rec,
			bool EmptyNull,
			wchar_t Delimiter,
			bool ignore_showGUID,
			bool detailed) const = 0;

	virtual bool get_binary_value(
			char *buf,
			bool null,
			const String &value) const = 0;

	virtual String get_XML_presentation(
			char* rec,
			Table *parent,
			bool ignore_showGUID) const = 0;

	virtual uint32_t getSortKey(
			const char* rec,
			unsigned char* SortKey,
			int32_t maxlen) const = 0;


	static FieldType *parse_field_type(tree *field_tree);
	static FieldType *Version8();

	// TODO: убрать это куда-нибудь
	static bool showGUIDasMS; // Признак, что GUID надо преобразовывать по стилю MS (иначе по стилю 1С)
	static bool showGUID;
};


#endif //TOOL1CD_PROJECT_FIELDTYPE_H
