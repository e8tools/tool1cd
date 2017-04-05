//---------------------------------------------------------------------------

#pragma hdrstop

#include "TempStream.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

String TTempStream::tempcat;
String TTempStream::tempname;
long TTempStream::tempno = 0;
TTempStreamStaticInit TempStreamStaticInit;

//---------------------------------------------------------------------------
__fastcall TTempStreamStaticInit::TTempStreamStaticInit()
{
	wchar_t temppath[MAX_PATH];
	wchar_t tempfile[MAX_PATH];

	if(TTempStream::tempcat.IsEmpty())
	{
		GetTempPath(MAX_PATH - 1, temppath);
		GetTempFileName(temppath, L"awa", 0, tempfile);
		TTempStream::tempcat = tempfile;
		DeleteFile(TTempStream::tempcat);
		CreateDir(TTempStream::tempcat);
		TTempStream::tempname = TTempStream::tempcat + "\\t";
	}
}

//---------------------------------------------------------------------------
__fastcall TTempStreamStaticInit::~TTempStreamStaticInit()
{
	RemoveDir(TTempStream::tempcat);
}

//---------------------------------------------------------------------------
__fastcall TTempStream::TTempStream() : THandleStream(0)
{
	FHandle = (THandle)CreateFile(gettempname().c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_TEMPORARY | FILE_FLAG_DELETE_ON_CLOSE, NULL);
}

//---------------------------------------------------------------------------
__fastcall TTempStream::~TTempStream()
{
	CloseHandle((HANDLE)FHandle);
}

//---------------------------------------------------------------------------

String __fastcall TTempStream::gettempname()
{
	return tempname + InterlockedIncrement(&tempno);
}

//---------------------------------------------------------------------------

