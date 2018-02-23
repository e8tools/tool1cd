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
