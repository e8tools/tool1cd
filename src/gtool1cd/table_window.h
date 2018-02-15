#ifndef TABLEWINDOW_H
#define TABLEWINDOW_H

#include <QMainWindow>
#include <Table.h>

namespace Ui {
class TableWindow;
}

class TableWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit TableWindow(QWidget *parent, Table *table);
	~TableWindow();

private:
	Table *table;
	Ui::TableWindow *ui;
};

#endif // TABLEWINDOW_H
