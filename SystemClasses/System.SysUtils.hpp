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
	explicit TStringBuilder();

	explicit TStringBuilder(const String &initial);

	TStringBuilder *Replace(const String &substring, const String &replace);

	String ToString() const;

	void Clear();

	void Append(const String &s);

	void Append(char c);

	String value;
};

class TMultiReadExclusiveWriteSynchronizer
{
public:
	void BeginWrite();
	void EndWrite();

	void BeginRead();
	void EndRead();
};

class TEncoding
{

public:

System::DynamicArray<System::Byte> GetPreamble();

static int GetBufferEncoding(const System::DynamicArray<Byte> &Buffer, TEncoding* &AEncoding);
static DynamicArray<Byte> Convert(TEncoding * const Source, TEncoding * const Destination, const DynamicArray<Byte> &Bytes, int StartIndex, int Count);

static TEncoding *Unicode;
static TEncoding *UTF8;

};

typedef System::DynamicArray<System::Byte> TBytes;

int StrToInt(const String &s);


struct TSearchRec {
	int     Time;
	int64_t Size;
	int     Attr;
	String  Name;
	int     ExcludeAttr;
};

int  FindFirst(const String &Path, int Attr, TSearchRec &rec);
int  FindNext(TSearchRec &rec);
void FindClose(TSearchRec &rec);

void CreateGUID(TGUID &guid);

String ExtractFileExt(const String &filename);

} // SysUtils

namespace Sysutils = SysUtils;

} // System

using namespace System::SysUtils;

#endif
