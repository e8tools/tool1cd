#ifndef SYSTEM_STRING_HPP
#define SYSTEM_STRING_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <System.hpp>

#define _DELPHI_STRING_UNICODE

namespace System {

class String : public std::string
{
public:

	String() {}

	String(const std::string &src)
		 : std::string(src)
	{
	}

	String(const char *src)
		 : std::string(src)
	{
	}

	String(const wchar_t *w_src)
	{
		// TODO: Do magic!
	}

	String UpperCase() const
	{
		std::string copy (*this);
		std::transform(copy.begin(), copy.end(), copy.begin(), ::toupper);
		return String(copy);
	}

	String LowerCase() const
	{
		std::string copy (*this);
		std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
		return String(copy);
	}

	bool IsEmpty() const
	{
		return empty();
	}

	int Length() const
	{
		return size();
	}

	int CompareIC(const String &b) const
	{
		return LowerCase().compare(b.LowerCase());
	}

	String SubString(int StartIndex, int Count) const
	{
		return String(substr(StartIndex - 1, Count));
	}

	String& operator += (const wchar_t b)
	{
		// TODO: String +=
		return *this;
	}

	String& operator += (const wchar_t *b)
	{
		// TODO: String +=
		return *this;
	}

	String& operator += (const char *b)
	{
		// TODO: String +=
		return *this;
	}

	String& operator += (const String &b)
	{
		// TODO: String +=
		return *this;
	}

	String operator + (const int value) const
	{
		// TODO: String +
		return *this;
	}
};

class TStringList : public std::vector<String>
{
public:

	void Add(const String &item)
	{
		push_back(item);
	}
};


} // System

using namespace System;

#endif
