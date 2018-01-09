
#ifndef CommonH
#define CommonH

#include <System.Classes.hpp>
#include <sstream>
#include <iomanip>

void time1CD_to_FileTime(System::FILETIME* ft, unsigned char* time1CD);
unsigned int reverse_byte_order(unsigned int value);
String GUIDas1C(const unsigned char* fr);
String GUIDasMS(const unsigned char* fr);
String GUID_to_string(const System::TGUID& guid);
bool string_to_GUID(const String& str, System::TGUID* guid);
String GUID_to_string_flat(System::TGUID* guid);
bool string_to_GUID_flat(const String& str, System::TGUID* guid);
bool two_hex_digits_to_byte(const wchar_t hi, const wchar_t lo, unsigned char& res);
bool string1C_to_date(const String& str, unsigned char* bytedate);
bool string_to_date(const String& str, unsigned char* bytedate);
String date_to_string1C(const unsigned char* bytedate);
String date_to_string(const void *bytedate);
String hexstring(char* buf, int n);
String hexstring(TStream* str);
String toXML(String in);
unsigned char from_hex_digit(char digit);

template< typename T >
String to_hex_string( T num, bool prefix = true ) {
	std::stringstream stream;
	if(prefix) {
		stream << "0x";
	};
	stream << std::setfill('0') << std::setw(sizeof(T) * 2)
		   << std::hex << num;
	return String(stream.str());
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

