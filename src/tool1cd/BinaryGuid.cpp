//
// Created by dmpas on 21.01.18.
//

#include "BinaryGuid.h"
#include "Common.h"

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
	if (presentation.size() != 36) {
		throw std::exception(); // TODO: Внятное сообщение
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