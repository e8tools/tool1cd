//---------------------------------------------------------------------------

#ifndef ConfigStorageH
#define ConfigStorageH

#include <System.Classes.hpp>
#include <vector>
#include <map>
#include <set>

#include "APIcfBase.h"
#include "Class_1CD.h"
//---------------------------------------------------------------------------

//class T_1CD;
//class table;

//---------------------------------------------------------------------------
// Структура открытого файла адаптера контейнера конфигурации
struct ConfigFile
{
	TStream* str;
	void* addin;
};

//---------------------------------------------------------------------------
// Базовый класс адаптеров контейнеров конфигурации
class ConfigStorage
{
public:
	ConfigStorage(){};
	virtual ~ConfigStorage(){};
	virtual ConfigFile* readfile(const String& path) = 0; // Если файл не существует, возвращается NULL
	virtual bool writefile(const String& path, TStream* str) = 0;
	virtual String presentation() = 0;
	virtual void close(ConfigFile* cf) = 0;
	virtual bool fileexists(const String& path) = 0;
};

//---------------------------------------------------------------------------
// Класс адаптера контейнера конфигурации - Директория
class ConfigStorageDirectory : public ConfigStorage
{
private:
	String fdir;
public:
	ConfigStorageDirectory(const String& _dir);
//	__property String dir = {read = fdir};
	virtual ConfigFile* readfile(const String& path);
	virtual bool writefile(const String& path, TStream* str);
	virtual String presentation();
	virtual void close(ConfigFile* cf){delete cf->str; delete cf;};
	virtual bool fileexists(const String& path);
	virtual ~ConfigStorageDirectory() {}
};

//---------------------------------------------------------------------------
// Класс адаптера контейнера конфигурации - cf (epf, erf, cfe) файл
class ConfigStorageCFFile : public ConfigStorage
{
private:
	String filename;
	v8catalog* cat;
public:
	ConfigStorageCFFile(const String& fname);
	virtual ~ConfigStorageCFFile();
	virtual ConfigFile* readfile(const String& path);
	virtual bool writefile(const String& path, TStream* str);
	virtual String presentation();
	virtual void close(ConfigFile* cf);
	virtual bool fileexists(const String& path);
};

//---------------------------------------------------------------------------
// Перечисление признака упакованности файла
enum table_file_packed
{
	tfp_unknown,
	tfp_no,
	tfp_yes
};

//---------------------------------------------------------------------------
// Структура файла контейнера файлов
struct container_file
{
	table_file* file;
	String name; // Приведенное имя (очищенное от динамического обновления)
	TStream* stream;
	TStream* rstream; // raw stream (нераспакованный поток)
	v8catalog* cat;
	table_file_packed packed;
	int dynno; // Номер (индекс) динамического обновления (0, 1 и т.д.). Если без динамического обновления, то -1, если UID динамического обновления не найден, то -2. Для пропускаемых файлов -3.
	//static wchar_t temppath[MAX_PATH];

	container_file(table_file* _f, const String& _name);
	~container_file();
	bool open();
	bool ropen(); // raw open
	void close();
	bool isPacked();
};

//---------------------------------------------------------------------------
// Базовый класс адаптера таблицы - контейнера конфигурации (CONFIG, CONFICAS, CONFIGSAVE, CONFICASSAVE)
class ConfigStorageTable : public ConfigStorage
{
private:
	T_1CD* base; // установлена, если база принадлежит адаптеру конфигурации
protected:
	std::map<String,container_file*> files;
	bool ready{false};
public:
	ConfigStorageTable(T_1CD* _base = NULL) : base(_base){};
	virtual ~ConfigStorageTable();
	virtual ConfigFile* readfile(const String& path);
	virtual bool writefile(const String& path, TStream* str);
	virtual void close(ConfigFile* cf);
	bool save_config(String _filename); // сохранение конфигурации в файл
	bool getready(){return ready;};
	virtual bool fileexists(const String& path);
};

//---------------------------------------------------------------------------
// Класс адаптера таблицы - контейнера конфигурации CONFIG (конфигурации базы данных)
class ConfigStorageTableConfig : public ConfigStorageTable
{
private:
	String present;
public:
	ConfigStorageTableConfig(TableFiles* tabf, T_1CD* _base = NULL);
	virtual String presentation();
	virtual ~ConfigStorageTableConfig() {}
};

//---------------------------------------------------------------------------
// Класс адаптера таблицы - контейнера конфигурации CONFIGSAVE (основной конфигурации)
class ConfigStorageTableConfigSave : public ConfigStorageTable
{
private:
	String present;
public:
	ConfigStorageTableConfigSave(TableFiles* tabc, TableFiles* tabcs, T_1CD* _base = NULL);
	virtual String presentation();
	virtual ~ConfigStorageTableConfigSave() {}
};

//---------------------------------------------------------------------------
// Класс адаптера таблицы - контейнера конфигурации CONFIGCAS (расширения конфигурации базы данных)
class ConfigStorageTableConfigCas : public ConfigStorageTable
{
private:
	String present;
public:
	ConfigStorageTableConfigCas(TableFiles* tabc, const String& configver, T_1CD* _base = NULL);
	virtual String presentation();
	virtual ~ConfigStorageTableConfigCas() {}
};

//---------------------------------------------------------------------------
// Класс адаптера таблицы - контейнера конфигурации CONFIGCASSAVE (расширения основной конфигурации)
class ConfigStorageTableConfigCasSave : public ConfigStorageTable
{
private:
	String present;
public:
	ConfigStorageTableConfigCasSave(TableFiles* tabc, TableFiles* tabcs, const TGUID& uid, const String& configver, T_1CD* _base = NULL);
	virtual String presentation();
	virtual ~ConfigStorageTableConfigCasSave() {}
};



//---------------------------------------------------------------------------
#endif

