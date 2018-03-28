//
// Created by dmpas on 07.01.18.
//

#include "FieldType.h"
#include "DetailedException.h"
#include "Common.h"
#include "Base64.h"
#include "Table.h"
#include "BinaryDecimalNumber.h"
#include <vector>

using namespace std;

// TODO: инициализация null_index
static char null_index[4096];
bool FieldType::showGUIDasMS = false;
bool FieldType::showGUID = false;


class CommonFieldType : public FieldType
{
public:

	CommonFieldType(field_type_declaration declaration)
			: type(declaration.type), length(declaration.length), precision(declaration.precision),
			  case_sensitive(declaration.case_sensitive)
	{}

	virtual type_fields get_type() const override
	{
		return type;
	}

	virtual int32_t get_length() const override
	{
		return length;
	}

	virtual int32_t get_precision() const override
	{
		return precision;
	}

	virtual bool get_case_sensitive() const override
	{
		return case_sensitive;
	}

	virtual string get_presentation_type() const override
	{
		switch(type)
		{
			case type_fields::tf_binary: return "binary";
			case type_fields::tf_bool: return "bool";
			case type_fields::tf_numeric: return "number";
			case type_fields::tf_char: return "fixed string";
			case type_fields::tf_varchar: return "string";
			case type_fields::tf_version: return "version";
			case type_fields::tf_string: return "memo";
			case type_fields::tf_text: return "text";
			case type_fields::tf_image: return "image";
			case type_fields::tf_datetime: return "datetime";
			case type_fields::tf_version8: return "hidden version";
			case type_fields::tf_varbinary: return "var binary";
		}
		return "{?}";
	}
	//---------------------------------------------------------------------------
	virtual int32_t get_size() const override
	{
		if(len) return len;

		switch(type)
		{
			case type_fields::tf_binary: len += length; break;
			case type_fields::tf_bool: len += 1; break;
			case type_fields::tf_numeric: len += (length + 2) >> 1; break;
			case type_fields::tf_char: len += length * 2; break;
			case type_fields::tf_varchar: len += length * 2 + 2; break;
			case type_fields::tf_version: len += 16; break;
			case type_fields::tf_string: len += 8; break;
			case type_fields::tf_text: len += 8; break;
			case type_fields::tf_image: len += 8; break;
			case type_fields::tf_datetime: len += 7; break;
			case type_fields::tf_version8: len += 8; break;
			case type_fields::tf_varbinary: len += length + 2; break;
		}
		return len;
	}

	virtual string get_presentation(
			const char *rec,
			bool EmptyNull,
			wchar_t Delimiter,
			bool ignore_showGUID,
			bool detailed) const override;

	virtual string get_fast_presentation(
			const char *rec) const;

	virtual bool get_binary_value(char *buf, const string &value) const override;

	virtual string get_XML_presentation(
			const char *rec,
			const Table *parent,
			bool ignore_showGUID) const override;

	virtual uint32_t get_sort_key(
			const char* rec,
			unsigned char* SortKey,
			int32_t maxlen) const override;

	type_fields type {type_fields::tf_binary};
	int32_t length {0};
	int32_t precision {0};
	bool case_sensitive {false};

	mutable int32_t len {0};
};

string CommonFieldType::get_fast_presentation(const char *rec) const
{
	return get_presentation(rec, false, 0, false, false);
}


class BinaryFieldType : public CommonFieldType
{
public:
	BinaryFieldType(field_type_declaration declaration)
		: CommonFieldType(declaration)
	{
	}

	virtual string get_presentation(
			const char *rec,
			bool EmptyNull,
			wchar_t Delimiter,
			bool ignore_showGUID,
			bool detailed) const override;

	virtual bool get_binary_value(
			char *buf,
			const string &value) const override;

	virtual string get_XML_presentation(
			const char *rec,
			const Table *parent,
			bool ignore_showGUID) const override;

	virtual uint32_t get_sort_key(
			const char* rec,
			unsigned char* SortKey,
			int32_t maxlen) const override;
};


class NumericFieldType : public CommonFieldType
{
public:
	NumericFieldType(field_type_declaration declaration)
		: CommonFieldType(declaration)
	{
	}

	virtual string get_presentation(
			const char* rec,
			bool EmptyNull,
			wchar_t Delimiter,
			bool ignore_showGUID,
			bool detailed) const override;

	virtual bool get_binary_value(
			char *buf,
			const string &value) const override;

	virtual string get_XML_presentation(
			const char *rec,
			const Table *parent,
			bool ignore_showGUID) const override;

