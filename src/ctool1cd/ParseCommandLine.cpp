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

#include "ParseCommandLine.h"
#include "SystemClasses/String.hpp"

using namespace System;

//---------------------------------------------------------------------------
#if !defined(_WIN32)
#pragma package(smart_init)
#endif

#include "MessageRegistration.h"

extern Registrator msreg_g;

CommandDefinition CommandParse::definitions[] =
{
	{"h",                  Command::help,                       0, ""}, // 1
	{"help",               Command::help,                       0, ""}, // 2
	{"?",                  Command::help,                       0, ""}, // 3
	{"nv",                 Command::no_verbose,                 0, ""}, // 4
	{"noverbose",          Command::no_verbose,                 0, ""}, // 5
	{"q",                  Command::quit,                       0, ""}, // 6
	{"quit",               Command::quit,                       0, ""}, // 7
	{"eax",                Command::export_all_to_xml,          1, ""}, // 8
	{"exportalltoxml",     Command::export_all_to_xml,          1, ""}, // 9
	{"bf",                 Command::xml_blob_to_file,           1, "0"}, // 10
	{"blobtofile",         Command::xml_blob_to_file,           1, "0"}, // 11
	{"pb",                 Command::xml_parse_blob,             1, "1"}, // 12
	{"parseblob",          Command::xml_parse_blob,             1, "1"}, // 13
	{"ex",                 Command::export_to_xml,              2, ""}, // 14
	{"exporttoxml",        Command::export_to_xml,              2, ""}, // 15
	{"ne",                 Command::not_exclusively,            0, ""}, // 16
	{"notexclusively",     Command::not_exclusively,            0, ""}, // 17
	{"ddc",                Command::save_config,                1, ""}, // 18
	{"dumpdbconfig",       Command::save_config,                1, ""}, // 29
	{"dc",                 Command::save_configsave,            1, ""}, // 20
	{"dumpconfig",         Command::save_configsave,            1, ""}, // 21
	{"dvc",                Command::save_vendors_configs,       1, ""}, // 22
	{"dumpvendorsconfigs", Command::save_vendors_configs,       1, ""}, // 23
	{"dac",                Command::save_all_configs,           1, ""}, // 24
	{"dumpallconfigs",     Command::save_all_configs,           1, ""}, // 25
	{"drc",                Command::save_depot_config,          2, ""}, // 26
	{"dumpdepotconfig",    Command::save_depot_config,          2, ""}, // 27
	{"dpc",                Command::save_depot_config_part,     2, ""}, // 28
	{"dumppartdepotconfig",Command::save_depot_config_part,     2, ""}, // 29
	{"l",                  Command::logfile,                    1, ""}, // 30
	{"logfile",            Command::logfile,                    1, ""}, // 31
	{"eb",                 Command::export_to_binary,           2, ""}, // 32
	{"exporttobinary",     Command::export_to_binary,           2, ""}, // 33
	{"ib",                 Command::import_from_binary,         2, ""}, // 34
	{"importfrombinary",   Command::import_from_binary,         2, ""}, // 35
	{"slo",                Command::find_and_save_lost_objects, 1, ""}, // 36
	{"savelostobjects",    Command::find_and_save_lost_objects, 1, ""}, // 37
	{"sts",                Command::save_tables_size,           1, ""}, // 38
	{"savetablessize",     Command::save_tables_size,           1, ""}, // 39
};


std::string CommandParse::helpstring =
"ctool1cd (с) 2009-2017 awa\r\n\
\r\n\
Запуск:\r\n\
ctool1cd.exe [<ключи>] <1CD файл> [<ключи>]\r\n\
\r\n\
Ключи:\r\n\
 -h, -help, -?\r\n\
   эта справка.\r\n\
\r\n\
 -nv, -noverbose\
   подавить вывод в консоль.\r\n\
\r\n\
-q, -quit\r\n\
   завершение программы после выполнения команд командной строки.\r\n"
