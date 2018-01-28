//
// Created by dmpas on 21.01.18.
//

#ifndef TOOL1CD_PROJECT_BINARYGUID_H
#define TOOL1CD_PROJECT_BINARYGUID_H

#include <array>
#include <string>

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
private:
	std::array<uint8_t , GUID_BINARY_SIZE> data;
};


#endif //TOOL1CD_PROJECT_BINARYGUID_H
