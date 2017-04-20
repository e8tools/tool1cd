#pragma hdrstop
#pragma argsused

//#include <tchar.h>
#include <stdio.h>
#include <boost/regex.hpp>

#include <iostream>
//#include <vcl.h>
//#include <windows.h>
#include <System.IOUtils.hpp>
#include "Zip.h"


#include "cTool_1CD_Main.h"
#include "ParseCommandLine.h"

using namespace std;

TMultiReadExclusiveWriteSynchronizer* tr_syn = new TMultiReadExclusiveWriteSynchronizer();

//---------------------------------------------------------------------------
MessageRegistrator* msreg;
const int TEMP_BUFFER_SIZE = 4096;
char temp[TEMP_BUFFER_SIZE];

//---------------------------------------------------------------------------
__fastcall Messager::Messager()
{
	has_error = false;
	logfile = "";
	noverbose = false;

}

//---------------------------------------------------------------------------
void __fastcall Messager::Status(const String& message)
{
}

//---------------------------------------------------------------------------
void __fastcall Messager::AddMessage(const String& message, const MessageState mstate, TStringList* param)
{
	cerr << message << endl;
	if (param) {
		for (auto it : *param) {
			cerr << "\t" << it << endl;
		}
	}
}

//---------------------------------------------------------------------------
void __fastcall Messager::setlogfile(String _logfile)
{
	logfile = System::Ioutils::TPath::GetFullPath(_logfile);
	if(FileExists(logfile)) DeleteFile(logfile);
}

bool IsTrueString(const String &str)
{
	String s = str.LowerCase();
	return s.Compare("1") == 0 || s.Compare("y") == 0 || s.Compare("yes") || s.Compare("д") || s.Compare("да") == 0;
}

