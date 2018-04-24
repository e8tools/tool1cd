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

#include "TableIterator.h"
#include "Table.h"

TableIterator::TableIterator(Table *table)
		: table(table), buf(new char[table->get_recordlen()]),
		  _current_record(table, buf), phys_record_num(0), _eof(false)
{
	find_first_not_removed();
}

void TableIterator::find_first_not_removed()
{
	while (true) {
		if (phys_record_num >= table->get_phys_numrecords()) {
			_eof = true;
			return;
		}

		if (_eof) {
			return;
		}

		table->get_record(phys_record_num, buf);
		if (!_current_record.is_removed()) {
			return;
		}
		++phys_record_num;
	}
}

bool TableIterator::eof() const
{
	return _eof;
}

const TableRecord &TableIterator::current() const
{
	return _current_record;
}

TableRecord TableIterator::current()
{
	return _current_record;
}

bool TableIterator::next()
{
	++phys_record_num;
	find_first_not_removed();
	return !eof();
}

IndexedTableIterator::IndexedTableIterator(Table *table, const std::string &index_name)
		: table(table), index(table->get_index(index_name)), buf(new char[table->get_recordlen()]),
		  _current_record(table, buf), index_record_num(0), _eof(false)
{
	find_first_not_removed();
}

void IndexedTableIterator::find_first_not_removed()
{
	while (true) {

		if (index_record_num >= index->get_numrecords()) {
			_eof = true;
			return;
		}

		auto phys_record_num = index->get_numrec(index_record_num);
		if (phys_record_num >= table->get_phys_numrecords()) {
			_eof = true;
			return;
		}

		if (_eof) {
			return;
		}

		table->get_record(phys_record_num, buf);
		if (!_current_record.is_removed()) {
			return;
		}
		++index_record_num;
	}
}

bool IndexedTableIterator::eof() const
{
	return _eof;
}

const TableRecord &IndexedTableIterator::current() const
{
	return _current_record;
}

TableRecord IndexedTableIterator::current()
{
	return _current_record;
}

bool IndexedTableIterator::next()
{
	++index_record_num;
	find_first_not_removed();
	return !eof();
}