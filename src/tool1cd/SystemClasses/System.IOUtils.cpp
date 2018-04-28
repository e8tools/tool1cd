/*
    Tool1CD library provides access to 1CD database files.
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of Tool1CD Library.

    Tool1CD Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tool1CD Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Tool1CD Library.  If not, see <http://www.gnu.org/licenses/>.
*/
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
