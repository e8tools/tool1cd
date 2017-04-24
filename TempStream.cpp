//---------------------------------------------------------------------------

#pragma hdrstop

#include "TempStream.h"
#include <boost/filesystem.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)

String TTempStream::tempcat;
long TTempStream::tempno = 0;
TTempStreamStaticInit TempStreamStaticInit;

//---------------------------------------------------------------------------
__fastcall TTempStreamStaticInit::TTempStreamStaticInit()
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
__fastcall TTempStreamStaticInit::~TTempStreamStaticInit()
{
	RemoveDir(TTempStream::tempcat);
}

//---------------------------------------------------------------------------
__fastcall TTempStream::TTempStream() : TFileStream(gettempname(), fmCreate)
{
}

//---------------------------------------------------------------------------
__fastcall TTempStream::~TTempStream()
{
}

//---------------------------------------------------------------------------

String __fastcall TTempStream::gettempname()
{
	return (boost::filesystem::path(tempcat.c_str()) / boost::filesystem::unique_path()).string();
}

//---------------------------------------------------------------------------

