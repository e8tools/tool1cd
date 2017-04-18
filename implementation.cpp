#include "System.IOUtils.hpp"
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
	return 0;
}

static DynamicArray<Byte> Convert(TEncoding * const Source, TEncoding * const Destination, const DynamicArra<Byte> &Bytes, int StartIndex, int Count)
{
	return Bytes;
}

} // SysUtils

} // System