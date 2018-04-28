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
#ifndef SYSTEM_IOUTILS_HPP
#define SYSTEM_IOUTILS_HPP

#include "String.hpp"

namespace System {
namespace Ioutils {

void CreateDir(const std::string &dirname);

bool FileExists(const std::string &filename);

void DeleteFile(const std::string &filename);

void RemoveDir(const std::string &dirname);

namespace TDirectory {

void CreateDirectory(const std::string &dirname);

} // TDirectory

namespace TPath {

std::string GetFullPath(const std::string &filename);
std::string GetTempPath();
void GetTempPath(int bufSize, char *buf);

} // TPath

} // Ioutils

} // System

using namespace System::Ioutils;

#endif
