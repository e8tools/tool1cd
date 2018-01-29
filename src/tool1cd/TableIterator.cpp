//
// Created by dmpas on 28.01.18.
//

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

		table->getrecord(phys_record_num, buf);
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

		table->getrecord(phys_record_num, buf);
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