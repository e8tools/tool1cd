/*
    GTool1CD provides GUI front end to Tool1CD library
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of GTool1CD.

    GTool1CD is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GTool1CD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GTool1CD.  If not, see <http://www.gnu.org/licenses/>.
*/
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
#include "about_dialog.h"
#include <QSortFilterProxyModel>
#include <QMenu>
#include <QMessageBox>

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
	
	ui->tableListView->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->tableListView, SIGNAL(customContextMenuRequested(QPoint)),
			this, SLOT(show_table_context_menu(QPoint)));
}

void MainWindow::show_table_context_menu(const QPoint &pos) 
{
	QMenu contextMenu(tr("Context menu"), this);
	
	QAction action_export_blob(tr("Экспорт BLOB"), this);
	connect(&action_export_blob, SIGNAL(triggered()), this, SLOT(export_blob_file()));
	contextMenu.addAction(&action_export_blob);
	
	QAction action_import_blob(tr("Импорт BLOB"), this);
	connect(&action_import_blob, SIGNAL(triggered()), this, SLOT(import_blob_file()));
	contextMenu.addAction(&action_import_blob);

	contextMenu.addSeparator();

	QAction action_clear_table(tr("Очистить таблицу"), this);
	connect(&action_clear_table, SIGNAL(triggered()), this, SLOT(clear_table_action()));
	contextMenu.addAction(&action_clear_table);

	contextMenu.exec(mapToGlobal(pos));
}

void MainWindow::export_blob_file()
{
	auto indexes = ui->tableListView->selectionModel()->selectedIndexes();
	if (indexes.empty()) {
		return;
	}
	QString dir = QFileDialog::getExistingDirectory(this);
	if (dir.isNull()) {
		return;
	}
	
	for (auto &index : indexes) {
		Table *t = db->get_table(index.row());
		t->export_table(boost::filesystem::path(dir.toStdWString()));
	}
}

void MainWindow::import_blob_file()
{
	auto indexes = ui->tableListView->selectionModel()->selectedIndexes();
	if (indexes.empty()) {
		return;
	}
	QString dir = QFileDialog::getExistingDirectory(this);
	if (dir.isNull()) {
		return;
	}
	
	boost::filesystem::path rootpath(dir.toStdWString());
	for (auto &index : indexes) {
		Table *t = db->get_table(index.row());
		t->import_table(rootpath);
	}
}

void MainWindow::clear_table_action()
{
	auto indexes = ui->tableListView->selectionModel()->selectedIndexes();
	if (indexes.empty()) {
		return;
	}

	auto *proxy = qobject_cast<QSortFilterProxyModel*>(ui->tableListView->model());
	QModelIndex src = proxy ? proxy->mapToSource(indexes.first()) : indexes.first();
	Table *t = db->get_table(src.row());

	if (db->get_readonly()) {
		QMessageBox::warning(this, tr("Очистка таблицы"),
		        tr("База открыта только для чтения — очистка невозможна."));
		return;
	}

	if (QMessageBox::warning(this, tr("Очистка таблицы"),
	        tr("Пометить удалёнными ВСЕ записи таблицы «%1»?\nДействие изменяет базу и необратимо.")
	            .arg(QString::fromStdString(t->get_name())),
	        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) != QMessageBox::Yes) {
		return;
	}

	uint32_t deleted = 0;
	try {
		t->begin_edit();
		uint32_t total = t->get_phys_numrecords();
		for (uint32_t i = 1; i < total; i++) {
			auto *rec = t->get_record(i);
			bool removed = rec->is_removed();
			delete rec;
			if (!removed) {
				t->mark_record_removed(i);
				deleted++;
			}
		}
		db->flush();
	} catch (DetailedException &ex) {
		QMessageBox::critical(this, tr("Очистка таблицы"), QString(ex.what()));
		return;
	}

	auto it = table_windows.find(t);
	if (it != table_windows.end()) {
		static_cast<TableDataWindow*>(it.value())->reload();
	}

	QMessageBox::information(this, tr("Очистка таблицы"),
	        tr("Готово. Помечено удалёнными записей: %1.").arg(deleted));
}

MainWindow::~MainWindow()
{
	delete ui;
}

void MainWindow::open(T_1CD *database)
{
	db = database;
	auto *model = new TablesListModel(db);
	auto *proxy = new QSortFilterProxyModel(this);
	proxy->setSourceModel(model);
	proxy->setSortRole(Qt::UserRole);
	proxy->setDynamicSortFilter(true);
	ui->tableListView->setModel(proxy);
	ui->tableListView->setSortingEnabled(true);
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
	auto *proxy = qobject_cast<QSortFilterProxyModel*>(ui->tableListView->model());
	QModelIndex src = proxy ? proxy->mapToSource(index) : index;
	Table *t = db->get_table(src.row());
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

void MainWindow::on_aboutAction_triggered()
{
	if (about_window == nullptr) {
		about_window = new AboutDialog(this);
	}
	about_window->show();
}
