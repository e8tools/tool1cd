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
#ifndef SYSTEM_STRING_HPP
#define SYSTEM_STRING_HPP

#include <string>
#include <vector>
#include "System.hpp"
#include <limits>

namespace System {

std::string LowerCase(const std::string &src);
std::string UpperCase(const std::string &src);
int CompareIC(const std::string &a, const std::string &b);
int Compare(const std::string &a, const std::string &b);

bool EndsWithIC(const std::string &str, const std::string &substr);
bool EqualIC(const std::string &a, const std::string &b);
bool Equal(const std::string &a, const std::string &b);

int ToIntDef(const std::string &s, int default_value);

template <typename char_type>
std::string operator + (const std::basic_string<char_type> &text, const int value)
{
	return std::string(text).append(std::to_string(value));
}

class TStringList : public std::vector<std::string>
{
public:

	void Add(const std::string &item);
	void SetText(const std::string &text);
	int Count() const;
	void Delete(int index);

	void LoadFromFile(const std::string &filename);
};


} // System

using System::TStringList;

#endif
