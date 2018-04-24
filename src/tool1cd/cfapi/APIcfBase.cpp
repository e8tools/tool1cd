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
#include "../Common.h"
#include "APIcfBase.h"

using namespace std;
//---------------------------------------------------------------------------
// возвращает секунды от эпохи UNIX
// https://stackoverflow.com/questions/6161776/convert-windows-filetime-to-second-in-unix-linux
//
unsigned WindowsTickToUnixSeconds(long long windowsTicks)
{
	return (unsigned)(windowsTicks / WINDOWS_TICK - SEC_TO_UNIX_EPOCH);
}

int32_t stBlockHeader::get_data_size() const {
	string hex("0x");
	hex.append(data_size_hex);

	return std::stoi(hex, nullptr, 16);
}

void stBlockHeader::set_data_size(int32_t value) {
	string hex;

	hex = to_hex_string(value, false);
	std::copy(hex.begin(),
				hex.end(),
				std::begin(data_size_hex));
}

int32_t stBlockHeader::get_page_size() const {
	string hex("0x");
	hex.append(page_size_hex);

	return std::stoi(hex, nullptr, 16);
}

void stBlockHeader::set_page_size(int32_t value) {
	string hex;

	hex = to_hex_string(value, false);
	std::copy(hex.begin(),
				hex.end(),
				std::begin(page_size_hex));
}

int32_t stBlockHeader::get_next_page_addr() const {
	string hex("0x");
	hex.append(next_page_addr_hex);

	return std::stoi(hex, nullptr, 16);
}

void stBlockHeader::set_next_page_addr(int32_t value) {
	string hex;

	hex = to_hex_string(value, false);
	std::copy(hex.begin(),
				hex.end(),
				std::begin(next_page_addr_hex));
}


stBlockHeader stBlockHeader::create(int32_t block_data_size, int32_t page_size, int32_t next_page_addr)
{
	stBlockHeader block_header;

	block_header.set_data_size(block_data_size);
	block_header.set_page_size(page_size);
	block_header.set_next_page_addr(next_page_addr);

	return block_header;
}
