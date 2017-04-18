#ifndef SYSTEM_SYSUTILS_HPP
#define SYSTEM_SYSUTILS_HPP

#include "System.IOUtils.hpp"
#include "String.hpp"

namespace System {

namespace SysUtils {

String StringReplace(const String &S, const String &OldPattern, const String &NewPattern, TReplaceFlags Flags);

class TStringBuilder
{
public:
	explicit TStringBuilder(const String &initial)
		: value(initial)
	{
	}

	TStringBuilder *Replace(const String &substring, const String &replace)
	{
		value = value.Replace(substring, replace);
		return this;
	}

	String ToString() const
	{
		return value;
	}

	String value;
};

class TMultiReadExclusiveWriteSynchronizer 
{

};

class TEncoding
{

public:

static int GetBufferEncoding(const System::DynamicArray<Byte> &Buffer, TEncoding* &AEncoding);
static DynamicArray<Byte> Convert(TEncoding * const Source, TEncoding * const Destination, const DynamicArray<Byte> &Bytes, int StartIndex, int Count);

static TEncoding *Unicode = nullptr;

};

typedef System::DynamicArray<System::Byte> TBytes;

} // SysUtils

namespace Sysutils = SysUtils;

} // System

using namespace System::SysUtils;

#endif
