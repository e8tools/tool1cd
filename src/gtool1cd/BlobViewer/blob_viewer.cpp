#include "blob_viewer.h"
#include "ui_blob_viewer.h"
#include <Parse_tree.h>
#include "../models/skobka_tree_model.h"
#include "../models/v8catalog_tree_model.h"
#include "../QHexEdit/qhexedit.h"
#include "../models/stream_device.h"
#include <Table.h>
#include <QDebug>

BlobViewer::BlobViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlobViewer)
{
	ui->setupUi(this);
	auto font = ui->plainTextEdit->font();
	font.setStyleHint(QFont::TypeWriter);
	ui->plainTextEdit->setFont(font);

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

	ui->plainTextEdit->setPlainText(textData);

	ui->tabWidget->addTab(ui->textDataTab, tr("Текст"));
	ui->tabWidget->setCurrentWidget(ui->textDataTab);

	if (textData.startsWith("{")) {
		try {
			auto t = parse_1Ctext(textData.toStdString(), "");
			if (t != nullptr) {
				ui->treeView->setModel(new SkobkaTreeModel( std::move(t) ));
				ui->tabWidget->addTab(ui->parsedDataTab, tr("Дерево"));
				ui->tabWidget->setCurrentWidget(ui->parsedDataTab);
			}
		} catch (...) {
		}
	}
}

QString extract_text_data(QIODevice *device)
{
	auto buf = device->read(3);
	if (buf.size() != 3) {
		return QString::null;
	}
	if (buf[0] == '\xEF'
	    && buf[1] == '\xBB'
	    && buf[2] == '\xBF') {
		return QString(device->readAll());
	}

	if (buf[0] == 'M'
	        && buf[1] == 'O'
	        && buf[2] == 'X') {
		buf = device->read(3);
		if (buf.size() != 3) {
			return QString::null;
		}
		if (buf[0] == 'C'
		        && buf[1] == 'E'
		        && buf[2] == 'L') {
			device->read(7); // заголовочные данные
			device->read(3); // BOM
			return QString(device->readAll());
		}
	}
	return QString::null;
}

void BlobViewer::setStream(TStream *stream, const QString &rootName)
{
	this->rootName = rootName;
	setWindowTitle(rootName);

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
	device->open(QIODevice::ReadOnly);

	QString textData = extract_text_data(device);

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

		if (!textData.isNull()) {

			ui->tabWidget->addTab(ui->textDataTab, tr("Текст"));
			ui->tabWidget->setCurrentWidget(ui->textDataTab);

			ui->plainTextEdit->setPlainText(textData);
			ui->plainTextEdit->setDocumentTitle(rootName);

			if (textData.startsWith("{")) {
				try {
					auto t = parse_1Ctext(textData.toStdString(), "");
					if (t != nullptr) {
						ui->treeView->setModel(new SkobkaTreeModel( std::move(t) ));
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

void BlobViewer::on_treeView_doubleClicked(const QModelIndex &index)
{
	auto model = ui->treeView->model();
	auto data = model->data(index, Qt::EditRole);
	auto as_stream = static_cast<StreamDevice*>(data.value<QIODevice*>());
	if (as_stream != nullptr) {
		QString elementName = model->data(index, Qt::DisplayRole).toString();
		BlobViewer *new_window = new BlobViewer(nullptr);
		new_window->setStream(as_stream->get_stream(), rootName + QString(" / ") + elementName);
		new_window->show();
	}
}
