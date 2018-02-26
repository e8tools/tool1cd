
#ifndef ConfigStorageH
#define ConfigStorageH

#include <boost/filesystem.hpp>
#include <System.Classes.hpp>
#include <vector>
#include <map>
#include <set>

#include "TableFiles.h"
#include "Class_1CD.h"
#include "cfapi/V8File.h"


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
	ConfigStorage() = default;
	virtual ~ConfigStorage() = default;
	virtual ConfigFile* readfile(const std::string &path) = 0; // Если файл не существует, возвращается NULL
	virtual bool writefile(const std::string &path, TStream *str) = 0;
	virtual std::string presentation() const = 0;
	virtual void close(ConfigFile* cf) = 0;
	virtual bool fileexists(const std::string &path) const = 0;
};

//---------------------------------------------------------------------------
// Класс адаптера контейнера конфигурации - Директория
class ConfigStorageDirectory : public ConfigStorage
{
private:
	boost::filesystem::path fdir;
public:
	explicit ConfigStorageDirectory(const std::string &_dir);
	virtual ConfigFile* readfile(const std::string &path) override;
	virtual bool writefile(const std::string &path, TStream *str) override;
	virtual std::string presentation() const override;
	virtual void close(ConfigFile* cf) override {delete cf->str; delete cf;}
	virtual bool fileexists(const std::string &path) const override;
	virtual ~ConfigStorageDirectory() {}
};

//---------------------------------------------------------------------------
// Класс адаптера контейнера конфигурации - cf (epf, erf, cfe) файл
class ConfigStorageCFFile : public ConfigStorage
{
private:
	std::string filename;
	V8Catalog* cat;
public:
	explicit ConfigStorageCFFile(const std::string &fname);
	virtual ~ConfigStorageCFFile();
	virtual ConfigFile* readfile(const std::string &path) override;
	virtual bool writefile(const std::string &path, TStream *str) override;
	virtual std::string presentation() const override;
	virtual void close(ConfigFile* cf) override;
	virtual bool fileexists(const std::string &path) const override;
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
class ContainerFile
{
public:
	TableFile* file;
	std::string name; // Приведенное имя (очищенное от динамического обновления)
	TStream* stream;
	TStream* rstream; // raw stream (нераспакованный поток)
	V8Catalog* cat;
	table_file_packed packed;
	int dynno; // Номер (индекс) динамического обновления (0, 1 и т.д.). Если без динамического обновления, то -1, если UID динамического обновления не найден, то -2. Для пропускаемых файлов -3.

	ContainerFile(TableFile *_f, const std::string &_name);
	~ContainerFile();
	bool open();
	bool ropen(); // raw open
	void close();
	bool isPacked() const;
};

//---------------------------------------------------------------------------
// Базовый класс адаптера таблицы - контейнера конфигурации (CONFIG, CONFICAS, CONFIGSAVE, CONFICASSAVE)
class ConfigStorageTable : public ConfigStorage
{
public:
	explicit ConfigStorageTable(T_1CD* _base = nullptr) : base(_base){}
	virtual ~ConfigStorageTable();
	virtual ConfigFile* readfile(const std::string &path) override;
	virtual bool writefile(const std::string &path, TStream *str) override;
	virtual void close(ConfigFile* cf) override;
	// сохранение конфигурации в файл
	bool save_config(const boost::filesystem::path& file_name);
	bool getready() const {return ready;}
	virtual bool fileexists(const std::string &path) const override;

protected:
	std::map<std::string,ContainerFile*> files;
	bool ready{false};
private:
	T_1CD* base; // установлена, если база принадлежит адаптеру конфигурации
};

//---------------------------------------------------------------------------
// Класс адаптера таблицы - контейнера конфигурации CONFIG (конфигурации базы данных)
class ConfigStorageTableConfig : public ConfigStorageTable
{
public:
	explicit ConfigStorageTableConfig(TableFiles* tabf, T_1CD* _base = nullptr);
	virtual std::string presentation() const override;
	virtual ~ConfigStorageTableConfig() = default;

private:
	std::string present;
};

//---------------------------------------------------------------------------
// Класс адаптера таблицы - контейнера конфигурации CONFIGSAVE (основной конфигурации)
class ConfigStorageTableConfigSave : public ConfigStorageTable
{
public:
	ConfigStorageTableConfigSave(TableFiles* tabc, TableFiles* tabcs, T_1CD* _base = nullptr);
	virtual std::string presentation() const override;
	virtual ~ConfigStorageTableConfigSave() {}

private:
	std::string present;
};

//---------------------------------------------------------------------------
// Класс адаптера таблицы - контейнера конфигурации CONFIGCAS (расширения конфигурации базы данных)
class ConfigStorageTableConfigCas : public ConfigStorageTable
{
public:
	ConfigStorageTableConfigCas(TableFiles *tabc, const std::string &configver, T_1CD *_base = nullptr);
	virtual std::string presentation() const override;
	virtual ~ConfigStorageTableConfigCas() {}

private:
	std::string present;
};

//---------------------------------------------------------------------------
// Класс адаптера таблицы - контейнера конфигурации CONFIGCASSAVE (расширения основной конфигурации)
class ConfigStorageTableConfigCasSave : public ConfigStorageTable
{
public:
	ConfigStorageTableConfigCasSave(TableFiles *tabc, TableFiles *tabcs, const BinaryGuid &uid,
									const std::string &configver, T_1CD *_base = nullptr);
	virtual std::string presentation() const override;
	virtual ~ConfigStorageTableConfigCasSave() = default;

private:
	std::string present;
};



//---------------------------------------------------------------------------
#endif

