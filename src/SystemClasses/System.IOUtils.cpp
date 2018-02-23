#include "System.IOUtils.hpp"
#include <boost/filesystem.hpp>

namespace System {

namespace Ioutils {


void CreateDir(const std::string &dirname)
{
	boost::filesystem::create_directory(boost::filesystem::path(dirname));
}

bool FileExists(const std::string &filename)
{
	return boost::filesystem::exists(boost::filesystem::path(filename));
}

void DeleteFile(const std::string &filename)
{
	boost::filesystem::remove_all(filename);
}

void RemoveDir(const std::string &dirname)
{
	// boost::filesystem::remove_all(dirname.c_str());
}

namespace TDirectory {

void CreateDirectory(const std::string &dirname)
{
	CreateDir(dirname);
}

} // TDirectory


namespace TPath {

std::string GetFullPath(const std::string &filename)
{
	auto absolute_path = boost::filesystem::absolute(boost::filesystem::path(filename));
	return absolute_path.string();
}

std::string GetTempPath()
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
