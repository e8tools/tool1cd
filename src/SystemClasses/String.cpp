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
String::String(const std::vector<uint8_t> &bytes, int offset, int size)
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
	std::vector<uint8_t> tmpdata;
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
	std::vector<uint8_t> tmpdata;
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


String::String(int                value) : string(to_string(value)) {}
String::String(unsigned int       value) : string(to_string(value)) {}
String::String(long               value) : string(to_string(value)) {}
String::String(unsigned long      value) : string(to_string(value)) {}
String::String(long long          value) : string(to_string(value)) {}
String::String(unsigned long long value) : string(to_string(value)) {}

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

/** Метод добавляющий в списко строк данные.
 *      @const String &item - строка для добавления
 *
 */
void TStringList::Add(const string &item)
{
	push_back(item);
}

/** Метод устанавливает значение в списке строк.
 *      @const String &text - строка для добавления
 *
 */
void TStringList::SetText(const string &text)
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
void TStringList::LoadFromFile(const string &filename)
{
	clear();
	boost::filesystem::ifstream ifs(boost::filesystem::path(filename.c_str()));
	string line;
	while (getline(ifs, line)) {
		push_back(line);
	}
}

std::string LowerCase(const std::string &src)
{
	std::vector<char> result_data(src.size() + 1);

	auto cstr = src.c_str();
	utf8::iterator<const char*> it (cstr, cstr, cstr + src.size()),
			eit(cstr + src.size(), cstr, cstr + src.size());

	auto rit = result_data.data();

	while (it != eit) {
		rit = utf8::append(__to_lower(*it++), rit);
	}
	result_data.back() = '\0';

	return result_data.data();
}

std::string UpperCase(const std::string &src)
{
	std::vector<char> result_data(src.size() + 1);

	auto cstr = src.c_str();
	utf8::iterator<const char*> it (cstr, cstr, cstr + src.size()),
			eit(cstr + src.size(), cstr, cstr + src.size());

	auto rit = result_data.data();

	while (it != eit) {
		rit = utf8::append(__to_upper(*it++), rit);
	}
	result_data.back() = '\0';

	return string(result_data.data());
}

int Compare(const std::string &a, const std::string &b)
{
	return a.compare(b);
}

int CompareIC(const std::string &a, const std::string &b)
{
	return Compare(LowerCase(a), LowerCase(b));
}

bool EqualIC(const std::string &a, const std::string &b)
{
	return CompareIC(a, b) == 0;
}

bool Equal(const std::string &a, const std::string &b)
{
	return Compare(a, b) == 0;
}

bool EndsWithIC(const std::string &str, const std::string &substr)
{
	if (substr.empty()) {
		return true;
	}
	if (str.size() < substr.size()) {
		return false;
	}
	return CompareIC(str.substr(str.size() - substr.size(), substr.size()), substr) == 0;
}

int ToIntDef(const std::string &s, int default_value)
{
	try {
		return stoi(s);
	} catch (const invalid_argument &) {
		return default_value;
	}
}

} // System

