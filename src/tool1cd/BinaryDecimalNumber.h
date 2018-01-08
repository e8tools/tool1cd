//
// Created by dmpas on 08.01.18.
//

#ifndef TOOL1CD_PROJECT_BINARYDECIMALNUMBER_H
#define TOOL1CD_PROJECT_BINARYDECIMALNUMBER_H

#include <vector>
#include <string>

class BinaryDecimalNumber {
public:

	BinaryDecimalNumber();
	BinaryDecimalNumber(const void *raw_data, int length, int precision, bool has_sign_flag);
	explicit BinaryDecimalNumber(const std::string &presentation, bool has_sign = false, int length = 0, int precision = 0);

	void write_to(void *raw_data) const;

	std::string get_presentation() const;
	std::string get_part(int startIndex, int count) const;

	std::vector<int> get_int() const;
	std::vector<int> get_frac() const;

	bool has_sing_flag {false};
	int precision {0};
	std::vector<int> data;
	int sign;
};

class BinaryDecimalDate : public BinaryDecimalNumber {
public:
	BinaryDecimalDate(const void *raw_data);
	explicit BinaryDecimalDate(const std::string &presentation, const std::string &format = "dd.MM.yyyy hh:mm:ss");

	std::string get_presentation() const;

	int get_year() const;
	int get_month() const;
	int get_day() const;

	int get_hour() const;
	int get_minute() const;
	int get_second() const;
};


#endif //TOOL1CD_PROJECT_BINARYDECIMALNUMBER_H
