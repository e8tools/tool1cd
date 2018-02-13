#ifndef CACHE_INCLUDED
#define CACHE_INCLUDED

#include <QString>
#include <QStringList>

class Cache
{
public:

	Cache(const QString &cacheFileName);

	QStringList getCachedList() const;
	void addDbToList(const QString &dbFileName);

	QString cacheFileName;
};

#endif // CACHE_INCLUDED
