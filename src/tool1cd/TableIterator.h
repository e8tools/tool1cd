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

#ifndef TOOL1CD_PROJECT_TABLEITERATOR_H
#define TOOL1CD_PROJECT_TABLEITERATOR_H


#include "TableRecord.h"

class Index;

class TableIterator
{
public:
	TableIterator(Table *table);

	const TableRecord &current() const;
	TableRecord current();

	bool eof() const;
	bool next();

private:

	void find_first_not_removed();

	Table *table;
	char *buf;
	TableRecord _current_record;
	uint32_t phys_record_num;
	bool _eof;
};

class IndexedTableIterator
{
public:
	IndexedTableIterator(Table *table, const std::string &index_name);

	const TableRecord &current() const;
	TableRecord current();

	bool eof() const;
	bool next();

private:

	void find_first_not_removed();

	Table *table;
	Index *index;
	char *buf;
	TableRecord _current_record;
	uint32_t index_record_num;
	bool _eof;
};


#endif //TOOL1CD_PROJECT_TABLEITERATOR_H
