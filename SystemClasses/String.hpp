#ifndef SYSTEM_STRING_HPP
#define SYSTEM_STRING_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <System.hpp>
#include <DynamicArray.hpp>
#include <stdexcept>

namespace System {

// Важно: в String индексация символов идёт с 1, а не с 0!

class String : public std::string
{
public:

	String() {}

	String(const std::string &src)
		 : std::string(src)
	{
	}

	String(const char *src, int limit_size = 0)
		 : std::string(src)
	{
	}

	String(const WCHART *w_src, int limit_size = 0)
	{
		// Костыль
		const WCHART *p = w_src;
		bool limit_exceeded = false;
		while (!limit_exceeded && *p) {
			append(1, (const char)(*p));
			++p;
			if (limit_size) {
				limit_exceeded = --limit_size == 0;
			}
		}
	}

	String(int value) : std::string(ToString(value)) {}
	String(unsigned int value) : std::string(ToString(value)) {}
	String(long value) : std::string(ToString(value)) {}
	String(unsigned long value) : std::string(ToString(value)) {}
	String(long long value) : std::string(ToString(value)) {}
	String(unsigned long long value) : std::string(ToString(value)) {}

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

	int Compare(const String &b) const
	{
		return compare(b);
	}

	String Replace(const String &sub, const String &replace) const
	{
		return *this;
	}

	String SubString(int StartIndex, int Count) const
	{
		if (StartIndex > Length()) {
			return String("");
		}
		return String(substr(StartIndex - 1, Count));
	}

	char &operator[] (int index)
	{
		return at(index - 1);
	}

	const char &operator[] (int index) const
	{
		return at(index - 1);
	}

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

	int ToInt() const
	{
		return std::stoi(*this);
	}

	int ToIntDef(int default_value) const
	{
		try {
			return ToInt();
		} catch (const std::invalid_argument &) {
			return default_value;
		}
	}

	int Pos(const String &substr)
	{
		auto index = find(substr);
		if (index == npos) {
			return 0;
		}
		return index;
	}

	int GetLength() const
	{
		return this->size();
	}

	void SetLength(int NewLength)
	{
		resize(NewLength);
	}

	int LastDelimiter(const String &delimiters) const
	{
		return 0;
	}

	const char *LastChar() const
	{
		if (size() == 0) {
			return nullptr;
		}
		return c_str() + (size() - 1);
	}

	int WideCharBufSize() const
	{
		return sizeof(wchar_t) * (size() + 1);
	}

	wchar_t *WideChar(wchar_t *wbuf, int destSize) const
	{
		// TODO: String:WideChar()
		for (int i = 0; i < size(); i++) {
			wbuf[i] = c_str()[i];
		}
		return wbuf;
	}

	static String IntToHex(int n, int digits);
	static String IntToStr(int n);
	static String UIntToStr(unsigned int n);
	static String LongToStr(long int n);
	static String ULongToStr(unsigned long n);
	static String LongLongToStr(long long n);
	static String ULongLongToStr(unsigned long long n);

	static String ToString(int n);
	static String ToString(unsigned int n);
	static String ToString(long n);
	static String ToString(unsigned long n);
	static String ToString(long long n);
	static String ToString(unsigned long long n);
};

template <typename char_type>
String operator + (const std::basic_string<char_type> &text, const int value)
{
	String a(text);
	a += value;
	return a;
}

class TStringList : public std::vector<String>
{
public:

	void Add(const String &item)
	{
		push_back(item);
	}

	void SetText(const String &text)
	{
		// TODO: Magic
	}

	int Count() const
	{
		return size();
	}

	void Delete(int index)
	{
		erase(begin() + index);
	}

	void LoadFromFile(const String &filename)
	{

	}

	DynamicArray<String> Strings;
};


} // System

using namespace System;

#endif
