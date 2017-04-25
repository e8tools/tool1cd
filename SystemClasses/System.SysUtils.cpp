#include "System.SysUtils.hpp"
#include <boost/filesystem.hpp>


namespace System {

namespace SysUtils {

class TUtf8Encoding : public TEncoding
{

virtual DynamicArray<Byte> GetPreamble()
{
	DynamicArray<Byte> result;
	result.push_back(0xEF);
	result.push_back(0xBB);
	result.push_back(0xBF);
	return result;
}


};

TStringBuilder::TStringBuilder()
{
}

TStringBuilder::TStringBuilder(const String &initial)
	: value(initial)
{
}

TStringBuilder *TStringBuilder::Replace(const String &substring, const String &replace)
{
	value = value.Replace(substring, replace);
	return this;
}

String TStringBuilder::ToString() const
{
	return value;
}

void TStringBuilder::Clear()
{
	value = "";
}

void TStringBuilder::Append(const String &s)
{
	value += s;
}

void TStringBuilder::Append(char c)
{
	value.append(1, c);
}



void TMultiReadExclusiveWriteSynchronizer::BeginWrite()
{
}

void TMultiReadExclusiveWriteSynchronizer::EndWrite()
{
}

void TMultiReadExclusiveWriteSynchronizer::BeginRead()
{
}

void TMultiReadExclusiveWriteSynchronizer::EndRead()
{
}


TEncoding *TEncoding::Unicode = new TUtf8Encoding();
TEncoding *TEncoding::UTF8 = new TUtf8Encoding();


int TEncoding::GetBufferEncoding(const System::DynamicArray<Byte> &Buffer, TEncoding* &AEncoding)
{
	// TODO:
	return 0;
}

DynamicArray<Byte> TEncoding::Convert(TEncoding * const Source, TEncoding * const Destination, const DynamicArray<Byte> &Bytes, int StartIndex, int Count)
{
	return Bytes;
}

DynamicArray<Byte> TEncoding::GetPreamble()
{
	return DynamicArray<Byte>();
}

int StrToInt(const String &s)
{
	return std::stoi(s.c_str());
}

int FindFirst(const String &Path, int Attr, TSearchRec &rec)
{
	// TODO: FindFirst
	return 1;
}

int  FindNext(TSearchRec &rec)
{
	// TODO:
	return 1;
}

void FindClose(TSearchRec &rec)
{
	// TODO:
}

void CreateGUID(TGUID &guid)
{
	// TODO: CreateGUID
}

String ExtractFileExt(const String &filename)
{
	boost::filesystem::path _p(filename.c_str());
	return _p.extension().string();
}

String StringReplace(const String &S, const String &OldPattern, const String &NewPattern, TReplaceFlags Flags)
{
	// TODO: StringReplace
	return S.Replace(OldPattern, NewPattern);
}



} // SysUtils
} // System
