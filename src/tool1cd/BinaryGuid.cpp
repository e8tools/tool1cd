//
// Created by dmpas on 21.01.18.
//

#include "BinaryGuid.h"
#include "Common.h"
#include "DetailedException.h"

BinaryGuid::BinaryGuid()
{
	int i = GUID_BINARY_SIZE;
	while (i--) {
		data[i] = '\0';
	}
}

BinaryGuid::BinaryGuid(const char *src_data)
{
	int i = GUID_BINARY_SIZE;
	while (i--) {
		data[i] = src_data[i];
	}
}

BinaryGuid::BinaryGuid(const BinaryGuid &src)
{
	int i = GUID_BINARY_SIZE;
	while (i--) {
		data[i] = src.data[i];
	}
}

bool BinaryGuid::operator == (const BinaryGuid &b) const
{
	int i = GUID_BINARY_SIZE;
	while (i--) {
		if (data[i] != b.data[i]) {
			return false;
		}
	}
	return true;
}

bool BinaryGuid::operator != (const BinaryGuid &b) const
{
	return ! operator==(b);
}

bool BinaryGuid::is_empty() const
{
	int i = GUID_BINARY_SIZE;
	while (i--) {
		if (data[i] != '\0') {
			return false;
		}
	}
	return true;
}

BinaryGuid::BinaryGuid(const std::string &presentation)
{
	if (presentation.size() != GUID_LEN) {
		throw DetailedException("Переданный параметр не может быть распознана как GUID")
				.add_detail("Значение параметра", presentation);
	}
	int j = 0;
	for (int ind = 12; ind < 16; ind++) {
		data[ind] = (from_hex_digit(presentation[j++]) << 4) + from_hex_digit(presentation[j++]);
	}
	j++;
	for (int ind = 10; ind < 12; ind++) {
		data[ind] = (from_hex_digit(presentation[j++]) << 4) + from_hex_digit(presentation[j++]);
	}
	j++;
	for (int ind = 8; ind < 10; ind++) {
		data[ind] = (from_hex_digit(presentation[j++]) << 4) + from_hex_digit(presentation[j++]);
	}
	j++;
	for (int ind = 0; ind < 2; ind++) {
		data[ind] = (from_hex_digit(presentation[j++]) << 4) + from_hex_digit(presentation[j++]);
	}
	j++;
	for (int ind = 2; ind < 8; ind++) {
		data[ind] = (from_hex_digit(presentation[j++]) << 4) + from_hex_digit(presentation[j++]);
	}
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
std::string GUIDas1C(const unsigned char* fr)
{
	int i, j;
	WCHART buf[GUID_LEN+1];
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

	return String(buf, GUID_LEN+1);
}

//---------------------------------------------------------------------------
std::string GUIDasMS(const unsigned char* fr)
{
	int i, j;
	WCHART buf[GUID_LEN+1];
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

	return String(buf, GUID_LEN+1);
}

//---------------------------------------------------------------------------
std::string GUID_to_string(const TGUID& guid)
{
	return GUIDas1C((unsigned char*)&guid);
}


std::string BinaryGuid::as_MS() const
{
	// TODO: Перенести логику сюда
	return GUIDasMS((const unsigned char*)data.data());
}

bool BinaryGuid::operator >(const BinaryGuid &b) const
{
	for (int i = 0; i < GUID_BINARY_SIZE; i++) {
		if (data[i] > b.data[i]) {
			return true;
		}
		if (data[i] < b.data[i]) {
			return false;
		}
	}
	return false;
}