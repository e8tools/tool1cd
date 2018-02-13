#include "cache.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QTextStream>


Cache::Cache(const QString &cacheFileName)
    : cacheFileName(cacheFileName)
{}

QStringList Cache::getCachedList() const
{
	QStringList list;

	if (QFile::exists(cacheFileName)) {
		QFile cacheFile(cacheFileName);
		cacheFile.open(QFile::ReadOnly);
		QTextStream stream(&cacheFile);
		while (true) {
			QString newLine = stream.readLine();
			if (newLine.isNull()) {
				break;
			}
			if (!newLine.isEmpty()) {
				list.append(newLine);
			}
		}
	}
	return list;
}

void Cache::addDbToList(const QString &dbFileName)
{
	QFile cacheFile(cacheFileName);
	if (cacheFile.exists()) {
		cacheFile.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text);
	} else {
		cacheFile.open(QFile::WriteOnly | QFile::Text);
	}
	QTextStream s(&cacheFile);
	s << dbFileName << "\n";
}

