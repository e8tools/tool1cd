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
#include "configurations_window.h"

void MainWindow::AddDetailedMessage(
        const std::string &description,
        const MessageState mstate,
        const TStringList *param)
{
	this->addLogMessage(QString(description.c_str()));
}

void MainWindow::Status(const std::string& message)
{

}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    configurationsWindow(nullptr)
{
	ui->setupUi(this);
	ui->logList->setModel(new QStringListModel(logData));
	ui->logList->setVisible(false);
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::open(T_1CD *database)
{
	db = database;
	ui->tableListView->setModel(new TablesListModel(db));
	setWindowTitle(QString::fromStdWString(db->get_filepath().wstring()));
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
	Table *t = db->get_table(index.row());
	if (table_windows.find(t) == table_windows.end()) {
		table_windows[t] = new TableDataWindow(this, t);
	}
	table_windows[t]->show();
	table_windows[t]->activateWindow();
}

void MainWindow::on_configurationsButton_clicked()
{
	if (configurationsWindow == nullptr) {
		configurationsWindow = new ConfigurationsWindow(db, this);
	}
	configurationsWindow->show();
	configurationsWindow->activateWindow();
}

void MainWindow::on_tableListView_activated(const QModelIndex &index)
{
	emit ui->tableListView->doubleClicked(index);
}
