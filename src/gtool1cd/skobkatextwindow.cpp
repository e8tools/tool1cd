#include "skobkatextwindow.h"
#include "ui_skobkatextwindow.h"

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
	QTextDocument *qd = new QTextDocument(text);
	qd->setDocumentLayout(new QPlainTextDocumentLayout(qd));
	ui->plainTextEdit->setDocument(qd);

	setWindowTitle(title);
}
