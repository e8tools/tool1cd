#include "System.hpp"

namespace System {

bool operator == (const TGUID &a, const TGUID &b)
{
	return a.D1 == b.D1
		&& a.D2 == b.D2
		&& a.D3 == b.D3
		&& a.D4[0] == b.D4[0]
		&& a.D4[1] == b.D4[1]
		&& a.D4[2] == b.D4[2]
		&& a.D4[3] == b.D4[3]
		&& a.D4[4] == b.D4[4]
		&& a.D4[5] == b.D4[5]
		&& a.D4[6] == b.D4[6]
		&& a.D4[7] == b.D4[7]
	;
}

bool SystemTimeToFileTime(const SYSTEMTIME *lpSystemTime, LPFILETIME lpFileTime)
{
	return false; // TODO: реализовать кроссплатформенное преобразование системного времени в файловое время
}

bool LocalFileTimeToFileTime(const FILETIME *lpLocalFileTime, LPFILETIME lpFileTime)
{
	return false; // TODO: реализовать кроссплатформенное преобразование локального системного файлового времени в универсальное (UTC) файловое время
}

bool FileTimeToLocalFileTime(const FILETIME *lpLocalFileTime, LPFILETIME lpFileTime)
{
	return false; // TODO: реализовать кроссплатформенное преобразование из универсального (UTC) файлового времени в локальноое системное файловое время
}

void GetSystemTime(LPSYSTEMTIME lpSystemTime)
{
	// TODO: реализовать кроссплатформенное получение времени операционной системы
}

} // System
