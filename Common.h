//---------------------------------------------------------------------------

#ifndef CommonH
#define CommonH

//#include <vcl.h>
#include <System.Classes.hpp>

//---------------------------------------------------------------------------
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
String tohex(int n);
String tohex64(int64_t n);
String hexstring(char* buf, int n);
String hexstring(TStream* str);

//---------------------------------------------------------------------------
#endif

