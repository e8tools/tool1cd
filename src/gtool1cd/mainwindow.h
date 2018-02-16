#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Class_1CD.h>
#include <QMap>

class Table;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow, public MessageRegistrator
{
	Q_OBJECT

public:
	explicit MainWindow(QWidget *parent = 0);
	void open(T_1CD *database);
	~MainWindow();

	void addLogMessage(const QString &message);

	virtual void AddMessage(
	        const String &description,
	        const MessageState mstate,
	        const TStringList *param = nullptr) override;

	virtual void Status(const String& message) override;

private slots:


	void on_exitAction_triggered();

	void on_openDatabaseFileAction_triggered();

	void on_tableListView_doubleClicked(const QModelIndex &index);

private:
	Ui::MainWindow *ui;
	T_1CD *db;
	QStringList logData;
	QMap<Table *, QWidget *> table_windows;
};

#endif // MAINWINDOW_H