	virtual uint32_t get_sort_key(
			const char* rec,
			unsigned char* SortKey,
			int32_t maxlen) const override;
};

class DatetimeFieldType : public CommonFieldType
{
public:
	DatetimeFieldType(field_type_declaration declaration)
	: CommonFieldType(declaration)
			{
			}

	virtual string get_presentation(
			const char* rec,
			bool EmptyNull,
			wchar_t Delimiter,
			bool ignore_showGUID,
			bool detailed) const override;

	virtual bool get_binary_value(
			char* buf,
			const string& value) const override;

	virtual string get_XML_presentation(
			const char *rec,
			const Table *parent,
			bool ignore_showGUID) const override;

	virtual uint32_t get_sort_key(
			const char* rec,
			unsigned char* SortKey,
			int32_t maxlen) const override;
};

bool BinaryFieldType::get_binary_value(char *binary_value, const string &value) const
{
	unsigned char* fr = (unsigned char*)binary_value;
	memset(fr, 0, len);

	switch(type)
	{
		case type_fields::tf_binary: {
			if(value.size() == 0) {
				break;
			}
			int32_t j = 1;
			if(length == 16 && showGUID) // TODO Надо доделать для showGUIDasMS
			{
				if(value.size() < GUID_LEN) {
					break;
				}
				for(int32_t ind = 12; ind < 16; ind++) {
					fr[ind] = (from_hex_digit(value[j++]) << 4) + from_hex_digit(value[j++]);
				}
				j++;
				for(int32_t ind = 10; ind < 12; ind++) {
					fr[ind] = (from_hex_digit(value[j++]) << 4) + from_hex_digit(value[j++]);
				}
				j++;
				for(int32_t ind = 8; ind < 10; ind++) {
					fr[ind] = (from_hex_digit(value[j++]) << 4) + from_hex_digit(value[j++]);
				}
				j++;
				for(int32_t ind = 0; ind < 2; ind++) {
					fr[ind] = (from_hex_digit(value[j++]) << 4) + from_hex_digit(value[j++]);
				}
				j++;
				for(int32_t ind = 2; ind < 8; ind++) {
					fr[ind] = (from_hex_digit(value[j++]) << 4) + from_hex_digit(value[j++]);
				}
			}
			else {
				if(value.size() < length * 2) {
					break;
				}
				for(int32_t ind = 0; ind < length; ind++) {
					fr[ind] = (from_hex_digit(value[j++]) << 4) + from_hex_digit(value[j++]);
				}
			}
			break;
		}
		case type_fields::tf_varbinary: {
			return false;
		}
	}

	return true;
}

string BinaryFieldType::get_presentation(const char *rec, bool EmptyNull, wchar_t Delimiter, bool ignore_showGUID,
										 bool detailed) const
{
	char sym;
	int32_t i, m;

	unsigned char* fr = (unsigned char*)rec;

	char *buf = new char[(length + 1) * 2]; // TODO: адовый костыль с утечкой памяти
	switch(type)
	{
		case type_fields::tf_binary:
			if(length == 16 && (showGUID || ignore_showGUID))
			{
				if(showGUIDasMS) return GUIDasMS(fr);
				else return GUIDas1C(fr);
			}
			else
			{
				for(i = 0; i < length; i++)
				{
					sym = '0' + (fr[i] >> 4);
					if(sym > '9') sym += ('a' - '9' - 1);
					buf[i << 1] = sym;
					sym = '0' + (fr[i] & 0xf);
					if(sym > '9') sym += ('a' - '9' - 1);
					buf[(i << 1) + 1] = sym;
				}
				buf[length << 1] = 0;
			}
			return buf;
		case type_fields::tf_varbinary:
			m = *(int16_t*)fr; // длина + смещение
			for(i = 0; i < m; i++)
			{
				sym = '0' + (fr[i + 2] >> 4);
				if(sym > '9') sym += ('a' - '9' - 1);
				buf[i << 1] = sym;
				sym = '0' + (fr[i + 2] & 0xf);
				if(sym > '9') sym += ('a' - '9' - 1);
				buf[(i << 1) + 1] = sym;
			}
			buf[m << 1] = 0;
			return buf;
	}

	return "{?}";
}

