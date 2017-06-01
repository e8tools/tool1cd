//---------------------------------------------------------------------------

#pragma hdrstop

#include "ParseCommandLine.h"

//---------------------------------------------------------------------------
#if !defined(_WIN32)
#pragma package(smart_init)
#endif

CommandDefinition CommandParse::definitions[] =
{
	{"h",                  cmd_help,                   0, ""}, // 1
	{"help",               cmd_help,                   0, ""}, // 2
	{"?",                  cmd_help,                   0, ""}, // 3
	{"nv",                 cmd_no_verbose,             0, ""}, // 4
	{"noverbose",          cmd_no_verbose,             0, ""}, // 5
	{"q",                  cmd_quit,                   0, ""}, // 6
	{"quit",               cmd_quit,                   0, ""}, // 7
	{"eax",                cmd_export_all_to_xml,      1, ""}, // 8
	{"exportalltoxml",     cmd_export_all_to_xml,      1, ""}, // 9
	{"bf",                 cmd_xml_blob_to_file,       1, "0"}, // 10
	{"blobtofile",         cmd_xml_blob_to_file,       1, "0"}, // 11
	{"pb",                 cmd_xml_parse_blob,         1, "1"}, // 12
	{"parseblob",          cmd_xml_parse_blob,         1, "1"}, // 13
	{"ex",                 cmd_export_to_xml,          2, ""}, // 14
	{"exporttoxml",        cmd_export_to_xml,          2, ""}, // 15
	{"ne",                 cmd_not_exclusively,        0, ""}, // 16
	{"notexclusively",     cmd_not_exclusively,        0, ""}, // 17
	{"ddc",                cmd_save_config,            1, ""}, // 18
	{"dumpdbconfig",       cmd_save_config,            1, ""}, // 29
	{"dc",                 cmd_save_configsave,        1, ""}, // 20
	{"dumpconfig",         cmd_save_configsave,        1, ""}, // 21
	{"dvc",                cmd_save_vendors_configs,   1, ""}, // 22
	{"dumpvendorsconfigs", cmd_save_vendors_configs,   1, ""}, // 23
	{"dac",                cmd_save_all_configs,       1, ""}, // 24
	{"dumpallconfigs",     cmd_save_all_configs,       1, ""}, // 25
	{"drc",                cmd_save_depot_config,      2, ""}, // 26
	{"dumpdepotconfig",    cmd_save_depot_config,      2, ""}, // 27
	{"dpc",                cmd_save_depot_config_part, 2, ""}, // 28
	{"dumppartdepotconfig",cmd_save_depot_config_part, 2, ""}, // 29
	{"l",                  cmd_logfile,                1, ""}, // 30
	{"logfile",            cmd_logfile,                1, ""} // 31
};


String CommandParse::helpstring =
#ifdef console
"c"
#endif
"Tool_1CD (с) 2009-2017 awa\r\n"
"\r\n\
Запуск:\r\n"
#ifdef console
"c"
#endif
"Tool_1CD.exe [<ключи>] <1CD файл> [<ключи>]\r\n\
\r\n\
Ключи:\r\n"
#ifdef console
" -h\r\n\
 -help\r\n\
 -?\r\n\
   эта справка.\r\n\
\r\n\
 -nv\r\n\
 -noverbose\r\n\
   подавить вывод в консоль.\r\n"
#else
" -q\r\n\
 -quit\r\n\
   завершение программы после выполнения команд командной строки.\r\n"
