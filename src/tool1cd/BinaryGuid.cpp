//
// Created by dmpas on 21.01.18.
//

#include "BinaryGuid.h"

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