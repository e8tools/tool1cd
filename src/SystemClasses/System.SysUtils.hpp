#ifndef SYSTEM_SYSUTILS_HPP
#define SYSTEM_SYSUTILS_HPP

#include <vector>

#include "System.IOUtils.hpp"
#include "String.hpp"

namespace System {

namespace SysUtils {

std::string StringReplace(const std::string &S, const std::string &OldPattern, const std::string &NewPattern,
						  int Flags = 0);

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

virtual std::vector<uint8_t> GetPreamble() = 0;
virtual std::string toUtf8(const std::vector<uint8_t> &Buffer, int offset = 0) const = 0;
virtual std::vector<uint8_t> fromUtf8(const std::string &str) = 0;

static int GetBufferEncoding(const std::vector<uint8_t> &Buffer, TEncoding* &AEncoding);
static std::vector<uint8_t> Convert(TEncoding * const Source, TEncoding * const Destination, const std::vector<uint8_t> &Bytes, int StartIndex, int Count);

//! двухбайтная кодировка WCHART
static TEncoding *Unicode;

static TEncoding *UTF8;

};


struct TSearchRec {
	int          Time;
	int64_t      Size;
	int          Attr;
	std::string  Name;
	int          ExcludeAttr;
};

std::string ExtractFileExt(const std::string &filename);

} // SysUtils

namespace Sysutils = SysUtils;

} // System

using namespace System::SysUtils;

#endif
