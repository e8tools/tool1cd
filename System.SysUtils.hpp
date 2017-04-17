#ifndef SYSTEM_SYSUTILS_HPP
#define SYSTEM_SYSUTILS_HPP

#include "System.IOUtils.hpp"
#include "String.hpp"

namespace System {

namespace SysUtils {

String StringReplace(const String &S, const String &OldPattern, const String &NewPattern, TReplaceFlags Flags);

class TStringBuilder
{
public:
	explicit TStringBuilder(const String &initial)
	{
		// TODO: Magic
	}

	void Replace(const String &substring, const String &replace)
	{
		// TODO: Magic
	}

	String ToString() const
	{
		// TODO: Magic
		return String("");
	}
};

} // SysUtils

namespace Sysutils = SysUtils;

} // System

using namespace System::SysUtils;

#endif
