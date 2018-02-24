#ifndef SYSTEM_STRING_HPP
#define SYSTEM_STRING_HPP

#include <string>
#include <vector>
#include "System.hpp"
#include <limits>

namespace System {

std::string LowerCase(const std::string &src);
std::string UpperCase(const std::string &src);
int CompareIC(const std::string &a, const std::string &b);
int Compare(const std::string &a, const std::string &b);

bool EndsWithIC(const std::string &str, const std::string &substr);
bool EqualIC(const std::string &a, const std::string &b);
bool Equal(const std::string &a, const std::string &b);

int ToIntDef(const std::string &s, int default_value);

template <typename char_type>
std::string operator + (const std::basic_string<char_type> &text, const int value)
{
	return std::string(text).append(std::to_string(value));
}

class TStringList : public std::vector<std::string>
{
public:

	void Add(const std::string &item);
	void SetText(const std::string &text);
	int Count() const;
	void Delete(int index);

	void LoadFromFile(const std::string &filename);
};


} // System

using System::TStringList;

#endif
