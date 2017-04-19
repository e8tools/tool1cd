#include "System.IOUtils.hpp"
#include "System.SysUtils.hpp"
#include <boost/filesystem.hpp>


namespace System {

namespace Classes {

unsigned GetTickCount()
{
	// TODO: GetTickCount()
	return 0;
}

} // Classes

namespace Ioutils {


void CreateDir(const String &dirname)
{
	boost::filesystem::create_directory(boost::filesystem::path(dirname));
}

bool FileExists(const String &filename)
{
	return boost::filesystem::exists(boost::filesystem::path(filename));
}

bool DirectoryExists(const String &filename)
{
	return boost::filesystem::exists(boost::filesystem::path(filename))
		&& boost::filesystem::is_directory(boost::filesystem::path(filename));
}

void DeleteFile(const String &filename)
{
	boost::filesystem::remove(boost::filesystem::path(filename));
}

namespace TDirectory {
	
void CreateDirectory(const String &dirname)		
{
	CreateDir(dirname);
}

} // TDirectory


namespace TPath {

String GetFullPath(const String &filename)
{
	auto absolute_path = boost::filesystem::absolute(boost::filesystem::path(filename));
	return String(absolute_path.string());
}

} // TPath

} // Ioutils

namespace SysUtils {

static int GetBufferEncoding(const System::DynamicArray<Byte> &Buffer, TEncoding* &AEncoding)
{
	// TODO: 
	return 0;
}

static DynamicArray<Byte> TEncoding::Convert(TEncoding * const Source, TEncoding * const Destination, const DynamicArray<Byte> &Bytes, int StartIndex, int Count)
{
	return Bytes;
}

DynamicArray<Byte> TEncoding::GetPreamble()
{
	return DynamicArray<Byte>();
}

int StrToInt(const String &s)
{
	return std::atoi(s.c_str());
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


} // SysUtils

} // System