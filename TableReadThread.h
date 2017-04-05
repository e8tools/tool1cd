//---------------------------------------------------------------------------

#ifndef TableReadThreadH
#define TableReadThreadH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include "AdvGrid.hpp"
#include "BaseGrid.hpp"

#include "Main_1CD.h"
#include "Class_1CD.h"
//---------------------------------------------------------------------------
class TFormMain;

class TableReadThread : public TThread
{
private:
protected:
	void __fastcall Execute();
	TFormMain* FormMain;
public:
	__fastcall TableReadThread(bool CreateSuspended, TFormMain* _FormMain);
	void __fastcall SetLengthTable();
};
//---------------------------------------------------------------------------
#endif
