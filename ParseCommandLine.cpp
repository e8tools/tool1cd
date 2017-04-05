//---------------------------------------------------------------------------

#pragma hdrstop

#include "ParseCommandLine.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

CommandDefinition CommandParse::definitions[] =
{
	{L"h",                  cmd_help,                   0, L""}, // 1
	{L"help",               cmd_help,                   0, L""}, // 2
	{L"?",                  cmd_help,                   0, L""}, // 3
	{L"nv",                 cmd_no_verbose,             0, L""}, // 4
	{L"noverbose",          cmd_no_verbose,             0, L""}, // 5
	{L"q",                  cmd_quit,                   0, L""}, // 6
	{L"quit",               cmd_quit,                   0, L""}, // 7
	{L"eax",                cmd_export_all_to_xml,      1, L""}, // 8
	{L"exportalltoxml",     cmd_export_all_to_xml,      1, L""}, // 9
	{L"bf",                 cmd_xml_blob_to_file,       1, L"0"}, // 10
	{L"blobtofile",         cmd_xml_blob_to_file,       1, L"0"}, // 11
	{L"pb",                 cmd_xml_parse_blob,         1, L"1"}, // 12
	{L"parseblob",          cmd_xml_parse_blob,         1, L"1"}, // 13
	{L"ex",                 cmd_export_to_xml,          2, L""}, // 14
	{L"exporttoxml",        cmd_export_to_xml,          2, L""}, // 15
	{L"ne",                 cmd_not_exclusively,        0, L""}, // 16
	{L"notexclusively",     cmd_not_exclusively,        0, L""}, // 17
	{L"ddc",                cmd_save_config,            1, L""}, // 18
	{L"dumpdbconfig",       cmd_save_config,            1, L""}, // 29
	{L"dc",                 cmd_save_configsave,        1, L""}, // 20
	{L"dumpconfig",         cmd_save_configsave,        1, L""}, // 21
	{L"dvc",                cmd_save_vendors_configs,   1, L""}, // 22
	{L"dumpvendorsconfigs", cmd_save_vendors_configs,   1, L""}, // 23
	{L"dac",                cmd_save_all_configs,       1, L""}, // 24
	{L"dumpallconfigs",     cmd_save_all_configs,       1, L""}, // 25
	{L"drc",                cmd_save_depot_config,      2, L""}, // 26
	{L"dumpdepotconfig",    cmd_save_depot_config,      2, L""}, // 27
	{L"dpc",                cmd_save_depot_config_part, 2, L""}, // 28
	{L"dumppartdepotconfig",cmd_save_depot_config_part, 2, L""}, // 29
	{L"l",                  cmd_logfile,                1, L""}, // 30
	{L"logfile",            cmd_logfile,                1, L""} // 31
};


String CommandParse::helpstring =
#ifdef console
L"c"
#endif
L"Tool_1CD (с) 2009-2016 awa\r\n"
L"\r\n\
Запуск:\r\n"
#ifdef console
L"c"
#endif
L"Tool_1CD.exe [<ключи>] <1CD файл> [<ключи>]\r\n\
\r\n\
Ключи:\r\n"
#ifdef console
L" -h\r\n\
 -help\r\n\
 -?\r\n\
   эта справка.\r\n\
\r\n\
 -nv\r\n\
 -noverbose\r\n\
   подавить вывод в консоль.\r\n"
#else
L" -q\r\n\
 -quit\r\n\
   завершение программы после выполнения команд командной строки.\r\n"
#endif
L"\r\n\
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
	if(str[1] == L'\"' && str[str.Length()] == L'\"') str = str.SubString(2, str.Length() - 2);
	while(str[str.Length()] == L'\"') str = str.SubString(1, str.Length() - 1);
	return str;
}

//---------------------------------------------------------------------------

//__fastcall CommandParse::CommandParse(LPWSTR *szArglist, int nArgs)
__fastcall CommandParse::CommandParse(LPWSTR _CommandLine, MessageRegistrator* _mess)
{
	int i, j, n, l, m;
	int numdef = sizeof(definitions) / sizeof(CommandDefinition);
	String k, p;
	LPWSTR *szArglist;
	int nArgs;

	mess = _mess;
	szArglist = CommandLineToArgvW(_CommandLine, &nArgs);

	//for(i = 1; i < nArgs; i++) MessageBoxW(NULL, szArglist[i], L"Параметр", MB_OK);

	filename = L"";
	for(i = 1; i < nArgs; i++)
	{
		p = szArglist[i];
		if(p[1] == L'/' || p[1] == L'-')
		{
			k = p.SubString(2, p.Length() - 1).LowerCase();
			for(j = 0; j < numdef; j++) if(k.Compare(definitions[j].key) == 0) break;
			if(j < numdef)
			{
				n = commands.get_length();
				commands.set_length(n + 1);
				commands[n].command = definitions[j].command;
				commands[n].param1 = L"";
				commands[n].param2 = L"";
				commands[n].param3 = L"";
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
						mess->AddMessage_(L"Недостаточно параметров ключа командной строки.", msError,
							L"Ключ", k);
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
				mess->AddMessage_(L"Неизвестный ключ командной строки.", msError,
					L"Ключ", k);
			}

		}
		else
		{
			if(filename.Length() > 0)
			{
				// Ошибка! Имя файла базы уже было в командной строке!
				mess->AddMessage_(L"Повторное имя файла базы в командной строке.", msError,
					L"Имя файла", filename,
					L"Повторное имя файла", p);
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

String& __fastcall CommandParse::getfilename()
{
	return filename;
}

String& __fastcall CommandParse::gethelpstring()
{
	return helpstring;
}

