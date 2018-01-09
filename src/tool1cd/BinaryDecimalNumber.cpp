//
// Created by dmpas on 08.01.18.
//

#include <cstdint>
#include "BinaryDecimalNumber.h"
#include <map>
#include <vector>
#include <array>

class BinaryDecimalBuilder
{
public:
	BinaryDecimalBuilder(std::vector<int> &data)
			: data(data) {}
	std::vector<int> &data;

	void push1(uint8_t lo)
	{
		data.push_back(lo);
	}

	void push2(uint8_t hilo)
	{
		push1(hilo >> 4);
		push1(hilo & 0x0f);
	}
};

BinaryDecimalNumber::BinaryDecimalNumber(const void *raw_data, int length, int precision, bool has_sign_flag)
	: has_sing_flag(has_sign_flag), precision(precision), sign(1)
{
	data.clear();
	BinaryDecimalBuilder builder(data);

	uint8_t *byte_data = (uint8_t*)raw_data;
	uint8_t first_byte = *byte_data;
	if (has_sign_flag) {
		if (first_byte >> 4) {
			sign = 1;
		} else {
			sign = -1;
		}
		builder.push1(first_byte & 0x0f);
	} else {
		builder.push2(first_byte);
	}
	++byte_data;
	int i = 1;
	while (i < length) {
		if (i + 1 < length) {
			builder.push2(*byte_data);
			i += 2;
		} else {
			builder.push1(*byte_data >> 4);
			i++;
		}
		byte_data++;
	}
}

BinaryDecimalNumber::BinaryDecimalNumber(const std::string &presentation, bool has_sign, int length, int precision)
	: has_sing_flag(has_sign), precision(precision), sign(1)
{
	const int INT_PART  = 0;
	const int FRAC_PART = 1;
	std::array< std::vector<int>, 2> parts;
	int part_no = 0;

	for (char c : presentation) {
		if (c == '.') {
			part_no++;
		} else {
			parts[part_no].push_back(c - '0');
		}
	}

	if (precision != 0) {
		parts[FRAC_PART].resize(precision);
	}
	int add_length = 0;
	if (length != 0) {
		add_length = length - parts[INT_PART].size() - parts[FRAC_PART].size();
		if (add_length < 0) {
			parts[INT_PART].clear();
			parts[INT_PART].resize(length - precision, 9); // забиваем девяточками при превышении размера
			add_length = 0;
		}
	}
	while (add_length--) {
		data.push_back(0);
	}

	for (auto part : parts) {
		for (auto num : part) {
			data.push_back(num);
		}
	}
}

std::string BinaryDecimalNumber::get_part(int startIndex, int count) const
{
	std::string result;
	for (int i = startIndex; i < data.size() && count != 0; count--, i++) {
		result += (data[i] + '0');
	}
	return result;
}

std::string BinaryDecimalNumber::get_presentation() const
{
	std::string result;
	if (has_sing_flag) {
		if (sign == -1) {
			result += "-";
		}
	}
	int int_size = data.size() - precision;
	{
		int i = 0;
		while (i < int_size && data[i] == 0) {
			i++;
		}
		if (i < int_size) {
			while (i < int_size) {
				result += '0' + data[i];
				i++;
			}
		} else {
			result += '0';
		}
	}
	if (precision != 0) {
		std::string frac = ".";
		bool has_significant_digits = false;
		int max_significant_size = data.size();
		while (max_significant_size > int_size) {
			if (data[max_significant_size - 1] == 0) {
				max_significant_size--;
			} else {
				break;
			}
		}
		for (int i = int_size; i < max_significant_size; i++) {
			if (data[i] != 0) {
				has_significant_digits = true;
			}
			frac += '0' + data[i];
		}
		if (has_significant_digits) {
			result += frac;
		}
	}
	return result;
}

std::vector<int> BinaryDecimalNumber::get_int() const {
	std::vector<int> result;
	result.assign(data.begin(), data.end() - precision);
	return result;
}

std::vector<int> BinaryDecimalNumber::get_frac() const {
	std::vector<int> result;
	result.assign(data.end() - precision, data.end());
	return result;
}

void BinaryDecimalNumber::write_to(void *raw_data) const
{
	uint8_t *byte_data = (uint8_t *)raw_data;
	auto it = data.begin();
	if (has_sing_flag) {
		*byte_data = (sign == -1 ? 0 : 0x10) | *it;
		byte_data++;
		it++;
	}
	while (it != data.end()) {
		uint8_t ddMMyyyy_presentation = *it << 4;
		if (++it != data.end()) {
			ddMMyyyy_presentation |= *it;
			it++;
		}
		*byte_data = ddMMyyyy_presentation;
		byte_data++;
	}
}

BinaryDecimalNumber::BinaryDecimalNumber()
	: has_sing_flag(false), precision(0), sign(1)
{

}

BinaryDecimalDate::BinaryDecimalDate(const void *raw_data)
	: BinaryDecimalNumber(raw_data, 19, 0, false)
{}

BinaryDecimalDate::BinaryDecimalDate(const std::string &presentation, const std::string &format)
{
	std::map<char, std::vector<int> > indexes;
	for (int i = 0; i < format.size(); i++) {
		indexes.at(format[i]).push_back(i);
	}
	for (auto part : {'y', 'M', 'd', 'h', 'm', 's'}) {
		for (auto i : indexes[part]) {
			if (i < presentation.size()) {
				data.push_back(presentation[i]);
			} else {
				data.push_back(0);
			}
		}
	}
}

std::string BinaryDecimalDate::get_presentation() const
{
	std::string result;
	result += get_part(6, 2);
	result += ".";
	result += get_part(4, 2);
	result += ".";
	result += get_part(0, 4);
	result += " ";
	result += get_part(8, 2);
	result += ":";
	result += get_part(10, 2);
	result += ":";
	result += get_part(12, 2);
	return result;
}

int BinaryDecimalDate::get_year() const
{
	return data[0] * 1000
			+ data[1] * 100
			+ data[2] * 10
			+ data[3];
}

int BinaryDecimalDate::get_month() const
{
	return data[4] * 10 + data[5];
}

int BinaryDecimalDate::get_day() const
{
	return data[6] * 10 + data[7];
}

int BinaryDecimalDate::get_hour() const
{
	return data[8] * 10 + data[9];
}

int BinaryDecimalDate::get_minute() const
{
	return data[10] * 10 + data[11];
}

int BinaryDecimalDate::get_second() const
{
	return data[12] * 10 + data[13];
}
