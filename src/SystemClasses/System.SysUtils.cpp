#include "System.SysUtils.hpp"
#include <boost/filesystem.hpp>
#include <string>
#include "utf8.h"

using namespace std;

namespace System {

namespace SysUtils {

class TUtf8Encoding : public TEncoding
{

virtual std::vector<uint8_t> GetPreamble()
{
	std::vector<uint8_t> result;
	result.push_back(0xEF);
	result.push_back(0xBB);
	result.push_back(0xBF);
	return result;
}

virtual String toUtf8(const std::vector<uint8_t> &Buffer, int offset = 0) const
{
	return String(Buffer, offset);
}

virtual std::vector<uint8_t> fromUtf8(const String &str)
{
	std::vector<uint8_t> result;
	for (char c : str) {
		result.push_back((uint8_t)c);
	}
	result.push_back(0);
	return result;
}

};

class TUcs2Encoding : public TEncoding
{

virtual std::vector<uint8_t> GetPreamble()
{
	std::vector<uint8_t> result;
	result.push_back(0xFF);
	result.push_back(0xFE);
	return result;
}

virtual String toUtf8(const std::vector<uint8_t> &Buffer, int offset) const
{
	auto data_first = (const uint16_t *)(Buffer.data() + offset);
	auto data_last  = data_first + (Buffer.size() - offset) / sizeof(uint16_t);
	std::vector<uint8_t> result_vector;
	utf8::utf16to8(data_first, data_last, back_inserter(result_vector));
	return String(result_vector);
}

virtual std::vector<uint8_t> fromUtf8(const String &data)
{
	auto data_first = data.c_str();
	auto data_last = data_first + data.size();
	std::vector<uint8_t> result_vector;
	utf8::utf8to16(data_first, data_last, back_inserter(result_vector));
	return result_vector;
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


int TEncoding::GetBufferEncoding(const std::vector<uint8_t> &Buffer, TEncoding* &AEncoding)
{

	std::vector<TEncoding*> known_encodings {UTF8, Unicode};
	for (auto enc : known_encodings) {
		auto preamble = enc->GetPreamble();
		if (preamble.size() > Buffer.size()) {
			continue;
		}
		if (std::equal(preamble.begin(), preamble.end(), Buffer.begin())) {
			AEncoding = enc;
			return preamble.size();
		}
	}

	return 0;
}

std::vector<uint8_t> TEncoding::Convert(TEncoding * const Source, TEncoding * const Destination, const std::vector<uint8_t> &Bytes, int StartIndex, int Count)
{
	// TODO: Исправить работу TEncoding::Convert
	auto data = Source->toUtf8(Bytes, StartIndex); // TODO: StartIndex, Count
	std::vector<uint8_t> Result = Destination->fromUtf8(data);
	return Result;
}

std::vector<uint8_t> TEncoding::GetPreamble()
{
	return std::vector<uint8_t>();
}

int StrToInt(const String &s)
{
	return stoi(s.c_str());
}

String ExtractFileExt(const String &filename)
{
	boost::filesystem::path _p(filename.c_str());
	return _p.extension().string();
}

String StringReplace(const String &S, const String &OldPattern, const String &NewPattern, TReplaceFlags Flags)
{
	// TODO: реализовать StringReplace
	return S.Replace(OldPattern, NewPattern);
}



} // SysUtils
} // System
