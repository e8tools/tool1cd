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
#ifndef V8CATALOG_TREE_MODEL_H
#define V8CATALOG_TREE_MODEL_H

#include <cfapi/V8Catalog.h>
#include <cfapi/V8File.h>
#include <QAbstractItemModel>

class V8CatalogTreeModel : public QAbstractItemModel
{
public:
	V8CatalogTreeModel(V8Catalog *catalog, const QString &catalogName);

	virtual int rowCount(const QModelIndex &parent) const override;

	virtual int columnCount(const QModelIndex &parent) const override;

	virtual QModelIndex index(int row, int column, const QModelIndex &parent) const override;

	virtual QModelIndex parent(const QModelIndex &child) const override;

	virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

	virtual QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

private:
	V8Catalog* catalog;
	QString catalogName;
};


#endif // V8CATALOG_TREE_MODEL_H
