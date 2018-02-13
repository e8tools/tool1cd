#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <QStringListModel>
#include "starter.h"
#include "cache.h"

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
	// refresh data
}

void MainWindow::addLogMessage(const QString &message)
{
	logData.append(message);
}

void MainWindow::on_exitAction_triggered()
{
	close();
}

void MainWindow::on_openDatabaseFileAction_triggered()
{
	StarterWindow *w = new StarterWindow();
	w->setCache(new Cache());
	w->show();
}
