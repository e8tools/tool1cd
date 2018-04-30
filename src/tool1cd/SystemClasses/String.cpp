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

