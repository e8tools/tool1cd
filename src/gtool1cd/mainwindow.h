#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Class_1CD.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	void open(T_1CD *database);
	~MainWindow();

	void addLogMessage(const QString &message);

private slots:


private:
	Ui::MainWindow *ui;
	T_1CD *db;
};

#endif // MAINWINDOW_H