string BinaryFieldType::get_XML_presentation(const char *rec, const Table *parent, bool ignore_showGUID) const
{
	char sym;
	int32_t i, m;

	unsigned char* fr = (unsigned char*)rec;

	char *buf = new char[(length + 1)*2]; // TODO: адовый костыль с утечкой памяти
	switch(type)
	{
		case type_fields::tf_binary:
			if(length == 16 && (showGUID || ignore_showGUID))
			{
				if(showGUIDasMS)
					return GUIDasMS(fr);
				else
					return GUIDas1C(fr);
			}
			else
			{
				for(i = 0; i < length; i++)
				{
					sym = '0' + (fr[i] >> 4);
					if(sym > '9') sym += ('a' - '9' - 1);
					buf[i << 1] = sym;
					sym = '0' + (fr[i] & 0xf);
					if(sym > '9') sym += ('a' - '9' - 1);
					buf[(i << 1) + 1] = sym;
				}
				buf[length << 1] = 0;
			}
			return buf;
		case type_fields::tf_varbinary:
			m = *(int16_t*)fr; // длина + смещение
			for(i = 0; i < m; i++)
			{
				sym = '0' + (fr[i + 2] >> 4);
				if(sym > '9') sym += ('a' - '9' - 1);
				buf[i << 1] = sym;
				sym = '0' + (fr[i + 2] & 0xf);
				if(sym > '9') sym += ('a' - '9' - 1);
				buf[(i << 1) + 1] = sym;
			}
			buf[m << 1] = 0;
			return buf;
	}

	return "{?}";
}

uint32_t BinaryFieldType::get_sort_key(const char* rec, unsigned char* SortKey, int32_t maxlen) const
{
	if (!maxlen) {
		throw SerializationException("Ошибка получения ключа сортировки поля. Нулевая длина буфера.")
				.add_detail("Значение поля", get_fast_presentation(rec));
	}

	switch(type)
	{
		case type_fields::tf_binary:
			if (len > maxlen) {
				throw SerializationException("Ошибка получения ключа сортировки поля. Длина буфера меньше необходимой.")
						.add_detail("Значение поля", get_fast_presentation(rec))
						.add_detail("Длина буфера", maxlen)
						.add_detail("Необходимая длина буфера", len);
			}
			memcpy(SortKey, (void*)rec, len);
			return len;

		case type_fields::tf_varbinary:
			throw SerializationException("Попытка получения ключа сортировки неподдерживаемого типа поля.")
					.add_detail("Значение поля", get_fast_presentation(rec));
	}

	return 0;

}


bool NumericFieldType::get_binary_value(char *binary_value, const string &value) const
{
	int32_t l = value.size();
	if(!l) {
		return true;
	}

	BinaryDecimalNumber bdn(value, true, get_length(), get_precision());
	bdn.write_to(binary_value);

	return true;
}

string NumericFieldType::get_presentation(const char* rec, bool EmptyNull, wchar_t Delimiter, bool ignore_showGUID, bool detailed) const
{
	BinaryDecimalNumber bdn(rec, length, precision, true);
	return bdn.get_presentation();
}

string NumericFieldType::get_XML_presentation(const char *rec, const Table *parent, bool ignore_showGUID) const
{
	return get_presentation(rec, false, 0, false, false);
}

uint32_t NumericFieldType::get_sort_key(const char* rec, unsigned char* SortKey, int32_t maxlen) const
{

	int32_t i, j;
	bool k;
	uint32_t addlen = 0;
	unsigned char c;

	unsigned char* fr = (unsigned char *)rec;

	char nbuf[64];

	if (!maxlen) {
		throw SerializationException("Ошибка получения ключа сортировки поля. Нулевая длина буфера.")
				.add_detail("Значение поля", get_fast_presentation(rec));
	}

	memcpy(nbuf, fr, len - addlen);
	if((*nbuf & 0xf0) == 0) //Отрицательное!
	{
		k = false; // перенос
		for(i = length; i > 0; i--)
		{
			j = i >> 1;
			if(i & 1)
			{
				c = nbuf[j] & 0x0f;
				if(k) c = 9 - c;
				else
				{
					if(c)
					{
						k = true;
						c = 10 - c;
					}
				}
				nbuf[j] = (nbuf[j] & 0xf0) | c;
			}
			else
			{
				c = nbuf[j] >> 4 & 0x0f;
				if(k) c = 9 - c;
				else
				{
					if(c)
					{
						k = true;
						c = 10 - c;
					}
				}
				nbuf[j] = (nbuf[j] & 0x0f) | (c << 4);
			}
		}
		if(!k) *nbuf = 0x10; // если значение оказалось -0 (минус ноль), то должно получиться +0.
	}

	if(len > maxlen)
	{
		throw SerializationException("Ошибка получения ключа сортировки поля. Длина буфера меньше необходимой.")
				.add_detail("Значение поля", get_fast_presentation(rec))
				.add_detail("Длина буфера", maxlen)
				.add_detail("Необходимая длина буфера", len);
	}
	memcpy(SortKey, nbuf, len - addlen);
	return len;
}


