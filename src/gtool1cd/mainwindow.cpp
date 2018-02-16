#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QStringListModel>
#include "starter.h"
#include "cache.h"
#include "table_fields_window.h"
#include "table_data_window.h"
#include "models/tables_list_model.h"

Registrator msreg_g;

TMultiReadExclusiveWriteSynchronizer*
tr_syn = new TMultiReadExclusiveWriteSynchronizer();

class MainWindowRegistrator : public MessageRegistrator
{
public:
	MainWindowRegistrator(MainWindow *window)
	    : target(window)
	{

	}

	virtual void AddMessage(
	        const String &description,
	        const MessageState mstate,
	        const TStringList *param = nullptr) override
	{
		qDebug() << QString(description.c_str());
		target->addLogMessage(QString(description.c_str()));
	}
	virtual void Status(const String& message) override
	{

	}
private:
	MainWindow *target;
};

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	ui->logList->setModel(new QStringListModel(logData));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::open(T_1CD *database)
{
	db = database;
	ui->tableListView->setModel(new TablesListModel(db));
	setWindowTitle(QString::fromStdString(static_cast<std::string>(db->getfilename())));
	// refresh data
}

void MainWindow::addLogMessage(const QString &message)
{
	logData.append(message);
}

void MainWindow::on_exitAction_triggered()
{
	for (auto w : table_windows) {
		delete w;
	}
	close();
}

void MainWindow::on_openDatabaseFileAction_triggered()
{
	StarterWindow *w = new StarterWindow();
	w->setCache(new Cache());
	w->show();
}

void MainWindow::on_tableListView_doubleClicked(const QModelIndex &index)
{
	Table *t = db->gettable(index.row());
	if (table_windows.find(t) == table_windows.end()) {
		table_windows[t] = new TableDataWindow(this, t);
	}
	table_windows[t]->show();
	table_windows[t]->activateWindow();
}
