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
#include "skobkatextwindow.h"
#include "ui_skobkatextwindow.h"
#include "models/skobka_tree_model.h"
#include "BlobViewer/blob_viewer.h"

SkobkaTextWindow::SkobkaTextWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SkobkaTextWindow)
{
	ui->setupUi(this);
}

SkobkaTextWindow::~SkobkaTextWindow()
{
	delete ui;
}

void SkobkaTextWindow::setText(const QString &text, const QString &title)
{
	BlobViewer *bv = static_cast<BlobViewer*>(ui->widget);
	/*
	QTextDocument *qd = new QTextDocument(text);
	qd->setDocumentLayout(new QPlainTextDocumentLayout(qd));
	ui->plainTextEdit->setDocument(qd);
	ui->treeView->setModel(new SkobkaTreeModel(parse_1Ctext(text.toStdString(), title.toStdString())));
	*/
	bv->setText(text);
	setWindowTitle(title);
}
