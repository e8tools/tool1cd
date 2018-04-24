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
#ifndef CONTAINER_FORM_H
#define CONTAINER_FORM_H

#include <QWidget>

namespace Ui {
class ContainerForm;
}

class ContainerForm : public QWidget
{
	Q_OBJECT

public:
	explicit ContainerForm(QWidget *parent = 0);
	~ContainerForm();

	void open(const QString &filename);

private:
	Ui::ContainerForm *ui;
};

#endif // CONTAINER_FORM_H
