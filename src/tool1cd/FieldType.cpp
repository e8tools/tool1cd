//
// Created by dmpas on 07.01.18.
//

#include "FieldType.h"
#include "DetailedException.h"
#include "Common.h"
#include "Base64.h"
#include "Table.h"

// TODO: инициализация null_index
static char null_index[4096];
bool FieldType::showGUIDasMS = false;
bool FieldType::showGUID = false;


struct field_type_declaration {
	type_fields type {type_fields::tf_binary};
	bool null_exists {false};
	int32_t length {0};
	int32_t precision {0};
	bool case_sensitive {false};
};

class CommonFieldType : public FieldType
{
public:

	CommonFieldType(field_type_declaration declaration)
			: type(declaration.type), length(declaration.length), precision(declaration.precision),
			  null_exists(declaration.null_exists), case_sensitive(declaration.case_sensitive)
	{}

	virtual type_fields gettype() const override
	{
		return type;
	}

	virtual bool getnull_exists() const override
	{
		return null_exists;
	}

	virtual int32_t getlength() const override
	{
		return length;
	}

	virtual int32_t getprecision() const override
	{
		return precision;
	}

	virtual bool getcase_sensitive() const override
	{
		return case_sensitive;
	}

	virtual String get_presentation_type() const override
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
	virtual int32_t getlen() const override
	{
		if(len) return len;

		len = null_exists ? 1 : 0;
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

	virtual String get_presentation(
				const char* rec,
				bool EmptyNull,
				wchar_t Delimiter,
				bool ignore_showGUID,
				bool detailed) const override;

	virtual String get_fast_presentation(
			const char* rec) const;

	virtual bool get_binary_value(
			char *buf,
			bool null,
			const String &value) const override;

	virtual String get_XML_presentation(
			char* rec,
			Table *parent,
			bool ignore_showGUID) const override;

	virtual uint32_t getSortKey(
			const char* rec,
			unsigned char* SortKey,
			int32_t maxlen) const override;

	type_fields type {type_fields::tf_binary};
	bool null_exists {false};
	int32_t length {0};
	int32_t precision {0};
	bool case_sensitive {false};

	mutable int32_t len;
};

String CommonFieldType::get_fast_presentation(const char* rec) const
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

	virtual String get_presentation(
			const char* rec,
			bool EmptyNull,
			wchar_t Delimiter,
			bool ignore_showGUID,
			bool detailed) const override;

	virtual bool get_binary_value(
			char* buf,
			bool null,
			const String& value) const override;

	virtual String get_XML_presentation(
			char* rec,
			Table *parent,
			bool ignore_showGUID) const override;

