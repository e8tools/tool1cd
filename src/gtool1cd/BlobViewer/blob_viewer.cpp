#include "blob_viewer.h"
#include "ui_blob_viewer.h"
#include <Parse_tree.h>
#include "../models/skobka_tree_model.h"

BlobViewer::BlobViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlobViewer)
{
	ui->setupUi(this);
}

BlobViewer::~BlobViewer()
{
	delete ui;
}

void BlobViewer::setText(const QString &textData)
{
	QTextDocument *qd = new QTextDocument(textData);
	qd->setDocumentLayout(new QPlainTextDocumentLayout(qd));
	ui->plainTextEdit->setDocument(qd);

	ui->textDataTab->setVisible(true);
	ui->tabWidget->setCurrentWidget(ui->textDataTab);
	ui->blobDataTab->setVisible(false);

	try {
		tree* t = parse_1Ctext(textData.toStdString(), "");
		if (t != nullptr) {
			ui->treeView->setModel(new SkobkaTreeModel(t));
			ui->parsedDataTab->setVisible(true);
			ui->tabWidget->setCurrentWidget(ui->parsedDataTab);
		}
	} catch (...) {
		ui->parsedDataTab->setVisible(false);
	}
}

void BlobViewer::setStream(TStream *stream)
{

}
