/*
    CTool1CD provides console front end to Tool1CD library
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of CTool1CD.

    CTool1CD is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CTool1CD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CTool1CD.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <iosfwd>

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

