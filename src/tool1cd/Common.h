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

#ifndef CommonH
#define CommonH

#include "SystemClasses/TStream.hpp"
#include "SystemClasses/System.hpp"
#include <boost/filesystem.hpp>
#include <sstream>
#include <iomanip>

void time1CD_to_FileTime(System::FILETIME *ft, const void *time1CD);
unsigned int reverse_byte_order(unsigned int value);
bool two_hex_digits_to_byte(char hi, char lo, unsigned char &res);
bool string1C_to_date(const std::string &str, void *bytedate);
bool string_to_date(const std::string &str, void *bytedate);
std::string date_to_string1C(const void *bytedate);
std::string date_to_string(const void *bytedate);
std::string hexstring(const char *buf, int n);
std::string hexstring(TStream *str);
std::string toXML(const std::string &in);
unsigned char from_hex_digit(char digit);

//---------------------------------------------------------------------------
// Структура адреса файла таблицы-контейнера файлов
struct table_blob_file
{
	uint32_t blob_start;
	uint32_t blob_length;
};

template< typename T >
std::string to_hex_string( T num, bool prefix = true ) {
	std::stringstream stream;
	if(prefix) {
		stream << "0x";
	};
	stream << std::setfill('0') << std::setw(sizeof(T) * 2)
		   << std::hex << num;
	return stream.str();
}

bool directory_exists(boost::filesystem::path& check_path, bool create_directory = false);

#endif

