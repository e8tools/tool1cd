#include "starter.h"
#include "ui_starter.h"
#include <QFileDialog>
#include <QDebug>

StarterWindow::StarterWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::StarterWindow)
{
	ui->setupUi(this);
}

StarterWindow::~StarterWindow()
{
	delete ui;
}

void StarterWindow::on_openNewButton_clicked()
{
	QString filename = QFileDialog::getOpenFileName();
	if (filename == nullptr) {
		return;
	}
	openDatabase(filename);
}

void StarterWindow::openDatabase(const QString &filename)
{

}