"\r\n\
 -l, -logfile <файл>\r\n\
   записывать все сообщения программы в текстовый лог-файл. Если файл существует, он перезаписывается. Кодировка файла UTF8\r\n\
\r\n\
 -ne, -NotExclusively\r\n\
   открыть базу не монопольно (Это небезопасно, возможны ошибки!).\r\n\
\r\n\
 -eax, -ExportAllToXML <путь>\r\n\
   экспортировать по указанному пути все таблицы в XML.\r\n\
\r\n\
 -ex, -ExportToXML <путь> <список>\r\n\
   экспортировать по указанному пути указанные таблицы в XML.\r\n\
   В списке через запятую, точку с запятой или пробел указывается список имен экспортируемых таблиц. Можно использовать знаки подстановки * и ?\r\n\
   Если в списке содержатся пробелы, список необходимо заключать в кавычки.\r\n\
\r\n\
 -bf, -BlobToFile [yes/no]\r\n\
   при экспорте в XML выгружать BLOB в отдельные файлы.\r\n\
   По умолчанию BLOB в отдельные файлы не выгружается.\r\n\
\r\n\
 -pb, -ParseBlob [yes/no]\r\n\
   при экспорте в XML и выгрузке BLOB в отдельные файлы по-возможности распаковывать данные BLOB.\r\n\
   По умолчанию BLOB при выгрузке в отдельные файлы распаковываются.\r\n\
\r\n\
 -dc, -DumpConfig <путь>\r\n\
   Выгрузить основную конфигурацию по указанному пути.\r\n\
\r\n\
 -ddc, -DumpDBConfig <путь>\r\n\
   Выгрузить конфигурацию базы данных по указанному пути.\r\n\
\r\n\
 -dvc, -DumpVendorsConfigs <путь>\r\n\
   Выгрузить конфигурации поставщиков информационной базы по указанному пути.\r\n\
\r\n\
 -dac, -DumpAllConfigs <путь>\r\n\
   Выгрузить все конфигурации информационной базы по указанному пути.\r\n\
\r\n\
 -drc, -DumpDepotConfig <номер версии> <путь>\r\n\
   Выгрузить конфигурацию хранилища заданной версии по указанному пути.\r\n\
   Номер версии - это целое число. 1, 2, 3 и т.д. - выгрузить конфигурацию указанной версии, 0 - выгрузить последнюю версию, -1 - предпоследнюю и т.д.\r\n\
\r\n\
 -dpc, -DumpPartDepotConfig <номер версии>[:<номер версии>] <путь>\r\n\
   Выгрузить частично файлы конфигурации хранилища заданной версии (или заданного диапазона версий) по указанному пути.\r\n\
   Номер версии - это целое число. 1, 2, 3 и т.д. - выгрузить файлы указанной версии, 0 - выгрузить файлы последней версии, -1 - предпоследней и т.д.\r\n\
\r\n\
 -eb, -ExportToBinary <путь> <список>\r\n\
   Экспортировать по указанному пути указанные таблицы.\r\n\
   В списке через запятую, точку с запятой или пробел указывается список имён экспортируемых таблиц. Можно использовать знаки подстановки * и ?\r\n\
   Если в списке содержатся пробелы, список необходимо заключать в кавычки.\r\n\
\r\n\
 -ib, -ImportFromBinary <путь> <список>\r\n\
   Импортировать по указанному пути указанные таблицы.\r\n\
   В списке через запятую, точку с запятой или пробел указывается список имён импортируемых таблиц. Можно использовать знаки подстановки * и ?\r\n\
   Если в списке содержатся пробелы, список необходимо заключать в кавычки.\r\n\
   Таблицы должны существовать в базе, новые таблицы не создаются.\r\n\
\r\n\
 -slo, -SaveLostObjects <путь>\r\n\
   Найти потерянные объекты и сохранить\r\n.\
 -sts, -SaveTablesSize <путь>\r\n\
   Сохранить в файл размеры таблиц и вспомогательных данных. Сохраняются с простой текстовый файл с разделителем колонок |\r\n\
Если в пути содержатся пробелы, его необходимо заключать в кавычки. Пути следует указывать без завершающего бэкслеша \"\\\".\r\n\
Для команд -dc, -ddc, -drc вместо пути можно указывать имя файла конфигурации (имя файла должно заканчиваться на \".cf\").\r\n\
";


//---------------------------------------------------------------------------
std::string dequote(const std::string &str)
{
	if (str.size() < 2) {
		return str;
	}
	std::string result = str;
	if (result.front() == '\"' && result.back() == '\"') {
		result = result.substr(1, result.size() - 2);
	}
	while (result.back() == '\"') {
		result = result.substr(0, str.size() - 1);
	}
	return result;
}

//---------------------------------------------------------------------------

CommandParse::CommandParse(char **szArglist, int nArgs)
{
	int numdef = sizeof(definitions) / sizeof(CommandDefinition);

	filename = "";
	for (int i = 1; i < nArgs; i++)
	{
		std::string param = szArglist[i];
		if (param.front() == '-') {
			param = LowerCase(param.substr(1, param.size() - 1));
			int j;
			for (j = 0; j < numdef; j++) {
				if (Equal(param, definitions[j].key)) {
					break;
				}
			}
			if (j < numdef)
			{
				int n = commands.size();
				commands.resize(n + 1);
				commands[n].command = definitions[j].command;
				commands[n].param1 = "";
				commands[n].param2 = "";
				commands[n].param3 = "";
				int l;
				for(l = 0; l < definitions[j].num_add_par; l++)
				{
					if(++i < nArgs)
					{
						switch(l)
						{
							case 0:
								commands[n].param1 = dequote(szArglist[i]);
								break;
							case 1:
								commands[n].param2 = dequote(szArglist[i]);
								break;
							case 2:
								commands[n].param3 = dequote(szArglist[i]);
								break;
							default:
								// Ошибка! Количество параметров ключа в описании превышает максимально возможное!
								break;
						}
					}
					else
					{
						// TODO: Ошибка! Недостаточно параметров ключа!
						msreg_g.AddMessage("Недостаточно параметров ключа командной строки.", MessageState::Error)
							.with("Ключ", param);
					}
				}
				if (!definitions[j].predefine_par.empty()) {
					switch(l)
					{
						case 0:
							commands[n].param1 = definitions[j].predefine_par;
							break;
						case 1:
							commands[n].param2 = definitions[j].predefine_par;
							break;
						case 2:
							commands[n].param3 = definitions[j].predefine_par;
							break;
						default:
							// Ошибка! Количество параметров ключа в описании превышает максимально возможное!
							break;
					}
				}
			}
			else
			{
				// TODO: Ошибка! Неизвестный ключ!
				msreg_g.AddMessage("Неизвестный ключ командной строки.", MessageState::Error)
					.with("Ключ", param);
			}

		}
		else
		{
			if(!filename.empty())
			{
				// TODO: Ошибка! Имя файла базы уже было в командной строке!
				msreg_g.AddMessage("Повторное имя файла базы в командной строке.", MessageState::Error)
					.with("Имя файла", filename)
					.with("Повторное имя файла", param);
			}
			else filename = dequote(param);
		}
	}
}

std::vector<ParsedCommand>& CommandParse::getcommands()
{
	return commands;
}

std::string & CommandParse::getfilename()
{
	return filename;
}

std::string & CommandParse::gethelpstring()
{
	return helpstring;
}


