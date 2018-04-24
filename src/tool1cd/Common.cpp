/*
    Tool1CD library provides access to 1CD database files.
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of Tool1CD Library.

    Tool1CD Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tool1CD Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Tool1CD Library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "Common.h"
#include "MessageRegistration.h"
#include "BinaryDecimalNumber.h"
//---------------------------------------------------------------------------
#if !defined(_WIN32)
#pragma package(smart_init)
#endif

using namespace System;
using namespace std;

const char hexdecode[] = "0123456789abcdef";

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
bool two_hex_digits_to_byte(char hi, char lo, unsigned char &res)
{
	if(hi >= '0' && hi <='9') res = (hi - '0') << 4;
	else if(hi >= 'a' && hi <='f') res = (hi - ('a' - 0xa)) << 4;
	else if(hi >= 'A' && hi <='F') res = (hi - ('A' - 0xa)) << 4;
	else{
		res = 0;
		return false;
	}

	if(lo >= '0' && lo <='9') res += (lo - '0');
	else if(lo >= 'a' && lo <='f') res += (lo - ('a' - 0xa));
	else if(lo >= 'A' && lo <='F') res += (lo - ('A' - 0xa));
	else{
		res = 0;
		return false;
	}

	return true;

}


//---------------------------------------------------------------------------
// yyyymmddhhmmss -> char[7]
bool string1C_to_date(const string &str, void *bytedate)
{
	BinaryDecimalDate bdd(str, "yyyyMMddhhmmss");
	bdd.write_to(bytedate);
	return true;
}

//---------------------------------------------------------------------------
// dd.mm.yyyy hh:mm:ss -> char[7]
bool string_to_date(const string &str, void *bytedate)
{
	BinaryDecimalDate bdd(str);
	bdd.write_to(bytedate);
	return true;
}

//---------------------------------------------------------------------------
// char[7] -> yyyymmddhhmmss
string date_to_string1C(const void *bytedate)
{
	BinaryDecimalDate bdd(bytedate);
	return bdd.get_part(0, 14);
}

//---------------------------------------------------------------------------
// char[7] -> dd.mm.yyyy hh:mm:ss
string date_to_string(const void *bytedate)
{
	BinaryDecimalDate bdd(bytedate);
	return bdd.get_presentation();
}

//---------------------------------------------------------------------------
string hexstring(const char *buf, int n)
{
	string result = "";

	for (int i = 0; i < n; i++)
	{
		uint8_t d = ((unsigned char*)buf)[i];
		result.push_back(hexdecode[d >> 4]);
		result.push_back(hexdecode[d & 0xf]);
	}

	return result;
}

//---------------------------------------------------------------------------
string hexstring(TStream *str)
{
	string result = "";
	uint8_t d;

	while (str->Read(&d, 1)) {
		result.push_back(hexdecode[d >> 4]);
		result.push_back(hexdecode[d & 0xf]);
	}

	return result;
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
