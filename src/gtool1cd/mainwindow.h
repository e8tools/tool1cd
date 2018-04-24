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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Class_1CD.h>
#include <QMap>

class Table;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public MessageRegistrator
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	void open(T_1CD *database);
	~MainWindow();

	void addLogMessage(const QString &message);

	virtual void AddDetailedMessage(
	        const std::string &description,
	        const MessageState mstate,
	        const TStringList *param = nullptr) override;

	virtual void Status(const std::string& message) override;

private slots:


	void on_exitAction_triggered();

	void on_openDatabaseFileAction_triggered();

	void on_tableListView_doubleClicked(const QModelIndex &index);

	void on_configurationsButton_clicked();

	void on_tableListView_activated(const QModelIndex &index);

private:
	Ui::MainWindow *ui;
	T_1CD *db;
	QStringList logData;
	QMap<Table *, QWidget *> table_windows;
	QWidget *configurationsWindow;
};

#endif // MAINWINDOW_H
