#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

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
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::open(T_1CD *database)
{
	db = database;
	// refresh data
}

void MainWindow::addLogMessage(const QString &message)
{
	//ui->logList->addItem(message);
}
