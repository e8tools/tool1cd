#include <iosfwd>

#include "cTool_1CD_Main.h"
#include "ErrorCode.h"
#include "App.h"

TMultiReadExclusiveWriteSynchronizer* tr_syn = new TMultiReadExclusiveWriteSynchronizer();

Registrator msreg_g;


//***************************************************************************

//---------------------------------------------------------------------------
// основная точка входа утилиты
int main(int argc, char* argv[])
{
	Messenger mess; // регистратор сообщений

	msreg_g.AddMessageRegistrator(&mess);

	App app(argv, argc, mess);

	bool result = app.Run();
	if (!result) {
		return result;
	}

	if(mess.has_error)
	{
		return CTOOL_1CD_ERROR;
	}

	return 0;
}

