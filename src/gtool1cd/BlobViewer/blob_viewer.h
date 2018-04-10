#ifndef BLOB_VIEWER_H
#define BLOB_VIEWER_H

#include <QWidget>
#include <TStream.hpp>

namespace Ui {
class BlobViewer;
}

class BlobViewer : public QWidget
{
	Q_OBJECT

public:
	explicit BlobViewer(QWidget *parent = 0);
	~BlobViewer();

	void setText(const QString &textData, bool do_not_hide_tabs = false);

	void setStream(TStream *stream, const QString &rootName);

private slots:
	void on_treeView_doubleClicked(const QModelIndex &index);

private:
	QString rootName;
	Ui::BlobViewer *ui;
};

#endif // BLOB_VIEWER_H
