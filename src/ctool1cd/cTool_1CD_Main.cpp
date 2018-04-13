#include <iosfwd>

#include "cTool_1CD_Main.h"
#include "ErrorCode.h"
#include "App.h"

extern Registrator msreg_g;


//***************************************************************************

//---------------------------------------------------------------------------
// основная точка входа утилиты
int main(int argc, char* argv[])
{
	Messenger mess; // регистратор сообщений

	msreg_g.AddMessageRegistrator(&mess);

	App app(argv, argc, mess);

	int result = app.Run();
	if (result != 0) {
		return result;
	}

	if(mess.has_error)
	{
		return CTOOL_1CD_ERROR;
	}

	return 0;
}

