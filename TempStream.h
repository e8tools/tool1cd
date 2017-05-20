//---------------------------------------------------------------------------

#ifndef TempStreamH
#define TempStreamH

#include <System.Classes.hpp>

class TTempStreamStaticInit
{
public:
	TTempStreamStaticInit();
	~TTempStreamStaticInit();
};


//---------------------------------------------------------------------------
class TTempStream : public TFileStream
{
public:
	static String tempcat;
	static long tempno;
	static String gettempname();

	TTempStream();
	virtual ~TTempStream();
};

//---------------------------------------------------------------------------
#endif
