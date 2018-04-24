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
#ifndef CONFIGURATIONS_WINDOW_H
#define CONFIGURATIONS_WINDOW_H

#include <QMainWindow>

class T_1CD;

namespace Ui {
class ConfigurationsWindow;
}

class ConfigurationsWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit ConfigurationsWindow(T_1CD *db, QWidget *parent = 0);
	~ConfigurationsWindow();

private slots:

	void on_saveConfigButton_clicked();

	void on_saveDbConfigButton_clicked();

	void on_saveVendorConfigButton_clicked();

private:
	Ui::ConfigurationsWindow *ui;
	T_1CD *db;
};

#endif // CONFIGURATIONS_WINDOW_H
