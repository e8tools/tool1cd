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

#ifndef TOOL1CD_PROJECT_BINARYGUID_H
#define TOOL1CD_PROJECT_BINARYGUID_H

#include <array>
#include <string>
#include <System.hpp>

const int GUID_BINARY_SIZE = 16;
const size_t GUID_LEN = 36;

// Обёртка над двоичным представлением GUID-записи в таблице
class BinaryGuid {
public:
	BinaryGuid();
	explicit BinaryGuid(const char *src_data);
	BinaryGuid(const BinaryGuid &src);
	explicit BinaryGuid(const std::string &presentation);

	bool operator == (const BinaryGuid &b) const;
	bool operator != (const BinaryGuid &b) const;
	bool operator >  (const BinaryGuid &b) const;

	bool is_empty() const;

	// TODO: Мудрое название
	std::string as_MS() const;
	std::string as_1C() const;
private:
	std::array<uint8_t , GUID_BINARY_SIZE> data;
};


std::string GUIDas1C(const unsigned char* fr);
std::string GUIDasMS(const unsigned char* fr);
std::string GUID_to_string(const BinaryGuid& guid);
bool string_to_GUID(const std::string &str, BinaryGuid *guid);

#endif //TOOL1CD_PROJECT_BINARYGUID_H
