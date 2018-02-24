//---------------------------------------------------------------------------

#ifndef ParseCommandLineH
#define ParseCommandLineH

#include <vector>
#include "String.hpp"

//---------------------------------------------------------------------------
enum class Command
{
	help,                       // выдать подсказку про ключи запуска
	no_verbose,                 // не выводить сообщения в консоль
	quit,                       // завершить работу после выполнения всех команд командной строки
	not_exclusively,            // открывать базу не монопольно
	export_all_to_xml,          // выгрузить все таблицы в XML
	xml_blob_to_file,           // при выгрузке в XML выгружать blob в отдельные файлы
	xml_parse_blob,             // при выгрузке в XML в отдельные файлы распаковывать данные blob
	save_config,                // сохранить конфигурацию базы данных в файл
	save_configsave,            // сохранить основную конфигурацию в файл
	save_vendors_configs,       // сохранить конфигурации поставщиков в файл
	save_all_configs,           // сохранить конфигурации поставщиков в файл
	export_to_xml,              // выгрузить таблицы в XML по заданному фильтру
	save_depot_config,          // сохранить конфигурацию хранилища в файл
	save_depot_config_part,     // частично сохранить конфигурацию хранилища в каталог
	logfile,                    // записывать лог-файл
	export_to_binary,           // выгрузить таблицы в двоичные файлы по заданному фильтру
	import_from_binary,         // загрузить таблицы из двоичных файлов, выгруженных экспортом
	find_and_save_lost_objects, // найти и сохранить потерянные объекты
};

struct CommandDefinition
{
	std::string key;           // строковое значение ключа
	Command command;           // команда
	int num_add_par;           // количество доп. параметров команды в командной строке
	std::string predefine_par; // значение первого доп. параметра по умолчанию (доп. параметр идет после параметров командной строки, количество которых указано в num_add_par)
};

struct ParsedCommand
{
	Command command;      // команда
	std::string param1;        // значение первого доп. параметра
	std::string param2;        // значение второго доп. параметра
	std::string param3;        // значение третьего доп. параметра
};

class CommandParse
{
public:
	CommandParse(char **szArglist, int nArgs);
	std::vector<ParsedCommand>& getcommands();
	std::string & getfilename();
	static std::string & gethelpstring();

private:
	static CommandDefinition definitions[];
	static std::string helpstring;
	std::string filename;
	std::vector<ParsedCommand> commands;
};

//---------------------------------------------------------------------------
#endif

