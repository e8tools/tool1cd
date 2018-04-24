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
#ifndef BLOB_VIEWER_H
#define BLOB_VIEWER_H

#include <QWidget>
#include <TStream.hpp>
#include <QMap>

namespace Ui {
class BlobViewer;
}

class BlobViewer : public QWidget
{
	Q_OBJECT

public:
	explicit BlobViewer(QWidget *parent = 0);
	~BlobViewer();

	void setText(const QString &textData, bool do_not_hide_tabs = false);

	void setStream(TStream *stream, const QString &rootName);

private slots:
	void on_treeView_doubleClicked(const QModelIndex &index);
	void prevTabActivated();
	void nextTabActivated();

	void on_treeView_activated(const QModelIndex &index);

private:
	QString rootName;
	QMap<QString, QWidget *> widgets;
	Ui::BlobViewer *ui;
};

#endif // BLOB_VIEWER_H
