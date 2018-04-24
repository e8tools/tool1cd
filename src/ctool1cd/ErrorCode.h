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

#ifndef ERRORCODE_H_INCLUDED
#define ERRORCODE_H_INCLUDED

const int CTOOL_1CD_ERROR = -50;

const int CTOOL_1CD_ARGUMENT_ERROR = CTOOL_1CD_ERROR - 20;
const int CTOOL_1CD_INVALID_ARGUMENT = CTOOL_1CD_ARGUMENT_ERROR - 1;

const int CTOOL_1CD_FILE_ERROR = CTOOL_1CD_ERROR - 30;
const int CTOOL_1CD_FILE_NOT_EXISTS  = CTOOL_1CD_FILE_ERROR - 1;
const int CTOOL_1CD_FILE_NOT_OPEN    = CTOOL_1CD_FILE_ERROR - 2;

#endif // ERRORCODE_H_INCLUDED
