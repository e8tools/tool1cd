#include "TempStream.h"
#include <boost/filesystem.hpp>
//---------------------------------------------------------------------------
#if !defined(_WIN32)
#pragma package(smart_init)
#endif

String TTempStream::tempcat;
long TTempStream::tempno = 0;
TTempStreamStaticInit TempStreamStaticInit;

//---------------------------------------------------------------------------
TTempStreamStaticInit::TTempStreamStaticInit()
{
	if(TTempStream::tempcat.IsEmpty())
	{
		boost::filesystem::path p_tempcat = boost::filesystem::temp_directory_path() / "awa";
		TTempStream::tempcat = p_tempcat.string();
		DeleteFile(TTempStream::tempcat);
		CreateDir(TTempStream::tempcat);
	}
}

//---------------------------------------------------------------------------
TTempStreamStaticInit::~TTempStreamStaticInit()
{
	RemoveDir(TTempStream::tempcat);
}

//---------------------------------------------------------------------------
TTempStream::TTempStream() : TFileStream(gettempname(), fmCreate)
{
}

//---------------------------------------------------------------------------
TTempStream::~TTempStream()
{
}

//---------------------------------------------------------------------------

String TTempStream::gettempname()
{
	return (boost::filesystem::path(tempcat.c_str()) / boost::filesystem::unique_path()).string();
}

//---------------------------------------------------------------------------

