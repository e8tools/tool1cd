#include "System.SysUtils.hpp"
#include <boost/filesystem.hpp>
#include <codecvt>
#include <string>

using namespace std;

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

// TODO: Убрать кучу магии

virtual String toUtf8(const System::DynamicArray<Byte> &Buffer) const
{
	return String(Buffer);
}

virtual DynamicArray<Byte> fromUtf8(const String &str)
{
	DynamicArray<Byte> result;
	for (char c : str) {
		result.push_back((Byte)c);
	}
	result.push_back(0);
	return result;
}

};

class TUcs2Encoding : public TEncoding
{

virtual DynamicArray<Byte> GetPreamble()
{
	DynamicArray<Byte> result;
	result.push_back(0xFF);
	result.push_back(0xFE);
	return result;
}

virtual String toUtf8(const System::DynamicArray<Byte> &Buffer) const
{
	wstring_convert<codecvt_utf8<char16_t>, char16_t> conv;
	auto data_first = (const char16_t *)Buffer.data();
	auto data_last = data_first + Buffer.size() / 2;
	auto result = conv.to_bytes(data_first, data_last);
	return String(result);
}

virtual DynamicArray<Byte> fromUtf8(const String &data)
{
	wstring_convert<codecvt_utf8<char16_t>, char16_t> converter;
	auto result_data = converter.from_bytes(data.c_str());

	DynamicArray<Byte> result;
	const char *outdata = (const char *)result_data.c_str();
	do {
		result.push_back(*outdata);
	} while (*outdata++);
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


TEncoding *TEncoding::UTF8 = new TUtf8Encoding();
TEncoding *TEncoding::Unicode = new TUcs2Encoding();


int TEncoding::GetBufferEncoding(const System::DynamicArray<Byte> &Buffer, TEncoding* &AEncoding)
{
	// TODO:
	return 0;
}

DynamicArray<Byte> TEncoding::Convert(TEncoding * const Source, TEncoding * const Destination, const DynamicArray<Byte> &Bytes, int StartIndex, int Count)
{
	// TODO: Костыли
	auto data = Source->toUtf8(Bytes); // TODO: StartIndex, Count
	DynamicArray<Byte> Result = Destination->fromUtf8(data);
	return Result;
}

DynamicArray<Byte> TEncoding::GetPreamble()
{
	return DynamicArray<Byte>();
}

int StrToInt(const String &s)
{
	return stoi(s.c_str());
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
