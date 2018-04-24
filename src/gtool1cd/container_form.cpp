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
#include "container_form.h"
#include "ui_container_form.h"
#include <QFileInfo>
#include <TFileStream.hpp>

ContainerForm::ContainerForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ContainerForm)
{
	ui->setupUi(this);
}

ContainerForm::~ContainerForm()
{
	delete ui;
}

void ContainerForm::open(const QString &filename)
{
	QFileInfo finfo(filename);
	auto stream = new TFileStream(filename.toStdString(), fmOpenRead);
	ui->widget->setStream(stream, finfo.baseName());
	setWindowTitle(filename);
}