	virtual uint32_t getSortKey(
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

	virtual String get_presentation(
			const char* rec,
			bool EmptyNull,
			wchar_t Delimiter,
			bool ignore_showGUID,
			bool detailed) const override;

	virtual bool get_binary_value(
			char* buf,
			bool null,
			const String& value) const override;

	virtual String get_XML_presentation(
			char* rec,
			Table *parent,
			bool ignore_showGUID) const override;

	virtual uint32_t getSortKey(
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

	virtual String get_presentation(
			const char* rec,
			bool EmptyNull,
			wchar_t Delimiter,
			bool ignore_showGUID,
			bool detailed) const override;

	virtual bool get_binary_value(
			char* buf,
			bool null,
			const String& value) const override;

	virtual String get_XML_presentation(
			char* rec,
			Table *parent,
			bool ignore_showGUID) const override;

	virtual uint32_t getSortKey(
			const char* rec,
			unsigned char* SortKey,
			int32_t maxlen) const override;
};

bool BinaryFieldType::get_binary_value(char* binary_value, bool null, const String& value) const
{
	unsigned char* fr = (unsigned char*)binary_value;
	memset(fr, 0, len);

	if (null_exists)
	{
		if (null) {
			return true;
		}
		*fr = 1;
		fr++;
	}
	switch(type)
	{
		case type_fields::tf_binary: {
			if(value.GetLength() == 0) {
				break;
			}
			int32_t j = 1;
			if(length == 16 && showGUID) // TODO Надо доделать для showGUIDasMS
			{
				if(value.GetLength() < 36) {
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
				if(value.GetLength() < length * 2) {
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

String BinaryFieldType::get_presentation(const char* rec, bool EmptyNull, wchar_t Delimiter, bool ignore_showGUID, bool detailed) const
{
	char sym;
	int32_t i, j, m;
	bool k;

	unsigned char* fr = (unsigned char*)rec;

	if (getnull_exists()) {
		if (fr[0] == 0) {
			return EmptyNull ? "" : "{NULL}";
		}
		fr++;
	}
	char *buf = new char[length * 2]; // TODO: адовый костыль с утечкой памяти
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

String BinaryFieldType::get_XML_presentation(char* rec, Table *parent, bool ignore_showGUID) const
{
	char sym;
	int32_t i, j, m;
	bool k;

	TMemoryStream* in;
	TMemoryStream* out;
	String s;

	unsigned char* fr = (unsigned char*)rec;

	if(null_exists) {
		if(fr[0] == 0) {
			return "";
		}
		fr++;
	}

	char *buf = new char[length*2]; // TODO: адовый костыль с утечкой памяти
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

uint32_t BinaryFieldType::getSortKey(const char* rec, unsigned char* SortKey, int32_t maxlen) const
{

	int32_t i, j;
	bool k;
	uint32_t addlen = 0;
	int32_t maxl = maxlen;
	bool isnull = false;
	String s;
	unsigned char c;

	unsigned char* fr = (unsigned char *)rec;

	char nbuf[64];

	if (!maxlen) {
		throw SerializationException("Ошибка получения ключа сортировки поля. Нулевая длина буфера.")
				.add_detail("Значение поля", get_fast_presentation(rec));
	}

	if (null_exists) {
		if (*fr == 0) {
			*(SortKey++) = 0;
			isnull = true;
		}
		else *(SortKey++) = 1;

		fr++;
		maxl--;
		addlen = 1;
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
			memcpy(SortKey, isnull ? (void *)null_index : (void*)fr, len - addlen);
			return len;

		case type_fields::tf_varbinary:
			throw SerializationException("Попытка получения ключа сортировки неподдерживаемого типа поля.")
					.add_detail("Значение поля", get_fast_presentation(rec));
	}

	return addlen;

}


bool NumericFieldType::get_binary_value(char* binary_value, bool null, const String& value) const
{
	unsigned char* fr = (unsigned char*)binary_value;
	memset(fr, 0, len);

	if (null_exists) {
		if (null) {
			return true;
		}
		*fr = 1;
		fr++;
	}
	switch(type)
	{
		case type_fields::tf_numeric: {
			int32_t l = value.GetLength();
			if(!l) {
				break;
			}

			unsigned char* b = new unsigned char[l];
			bool k = false; // знак минус
			int32_t m = -1; // позиция точки
			bool n = false; // признак наличия значащих цифр в начале
			int32_t j = 0;

			for(int32_t ind = 0; ind < l; ind++)
			{
				wchar_t sym = value[ind + 1];
				if(sym == L'-')
				{
					k = true;
					continue;
				}
				if(sym == L'.')
				{
					m = j;
					n = true;
					continue;
				}
				if(!n) {
					if(sym == L'0')
					{
						continue;
					}
				}
				if(sym >= L'0' || sym <= L'9')
				{
					b[j++] = sym - L'0';
					n = true;
				}
			}
			if(m == -1) {
				m = j;
			}

			// тут имеем:
			// в b значащие цифры
			// k - признак минуса
			// j - всего значащих цифр
			// m - позиция точки (количество цифр до запятой, что одно и то же)

			//     0     1     2     3
			//+-----+-----+-----+-----+
			//I  .  I  .  I  .  I  .  I
			//+-----+-----+-----+-----+
			//  S  0  1  2  3  4  5  6  (номер цифры (полубайта), ниже равен i)

			l = length - precision; // макс. количество цифр до запятой
			if(m > l)
			{
				// значение превышает максимально допустимое, заменяем на все 9ки
				for(int32_t ind = 0; ind < length; ind++)
				{
					if(ind & 1) {
						fr[(ind + 1) >> 1] |= 0x90;
					}
					else {
						fr[(ind + 1) >> 1] |= 0x9;
					}
				}
			}
			else
			{
				int32_t p = 0;
				for(int32_t ind = l - 1, p = m - 1; p >= 0; ind--, p--)
				{
					if(ind & 1) {
						fr[(ind + 1) >> 1] |= b[p] << 4;
					}
					else {
						fr[(ind + 1) >> 1] |= b[p];
					}
				}
				int32_t q = std::min(j - m, precision); // количество цифр после запятой
				for(int32_t ind = l, p = m; p < m + q; ind++, p++)
				{
					if(ind & 1) {
						fr[(ind + 1) >> 1] |= b[p] << 4;
					}
					else {
						fr[(ind + 1) >> 1] |= b[p];
					}
				}
			}

			if(!k) {
				*fr |= 0x10; // Знак
			}

			delete[] b;

			break;
		}
	}

	return true;
}

String NumericFieldType::get_presentation(const char* rec, bool EmptyNull, wchar_t Delimiter, bool ignore_showGUID, bool detailed) const
{
	char sym;
	unsigned char* fr = (unsigned char*)rec;

	if (getnull_exists()) {
		if (fr[0] == 0) {
			return EmptyNull ? "" : "{NULL}";
		}
		fr++;
	}
	TStringBuilder sb;
	bool digitNotStarted = true; // признак, что значащие цифры еще не начались
	int pointPosition = length - precision; // позиция десятичной точки слева
	if(fr[0] >> 4 == 0) {
		sb.Append('-');
	}
	for (int j = 0; j < length; j++) {
		if (Delimiter) {
			if (!digitNotStarted) {
				if (pointPosition - j > 0) {
					if ((pointPosition - j) % 3 == 0) {
						sb.Append(Delimiter);
					}
				}
			}
		}
		if (j == pointPosition) {
			sb.Append('.');
			digitNotStarted = false;
		}
		if (j & 1) sym = fr[(j + 1) >> 1] >> 4;
		else sym = fr[j >> 1] & 0xf;

		if (sym == 0 && digitNotStarted) {
			continue;
		}

		digitNotStarted = false;
		sb.Append('0' + sym);
	}

	return sb.ToString();
}

String NumericFieldType::get_XML_presentation(char* rec, Table *parent, bool ignore_showGUID) const
{
	char sym;

	unsigned char* fr = (unsigned char*)rec;

	if (null_exists) {
		if(fr[0] == 0) {
			return "";
		}
		fr++;
	}

	TStringBuilder sb;
	bool digitNotStarted = true; // признак, что значащие цифры еще не начались
	int m = length - precision; // позиция десятичной точки слева
	if (fr[0] >> 4 == 0) {
		sb.Append('-');
	}
	for (int j = 0; j < length; j++) {
		if (j == m) {
			sb.Append('.');
			digitNotStarted = false;
		}
		if(j & 1) sym = fr[(j + 1) >> 1] >> 4;
		else sym = fr[j >> 1] & 0xf;
		if (sym == 0 && digitNotStarted) {
			continue;
		}
		digitNotStarted = false;
		sb.Append('0' + sym);
	}
	if (digitNotStarted) {
		return "0";
	}
	return sb.ToString();
}

uint32_t NumericFieldType::getSortKey(const char* rec, unsigned char* SortKey, int32_t maxlen) const
{

	int32_t i, j;
	bool k;
	uint32_t addlen = 0;
	int32_t maxl = maxlen;
	bool isnull = false;
	String s;
	unsigned char c;

	unsigned char* fr = (unsigned char *)rec;

	char nbuf[64];

	if (!maxlen) {
		throw SerializationException("Ошибка получения ключа сортировки поля. Нулевая длина буфера.")
				.add_detail("Значение поля", get_fast_presentation(rec));
	}

	if (null_exists) {
		if(*fr == 0) {
			*(SortKey++) = 0;
			isnull = true;
		}
		else *(SortKey++) = 1;

		fr++;
		maxl--;
		addlen = 1;
	}

	if (isnull)
	{
		if (len > maxlen) {
			throw SerializationException("Ошибка получения ключа сортировки поля. Длина буфера меньше необходимой.")
					.add_detail("Значение поля", get_fast_presentation(rec))
					.add_detail("Длина буфера", maxlen)
					.add_detail("Необходимая длина буфера", len);
		}
		memcpy(SortKey, null_index, len - addlen);
		return len;

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


bool DatetimeFieldType::get_binary_value(char* binary_value, bool null, const String& inValue) const
{
	// TODO: (DatetimeFieldType::get_binary_value) тут колдунство!

	String value = inValue;

	unsigned char* fr = (unsigned char*)binary_value;
	memset(fr, 0, len);

	if (null_exists) {
		if (null) {
			return true;
		}
		*fr = 1;
		fr++;
	}

	if (value.GetLength() < 19) {
		fr[1] = 1;
		fr[2] = 1;
		fr[3] = 1;
	} else {

		#define correct_spaces(A,B)\
		if(value[B] == L' ')\
		{\
			value[B] = value[A];\
			value[A] = L'0';\
		}\
		if(value[A] == L' ') value[A] = L'0';

		correct_spaces(1,2) // корректируем день
		correct_spaces(4,5) // корректируем месяц
		correct_spaces(12,13) // корректируем часы
		correct_spaces(15,16) // корректируем минуты
		correct_spaces(18,19) // корректируем секунды

		int32_t i = 0;
		// корректируем год
		while(value[10] == L' ')
		{
			value[10] = value[9];
			value[9] = value[8];
			value[8] = value[7];
			value[7] = L'0';
			i++;
		}
		while(value[9] == L' ')
		{
			value[9] = value[8];
			value[8] = value[7];
			value[7] = L'0';
			i++;
		}
		while(value[8] == L' ')
		{
			value[8] = value[7];
			value[7] = L'0';
			i++;
		}
		if(value[7] == L' ') {
			value[7] = L'0';
		}

		// дополняем год при необходимости
		switch(i)
		{
			case 1:
				value[7] = L'2';
				break;
			case 2:
				value[7] = L'2';
				value[8] = L'0';
				break;
			case 3:
				value[7] = L'2';
				value[8] = L'0';
				value[9] = L'1';
				break;
		}

		// исправляем день, месяц, год
		int32_t day = (value[1] - L'0') * 10 + (value[2] - L'0'); // день
		int32_t month = (value[4] - L'0') * 10 + (value[5] - L'0'); // месяц
		int32_t year = (value[7] - L'0') * 1000 + (value[8] - L'0') * 100 + (value[9] - L'0') * 10 + (value[10] - L'0'); // год

		if(month > 12)
		{
			month = 12;
			value[4] = L'1';
			value[5] = L'2';
		}
		else if(month == 0)
		{
			month = 1;
			value[4] = L'0';
			value[5] = L'1';
		}

		if(year == 0)
		{
			year = 1;
			value[7] = L'0'; //-V525
			value[8] = L'0';
			value[9] = L'0';
			value[10] = L'1';
		}

		if(day == 0)
		{
			value[1] = L'0';
			value[2] = L'1';
		}
		else if(day > 28) switch(month)
			{
				case 1:
				case 3:
				case 5:
				case 7:
				case 8:
				case 10:
				case 12:
					if(day > 31)
					{
						value[1] = L'3';
						value[2] = L'1';
					}
					break;
				case 4:
				case 6:
				case 9:
				case 11:
					if(day > 30)
					{
						value[1] = L'3';
						value[2] = L'0';
					}
					break;
				case 2:
					if(year % 4 == 0 && (year % 100 != 0 || year % 400 == 0))
					{
						if(day > 29)
						{
							value[1] = L'2';
							value[2] = L'9';
						}
					}
					else
					{
						if(day > 28)
						{
							value[1] = L'2';
							value[2] = L'8';
						}
					}
					break;
			}

		// исправляем часы, минуты, секунды
		int32_t hours = (value[12] - L'0') * 10 + (value[13] - L'0'); // часы
		if(hours > 23)
		{
			value[12] = L'2';
			value[13] = L'3';
		}

		int32_t minutes = (value[15] - L'0') * 10 + (value[16] - L'0'); // минуты
		if(minutes > 59)
		{
			value[15] = L'5';
			value[16] = L'9';
		}

		int32_t seconds = (value[18] - L'0') * 10 + (value[19] - L'0'); // секунды
		if(seconds > 59)
		{
			value[18] = L'5';
			value[19] = L'9';
		}

		fr[3] = ((value[1] - L'0') << 4) + (value[2] - L'0');
		fr[2] = ((value[4] - L'0') << 4) + (value[5] - L'0');
		fr[0] = ((value[7] - L'0') << 4) + (value[8] - L'0');
		fr[1] = ((value[9] - L'0') << 4) + (value[10] - L'0');
		fr[4] = ((value[12] - L'0') << 4) + (value[13] - L'0');
		fr[5] = ((value[15] - L'0') << 4) + (value[16] - L'0');
		fr[6] = ((value[18] - L'0') << 4) + (value[19] - L'0');
	}

	return true;
}

String DatetimeFieldType::get_presentation(const char* rec, bool EmptyNull, wchar_t Delimiter, bool ignore_showGUID, bool detailed) const
{
	unsigned char* fr = (unsigned char*)rec;

	if (getnull_exists()) {
		if (fr[0] == 0) {
			return EmptyNull ? "" : "{NULL}";
		}
		fr++;
	}
	return date_to_string(fr);
}

String DatetimeFieldType::get_XML_presentation(char* rec, Table *parent, bool ignore_showGUID) const
{

	unsigned char* fr = (unsigned char*)rec;

	if (null_exists) {
		if(fr[0] == 0) {
			return "";
		}
		fr++;
	}

	char *buf = new char[length*2]; // TODO: адовый костыль с утечкой памяти

	buf[0] = '0' + (fr[0] >> 4);
	buf[1] = '0' + (fr[0] & 0xf);
	buf[2] = '0' + (fr[1] >> 4);
	buf[3] = '0' + (fr[1] & 0xf);
	buf[4] = '-';
	buf[5] = '0' + (fr[2] >> 4);
	buf[6] = '0' + (fr[2] & 0xf);
	buf[7] = '-';
	buf[8] = '0' + (fr[3] >> 4);
	buf[9] = '0' + (fr[3] & 0xf);
	buf[10] = 'T';
	buf[11] = '0' + (fr[4] >> 4);
	buf[12] = '0' + (fr[4] & 0xf);
	buf[13] = ':';
	buf[14] = '0' + (fr[5] >> 4);
	buf[15] = '0' + (fr[5] & 0xf);
	buf[16] = ':';
	buf[17] = '0' + (fr[6] >> 4);
	buf[18] = '0' + (fr[6] & 0xf);
	buf[19] = 0;
	return buf;

}

uint32_t DatetimeFieldType::getSortKey(const char* rec, unsigned char* SortKey, int32_t maxlen) const
{

	int32_t i, j;
	bool k;
	uint32_t addlen = 0;
	int32_t maxl = maxlen;
	bool isnull = false;
	String s;
	unsigned char c;

	unsigned char* fr = (unsigned char *)rec;

	char nbuf[64];

	if (!maxlen) {
		throw SerializationException("Ошибка получения ключа сортировки поля. Нулевая длина буфера.")
				.add_detail("Значение поля", get_fast_presentation(rec));
	}

	if (null_exists) {
		if (*fr == 0) {
			*(SortKey++) = 0;
			isnull = true;
		}
		else *(SortKey++) = 1;

		fr++;
		maxl--;
		addlen = 1;
	}

	if (len > maxlen) {
		throw SerializationException("Ошибка получения ключа сортировки поля. Длина буфера меньше необходимой.")
				.add_detail("Значение поля", get_fast_presentation(rec))
				.add_detail("Длина буфера", maxlen)
				.add_detail("Необходимая длина буфера", len);
	}
	memcpy(SortKey, isnull ? (void *)null_index : (void*)fr, len - addlen);
	return len;

}


bool CommonFieldType::get_binary_value(char *binary_value, bool null, const String &value) const
{
	unsigned char* fr = (unsigned char*)binary_value;
	memset(fr, 0, len);

	if (null_exists) {
		if(null) {
			return true;
		}
		*fr = 1;
		fr++;
	}
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
			int32_t i = std::min(value.GetLength(), length);
			memcpy(fr, value.c_str(), i << 1);
			while(i < length) {
				((WCHART*)fr)[i++] = L' ';
			}
			break;
		}
		case type_fields::tf_varchar: {
			int32_t i = std::min(value.GetLength(), length);
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

String CommonFieldType::get_presentation(const char* rec, bool EmptyNull, wchar_t Delimiter, bool ignore_showGUID, bool detailed) const
{
	char sym;
	int32_t i, j, m;

	unsigned char* fr = (unsigned char*)rec;

	if (getnull_exists()) {
		if (fr[0] == 0) {
			return EmptyNull ? "" : "{NULL}";
		}
		fr++;
	}
	char *buf = new char[length * 2]; // TODO: адовый костыль с утечкой памяти
	switch(type)
	{
		case type_fields::tf_bool:
			if(fr[0]) return "true";
			return "false";
		case type_fields::tf_char:
			return String((WCHART*)fr, length);
		case type_fields::tf_varchar:
			i = *(int16_t*)fr;
			return String((WCHART*)(fr + 2), i);
		case type_fields::tf_version:
			return String(*(int32_t*)fr) + ":" + *(int32_t*)(fr + 4) + ":" + *(int32_t*)(fr + 8) + ":" + *(int32_t*)(fr + 12);
		case type_fields::tf_version8:
			return String(*(int32_t*)fr) + ":" + *(int32_t*)(fr + 4);
		case type_fields::tf_string:
			return detailed ? String("{MEMO} [") + to_hex_string(*(int32_t*)fr) + "][" + to_hex_string(*(int32_t*)(fr + 4)) + "]" : String("{MEMO}");
		case type_fields::tf_text:
			return detailed ? String("{TEXT} [") + to_hex_string(*(int32_t*)fr) + "][" + to_hex_string(*(int32_t*)(fr + 4)) + "]" : String("{TEXT}");
		case type_fields::tf_image:
			return detailed ? String("{IMAGE} [") + to_hex_string(*(int32_t*)fr) + "][" + to_hex_string(*(int32_t*)(fr + 4)) + "]" : String("{IMAGE}");
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

String CommonFieldType::get_XML_presentation(char* rec, Table *parent, bool ignore_showGUID) const
{
	int32_t i, j, m;

	TMemoryStream* in;
	TMemoryStream* out;
	String s;

	unsigned char* fr = (unsigned char*)rec;

	if (null_exists) {
		if (fr[0] == 0) {
			return "";
		}
		fr++;
	}

	char *buf = new char[length*2]; // TODO: адовый костыль с утечкой памяти
	switch(type)
	{

		case type_fields::tf_bool:
			if(fr[0]) return "true";
			return "false";

		case type_fields::tf_char:
			return toXML(String((WCHART*)fr, length));
		case type_fields::tf_varchar:
			i = *(int16_t*)fr;
			return toXML(String(((WCHART*)fr) + 1, i));
		case type_fields::tf_version: {
			int32_t *retyped = (int32_t*)fr;
			return String(*(int32_t*)fr) + ":" + retyped[1] + ":" + retyped[2] + ":" + retyped[3];
		}
		case type_fields::tf_version8: {
			int32_t *retyped = (int32_t*)fr;
			return String(retyped[0] + ":" + retyped[1]);
		}
		case type_fields::tf_string: {
			uint32_t *retyped = (uint32_t*)fr;
			out = new TMemoryStream();
			parent->readBlob(out, retyped[0], retyped[1]);
			s = toXML(String((WCHART*)(out->GetMemory()), out->GetSize() / 2));
			delete out;
			return s;
		}
		case type_fields::tf_text: {
			uint32_t *retyped = (uint32_t*)fr;
			out = new TMemoryStream();
			parent->readBlob(out, retyped[0], retyped[1]);
			s = toXML(String((char*)(out->GetMemory()), out->GetSize()));
			delete out;
			return s;
		}
		case type_fields::tf_image: {
			uint32_t *retyped = (uint32_t*)fr;
			in = new TMemoryStream();
			out = new TMemoryStream();
			parent->readBlob(in, retyped[0], retyped[1]);
			base64_encode(in, out, 72);
			s = String((WCHART*)(out->GetMemory()), out->GetSize() / 2);
			delete in;
			delete out;
			return s;
		}
	}

	return "{?}";
}

uint32_t CommonFieldType::getSortKey(const char* rec, unsigned char* SortKey, int32_t maxlen) const
{
	uint32_t addlen = 0;
	bool isnull = false;

	unsigned char* fr = (unsigned char *)rec;

	if (!maxlen) {
		throw SerializationException("Ошибка получения ключа сортировки поля. Нулевая длина буфера.")
				.add_detail("Значение поля", get_fast_presentation(rec));
	}

	if (null_exists) {
		if (*fr == 0) {
			*(SortKey++) = 0;
			isnull = true;
		} else *(SortKey++) = 1;

		fr++;
		addlen = 1;
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
			memcpy(SortKey, isnull ? (void *)null_index : (void*)fr, len - addlen);
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


field_type_declaration parse_field_type_declaration(tree *field_tree)
{
	field_type_declaration type_declaration;

	if(field_tree->get_type() != node_type::nd_string) {
		throw FieldStreamParseException("Ошибка получения типа поля таблицы. Узел не является строкой.");
	}
	String sFieldType = field_tree->get_value();
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
	String sNullExists = field_tree->get_value();
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
	type_declaration.length = StrToInt(field_tree->get_value());

	field_tree = field_tree->get_next();
	if(field_tree->get_type() != node_type::nd_number) {
		throw FieldStreamParseException("Ошибка получения точности поля таблицы. Узел не является числом.");
	}
	type_declaration.precision = StrToInt(field_tree->get_value());

	field_tree = field_tree->get_next();
	if(field_tree->get_type() != node_type::nd_string) {
		throw FieldStreamParseException("Ошибка получения регистрочувствительности поля таблицы. Узел не является строкой.");
	}

	String sCaseSensitive = field_tree->get_value();
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

FieldType *FieldType::parse_field_type(tree *field_tree)
{
	field_type_declaration type_declaration = parse_field_type_declaration(field_tree);
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
