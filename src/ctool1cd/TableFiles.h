/*
 *
 *
 *
 *
 */

#ifndef SRC_CTOOL1CD_TABLEFILES_H_
#define SRC_CTOOL1CD_TABLEFILES_H_

#include "Common.h"
#include "Table.h"

//---------------------------------------------------------------------------
// Структура адреса файла таблицы-контейнера файлов
struct table_blob_file
{
	uint32_t blob_start;
	uint32_t blob_length;
};

//---------------------------------------------------------------------------
// Структура записи таблицы контейнера файлов
struct table_rec
{
	String name;
	table_blob_file addr;
	int32_t partno;
	FILETIME ft_create;
	FILETIME ft_modify;
};

//---------------------------------------------------------------------------
// Структура файла таблицы контейнера файлов
struct table_file
{
	Table* t;
	String name; // Имя, как оно хранится в таблице
	uint32_t maxpartno;
	table_blob_file* addr;
	FILETIME ft_create;
	FILETIME ft_modify;

	table_file(Table* _t, const String& _name, uint32_t _maxpartno);
	~table_file();
};

//---------------------------------------------------------------------------
// Класс таблицы контейнера файлов (CONFIG, CONFIGSAVE, PARAMS, FILES, CONFICAS, CONFICASSAVE)
class TableFiles
{
public:
	TableFiles(Table* t);
	virtual ~TableFiles();
	bool getready(){return ready;}
	table_file* getfile(const String& name);
	Table* gettable(){return tab;}

	// __property std::map<String,table_file*> files = {read = allfiles};
	std::map<String,table_file*> &files();
private:
	Table* tab;
	std::map<String,table_file*> allfiles;
	char* rec;
	bool ready;

	bool test_table();
};


#endif /* SRC_CTOOL1CD_TABLEFILES_H_ */
