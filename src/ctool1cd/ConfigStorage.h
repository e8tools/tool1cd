
#ifndef ConfigStorageH
#define ConfigStorageH

#include <System.Classes.hpp>
#include <vector>
#include <map>
#include <set>

#include "APIcfBase.h"
#include "TableFiles.h"
#include "Class_1CD.h"
#include "MessageRegistration.h"


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
	virtual ConfigFile* readfile(const String& path) override;
	virtual bool writefile(const String& path, TStream* str) override;
	virtual String presentation() override;
	virtual void close(ConfigFile* cf) override {delete cf->str; delete cf;}
	virtual bool fileexists(const String& path) override;
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
	virtual ConfigFile* readfile(const String& path) override;
	virtual bool writefile(const String& path, TStream* str) override;
	virtual String presentation() override;
	virtual void close(ConfigFile* cf) override;
	virtual bool fileexists(const String& path) override;
};

//---------------------------------------------------------------------------
// Перечисление признака упакованности файла
enum class table_file_packed
{
	unknown,
	no,
	yes
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

	container_file(table_file* _f, const String& _name);
	~container_file();
	bool open();
	bool ropen(); // raw open
	void close();
	bool isPacked();
};

//---------------------------------------------------------------------------
// Базовый класс адаптера таблицы - контейнера конфигурации (CONFIG, CONFICAS, CONFIGSAVE, CONFICASSAVE)
class ConfigStorageTable : public ConfigStorage, public IControlMessageRegistration
{
public:
	ConfigStorageTable(T_1CD* _base = NULL) : base(_base){};
	virtual ~ConfigStorageTable();
	virtual ConfigFile* readfile(const String& path) override;
	virtual bool writefile(const String& path, TStream* str) override;
	virtual void close(ConfigFile* cf) override;
	bool save_config(String _filename); // сохранение конфигурации в файл
	bool getready(){return ready;};
	virtual bool fileexists(const String& path) override;

	virtual void AddMessageRegistrator(MessageRegistrator* messageregistrator) override;
	virtual void RemoveMessageRegistrator() override;
protected:
	std::map<String,container_file*> files;
	bool ready{false};
private:
	T_1CD* base; // установлена, если база принадлежит адаптеру конфигурации
	Registrator msreg_m;

};

//---------------------------------------------------------------------------
// Класс адаптера таблицы - контейнера конфигурации CONFIG (конфигурации базы данных)
class ConfigStorageTableConfig : public ConfigStorageTable
{
public:
	ConfigStorageTableConfig(TableFiles* tabf, T_1CD* _base = NULL);
	virtual String presentation() override;
	virtual ~ConfigStorageTableConfig() {}

	void AddMessageRegistrator(MessageRegistrator* messageregistrator) override;
	void RemoveMessageRegistrator() override;
private:
	String present;
	Registrator msreg_m;
};

//---------------------------------------------------------------------------
// Класс адаптера таблицы - контейнера конфигурации CONFIGSAVE (основной конфигурации)
class ConfigStorageTableConfigSave : public ConfigStorageTable
{
public:
	ConfigStorageTableConfigSave(TableFiles* tabc, TableFiles* tabcs, T_1CD* _base = NULL);
	virtual String presentation() override;
	virtual ~ConfigStorageTableConfigSave() {}

	void AddMessageRegistrator(MessageRegistrator* messageregistrator) override;
	void RemoveMessageRegistrator() override;
private:
	String present;
	Registrator msreg_m;
};

//---------------------------------------------------------------------------
// Класс адаптера таблицы - контейнера конфигурации CONFIGCAS (расширения конфигурации базы данных)
class ConfigStorageTableConfigCas : public ConfigStorageTable
{
public:
	ConfigStorageTableConfigCas(TableFiles* tabc, const String& configver, T_1CD* _base = NULL);
	virtual String presentation() override;
	virtual ~ConfigStorageTableConfigCas() {}

	void AddMessageRegistrator(MessageRegistrator* messageregistrator) override;
	void RemoveMessageRegistrator() override;
private:
	String present;
	Registrator msreg_m;
};

//---------------------------------------------------------------------------
// Класс адаптера таблицы - контейнера конфигурации CONFIGCASSAVE (расширения основной конфигурации)
class ConfigStorageTableConfigCasSave : public ConfigStorageTable
{
public:
	ConfigStorageTableConfigCasSave(TableFiles* tabc, TableFiles* tabcs, const TGUID& uid, const String& configver, T_1CD* _base = NULL);
	virtual String presentation() override;
	virtual ~ConfigStorageTableConfigCasSave() {}

	void AddMessageRegistrator(MessageRegistrator* messageregistrator) override;
	void RemoveMessageRegistrator() override;
private:
	String present;
	Registrator msreg_m;
};



//---------------------------------------------------------------------------
#endif

