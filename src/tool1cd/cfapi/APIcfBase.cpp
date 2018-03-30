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
