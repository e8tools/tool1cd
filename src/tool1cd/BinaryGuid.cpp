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

class WrongGuidPresentation : DetailedException {
public:
	WrongGuidPresentation(const std::string &presentation)
		: DetailedException("Переданный параметр не может быть распознан как GUID")
	{
		add_detail("Значение параметра", presentation);
	}
};

BinaryGuid::BinaryGuid(const std::string &presentation)
{
	if (presentation.size() != GUID_LEN) {
		throw WrongGuidPresentation(presentation);
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
bool string_to_GUID(const std::string &str, BinaryGuid *guid)
{
	try {

		BinaryGuid bguid(str);
		*guid = bguid;

	} catch (WrongGuidPresentation) {
		return false;
	}
	return true;
}

std::string format_guid(const unsigned char *fr, const std::vector<int> &indices)
{
	std::string result;
	const char hex[] = "0123456789abcdef";

	for (int i : indices)
	{
		if (i == -1) {
			result += '-';
		} else {
			result += hex[fr[i] >> 4];
			result += hex[fr[i] & 0xf];
		}
	}

	return result;
}

//---------------------------------------------------------------------------
std::string GUIDas1C(const unsigned char* fr)
{
	std::vector<int> indices = {
			12, 13, 14, 15, -1,
			10, 11, -1,
			8, 9, -1,
			0, 1, -1,
			2, 3, 4, 5, 6, 7};

	return format_guid(fr, indices);
}

//---------------------------------------------------------------------------
std::string GUIDasMS(const unsigned char* fr)
{
	std::vector<int> indices = {
			3, 2, 1, 0, -1,
			5, 4, -1,
			7, 6, -1,
			8, 9, -1,
			10, 11, 12, 13, 14, 15
	};

	return format_guid(fr, indices);
}

//---------------------------------------------------------------------------
std::string GUID_to_string(const BinaryGuid& guid)
{
	return GUIDas1C((unsigned char*)&guid);
}


std::string BinaryGuid::as_MS() const
{
	// TODO: Перенести логику сюда
	return GUIDasMS((const unsigned char*)data.data());
}

std::string BinaryGuid::as_1C() const
{
	return GUIDas1C((const unsigned char*)data.data());
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