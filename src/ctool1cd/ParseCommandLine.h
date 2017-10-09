//---------------------------------------------------------------------------

#ifndef ParseCommandLineH
#define ParseCommandLineH

#include <Classes.hpp>
#include "MessageRegistration.h"

//---------------------------------------------------------------------------
enum class Command
{
	help,                   // выдать подсказку про ключи запуска
	no_verbose,             // не выводить сообщения в консоль
	quit,                   // завершить работу после выполнения всех команд командной строки
	not_exclusively,        // открывать базу не монопольно
	export_all_to_xml,      // выгрузить все таблицы в XML
	xml_blob_to_file,       // при выгрузке в XML выгружать blob в отдельные файлы
	xml_parse_blob,         // при выгрузке в XML в отдельные файлы распаковывать данные blob
	save_config,            // сохранить конфигурацию базы данных в файл
	save_configsave,        // сохранить основную конфигурацию в файл
	save_vendors_configs,   // сохранить конфигурации поставщиков в файл
	save_all_configs,       // сохранить конфигурации поставщиков в файл
	export_to_xml,          // выгрузить таблицы в XML по заданному фильтру
	save_depot_config,      // сохранить конфигурацию хранилища в файл
	save_depot_config_part, // частично сохранить конфигурацию хранилища в каталог
	logfile,                // записывать лог-файл
	export_to_binary,       // выгрузить таблицы в двоичные файлы по заданному фильтру
	import_from_binary,     // загрузить таблицы из двоичных файлов, выгруженных экспортом
};

struct CommandDefinition
{
	String key;           // строковое значение ключа
	Command command;      // команда
	int num_add_par;      // количество доп. параметров команды в командной строке
	String predefine_par; // значение первого доп. параметра по умолчанию (доп. параметр идет после параметров командной строки, количество которых указано в num_add_par)
};

struct ParsedCommand
{
	Command command;      // команда
	String param1;        // значение первого доп. параметра
	String param2;        // значение второго доп. параметра
	String param3;        // значение третьего доп. параметра
};

class CommandParse: public IControlMessageRegistration
{
public:
	CommandParse(LPSTR *szArglist, int nArgs, MessageRegistrator* _mess = nullptr);
	DynamicArray<ParsedCommand>& getcommands();
	String& getfilename();
	static String& gethelpstring();

	void AddMessageRegistrator(MessageRegistrator* messageregistrator) override;
	void RemoveMessageRegistrator() override;
private:
	static CommandDefinition definitions[];
	static String helpstring;
	String filename;
	DynamicArray<ParsedCommand> commands;
	Registrator mess; // регистратор сообщений
};

//---------------------------------------------------------------------------
#endif

