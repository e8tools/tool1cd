#ifndef SYSTEM_CLASSES_HPP
#define SYSTEM_CLASSES_HPP

#include <cstdint>
#include <string>
#include <vector>
#include <algorithm>
#include <System.hpp>
#include "DynamicArray.hpp"
#include "String.hpp"
#include "TStream.hpp"
#include "TMemoryStream.hpp"
#include "TFileStream.hpp"
#include "System.SysUtils.hpp"
#include "Exception.hpp"
#include "TStreamWriter.hpp"
#include "TStreamReader.hpp"

namespace System {

namespace Classes {


class TCriticalSection
{
public:
	TCriticalSection();

	virtual void Acquire();
	virtual void Release();
};

unsigned GetTickCount();

} // Classes


} // System

#endif
