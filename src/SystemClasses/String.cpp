#include "String.hpp"
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <string>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>
#include "System.SysUtils.hpp"
#include "utf8.h"

using namespace std;

namespace System {

/** Конструктор (C strings).
 *      @param - без параметров
 *
 *
 *
 */
String::String()
{
}

/** Конструктор (C strings).
 *      @src - входная строка
 */
String::String(const string &src) : string(src)
{
}

/** Конструктор (C strings).
 *      @src - входная строка
 */
String::String(const char *src) : string(src)
{
}

/** Конструктор (C strings).
 *      @src        - входная строка
 *      @limit_size - лимит в размере
 */
String::String(const char *src, int limit_size) : string(src, limit_size)
{
}

/** Конструктор (C strings).
 *      @src        - входная строка
 *      @limit_size - лимит в размере
 */
String::String(const std::vector<t::Byte> &bytes, int offset, int size)
		: string(reinterpret_cast<const char *>(bytes.data() + offset), ( size == -1 ? (bytes.size()-offset) : size))
{
}

/** Конструктор (C strings).
 *      @w_src        - входная строка
 *      @limit_size - лимит в размере
 */
String::String(const WCHART *w_src, int limit_size)
{
	// Костыль
	std::vector<t::Byte> tmpdata;
	const WCHART *p = w_src;
	bool limit_exceeded = false;

	if( limit_size > 0 && p != nullptr ) {

		while ( !limit_exceeded && *p != '\0' ) {

			tmpdata.push_back((uint16_t)(*p) & 0xFF);
			tmpdata.push_back((uint16_t)(*p) >> 8);

			++p;

			if (limit_size) {
				limit_exceeded = --limit_size == 0;
			}
		}
	}
	string tmp(SysUtils::TEncoding::Unicode->toUtf8(tmpdata));
	append(tmp);
}

/** Конструктор (C strings).
 *      @w_src        - входная строка
 */
String::String(const WCHART *w_src)
{
	// Костыль
	std::vector<t::Byte> tmpdata;
	const WCHART *p = w_src;

	if( p != nullptr ) {

		while ( *p != u'\0' ) {

			tmpdata.push_back((uint16_t)(*p) & 0xFF);
			tmpdata.push_back((uint16_t)(*p) >> 8);

			++p;

		}
	}
	string tmp(SysUtils::TEncoding::Unicode->toUtf8(tmpdata));
	append(tmp);
}


String::String(int                value) : string(ToString(value)) {}
String::String(unsigned int       value) : string(ToString(value)) {}
String::String(long               value) : string(ToString(value)) {}
String::String(unsigned long      value) : string(ToString(value)) {}
String::String(long long          value) : string(ToString(value)) {}
String::String(unsigned long long value) : string(ToString(value)) {}

// TODO: Впилить толковые строки
static uint32_t __to_lower(uint32_t code_point)
{
	if (code_point >= 'A' && code_point <= 'Z') {
		return code_point - 'A' + 'a';
	}
	if (code_point == 0x0401)
		return 0x0451;
	if (code_point >= 0x0410 && code_point <= 0x042F)
		return code_point - 0x0410 + 0x430;
	return code_point;
}

// TODO: Впилить толковые строки
static uint32_t __to_upper(uint32_t code_point)
{
	if (code_point >= 'a' && code_point <= 'z') {
		return code_point - 'a' + 'A';
	}
	if (code_point == 0x0451)
		return 0x0401;
	if (code_point >= 0x0430 && code_point <= 0x044F)
		return code_point - 0x0430 + 0x410;
	return code_point;
}

/** Функция перевода каждого символа в строчные (C strings).
 *      @param        - нет
 *      Переводит символы в строчные
 */
String String::UpperCase() const
{
	std::vector<char> result_data(size() + 1);

	auto cstr = c_str();
	utf8::iterator<const char*> it (cstr, cstr, cstr + size()),
		eit(cstr + size(), cstr, cstr + size());

	auto rit = result_data.data();

	while (it != eit) {
		rit = utf8::append(__to_upper(*it++), rit);
	}

	return String(result_data.data(), size());
}

/** Функция перевода каждого символа в нижний регистр (C strings).
 *      @param        - нет
 *      Переводит символы в нижний регистр
 */
String String::LowerCase() const
{
	std::vector<char> result_data(size());

	auto cstr = c_str();
	utf8::iterator<const char*> it (cstr, cstr, cstr + size()),
			eit(cstr + size(), cstr, cstr + size());

	auto rit = result_data.data();

	while (it != eit) {
		rit = utf8::append(__to_lower(*it++), rit);
	}

	return String(result_data.data(), size());
}

/** Функция определяет пустая строка или нет.
 *      @param        - нет
 *
 */
bool String::IsEmpty() const
{
	return empty();
}

/** Функция возвращает длину строки.
 *      @param        - нет
 *
 */
int String::Length() const
{
	return size();
}

/** Функция сравнивает строки.
 *      @const String &b - строка для сравнения
 *
 */
int String::CompareIC(const String &b) const
{
	return LowerCase().compare(b.LowerCase());
}

/** Функция сравнивает строки.
 *      @const String &b - строка для сравнения
 *
 */
int String::Compare(const String &b) const
{
	return compare(b);
}

/** Функция заменяет подстроки в строке.
 *      @const String &b - строка для сравнения
 *
 */
String String::Replace(const String &sub, const String &replace) const
{
	string _copy(*this);

	auto pos = _copy.find(sub);

	while (pos != string::npos) {

		_copy.replace(pos, sub.size(), replace);
		pos += replace.size();
		pos = _copy.find(sub, pos);

	}

	return _copy;
}

/** Функция выделяет подстроку в строке.
 *      @int StartIndex - начальный индекс
 *      @int Count      - количество символов
 *
 */
String String::SubString(int StartIndex, int Count) const
{
	if (StartIndex > Length() || StartIndex <= 0) {

		return String("");

	}

	return String(substr(StartIndex - 1, Count));
}

/** Функция позволяет обращаться со строкой как с массивом, доступ по индексу.
 *      @int index - индекс символа
 *
 */
char &String::operator[] (int index)
{
	return at(index - 1);
}

/** Функция позволяет обращаться со строкой как с массивом, доступ по индексу.
 *      @int index - индекс символа
 *
 */
const char &String::operator[] (int index) const
{
	return at(index - 1);
}

/** Функция позволяет преобразовать строку в число int.
 *      @param - нет
 *
 */
int String::ToInt() const
{
	return stoi(*this);
}

/** Функция позволяет преобразовать строку в число int.
 *      @int default_value - значение по умолчанию
 *
 */
int String::ToIntDef(int default_value) const
{
	try {
		return ToInt();
	} catch (const invalid_argument &) {
		return default_value;
	}
}

/** Функция ищет подстроку в строке.
 *      @const String &substr - подстрока поиска
 *
 */
int String::Pos(const String &substr)
{
	auto index = find(substr);
	if (index == npos) {
		return 0;
	}
	return index + 1;
}

/** Функция возвращает длину строки.
 *      @params - нет
 *
 */
int String::GetLength() const
{
	return this->size();
}

/** Функция устанавливает длину строки.
 *      @int NewLength - новая длина строки
 *
 */
void String::SetLength(int NewLength)
{
	resize(NewLength);
}

/** Функция ищет вхождение последнего разделителя в строке.
 *      @const String &delimiters - разделитель который ищем
 *
 */
int String::LastDelimiter(const String &delimiters) const
{
	return 0; // TODO: реализовать функцию поиска последнего разделителя
}

/** Функция ищет вхождение последнего символа в строке.
 *      @params - нет
 *
 */
const char *String::LastChar() const
{
	if (size() == 0) {
		return nullptr;
	}
	return c_str() + (size() - 1);
}

/** Функция возвращает размер широких символов юникод.
 *      @params - нет
 *
 */
int String::WideCharBufSize() const
{
	return sizeof(wchar_t) * (size() + 1);
}

/** Функция возвращает широкую строку символов юникод.
 *      @WCHART *wbuf - буфер
 *      @int destsize - размер
 *
 */
WCHART *String::WideChar(WCHART *wbuf, int destSize) const
{
	bool limit_exceeded = false;
	for (size_t i = 0; i < size() && !limit_exceeded; i++) {
		wbuf[i] = c_str()[i];
		if (destSize) {
			limit_exceeded = --destSize == 0;
		}
	}
	return wbuf;
}

/** Метод добавляющий в списко строк данные.
 *      @const String &item - строка для добавления
 *
 */
void TStringList::Add(const String &item)
{
	push_back(item);
}

/** Метод устанавливает значение в списке строк.
 *      @const String &text - строка для добавления
 *
 */
void TStringList::SetText(const String &text)
{
	clear();
	stringstream iss(text);
	string line;
	while (getline(iss, line)) {
		push_back(line);
	}
}

/** Метод возвращает размер списка строк.
 *      @params - нет
 *
 */
int TStringList::Count() const
{
	return size();
}

/** Метод удаляет элемент списка по индексу.
 *      @int index - индекс удаляемого элемента
 *
 */
void TStringList::Delete(int index)
{
	erase(begin() + index);
}

/** Метод загружает данные из файла в список строк.
 *      @const String &filename - имя файла для загрузки строк
 *
 */
void TStringList::LoadFromFile(const String &filename)
{
	clear();
	boost::filesystem::ifstream ifs(boost::filesystem::path(filename.c_str()));
	string line;
	while (getline(ifs, line)) {
		push_back(line);
	}
}

/** Метод возвращает строку преобразования из числа.
 *      @int n - число для преобразования
 *
 */
String String::IntToStr(int n)
{
	return to_string(n);
}

/** Метод возвращает строку преобразования из числа.
 *      @unsigned int n - число для преобразования
 *
 */
String String::UIntToStr(unsigned int n)
{
	return to_string(n);
}

/** Метод возвращает строку преобразования из числа.
 *      @unsigned long n - число для преобразования
 *
 */
String String::ULongToStr(unsigned long n)
{
	return to_string(n);
}

/** Метод возвращает строку преобразования из числа.
 *      @long n - число для преобразования
 *
 */
String String::LongToStr(long n)
{
	return to_string(n);
}

/** Метод возвращает строку преобразования из числа.
 *      @unsigned long long n - число для преобразования
 *
 */
String String::ULongLongToStr(unsigned long long n)
{
	return to_string(n);
}

/** Метод возвращает строку преобразования из числа.
 *      @long long n - число для преобразования
 *
 */
String String::LongLongToStr(long long n)
{
	return to_string(n);
}

/** Метод возвращает строку преобразования из числа.
 *      @int n - число для преобразования
 *
 */
String String::ToString(int n)
{
	return IntToStr(n);
}
/** Метод возвращает строку преобразования из числа.
 *      @unsigned int n - число для преобразования
 *
 */
String String::ToString(unsigned int n)
{
	return UIntToStr(n);
}

/** Метод возвращает строку преобразования из числа.
 *      unsigned long n - число для преобразования
 *
 */
String String::ToString(unsigned long n)
{
	return ULongToStr(n);
}

/** Метод возвращает строку преобразования из числа.
 *      @long n - число для преобразования
 *
 */
String String::ToString(long n)
{
	return LongToStr(n);
}

/** Метод возвращает строку преобразования из числа.
 *      @unsigned long long n - число для преобразования
 *
 */
String String::ToString(unsigned long long n)
{
	return ULongLongToStr(n);
}

/** Метод возвращает строку преобразования из числа.
 *      @long long n - число для преобразования
 *
 */
String String::ToString(long long n)
{
	return LongLongToStr(n);
}

/** Метод возвращает строку преобразования из числа
 *    десятичного в число шестнадцатеричное
 *      @int n - число для преобразования
 *      @int digits - число цифр
 */
String String::IntToHex(int n, int digits)
{
	stringstream ss;
	ss << hex;
	if (digits != 0) {
		ss << setfill('0') << setw(digits);
	}
	ss << n;

	return String(ss.str());
}


} // System

