
#pragma hdrstop

#include "Common.h"
//---------------------------------------------------------------------------
#if !defined(_WIN32)
#pragma package(smart_init)
#endif

const wchar_t hexdecode[] = L"0123456789abcdef";

//---------------------------------------------------------------------------
void time1CD_to_FileTime(FILETIME* ft, unsigned char* time1CD)
{
	SYSTEMTIME st;
	FILETIME lft;
	st.wYear = (time1CD[0] >> 4) * 1000 + (time1CD[0] & 0xf) * 100 + (time1CD[1] >> 4) * 10 + (time1CD[1] & 0xf);
	st.wMonth = (time1CD[2] >> 4) * 10 + (time1CD[2] & 0xf);
	st.wDay = (time1CD[3] >> 4) * 10 + (time1CD[3] & 0xf);
	st.wHour = (time1CD[4] >> 4) * 10 + (time1CD[4] & 0xf);
	st.wMinute = (time1CD[5] >> 4) * 10 + (time1CD[5] & 0xf);
	st.wSecond = (time1CD[6] >> 4) * 10 + (time1CD[6] & 0xf);
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
String GUIDas1C(const unsigned char* fr)
{
	int i, j;
	WCHART buf[37];
	WCHART sym;

	j = 0;
	for(i = 12; i < 16; i++)
	{
		sym = L'0' + (fr[i] >> 4);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
		sym = L'0' + (fr[i] & 0xf);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
	}
	buf[j++] = '-';
	for(i = 10; i < 12; i++)
	{
		sym = L'0' + (fr[i] >> 4);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
		sym = L'0' + (fr[i] & 0xf);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
	}
	buf[j++] = '-';
	for(i = 8; i < 10; i++)
	{
		sym = L'0' + (fr[i] >> 4);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
		sym = L'0' + (fr[i] & 0xf);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
	}
	buf[j++] = '-';
	for(i = 0; i < 2; i++)
	{
		sym = L'0' + (fr[i] >> 4);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
		sym = L'0' + (fr[i] & 0xf);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
	}
	buf[j++] = '-';
	for(i = 2; i < 8; i++)
	{
		sym = L'0' + (fr[i] >> 4);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
		sym = L'0' + (fr[i] & 0xf);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
	}
	buf[j] = 0;
	return String(buf);
}

//---------------------------------------------------------------------------
String GUIDasMS(const unsigned char* fr)
{
	int i, j;
	WCHART buf[37];
	WCHART sym;

	j = 0;
	for(i = 3; i >= 0; i--)
	{
		sym = L'0' + (fr[i] >> 4);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
		sym = L'0' + (fr[i] & 0xf);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
	}
	buf[j++] = '-';
	for(i = 5; i >= 4; i--)
	{
		sym = L'0' + (fr[i] >> 4);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
		sym = L'0' + (fr[i] & 0xf);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
	}
	buf[j++] = '-';
	for(i = 7; i >= 6; i--)
	{
		sym = L'0' + (fr[i] >> 4);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
		sym = L'0' + (fr[i] & 0xf);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
	}
	buf[j++] = '-';
	for(i = 8; i < 10; i++)
	{
		sym = L'0' + (fr[i] >> 4);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
		sym = L'0' + (fr[i] & 0xf);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
	}
	buf[j++] = '-';
	for(i = 10; i < 16; i++)
	{
		sym = L'0' + (fr[i] >> 4);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
		sym = L'0' + (fr[i] & 0xf);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
	}
	buf[j] = 0;
	//return buf;
	return ::String(buf, 37);
}

//---------------------------------------------------------------------------
String GUID_to_string(const TGUID& guid)
{
	return GUIDas1C((unsigned char*)&guid);
}

//---------------------------------------------------------------------------
bool string_to_GUID(const String& str, TGUID* guid)
{
	int i,j;

	unsigned char* g = (unsigned char*)guid;
	wchar_t hi,lo;

	memset(guid, 0, sizeof(TGUID));

	bool res = true;
	if(str.Length() != 36) res = false;
	else
	{
		j = 1;
		for(i = 12; i < 16; i++)
		{
			hi = str[j++];
			lo = str[j++];
			res = res && two_hex_digits_to_byte(hi, lo, g[i]);
		}
		res = res && (str[j++] == L'-');
		for(i = 10; i < 12; i++)
		{
			hi = str[j++];
			lo = str[j++];
			res = res && two_hex_digits_to_byte(hi, lo, g[i]);
		}
		res = res && (str[j++] == L'-');
		for(i = 8; i < 10; i++)
		{
			hi = str[j++];
			lo = str[j++];
			res = res && two_hex_digits_to_byte(hi, lo, g[i]);
		}
		res = res && (str[j++] == L'-');
		for(i = 0; i < 2; i++)
		{
			hi = str[j++];
			lo = str[j++];
			res = res && two_hex_digits_to_byte(hi, lo, g[i]);
		}
		res = res && (str[j++] == L'-');
		for(i = 2; i < 8; i++)
		{
			hi = str[j++];
			lo = str[j++];
			res = res && two_hex_digits_to_byte(hi, lo, g[i]);
		}

	}

	return res;
}

//---------------------------------------------------------------------------
String GUID_to_string_flat(TGUID* guid)
{
	int i,j;

	WCHART buf[33];
	WCHART sym;
	unsigned char* g = (unsigned char*)guid;

	j = 0;
	for(i = 0; i < 16; i++)
	{
		sym = L'0' + (g[i] >> 4);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
		sym = L'0' + (g[i] & 0xf);
		if(sym > L'9') sym += (L'a' - L'9' - 1);
		buf[j++] = sym;
	}
	buf[j] = 0;

	return String(buf);
}

//---------------------------------------------------------------------------
bool string_to_GUID_flat(const String& str, TGUID* guid)
{
	int i,j;

	unsigned char* g = (unsigned char*)guid;
	wchar_t hi,lo;

	bool res = true;
	if(str.Length() != 32) res = false;
	else
	{
		j = 1;
		for(i = 0; i < 16; i++)
		{
			hi = str[j++];
			lo = str[j++];
			res = res || two_hex_digits_to_byte(hi, lo, g[i]);
		}
	}

	return res;
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
bool string1C_to_date(const String& str, unsigned char* bytedate)
{
	bytedate[0] = ((str[1] - L'0') << 4) + (str[2] - L'0');
	bytedate[1] = ((str[3] - L'0') << 4) + (str[4] - L'0');
	bytedate[2] = ((str[5] - L'0') << 4) + (str[6] - L'0');
	bytedate[3] = ((str[7] - L'0') << 4) + (str[8] - L'0');
	bytedate[4] = ((str[9] - L'0') << 4) + (str[10] - L'0');
	bytedate[5] = ((str[11] - L'0') << 4) + (str[12] - L'0');
	bytedate[6] = ((str[13] - L'0') << 4) + (str[14] - L'0');
	return true;
}

//---------------------------------------------------------------------------
// dd.mm.yyyy hh:mm:ss -> char[7]
bool string_to_date(const String& str, unsigned char* bytedate)
{
	bytedate[3] = ((str[1] - L'0') << 4) + (str[2] - L'0');
	bytedate[2] = ((str[4] - L'0') << 4) + (str[5] - L'0');
	bytedate[0] = ((str[7] - L'0') << 4) + (str[8] - L'0');
	bytedate[1] = ((str[9] - L'0') << 4) + (str[10] - L'0');
	bytedate[4] = ((str[12] - L'0') << 4) + (str[13] - L'0');
	bytedate[5] = ((str[15] - L'0') << 4) + (str[16] - L'0');
	bytedate[6] = ((str[18] - L'0') << 4) + (str[19] - L'0');
	return true;
}

//---------------------------------------------------------------------------
// char[7] -> yyyymmddhhmmss
String date_to_string1C(const unsigned char* bytedate)
{
	WCHART buf[15];

	buf[0] = L'0' + (bytedate[0] >> 4);
	buf[1] = L'0' + (bytedate[0] & 0xf);
	buf[2] = L'0' + (bytedate[1] >> 4);
	buf[3] = L'0' + (bytedate[1] & 0xf);
	buf[4] = L'0' + (bytedate[2] >> 4);
	buf[5] = L'0' + (bytedate[2] & 0xf);
	buf[6] = L'0' + (bytedate[3] >> 4);
	buf[7] = L'0' + (bytedate[3] & 0xf);
	buf[8] = L'0' + (bytedate[4] >> 4);
	buf[9] = L'0' + (bytedate[4] & 0xf);
	buf[10] = L'0' + (bytedate[5] >> 4);
	buf[11] = L'0' + (bytedate[5] & 0xf);
	buf[12] = L'0' + (bytedate[6] >> 4);
	buf[13] = L'0' + (bytedate[6] & 0xf);
	buf[14] = 0;
	return String(buf);

}

//---------------------------------------------------------------------------
// char[7] -> dd.mm.yyyy hh:mm:ss
String date_to_string(const unsigned char* bytedate)
{
	WCHART buf[20];

	buf[0] = L'0' + (bytedate[3] >> 4);
	buf[1] = L'0' + (bytedate[3] & 0xf);
	buf[2] = L'.';
	buf[3] = L'0' + (bytedate[2] >> 4);
	buf[4] = L'0' + (bytedate[2] & 0xf);
	buf[5] = L'.';
	buf[6] = L'0' + (bytedate[0] >> 4);
	buf[7] = L'0' + (bytedate[0] & 0xf);
	buf[8] = L'0' + (bytedate[1] >> 4);
	buf[9] = L'0' + (bytedate[1] & 0xf);
	buf[10] = L' ';
	buf[11] = L'0' + (bytedate[4] >> 4);
	buf[12] = L'0' + (bytedate[4] & 0xf);
	buf[13] = L':';
	buf[14] = L'0' + (bytedate[5] >> 4);
	buf[15] = L'0' + (bytedate[5] & 0xf);
	buf[16] = L':';
	buf[17] = L'0' + (bytedate[6] >> 4);
	buf[18] = L'0' + (bytedate[6] & 0xf);
	buf[19] = 0;
	return String(buf);

}

//---------------------------------------------------------------------------
String hexstring(char* buf, int n)
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
String tohex(int n)
{
	return String("0x") + String::IntToHex(n, 0);
}

//---------------------------------------------------------------------------
String tohex64(int64_t n)
{
	int nl;
	int nh;
	nh = *(int*)(((char*)&n) + 4);
	nl = *(int*)&n;
	if(nh) return String("0x") + String::IntToHex(nh, 0) + String::IntToHex(nl, 8);
	return String("0x") + String::IntToHex(nl, 0);
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

