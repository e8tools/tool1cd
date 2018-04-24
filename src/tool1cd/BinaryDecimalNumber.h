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
