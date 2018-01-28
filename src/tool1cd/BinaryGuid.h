//
// Created by dmpas on 21.01.18.
//

#ifndef TOOL1CD_PROJECT_BINARYGUID_H
#define TOOL1CD_PROJECT_BINARYGUID_H

#include <array>
#include <string>
#include <System.hpp>

const int GUID_BINARY_SIZE = 16;
const size_t GUID_LEN = 36;

// Обёртка над двоичным представлением GUID-записи в таблице
class BinaryGuid {
public:
	BinaryGuid();
	BinaryGuid(const char *src_data);
	BinaryGuid(const BinaryGuid &src);
	explicit BinaryGuid(const std::string &presentation);

	bool operator == (const BinaryGuid &b) const;
	bool operator != (const BinaryGuid &b) const;
	bool operator >  (const BinaryGuid &b) const;

	bool is_empty() const;

	// TODO: Мудрое название
	std::string as_MS() const;
	std::string as_1C() const;
private:
	std::array<uint8_t , GUID_BINARY_SIZE> data;
};


std::string GUIDas1C(const unsigned char* fr);
std::string GUIDasMS(const unsigned char* fr);
std::string GUID_to_string(const BinaryGuid& guid);
bool string_to_GUID(const String& str, BinaryGuid* guid);

#endif //TOOL1CD_PROJECT_BINARYGUID_H
