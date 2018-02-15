#ifndef TABLEDATAWINDOW_H
#define TABLEDATAWINDOW_H

#include <QMainWindow>
#include <QAbstractItemModel>
#include <Table.h>
#include <TableIterator.h>

namespace Ui {
class TableDataWindow;
}

class TableDataWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit TableDataWindow(QWidget *parent, Table *table);
	~TableDataWindow();

private slots:

	void on_descriptionButton_clicked();

	void on_fieldsButton_clicked();

private:
	Table *table;
	Ui::TableDataWindow *ui;
	QWidget *tableWindow;
};

#endif // TABLEDATAWINDOW_H