#endif
"\r\n\
 -l <файл>\r\n\
 -logfile <файл>\r\n\
   записывать все сообщения программы в текстовый лог-файл. Если файл существует, он перезаписывается. Кодировка файла UTF8\r\n\
\r\n\
 -ne\r\n\
 -NotExclusively\r\n\
   открыть базу не монопольно (Это небезопасно, возможны ошибки!).\r\n\
\r\n\
 -eax <путь>\r\n\
 -ExportAllToXML <путь>\r\n\
   экспортировать по указанному пути все таблицы в XML.\r\n\
\r\n\
 -ex <путь> <список>\r\n\
 -ExportToXML <путь> <список>\r\n\
   экспортировать по указанному пути указанные таблицы в XML.\r\n\
   В списке через запятую, точку с запятой или пробел указывается список имен экспортируемых таблиц. Можно использовать знаки подстановки * и ?\r\n\
   Если в списке содержатся пробелы, список необходимо заключать в кавычки.\r\n\
\r\n\
 -bf [y/n]\r\n\
 -BlobToFile [yes/no]\r\n\
   при экспорте в XML выгружать BLOB в отдельные файлы.\r\n\
   По умолчанию BLOB в отдельные файлы не выгружается.\r\n\
\r\n\
 -pb [y/n]\r\n\
 -ParseBlob [yes/no]\r\n\
   при экспорте в XML и выгрузке BLOB в отдельные файлы по-возможности распаковывать данные BLOB.\r\n\
   По умолчанию BLOB при выгрузке в отдельные файлы распаковываются.\r\n\
\r\n\
 -dc <путь>\r\n\
 -DumpConfig <путь>\r\n\
   Выгрузить основную конфигурацию по указанному пути.\r\n\
\r\n\
 -ddc <путь>\r\n\
 -DumpDBConfig <путь>\r\n\
   Выгрузить конфигурацию базы данных по указанному пути.\r\n\
\r\n\
 -dvc <путь>\r\n\
 -DumpVendorsConfigs <путь>\r\n\
   Выгрузить конфигурации поставщиков информационной базы по указанному пути.\r\n\
\r\n\
 -dac <путь>\r\n\
 -DumpAllConfigs <путь>\r\n\
   Выгрузить все конфигурации информационной базы по указанному пути.\r\n\
\r\n\
 -drc <номер версии> <путь>\r\n\
 -DumpDepotConfig <номер версии> <путь>\r\n\
   Выгрузить конфигурацию хранилища заданной версии по указанному пути.\r\n\
   Номер версии - это целое число. 1, 2, 3 и т.д. - выгрузить конфигурацию указанной версии, 0 - выгрузить последнюю версию, -1 - предпоследнюю и т.д.\r\n\
\r\n\
 -dpc <номер версии>[:<номер версии>] <путь>\r\n\
 -DumpPartDepotConfig <номер версии>[:<номер версии>] <путь>\r\n\
   Выгрузить частично файлы конфигурации хранилища заданной версии (или заданного диапазона версий) по указанному пути.\r\n\
   Номер версии - это целое число. 1, 2, 3 и т.д. - выгрузить файлы указанной версии, 0 - выгрузить файлы последней версии, -1 - предпоследней и т.д.\r\n\
\r\n\
Если в пути содержатся пробелы, его необходимо заключать в кавычки. Пути следует указывать без завершающего бэкслеша \"\\\".\r\n\
Для команд -dc, -ddc, -drc вместо пути можно указывать имя файла конфигурации (имя файла должно заканчиваться на \".cf\").\r\n\
";


//---------------------------------------------------------------------------
String dequote(String str)
{
	if(str.Length() < 2) return str;
	if(str[1] == '\"' && str[str.Length()] == '\"') str = str.SubString(2, str.Length() - 2);
	while(str[str.Length()] == '\"') str = str.SubString(1, str.Length() - 1);
	return str;
}

//---------------------------------------------------------------------------

CommandParse::CommandParse(LPSTR *szArglist, int nArgs, MessageRegistrator* _mess)
//CommandParse::CommandParse(LPWSTR _CommandLine, MessageRegistrator* _mess)
{
	int numdef = sizeof(definitions) / sizeof(CommandDefinition);
	String k, p;

	mess = _mess;

	filename = "";
	for (int i = 1; i < nArgs; i++)
	{
		p = szArglist[i];
		// ВАЖНО: ломающие изменения. С "/" может начинаться путь к файлу.
		// if (p[1] == '/' || p[1] == '-')
		if (p[1] == '-')
		{
			k = p.SubString(2, p.Length() - 1).LowerCase();
			int j;
			for (j = 0; j < numdef; j++) if(k.Compare(definitions[j].key) == 0) break;
			if (j < numdef)
			{
				int n = commands.get_length();
				commands.set_length(n + 1);
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
						mess->AddMessage_("Недостаточно параметров ключа командной строки.", msError,
							"Ключ", k);
						// Ошибка! Недостаточно параметров ключа!
					}
				}
				if(definitions[j].predefine_par.Length() > 0)
				{
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
				// Ошибка! Неизвестный ключ!
				mess->AddMessage_("Неизвестный ключ командной строки.", msError,
					"Ключ", k);
			}

		}
		else
		{
			if(filename.Length() > 0)
			{
				// Ошибка! Имя файла базы уже было в командной строке!
				mess->AddMessage_("Повторное имя файла базы в командной строке.", msError,
					"Имя файла", filename,
					"Повторное имя файла", p);
			}
			else filename = dequote(p);
		}
	}
	LocalFree(szArglist);
}

DynamicArray<ParsedCommand>& CommandParse::getcommands()
{
	return commands;
}

String& CommandParse::getfilename()
{
	return filename;
}

String& CommandParse::gethelpstring()
{
	return helpstring;
}

