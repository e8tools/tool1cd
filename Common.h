//---------------------------------------------------------------------------

#ifndef CommonH
#define CommonH

//#include <vcl.h>
#include <System.Classes.hpp>

//---------------------------------------------------------------------------
void __fastcall time1CD_to_FileTime(FILETIME* ft, unsigned char* time1CD);
unsigned int __fastcall reverse_byte_order(unsigned int value);
String __fastcall GUIDas1C(const unsigned char* fr);
String __fastcall GUIDasMS(const unsigned char* fr);
String __fastcall GUID_to_string(const TGUID& guid);
bool __fastcall string_to_GUID(const String& str, TGUID* guid);
String __fastcall GUID_to_string_flat(TGUID* guid);
bool __fastcall string_to_GUID_flat(const String& str, TGUID* guid);
bool __fastcall two_hex_digits_to_byte(const wchar_t hi, const wchar_t lo, unsigned char& res);
bool __fastcall string1C_to_date(const String& str, unsigned char* bytedate);
bool __fastcall string_to_date(const String& str, unsigned char* bytedate);
String __fastcall date_to_string1C(const unsigned char* bytedate);
String __fastcall date_to_string(const unsigned char* bytedate);
String __fastcall tohex(int n);
String __fastcall tohex64(__int64 n);
String __fastcall hexstring(char* buf, int n);
String __fastcall hexstring(TStream* str);

//---------------------------------------------------------------------------
#endif

