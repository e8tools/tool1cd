#ifndef TABLEWINDOW_H
#define TABLEWINDOW_H

#include <QMainWindow>
#include <Table.h>

namespace Ui {
class TableFieldsWindow;
}

class TableFieldsWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit TableFieldsWindow(QWidget *parent, Table *table);
	~TableFieldsWindow();

private:
	Table *table;
	Ui::TableFieldsWindow *ui;
};

#endif // TABLEWINDOW_H
