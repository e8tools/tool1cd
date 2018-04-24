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
#include "cache.h"
#include <QStandardPaths>
#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QDebug>

QString getCacheFilename()
{
	QDir cacheDir = QDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
	if (!cacheDir.exists()) {
		qDebug() << cacheDir.path();
		cacheDir.mkpath(".");
	}
	QString cacheFileName = QDir::cleanPath(cacheDir.absoluteFilePath(".gtool1cd.list"));
	qDebug() << cacheFileName;
	return cacheFileName;
}

Cache::Cache()
    : Cache(getCacheFilename())
{

}

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