// Ожидаем дату строго в формате "дд.ММ.гггг чч:мм:сс"
bool DatetimeFieldType::get_binary_value(char* binary_value, const string& value) const
{
	unsigned char* fr = (unsigned char*)binary_value;
	memset(fr, 0, len);

	if (value.size() < 19) {
		fr[1] = 1;
		fr[2] = 1;
		fr[3] = 1;
	} else {
		BinaryDecimalDate bdc(value);
		bdc.write_to(fr);
	}

	return true;
}

string DatetimeFieldType::get_presentation(const char* rec, bool EmptyNull, wchar_t Delimiter, bool ignore_showGUID, bool detailed) const
{
	return date_to_string(rec);
}

std::string s02(const std::string &s)
{
	if (s.size() < 2) {
		string result = "0";
		return result + s;
	}
	return s;
}

string DatetimeFieldType::get_XML_presentation(const char *rec, const Table *parent, bool ignore_showGUID) const
{
	BinaryDecimalDate bdd(rec);
	string result;
	result.append(to_string(bdd.get_year()));
	result.append("-");
	result.append(s02(to_string(bdd.get_month())));
	result.append("-");
	result.append(s02(to_string(bdd.get_day())));
	result.append("T");

	result.append(s02(to_string(bdd.get_hour())));
	result.append(":");
	result.append(s02(to_string(bdd.get_minute())));
	result.append(":");
	result.append(s02(to_string(bdd.get_second())));

	return result;
}

uint32_t DatetimeFieldType::get_sort_key(const char* rec, unsigned char* SortKey, int32_t maxlen) const
{

	uint32_t addlen = 0;

	unsigned char* fr = (unsigned char *)rec;

	if (!maxlen) {
		throw SerializationException("Ошибка получения ключа сортировки поля. Нулевая длина буфера.")
				.add_detail("Значение поля", get_fast_presentation(rec));
	}

	if (len > maxlen) {
		throw SerializationException("Ошибка получения ключа сортировки поля. Длина буфера меньше необходимой.")
				.add_detail("Значение поля", get_fast_presentation(rec))
				.add_detail("Длина буфера", maxlen)
				.add_detail("Необходимая длина буфера", len);
	}
	memcpy(SortKey, (void*)fr, len - addlen);
	return len;

}


bool CommonFieldType::get_binary_value(char *binary_value, const string &value) const
{
	unsigned char* fr = (unsigned char*)binary_value;
	memset(fr, 0, len);

	switch(type)
	{

		case type_fields::tf_bool: {
			if(value == "true") {
				*fr = 1;
			}
			else {
				*fr = 0;
			}
			break;
		}

		case type_fields::tf_char: {
			int32_t i = std::min(value.size(), (size_t)length);
			memcpy(fr, value.c_str(), i << 1);
			while(i < length) {
				((WCHART*)fr)[i++] = L' ';
			}
			break;
		}
		case type_fields::tf_varchar: {
			int32_t i = std::min(value.size(), (size_t)length);
			*(int16_t*)fr = i;
			memcpy(fr + 2, value.c_str(), i * 2);
			while(i < length) {
				((WCHART*)(fr + 2))[i++] = L' ';
			}
			break;
		}
		case type_fields::tf_version: {
			return false;
		}
		case type_fields::tf_version8: {
			return false;
		}
		case type_fields::tf_string: {
			return false;
		}
		case type_fields::tf_text: {
			return false;
		}
		case type_fields::tf_image: {
			return false;
		}

	}

	return true;
}

