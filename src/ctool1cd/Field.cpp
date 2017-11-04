/*
 *
 *
 *
 *
 */

#include "Field.h"
#include "Base64.h"
#include "TempStream.h"

extern Registrator msreg_g;

char Field::buf[2048 + 16];
char Field::null_index[4096];
bool Field::null_index_initialized = false;
bool Field::showGUID = true;
bool Field::showGUIDasMS = false;


//---------------------------------------------------------------------------
Field::Field(Table* _parent)
{
	if(!null_index_initialized)
	{
		*null_index = 1;
		memset(null_index + 1, 0, 4095);
		null_index_initialized = true;
	}
	null_exists = false;
	length = 0;
	precision = 0;
	case_sensitive = false;

	parent = _parent;
	len = 0;
	offset = 0;
	type = type_fields::tf_binary;
}

//---------------------------------------------------------------------------
String Field::getname()
{
	return name;
}

//---------------------------------------------------------------------------
int32_t Field::getlen() // возвращает длину поля в байтах
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

//---------------------------------------------------------------------------
// При ignore_showGUID binary16 всегда преобразуется в GUID
String Field::get_presentation(const char* rec, bool EmptyNull, wchar_t Delimiter, bool ignore_showGUID, bool detailed)
{
	char sym;
	int32_t i, j, m;
	bool k;

	unsigned char* fr = (unsigned char*)rec + offset;

	if(null_exists)
	{
		if(fr[0] == 0) return EmptyNull ? "" : "{NULL}";
		fr++;
	}
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
		case type_fields::tf_bool:
			if(fr[0]) return "true";
			return "false";
		case type_fields::tf_numeric:
			i = 0; // текущий индекс в buf
			k = true; // признак, что значащие цифры еще не начались
			m = length - precision; // позиция десятичной точки слева
			if(fr[0] >> 4 == 0) buf[i++] = '-';
			for(j = 0; j < length; j++)
			{
				if(Delimiter) if(!k) if(m - j > 0) if((m - j) % 3 == 0) buf[i++] = Delimiter;
				if(j == m)
				{
					buf[i++] = '.';
					k = false;
				}
				if(j & 1) sym = fr[(j + 1) >> 1] >> 4;
				else sym = fr[j >> 1] & 0xf;
				if(sym == 0 && k) continue;
				k = false;
				buf[i++] = '0' + sym;
			}

			if(k) buf[i++] = '0';

			buf[i] = 0;
			return buf;
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
			return detailed ? String("{MEMO} [") + tohex(*(int32_t*)fr) + "][" + tohex(*(int32_t*)(fr + 4)) + "]" : String("{MEMO}");
		case type_fields::tf_text:
			return detailed ? String("{TEXT} [") + tohex(*(int32_t*)fr) + "][" + tohex(*(int32_t*)(fr + 4)) + "]" : String("{TEXT}");
		case type_fields::tf_image:
			return detailed ? String("{IMAGE} [") + tohex(*(int32_t*)fr) + "][" + tohex(*(int32_t*)(fr + 4)) + "]" : String("{IMAGE}");
		case type_fields::tf_datetime:
			return date_to_string(fr);
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

//---------------------------------------------------------------------------
bool Field::get_bynary_value(char* binary_value, bool null, String& value)
{
	unsigned char* fr = (unsigned char*)binary_value;
	memset(fr, 0, len);

	if(null_exists)
	{
		if(null)
		{
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
		case type_fields::tf_bool: {
			if(value == "true") {
				*fr = 1;
			}
			else {
				*fr = 0;
			}
			break;
		}
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
		case type_fields::tf_datetime: {
			if(value.GetLength() < 19)
			{
				fr[1] = 1;
				fr[2] = 1;
				fr[3] = 1;
			}
			else
			{

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
			break;
		}
		case type_fields::tf_varbinary: {
			return false;
		}
	}

	return true;
}

//---------------------------------------------------------------------------
String Field::get_XML_presentation(char* rec, bool ignore_showGUID)
{
	char sym;
	int32_t i, j, m;
	bool k;

	TMemoryStream* in;
	TMemoryStream* out;
	String s;

	unsigned char* fr = (unsigned char*)rec + offset;

	if(null_exists)
	{
		if(fr[0] == 0) return "";
		fr++;
	}
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
		case type_fields::tf_bool:
			if(fr[0]) return "true";
			return "false";
		case type_fields::tf_numeric:
			i = 0; // текущий индекс в buf
			k = true; // признак, что значащие цифры еще не начались
			m = length - precision; // позиция десятичной точки слева
			if(fr[0] >> 4 == 0) buf[i++] = '-';
			for(j = 0; j < length; j++)
			{
				if(j == m)
				{
					buf[i++] = '.';
					k = false;
				}
				if(j & 1) sym = fr[(j + 1) >> 1] >> 4;
				else sym = fr[j >> 1] & 0xf;
				if(sym == 0 && k) continue;
				k = false;
				buf[i++] = '0' + sym;
			}
			if(k) return "0";
			buf[i] = 0;
			return buf;
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
		case type_fields::tf_datetime:
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

//---------------------------------------------------------------------------
type_fields Field::gettype()
{
	return type;
}

//---------------------------------------------------------------------------
Table* Field::getparent()
{
	return parent;
}

//---------------------------------------------------------------------------
bool Field::getnull_exists()
{
	return null_exists;
}

//---------------------------------------------------------------------------
int32_t Field::getlength()
{
	return length;
}

//---------------------------------------------------------------------------
int32_t Field::getprecision()
{
	return precision;
}

//---------------------------------------------------------------------------
bool Field::getcase_sensitive()
{
	return case_sensitive;
}

//---------------------------------------------------------------------------
int32_t Field::getoffset()
{
	return offset;
}

//---------------------------------------------------------------------------
String Field::get_presentation_type()
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
String TrimSpacesRight(String s)
{
	while(s.SubString(s.GetLength(), 1) == " ") s.SetLength(s.GetLength() - 1);
	return s;
}

//---------------------------------------------------------------------------
uint32_t Field::getSortKey(const char* rec, unsigned char* SortKey, int32_t maxlen)
{

	int32_t i, j;
	bool k;
	uint32_t addlen = 0;
	int32_t maxl = maxlen;
	bool isnull = false;
	String s;
	unsigned char c;

	unsigned char* fr = (unsigned char *)rec + offset;

	char nbuf[64];

	if(!maxlen)
	{
		msreg_g.AddError("Ошибка получения ключа сортировки поля. Нулевая длина буфера.",
			"Таблица", parent->name,
			"Поле", name,
			"Значение поля", get_presentation(rec));

		return 0;
	}

	if(null_exists)
	{
		if(*fr == 0)
		{
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
		case type_fields::tf_datetime:
		case type_fields::tf_bool:
			if(len > maxlen)
			{
				msreg_g.AddError("Ошибка получения ключа сортировки поля. Длина буфера меньше необходимой.",
					"Таблица", parent->name,
					"Поле", name,
					"Значение поля", get_presentation(rec),
					"Длина буфера", maxlen,
					"Необходимая длина буфера", len);

				memcpy(SortKey, isnull ? (void *)null_index : (void *)fr, maxl);
				return maxlen;
			}
			memcpy(SortKey, isnull ? (void *)null_index : (void*)fr, len - addlen);
			return len;

		case type_fields::tf_numeric:

			if(isnull)
			{
				if(len > maxlen)
				{
					msreg_g.AddError("Ошибка получения ключа сортировки поля. Длина буфера меньше необходимой.",
						"Таблица", parent->name,
						"Поле", name,
						"Значение поля", get_presentation(rec),
						"Длина буфера", maxlen,
						"Необходимая длина буфера", len);

					memcpy(SortKey, null_index, maxl);
					return maxlen;
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
				msreg_g.AddError("Ошибка получения ключа сортировки поля. Длина буфера меньше необходимой.",
					"Таблица", parent->name,
					"Поле", name,
					"Значение поля", get_presentation(rec),
					"Длина буфера", maxlen,
					"Необходимая длина буфера", len);

				memcpy(SortKey, nbuf, maxl);
				return maxlen;
			}
			memcpy(SortKey, nbuf, len - addlen);
			return len;

		case type_fields::tf_char:
					msreg_g.AddError("Ошибка получения ключа сортировки поля. Неизвестный код возврата.",
						"Таблица", parent->name,
						"Поле", name,
						"Значение поля", get_presentation(rec));
					return addlen;

		case type_fields::tf_varchar:
					msreg_g.AddError("Ошибка получения ключа сортировки поля. Неизвестный код возврата.",
						"Таблица", parent->name,
						"Поле", name,
						"Значение поля", get_presentation(rec));
					return addlen;

		case type_fields::tf_version:
		case type_fields::tf_version8:
		case type_fields::tf_string:
		case type_fields::tf_text:
		case type_fields::tf_image:
		case type_fields::tf_varbinary:
			msreg_g.AddError("Попытка получения ключа сортировки неподдерживаемого типа поля.",
				"Таблица", parent->name,
				"Поле", name,
				"Тип поля", get_presentation_type());
			return 0;
	}

	return addlen;

}

//---------------------------------------------------------------------------
bool Field::save_blob_to_file(char* rec, String _filename, bool unpack)
{
	TStream* blob_stream;
	TStream* _s;
	TStream* _s2;
	TStream* _sx;
	TStream* _sx2;
	uint32_t k, l;
	bool usetemporaryfiles = false;

	char *orec = rec;
	rec += offset;
	if (null_exists) {
		if(*rec == 0) {
			return false;
		}
		rec++;
	}

	if (*(uint32_t*)rec == 0 || *(uint32_t*)(rec + 4) == 0) {
		return false;
	}

	if(!unpack) {
		TFileStream temp_stream(_filename, fmCreate);
		parent->readBlob(&temp_stream, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
		return true;
	}

	usetemporaryfiles = *(uint32_t*)(rec + 4) > 10 * 1024 * 1024;
	if(usetemporaryfiles) blob_stream = new TTempStream;
	else blob_stream = new TMemoryStream;
	parent->readBlob(blob_stream, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
	if(blob_stream->GetSize() == 0)
	{
		delete blob_stream;
		return false;
	}

	Table *tab = parent;
	if(usetemporaryfiles) _s = new TTempStream;
	else _s = new TMemoryStream;

	if(tab->get_issystem())
	{

		// спецобработка для users.usr
		String tabname = tab->getname();
		bool is_users_usr = false;
		if(tabname.CompareIC("PARAMS") == 0)
		{
			Field *_f = tab->getfield(0);
			if(_f->get_presentation(orec).CompareIC("users.usr") == 0) is_users_usr = true;
		}
		else if(tabname.CompareIC("V8USERS") == 0) is_users_usr = true;

		bool maybezipped_twice = true;
		if(tabname.CompareIC("CONFIG") == 0 || tabname.CompareIC("CONFIGSAVE") == 0)
		{
			Field *_f = tab->getfield(0);
			maybezipped_twice = _f->get_presentation(orec).GetLength() > 72;
		}

		if(is_users_usr)
		{

			size_t stream_size = blob_stream->GetSize();
			char *_bb = new char[stream_size];
			blob_stream->Seek(0, soFromBeginning);
			blob_stream->Read(_bb, stream_size);

			size_t xor_mask_size = _bb[0];
			char *_xor_mask = &_bb[1];
			char *_xor_buf = &_xor_mask[xor_mask_size];
			size_t data_size = stream_size - xor_mask_size - 1;
			for(size_t i = 0, k = 0; i < data_size; i++, k++)
			{
				if (k >= xor_mask_size) {
					k = 0;
				}
				_xor_buf[i] ^= _xor_mask[k];
			}
			TFileStream temp_stream(_filename, fmCreate);
			temp_stream.SetSize(0);
			temp_stream.WriteBuffer(_xor_buf, data_size);
			delete[] _bb;
		}
		else
		{
			bool zippedContainer = false;
			bool zipped = false;
			try
			{
				blob_stream->Seek(0, soFromBeginning);
				ZInflateStream(blob_stream, _s);
				zipped = true;
				if(maybezipped_twice) _sx = _s;
				else _sx2 = _s;
				_s = NULL;
				delete blob_stream;
				blob_stream = NULL;
			}
			catch (...)
			{
				_sx2 = blob_stream;
				delete _s;
				_s = NULL;
				blob_stream = NULL;
				zipped = false;
			}

			if(zipped && maybezipped_twice)
			{
				if(usetemporaryfiles) _s2 = new TTempStream;
				else _s2 = new TMemoryStream;
				try
				{
					_sx->Seek(0, soFromBeginning);
					ZInflateStream(_sx, _s2);
					zippedContainer = true;
					_sx2 = _s2;
					_s2 = NULL;
					delete _sx;
					_sx = NULL;
				}
				catch (...)
				{
					_sx2 = _sx;
					_sx = NULL;
					delete _s2;
					_s2 = NULL;
				}
			}

			v8catalog *cat = new v8catalog(_sx2, zippedContainer, true);
			if(!cat->GetFirst())
			{
				TFileStream temp_stream(_filename, fmCreate);
				temp_stream.CopyFrom(_sx2, 0);
			}
			else cat->SaveToDir(_filename);
			delete cat;
			delete _sx2;

		}
	}
	else /*if(tab->get_issystem())*/
	{
		char _buf[16];
		_s->CopyFrom(blob_stream, 0);
		blob_stream->Seek(0, soFromBeginning);
		if(blob_stream->Read(_buf, 2) >= 2) if((_buf[0] == 1 || _buf[0] == 2) && _buf[1] == 1)
		{
			if(usetemporaryfiles) _s2 = new TTempStream;
			else _s2 = new TMemoryStream;
			bool isOK = true;
			if(_buf[0] == 1) // неупакованное хранилище
			{
				_s2->CopyFrom(blob_stream, blob_stream->GetSize() - 2);
			}
			else
			{
				if(blob_stream->Read(_buf, 16) < 16) isOK = false;
				else
				{
					if(memcmp(_buf, SIG_ZIP, 16) != 0) isOK = false;
					else
					{
						try
						{
							ZInflateStream(blob_stream, _s2);
						}
						catch(...)
						{
							isOK = false;
						}
					}
				}
			}
			if(isOK)
			{
				_s2->Seek(0, soFromBeginning);
				if(_s2->Read(_buf, 8) < 8) isOK = false;
				else
				{
					_s->SetSize(0);
					_s->CopyFrom(_s2, _s2->GetSize() - 8);
				}

			}

			if(isOK)
			{
				int64_t len1C = *(int64_t*)_buf;
				if(_s->GetSize() > len1C)
				{
					_s->Seek(len1C, (TSeekOrigin)soFromBeginning);
					_s2->SetSize(0);
					_s2->CopyFrom(_s, _s->GetSize() - len1C);
					_s2->Seek(0, soFromBeginning);
					if(_s2->Read(_buf, 12) >= 12)
					{
						len1C = *(int64_t*)&_buf[4];
						if(len1C <= _s2->GetSize() - 12)
						{
							_s->SetSize(0);
							_s->CopyFrom(_s2, len1C);
						}
					}
				}
			}
			delete _s2;
		}

		TFileStream temp_stream(_filename, fmCreate);
		temp_stream.CopyFrom(_s, 0);
	}

	delete _s;
	delete blob_stream;

	return true;
}
