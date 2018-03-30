
#ifndef APIcfBaseH
#define APIcfBaseH

#include <System.Classes.hpp>
#include <limits>
#include <string>

using namespace System;

const char CFU_STR[] = ".cfu";
const char CFE_STR[] = ".cfe";
const char CF_STR[]  = ".cf";
const char EPF_STR[] = ".epf";
const char ERF_STR[] = ".erf";
const char BACKSLASH_STR[] = "\\";

// шаблон заголовка блока
const char _EMPTY_CATALOG_TEMPLATE[16] = {'\xff','\xff','\xff','\x7f',0,2,0,0,0,0,0,0,0,0,0,0};

const int32_t LAST_BLOCK = std::numeric_limits<int>::max();

//---------------------------------------------------------------------------
struct v8header_struct
{
	int64_t time_create;
	int64_t time_modify;
	int32_t zero;
};

struct stElemAddr
{
	int32_t header_addr;
	int32_t data_addr;
	int32_t fffffff; //всегда 0x7fffffff ?

	constexpr static const uint32_t size()
	{
		return 4 + 4 + 4;
	}

};

struct stFileHeader
{
	int32_t next_page_addr {0}; // начало первого пустого блока
	int32_t page_size {0};      // размер страницы по умолчанию
	int32_t storage_ver {0};    // версия
	int32_t reserved {0};       // всегда 0x00000000 ?

	constexpr static const uint32_t size()
	{
		return 4 + 4 + 4 + 4;
	}
};

class stBlockHeader
{
public:

	stBlockHeader():
		EOL_0D(0xd), EOL_0A(0xa),
		space1(' '), space2(' '), space3(' '),
		EOL2_0D(0xd), EOL2_0A(0xa)
		{}

	static stBlockHeader create(int32_t block_data_size, int32_t get_page_size, int32_t get_next_page_addr);

	constexpr static const uint32_t size()
	{
		return 1 + 1 + 8 + 1 + 8 + 1 + 8 + 1 + 1 + 1;
	}

	bool is_correct() const
	{
		return EOL_0D == 0x0d
			&& EOL_0A == 0x0a
			&& space1 == 0x20
			&& space2 == 0x20
			&& space3 == 0x20
			&& EOL2_0D == 0x0d
			&& EOL2_0A == 0x0a;
	}

	int32_t get_data_size() const;
	void set_data_size(int32_t value);

	int32_t get_page_size() const;
	void set_page_size(int32_t value);

	int32_t get_next_page_addr() const;
	void set_next_page_addr(int32_t value);

	const std::string toString() const
	{
		auto _begin = reinterpret_cast<const char *>(this);
		auto _end = _begin + size();
		std::string result(_begin, _end);
		return result;
	}

private:
	char EOL_0D;
	char EOL_0A;
	char data_size_hex[8] {0};
	char space1;
	char page_size_hex[8] {0};
	char space2;
	char next_page_addr_hex[8] {0};
	char space3;
	char EOL2_0D;
	char EOL2_0A;

};

unsigned WindowsTickToUnixSeconds(long long windowsTicks);

#endif

