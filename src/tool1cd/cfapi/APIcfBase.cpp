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

stBlockHeader stBlockHeader::create(uint32_t block_data_size, uint32_t page_size, uint32_t next_page_addr)
{
	stBlockHeader BlockHeader;
	char buf[9];

	sprintf(buf, "%08x", block_data_size);
	strncpy(BlockHeader.data_size_hex, buf, 8);

	sprintf(buf, "%08x", page_size);
	strncpy(BlockHeader.page_size_hex, buf, 8);

	sprintf(buf, "%08x", next_page_addr);
	strncpy(BlockHeader.next_page_addr_hex, buf, 8);

	return BlockHeader;
}
