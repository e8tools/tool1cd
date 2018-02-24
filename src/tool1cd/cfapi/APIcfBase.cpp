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

int32_t stBlockHeader::data_size() const {
	string hex("0x");
	hex.append(data_size_hex);

	return std::stoi(hex, nullptr, 16);
}

int32_t stBlockHeader::page_size() const {
	string hex("0x");
	hex.append(page_size_hex);

	return std::stoi(hex, nullptr, 16);
}

int32_t stBlockHeader::next_page_addr() const {
	string hex("0x");
	hex.append(next_page_addr_hex);

	return std::stoi(hex, nullptr, 16);
}
