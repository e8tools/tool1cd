#include "Common.h"
#include "MessageRegistration.h"
#include "BinaryDecimalNumber.h"
//---------------------------------------------------------------------------
#if !defined(_WIN32)
#pragma package(smart_init)
#endif

using namespace System;
using namespace std;

const wchar_t hexdecode[] = L"0123456789abcdef";

extern Registrator msreg_g;

//---------------------------------------------------------------------------
void time1CD_to_FileTime(FILETIME *ft, const void *time1CD)
{
	SYSTEMTIME st;
	FILETIME lft;
	BinaryDecimalDate bdd(time1CD);
	st.wYear = bdd.get_year();
	st.wMonth = bdd.get_month();
	st.wDay = bdd.get_day();
	st.wHour = bdd.get_hour();
	st.wMinute = bdd.get_minute();
	st.wSecond = bdd.get_second();
	SystemTimeToFileTime(&st, &lft);
	LocalFileTimeToFileTime(&lft, ft);
}

//---------------------------------------------------------------------------
unsigned int reverse_byte_order(unsigned int value)
{
	unsigned int ret;
	((char*)(&ret))[0] = ((char*)(&value))[3];
	((char*)(&ret))[1] = ((char*)(&value))[2];
	((char*)(&ret))[2] = ((char*)(&value))[1];
	((char*)(&ret))[3] = ((char*)(&value))[0];
	return ret;
}

//---------------------------------------------------------------------------
bool two_hex_digits_to_byte(const wchar_t hi, const wchar_t lo, unsigned char& res)
{
	if(hi >= L'0' && hi <=L'9') res = (hi - L'0') << 4;
	else if(hi >= L'a' && hi <=L'f') res = (hi - (L'a' - 0xa)) << 4;
	else if(hi >= L'A' && hi <=L'F') res = (hi - (L'A' - 0xa)) << 4;
	else{
		res = 0;
		return false;
	}

	if(lo >= L'0' && lo <=L'9') res += (lo - L'0');
	else if(lo >= L'a' && lo <=L'f') res += (lo - (L'a' - 0xa));
	else if(lo >= L'A' && lo <=L'F') res += (lo - (L'A' - 0xa));
	else{
		res = 0;
		return false;
	}

	return true;

}


//---------------------------------------------------------------------------
// yyyymmddhhmmss -> char[7]
bool string1C_to_date(const String &str, void *bytedate)
{
	BinaryDecimalDate bdd(str, "yyyyMMddhhmmss");
	bdd.write_to(bytedate);
	return true;
}

//---------------------------------------------------------------------------
// dd.mm.yyyy hh:mm:ss -> char[7]
bool string_to_date(const String &str, void *bytedate)
{
	BinaryDecimalDate bdd(str);
	bdd.write_to(bytedate);
	return true;
}

//---------------------------------------------------------------------------
// char[7] -> yyyymmddhhmmss
String date_to_string1C(const void *bytedate)
{
	BinaryDecimalDate bdd(bytedate);
	return bdd.get_part(0, 14);
}

//---------------------------------------------------------------------------
// char[7] -> dd.mm.yyyy hh:mm:ss
String date_to_string(const void *bytedate)
{
	BinaryDecimalDate bdd(bytedate);
	return bdd.get_presentation();
}

//---------------------------------------------------------------------------
String hexstring(const char *buf, int n)
{
	int i;
	String s = "";
	wchar_t b;
	unsigned char c;

	for(i = 0; i < n; i++)
	{
		c = ((unsigned char*)buf)[i];
		c >>= 4;
		b = hexdecode[c];
		s += b;
		c = ((unsigned char*)buf)[i];
		c &= 0xf;
		b = hexdecode[c];
		s += b;
		if(i < n - 1) s += " ";
	}

	return s;
}

//---------------------------------------------------------------------------
String hexstring(TStream* str)
{
	String s = "";
	wchar_t b;
	unsigned char c, d;

	while(str->Read(&d, 1))
	{
		c = d >> 4;
		b = hexdecode[c];
		s += b;
		c = d & 0xf;
		b = hexdecode[c];
		s += b;
	}

	return s;
}

//---------------------------------------------------------------------------
std::string toXML(const std::string &in)
{
	string result(in);
	result = StringReplace(result, "&", "&amp;");
	result = StringReplace(result, "<", "&lt;");
	result = StringReplace(result, ">", "&gt;");
	result = StringReplace(result, "'", "&apos;");
	result = StringReplace(result, "\"", "&quot;");
	return result;
}

//---------------------------------------------------------------------------
unsigned char from_hex_digit(char digit)
{
	if(digit >= '0' && digit <= '9') return digit - '0';
	if(digit >= 'a' && digit <= 'f') return digit - 'a' + 10;
	if(digit >= 'A' && digit <= 'F') return digit - 'A' + 10;
	return 0;
}

bool directory_exists(boost::filesystem::path& check_path, bool create_directory) {
	if(!boost::filesystem::exists(check_path)) {
		if(create_directory) {
			boost::filesystem::create_directory(check_path);
			return true;
		}
		else {
			msreg_g.AddError("Каталог не существует.").with("Каталог", check_path.string());
			return false;
		}
	} else if(!boost::filesystem::is_directory(check_path)) {
		msreg_g.AddError("Указанный путь не является каталогом.").with("Каталог", check_path.string());
		return false;
	}

	return true;
}
