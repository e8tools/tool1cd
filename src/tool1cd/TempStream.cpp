#include "TempStream.h"
#include <boost/filesystem.hpp>
//---------------------------------------------------------------------------
#if !defined(_WIN32)
#pragma package(smart_init)
#endif

using namespace std;

string TTempStream::tempcat;
long TTempStream::tempno = 0;
TTempStreamStaticInit TempStreamStaticInit;

//---------------------------------------------------------------------------
TTempStreamStaticInit::TTempStreamStaticInit()
{
	if (!TTempStream::tempcat.empty()) {
		return;
	}
	boost::filesystem::path p_tempcat = boost::filesystem::temp_directory_path() / "awa";
	TTempStream::tempcat = p_tempcat.string();
	DeleteFile(TTempStream::tempcat);
	CreateDir(TTempStream::tempcat);
}

//---------------------------------------------------------------------------
TTempStreamStaticInit::~TTempStreamStaticInit()
{
	RemoveDir(TTempStream::tempcat);
}

//---------------------------------------------------------------------------
TTempStream::TTempStream() : TFileStream(get_temp_name(), fmCreate)
{
}

//---------------------------------------------------------------------------
TTempStream::~TTempStream()
{
}

//---------------------------------------------------------------------------

std::string TTempStream::get_temp_name()
{
	return (boost::filesystem::path(tempcat) / boost::filesystem::unique_path()).string();
}

//---------------------------------------------------------------------------

