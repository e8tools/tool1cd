#ifndef SYSTEM_IOUTILS_HPP
#define SYSTEM_IOUTILS_HPP

#include "String.hpp"

namespace System {
namespace Ioutils {

void CreateDir(const String &dirname);

bool FileExists(const String &filename);

bool DirectoryExists(const String &filename);

void DeleteFile(const String &filename);

namespace TDirectory {
	
void CreateDirectory(const String &dirname);

} // TDirectory

namespace TPath {

String GetFullPath(const String &filename);

} // TPath

} // Ioutils

} // System

using namespace System::Ioutils;

#endif
