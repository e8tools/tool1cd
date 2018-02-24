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

virtual string toUtf8(const std::vector<uint8_t> &Buffer, int offset = 0) const
{
	if (Buffer.empty()) {
		return string("");
	}
	return string(reinterpret_cast<const char*>(Buffer.data()), offset, Buffer.size() - offset);
}

virtual std::vector<uint8_t> fromUtf8(const string &str)
{
	std::vector<uint8_t> result;
	for (char c : str) {
		result.push_back((uint8_t)c);
	}
	result.push_back(0); // TODO: Вероятно, это теперь лишнее
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

virtual string toUtf8(const std::vector<uint8_t> &Buffer, int offset) const
{
	auto data_first = (const uint16_t *)(Buffer.data() + offset);
	auto data_last  = data_first + (Buffer.size() - offset) / sizeof(uint16_t);
	std::vector<uint8_t> result_vector;
	utf8::utf16to8(data_first, data_last, back_inserter(result_vector));
	return TEncoding::UTF8->toUtf8(result_vector);
}

virtual std::vector<uint8_t> fromUtf8(const string &data)
{
	auto data_first = data.c_str();
	auto data_last = data_first + data.size();
	vector<uint16_t> _vector;
	utf8::utf8to16(data_first, data_last, back_inserter(_vector));

	vector<uint8_t> result_vector;
	for (auto u16 : _vector) {
		result_vector.push_back(static_cast<uint8_t>(u16 & 0xff));
		result_vector.push_back(static_cast<uint8_t>(u16 >> 8));
	}
	return result_vector;
}


};


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

string ExtractFileExt(const string &filename)
{
	boost::filesystem::path _p(filename.c_str());
	return _p.extension().string();
}

string StringReplace(const string &S, const string &OldPattern, const string &NewPattern, int Flags)
{
	string result(S);
	string::size_type spos = 0;
	do {
		auto pos = result.find(OldPattern, spos);
		if (pos == string::npos) {
			break;
		}
		result = result.replace(pos, OldPattern.size(), NewPattern);
		spos = pos + NewPattern.size();
	} while (Flags & rfReplaceAll);
	return result;
}



} // SysUtils
} // System
