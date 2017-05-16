#include "String.hpp"
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>
#include "System.SysUtils.hpp"

namespace System {


String::String()
{
}

String::String(const std::string &src)
	 : std::string(src)
{
}

String::String(const char *src, int limit_size)
	 : std::string(src)
{
}

String::String(const WCHART *w_src, int limit_size)
{
	// Костыль
	DynamicArray<Byte> tmpdata;
	const WCHART *p = w_src;
	bool limit_exceeded = false;
	while (!limit_exceeded && *p) {
		tmpdata.push_back((uint16_t)(*p) && 0xFF);
		tmpdata.push_back((uint16_t)(*p) >> 16);
		++p;
		if (limit_size) {
			limit_exceeded = --limit_size == 0;
		}
	}
	std::string tmp(SysUtils::TEncoding::Unicode->toUtf8(tmpdata));
	append(tmp);
}

String::String(int                value) : std::string(ToString(value)) {}
String::String(unsigned int       value) : std::string(ToString(value)) {}
String::String(long               value) : std::string(ToString(value)) {}
String::String(unsigned long      value) : std::string(ToString(value)) {}
String::String(long long          value) : std::string(ToString(value)) {}
String::String(unsigned long long value) : std::string(ToString(value)) {}

String String::UpperCase() const
{
	std::string copy (*this);
	std::transform(copy.begin(), copy.end(), copy.begin(), ::toupper);
	return String(copy);
}

String String::LowerCase() const
{
	std::string copy (*this);
	std::transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
	return String(copy);
}

bool String::IsEmpty() const
{
	return empty();
}

int String::Length() const
{
	return size();
}

int String::CompareIC(const String &b) const
{
	return LowerCase().compare(b.LowerCase());
}

int String::Compare(const String &b) const
{
	return compare(b);
}

String String::Replace(const String &sub, const String &replace) const
{
	return *this;
}

String String::SubString(int StartIndex, int Count) const
{
	if (StartIndex > Length()) {
		return String("");
	}
	return String(substr(StartIndex - 1, Count));
}

char &String::operator[] (int index)
{
	return at(index - 1);
}

const char &String::operator[] (int index) const
{
	return at(index - 1);
}

int String::ToInt() const
{
	return std::stoi(*this);
}

int String::ToIntDef(int default_value) const
{
	try {
		return ToInt();
	} catch (const std::invalid_argument &) {
		return default_value;
	}
}

int String::Pos(const String &substr)
{
	auto index = find(substr);
	if (index == npos) {
		return 0;
	}
	return index;
}

int String::GetLength() const
{
	return this->size();
}

void String::SetLength(int NewLength)
{
	resize(NewLength);
}

int String::LastDelimiter(const String &delimiters) const
{
	return 0;
}

const char *String::LastChar() const
{
	if (size() == 0) {
		return nullptr;
	}
	return c_str() + (size() - 1);
}

int String::WideCharBufSize() const
{
	return sizeof(wchar_t) * (size() + 1);
}

WCHART *String::WideChar(WCHART *wbuf, int destSize) const
{
	bool limit_exceeded = false;
	for (size_t i = 0; i < size() && !limit_exceeded; i++) {
		wbuf[i] = c_str()[i];
		if (destSize) {
			limit_exceeded = --destSize == 0;
		}
	}
	return wbuf;
}




void TStringList::Add(const String &item)
{
	push_back(item);
}

void TStringList::SetText(const String &text)
{
	clear();
	std::stringstream iss(text);
	std::string line;
	while (std::getline(iss, line)) {
		push_back(line);
	}
}

int TStringList::Count() const
{
	return size();
}

void TStringList::Delete(int index)
{
	erase(begin() + index);
}

void TStringList::LoadFromFile(const String &filename)
{
	clear();
	boost::filesystem::ifstream ifs(boost::filesystem::path(filename.c_str()));
	std::string line;
	while (std::getline(ifs, line)) {
		push_back(line);
	}
}


String String::IntToStr(int n)
{
	return std::to_string(n);
}

String String::UIntToStr(unsigned int n)
{
	return std::to_string(n);
}

String String::ULongToStr(unsigned long n)
{
	return std::to_string(n);
}

String String::LongToStr(long n)
{
	return std::to_string(n);
}

String String::ULongLongToStr(unsigned long long n)
{
	return std::to_string(n);
}

String String::LongLongToStr(long long n)
{
	return std::to_string(n);
}

String String::ToString(int n)
{
	return IntToStr(n);
}

String String::ToString(unsigned int n)
{
	return UIntToStr(n);
}

String String::ToString(unsigned long n)
{
	return ULongToStr(n);
}

String String::ToString(long n)
{
	return LongToStr(n);
}

String String::ToString(unsigned long long n)
{
	return ULongLongToStr(n);
}

String String::ToString(long long n)
{
	return LongLongToStr(n);
}


String String::IntToHex(int n, int digits)
{
	std::stringstream ss;
	ss << std::hex;
	if (digits != 0) {
		ss << std::setfill('0') << std::setw(digits);
	}
	ss << n;

	return String(ss.str());
}


} // System

