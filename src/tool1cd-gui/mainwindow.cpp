#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QDebug>

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

void MainWindow::on_action_5_triggered()
{
	close();
}

void MainWindow::on_action_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this, ("Open base"), "", ("Bases (*.1CD)"));
	if (fileName.isNull() || fileName.isEmpty()) {
		return;
	}
}
