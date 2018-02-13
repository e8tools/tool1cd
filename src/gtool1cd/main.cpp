#include "starter.h"
#include <QApplication>
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include "cache.h"
#include <QDebug>

int main(int argc, char *argv[])
{
	QDir cacheDir = QDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
	if (!cacheDir.exists()) {
		cacheDir.mkpath("");
	}
	QString cacheFileName = QDir::cleanPath(cacheDir.absoluteFilePath(".gtool1cd.list"));
	qDebug() << cacheFileName;
	Cache cache(cacheFileName);

	QApplication a(argc, argv);
	StarterWindow w;
	w.setCache(&cache);
	w.show();

	return a.exec();
}
