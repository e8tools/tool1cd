#include "starter.h"
#include <QApplication>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "cache.h"
#include <QDebug>
#include <MessageRegistration.h>

Registrator msreg_g;

TMultiReadExclusiveWriteSynchronizer*
tr_syn = new TMultiReadExclusiveWriteSynchronizer();



int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	StarterWindow w;
	w.setCache(new Cache());
	w.show();

	return a.exec();
}
