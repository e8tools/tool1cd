//---------------------------------------------------------------------------

#ifndef TempStreamH
#define TempStreamH

#include <System.Classes.hpp>

class TTempStreamStaticInit
{
public:
	__fastcall TTempStreamStaticInit();
	__fastcall ~TTempStreamStaticInit();
};


//---------------------------------------------------------------------------
class TTempStream : public TFileStream
{
public:
	static String tempcat;
	static String tempname;
	static long tempno;
	static String __fastcall gettempname();

	__fastcall TTempStream();
	virtual __fastcall ~TTempStream();
};

//---------------------------------------------------------------------------
#endif
