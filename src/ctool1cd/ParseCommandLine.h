/*
    CTool1CD provides console front end to Tool1CD library
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of CTool1CD.

    CTool1CD is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CTool1CD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CTool1CD.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ParseCommandLineH
#define ParseCommandLineH

#include <vector>
#include <string>

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

