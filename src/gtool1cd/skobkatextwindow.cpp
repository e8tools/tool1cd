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
