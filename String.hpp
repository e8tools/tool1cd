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
};

class TStringList : public std::vector<String>
{

};


} // System

#endif
