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

#ifndef SRC_CTOOL1CD_TABLEFILES_H_
#define SRC_CTOOL1CD_TABLEFILES_H_

#include <map>

#include "Common.h"
#include "Table.h"
#include "TableRecord.h"

//---------------------------------------------------------------------------
// Структура записи таблицы контейнера файлов
struct table_rec
{
	std::string name;
	table_blob_file addr;
	int32_t partno;
	System::FILETIME ft_create;
	System::FILETIME ft_modify;
};

//---------------------------------------------------------------------------
// Структура файла таблицы контейнера файлов
struct TableFile
{
	Table* t;
	std::string name; // Имя, как оно хранится в таблице
	uint32_t maxpartno;
	table_blob_file* addr;
	System::FILETIME ft_create;
	System::FILETIME ft_modify;

	TableFile(Table *_t, const std::string &_name, uint32_t _maxpartno);
	~TableFile();
};

//---------------------------------------------------------------------------
// Класс таблицы контейнера файлов (CONFIG, CONFIGSAVE, PARAMS, FILES, CONFICAS, CONFICASSAVE)
class TableFiles
{
public:
	explicit TableFiles(Table* t);
	virtual ~TableFiles();

	bool get_ready() const { return ready; }
	TableFile* get_file(const std::string &name);
	Table* get_table() { return table; }

	std::map<std::string,TableFile*> &files();
	const std::map<std::string,TableFile*> &files() const;
private:
	Table* table {nullptr};
	std::map<std::string,TableFile*> allfiles;
	bool ready {false};

	bool test_table();
};


#endif /* SRC_CTOOL1CD_TABLEFILES_H_ */
