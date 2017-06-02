#include "System.IOUtils.hpp"
#include <boost/filesystem.hpp>

namespace System {

namespace Ioutils {


void CreateDir(const String &dirname)
{
	boost::filesystem::create_directory(boost::filesystem::path(dirname.c_str()));
}

bool FileExists(const String &filename)
{
	return boost::filesystem::exists(boost::filesystem::path(filename.c_str()));
}

bool DirectoryExists(const String &filename)
{
	return boost::filesystem::exists(boost::filesystem::path(filename.c_str()))
		&& boost::filesystem::is_directory(boost::filesystem::path(filename.c_str()));
}

void DeleteFile(const String &filename)
{
	boost::filesystem::remove_all(filename.c_str());
}

void RemoveDir(const String &dirname)
{
	// boost::filesystem::remove_all(dirname.c_str());
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
	auto absolute_path = boost::filesystem::absolute(boost::filesystem::path(filename.c_str()));
	return String(absolute_path.string());
}

String GetTempPath()
{
	return boost::filesystem::temp_directory_path().string();
}

#if defined(_MSC_EXTENSIONS)
void GetTempPath(int bufSize, char *buf)
{
	strncpy_s(buf, bufSize, GetTempPath().c_str(), bufSize);
}

#else
void GetTempPath(int bufSize, char *buf)
{
	strncpy(buf, GetTempPath().c_str(), bufSize);
}

#endif


} // TPath

} // Ioutils


} // System
