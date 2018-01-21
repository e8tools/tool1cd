//
// Created by dmpas on 21.01.18.
//

#ifndef TOOL1CD_PROJECT_BINARYGUID_H
#define TOOL1CD_PROJECT_BINARYGUID_H

#include <array>

const int GUID_BINARY_SIZE = 16;

// Обёртка над двоичным представлением GUID-записи в таблице
class BinaryGuid {
public:
	BinaryGuid();
	BinaryGuid(const char *src_data);
	BinaryGuid(const BinaryGuid &src);

	bool operator == (const BinaryGuid &b) const;
	bool operator != (const BinaryGuid &b) const;

	bool is_empty() const;
private:
	std::array<char, GUID_BINARY_SIZE> data;
};


#endif //TOOL1CD_PROJECT_BINARYGUID_H