string CommonFieldType::get_presentation(const char *rec, bool EmptyNull, wchar_t Delimiter, bool ignore_showGUID,
										 bool detailed) const
{
	char sym;
	int32_t i, m;

	auto fr = reinterpret_cast<const uint8_t *>(rec);

	char *buf = new char[(length + 1) * 2]; // TODO: адовый костыль с утечкой памяти
	switch(type)
	{
		case type_fields::tf_bool:
			if(fr[0]) return "true";
			return "false";
		case type_fields::tf_char: {
			return TEncoding::Unicode->toUtf8(vector<uint8_t>(fr, fr + (length * sizeof(WCHART))));
		}
		case type_fields::tf_varchar: {
			int16_t length = *(int16_t *) fr;
			fr += sizeof(length);
			auto full_size = length * sizeof(WCHART);
			if (full_size > get_size()) {
				full_size = get_size();
			}
			return TEncoding::Unicode->toUtf8(vector<uint8_t>(fr, fr + full_size));
		}
		case type_fields::tf_version: {
			auto retyped = reinterpret_cast<const int32_t *>(fr);
			return to_string(retyped[0])
				   + ":" + to_string(retyped[1])
				   + ":" + to_string(retyped[2])
				   + ":" + to_string(retyped[3]);
		}
		case type_fields::tf_version8: {
			auto retyped = reinterpret_cast<const int32_t*>(fr);
			return to_string(retyped[0]) + string(":") + to_string(retyped[1]);
		}
		case type_fields::tf_string:
			return detailed ? string("{MEMO} [") + to_hex_string(*(int32_t*)fr)
							  + string("][") + to_hex_string(*(int32_t*)(fr + 4)) + string("]") : string("{MEMO}");
		case type_fields::tf_text:
			return detailed ? string("{TEXT} [") + to_hex_string(*(int32_t*)fr)
							  + string("][") + to_hex_string(*(int32_t*)(fr + 4)) + string("]") : string("{TEXT}");
		case type_fields::tf_image:
			return detailed ? string("{IMAGE} [") + to_hex_string(*(int32_t*)fr)
							  + string("][") + to_hex_string(*(int32_t*)(fr + 4)) + "]" : string("{IMAGE}");
		case type_fields::tf_varbinary:
			m = *(int16_t*)fr; // длина + смещение
			for(i = 0; i < m; i++)
			{
				sym = '0' + (fr[i + 2] >> 4);
				if(sym > '9') sym += ('a' - '9' - 1);
				buf[i << 1] = sym;
				sym = '0' + (fr[i + 2] & 0xf);
				if(sym > '9') sym += ('a' - '9' - 1);
				buf[(i << 1) + 1] = sym;
			}
			buf[m << 1] = 0;
			return buf;
	}

	return "{?}";
}

string CommonFieldType::get_XML_presentation(const char *rec, const Table *parent, bool ignore_showGUID) const
{
	int32_t i;

	TMemoryStream* in;
	TMemoryStream* out;

	unsigned char* fr = (unsigned char*)rec;

	switch(type)
	{

		case type_fields::tf_bool:
			if(fr[0]) return "true";
			return "false";

		case type_fields::tf_varchar:
		case type_fields::tf_char:
		case type_fields::tf_version:
		case type_fields::tf_version8:
			return toXML(get_presentation(rec, true, 0, false, false));

		case type_fields::tf_string: {
			uint32_t *retyped = (uint32_t*)fr;
			out = new TMemoryStream();
			parent->readBlob(out, retyped[0], retyped[1]);
			string s = TEncoding::Unicode->toUtf8(out->GetBytes());
			delete out;
			return toXML(s);
		}
		case type_fields::tf_text: {
			uint32_t *retyped = (uint32_t*)fr;
			out = new TMemoryStream();
			parent->readBlob(out, retyped[0], retyped[1]);
			string s = toXML(string(static_cast<char*>(out->GetMemory()), 0, out->GetSize()));
			delete out;
			return s;
		}
		case type_fields::tf_image: {
			uint32_t *retyped = (uint32_t*)fr;
			in = new TMemoryStream();
			out = new TMemoryStream();
			parent->readBlob(in, retyped[0], retyped[1]);
			base64_encode(in, out, 72);
			string s = TEncoding::Unicode->toUtf8(out->GetBytes());
			delete in;
			delete out;
			return s;
		}
	}

	return "{?}";
}

uint32_t CommonFieldType::get_sort_key(const char* rec, unsigned char* SortKey, int32_t maxlen) const
{
	uint32_t addlen = 0;

	unsigned char* fr = (unsigned char *)rec;

	if (!maxlen) {
		throw SerializationException("Ошибка получения ключа сортировки поля. Нулевая длина буфера.")
				.add_detail("Значение поля", get_fast_presentation(rec));
	}

	switch(type)
	{
		case type_fields::tf_bool:
			if (len > maxlen) {
				throw SerializationException("Ошибка получения ключа сортировки поля. Длина буфера меньше необходимой.")
						.add_detail("Значение поля", get_fast_presentation(rec))
						.add_detail("Длина буфера", maxlen)
						.add_detail("Необходимая длина буфера", len);
			}
			memcpy(SortKey, (void*)fr, len - addlen);
			return len;

		case type_fields::tf_char:
			throw SerializationException("Ошибка получения ключа сортировки поля. Неизвестный код возврата.")
					.add_detail("Значение поля", get_fast_presentation(rec));

		case type_fields::tf_varchar:
			throw SerializationException("Ошибка получения ключа сортировки поля. Неизвестный код возврата.")
					.add_detail("Значение поля", get_fast_presentation(rec));
	}

	throw SerializationException("Попытка получения ключа сортировки неподдерживаемого типа поля.")
			.add_detail("Значение поля", get_fast_presentation(rec));
}


