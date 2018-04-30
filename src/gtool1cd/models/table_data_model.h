/*
    GTool1CD provides GUI front end to Tool1CD library
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of GTool1CD.

    GTool1CD is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GTool1CD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GTool1CD.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef TABLEDATAMODEL_H
#define TABLEDATAMODEL_H

#include <QAbstractItemModel>
#include <Table.h>
#include <TableIterator.h>
#include <SystemClasses/TStream.hpp>

class TableDataModel : public QAbstractItemModel
{
public:
	explicit TableDataModel(Table *table, Index *index = nullptr);

	int rowCount(const QModelIndex &parent) const override;

	int columnCount(const QModelIndex &parent) const override;

	QModelIndex index(int row, int column, const QModelIndex &parent) const override;

	QModelIndex parent(const QModelIndex &child) const override;

	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

	void dumpBlob(const QModelIndex &index, const QString &filename) const;

	bool isBlobValue(const QModelIndex &index) const;

	bool isClobValue(const QModelIndex &index) const;

	V8Catalog *getCatalog(const QModelIndex &index) const;

	const TableRecord *getRecord(const QModelIndex &index) const;

	TStream *getBlobStream(const QModelIndex &index) const;

private:
	Table *table;
	Index *_index;
};


#endif // TABLEDATAMODEL_H
