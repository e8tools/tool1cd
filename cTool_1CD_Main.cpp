#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <stdio.h>
#include <boost/regex.hpp>

#include <iostream.h>
//#include <vcl.h>
//#include <windows.h>
#include <System.IOUtils.hpp>
#include "Zip.h"


#include "cTool_1CD_Main.h"
#include "ParseCommandLine.h"

//---------------------------------------------------------------------------
MessageRegistrator* msreg;
const int TEMP_BUFFER_SIZE = 4096;
char temp[TEMP_BUFFER_SIZE];


//---------------------------------------------------------------------------
char* oem(const char* str)
{
	CharToOemA(str, temp);
	return temp;
}

//---------------------------------------------------------------------------
char* oem(const String& str)
{
	WideCharToMultiByte(CP_OEMCP, WC_NO_BEST_FIT_CHARS, str.c_str(), -1, temp, TEMP_BUFFER_SIZE - 1, "?", NULL);
	return temp;
}

//---------------------------------------------------------------------------
__fastcall Messager::Messager()
{
	has_error = false;
	logfile = L"";
	noverbose = false;

}

//---------------------------------------------------------------------------
void __fastcall Messager::Status(const String& message)
{
}

//---------------------------------------------------------------------------
void __fastcall Messager::AddMessage(const String& message, const MessageState mstate, TStringList* param)
{
	int i;
	TFileStream* log = NULL;
	TStreamWriter* sw;
	String s;

	if(!logfile.IsEmpty())
	{
		if(FileExists(logfile))
		{
			log = new TFileStream(logfile, fmOpenReadWrite | fmShareDenyNone);
			log->Seek(0, soFromEnd);
		}
		else
		{
			log = new TFileStream(logfile, fmCreate | fmShareDenyNone);
			log->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().Length);
		}
		sw = new TStreamWriter(log, TEncoding::UTF8, 4096);
		sw->Write(DateTimeToStr(Now(), FormatSettings));
		s = L" "; sw->Write(s);
		switch(mstate)
		{
			case msEmpty: s = L"<>"; break;
			case msSuccesfull: s = L"<ok>"; break;
			case msWarning: s = L"<warning>"; break;
			case msInfo: s = L"<info>"; break;
			case msError: s = L"<error>"; break;
			case msWait: s = L"<wait>"; break;
			case msHint: s = L"<hint>"; break;
			default: s = L"<>";
		}
		sw->Write(s);
		s = L" "; sw->Write(s);
		sw->Write(message);
		if(param) for(int i = 0; i < param->Count; i++)
		{
			s = L"\r\n\t"; sw->Write(s);
			sw->Write((*param)[i]);
		}
		s = L"\r\n\r\n"; sw->Write(s);
		delete sw;
		delete log;
	}


	if(mstate == msWarning || mstate == msError) has_error = true;

	if(!noverbose)
	{
		switch(mstate)
		{
			case msEmpty: break;
			case msSuccesfull: break;
			case msWarning: cout << "<Warning> "; break;
			case msInfo: cout << "<Info> "; break;
			case msError: cout << "<Error> "; break;
			case msWait: cout << "<Wait> "; break;
			case msHint: cout << "<Hint> "; break;
		}
		cout << oem(message) << "\n";
		if(param) for(i = 0; i < param->Count; i++) cout << "\t" << oem((*param)[i]) << "\n";
	}
}

//---------------------------------------------------------------------------
void __fastcall Messager::setlogfile(String _logfile)
{
	logfile = System::Ioutils::TPath::GetFullPath(_logfile);
	if(FileExists(logfile)) DeleteFile(logfile);
}

