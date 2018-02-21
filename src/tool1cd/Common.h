
#ifndef CommonH
#define CommonH

#include <System.Classes.hpp>
#include <sstream>
#include <iomanip>

void time1CD_to_FileTime(System::FILETIME *ft, const void *time1CD);
unsigned int reverse_byte_order(unsigned int value);
bool two_hex_digits_to_byte(const wchar_t hi, const wchar_t lo, unsigned char& res);
bool string1C_to_date(const String &str, void *bytedate);
bool string_to_date(const String &str, void *bytedate);
String date_to_string1C(const void *bytedate);
String date_to_string(const void *bytedate);
String hexstring(const char *buf, int n);
String hexstring(TStream* str);
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

template< typename T >
std::wstring to_hex_wstring( T num, bool prefix = true ) {
	std::wstringstream stream;
	if(prefix) {
		stream << L"0x";
	};
	stream << std::setfill(L'0') << std::setw(sizeof(T) * 2)
		   << std::hex << num;
	return stream.str();
}

bool directory_exists(boost::filesystem::path& check_path, bool create_directory = false);

#endif

