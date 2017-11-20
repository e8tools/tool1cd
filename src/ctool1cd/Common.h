
#ifndef CommonH
#define CommonH

#include <System.Classes.hpp>
#include <sstream>
#include <iomanip>

void time1CD_to_FileTime(FILETIME* ft, unsigned char* time1CD);
unsigned int reverse_byte_order(unsigned int value);
String GUIDas1C(const unsigned char* fr);
String GUIDasMS(const unsigned char* fr);
String GUID_to_string(const TGUID& guid);
bool string_to_GUID(const String& str, TGUID* guid);
String GUID_to_string_flat(TGUID* guid);
bool string_to_GUID_flat(const String& str, TGUID* guid);
bool two_hex_digits_to_byte(const wchar_t hi, const wchar_t lo, unsigned char& res);
bool string1C_to_date(const String& str, unsigned char* bytedate);
bool string_to_date(const String& str, unsigned char* bytedate);
String date_to_string1C(const unsigned char* bytedate);
String date_to_string(const unsigned char* bytedate);
String hexstring(char* buf, int n);
String hexstring(TStream* str);
String toXML(String in);
unsigned char from_hex_digit(char digit);

template< typename T >
String to_hex_string( T num ) {
	std::stringstream stream;
	stream << "0x"
		   << std::setfill('0') << std::setw(sizeof(T))
		   << std::hex << num;
	return String(stream.str());
}

template< typename T >
std::wstring to_hex_wstring( T num ) {
	std::wstringstream stream;
	stream << L"0x"
		   << std::setfill(L'0') << std::setw(sizeof(T))
		   << std::hex << num;
	return stream.str();
}

bool directory_exists(boost::filesystem::path& check_path, bool create_directory = false);

#endif

