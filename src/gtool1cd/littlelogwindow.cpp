#include "littlelogwindow.h"
#include "ui_littlelogwindow.h"
#include <QFileDialog>
#include <QDebug>
#include <MessageRegistration.h>

LittleLogWindow::LittleLogWindow(QWidget *parent) :
	QMainWindow(parent),
    ui(new Ui::LittleLogWindow)
{
	ui->setupUi(this);
}

LittleLogWindow::~LittleLogWindow()
{
	delete ui;
}


void LittleLogWindow::addLogMessage(const QString &message)
{
	qDebug() << message;
	ui->logList->addItem(message);
}
