#include "blob_viewer.h"
#include "ui_blob_viewer.h"
#include <Parse_tree.h>
#include "../models/skobka_tree_model.h"
#include "../models/v8catalog_tree_model.h"
#include "../QHexEdit/qhexedit.h"
#include "../models/stream_device.h"
#include <Table.h>

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
	ui->tabWidget->removeTab(0);
	ui->tabWidget->removeTab(0);
	ui->tabWidget->removeTab(0);

	QTextDocument *qd = new QTextDocument(textData);
	qd->setDocumentLayout(new QPlainTextDocumentLayout(qd));
	ui->plainTextEdit->setDocument(qd);

	ui->tabWidget->addTab(ui->textDataTab, tr("Текст"));
	ui->tabWidget->setCurrentWidget(ui->textDataTab);

	if (textData.startsWith("{")) {
		try {
			tree* t = parse_1Ctext(textData.toStdString(), "");
			if (t != nullptr) {
				ui->treeView->setModel(new SkobkaTreeModel(t));
				ui->tabWidget->addTab(ui->parsedDataTab, tr("Дерево"));
				ui->tabWidget->setCurrentWidget(ui->parsedDataTab);
			}
		} catch (...) {
		}
	}
}

// проверяет наличие UTF-8 BOM
bool is_text_stream(QIODevice *stream)
{
	auto buf = stream->read(3);
	if (buf.size() != 3) {
		return false;
	}
	return buf[0] == '\xEF'
	        && buf[1] == '\xBB'
	        && buf[2] == '\xBF';
}

void BlobViewer::setStream(TStream *stream, const QString &rootName)
{
	ui->tabWidget->removeTab(0);
	ui->tabWidget->removeTab(0);
	ui->tabWidget->removeTab(0);

	auto cat = new V8Catalog(stream, false, true);
	V8CatalogTreeModel *catalog_model = nullptr;
	if (cat->isOpen() && cat->is_catalog()) {
		catalog_model = new V8CatalogTreeModel(cat, rootName);
	} else {
		delete cat;
	}

	auto device = new StreamDevice(stream);

	QString textData;
	{
		device->open(QIODevice::ReadOnly);
		if (is_text_stream(device)) {
			textData = QString(device->readAll());
		}
	}
	device->close();

	auto doc = QHexDocument::fromDevice(device);
	ui->frame->setDocument(doc);

	ui->tabWidget->addTab(ui->blobDataTab, tr("BLOB"));
	ui->tabWidget->setCurrentWidget(ui->blobDataTab);

	{
		if (catalog_model) {
			ui->tabWidget->addTab(ui->parsedDataTab, tr("Дерево"));
			ui->tabWidget->setCurrentWidget(ui->parsedDataTab);
			ui->treeView->setModel(catalog_model);
			ui->treeView->expandAll();
			return;
		}

		if (textData.size() != 0) {

			ui->tabWidget->addTab(ui->textDataTab, tr("Текст"));
			ui->tabWidget->setCurrentWidget(ui->textDataTab);

			QTextDocument *qd = new QTextDocument(textData);
			qd->setDocumentLayout(new QPlainTextDocumentLayout(qd));
			ui->plainTextEdit->setDocument(qd);
			ui->plainTextEdit->setDocumentTitle(rootName);

			if (textData.startsWith("{")) {
				try {
					tree* t = parse_1Ctext(textData.toStdString(), "");
					if (t != nullptr) {
						ui->treeView->setModel(new SkobkaTreeModel(t));
						ui->treeView->expandAll();
						ui->tabWidget->addTab(ui->parsedDataTab, tr("Дерево"));
						ui->tabWidget->setCurrentWidget(ui->parsedDataTab);
					}
				} catch (...) {
				}
			}
		}
	}

}
