//---------------------------------------------------------------------------

#ifndef ParseCommandLineH
#define ParseCommandLineH

#include <Classes.hpp>
#include "MessageRegistration.h"

//---------------------------------------------------------------------------
enum Command
{
	cmd_help,					// выдать подсказку про ключи запуска
	cmd_no_verbose,          	// не выводить сообщения в консоль
	cmd_quit,                	// завершить работу после выполнения всех команд командной строки
	cmd_not_exclusively,     	// открывать базу не монопольно
	cmd_export_all_to_xml,   	// выгрузить все таблицы в XML
	cmd_xml_blob_to_file,    	// при выгрузке в XML выгружать blob в отдельные файлы
	cmd_xml_parse_blob,      	// при выгрузке в XML в отдельные файлы распаковывать данные blob
	cmd_save_config,         	// сохранить конфигурацию базы данных в файл
	cmd_save_configsave,     	// сохранить основную конфигурацию в файл
	cmd_save_vendors_configs,	// сохранить конфигурации поставщиков в файл
	cmd_save_all_configs,		// сохранить конфигурации поставщиков в файл
	cmd_export_to_xml,        	// выгрузить таблицы в XML по заданному фильтру
	cmd_save_depot_config,		// сохранить конфигурацию хранилища в файл
	cmd_save_depot_config_part, // частично сохранить конфигурацию хранилища в каталог
	cmd_logfile					// записывать лог-файл
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

class CommandParse
{
private:
	static CommandDefinition definitions[];
	static String helpstring;
	String filename;
	DynamicArray<ParsedCommand> commands;
	MessageRegistrator* mess; // регистратор сообщений
public:
	CommandParse(LPSTR *szArglist, int nArgs, MessageRegistrator* _mess = NULL);
	//CommandParse(LPWSTR CommandLine, MessageRegistrator* _mess = NULL);
	DynamicArray<ParsedCommand>& getcommands();
	String& getfilename();
	static String& gethelpstring();
};

//---------------------------------------------------------------------------
#endif