//---------------------------------------------------------------------------
int _tmain(int argc, _TCHAR* argv[])
{
	CommandParse* comm; // класс команд командной строки
	Messager* mess; // регистратор сообщений
	int i, j, k, m;
	int ret;
	unsigned int n;
	table* t;
	String f, v;
	bool b;
	Sysutils::TStringBuilder* filter;
	TStringList* filters;
	boost::wregex* expr;

	T_1CD* base1CD; // база 1CD

	bool ActionOpenBaseNotMonopolyChecked = false;
	bool ActionXMLSaveBLOBToFileChecked = false;
	bool ActionXMLUnpackBLOBChecked = true;

	mess = new Messager;
	msreg = mess;
	base1CD = NULL;

	comm = new CommandParse(GetCommandLineW(), mess);

	DynamicArray<ParsedCommand>& commands = comm->getcommands();

	if(commands.get_length() == 0)
	{
		cout << oem("cTool_1CD (c) awa 2009 - 2016\r\nЗапусти cTool_1CD -h для справки");
		return 0;
	}

	// Первый цикл просмотра команд для определения ключей параметров
	for(i = 0; i < commands.get_length(); i++)
	{
		ParsedCommand& pc = commands[i];
		switch(pc.command)
		{
			case cmd_help:
				cout << oem(comm->gethelpstring());
				delete comm;
				delete mess;
				return 0;
				break;
			case cmd_no_verbose:
				mess->setnoverbose(true);
				break;
			case cmd_quit:
				//quit = true;
				break;
			case cmd_logfile:
				mess->setlogfile(pc.param1);
				break;
			case cmd_not_exclusively:
				ActionOpenBaseNotMonopolyChecked = true;
				break;
			case cmd_xml_blob_to_file:
				f = pc.param1.LowerCase();
				b = f.Compare(L"1") == 0 || f.Compare(L"y") == 0 || f.Compare(L"yes") || f.Compare(L"д") || f.Compare(L"да") == 0;
				ActionXMLSaveBLOBToFileChecked = b;
				break;
			case cmd_xml_parse_blob:
				f = pc.param1.LowerCase();
				b = f.Compare(L"1") == 0 || f.Compare(L"y") == 0 || f.Compare(L"yes") || f.Compare(L"д") || f.Compare(L"да") == 0;
				ActionXMLUnpackBLOBChecked = b;
				break;
		}
	}

	f = comm->getfilename();
	if(f.IsEmpty())
	{
		mess->AddMessage(L"В командной строке не найден файл базы 1CD", msError);
		return 3;
	}
	f = System::Ioutils::TPath::GetFullPath(f);
	base1CD = new T_1CD(f, mess, !ActionOpenBaseNotMonopolyChecked);
	if(base1CD->is_open())
	{
		mess->AddMessage_(L"База данных 1CD открыта", msSuccesfull,
			L"Файл", f,
			L"Версия базы", base1CD->ver,
			L"Locale", base1CD->locale,
			L"Режим", base1CD->get_readonly() ? L"Только чтение" : L"Редактирование",
			L"Количество таблиц", base1CD->get_numtables());
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

							f = pc.param1 + L"\\" + t->getname() + L".xml";

							t->export_to_xml(f, ActionXMLSaveBLOBToFileChecked, ActionXMLUnpackBLOBChecked);
							mess->AddMessage_(L"Выполнен экспорт таблицы в файл.", msSuccesfull,
								L"Таблица", t->getname(),
								L"Файл", f);
						}
					}
					else mess->AddError(L"Попытка выгрузки всех таблиц в XML без открытой базы.");
					break;

				case cmd_export_to_xml:
					if(base1CD->is_open())
					{

						filter = new Sysutils::TStringBuilder(pc.param2);
						filter->Replace(L"*", L".*");
						filter->Replace(L'?', L'.');
						filter->Replace(L" ", L"\r\n");
						filter->Replace(L"\t", L"\r\n");
						filter->Replace(L",", L"\r\n");
						filter->Replace(L";", L"\r\n");

						filters = new TStringList;
						filters->Text = filter->ToString();
						for(m = filters->Count - 1; m >= 0; m--)
						{
							if(filters->Strings[m].Length() == 0) filters->Delete(m);
							else filters->Strings[m] = String(L"^") + filters->Strings[m].UpperCase() + L"$";
						}

						k = filters->Count;
						if(k == 0)
						{
							mess->AddError(L"Список таблиц для выгрузки в XML пуст.");
							delete filter;
							delete filters;
							break;
						}

						expr = new boost::wregex[k];
						for(m = 0; m < k; m++) expr[m] = boost::wregex(filters->Strings[m].w_str());

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

								f = pc.param1 + L"\\" + t->getname() + L".xml";

								t->export_to_xml(f, ActionXMLSaveBLOBToFileChecked, ActionXMLUnpackBLOBChecked);
								mess->AddMessage_(L"Выполнен экспорт таблицы в файл.", msSuccesfull,
									L"Таблица", t->getname(),
									L"Файл", f);
							}

						}

						delete[] expr;
						delete filter;
						delete filters;

					}
					else mess->AddError(L"Попытка выгрузки таблиц в XML без открытой базы.");

					break;
				case cmd_save_config:
					if(base1CD->is_open())
					{
						f = pc.param1;
						f = System::Ioutils::TPath::GetFullPath(f);
						if(f.SubString(f.Length() - 2, 3).CompareIC(L".cf") != 0)
						{
							if(!DirectoryExists(f))
							{
								mess->AddMessage_(L"Каталог не существует.", msError,
									L"Каталог", f);
								break;
							}
							f = f + L"\\dbcf.cf";
						}
						if(base1CD->save_config(f))
							mess->AddMessage_(L"Сохранение конфигурации базы данных завершено.", msSuccesfull,
								L"Файл", f);
						else
							mess->AddMessage_(L"Не удалось сохранить конфигурацию базы данных.", msError,
								L"Файл", f);
					}
					else mess->AddError(L"Попытка выгрузки конфигурации базы данных без открытой базы.");
					break;
				case cmd_save_configsave:
					if(base1CD->is_open())
					{
						f = pc.param1;
						f = System::Ioutils::TPath::GetFullPath(f);
						if(f.SubString(f.Length() - 2, 3).CompareIC(L".cf") != 0)
						{
							if(!DirectoryExists(f))
							{
								mess->AddMessage_(L"Каталог не существует.", msError,
									L"Каталог", f);
								break;
							}
							f = f + L"\\cf.cf";
						}
						if(base1CD->save_configsave(f))
							mess->AddMessage_(L"Сохранение основной конфигурации завершено.", msSuccesfull,
								L"Файл", f);
						else
							mess->AddMessage_(L"Не удалось сохранить основную конфигурацию.", msError,
								L"Файл", f);
					}
					else mess->AddError(L"Попытка выгрузки основной конфигурации без открытой базы.");
					break;
				case cmd_save_vendors_configs:
					if(base1CD->is_open())
					{
						base1CD->find_supplier_configs();
						for(n = 0; n < base1CD->supplier_configs.size(); n++)
						{
							f = pc.param1 + L"\\" + base1CD->supplier_configs[n].name + L" " + base1CD->supplier_configs[n].version + L".cf";
							if(base1CD->save_supplier_configs(n, f))
								mess->AddMessage_(L"Сохранение конфигурации поставщика завершено.", msSuccesfull,
									L"Файл", f);
							else
								mess->AddMessage_(L"Не удалось сохранить конфигурацию поставщика.", msError,
									L"Файл", f);
						}
					}
					else mess->AddError(L"Попытка выгрузки конфигураций поставщиков без открытой базы.");
					break;
				case cmd_save_all_configs:
					if(base1CD->is_open())
					{
						f = pc.param1 + L"\\dbcf.cf";
						if(base1CD->save_config(f))
							mess->AddMessage_(L"Сохранение конфигурации базы данных завершено.", msSuccesfull,
								L"Файл", f);
						else
							mess->AddMessage_(L"Не удалось сохранить конфигурацию базы данных.", msError,
								L"Файл", f);

						f = pc.param1 + L"\\cf.cf";
						if(base1CD->save_configsave(f))
							mess->AddMessage_(L"Сохранение основной конфигурации завершено.", msSuccesfull,
								L"Файл", f);
						else
							mess->AddMessage_(L"Не удалось сохранить основную конфигурацию.", msError,
								L"Файл", f);

						base1CD->find_supplier_configs();
						for(n = 0; n < base1CD->supplier_configs.size(); n++)
						{
							f = pc.param1 + L"\\" + base1CD->supplier_configs[n].name + L" " + base1CD->supplier_configs[n].version + L".cf";
							if(base1CD->save_supplier_configs(n, f))
								mess->AddMessage_(L"Сохранение конфигурации поставщика завершено.", msSuccesfull,
									L"Файл", f);
							else
								mess->AddMessage_(L"Не удалось сохранить конфигурацию поставщика.", msError,
									L"Файл", f);
						}
					}
					else mess->AddError(L"Попытка выгрузки всех конфигураций без открытой базы.");
					break;
				case cmd_save_depot_config:
					if(!base1CD->is_open())
					{
						mess->AddError(L"Попытка выгрузки конфигурации хранилища без открытой базы.");
						break;
					}
					if(!base1CD->is_depot)
					{
						if(mess) mess->AddError(L"Попытка выгрузки конфигурации хранилища из базы, не являющейся хранилищем конфигурации.");
						break;
					}
					f = pc.param1;
					if(f.Compare(L"0") == 0) j = 0;
					else j = f.ToIntDef(0);
					j = base1CD->get_ver_depot_config(j);
					if(!j)
					{
						if(mess) mess->AddError(L"Запрошенной версии конфигурации нет в хранилище конфигурации.");
						break;
					}
					f = pc.param2;
					f = System::Ioutils::TPath::GetFullPath(f);
					if(f.SubString(f.Length() - 2, 3).CompareIC(L".cf") != 0)
					{
						if(!DirectoryExists(f))
						{
							mess->AddMessage_(L"Каталог не существует.", msError,
								L"Каталог", f);
							break;
						}
						f = f + L"\\v" + j + L".cf";
					}
					if(base1CD->save_depot_config(f, j))
						mess->AddMessage_(L"Сохранение конфигурации хранилища завершено.", msSuccesfull,
							L"Файл", f);
					else
						mess->AddMessage_(L"Не удалось сохранить конфигурацию хранилища.", msError,
							L"Файл", f);
					break;
				case cmd_save_depot_config_part:
					if(!base1CD->is_open())
					{
						mess->AddError(L"Попытка выгрузки файлов конфигурации хранилища без открытой базы.");
						break;
					}
					if(!base1CD->is_depot)
					{
						if(mess) mess->AddError(L"Попытка выгрузки файлов конфигурации хранилища из базы, не являющейся хранилищем конфигурации.");
						break;
					}
					f = pc.param1;
					k = f.Pos(L":");
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
						if(mess) mess->AddError(L"Запрошенной версии конфигурации нет в хранилище конфигурации."
							, "Версия", j);
						break;
					}
					k = base1CD->get_ver_depot_config(k);
					if(!k)
					{
						if(mess) mess->AddError(L"Запрошенной версии конфигурации нет в хранилище конфигурации."
							, "Версия", k);
						break;
					}
					f = pc.param2;
					f = System::Ioutils::TPath::GetFullPath(f);
					if(!DirectoryExists(f)) System::Ioutils::TDirectory::CreateDirectory(f);
					if(base1CD->save_part_depot_config(f, j, k))
						mess->AddMessage_(L"Сохранение файлов конфигурации хранилища завершено.", msSuccesfull,
							L"Файл", f);
					else
						mess->AddMessage_(L"Не удалось сохранить файлы конфигурации хранилища.", msError,
							L"Файл", f);
					break;
			}
		}
		catch(String s)
		{
			if(mess) mess->AddError(s);
		}
		catch(Exception& ex)
		{
			if(mess) mess->AddError(ex.Message);
		}
		catch(...)
		{
			if(mess) mess->AddError(L"Неизвестная ошибка.");
		}
	}

	delete base1CD;
	delete comm;
	comm = NULL;

	if(mess->has_error) ret = 1;
	else ret = 0;
	delete mess;
	return ret;
}

