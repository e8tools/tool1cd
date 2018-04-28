/*
    Tool1CD library provides access to 1CD database files.
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of Tool1CD Library.

    Tool1CD Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tool1CD Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Tool1CD Library.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef NodeTypesH
#define NodeTypesH

#include <string>
//---------------------------------------------------------------------------
enum class node_type{
	nd_empty = 0,		// пусто
	nd_string = 1,		// строка
	nd_number = 2,		// число
	nd_number_exp = 3,	// число с показателем степени
	nd_guid = 4,		// уникальный идентификатор
	nd_list = 5,		// список
	nd_binary = 6,		// двоичные данные (с префиксом #base64:)
	nd_binary2 = 7,		// двоичные данные формата 8.2 (без префикса)
	nd_link = 8,		// ссылка
	nd_binary_d = 9,	// двоичные данные (с префиксом #data:)
	nd_unknown			// неизвестный тип
};

std::string get_node_type_presentation(node_type type);
//---------------------------------------------------------------------------
#endif
