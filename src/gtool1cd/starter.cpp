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
#include "starter.h"
#include "ui_starter.h"
#include <QFileDialog>
#include <QDebug>
#include <QStringListModel>
#include "mainwindow.h"
#include "littlelogwindow.h"
#include "cache.h"
#include <QDragEnterEvent>
#include <QDropEvent>
#include <QMimeData>
#include "container_form.h"

Cache *global_cache = nullptr;
extern Registrator msreg_g;

class RegistratorSwitcher : public MessageRegistrator
{
public:

	explicit RegistratorSwitcher(MessageRegistrator *target)
	    : _target(target) {}

	void setRegistrator(MessageRegistrator *target)
	{
		_target = target;
	}

	virtual void AddDetailedMessage(
	        const std::string &description,
	        const MessageState mstate,
	        const TStringList *param = nullptr) override
	{
		qDebug() << QString(description.c_str());
		_target->AddDetailedMessage(description, mstate, param);
	}
	virtual void Status(const std::string& message) override
	{
		qDebug() << QString(message.c_str());
		_target->Status(message);
	}
private:
	MessageRegistrator *_target;
};

StarterWindow::StarterWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::StarterWindow)
{
	ui->setupUi(this);
	setAcceptDrops(true);
}

StarterWindow::~StarterWindow()
{
	delete ui;
}

void StarterWindow::on_openNewButton_clicked()
{
	QString filename = QFileDialog::getOpenFileName(
	            this, tr("Open database"), "",
	            tr("Databases (*.1CD *.1cd);;"
	               "Containers (*.epf *.erf *.cf *.cfu);;"
	               "All files (*.*);;"));
	if (filename == nullptr) {
		return;
	}
	if (openDatabase(filename)) {
		this->cache->addDbToList(filename);
	}
}

void StarterWindow::setCache(Cache *cache)
{
	this->cache = cache;
	QStringList list = this->cache->getCachedList();
	ui->listView->setModel(new QStringListModel(list));
}

bool is_container_file(const QString &filename)
{
	QFile f(filename);
	f.open(QIODevice::ReadOnly);
	uint32_t signature;
	if (f.read(reinterpret_cast<char*>(&signature), 4) != 4) {
		return false;
	}
	if (signature != 0x7fffffff) {
		return false;
	}
	return true;
}

bool StarterWindow::openDatabase(const QString &filename)
{
	if (is_container_file(filename)) {
		ContainerForm *cf_window = new ContainerForm(nullptr);
		cf_window->open(filename);
		cf_window->show();
		close();
		return true;
	}

	LittleLogWindow *lw = new LittleLogWindow(this);
	RegistratorSwitcher *reg = new RegistratorSwitcher(lw);
	msreg_g.AddMessageRegistrator(reg);
	T_1CD *db = nullptr;
	try {

		db = new T_1CD(boost::filesystem::path(filename.toStdWString()), reg);
		if (!db->is_open()) {
			lw->show();
			return false;
		}

	} catch (DetailedException ex) {

		ex.show();
		lw->show();
		return false;

	}
	MainWindow *db_window = new MainWindow(nullptr);
	reg->setRegistrator(db_window);
	db_window->open(db);
	db_window->show();
	close();
	return true;
}

void StarterWindow::on_listView_doubleClicked(const QModelIndex &index)
{
	openDatabase(index.data().toString());
}

void StarterWindow::on_listView_activated(const QModelIndex &index)
{
	openDatabase(index.data().toString());
}

void StarterWindow::dragEnterEvent(QDragEnterEvent *event)
{
	event->acceptProposedAction();
}

void StarterWindow::dropEvent(QDropEvent *event)
{
	auto mimeData = event->mimeData();
	if (!mimeData->hasUrls()) {
		return;
	}

	for (auto &url: mimeData->urls()) {

		if (!url.isLocalFile()) {
			continue;
		}

		if (openDatabase(url.toLocalFile())) {
			event->acceptProposedAction();
			return;
		}
	}
}
