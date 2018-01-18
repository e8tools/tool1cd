//
// Created by dmpas on 07.01.18.
//

#ifndef TOOL1CD_PROJECT_FIELDTYPE_H
#define TOOL1CD_PROJECT_FIELDTYPE_H

#include "Constants.h"
#include "Parse_tree.h"

class Table;

struct field_type_declaration {
	type_fields type {type_fields::tf_binary};
	bool null_exists {false};
	int32_t length {0};
	int32_t precision {0};
	bool case_sensitive {false};

	static field_type_declaration parse_tree(tree *field_tree);
};


class FieldType {
public:
	virtual type_fields gettype() const = 0;
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

	virtual bool get_binary_value(char *buf, const String &value) const = 0;

	virtual String get_XML_presentation(
			const char *rec,
			const Table *parent,
			bool ignore_showGUID) const = 0;

	virtual uint32_t getSortKey(
			const char* rec,
			unsigned char* SortKey,
			int32_t maxlen) const = 0;


	static FieldType *create_type_manager(const field_type_declaration &type_declaration);
	static FieldType *Version8();

	// TODO: убрать это куда-нибудь
	static bool showGUIDasMS; // Признак, что GUID надо преобразовывать по стилю MS (иначе по стилю 1С)
	static bool showGUID;
};


#endif //TOOL1CD_PROJECT_FIELDTYPE_H
