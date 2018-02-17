/*
 *
 *
 *
 *
 */

#ifndef SRC_CTOOL1CD_TABLEFILES_H_
#define SRC_CTOOL1CD_TABLEFILES_H_

#include <map>

#include "Common.h"
#include "Table.h"
#include "TableRecord.h"

//---------------------------------------------------------------------------
// Структура записи таблицы контейнера файлов
struct table_rec
{
	String name;
	table_blob_file addr;
	int32_t partno;
	System::FILETIME ft_create;
	System::FILETIME ft_modify;
};

//---------------------------------------------------------------------------
// Структура файла таблицы контейнера файлов
struct TableFile
{
	Table* t;
	std::string name; // Имя, как оно хранится в таблице
	uint32_t maxpartno;
	table_blob_file* addr;
	System::FILETIME ft_create;
	System::FILETIME ft_modify;

	TableFile(Table *_t, const std::string &_name, uint32_t _maxpartno);
	~TableFile();
};

//---------------------------------------------------------------------------
// Класс таблицы контейнера файлов (CONFIG, CONFIGSAVE, PARAMS, FILES, CONFICAS, CONFICASSAVE)
class TableFiles
{
public:
	explicit TableFiles(Table* t);
	virtual ~TableFiles();

	bool getready() { return ready; }
	TableFile* getfile(const std::string &name);
	Table* gettable() { return table; }

	std::map<String,TableFile*> &files();
private:
	Table* table {nullptr};
	std::map<String,TableFile*> allfiles;
	bool ready {false};

	bool test_table();
};


#endif /* SRC_CTOOL1CD_TABLEFILES_H_ */
