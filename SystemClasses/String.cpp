#include "String.hpp"
#include <algorithm>
#include <stdexcept>
#include <iomanip>
#include <sstream>
#include <boost/filesystem/fstream.hpp>
#include <boost/filesystem.hpp>
#include "System.SysUtils.hpp"

using namespace std;

namespace System {


String::String()
{
}

String::String(const string &src)
	 : string(src)
{
}

String::String(const char *src)
	 : string(src)
{
}

String::String(const char *src, int limit_size)
	 : string(src, limit_size)
{
}

String::String(const DynamicArray<t::Byte> &bytes)
	: string(reinterpret_cast<const char *>(bytes.data()), bytes.size()) {
}

String::String(const WCHART *w_src, int limit_size)
{
	// Костыль
	DynamicArray<t::Byte> tmpdata;
	const WCHART *p = w_src;
	bool limit_exceeded = false;
	while (!limit_exceeded && *p) {
		tmpdata.push_back((uint16_t)(*p) & 0xFF);
		tmpdata.push_back((uint16_t)(*p) >> 8);
		++p;
		if (limit_size) {
			limit_exceeded = --limit_size == 0;
		}
	}
	string tmp(SysUtils::TEncoding::Unicode->toUtf8(tmpdata));
	append(tmp);
}

String::String(int                value) : string(ToString(value)) {}
String::String(unsigned int       value) : string(ToString(value)) {}
String::String(long               value) : string(ToString(value)) {}
String::String(unsigned long      value) : string(ToString(value)) {}
String::String(long long          value) : string(ToString(value)) {}
String::String(unsigned long long value) : string(ToString(value)) {}

String String::UpperCase() const
{
	string copy (*this);
	transform(copy.begin(), copy.end(), copy.begin(), ::toupper);
	return String(copy);
}

String String::LowerCase() const
{
	string copy (*this);
	transform(copy.begin(), copy.end(), copy.begin(), ::tolower);
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
	string _copy(*this);
	auto pos = _copy.find(sub);
	while (pos != string::npos) {
		_copy.replace(pos, sub.size(), replace);
		pos = _copy.find(sub);
	}
	return _copy;
}

String String::SubString(int StartIndex, int Count) const
{
	if (StartIndex > Length() || StartIndex <= 0) {
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
	return stoi(*this);
}

int String::ToIntDef(int default_value) const
{
	try {
		return ToInt();
	} catch (const invalid_argument &) {
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
	stringstream iss(text);
	string line;
	while (getline(iss, line)) {
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
	string line;
	while (getline(ifs, line)) {
		push_back(line);
	}
}


String String::IntToStr(int n)
{
	return to_string(n);
}

String String::UIntToStr(unsigned int n)
{
	return to_string(n);
}

String String::ULongToStr(unsigned long n)
{
	return to_string(n);
}

String String::LongToStr(long n)
{
	return to_string(n);
}

String String::ULongLongToStr(unsigned long long n)
{
	return to_string(n);
}

String String::LongLongToStr(long long n)
{
	return to_string(n);
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
	stringstream ss;
	ss << hex;
	if (digits != 0) {
		ss << setfill('0') << setw(digits);
	}
	ss << n;

	return String(ss.str());
}


} // System

