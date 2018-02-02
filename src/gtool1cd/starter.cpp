#include "starter.h"
#include "ui_starter.h"
#include <QFileDialog>
#include <QDebug>
#include "mainwindow.h"
#include "littlelogwindow.h"

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
	QString filename = QFileDialog::getOpenFileName(
	            this, tr("Open database"), "",
	            tr("Databases (*.1CD *.1cd);;All files (*.*)"));
	if (filename == nullptr) {
		return;
	}
	openDatabase(filename);
}

void StarterWindow::openDatabase(const QString &filename)
{
	LittleLogWindow *lw = new LittleLogWindow(this);
	T_1CD *db = new T_1CD(filename.toStdString(), lw);
	if (!db->is_open()) {
		lw->show();
		return;
	}
	MainWindow *db_window = new MainWindow(nullptr);
	db_window->open(db);
	db_window->show();
	close();
}
