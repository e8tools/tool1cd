
#ifndef APIcfBaseH
#define APIcfBaseH

#include <System.Classes.hpp>
#include <limits>

#ifdef _MSC_VER

	#include <sys/utime.h>

#else

	#include <sys/types.h>
	#include <utime.h>

#endif // _MSC_VER

using namespace System;

const char str_cfu[] = ".cfu";
const char str_cfe[] = ".cfe";
const char str_cf[]  = ".cf";
const char str_epf[] = ".epf";
const char str_erf[] = ".erf";
const char str_backslash[] = "\\";

// шаблон заголовка блока
const char _BLOCK_HEADER_TEMPLATE[] = "\r\n00000000 00000000 00000000 \r\n";
const char _EMPTY_CATALOG_TEMPLATE[16] = {'\xff','\xff','\xff','\x7f',0,2,0,0,0,0,0,0,0,0,0,0};

const int32_t LAST_BLOCK = std::numeric_limits<int>::max();
const uint32_t BLOCK_HEADER_LEN = 32U;
const uint32_t CATALOG_HEADER_LEN = 16U;

const int64_t EPOCH_START_WIN = 504911232000000;

constexpr uint32_t HEX_INT_LEN = sizeof(int32_t) * 2;

//---------------------------------------------------------------------------
struct v8header_struct{
	int64_t time_create;
	int64_t time_modify;
	int32_t zero;
};

//---------------------------------------------------------------------------
struct catalog_header {
	int32_t start_empty; // начало первого пустого блока
	int32_t page_size;   // размер страницы по умолчанию
	int32_t version;     // версия
	int32_t zero;        // всегда ноль?
};

enum class block_header: int {
	doc_len = 2,
	block_len = 11,
	nextblock = 20
};

unsigned WindowsTickToUnixSeconds(long long windowsTicks);
void V8timeToFileTime(const int64_t* v8t, FILETIME* ft);
void FileTimeToV8time(const FILETIME* ft, int64_t* v8t);
void setCurrentTime(int64_t* v8t);

#endif