field_type_declaration field_type_declaration::parse_tree(tree *field_tree)
{
	field_type_declaration type_declaration;

	if(field_tree->get_type() != node_type::nd_string) {
		throw FieldStreamParseException("Ошибка получения типа поля таблицы. Узел не является строкой.");
	}
	string sFieldType = field_tree->get_value();
	if     (sFieldType == "B")   type_declaration.type = type_fields::tf_binary;
	else if(sFieldType == "L")   type_declaration.type = type_fields::tf_bool;
	else if(sFieldType == "N")   type_declaration.type = type_fields::tf_numeric;
	else if(sFieldType == "NC")  type_declaration.type = type_fields::tf_char;
	else if(sFieldType == "NVC") type_declaration.type = type_fields::tf_varchar;
	else if(sFieldType == "RV")  type_declaration.type = type_fields::tf_version;
	else if(sFieldType == "NT")  type_declaration.type = type_fields::tf_string;
	else if(sFieldType == "T")   type_declaration.type = type_fields::tf_text;
	else if(sFieldType == "I")   type_declaration.type = type_fields::tf_image;
	else if(sFieldType == "DT")  type_declaration.type = type_fields::tf_datetime;
	else if(sFieldType == "VB")  type_declaration.type = type_fields::tf_varbinary;
	else {
		throw FieldStreamParseException("Неизвестный тип поля таблицы.")
				.add_detail("Тип поля", sFieldType);
	}

	field_tree = field_tree->get_next();
	if (field_tree->get_type() != node_type::nd_number) {
		throw FieldStreamParseException("Ошибка получения признака NULL поля таблицы. Узел не является числом.")
				.add_detail("Тип поля", sFieldType);
	}
	string sNullExists = field_tree->get_value();
	if     (sNullExists == "0") type_declaration.null_exists = false;
	else if(sNullExists == "1") type_declaration.null_exists = true;
	else {
		throw FieldStreamParseException("Неизвестное значение признака NULL поля таблицы.")
				.add_detail("Признак NUL", sNullExists);
	}

	field_tree = field_tree->get_next();
	if (field_tree->get_type() != node_type::nd_number) {
		throw FieldStreamParseException("Ошибка получения длины поля таблицы. Узел не является числом.");
	}
	type_declaration.length = stoi(field_tree->get_value());

	field_tree = field_tree->get_next();
	if(field_tree->get_type() != node_type::nd_number) {
		throw FieldStreamParseException("Ошибка получения точности поля таблицы. Узел не является числом.");
	}
	type_declaration.precision = stoi(field_tree->get_value());

	field_tree = field_tree->get_next();
	if(field_tree->get_type() != node_type::nd_string) {
		throw FieldStreamParseException("Ошибка получения регистрочувствительности поля таблицы. Узел не является строкой.");
	}

	string sCaseSensitive = field_tree->get_value();
	if     (sCaseSensitive == "CS") type_declaration.case_sensitive = true;
	else if(sCaseSensitive == "CI") type_declaration.case_sensitive = false;
	else {
		throw FieldStreamParseException("Неизвестное значение регистрочувствительности поля таблицы.")
				.add_detail("Регистрочувствительность", sFieldType);
	}

	return type_declaration;
}

FieldType *FieldType::Version8()
{
	field_type_declaration td;
	td.type = type_fields::tf_version8;
	return new CommonFieldType(td);
}

FieldType *FieldType::create_type_manager(const field_type_declaration &type_declaration)
{
	switch (type_declaration.type) {

		case type_fields::tf_binary:
		case type_fields::tf_varbinary:
			return new BinaryFieldType(type_declaration);

		case type_fields::tf_numeric:
			return new NumericFieldType(type_declaration);

		case type_fields::tf_datetime:
			return new DatetimeFieldType(type_declaration);
	}
	return new CommonFieldType(type_declaration);
}
