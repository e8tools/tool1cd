#include "System.IOUtils.hpp"
#include <boost/filesystem.hpp>


namespace System {
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

} // System