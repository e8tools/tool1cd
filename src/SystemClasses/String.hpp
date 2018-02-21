#ifndef SYSTEM_STRING_HPP
#define SYSTEM_STRING_HPP

#include <string>
#include <vector>
#include "System.hpp"
#include <limits>

namespace System {

// Важно: в String индексация символов идёт с 1, а не с 0!

class String : public std::string
{
public:

	String();

	String(const std::string &src);

	String(const char *src);

	String(const char *src, int limit_size);

	explicit String(const std::vector<uint8_t> &bytes, int offset = 0, int size = -1);

	String(const WCHART *w_src, int limit_size);

	String(const WCHART *w_src);


	String(int value);
	String(unsigned int value);
	String(long value);
	String(unsigned long value);
	String(long long value);
	String(unsigned long long value);

	String Replace(const String &sub, const String &replace) const;

	char &operator[] (int index);
	const char &operator[] (int index) const;

	template<typename AnyStringable>
	String& operator += (AnyStringable b)
	{
		append(String(b));
		return *this;
	}

	template<typename AnyStringable>
	String operator + (AnyStringable value) const
	{
		String result(*this);
		result.append(String(value));
		return result;
	}

	int ToInt() const;

};

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

using System::String;
using System::TStringList;

#endif