//---------------------------------------------------------------------------
int main(int argc, char* argv[])
{
	Messager mess; // регистратор сообщений
	int i, j, k, m;
	int ret;
	unsigned int n;
	table* t;
	String f, v;
	bool b;
	Sysutils::TStringBuilder* filter;
	TStringList* filters;
	boost::regex* expr;

	T_1CD* base1CD; // база 1CD

	bool ActionOpenBaseNotMonopolyChecked = false;
	bool ActionXMLSaveBLOBToFileChecked = false;
	bool ActionXMLUnpackBLOBChecked = true;

	msreg = &mess;
	base1CD = NULL;

	CommandParse comm(argv, argc, &mess);

	DynamicArray<ParsedCommand>& commands = comm.getcommands();

	if(commands.get_length() == 0)
	{
		cout << "cTool_1CD (c) awa 2009 - 2016" << endl << "Запусти cTool_1CD -h для справки" << endl;
		return 0;
	}

	// Первый цикл просмотра команд для определения ключей параметров
	for(i = 0; i < commands.get_length(); i++)
	{
		ParsedCommand& pc = commands[i];
		switch(pc.command)
		{
			case cmd_help:
				cout << comm.gethelpstring() << endl;
				return 0;
				break;
			case cmd_no_verbose:
				mess.setnoverbose(true);
				break;
			case cmd_quit:
				//quit = true;
				break;
			case cmd_logfile:
				mess.setlogfile(pc.param1);
				break;
			case cmd_not_exclusively:
				ActionOpenBaseNotMonopolyChecked = true;
				break;
			case cmd_xml_blob_to_file:
				ActionXMLSaveBLOBToFileChecked = IsTrueString(pc.param1);
				break;
			case cmd_xml_parse_blob:
				ActionXMLUnpackBLOBChecked = IsTrueString(pc.param1);
				break;
		}
	}

	f = comm.getfilename();
	if(f.IsEmpty())
	{
		mess.AddMessage("В командной строке не найден файл базы 1CD", msError);
		return 3;
	}
	f = System::Ioutils::TPath::GetFullPath(f);
	base1CD = new T_1CD(f, &mess, !ActionOpenBaseNotMonopolyChecked);
	if(base1CD->is_open())
	{
		mess.AddMessage_("База данных 1CD открыта", msSuccesfull,
			"Файл", f,
			"Версия базы", base1CD->ver,
			"Locale", base1CD->locale,
			"Режим", base1CD->get_readonly() ? "Только чтение" : "Редактирование",
			"Количество таблиц", base1CD->get_numtables());
	}
	else return 2;

	for(i = 0; i < commands.get_length(); i++)
	{
		ParsedCommand& pc = commands[i];
		try
		{
			switch(pc.command)
			{
				case cmd_export_all_to_xml:
					if(base1CD->is_open())
					{
						for(j = 0; j < base1CD->get_numtables(); j++)
						{
							t = base1CD->gettable(j);

							if(!t->get_numindexes())
							{
								t->fillrecordsindex();
							}

							f = pc.param1 + "\\" + t->getname() + ".xm";

							t->export_to_xml(f, ActionXMLSaveBLOBToFileChecked, ActionXMLUnpackBLOBChecked);
							mess.AddMessage_("Выполнен экспорт таблицы в файл.", msSuccesfull,
								"Таблица", t->getname(),
								"Файл", f);
						}
					}
					else mess.AddError("Попытка выгрузки всех таблиц в XML без открытой базы.");
					break;

				case cmd_export_to_xml:
					if(base1CD->is_open())
					{

						filter = new Sysutils::TStringBuilder(pc.param2);
						filter->Replace("*", ".*");
						filter->Replace("?", ".");
						filter->Replace(" ", "\r\n");
						filter->Replace("\t", "\r\n");
						filter->Replace(",", "\r\n");
						filter->Replace(";", "\r\n");

						filters = new TStringList;
						filters->SetText(filter->ToString());
						for(m = filters->Count() - 1; m >= 0; m--)
						{
							if(filters->Strings[m].Length() == 0) filters->Delete(m);
							else filters->Strings[m] = String("^") + filters->Strings[m].UpperCase() + "$";
						}

						k = filters->Count();
						if(k == 0)
						{
							mess.AddError("Список таблиц для выгрузки в XML пуст.");
							delete filter;
							delete filters;
							break;
						}

						expr = new boost::regex[k];
						for(m = 0; m < k; m++) expr[m] = boost::regex(filters->Strings[m].c_str());

						for(j = 0; j < base1CD->get_numtables(); j++)
						{
							t = base1CD->gettable(j);

							b = false;

							for(m = 0; m < k; m++)
							{
								if(regex_match(t->getname().UpperCase().c_str(), expr[m]))
								{
									b = true;
									break;
								}
							}

							if(b)
							{
								if(!t->get_numindexes())
								{
									t->fillrecordsindex();
								}

								f = pc.param1 + "\\" + t->getname() + ".xml";

								t->export_to_xml(f, ActionXMLSaveBLOBToFileChecked, ActionXMLUnpackBLOBChecked);
								mess.AddMessage_("Выполнен экспорт таблицы в файл.", msSuccesfull,
									"Таблица", t->getname(),
									"Файл", f);
							}

						}

						delete[] expr;
						delete filter;
						delete filters;

					}
					else mess.AddError("Попытка выгрузки таблиц в XML без открытой базы.");

					break;
				case cmd_save_config:
					if(base1CD->is_open())
					{
						f = pc.param1;
						f = System::Ioutils::TPath::GetFullPath(f);
						if(f.SubString(f.Length() - 2, 3).CompareIC(".cf") != 0)
						{
							if(!DirectoryExists(f))
							{
								mess.AddMessage_("Каталог не существует.", msError,
									"Каталог", f);
								break;
							}
							f = f + "\\dbcf.cf";
						}
						if(base1CD->save_config(f))
							mess.AddMessage_("Сохранение конфигурации базы данных завершено.", msSuccesfull,
								"Файл", f);
						else
							mess.AddMessage_("Не удалось сохранить конфигурацию базы данных.", msError,
								"Файл", f);
					}
					else mess.AddError("Попытка выгрузки конфигурации базы данных без открытой базы.");
					break;
				case cmd_save_configsave:
					if(base1CD->is_open())
					{
						f = pc.param1;
						f = System::Ioutils::TPath::GetFullPath(f);
						if(f.SubString(f.Length() - 2, 3).CompareIC(".cf") != 0)
						{
							if(!DirectoryExists(f))
							{
								mess.AddMessage_("Каталог не существует.", msError,
									"Каталог", f);
								break;
							}
							f = f + "\\cf.cf";
						}
						if(base1CD->save_configsave(f))
							mess.AddMessage_("Сохранение основной конфигурации завершено.", msSuccesfull,
								"Файл", f);
						else
							mess.AddMessage_("Не удалось сохранить основную конфигурацию.", msError,
								"Файл", f);
					}
					else mess.AddError("Попытка выгрузки основной конфигурации без открытой базы.");
					break;
				case cmd_save_vendors_configs:
					if(base1CD->is_open())
					{
						base1CD->find_supplier_configs();
						for(n = 0; n < base1CD->supplier_configs.size(); n++)
						{
							f = pc.param1 + "\\" + base1CD->supplier_configs[n].name + " " + base1CD->supplier_configs[n].version + ".cf";
							if(base1CD->save_supplier_configs(n, f))
								mess.AddMessage_("Сохранение конфигурации поставщика завершено.", msSuccesfull,
									"Файл", f);
							else
								mess.AddMessage_("Не удалось сохранить конфигурацию поставщика.", msError,
									"Файл", f);
						}
					}
					else mess.AddError("Попытка выгрузки конфигураций поставщиков без открытой базы.");
					break;
				case cmd_save_all_configs:
					if(base1CD->is_open())
					{
						f = pc.param1 + "\\dbcf.cf";
						if(base1CD->save_config(f))
							mess.AddMessage_("Сохранение конфигурации базы данных завершено.", msSuccesfull,
								"Файл", f);
						else
							mess.AddMessage_("Не удалось сохранить конфигурацию базы данных.", msError,
								"Файл", f);

						f = pc.param1 + "\\cf.cf";
						if(base1CD->save_configsave(f))
							mess.AddMessage_("Сохранение основной конфигурации завершено.", msSuccesfull,
								"Файл", f);
						else
							mess.AddMessage_("Не удалось сохранить основную конфигурацию.", msError,
								"Файл", f);

						base1CD->find_supplier_configs();
						for(n = 0; n < base1CD->supplier_configs.size(); n++)
						{
							f = pc.param1 + "\\" + base1CD->supplier_configs[n].name + " " + base1CD->supplier_configs[n].version + ".cf";
							if(base1CD->save_supplier_configs(n, f))
								mess.AddMessage_("Сохранение конфигурации поставщика завершено.", msSuccesfull,
									"Файл", f);
							else
								mess.AddMessage_("Не удалось сохранить конфигурацию поставщика.", msError,
									"Файл", f);
						}
					}
					else mess.AddError("Попытка выгрузки всех конфигураций без открытой базы.");
					break;
				case cmd_save_depot_config:
					if(!base1CD->is_open())
					{
						mess.AddError("Попытка выгрузки конфигурации хранилища без открытой базы.");
						break;
					}
					if(!base1CD->is_depot)
					{
						mess.AddError("Попытка выгрузки конфигурации хранилища из базы, не являющейся хранилищем конфигурации.");
						break;
					}
					f = pc.param1;
					if(f.Compare("0") == 0) j = 0;
					else j = f.ToIntDef(0);
					j = base1CD->get_ver_depot_config(j);
					if(!j)
					{
						mess.AddError("Запрошенной версии конфигурации нет в хранилище конфигурации.");
						break;
					}
					f = pc.param2;
					f = System::Ioutils::TPath::GetFullPath(f);
					if(f.SubString(f.Length() - 2, 3).CompareIC(".cf") != 0)
					{
						if(!DirectoryExists(f))
						{
							mess.AddMessage_("Каталог не существует.", msError,
								"Каталог", f);
							break;
						}
						f = f + "\\v" + j + ".cf";
					}
					if(base1CD->save_depot_config(f, j))
						mess.AddMessage_("Сохранение конфигурации хранилища завершено.", msSuccesfull,
							"Файл", f);
					else
						mess.AddMessage_("Не удалось сохранить конфигурацию хранилища.", msError,
							"Файл", f);
					break;
				case cmd_save_depot_config_part:
					if(!base1CD->is_open())
					{
						mess.AddError("Попытка выгрузки файлов конфигурации хранилища без открытой базы.");
						break;
					}
					if(!base1CD->is_depot)
					{
						mess.AddError("Попытка выгрузки файлов конфигурации хранилища из базы, не являющейся хранилищем конфигурации.");
						break;
					}
					f = pc.param1;
					k = f.Pos(":");
					if(k)
					{
						v = f.SubString(k + 1, f.Length() - k);
						f = f.SubString(1, k - 1);
						j = f.ToIntDef(0);
						k = v.ToIntDef(0);
					}
					else
					{
						j = f.ToIntDef(0);
						k = j;
					}
					j = base1CD->get_ver_depot_config(j);
					if(!j)
					{
						mess.AddError("Запрошенной версии конфигурации нет в хранилище конфигурации."
							, "Версия", j);
						break;
					}
					k = base1CD->get_ver_depot_config(k);
					if(!k)
					{
						mess.AddError("Запрошенной версии конфигурации нет в хранилище конфигурации."
							, "Версия", k);
						break;
					}
					f = pc.param2;
					f = System::Ioutils::TPath::GetFullPath(f);
					if(!DirectoryExists(f)) System::Ioutils::TDirectory::CreateDirectory(f);
					if(base1CD->save_part_depot_config(f, j, k))
						mess.AddMessage_("Сохранение файлов конфигурации хранилища завершено.", msSuccesfull,
							"Файл", f);
					else
						mess.AddMessage_("Не удалось сохранить файлы конфигурации хранилища.", msError,
							"Файл", f);
					break;
			}
		}
		catch(String s)
		{
			mess.AddError(s);
		}
		catch(Exception& ex)
		{
			mess.AddError(ex.Message());
		}
		catch(...)
		{
			mess.AddError("Неизвестная ошибка.");
		}
	}

	delete base1CD;

	if(mess.has_error) ret = 1;
	else ret = 0;
	return ret;
}

