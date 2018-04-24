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
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QStringListModel>
#include "starter.h"
#include "cache.h"
#include "table_fields_window.h"
#include "table_data_window.h"
#include "models/tables_list_model.h"
#include "configurations_window.h"

void MainWindow::AddDetailedMessage(
        const std::string &description,
        const MessageState mstate,
        const TStringList *param)
{
	this->addLogMessage(QString(description.c_str()));
}

void MainWindow::Status(const std::string& message)
{

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    configurationsWindow(nullptr)
{
	ui->setupUi(this);
	ui->logList->setModel(new QStringListModel(logData));
	ui->logList->setVisible(false);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::open(T_1CD *database)
{
	db = database;
	ui->tableListView->setModel(new TablesListModel(db));
	setWindowTitle(QString::fromStdString(db->get_filename()));
	// refresh data
}

void MainWindow::addLogMessage(const QString &message)
{
	logData.append(message);
}

void MainWindow::on_exitAction_triggered()
{
	for (auto w : table_windows) {
		delete w;
	}
	close();
}

void MainWindow::on_openDatabaseFileAction_triggered()
{
	StarterWindow *w = new StarterWindow();
	w->setCache(new Cache());
	w->show();
}

void MainWindow::on_tableListView_doubleClicked(const QModelIndex &index)
{
	Table *t = db->get_table(index.row());
	if (table_windows.find(t) == table_windows.end()) {
		table_windows[t] = new TableDataWindow(this, t);
	}
	table_windows[t]->show();
	table_windows[t]->activateWindow();
}

void MainWindow::on_configurationsButton_clicked()
{
	if (configurationsWindow == nullptr) {
		configurationsWindow = new ConfigurationsWindow(db, this);
	}
	configurationsWindow->show();
	configurationsWindow->activateWindow();
}

void MainWindow::on_tableListView_activated(const QModelIndex &index)
{
	emit ui->tableListView->doubleClicked(index);
}
