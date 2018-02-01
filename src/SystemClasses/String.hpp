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

	String UpperCase() const;

	String LowerCase() const;

	bool IsEmpty() const;

	int Length() const;

	int CompareIC(const String &b) const;

	int Compare(const String &b) const;

	String Replace(const String &sub, const String &replace) const;

	String SubString(int StartIndex, int Count) const;

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

	int ToIntDef(int default_value = std::numeric_limits<int>::max()) const;

	int Pos(const String &substr);

	int GetLength() const;
	void SetLength(int NewLength);

	int LastDelimiter(const String &delimiters) const;

	const char *LastChar() const;

	int WideCharBufSize() const;
	WCHART *WideChar(WCHART *wbuf, int destSize) const;

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

	void Add(const String &item);
	void SetText(const String &text);
	int Count() const;
	void Delete(int index);

	void LoadFromFile(const String &filename);
};


} // System

using System::String;
using System::TStringList;

#endif
