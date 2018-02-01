//
// Created by dmpas on 28.01.18.
//

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
	int phys_record_num;
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
	int index_record_num;
	bool _eof;
};


#endif //TOOL1CD_PROJECT_TABLEITERATOR_H
