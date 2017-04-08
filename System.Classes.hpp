#ifndef SYSTEM_CLASSES_HPP
#define SYSTEM_CLASSES_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <System.hpp>

namespace System {

class String;

namespace Classes {

enum TSeekOrigin {
  soBeginning,
  soCurrent,
  soEnd
};

const TSeekOrigin soFromBeginning = soBeginning;

class TStream
{

public:

	virtual int64_t GetSize() const
	{
		return 0;
	}
	virtual void SetSize(int64_t NewSize)
	{

	}

public:

	virtual int64_t Read(void *Buffer, int64_t Count)
	{

	}

	virtual int64_t Read(System::DynamicArray<System::Byte> &Buffer, int64_t Count)
	{
		
	}

	virtual int64_t Seek(const int64_t offset, const TSeekOrigin Origin)
	{

	}

	virtual int64_t CopyFrom(TStream *Source, const int64_t Count)
	{

	}

	virtual int64_t WriteBuffer(const void *Buffer, const int64_t Count)
	{
		return 0;
	}

	virtual int64_t Write(const void *Buffer, const int64_t Count)
	{
		return 0;
	}

	virtual int64_t Write(const System::DynamicArray<System::Byte> &Buffer, const int64_t Count)
	{
		return 0;
	}
};

class TMemoryStream : public TStream {
public:
	TMemoryStream() {}

	~TMemoryStream() {}


};

const uint16_t fmOpenReadWrite = 0x0002;
const uint16_t fmCreate = 0xFFFF;
typedef uint16_t HANDLE;

class TFileStream : public TStream
{
public:
	TFileStream(const String &FileName, const uint16_t fileMode) {}

	~TFileStream() {}


	int16_t GetHandle() const
	{
		return 0;
	}

};

class TCriticalSection
{
public:
	TCriticalSection() {}

	virtual void Acquire() {}
	virtual void Release() {}
};

} // Classes


class String : public std::string
{
public:

	String() {}

	String(const std::string &src)
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

	int WideCharBufSize() const
	{
		return size() *2 + 2;
	}

	int Length() const
	{
		return size();
	}
};

class TStringList : public std::vector<String>
{

};

struct FILETIME {};

} // System

#endif
