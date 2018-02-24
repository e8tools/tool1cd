#include "starter.h"
#include "ui_starter.h"
#include <QFileDialog>
#include <QDebug>
#include <QStringListModel>
#include "mainwindow.h"
#include "littlelogwindow.h"
#include "cache.h"

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

bool StarterWindow::openDatabase(const QString &filename)
{
	LittleLogWindow *lw = new LittleLogWindow(this);
	RegistratorSwitcher *reg = new RegistratorSwitcher(lw);
	msreg_g.AddMessageRegistrator(reg);
	T_1CD *db = nullptr;
	try {

		db = new T_1CD(filename.toStdString(), reg);
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
