//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <shlobj.h>
#include <boost/regex.hpp>
#include <IOUtils.hpp>

#include "Main_1CD.h"
#include "parse_tree.h"
#include "About.h"
#include "CommandLineDescriptionForm.h"
#include "OpenBlobDialog.h"
#include "Common.h"
//---------------------------------------------------------------------------
//#pragma package(smart_init)
#pragma link "ATBinHex"
#pragma link "ATViewer"
#pragma link "AdvGrid"
#pragma link "BaseGrid"
#pragma link "AdvMemo"
#pragma link "AdvmWS"
#pragma link "AdvGlowButton"
#pragma link "AdvSmoothButton"
#pragma link "GDIPPictureContainer"
#pragma link "AdvGlassButton"
#pragma link "AdvDirectoryEdit"
#pragma link "AdvEdBtn"
#pragma link "AdvEdit"
#pragma link "AdvToolBtn"
#pragma link "frmctrllink"
#pragma link "MaskEdEx"
#pragma link "AdvFileNameEdit"
#pragma link "AdvObj"
#pragma link "awaMaskEdit"
#pragma link "HTMLabel"
#pragma link "PictureContainer"
#pragma resource "*.dfm"
TFormMain *FormMain;

#pragma alias "@System@Win@Registry@TRegistry@SetRootKey$qqrpv" = "@System@Win@Registry@TRegistry@SetRootKey$qqrp6HKEY__"

const unsigned char SIG_ZIP[16] = {0x53,0x4b,0x6f,0xf4,0x88,0x8d,0xc1,0x4e,0xa0,0xd5,0xeb,0xb6,0xbd,0xa0,0xa7,0x0d};
//extern unsigned char SIG_ZIP[16];
extern TMultiReadExclusiveWriteSynchronizer* tr_syn;
const char sUTF16LEBOM[2] = {0xFF, 0xFE};
MessageRegistrator* msreg;

//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner) : TForm(Owner)
{

	ForceCurrentDirectory = true;

	mess = new Messager(ListViewMessage, StatusBar1);
	msreg = mess;
	//mess->setlogfile(L"c:\\tool_1cd.log");
	base1CD = new T_1CD;
	tab = NULL;
	tr_thread = NULL;
	tabrec = NULL;
	cat = NULL;
	blob_stream = new TMemoryStream;

	wchar_t* _td = new wchar_t[MAX_PATH + 2];
	temp_name = new wchar_t[MAX_PATH + 2];
	GetTempPath(256, _td);
	GetTempFileName(_td, L"awa", 0, temp_name);
	delete[] _td;
	temp_stream = new TMemoryStream;

	tr_syn = new TMultiReadExclusiveWriteSynchronizer;

	StringGridPhysicalView->CheckFalse = L"false";
	StringGridPhysicalView->CheckTrue = L"true";
	TabSheetDepotConfigSave->TabVisible = false;
	TabSheetConfigSave->TabVisible = false;

	SetEditTableVisible();

	edit_numfield = 0;
	edit_phys_numrecord = 0;
	edit_tab = false;

	#ifdef _DEBUG
	ActionDebugMessages->Visible = true;
	#endif

	comm = new CommandParse(GetCommandLineW(), mess);

	#ifdef PublicRelease
	TabSheetUtilities->TabVisible = false;
	#endif

}

//---------------------------------------------------------------------------
__fastcall TFormMain::~TFormMain()
{
	delete temp_stream;
	DeleteFile(temp_name);
	delete[] temp_name;
	delete blob_stream;
	ListViewTables->Clear();
	tab = NULL;
	blob_stream = NULL;
	if(tr_thread)
	{
		tr_thread->Terminate();
		tr_thread->WaitFor();
		delete tr_thread;
		tr_thread = NULL;
	}
	delete base1CD;
	delete mess;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::FormActivate(TObject *Sender)
{
	int i, j, k, m;
	unsigned int n;
	table* t;
	index* ci;
	String f, v;
	bool b;
	Sysutils::TStringBuilder* filter;
	TStringList* filters;
	boost::wregex* expr;

	if(comm)
	{
		DynamicArray<ParsedCommand>& commands = comm->getcommands();
		bool quit = false;

		// Первый цикл просмотра команд для определения ключей параметров
		for(i = 0; i < commands.get_length(); i++)
		{
			ParsedCommand& pc = commands[i];
			switch(pc.command)
			{
//				case cmd_help: // в GUI режиме это не работает!
//					cout << AnsiString(comm->gethelpstring()).c_str();
//					break;
//				case cmd_no_verbose:  // в GUI режиме это не работает!
//					break;
				case cmd_quit:
					quit = true;
					break;
				case cmd_logfile:
					mess->setlogfile(pc.param1);
					break;
				case cmd_not_exclusively:
					ActionOpenBaseNotMonopoly->Checked = true;
					break;
				case cmd_xml_blob_to_file:
					f = pc.param1.LowerCase();
					b = f.Compare(L"1") == 0 || f.Compare(L"y") == 0 || f.Compare(L"yes") || f.Compare(L"д") || f.Compare(L"да") == 0;
					ActionXMLSaveBLOBToFile->Checked = b;
					break;
				case cmd_xml_parse_blob:
					f = pc.param1.LowerCase();
					b = f.Compare(L"1") == 0 || f.Compare(L"y") == 0 || f.Compare(L"yes") || f.Compare(L"д") || f.Compare(L"да") == 0;
					ActionXMLUnpackBLOB->Checked = b;
					break;
			}
		}

		if(comm->getfilename().Length() > 0)
		{
			f = comm->getfilename();
			f = System::Ioutils::TPath::GetFullPath(f);
			Open1CDFile(f);
			//SetCurrentDir(f.SubString(1, f.LastDelimiter(L"\\")));

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

									t->export_to_xml(f, ActionXMLSaveBLOBToFile->Checked, ActionXMLUnpackBLOB->Checked);
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

										t->export_to_xml(f, ActionXMLSaveBLOBToFile->Checked, ActionXMLUnpackBLOB->Checked);
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
								ActionFindSupplierConfigsExecute(FormMain);
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

								ActionFindSupplierConfigsExecute(FormMain);
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
		}

		if(quit) PostMessage(Handle, WM_CLOSE, 0, 0);

		delete comm;
		comm = NULL;
	}

}


//---------------------------------------------------------------------------
__fastcall Messager::Messager(TListView* lv, TStatusBar* sb)
{
	ListView = lv;
	StatusBar = sb;
	FormatSettings.DateSeparator = L'.';
	FormatSettings.TimeSeparator = L':';
	//FormatSettings.LongDateFormat = L"dd.mm.yyyy";
	FormatSettings.ShortDateFormat = L"dd.mm.yyyy";
	FormatSettings.LongTimeFormat = L"hh:mm:ss:zzz";
}

//---------------------------------------------------------------------------
void __fastcall Messager::setlogfile(String _logfile)
{
	logfile = System::Ioutils::TPath::GetFullPath(_logfile);
	if(FileExists(logfile)) DeleteFile(logfile);
}


//---------------------------------------------------------------------------
void __fastcall Messager::Status(const String& message)
{
	StatusBar->Panels->Items[0]->Text = message;
	StatusBar->Update();
}

//---------------------------------------------------------------------------
void __fastcall Messager::AddMessage(const String& message, const MessageState mstate, TStringList* param)
{
	TFileStream* log = NULL;
	TStreamWriter* sw;
	String s;

	ListView->AddItem(message, param);
	TListItem* item = ListView->Items->Item[ListView->Items->Count - 1];
	item->StateIndex = mstate;
	ListView->Selected = item;
	ListView->Scroll(0, 0xfffffff);
	ListView->Update();

	if(logfile.Length())
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

}

//---------------------------------------------------------------------------
void __fastcall TFormMain::RefreshListOfTables()
{
	int i, j;
	table* t;
	float perc;
	unsigned __int64 fmax;
	unsigned __int64 bmax;
	unsigned __int64 dmax;
	unsigned int version;
	unsigned int pagesize;

	ListViewTables->Clear();
	AdvStringGridStatistic->RowCount = 1;
	if(!base1CD->is_open()) return;

	version = base1CD->getversion();
	pagesize = base1CD->getpagesize();
	if(version < ver8_3_8_0) fmax = 4265631744; // 4096 * 1018 * 1023
	else fmax = (unsigned __int64)pagesize * (pagesize / 4 - 6) * (pagesize / 4);
	bmax = min(0x10000000000ui64, fmax);

	for(i = 0; i < base1CD->get_numtables(); i++)
	{
		t = base1CD->gettable(i);

		TListItem* li = ListViewTables->Items->Add();
		li->Caption = t->getname();
		li->Data = (TObject*)i;
		li->ImageIndex = 2;

		if(t->get_file_data())
		{
			j = AdvStringGridStatistic->RowCount;
			AdvStringGridStatistic->RowCount = j + 1;
			AdvStringGridStatistic->Cells[0][j] = t->getname();
			AdvStringGridStatistic->Cells[1][j] = L"DATA";
			AdvStringGridStatistic->Cells[2][j] = t->get_file_data()->getlen();
			dmax = min(0x100000000ui64 * t->get_recordlen(), fmax);
			perc = t->get_file_data()->getlen();
			perc = perc * 100 / dmax;
			AdvStringGridStatistic->Floats[3][j] = perc;
			AdvStringGridStatistic->Cells[4][j] = dmax;
		}
		if(t->get_file_blob())
		{
			j = AdvStringGridStatistic->RowCount;
			AdvStringGridStatistic->RowCount = j + 1;
			AdvStringGridStatistic->Cells[0][j] = t->getname();
			AdvStringGridStatistic->Cells[1][j] = L"BLOB";
			AdvStringGridStatistic->Cells[2][j] = t->get_file_blob()->getlen();
			perc = t->get_file_blob()->getlen();
			perc = perc * 100 / bmax;
			AdvStringGridStatistic->Floats[3][j] = perc;
			AdvStringGridStatistic->Cells[4][j] = bmax;
		}
		if(t->get_file_index())
		{
			j = AdvStringGridStatistic->RowCount;
			AdvStringGridStatistic->RowCount = j + 1;
			AdvStringGridStatistic->Cells[0][j] = t->getname();
			AdvStringGridStatistic->Cells[1][j] = L"INDEX";
			AdvStringGridStatistic->Cells[2][j] = t->get_file_index()->getlen();
			perc = t->get_file_index()->getlen();
			perc = perc * 100 / fmax;
			AdvStringGridStatistic->Floats[3][j] = perc;
			AdvStringGridStatistic->Cells[4][j] = fmax;
		}
	}
	AdvStringGridStatistic->FixedRows = 1;
	AdvStringGridStatistic->SortSettings->Column = 2;
	AdvStringGridStatistic->SortSettings->Direction = sdDescending;
	AdvStringGridStatistic->QSort();

}


//---------------------------------------------------------------------------
void __fastcall TFormMain::Open1CDFile(String FileName)
{
	FileName = System::Ioutils::TPath::GetFullPath(FileName);
	ButtonedEditFile->Text = FileName;

	ListViewTablesSelectItem(ListViewTables, NULL, false);
	ListViewTables->Clear();
	MemoDescriptionTable->Clear();
	ValueListEditorDescriprionTable->Strings->Clear();
	StringGridDescriptionFields->RowCount = 1;
	TreeViewDescriptionIndexes->Items->Clear();
	StringGridPhysicalView->RowCount = 1;
	StringGridPhysicalView->ColCount = 1;
	StringGridPhysicalView->Cols[0]->Clear();
	PanelBlobView->Visible = false;
	TabSheetDepotConfigSave->TabVisible = false;
	TabSheetConfigSave->TabVisible = false;
	ATViewerBlob->Open(L"");
	ATViewerBlob->OpenStream(NULL, vmodeText);
	TreeViewContainer->Items->Clear();
	delete base1CD;
	ValueListEditorMessage->Strings->Clear();
	ListViewMessage->Clear();
	ActionSaveConfig->Visible = false;
	ActionSaveConfigSave->Visible = false;
	ActionSaveLastDepotConfig->Visible = false;
	ActionFindSupplierConfigs->Visible = false;
	StringGridSupplierConfigs->RowCount = 1;
	PanelSupplierConfigs->Visible = false;
	ActionSaveSupplierConfig->Visible = false;
	ActionExportTableToXML->Visible = false;
	ActionSaveIndexFile->Visible = false;
	ActionSaveDataFile->Visible = false;
	ActionSaveBlobFile->Visible = false;
	ToolButtonWriteBlobToFile->Visible = false;
	ToolButtonReadBlobFromFile->Visible = false;
	ToolButtonBlobClear->Visible = false;
	ToolButtonBlobRestore->Visible = false;
	LabelPhysPreview->Caption = L"";

	FormMain->Refresh();

	base1CD = new T_1CD(FileName, mess, !ActionOpenBaseNotMonopoly->Checked);
	AdvStringGridStatistic->RowCount = 2;
	if(base1CD->is_open())
	{
		RefreshListOfTables();

		mess->Status(L"");
		mess->AddMessage_(L"База данных 1CD открыта", msSuccesfull,
			L"Файл", ButtonedEditFile->Text,
			L"Версия базы", base1CD->ver,
			L"Размер страницы", base1CD->getpagesize(),
			L"Locale", base1CD->locale,
			L"Режим", base1CD->get_readonly() ? L"Только чтение" : L"Редактирование",
			L"Количество таблиц", base1CD->get_numtables());
		if(base1CD->table_config)
		{
			ActionSaveConfig->Visible = true;
			ActionFindSupplierConfigs->Visible = true;
			PanelSupplierConfigs->Visible = true;
//				ActionSaveSupplierConfig->Visible = true;
		}
		if(base1CD->table_configsave){
			ActionSaveConfigSave->Visible = true;
			PanelSupplierConfigs->Visible = true;
//				ActionSaveSupplierConfig->Visible = true;
			ActionFindSupplierConfigs->Visible = true;
		}
		if(base1CD->is_infobase)
		{
			TabSheetConfigSave->TabVisible = true;
		}
		if(base1CD->is_depot)
		{
			ActionSaveLastDepotConfig->Visible = true;
			TabSheetDepotConfigSave->TabVisible = true;
		}

		SetEditTableVisible();

	}
	ListViewTables->Enabled = true;
	#ifndef PublicRelease
	TabSheetUtilities->TabVisible = true;
	#endif
	ListViewTablesSelectItem(ListViewTables, ListViewTables->Selected, true);

}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionFileOpenExecute(TObject *Sender)
{
	if(OpenDialog->Execute()) Open1CDFile(OpenDialog->FileName);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionExitExecute(TObject *Sender)
{
	FormMain->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ListViewMessageSelectItem(TObject *Sender, TListItem *Item, bool Selected)
{
	if(Item->Data) ValueListEditorMessage->Strings = (TStrings*)(Item->Data);
	else ValueListEditorMessage->Strings->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::CheckBoxSortListTables_Click(TObject *Sender)
{
	ListViewTables->SortType = CheckBoxSortListTables->Checked ? TSortType::stText : TSortType::stData;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionDebugMessagesExecute(TObject *Sender)
{
	#ifdef _DEBUG
	bool ch = !mess->getDebugMode();
	mess->setDebugMode(ch);
	ActionDebugMessages->Checked = ch;
	#endif
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::setBlobViewPropertiesVisible(bool _Visible)
{
	ActionBlobView->Visible = _Visible;
	PanelBlobViewProperties->Visible = _Visible;
	SplitterBlobViewProperties->Visible = _Visible;
	SplitterBlobViewProperties->Left = PanelBlobViewProperties->Left - 1;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::setContainerListVisible(bool _Visible)
{
	PanelContainerList->Visible = _Visible;
	SplitterContainerList->Visible = _Visible;
	SplitterContainerList->Left = PanelContainerList->Left + 1;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::setBlobViewText(bool is_string, bool is_unicode)
{
	ATViewerBlob->Open(L"");
	ATViewerBlob->OpenStream(NULL, vmodeText);
	if(is_string)
	{
		if(is_unicode) ATViewerBlob->OpenStream(temp_stream, vmodeUnicode);
		else  ATViewerBlob->OpenStream(temp_stream, vmodeText);
		setBlobViewPropertiesVisible(false);
		setContainerListVisible(false);
	}
	else
	{
		TFileStream* fs = new TFileStream(temp_name, fmCreate | fmShareDenyNone);
		fs->CopyFrom(temp_stream, 0);
		delete fs;
		ATViewerBlob->Open(temp_name);

		setBlobViewPropertiesVisible(true);
		if(ATViewerBlob->ModeDetect) switch(ATViewerBlob->Mode)
		{
			case vmodeText:
				ComboBoxExBlobViewMode->ItemIndex = 0;
				ActionBlobViewANSI->Checked = true;
				ActionBlobViewTextWrap->Visible = true;
				break;
			case vmodeRTF:
				ComboBoxExBlobViewMode->ItemIndex = 1;
				ActionBlobViewUTF8->Checked = true;
				ActionBlobViewTextWrap->Visible = false;
				break;
			case vmodeUnicode:
				ComboBoxExBlobViewMode->ItemIndex = 2;
				ActionBlobViewUTF16->Checked = true;
				ActionBlobViewTextWrap->Visible = true;
				break;
			case vmodeMedia:
				ComboBoxExBlobViewMode->ItemIndex = 3;
				ActionBlobViewMedia->Checked = true;
				ActionBlobViewTextWrap->Visible = false;
				break;
			case vmodeWeb:
				ComboBoxExBlobViewMode->ItemIndex = 4;
				ActionBlobViewWEB->Checked = true;
				ActionBlobViewTextWrap->Visible = false;
				break;
			case vmodeHex:
				ComboBoxExBlobViewMode->ItemIndex = 5;
				ActionBlobViewHEX->Checked = true;
				ActionBlobViewTextWrap->Visible = false;
				break;
		}

	}

}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ParseBlob(int ARow = -1)
{
	bool zippedContainer;
	char _buf[16];
	TMemoryStream* _s;
	TMemoryStream* _s2;
	__int64 len1C;

	bool is_users_usr;
	field* _f;
	unsigned int i, j, k, l;
	char* _bb;
	char* _xor_mask;
	char* _xor_buf;

	if(cat)
	{
		delete cat;
		cat = NULL;
	}

	TreeViewContainer->Items->Clear();
	if(blob_stream->Size == 0)
	{
		ATViewerBlob->Open(L"");
		ATViewerBlob->OpenStream(NULL, vmodeText);
		setBlobViewPropertiesVisible(false);
		setContainerListVisible(false);
		return;
	}

	_s = new TMemoryStream;
	if(tab->get_issystem())
	{

		// спецобработка для users.usr
		is_users_usr = false;
		if(tab->getname().CompareIC(L"PARAMS") == 0)
		{
			if(ARow == -1) ARow = StringGridPhysicalView->Selection.Top;
			_f = tab->getfield(0);
			i = get_phys_numrec(ARow);
			_bb = tab->get_edit_record(i, tabrec);
			if(_f->get_presentation(_bb).CompareIC(L"users.usr") == 0) is_users_usr = true;
		}
		else if(tab->getname().CompareIC(L"V8USERS") == 0) is_users_usr = true;

		if(is_users_usr)
		{

			ActionBlobParseUnzip->Visible = false;
			ActionBlobParseUnzip2->Visible = false;
			ActionBlobParseContainer->Visible = false;
			ActionBlobParseUsersUsr->Visible = true;
			CheckBoxBlobParseUsersUsr->Top = CheckBoxBlobParseModeDetect->Top + 1;

			if(ActionBlobParseModeDetect->Checked) ActionBlobParseUsersUsr->Checked = true;
			if(ActionBlobParseUsersUsr->Checked)
			{
				i = blob_stream->Size;
				_bb = new char[i];
				blob_stream->Seek(0, soFromBeginning);
				blob_stream->Read(_bb, i);

				j = _bb[0];
				_xor_mask = _bb + 1;
				_xor_buf = _xor_mask + j;
				l = i - j - 1;
				for(i = 0, k = 0; i < l; i++, k++)
				{
					if(k >= j) k = 0;
					_xor_buf[i] ^= _xor_mask[k];
				}
				temp_stream->Size = 0;
				temp_stream->WriteBuffer(_xor_buf, l);
				delete[] _bb;
				setContainerListVisible(false);
				setBlobViewText(false);
			}
			else{
				setContainerListVisible(false);
				temp_stream->Size = 0;
				temp_stream->CopyFrom(blob_stream, 0);
				setBlobViewText(false);
			}
		}
		else
		{
			ActionBlobParseUnzip->Visible = true;
			CheckBoxBlobParseUnzip->Top = CheckBoxBlobParseModeDetect->Top + 1;
			ActionBlobParseUnzip2->Visible = true;
			CheckBoxBlobParseUnzip2->Top = CheckBoxBlobParseUnzip->Top + 1;
			ActionBlobParseContainer->Visible = true;
			CheckBoxBlobParseContainer->Top = CheckBoxBlobParseUnzip2->Top + 1;
			ActionBlobParseUsersUsr->Visible = false;

			zippedContainer = false;
			if(ActionBlobParseModeDetect->Checked || ActionBlobParseUnzip->Checked)
			{
				_s2 = new TMemoryStream;
				_s2->CopyFrom(blob_stream, 0);
				try
				{
					_s2->Seek(0, soFromBeginning);
					_s->Size = 0;
					InflateStream(_s2, _s);
					ActionBlobParseUnzip->Checked = true;
				}
				catch (...)
				{
					_s->Size = 0;
					_s->CopyFrom(blob_stream, 0);
					if(ActionBlobParseModeDetect->Checked)
					{
						ActionBlobParseUnzip->Checked = false;
						ActionBlobParseUnzip2->Checked = false;
					}
					else if(mess)
						mess->AddError(L"Ошибка распаковки Blob",
						L"Таблица", tab->getname());
				}

				if((ActionBlobParseModeDetect->Checked && ActionBlobParseUnzip->Checked) || (ActionBlobParseUnzip->Checked && ActionBlobParseUnzip2->Checked))
				{
					_s2->Size = 0;
					_s2->CopyFrom(_s, 0);
					try
					{
						_s2->Seek(0, soFromBeginning);
						_s->Size = 0;
						InflateStream(_s2, _s);
						ActionBlobParseUnzip2->Checked = true;
						zippedContainer = true;
					}
					catch (...)
					{
						_s->Size = 0;
						_s->CopyFrom(_s2, 0);
						if(ActionBlobParseModeDetect->Checked) ActionBlobParseUnzip2->Checked = false;
						else if(mess)
							mess->AddError(L"Ошибка распаковки 2 Blob",
							L"Таблица", tab->getname());
					}
				}

				delete _s2;
			}
			else _s->CopyFrom(blob_stream, 0);

			if(ActionBlobParseModeDetect->Checked || ActionBlobParseContainer->Checked)
			{

				cat = new v8catalog(_s, zippedContainer, true);
				if(!cat->GetFirst())
				{
					delete cat;
					cat = NULL;
					if(ActionBlobParseModeDetect->Checked) ActionBlobParseContainer->Checked = false;
					else if(mess)
						mess->AddError(L"Ошибка преобразования Blob в контейнер",
						L"Таблица", tab->getname());
					setContainerListVisible(false);
					temp_stream->Size = 0;
					temp_stream->CopyFrom(_s, 0);
					setBlobViewText(false);
				}
				else
				{
					_s = NULL; // Поток _s используется в контейнере cat, для предотвращения удаления потока _s
					ActionBlobParseContainer->Checked = true;
					v8file* v8f = cat->GetFirst();
					while(v8f)
					{
						TTreeNode* node = TreeViewContainer->Items->Add(NULL, v8f->GetFileName());
						if(v8f->IsCatalog())
						{
							node->ImageIndex = 10; // папка
							node->SelectedIndex = 10; // папка
							node->ExpandedImageIndex = 11; // раскрытая папка
							node->Data = NULL;
							v8file* v8f2 = v8f->GetCatalog()->GetFirst();
							while(v8f2)
							{
								TTreeNode* node2 = TreeViewContainer->Items->AddChildObject(node, v8f2->GetFileName(), v8f2);
								node2->ImageIndex = 12; // простой файл
								node2->SelectedIndex = 13; // читаемый файл
								v8f2 = v8f2->GetNext();
							}
						}
						else
						{
							node->ImageIndex = 12; // простой файл
							node->SelectedIndex = 13; // читаемый файл
							node->Data = v8f;
						}
						v8f = v8f->GetNext();
					}
					setContainerListVisible(true);
					TreeViewContainer->Items->Item[0]->Selected = true;
				}
			}
			else{
				setContainerListVisible(false);
				temp_stream->Size = 0;
				temp_stream->CopyFrom(_s, 0);
				setBlobViewText(false);
			}

		}

	}
	else /*if(tab->get_issystem())*/
	{
		setContainerListVisible(false);
		_s->CopyFrom(blob_stream, 0);
		if(ActionBlobParseModeDetect->Checked || ActionBlobParseValueStorage->Checked)
		{
			blob_stream->Seek(0, soFromBeginning);
			if(blob_stream->Read(_buf, 2) < 2)
			{
				if(ActionBlobParseModeDetect->Checked) ActionBlobParseValueStorage->Checked = false;
				else if(mess)
					mess->AddError(L"Ошибка преобразования Blob в хранилище значения. Длина поля меньше 2",
					L"Таблица", tab->getname(),
					L"Длина Blob", blob_stream->Size);
			}
			else
			{
				if(!(_buf[0] == 1 || _buf[0] == 2) || _buf[1] != 1)
				{
					if(ActionBlobParseModeDetect->Checked) ActionBlobParseValueStorage->Checked = false;
					else if(mess)
						mess->AddError(L"Ошибка преобразования Blob в хранилище значения. Неизвестный заголовок",
						L"Таблица", tab->getname(),
						L"Байт0", tohex(_buf[0]),
						L"Байт1", tohex(_buf[1]));
				}
				else
				{
					_s2 = new TMemoryStream;
					bool isOK = true;
					if(_buf[0] == 1) // неупакованное хранилище
					{
						_s2->CopyFrom(blob_stream, blob_stream->Size - 2);
					}
					else
					{
						if(blob_stream->Read(_buf, 16) < 16)
						{
							if(ActionBlobParseModeDetect->Checked) ActionBlobParseValueStorage->Checked = false;
							else if(mess)
								mess->AddError(L"Ошибка преобразования Blob в хранилище значения. Длина упакованного поля меньше 18",
								L"Таблица", tab->getname(),
								L"Длина Blob", blob_stream->Size);
							isOK = false;
						}
						else
						{
							if(memcmp(_buf, SIG_ZIP, 16) != 0)
							{
								if(ActionBlobParseModeDetect->Checked) ActionBlobParseValueStorage->Checked = false;
								else if(mess)
									mess->AddError(L"Ошибка преобразования Blob в хранилище значения. Сигнатура упаковщика не 0x534b6ff4888dc14ea0d5ebb6bda0a70d",
//									L"Таблица", tab->getname(),
//									L"Длина Blob", blob_stream->Size);
									L"Таблица", tab->getname());
								isOK = false;
							}
							else
							{
								try
								{
									InflateStream(blob_stream, _s2);
								}
								catch(...)
								{
									if(ActionBlobParseModeDetect->Checked) ActionBlobParseValueStorage->Checked = false;
									else if(mess)
										mess->AddError(L"Ошибка преобразования Blob в хранилище значения. Ошибка распаковки",
										L"Таблица", tab->getname());
									isOK = false;
								}
							}
						}
					}
					if(isOK)
					{
						_s2->Seek(0, soFromBeginning);
						if(_s2->Read(_buf, 8) < 8)
						{
							if(ActionBlobParseModeDetect->Checked) ActionBlobParseValueStorage->Checked = false;
							else if(mess)
								mess->AddError(L"Ошибка преобразования Blob в хранилище значения. Длина поля заголовка меньше 4",
								L"Таблица", tab->getname(),
								L"Длина поля заголовка", _s2->Size);
							isOK = false;
						}
						else
						{
							_s->Size = 0;
							_s->CopyFrom(_s2, _s2->Size - 8);
						}

					}
					if(ActionBlobParseModeDetect->Checked && isOK) ActionBlobParseValueStorage->Checked = true;

					if(isOK)
					{
						if(ActionBlobParseModeDetect->Checked || ActionBlobParseValue1C->Checked)
						{
							len1C = *(__int64*)_buf;
							if(_s->Size <= len1C)
							{
								if(ActionBlobParseModeDetect->Checked) ActionBlobParseValue1C->Checked = false;
								else if(mess)
									mess->AddError(L"Ошибка преобразования в значение 1С. Длина, указанная в заголовке, больше длины данных",
									L"Таблица", tab->getname(),
									L"Длина в заголовке", len1C,
									L"Длина данных", _s->Size);
								isOK = false;
							}
							else
							{
								_s->Seek(len1C, (TSeekOrigin)soFromBeginning);
								_s2->Size = 0;
								_s2->CopyFrom(_s, _s->Size - len1C);
								_s2->Seek(0, soFromBeginning);
								if(_s2->Read(_buf, 12) < 12)
								{
									if(ActionBlobParseModeDetect->Checked) ActionBlobParseValue1C->Checked = false;
									else if(mess)
										mess->AddError(L"Ошибка преобразования в значение 1С. Длина заголовка меньше 12",
										L"Таблица", tab->getname(),
										L"Длина заголовка", _s2->Size);
									isOK = false;
								}
								else
								{
									len1C = *(__int64*)&_buf[4];
									if(len1C > _s2->Size - 12)
									{
										if(ActionBlobParseModeDetect->Checked) ActionBlobParseValue1C->Checked = false;
										else if(mess)
											mess->AddError(L"Ошибка преобразования в значение 1С. Длина, указанная в заголовке, больше длины данных",
											L"Таблица", tab->getname(),
											L"Длина в заголовке", len1C,
											L"Длина данных", _s2->Size - 12);
										isOK = false;
									}
									else
									{
										_s->Size = 0;
										_s->CopyFrom(_s2, len1C);
									}
								}

							}
							if(isOK && ActionBlobParseModeDetect->Checked)  ActionBlobParseValue1C->Checked = true;

						}
					}

					delete _s2;
				}
			}
		}

		temp_stream->Size = 0;
		temp_stream->CopyFrom(_s, 0);
		setBlobViewText(false);
	}

	delete _s;

}

//---------------------------------------------------------------------------
void __fastcall TFormMain::StringGridPhysicalViewSelectCell(TObject *Sender, int ACol,
		  int ARow, bool &CanSelect)
{
	unsigned int numr;
	TStream* st = NULL;
	bool notnull = true;

	if(cat)
	{
		delete cat;
		cat = NULL;
	}
	TreeViewContainer->Items->Clear();
	if(!base1CD->is_open()) return;
	if(!tab) return;
	blob_stream->Size = 0;

	ToolButtonWriteBlobToFile->Visible = false;
	ToolButtonReadBlobFromFile->Visible = false;
	ToolButtonBlobClear->Visible = false;
	ToolButtonBlobRestore->Visible = false;

	ActionSaveDepotConfig->Visible = false;
	if(base1CD->is_depot) if(tab == base1CD->table_versions) if(ARow > 0) ActionSaveDepotConfig->Visible = true;
	ActionSaveConfigExtension->Visible = false;
	if(base1CD->is_infobase) if(tab == base1CD->table__extensionsinfo) if(ARow > 0) ActionSaveConfigExtension->Visible = true;
	ActionSaveConfigExtensionDb->Visible = false;
	if(base1CD->is_infobase) if(tab == base1CD->table__extensionsinfo) if(ARow > 0) ActionSaveConfigExtensionDb->Visible = true;

	LabelPhysPreview->Caption = PhysViewGetDisplText(ACol, ARow, false, 160, true);

	if(ARow == 0 || ACol == 0)
	{
		ATViewerBlob->Open(L"");
		ATViewerBlob->OpenStream(NULL, vmodeText);
		setBlobViewPropertiesVisible(false);
		setContainerListVisible(false);
		return;
	}

	field* f = tab->getfield(ACol - 1);
	type_fields t = f->gettype();
	if(t == tf_image || t == tf_string || t == tf_text)
	{
//		if(cur_index) numr = cur_index->get_numrec(ARow - 1);
//		else
//		{
//			tr_syn->BeginRead();
//			numr = tab->recordsindex[ARow - 1];
//			tr_syn->EndRead();
//		}
		numr = get_phys_numrec(ARow);

		char* rec = tab->get_edit_record(numr, tabrec);
		rec += f->getoffset();
		if(f->getnull_exists())
		{
			if(*rec == 0)
			{
				temp_stream->Size = 0;
				temp_stream->WriteBuffer(L"{NULL}", 12);
				setBlobViewText(true);
				notnull = false;
			}
			rec++;
		}
#ifndef PublicRelease
		changed_rec_type tc = tab->get_rec_type(numr, ACol - 1);
		if(notnull)
		{
			if(tc == crt_changed || tc == crt_insert) st = *(TStream**)rec;
			if(t == tf_string)
			{
				if(st)
				{
					temp_stream->Size = 0;
					temp_stream->CopyFrom(st, 0);
				}
				else f->getparent()->readBlob(temp_stream, *(unsigned int*)rec, *(unsigned int*)(rec + 4));
				setBlobViewText(true);
			}
			else if(t == tf_text)
			{
				if(st)
				{
					temp_stream->Size = 0;
					temp_stream->CopyFrom(st, 0);
				}
				else f->getparent()->readBlob(temp_stream, *(unsigned int*)rec, *(unsigned int*)(rec + 4));
				setBlobViewText(true, false);
			}
			else //if(t == tf_image)
			{
				if(st)
				{
					blob_stream->Size = 0;
					blob_stream->CopyFrom(st, 0);
				}
				else f->getparent()->readBlob(blob_stream, *(unsigned int*)rec, *(unsigned int*)(rec + 4));
				ParseBlob(ARow);
			}
			//ToolButtonWriteBlobToFile->Visible = (*(unsigned int*)rec != 0 && *(unsigned int*)(rec + 4) != 0);
			ToolButtonWriteBlobToFile->Visible = *(unsigned int*)rec != 0;
		}
		if(tab->get_edit())
		{
			ToolButtonReadBlobFromFile->Visible = true;
			ToolButtonBlobClear->Visible = ToolButtonWriteBlobToFile->Visible;
			ToolButtonBlobRestore->Visible = tc == crt_changed;
		}
#else //#ifdef PublicRelease
		if(notnull)
		{
			if(t == tf_string)
			{
				f->getparent()->readBlob(temp_stream, *(unsigned int*)rec, *(unsigned int*)(rec + 4));
				setBlobViewText(true);
			}
			else if(t == tf_text)
			{
				f->getparent()->readBlob(temp_stream, *(unsigned int*)rec, *(unsigned int*)(rec + 4));
				setBlobViewText(true, false);
			}
			else //if(t == tf_image)
			{
				f->getparent()->readBlob(blob_stream, *(unsigned int*)rec, *(unsigned int*)(rec + 4));
				ParseBlob(ARow);
			}
			//ToolButtonWriteBlobToFile->Visible = (*(unsigned int*)rec != 0 && *(unsigned int*)(rec + 4) != 0);
			ToolButtonWriteBlobToFile->Visible = *(unsigned int*)rec != 0;
		}
#endif //#ifdef PublicRelease
	}
	else
	{
		ATViewerBlob->Open(L"");
		ATViewerBlob->OpenStream(NULL, vmodeText);
		setBlobViewPropertiesVisible(false);
		setContainerListVisible(false);
	}

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ListViewTablesCompare(TObject *Sender, TListItem *Item1,
		  TListItem *Item2, int Data, int &Compare)
{
	if(ListViewTables->SortType == TSortType::stText)
		Compare = Item1->Caption.CompareIC(Item2->Caption);
	else //if(ListViewTables->SortType == TSortType::stData)
		Compare = (int)(Item1->Data) - (int)(Item2->Data);

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PageControlTableViewChange(TObject *Sender)
{
	int i, j;
	TStringBuilder* sb;
	int numf;
	field* f;
	type_fields tf;
	bool hasBlob;
	bool _notused;

	// Очищаем данные других страниц
	MemoDescriptionTable->Text = L"";
	ComboBoxCurrentIndex->Items->Clear();
	ComboBoxCurrentIndex->Text = L"";

	StringGridPhysicalView->ColCount = 1;
	StringGridPhysicalView->RowCount = 1;
	StringGridPhysicalView->Cells[0][0] = L"";
	ATViewerBlob->Open(L"");
	ATViewerBlob->OpenStream(NULL, vmodeText);

	if(!tab) return;

	if(PageControlTableView->ActivePage == TabSheetDescription)
	{
		sb = new TStringBuilder(tab->getdescription());
		sb->Replace(L"\n", L"\r\n");
		MemoDescriptionTable->Text = sb->ToString();
		delete sb;

		ValueListEditorDescriprionTable->Strings->Clear();
		ValueListEditorDescriprionTable->Strings->Add(String(L"Имя=") + tab->getname());
		ValueListEditorDescriprionTable->Strings->Add(String(L"Длина записи=") + tab->get_recordlen());
		ValueListEditorDescriprionTable->Strings->Add(String(L"Recordlock=") + (tab->get_recordlock() ? L"Да" : L"Нет"));
		ValueListEditorDescriprionTable->Strings->Add(String(L"Размер файла записей=") + (tab->get_file_data() ? tab->get_file_data()->getlen() : 0));
		ValueListEditorDescriprionTable->Strings->Add(String(L"Размер Blob-файла=") + (tab->get_file_blob() ? tab->get_file_blob()->getlen() : 0));
		ValueListEditorDescriprionTable->Strings->Add(String(L"Размер файла индексов=") + (tab->get_file_index() ? tab->get_file_index()->getlen() : 0));
		#ifdef _DEBUG
		_version _ver;
		if(tab->get_file_data())
		{
			tab->get_file_data()->get_version(&_ver);
			ValueListEditorDescriprionTable->Strings->Add(String(L"Версия файла записей=") + _ver.version_1 + L":" + _ver.version_2);
		}
		if(tab->get_file_blob())
		{
			tab->get_file_blob()->get_version(&_ver);
			ValueListEditorDescriprionTable->Strings->Add(String(L"Версия Blob-файла=") + _ver.version_1 + L":" + _ver.version_2);
		}
		if(tab->get_file_index())
		{
			tab->get_file_index()->get_version(&_ver);
			ValueListEditorDescriprionTable->Strings->Add(String(L"Версия файла индексов=") + _ver.version_1 + L":" + _ver.version_2);
		}
		for(i = 0; i < tab->get_numindexes(); i++)
		{
			ValueListEditorDescriprionTable->Strings->Add(tab->getindex(i)->getname() + L"=" + tab->getindex(i)->get_length());
		}
		#endif

		StringGridDescriptionFields->RowCount = tab->get_numfields() + 1;
		StringGridDescriptionFields->Cells[0][0] = L"Имя";
		StringGridDescriptionFields->Cells[1][0] = L"Тип";
		StringGridDescriptionFields->Cells[2][0] = L"NULL";
		StringGridDescriptionFields->Cells[3][0] = L"Длина";
		StringGridDescriptionFields->Cells[4][0] = L"Точность";
		StringGridDescriptionFields->Cells[5][0] = L"Case sensitive";
		StringGridDescriptionFields->Cells[6][0] = L"Размер";
		StringGridDescriptionFields->Cells[7][0] = L"Смещение";

		for(i = 1; i <= tab->get_numfields(); i++)
		{
			f = tab->getfield(i - 1);
			StringGridDescriptionFields->Cells[0][i] = f->getname();
			StringGridDescriptionFields->Cells[1][i] = f->get_presentation_type();
			StringGridDescriptionFields->Cells[2][i] = f->getnull_exists() ? L"Да" : L"Нет";
			StringGridDescriptionFields->Cells[3][i] = f->getlength();
			StringGridDescriptionFields->Cells[4][i] = f->getprecision();
			StringGridDescriptionFields->Cells[5][i] = f->getcase_sensitive() ? L"Да" : L"Нет";
			StringGridDescriptionFields->Cells[6][i] = f->getlen();
			StringGridDescriptionFields->Cells[7][i] = f->getoffset();
		}
		StringGridDescriptionFields->AutoSize = true;

		TreeViewDescriptionIndexes->Items->Clear();
		for(i = 0; i < tab->get_numindexes(); i++)
		{
			index* ind = tab->getindex(i);
			TTreeNode* item = TreeViewDescriptionIndexes->Items->Add(NULL, ind->getname());
			if(ind->get_is_primary())
			{
				item->ImageIndex = 14;
				item->SelectedIndex = 14;
			}
			else
			{
				item->ImageIndex = 16;
				item->SelectedIndex = 16;
			}
			for(j = 0; j < ind->get_num_records(); j++)
			{
				TTreeNode* subitem = TreeViewDescriptionIndexes->Items->AddChild(item, ind->get_records()[j].field->getname());
				subitem->ImageIndex = 15;
				subitem->SelectedIndex = 15;
			}
		}

	}
//	else if(PageControlTableView->ActivePage == TabSheetLogicalView)
//	{
//
//	}
	else if(PageControlTableView->ActivePage == TabSheetPhysicalView)
	{
		numf = tab->get_numfields();

		hasBlob = false;

		StringGridPhysicalView->ColCount = numf + 1;
		for(i = 0; i < numf; i++)
		{
			f = tab->getfield(i);
			tf = f->gettype();
			if(tf == tf_image || tf == tf_string || tf == tf_text) hasBlob = true;
			if(tf == tf_bool) StringGridPhysicalView->AddCheckBoxColumn(i + 1, false, false);
		}
		PanelBlobView->Visible = hasBlob;
		SplitterBlobView->Visible = hasBlob;
		if(hasBlob) SplitterBlobView->Top = PanelBlobView->Top - 1;

		ComboBoxCurrentIndex->Items->Add(L"<None>");
		for(i = 0; i < tab->get_numindexes(); i++) ComboBoxCurrentIndex->Items->Add(tab->getindex(i)->getname());

		if(tab->get_numindexes()) ComboBoxCurrentIndex->ItemIndex = 1;
		else ComboBoxCurrentIndex->ItemIndex = 0;
		//ComboBoxCurrentIndex->ItemIndex = 0;

		ComboBoxCurrentIndexChange(ComboBoxCurrentIndex);

		StringGridPhysicalViewSelectCell(Sender, StringGridPhysicalView->Selection.Left, StringGridPhysicalView->Selection.Top, _notused);
	}
	else if(PageControlTableView->ActivePage == TabSheetUtilities)
	{

	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ListViewTablesSelectItem(TObject *Sender, TListItem *Item, bool Selected)
{
//	if(mess)
//	{
//		String s = L"ListViewTablesSelectItem; ";
//		if(Item)
//		{
//			s += L"Таблица = ";
//			s += Item->Caption;
//		}
//		else s += L"Item = NULL";
//		s += L"; Selected = ";
//		s += Selected ? L"true" : L"false";
//		mess->AddMessage(s, msHint, NULL);
//	}

	ActionSaveIndexFile->Visible = false;
	ActionSaveDataFile->Visible = false;
	ActionSaveBlobFile->Visible = false;
	if(!Selected || !Item)
	{
		if(!tab) return;
		if(Item) if(tab != base1CD->gettable((int)(Item->Data)))
		{
			return;
		}

		if(tr_thread)
		{
			tr_thread->Terminate();
			tr_thread->WaitFor();
			delete tr_thread;
			tr_thread = NULL;
		}

		if(tab) tab->set_lockinmemory(false);
		tab = NULL;
		delete[] tabrec;
		tabrec = NULL;

		ActionExportTableToXML->Visible = false;
		LabelCurrentTable->Caption = L"";
	}
	else if(base1CD->is_open())
	{
		tab = base1CD->gettable((int)(Item->Data));
		tab->set_lockinmemory(true);
		delete[] tabrec;
		tabrec = new char[tab->get_recordlen()];
		bool issys = tab->get_issystem();
		ActionBlobParseUnzip->Visible = issys;
		CheckBoxBlobParseUnzip->Top = CheckBoxBlobParseModeDetect->Top + 1;
		ActionBlobParseUnzip2->Visible = issys;
		CheckBoxBlobParseUnzip2->Top = CheckBoxBlobParseUnzip->Top + 1;
		ActionBlobParseContainer->Visible = issys;
		CheckBoxBlobParseContainer->Top = CheckBoxBlobParseUnzip2->Top + 1;
		ActionBlobParseUsersUsr->Visible = issys;
		CheckBoxBlobParseUsersUsr->Top = CheckBoxBlobParseContainer->Top + 1;

		ActionBlobParseValueStorage->Visible = !issys;
		CheckBoxBlobParseValueStorage->Top = CheckBoxBlobParseModeDetect->Top + 1;
		ActionBlobParseValue1C->Visible = !issys;
		CheckBoxBlobParseValue1C->Top = CheckBoxBlobParseValueStorage->Top + 1;

		ActionSaveIndexFile->Visible = true;
		ActionSaveDataFile->Visible = true;
		ActionSaveBlobFile->Visible = true;

		ActionExportTableToXML->Visible = true;
		LabelCurrentTable->Caption = tab->getname();
	}
	else
	{
		if(tr_thread)
		{
			tr_thread->Terminate();
			tr_thread->WaitFor();
			delete tr_thread;
			tr_thread = NULL;
		}
		tab = NULL;
		delete[] tabrec;
		tabrec = NULL;

		ActionExportTableToXML->Visible = false;
		LabelCurrentTable->Caption = L"";
	}
	SetEditTableVisible();
	PageControlTableViewChange(Sender);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionBlobViewModeDetectExecute(TObject *Sender)
{
	ATViewerBlob->ModeDetect = ActionBlobViewModeDetect->Checked;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionBlobViewExecute(TObject *Sender)
{
	return;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionBlobViewANSIExecute(TObject *Sender)
{
	ATViewerBlob->Mode = vmodeText;
	ComboBoxExBlobViewMode->ItemIndex = 0;
	ActionBlobViewTextWrap->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionBlobViewUTF8Execute(TObject *Sender)
{
	ATViewerBlob->Mode = vmodeRTF;
	ComboBoxExBlobViewMode->ItemIndex = 1;
	ActionBlobViewTextWrap->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionBlobViewUTF16Execute(TObject *Sender)
{
	ATViewerBlob->Mode = vmodeUnicode;
	ComboBoxExBlobViewMode->ItemIndex = 2;
	ActionBlobViewTextWrap->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionBlobViewMediaExecute(TObject *Sender)
{
	ATViewerBlob->Mode = vmodeMedia;
	ComboBoxExBlobViewMode->ItemIndex = 3;
	ActionBlobViewTextWrap->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionBlobViewWEBExecute(TObject *Sender)
{
	ATViewerBlob->Mode = vmodeWeb;
	ComboBoxExBlobViewMode->ItemIndex = 4;
	ActionBlobViewTextWrap->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionBlobViewHEXExecute(TObject *Sender)
{
	ATViewerBlob->Mode = vmodeHex;
	ComboBoxExBlobViewMode->ItemIndex = 5;
	ActionBlobViewTextWrap->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxExBlobViewModeChange(TObject *Sender)
{
	switch(ComboBoxExBlobViewMode->ItemIndex)
	{
		case 0:
			ATViewerBlob->Mode = vmodeText;
			ActionBlobViewANSI->Checked = true;
			ActionBlobViewTextWrap->Visible = true;
			break;
		case 1:
			ATViewerBlob->Mode = vmodeRTF;
			ActionBlobViewUTF8->Checked = true;
			ActionBlobViewTextWrap->Visible = false;
			break;
		case 2:
			ATViewerBlob->Mode = vmodeUnicode;
			ActionBlobViewUTF16->Checked = true;
			ActionBlobViewTextWrap->Visible = true;
			break;
		case 3:
			ATViewerBlob->Mode = vmodeMedia;
			ActionBlobViewMedia->Checked = true;
			ActionBlobViewTextWrap->Visible = false;
			break;
		case 4:
			ATViewerBlob->Mode = vmodeWeb;
			ActionBlobViewWEB->Checked = true;
			ActionBlobViewTextWrap->Visible = false;
			break;
		case 5:
			ATViewerBlob->Mode = vmodeHex;
			ActionBlobViewHEX->Checked = true;
			ActionBlobViewTextWrap->Visible = false;
			break;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionBlobViewTextWrapExecute(TObject *Sender)
{
	ATViewerBlob->TextWrap = ActionBlobViewTextWrap->Checked;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionBlobParseModeDetectExecute(TObject *Sender)
{
	ActionBlobParseUnzip->Enabled = !ActionBlobParseModeDetect->Checked;
	ActionBlobParseUnzip2->Enabled = !ActionBlobParseModeDetect->Checked;
	ActionBlobParseContainer->Enabled = !ActionBlobParseModeDetect->Checked;
	ActionBlobParseValueStorage->Enabled = !ActionBlobParseModeDetect->Checked;
	ActionBlobParseValue1C->Enabled = !ActionBlobParseModeDetect->Checked;
	ActionBlobParseUsersUsr->Enabled = !ActionBlobParseModeDetect->Checked;
	ParseBlob();
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionBlobParseUnzipExecute(TObject *Sender)
{
	if(!ActionBlobParseUnzip->Checked) ActionBlobParseUnzip2->Checked = false;
	ParseBlob();
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionBlobParseContainerExecute(TObject *Sender)
{
	ParseBlob();
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionBlobParseUnzip2Execute(TObject *Sender)
{
	if(ActionBlobParseUnzip2->Checked) ActionBlobParseUnzip->Checked = true;
	ParseBlob();
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::TreeViewContainerChange(TObject *Sender, TTreeNode *Node)
{
	if(!Node) return;
	if(!cat) return;

	if(Node->Data)
	{
		temp_stream->Size = 0;
		((v8file*)Node->Data)->SaveToStream(temp_stream);
		setBlobViewText(false);
	}
	else
	{
		ATViewerBlob->Open(L"");
		ATViewerBlob->OpenStream(NULL, vmodeText);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TreeViewContainerExpanded(TObject *Sender, TTreeNode *Node)
{
	Node->ImageIndex = 11;
	Node->SelectedIndex = 11;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TreeViewContainerCollapsed(TObject *Sender, TTreeNode *Node)
{
	Node->ImageIndex = 10;
	Node->SelectedIndex = 10;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ActionSaveConfigExecute(TObject *Sender)
{
	if(!base1CD->is_open()) return;
	SaveDialogCF->FileName = L"dbcf.cf";
	if(SaveDialogCF->Execute())
	{
		if(base1CD->save_config(SaveDialogCF->FileName))
			mess->AddMessage_(L"Сохранение конфигурации базы данных завершено.", msSuccesfull,
				L"Файл", SaveDialogCF->FileName);
		else
			mess->AddMessage_(L"Не удалось сохранить конфигурацию базы данных.", msError,
				L"Файл", SaveDialogCF->FileName);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionSaveConfigSaveExecute(TObject *Sender)
{
	if(!base1CD->is_open()) return;
	SaveDialogCF->FileName = L"cf.cf";
	if(SaveDialogCF->Execute())
	{
		if(base1CD->save_configsave(SaveDialogCF->FileName))
			mess->AddMessage_(L"Сохранение основной конфигурации завершено.", msSuccesfull,
				L"Файл", SaveDialogCF->FileName);
		else
			mess->AddMessage_(L"Не удалось сохранить основную конфигурацию.", msError,
				L"Файл", SaveDialogCF->FileName);

	}
}

//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionBlobParseValueStorageExecute(TObject *Sender)
{
	if(!ActionBlobParseValueStorage->Checked) ActionBlobParseValue1C->Checked = false;
	ParseBlob();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionBlobParseValue1CExecute(TObject *Sender)
{
	if(ActionBlobParseValue1C->Checked) ActionBlobParseValueStorage->Checked = true;
	ParseBlob();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TimerGarbageMemoryTimer(TObject *Sender)
{
	tr_syn->BeginWrite();
	v8object::garbage();
	memblock::garbage();
	tr_syn->EndWrite();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::StringGridPhysicalViewGetAlignment(TObject *Sender, int ARow,
		  int ACol, TAlignment &HAlign, Advobj::TVAlignment &VAlign)
{
	VAlign = vtaTop;
	if(ARow == 0) HAlign = taCenter;
	else if(tab)
	{
		if(ACol)
		{
			if(tab->getfield(ACol - 1)->gettype() == tf_numeric) HAlign = taRightJustify;
			else HAlign = taLeftJustify;
		}
		else HAlign = taRightJustify;
	}
	else HAlign = taLeftJustify;

}

//---------------------------------------------------------------------------
void __fastcall TFormMain::StringGridPhysicalViewGetDisplText(TObject *Sender, int ACol,
		  int ARow, UnicodeString &Value)
{
	Value = PhysViewGetDisplText(ACol, ARow, false, 160);

}

//---------------------------------------------------------------------------
void __fastcall TFormMain::PanelMessagesResize(TObject *Sender)
{
	StatusBar1->Top = PanelMessages->Top + PanelMessages->Height + 1;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionFindSupplierConfigsExecute(TObject *Sender)
{
	if(!base1CD->is_open()) return;
	base1CD->find_supplier_configs();
	int numc = base1CD->supplier_configs.size();
	StringGridSupplierConfigs->RowCount = numc + 1;
	if(numc)
	{
		mess->AddMessage_(L"Найдены конфигурации поставщика", msSuccesfull,
			L"Количество", numc);

		for(int i = 0; i < numc; i++)
		{
			SupplierConfig& sc = base1CD->supplier_configs[i];
			StringGridSupplierConfigs->Cells[0][i + 1] = sc.name;
			StringGridSupplierConfigs->Cells[1][i + 1] = sc.version;
			StringGridSupplierConfigs->Cells[2][i + 1] = sc.supplier;
		}
	}
	else mess->AddMessage(L"Конфигурации поставщика не найдены", msWarning);

	ActionSaveSupplierConfig->Visible = numc;
	StringGridSupplierConfigs->AutoSize = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionSaveSupplierConfigExecute(TObject *Sender)
{
	if(!base1CD->is_open()) return;
	int numcon = StringGridSupplierConfigs->Selection.Top - 1;
	if(numcon < 0) return;

	SaveDialogCF->FileName = base1CD->supplier_configs[numcon].name + L" " + base1CD->supplier_configs[numcon].version + L".cf";
	if(SaveDialogCF->Execute())
	{
		if(base1CD->save_supplier_configs(numcon, SaveDialogCF->FileName))
			mess->AddMessage_(L"Сохранение конфигурации поставщика завершено.", msSuccesfull,
				L"Файл", SaveDialogCF->FileName);
		else
			mess->AddMessage_(L"Не удалось сохранить конфигурацию поставщика.", msError,
				L"Файл", SaveDialogCF->FileName);

	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionSaveDataFileExecute(TObject *Sender)
{
	if(!base1CD->is_open()) return;
	if(!tab) return;
	if(!tab->get_file_data()) return;
	SaveDialogFile->Title = L"Сохранение файла Records таблицы";
	SaveDialogFile->FileName = tab->getname() + L"_Records";
	if(SaveDialogFile->Execute())
	{
		tab->get_file_data()->savetofile(SaveDialogFile->FileName);
		mess->AddMessage_(L"Сохранен файл Records.", msSuccesfull,
			L"Таблица", tab->getname(),
			L"Файл", SaveDialogFile->FileName);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionSaveBlobFileExecute(TObject *Sender)
{
	if(!base1CD->is_open()) return;
	if(!tab) return;
	if(!tab->get_file_blob()) return;
	SaveDialogFile->Title = L"Сохранение файла Blob таблицы";
	SaveDialogFile->FileName = tab->getname() + L"_Blob";
	if(SaveDialogFile->Execute())
	{
		tab->get_file_blob()->savetofile(SaveDialogFile->FileName);
		mess->AddMessage_(L"Сохранен файл Blob.", msSuccesfull,
			L"Таблица", tab->getname(),
			L"Файл", SaveDialogFile->FileName);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionSaveIndexFileExecute(TObject *Sender)
{
	if(!base1CD->is_open()) return;
	if(!tab) return;
	if(!tab->get_file_index()) return;
	SaveDialogFile->Title = L"Сохранение файла _Index таблицы";
	SaveDialogFile->FileName = tab->getname() + L"_Index";
	if(SaveDialogFile->Execute())
	{
		tab->get_file_index()->savetofile(SaveDialogFile->FileName);
		mess->AddMessage_(L"Сохранен файл Index.", msSuccesfull,
			L"Таблица", tab->getname(),
			L"Файл", SaveDialogFile->FileName);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxCurrentIndexChange(TObject *Sender)
{
	if(tr_thread)
	{
		//tr_thread->Terminate();
		tr_thread->WaitFor();
		delete tr_thread;
		tr_thread = NULL;
	}

	if(ComboBoxCurrentIndex->ItemIndex) cur_index = tab->getindex(ComboBoxCurrentIndex->ItemIndex - 1);
	else cur_index = NULL;

	sorted_column = 0;
	if(cur_index)
	{
		field* curfield = cur_index->get_records()->field;
		for(; sorted_column < tab->get_numfields(); sorted_column++)
		{
			if(tab->getfield(sorted_column) == curfield) break;
		}
		sorted_column++;
	}

	reverse_sort = false;
	ReSort();
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionDumpIndexExecute(TObject *Sender)
{
#ifndef PublicRelease
	if(!base1CD->is_open()) return;
	if(!tab) return;
	if(!tab->get_file_index()) return;

	TTreeNode* node = TreeViewDescriptionIndexes->Selected;
	if(!node) return;
	while(node->Level) node = node->Parent;
	index* ind = tab->getindex(node->Index);

	SaveDialogFile->Title = L"Сохранение дампа индекса таблицы";
	SaveDialogFile->FileName = tab->getname() + L" " + ind->getname() + L" Dump";
	if(SaveDialogFile->Execute())
	{
		ind->dump(SaveDialogFile->FileName);
		mess->AddMessage_(L"Сохранен дамп индекса.", msSuccesfull,
			L"Таблица", tab->getname(),
			L"Индекс", ind->getname(),
			L"Файл", SaveDialogFile->FileName);
	}
#endif //#ifdef PublicRelease
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionExportTableToXMLExecute(TObject *Sender)
{

	if(!tab) return;
	if(cur_index) cur_index->get_numrecords();
	else if(!tab->recordsindex_complete) tr_thread->WaitFor();

	SaveDialogXML->FileName = tab->getname() + L".xml";
	if(SaveDialogXML->Execute())
	{
		tab->export_to_xml(SaveDialogXML->FileName, ActionXMLSaveBLOBToFile->Checked, ActionXMLUnpackBLOB->Checked);
		mess->AddMessage_(L"Выполнен экспорт таблицы в файл.", msSuccesfull,
			L"Таблица", tab->getname(),
			L"Файл", SaveDialogXML->FileName);
	}


}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionAboutExecute(TObject *Sender)
{
	AboutBox->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionViewFileOffsetExecute(TObject *Sender)
{
	StringGridPhysicalView->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionShowGUIDsExecute(TObject *Sender)
{
	field::showGUID = ActionShowGUIDs->Checked;
	StringGridPhysicalView->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionOpenBaseNotMonopolyExecute(TObject *Sender)
{
	return;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TabSheetPhysicalViewResize(TObject *Sender)
{
//	if(SplitterBlobView->Visible)
//	{
////		StringGridPhysicalView->Height = (TabSheetPhysicalView->Height - StringGridPhysicalView->Top) / 2;
//		PanelBlobView->Height = TabSheetPhysicalView->Height - (StringGridPhysicalView->Top + StringGridPhysicalView->Height);
//		SplitterBlobView->Top = SplitterBlobView->Top - 1;
//		SplitterBlobViewProperties->Left = PanelBlobViewProperties->Left - 1;
//	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionTestIndexesExecute(TObject *Sender)
{
#ifndef PublicRelease
	String s;
	int i, j, m;
	unsigned int ni, k;
	unsigned int numrec;
	table* t;
	index* ind;
	field* fld;
	unsigned __int64 curblock;
	unsigned int index_len;
	v8object* file_index;
	branch_page_header* branch_page;
	leaf_page_header* leaf_page;
	char* unpack_page;
	char* indrec;
	char* indrec_calc;
	char* rec;

	char* buf;
	bool has_error = false;
	db_ver version;
	unsigned int pagesize;

	if(!base1CD->is_open()) return;
	version = base1CD->getversion();
	pagesize = base1CD->getpagesize();
	buf = new char[pagesize];

	SaveDialogFile->FileName = L"Index_test.txt";
	if(!SaveDialogFile->Execute()) return;

	TFileStream* f = new TFileStream(SaveDialogFile->FileName, fmCreate);
	f->Write("\xff\xfe", 2);

	for(i = 0; i < base1CD->get_numtables() ; i++)
	{
		t = base1CD->gettable(i);

		mess->Status(t->getname());

		s = L"TABLE ";
		s += t->getname();
		s += L"\r\n";

		f->Write(s.c_str(), s.Length() * s.ElementSize());

		rec = new char[t->get_recordlen()];

		file_index = t->get_file_index();

		for(j = 0; j < t->get_numindexes(); j++)
		{
			ind = t->getindex(j);
			s = L"  INDEX ";
			s += ind->getname();
			s += L"\r\n";

			f->Write(s.c_str(), s.Length() * s.ElementSize());

			curblock = ind->get_rootblock();
			if(!curblock) continue;
			index_len = ind->get_length();

			indrec_calc = new char[index_len];

			while(true)
			{
				file_index->getdata(buf, curblock, pagesize);
				branch_page = (branch_page_header*)buf;
				if(branch_page->flags & indexpage_is_leaf) break;
				curblock = reverse_byte_order(*(unsigned int*)(buf + index_len + 16));
				if(version >= ver8_3_8_0) curblock *= pagesize;
			}

			while(true)
			{
				unpack_page = ind->unpack_leafpage(buf, ni);
				if(unpack_page)
				{
					indrec = unpack_page;
					for(k = 0; k < ni; k++)
					{
						numrec = *(unsigned int*)indrec;
						indrec += 4;
						t->getrecord(numrec, rec);

						ind->calcRecordIndex(rec, indrec_calc);

						if(memcmp(indrec, indrec_calc, index_len) != 0)
						{
							has_error = true;

							for(m = 0; m < ind->get_num_records(); m++)
							{
								fld = ind->get_records()[m].field;
								s = L"    ";
								s += fld->getname();
								s += L" : ";
								s += hexstring(&rec[fld->getoffset()], fld->getlen());
								s += L" : (";
								s += fld->get_presentation(rec);
								s += L")\r\n";
								f->Write(s.c_str(), s.Length() * s.ElementSize());
							}
							s = L"    - Index1 : ";
							s += hexstring(indrec, index_len);
							s += L"\r\n";
							f->Write(s.c_str(), s.Length() * s.ElementSize());

							s = L"    - Index2 : ";
							s += hexstring(indrec_calc, index_len);
							s += L"\r\n\r\n";
							f->Write(s.c_str(), s.Length() * s.ElementSize());
						}

						indrec += index_len;
					}

					delete[] unpack_page;
				}


				leaf_page = (leaf_page_header*)buf;
				curblock = leaf_page->next_page;
				if(curblock == 0xffffffff) break;
				if(version >= ver8_3_8_0) curblock *= pagesize;
				file_index->getdata(buf, curblock, pagesize);
			}

			delete[] indrec_calc;

		}

		delete[] rec;
	}

	delete f;
	delete[] buf;
	mess->AddMessage_(L"Сохранен файл теста индексов.", msSuccesfull,
		L"Файл", SaveDialogFile->FileName,
		L"Ошибки", has_error ? L"Есть" : L"Нет");
#endif //#ifdef PublicRelease
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::StringGridPhysicalViewCanEditCell(TObject *Sender, int ARow, int ACol, bool &CanEdit)
{
	//unsigned int numrec;
	type_fields tf;

	CanEdit = false;

	if(!tab) return;
	if(!ARow) return;
	if(!ACol) return;
	if(base1CD->get_readonly()) return;
	if(!tab->get_edit()) return;

	tf = tab->getfield(ACol - 1)->gettype();
	if(tf == tf_version || tf == tf_version8) return;

	CanEdit = true;
	return;

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::StringGridPhysicalViewGetEditorType(TObject *Sender, int ACol, int ARow, TEditorType &AEditor)
{
#ifndef PublicRelease
	field* fld;
	type_fields tf;
	String s;
	int i;

	AEditor = edNone;
	curedit = NULL;

	if(!tab) return;
	if(!ARow) return;
	if(!ACol) return;
	if(base1CD->get_readonly()) return;

	fld = tab->getfield(ACol - 1);
	tf = fld->gettype();

	edit_clear = false;
	edit_null = false;
	edit_restore = false;

	edit_phys_numrecord = get_phys_numrec(ARow);
	if(tab->get_rec_type(edit_phys_numrecord) == crt_delete) return;

	switch(tf)
	{
		case tf_binary:
			AEditor = edCustom;
			StringGridPhysicalView->EditLink = FormControlEditLinkMasked;
			curedit = MaskEditCommonEdit;
			if(fld->getlength() == 16 && field::showGUID)
			{
				MaskEditCommonEdit->MaxLength = 36;
				MaskEditCommonEdit->EditMask = L"AAAAAAAA-AAAA-AAAA-AAAA-AAAAAAAAAAAA;1; ";
			}
			else
			{
				MaskEditCommonEdit->MaxLength = fld->getlength() * 2;
				s = L"";
				for(i = 0; i < fld->getlength(); i++) s += L"AA";
				s += L";1; ";
				MaskEditCommonEdit->EditMask = s;
			}
			curedit->Text = PhysViewGetDisplText(ACol, ARow, true);
			edit_clear = true;
			edit_null = fld->getnull_exists();
			edit_restore = edit_phys_numrecord < tab->get_phys_numrecords();
			curedit->Alignment = taLeftJustify;
			break;
		case tf_bool:
			AEditor = edCustom;
			StringGridPhysicalView->EditLink = FormControlEditLinkCommon;
			curedit = ButtonedEditCommonEdit;
			ButtonedEditCommonEdit->MaxLength = 5;
			curedit->Text = PhysViewGetDisplText(ACol, ARow, true);
			edit_clear = true;
			edit_null = fld->getnull_exists();
			edit_restore = edit_phys_numrecord < tab->get_phys_numrecords();
			curedit->Alignment = taLeftJustify;
			break;
		case tf_char:
			AEditor = edCustom;
			StringGridPhysicalView->EditLink = FormControlEditLinkCommon;
			curedit = ButtonedEditCommonEdit;
			ButtonedEditCommonEdit->MaxLength = fld->getlength();
			curedit->Text = PhysViewGetDisplText(ACol, ARow, true).TrimRight();
			edit_clear = true;
			edit_null = fld->getnull_exists();
			edit_restore = edit_phys_numrecord < tab->get_phys_numrecords();
			curedit->Alignment = taLeftJustify;
			break;
		case tf_datetime:
			AEditor = edCustom;
			StringGridPhysicalView->EditLink = FormControlEditLinkMasked;
			curedit = MaskEditCommonEdit;
			MaskEditCommonEdit->MaxLength = 19; // DD.MM.YYYY hh:mm:ss
			MaskEditCommonEdit->EditMask = L"00.00.0000 00:00:00;1; ";
			curedit->Text = PhysViewGetDisplText(ACol, ARow, true);
			edit_clear = true;
			edit_null = fld->getnull_exists();
			edit_restore = edit_phys_numrecord < tab->get_phys_numrecords();
			curedit->Alignment = taLeftJustify;
			break;
		case tf_image:
			AEditor = edNone;
			return;
//		case tf_numeric:
//			AEditor = edCustom;
//			StringGridPhysicalView->EditLink = FormControlEditLinkMasked;
//			curedit = MaskEditCommonEdit;
//			s = L"!#";
//			for(i = 0; i < fld->getlength() - fld->getprecision(); i++) s += L"9";
//			if(fld->getprecision())
//			{
//				s += L".";
//				for(; i < fld->getlength(); i++) s += L"9";
//				MaskEditCommonEdit->MaxLength = fld->getlength() + 2; // доп. на точку и знак
//			}
//			else
//			{
//				MaskEditCommonEdit->MaxLength = fld->getlength() + 1; // доп. на знак
//			}
//			s += L";1; ";
//			MaskEditCommonEdit->EditMask = s;
//			s = PhysViewGetDisplText(ACol, ARow, true);
//			while(s.Length() < MaskEditCommonEdit->MaxLength) s += L" ";
//			curedit->Text = s;
//			edit_clear = true;
//			edit_null = fld->getnull_exists();
//			edit_restore = edit_phys_numrecord < tab->get_phys_numrecords();
//			curedit->Alignment = taRightJustify;
//			break;
		case tf_numeric:
			AEditor = edCustom;
			StringGridPhysicalView->EditLink = FormControlEditLinkCommon;
			curedit = ButtonedEditCommonEdit;
			ButtonedEditCommonEdit->MaxLength = fld->getlength() + 2; // доп. на точку и знак
			curedit->Text = PhysViewGetDisplText(ACol, ARow, true);
			edit_clear = true;
			edit_null = fld->getnull_exists();
			edit_restore = edit_phys_numrecord < tab->get_phys_numrecords();
			curedit->Alignment = taRightJustify;
			break;
		case tf_string:
			AEditor = edNone;
			return;
		case tf_text:
			AEditor = edNone;
			return;
		case tf_varbinary:
			AEditor = edNone;
			return;
		case tf_varchar:
			AEditor = edCustom;
			StringGridPhysicalView->EditLink = FormControlEditLinkCommon;
			curedit = ButtonedEditCommonEdit;
			curedit->Text = PhysViewGetDisplText(ACol, ARow, true);
			ButtonedEditCommonEdit->MaxLength = fld->getlength();
			edit_clear = true;
			edit_null = fld->getnull_exists();
			edit_restore = edit_phys_numrecord < tab->get_phys_numrecords();
			curedit->Alignment = taLeftJustify;
			break;
		case tf_version:
			AEditor = edNone;
			return;
		case tf_version8:
			AEditor = edNone;
			return;
	}

	AdvToolButtonStringNULL->Visible = edit_null;
	AdvToolButtonStringClear->Visible = edit_clear;
	AdvToolButtonStringRestore->Visible = edit_restore;
	ActionStringEditDropDown->Checked = false;

	edit_numfield = ACol - 1;
	edit_tab = true;
	edit_ARow = ARow;

#endif //#ifdef PublicRelease

}

//---------------------------------------------------------------------------
String __fastcall TFormMain::PhysViewGetDisplText(int ACol, int ARow, bool EmptyNull, wchar_t Delimiter, bool detailed)
{
	unsigned int numrec;
	changed_rec_type rectype;
	field* fld;

	if(!tab) return L"";
	if(ARow)
	{
		if(ACol)
		{
			numrec = get_phys_numrec(ARow);

			tab->get_edit_record(numrec, tabrec);
			fld = tab->getfield(ACol - 1);
			return fld->get_presentation(tabrec, EmptyNull, Delimiter, false, detailed);
		}
		else if(ActionViewFileOffset->Checked)
		{
			if((unsigned int)ARow > tab->get_log_numrecords()) return L"-";
			numrec = get_phys_numrec(ARow);
			return tohex64(tab->get_fileoffset(numrec));
		}
		else return ARow;
	}
	else
	{
		if(ACol) return tab->getfield(ACol - 1)->getname();
		else if(ActionViewFileOffset->Checked) return L"file offset";
		else return L"№";
	}

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ListViewTablesChanging(TObject *Sender, TListItem *Item, TItemChange Change, bool &AllowChange)
{
	if(Change == TItemChange::ctState) if(tab) if(tab->get_edit()) AllowChange = false;
	//if(tab) if(tab->get_edit()) AllowChange = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionEditTableExecute(TObject *Sender)
{
#ifndef PublicRelease
	bool tmp;

	if(!base1CD) return;
	if(base1CD->get_readonly()) return;
	if(!tab) return;
	if(PageControlTableView->ActivePage != TabSheetPhysicalView) return;
	if(!cur_index) tr_thread->WaitFor();
	tab->begin_edit();

	SetEditTableVisible();

	ListViewTables->Enabled = false;
	TabSheetUtilities->TabVisible = false;

	StringGridPhysicalViewSelectCell(Sender, StringGridPhysicalView->Selection.Left, StringGridPhysicalView->Selection.Top, tmp);

#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::SetEditTableVisible()
{
	bool visible_edit = false;
	bool visible_end_edit = false;
	bool visible_XML = false;

	if(base1CD)
	{
		if(!base1CD->get_readonly())
		{
			if(tab)
			{
				visible_end_edit = tab->get_edit();
				visible_edit = !visible_end_edit;
				visible_XML = visible_edit;
			}
		}
		else visible_XML = true;
	}

	ActionEditTable->Visible = visible_edit;
	ActionSaveTableChanges->Visible = visible_end_edit;
	ActionDiscardTableChanges->Visible = visible_end_edit;
	ToolButtonEditSeparator->Visible = visible_end_edit || visible_edit;

	ActionExportTableToXML->Visible = visible_XML;
	ToolButtonExportXMLSeparator->Visible = visible_XML;

	ActionEditTableInsert->Visible = visible_end_edit;
	ActionEditTableDelete->Visible = visible_end_edit;
	ToolButtonInsDelSeparator->Visible = visible_end_edit;

	if(visible_end_edit) StringGridPhysicalView->Options << goEditing;
	else StringGridPhysicalView->Options >> goEditing;
	//StringGridPhysicalView->Repaint();

}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionExportTableExecute(TObject *Sender)
{
#ifndef PublicRelease
	if(tab)
	{
		tab->export_table(DirectoryEditExportImportTable->Text);
		mess->AddMessage_(L"Экспортирована таблица", msSuccesfull,
			L"Таблица", tab->getname(),
			L"Путь экспорта", DirectoryEditExportImportTable->Text);
	}
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionImportTableExecute(TObject *Sender)
{
#ifndef PublicRelease
	if(tab)
	{
		tab->import_table(DirectoryEditExportImportTable->Text);
		mess->AddMessage_(L"Импортирована таблица", msSuccesfull,
			L"Таблица", tab->getname(),
			L"Путь экспорта", DirectoryEditExportImportTable->Text);
	}
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionExportDataTablesExecute(TObject *Sender)
{
#ifndef PublicRelease
	table* t;
	String s;
	int n;

	if(base1CD)
	{
		n = base1CD->get_numtables();
		for(int i = 0; i < n; i++)
		{
			t = base1CD->gettable(i);
			s = t->getname();
			s += L" - ";
			s += i;
			s += L" из ";
			s += n;
			mess->Status(s);

			//if(!t->get_issystem()) t->export_table(DirectoryEditExportImportTable->Text);
			t->export_table(DirectoryEditExportImportTable->Text);
		}

		mess->AddMessage_(L"Экспортированы таблицы данных", msSuccesfull,
			L"Путь экспорта", DirectoryEditExportImportTable->Text);
	}
	mess->Status(L"");
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionImportDataTablesExecute(TObject *Sender)
{
#ifndef PublicRelease
	table* t;
	String s;
	int n;

	if(base1CD)
	{
		n = base1CD->get_numtables();
		for(int i = 0; i < n; i++)
		{
			t = base1CD->gettable(i);
			s = t->getname();
			s += L" - ";
			s += i;
			s += L" из ";
			s += n;
			mess->Status(s);

			if(!t->get_issystem()) t->import_table(DirectoryEditExportImportTable->Text);
		}

		mess->AddMessage_(L"Импортированы таблицы данных", msSuccesfull,
			L"Путь экспорта", DirectoryEditExportImportTable->Text);
	}
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::FormControlEditLinkCommonGetEditorValue(TObject *Sender, TAdvStringGrid *Grid, UnicodeString &AValue)
{
#ifndef PublicRelease
	TGridRect coord;

	if(edit_tab)
	{
		tab->set_edit_value(edit_phys_numrecord, edit_numfield, false, curedit->Text);
		edit_tab = false;
		coord = StringGridPhysicalView->Selection;
		coord.Left = 0;
		coord.Right = 0;
		StringGridPhysicalView->InvalidateGridRect(coord);
	}
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::FormControlEditLinkCommonSetEditorFocus(TObject *Sender, TAdvStringGrid *Grid, TWinControl *AControl)
{
	curedit->SetFocus();
	curedit->SelectAll();
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::StringGridPhysicalViewGetCellColor(TObject *Sender, int ARow, int ACol, TGridDrawState AState,
		  TBrush *ABrush, TFont *AFont)
{
#ifndef PublicRelease
	unsigned int numrec;
	changed_rec_type rectype = crt_not_changed;
	field* fld;

	if(!tab) return;

	if(ARow)
	{
		numrec = get_phys_numrec(ARow);
		if(ACol) rectype = tab->get_rec_type(numrec, ACol - 1);
		else rectype = tab->get_rec_type(numrec);
	}

	switch(rectype)
	{
		case crt_not_changed:
//			AFont->Color = clBlack;
			break;
		case crt_changed:
			ABrush->Color = (TColor)0xEEEEAF; // Бледно-синий
			break;
		case crt_delete:
			ABrush->Color = (TColor)0xDCD1FF; // Пастельно-розовый
			break;
		case crt_insert:
			ABrush->Color = (TColor)0xD1FFD3; // Бело-зеленый
			break;
	}
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::StringGridPhysicalViewCanInsertRow(TObject *Sender, int ARow, bool &CanInsert)
{
	CanInsert = false;
	ActionEditTableInsert->Execute();
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::StringGridPhysicalViewCanDeleteRow(TObject *Sender, int ARow, bool &CanDelete)
{
//	mess->AddMessage_(L"Удаление", msSuccesfull,
//		L"Строка", ARow);
	//ActionEditTableDelete->Execute();
	CanDelete = false;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionEditTableInsertExecute(TObject *Sender)
{
#ifndef PublicRelease
	if(tab) if(tab->get_edit())
	{
		tab->set_rec_type(tab->get_phys_numrecords() + tab->get_added_numrecords(), crt_insert);
		StringGridPhysicalView->RowCount++;
		//StringGridPhysicalView->SelectCells(1, StringGridPhysicalView->RowCount - 1, 1, StringGridPhysicalView->RowCount - 1);
		StringGridPhysicalView->ScrollInView(1, StringGridPhysicalView->RowCount - 1);
	}
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionEditTableDeleteExecute(TObject *Sender)
{
#ifndef PublicRelease
	int ARow;
	TGridRect coord;
	unsigned int phys_numrecord;

	if(tab) if(tab->get_edit())
	{
		ARow = StringGridPhysicalView->Selection.Top;
		if(!ARow) return;
		phys_numrecord = get_phys_numrec(ARow);

		if(tab->get_rec_type(phys_numrecord) == crt_delete)
		{
			tab->set_rec_type(phys_numrecord, crt_not_changed);
//			coord.Left = 0;
//			coord.Right = StringGridPhysicalView->ColCount - 1;
//			coord.Top = ARow;
//			coord.Bottom = ARow;
//			StringGridPhysicalView->InvalidateGridRect(coord);
			StringGridPhysicalView->RepaintRow(ARow);
		}
		else
		{
			tab->set_rec_type(phys_numrecord, crt_delete);
			if((unsigned int)ARow > tab->get_log_numrecords())
			{
				StringGridPhysicalView->RowCount--;
				if(ARow < StringGridPhysicalView->RowCount)
				{
					coord.Left = 0;
					coord.Right = StringGridPhysicalView->ColCount - 1;
					coord.Top = ARow;
					coord.Bottom = StringGridPhysicalView->RowCount - 1;
					StringGridPhysicalView->InvalidateGridRect(coord);
				}
			}
			else
			{
//				coord.Left = 0;
//				coord.Right = StringGridPhysicalView->ColCount - 1;
//				coord.Top = ARow;
//				coord.Bottom = ARow;
//				StringGridPhysicalView->InvalidateGridRect(coord);
				StringGridPhysicalView->RepaintRow(ARow);
			}
		}
	}
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionSaveTableChangesExecute(TObject *Sender)
{
#ifndef PublicRelease
	TGridRect coord;
	int ARow;
	int ACol;
	bool tmp;

	if(tab) if(tab->get_edit())
	{
		tab->end_edit();
		StringGridPhysicalView->RowCount = tab->get_log_numrecords() + 1;
		SetEditTableVisible();
		StringGridPhysicalView->Invalidate();
	}
	ListViewTables->Enabled = true;
	TabSheetUtilities->TabVisible = true;

	coord = StringGridPhysicalView->Selection;
	ACol = coord.Left;
	ARow = coord.Top;
	if(!ARow || !ACol) return;
	StringGridPhysicalViewSelectCell(Sender, ACol, ARow, tmp);
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionDiscardTableChangesExecute(TObject *Sender)
{
#ifndef PublicRelease
	TGridRect coord;
	int ARow;
	int ACol;
	bool tmp;

	if(tab) if(tab->get_edit())
	{
		tab->cancel_edit();
		StringGridPhysicalView->RowCount = tab->get_log_numrecords() + 1;
		SetEditTableVisible();
		StringGridPhysicalView->Invalidate();
	}
	ListViewTables->Enabled = true;
	TabSheetUtilities->TabVisible = true;

	coord = StringGridPhysicalView->Selection;
	ACol = coord.Left;
	ARow = coord.Top;
	if(!ARow || !ACol) return;
	StringGridPhysicalViewSelectCell(Sender, ACol, ARow, tmp);
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::AdvStringGridStatisticGetFormat(TObject *Sender, int ACol, TSortStyle &AStyle, UnicodeString &aPrefix, UnicodeString &aSuffix)
{
	switch(ACol)
	{
		case 0:
		case 1:
			AStyle = ssAlphaNoCase;
			break;
		case 2:
		case 3:
			AStyle = ssNumeric;
			break;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::AdvStringGridStatisticCanSort(TObject *Sender, int ACol, bool &DoSort)
{
	DoSort = true;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::AdvStringGridStatisticGetAlignment(TObject *Sender, int ARow, int ACol, TAlignment &HAlign, Advobj::TVAlignment &VAlign)
{
	if(ARow)
	{
		switch(ACol)
		{
			case 0:
			case 1:
				HAlign = taLeftJustify;
				break;
			case 2:
			case 3:
				HAlign = taRightJustify;
				break;
		}
	}
	else HAlign = taCenter;
	VAlign = vtaCenter;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::AdvStringGridStatisticGridHint(TObject *Sender, int ARow, int ACol, UnicodeString &hintstr)
{
	if(ARow == 0) switch(ACol)
	{
		case 0:
			hintstr = L"Имя таблицы базы данных";
			break;
		case 1:
			hintstr = L"Вид файла таблицы";
			break;
		case 2:
			hintstr = L"Размер файла в байтах";
			break;
		case 3:
			hintstr = L"Размер файла в процентах от максимально возможного";
			break;
	}
	else hintstr = L"";

}

//---------------------------------------------------------------------------
void __fastcall TFormMain::PopupEditMenu(int x, int y)
{
	if(PanelEditDropDown->Visible) PanelEditDropDown->Visible = false;
	else
	{
		PanelEditDropDown->Width =
			(edit_clear ? AdvToolButtonStringClear->Width : 0) +
			(edit_null ? AdvToolButtonStringNULL->Width : 0) +
			(edit_restore ? AdvToolButtonStringRestore->Width : 0);
		if(x == -1)
		{
			PanelEditDropDown->Top = curedit->Top + curedit->Height;
			PanelEditDropDown->Left = curedit->Left + curedit->Width - PanelEditDropDown->Width;
		}
		else
		{
			PanelEditDropDown->Top = curedit->Top + y;
			PanelEditDropDown->Left = curedit->Left + x;
		}
		PanelEditDropDown->Visible = true;
	}

}

//---------------------------------------------------------------------------
void __fastcall TFormMain::StringGridPhysicalViewEditCellDone(TObject *Sender, int ACol, int ARow)
{
	PanelEditDropDown->Visible = false;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionNULLExecute(TObject *Sender)
{
#ifndef PublicRelease
	TGridRect coord;

	if(!tab) return;
	if(!tab->get_edit()) return;

	coord = StringGridPhysicalView->Selection;
	if(edit_tab)
	{
		tab->set_edit_value(edit_phys_numrecord, edit_numfield, true, L"");
		edit_tab = false;
		StringGridPhysicalView->SetFocus();
	}
	else
	{
		tab->set_edit_value(get_phys_numrec(coord.Top), coord.Left - 1, true, L"");
		StringGridPhysicalView->InvalidateGridRect(coord);
	}
	coord.Left = 0;
	coord.Right = 0;
	StringGridPhysicalView->InvalidateGridRect(coord);
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionRestoreExecute(TObject *Sender)
{
#ifndef PublicRelease
	TGridRect coord;

	if(!tab) return;
	if(!tab->get_edit()) return;

	coord = StringGridPhysicalView->Selection;
	if(edit_tab)
	{
		tab->restore_edit_value(edit_phys_numrecord, edit_numfield);
		edit_tab = false;
		StringGridPhysicalView->SetFocus();
	}
	else
	{
		tab->restore_edit_value(get_phys_numrec(coord.Top), coord.Left - 1);
		StringGridPhysicalView->InvalidateGridRect(coord);
	}
	coord.Left = 0;
	coord.Right = 0;
	StringGridPhysicalView->InvalidateGridRect(coord);
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionClearExecute(TObject *Sender)
{
#ifndef PublicRelease
	TGridRect coord;
	unsigned int phys_numrec;

	if(!tab) return;
	if(!tab->get_edit()) return;

	coord = StringGridPhysicalView->Selection;
	if(edit_tab)
	{
		tab->set_edit_value(edit_phys_numrecord, edit_numfield, false, L"");
		edit_tab = false;
		StringGridPhysicalView->SetFocus();
	}
	else
	{
		phys_numrec = get_phys_numrec(coord.Top);
		if(tab->get_rec_type(phys_numrec) == crt_delete) return;
		tab->set_edit_value(phys_numrec, coord.Left - 1, false, L"");
		StringGridPhysicalView->InvalidateGridRect(coord);
	}
	coord.Left = 0;
	coord.Right = 0;
	StringGridPhysicalView->InvalidateGridRect(coord);
#endif //#ifdef PublicRelease
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonedEditCommonEditKeyPress(TObject *Sender, wchar_t &Key)
{
	field* f;
	type_fields tf;
	int l, i, j, s, sl;
	int lm, l1, lp, l2; // длина минуса, знаков до точки, точки и знаков после точки
	int length, precision;
	String str;
	bool m;

	if(!tab) return;
	if(!tab->get_edit()) return;
	if(!edit_tab) return;

	f = tab->getfield(edit_numfield);
	tf = f->gettype();

	str = ButtonedEditCommonEdit->Text;
	l = str.Length();
	s = ButtonedEditCommonEdit->SelStart;
	sl = ButtonedEditCommonEdit->SelLength;

	switch(tf)
	{
		case tf_binary:
			switch(Key)
			{
				case L'A':
				case L'Ф':
				case L'ф':
					Key = L'a';
					break;
				case L'B':
				case L'И':
				case L'и':
					Key = L'b';
					break;
				case L'C':
				case L'С':
				case L'с':
					Key = L'c';
					break;
				case L'D':
				case L'В':
				case L'в':
					Key = L'd';
					break;
				case L'E':
				case L'У':
				case L'у':
					Key = L'e';
					break;
				case L'F':
				case L'А':
				case L'а':
					Key = L'f';
					break;
				case L'0':
				case L'1':
				case L'2':
				case L'3':
				case L'4':
				case L'5':
				case L'6':
				case L'7':
				case L'8':
				case L'9':
					break;
				default:
					Key = 0;
			}
			return;
		case tf_bool:
			switch(Key)
			{
				case L'0':
				case L'f':
				case L'F':
				case L'n':
				case L'N':
				case L'а':
				case L'А':
				case L'т':
				case L'Т':
					str = L"false";
					s = 0;
					sl = 5;
					break;
				case L'1':
				case L't':
				case L'T':
				case L'y':
				case L'Y':
				case L'е':
				case L'Е':
				case L'н':
				case L'Н':
					str = L"true";
					s = 0;
					sl = 4;
					break;
				case L' ':
					if(str == L"true")
					{
						str = L"false";
						s = 0;
						sl = 5;
					}
					else
					{
						str = L"true";
						s = 0;
						sl = 4;
					}
					break;
			}
			break;
//		case tf_datetime:
//			//
//			//  D  D  .  M  M  .  Y  Y  Y  Y  _  h  h  :  m  m  :  s  s
//			// 0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 16 17 18 19
//			//
//			if(s >= 19)
//			{
//				Key = 0;
//				return;
//			}
//			switch(Key)
//			{
//				case L'0':
//				case L'1':
//				case L'2':
//				case L'3':
//				case L'4':
//				case L'5':
//				case L'6':
//				case L'7':
//				case L'8':
//				case L'9':
//					if(s == 2 || s == 5 || s == 10 || s == 13 || s == 16)
//					{
//						s++;
//						if(sl) sl--;
//					}
//					str[++s] = Key;
//					if(sl) for(i = s + 1, sl--; sl; sl--, i++)
//					{
//						if(i == 3 || i == 6) str[i] = L'.';
////						else if(i == 11) str[i] = L' ';
//						else if(i == 14 || i == 17) str[i] = L':';
//						else str[i] = L' ';
//					}
//					break;
//				case L' ':
//					for(i = ++s; sl; sl--, i++)
//					{
//						if(i == 3 || i == 6) str[i] = L'.';
////						else if(i == 11) str[i] = L' ';
//						else if(i == 14 || i == 17) str[i] = L':';
//						else str[i] = L' ';
//					}
//					break;
//				case L'.':
//				case L',':
//				case L'-':
//				case L'/':
//				case L':':
//					for(i = s + 1; sl; sl--, i++)
//					{
//						if(i == 3 || i == 6) str[i] = L'.';
////						else if(i == 11) str[i] = L' ';
//						else if(i == 14 || i == 17) str[i] = L':';
//						else str[i] = L' ';
//					}
//					if(s < 3) s = 3;
//					else if(s < 6) s = 6;
//					else if(s < 11) s = 11;
//					else if(s < 14) s = 14;
//					else if(s < 17) s = 17;
//					break;
//			}
//			break;
		case tf_numeric:
			length = f->getlength();
			precision = f->getprecision();
			m = false;
			if(l) if(str[1] == L'-') m = true;
			//lm = m ? 1 : 0;
			if(Key == L'-')
			{
				if(l)
				{
					if(sl == l)
					{
						str = L"-";
						s = 1;
					}
					else if(m)
					{
						str = str.SubString(2, l - 1);
						if(s) s--;
						if(sl >= l) sl--;
					}
					else
					{
						str = String(L"-") + str;
						s++;
					}
				}
				else
				{
					str = L"-";
					s = 1;
				}
				break;
			}
			else if(Key == L'.' || Key == L',')
			{
				if(precision == 0)
				{
					Key = 0;
					return;
				}
				i = str.Pos(L".");
				if(i)
				{
					if(s >= i)
					{ // выделение после точки, перемещаем выделение сразу за точку
						s = i;
						sl = 0;
					}
					else
					{ // начало выделения до точки, удаляем от начала выделения до точки или конца выделения
						j = max(i, s + sl);
						str = str.SubString(1, s) + L"." + str.SubString(j + 1, l - j);
						s++;
						sl = 0;
					}
				}
				else
				{
					if(m && !s) s = 1;
					str = str.SubString(1, s) + L".";
					s++;
					sl = 0;
				}
				break;
			}
			else if(Key >= L'0' && Key <= L'9')
			{
				if(m && !s && !sl) s = 1;
				str = str.SubString(1, s) + Key + str.SubString(s + sl + 1, l - s - sl);
				s++;
				sl = 0;
			}
			else if(Key == L'\b')
			{
				if(!s && !sl)
				{
					Key = 0;
					return;
				}
				if(sl) str = str.SubString(1, s) + str.SubString(s + sl + 1, l - s - sl);
				else{
					str = str.SubString(1, s - 1) + str.SubString(s + 1, l - s);
					s--;
				}
				sl = 0;
			}
			else
			{
				Key = 0;
				return;
			}

			validateNumber(str, length, precision, s, sl);
			break;
		default:
			return;
	}

	Key = 0;
	ButtonedEditCommonEdit->Text = str;
	ButtonedEditCommonEdit->SelStart = s;
	ButtonedEditCommonEdit->SelLength = sl;

}

//---------------------------------------------------------------------------
void __fastcall TFormMain::validateNumber(String& str, int length, int precision, int& s, int& sl)
{

	int l, i, j;
	int lm, l1, lp, l2; // длина минуса, знаков до точки, точки и знаков после точки
	bool m;

	l = str.Length();
	m = false;
	if(l) if(str[1] == L'-') m = true;
	i = str.Pos(L".");
	lm = m ? 1 : 0;
	if(i)
	{
		l1 = i - lm - 1;
		//lp = 1;
		l2 = l - lm - l1 - 1;
	}
	else
	{
		l1 = l - lm;
		//lp = 0;
		l2 = 0;
	}
	if(l1 > length - precision)
	{
		if(precision)
		{
			if(i)
			{
				str[i] = str[i - 1];
				str[i - 1] = L'.';
				i--;
				if(s == i) s++;
				//l1--;
				l2++;
			}
			else
			{
				j = length - precision + lm;
				str = str.SubString(1, j) + L'.' + str.SubString(j + 1, l - j);
				s++;
				l++;
				//l1 = length - precision;
				//l2 = l - lm - l1 - 1;
				l2 = l - j - 1;
			}
		}
		else
		{
			str = str.SubString(1, lm + length);
		}
	}
	if(l2 > precision) str = str.SubString(1, l - 1);
	if(s > str.Length()) s = str.Length();
	if(s + sl > str.Length()) sl = str.Length() - s;

}

//---------------------------------------------------------------------------
void __fastcall TFormMain::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
//	field* f;
//	type_fields tf;
//	int l, i, j, s, sl;
//	int lm, l1, lp, l2; // длина минуса, знаков до точки, точки и знаков после точки
//	int length, precision;
//	String str;
//	bool m;

	if(tab) if(tab->get_edit())
	{
		if(Key == VK_CANCEL) // Control-break
		{
			ActionDiscardTableChanges->Execute();
			Key = 0;
			return;
		}
	}

//	if(!edit_tab) return;
//
//	f = tab->getfield(edit_numfield);
//	tf = f->gettype();
//
//	str = ButtonedEditCommonEdit->Text;
//	l = str.Length();
//	s = ButtonedEditCommonEdit->SelStart;
//	sl = ButtonedEditCommonEdit->SelLength;
//
//	switch(tf)
//	{
//		case tf_datetime:
//			switch(Key)
//			{
//				case VK_BACK:
//					if(sl) for(i = s + 1; sl; sl--, i++)
//					{
//						if(i == 3 || i == 6) str[i] = L'.';
////						else if(i == 11) str[i] = L' ';
//						else if(i == 14 || i == 17) str[i] = L':';
//						else str[i] = L' ';
//					}
//					else if(s)
//					{
//						if(s == 3 || s == 6 || s == 11 || s == 14 || s == 17) s--;
//						str[s--] = L' ';
//					}
//					break;
//				case VK_DELETE:
//					if(sl) for(i = s + 1, s += sl; sl; sl--, i++)
//					{
//						if(i == 3 || i == 6) str[i] = L'.';
////						else if(i == 11) str[i] = L' ';
//						else if(i == 14 || i == 17) str[i] = L':';
//						else str[i] = L' ';
//					}
//					else if(s < 19)
//					{
//						if(s == 2 || s == 5 || s == 10 || s == 13 || s == 16) s++;
//						str[++s] = L' ';
//					}
//					break;
//				default:
//					return;
//			}
//			break;
//		case tf_numeric:
//			length = f->getlength();
//			precision = f->getprecision();
////			m = false;
////			if(l) if(str[1] == L'-') m = true;
//			if(Key == VK_DELETE)
//			{
//				if(s == l && !sl)
//				{
//					Key = 0;
//					return;
//				}
//				if(sl) str = str.SubString(1, s) + str.SubString(s + sl + 1, l - s - sl);
//				else str = str.SubString(1, s) + str.SubString(s + 2, l - s - 1);
//				sl = 0;
//			}
//			else return;
//
//			l = str.Length();
//			m = false;
//			if(l) if(str[1] == L'-') m = true;
//			i = str.Pos(L".");
//			lm = m ? 1 : 0;
//			if(i)
//			{
//				l1 = i - lm - 1;
//				//lp = 1;
//				l2 = l - lm - l1 - 1;
//			}
//			else
//			{
//				l1 = l - lm;
//				//lp = 0;
//				l2 = 0;
//			}
//			if(l1 > length - precision)
//			{
//				if(precision)
//				{
//					if(i)
//					{
//						str[i] = str[i - 1];
//						str[i - 1] = L'.';
////						i--;
////						if(s == i) s++;
//						l2++;
//					}
//					else
//					{
//						j = length - precision + lm;
//						str = str.SubString(1, j) + L'.' + str.SubString(j + 1, l - j);
////						s++;
//						l++;
//						l2 = l - j - 1;
//					}
//				}
//			}
//			if(l2 > precision) str = str.SubString(1, l - 1);
//			if(s > str.Length()) s--;
//
//			break;
//		case tf_bool:
//			switch(Key)
//			{
//				case VK_BACK:
//				case VK_DELETE:
//					Key = 0;
//					return;
//				default:
//					return;
//			}
////			break;
//		default:
//			return;
//	}
//
//	Key = 0;
//	ButtonedEditCommonEdit->Text = str;
//	ButtonedEditCommonEdit->SelStart = s;
//	ButtonedEditCommonEdit->SelLength = sl;

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionFindLostObjectsExecute(TObject *Sender)
{
#ifndef PublicRelease
	if(!base1CD->is_open()) return;
	base1CD->find_lost_objects();
#endif //#ifdef PublicRelease
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionTestStreamFormatExecute(TObject *Sender)
{
	bool result;
	if(!base1CD->is_open()) return;
	result = base1CD->test_stream_format();
	if(result)
	{
		mess->AddMessage(L"Тестирование формата потока завершено без ошибок", msSuccesfull);
	}
	else
	{
		mess->AddError(L"Тестирование формата потока завершено c ошибками");
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionCreateEmptyObjectExecute(TObject *Sender)
{
#ifndef PublicRelease
	if(!base1CD->is_open()) return;

	// Создаем пустой объект в базе
	v8object* ob = new v8object(base1CD);
	base1CD->flush();
	mess->AddMessage_(L"Добавлен пустой объект", msSuccesfull,
		L"Номер блока", ob->get_block_number(),
		L"Номер блока шестнадцатирично", tohex(ob->get_block_number()));
#endif //#ifdef PublicRelease
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionCreateObjectFromFileExecute(TObject *Sender)
{
#ifndef PublicRelease
	if(!base1CD->is_open()) return;

	if(!OpenDialogExt->Execute()) return;

	TFileStream* fs = new TFileStream(OpenDialogExt->FileName, fmOpenRead | fmShareDenyWrite);

	// Создаем объект в базе
	v8object* ob = new v8object(base1CD);
	ob->setdata(fs);
	base1CD->flush();
	mess->AddMessage_(L"Добавлен новый объект", msSuccesfull,
		L"Номер блока", ob->get_block_number(),
		L"Номер блока шестнадцатирично", tohex(ob->get_block_number()));

#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionCreateAndImportTableExecute(TObject *Sender)
{
#ifndef PublicRelease
	if(DirectoryEditCreateImportTable->Text.Length() == 0) return;
	if(!base1CD->is_open()) return;
	base1CD->create_table(DirectoryEditCreateImportTable->Text);
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionEnableEditExecute(TObject *Sender)
{
#ifndef PublicRelease
	if(!base1CD) return;
	if(!base1CD->is_open()) return;
	base1CD->set_readonly(false);
	mess->AddMessage(L"Установлен режим редактирования.", msSuccesfull);
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionTestListTablesExecute(TObject *Sender)
{
	if(!base1CD) return;
	if(!base1CD->is_open()) return;
	if(base1CD->test_list_of_tables()) mess->AddMessage(L"Проверка состава таблиц завершена без ошибок.", msSuccesfull);
	else mess->AddMessage(L"Проверка состава таблиц завершена c ошибками.", msWarning);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionCreateTablesExecute(TObject *Sender)
{
#ifndef PublicRelease
	TSearchRec sr;
	String exppath;

	if(!base1CD) return;
	if(!base1CD->is_open()) return;
	exppath = DirectoryEditExportImportTable->Text;
	if(exppath.Length() == 0) return;
	exppath += L"\\";

	if(FindFirst(exppath + L"*.*", faDirectory, sr) == 0)
	{
		do
		{
			if(sr.Name == L"." || sr.Name == L"..") continue;
//			if(sr.Attr & faDirectory) base1CD->create_table(DirectoryEditExportImportTable->Text + L"\\" + sr.Name);
			if(sr.Attr & faDirectory) base1CD->create_table(exppath + sr.Name);
		}
		while(FindNext(sr) == 0);

		FindClose(sr);
	}

	mess->AddMessage(L"Импорт и создание таблиц завершено.", msSuccesfull);
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionReplaceTREFExecute(TObject *Sender)
{
#ifndef PublicRelease
	if(!base1CD) return;
	if(!base1CD->is_open()) return;
	if(AdvFileNameEditReplaceTREF->Text.Length() == 0) return;

	base1CD->replaceTREF(AdvFileNameEditReplaceTREF->Text);

	mess->AddMessage(L"Замена полей ...TREF завершена.", msSuccesfull);
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionBlobParseUsersUsrExecute(TObject *Sender)
{
	ParseBlob();
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::StringGridPhysicalViewCanSort(TObject *Sender, int ACol, bool &DoSort)
{
	field* curfield;
	int i;

	DoSort = false;

	if(ACol)
	{
		curfield = tab->getfield(ACol - 1);
		bool findindex = false;
		for(i = 0; i < tab->get_numindexes(); i++) if(tab->getindex(i)->get_records()->field == curfield)
		{
			findindex = true;
			break;
		}
		if(!findindex) return;

		if(tr_thread)
		{
			//tr_thread->Terminate();
			tr_thread->WaitFor();
			delete tr_thread;
			tr_thread = NULL;
		}

		cur_index = tab->getindex(i);
		ComboBoxCurrentIndex->ItemIndex = i + 1;
	}
	else
	{
		if(tr_thread)
		{
			//tr_thread->Terminate();
			tr_thread->WaitFor();
			delete tr_thread;
			tr_thread = NULL;
		}
		cur_index = NULL;
		ComboBoxCurrentIndex->ItemIndex = 0;
	}

	if(ACol == sorted_column) reverse_sort = !reverse_sort;
	else reverse_sort = false;
	sorted_column = ACol;

	ReSort();
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ReSort()
{
	if(tr_thread)
	{
		//tr_thread->Terminate();
		tr_thread->WaitFor();
		delete tr_thread;
		tr_thread = NULL;
	}

	if(cur_index)
	{
		StringGridPhysicalView->RowCount = cur_index->get_numrecords() + 1 + tab->get_added_numrecords();
	}
	else{
		if(!tab->recordsindex_complete)
		{
			StringGridPhysicalView->RowCount = 1;
			tr_thread = new TableReadThread(false, FormMain);
			if(tab->get_edit()) tr_thread->WaitFor();
		}
		else StringGridPhysicalView->RowCount = tab->numrecords_found + 1 + tab->get_added_numrecords();

	}

	if(StringGridPhysicalView->RowCount > 2)
	{
		StringGridPhysicalView->SortSettings->Column = sorted_column;
		StringGridPhysicalView->SortSettings->Direction = reverse_sort ? sdDescending : sdAscending;
	}
	else StringGridPhysicalView->SortSettings->Column = -1;

	SetEditTableVisible();
	StringGridPhysicalView->Refresh();

}

//---------------------------------------------------------------------------
unsigned int __fastcall TFormMain::get_phys_numrec(int ARow)
{
	if(!tab) return 0;
	if(reverse_sort) if(tab->get_log_numrecords() >= (unsigned int)ARow) return tab->get_phys_numrec(tab->get_log_numrecords() - ARow + 1, cur_index);
	return tab->get_phys_numrec(ARow, cur_index);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionAssociateFileExecute(TObject *Sender)
{
	if(MessageBoxW(Handle, L"Вы действительно хотите связать файлы *.1CD с программой Tool_1CD?\r\nПосле связывания можно будет запускать Tool_1CD из проводника двойным кликом по файлу *.1CD.", L"Внимание!", MB_YESNO) != IDYES) return;

	UnicodeString s;
	UnicodeString filetype = L".1cd";
	UnicodeString filedescr = L"v8.InfoBase";
	UnicodeString openwith = Application->ExeName;
	UnicodeString openaction = L"Open";
	UnicodeString openactiondescr = L"&Open";

	bool success = true;

	TRegistry* reg = new TRegistry;
	try
	{
		//reg->RootKey = (HKEY)0x80000000; //HKEY_CLASSES_ROOT;
		reg->RootKey = HKEY_CLASSES_ROOT;

		reg->OpenKey(filetype, true);
		s = reg->ReadString(L"");
		if(s.Length() == 0) reg->WriteString(L"", filedescr);
		else filedescr = s;
		reg->CloseKey();

		reg->OpenKey(filedescr, true);
		s = reg->ReadString(L"");
		if(s.Length() == 0) reg->WriteString(L"", filedescr);
		reg->CloseKey();

		if(!reg->OpenKey(filedescr + L"\\DefaultIcon", false))
		{
			reg->OpenKey(filedescr + L"\\DefaultIcon", true);
			reg->WriteString(L"", openwith + L",0");
			reg->CloseKey();
		}

		reg->OpenKey(filedescr + L"\\Shell\\" + openaction, true);
		reg->WriteString(L"", openactiondescr);
		reg->CloseKey();

		reg->OpenKey(filedescr + L"\\Shell\\" + openaction + L"\\Command", true);
		reg->WriteString(L"", "\"" + openwith + "\" \"%1\"");
		reg->CloseKey();

		SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
	}
	catch(...)
	{
		success = false;
	}

	delete reg;

	if(success) mess->AddMessage(L"Tool_1CD ассоциирована с файлами *.1cd", msSuccesfull);
	else mess->AddError(L"Не удалось ассоциировать Tool_1CD с файлами *.1cd");

}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionXMLSaveBLOBToFileExecute(TObject *Sender)
{
	ActionXMLUnpackBLOB->Visible = ActionXMLSaveBLOBToFile->Checked;
	return;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionXMLUnpackBLOBExecute(TObject *Sender)
{
	return;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionDeleteTableExecute(TObject *Sender)
{
#ifndef PublicRelease
	if(!tab) return;
	if(!base1CD->delete_table(tab)) return;

	RefreshListOfTables();
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionFindAndCreateLostTablesExecute(TObject *Sender)
{
#ifndef PublicRelease
	if(!base1CD) return;
	if(!base1CD->is_open()) return;

	base1CD->find_and_create_lost_tables();
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionDeleteWrongTablesExecute(TObject *Sender)
{
#ifndef PublicRelease
	table* t;
	int i, j;
	String n;

	if(!base1CD) return;
	j = 0;
	for(i = base1CD->get_numtables() - 1; i >= 0; i--)
	{
		t = base1CD->gettable(i);
		n = t->getname();
		if(n.SubString(n.Length() - 1, 2).CompareIC(L"NG") == 0 || n.SubString(n.Length() - 1, 2).CompareIC(L"OG") == 0 || n.CompareIC(L"DBCHANGES") == 0)
		{
			base1CD->delete_table(t);
			j++;
		}
	}

	RefreshListOfTables();

	mess->AddMessage_(L"Удаление лишних таблиц завершено", msSuccesfull,
		L"Удалено таблиц", j);
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionFindTableDoublesExecute(TObject *Sender)
{
	table* t;
	int i, j, k;
	String n;

	if(!base1CD) return;
	k = 0;
	for(i = 0; i < base1CD->get_numtables(); i++)
	{
		t = base1CD->gettable(i);
		n = t->getname();
		for(j = i + 1; j < base1CD->get_numtables(); j++)
		if(n.CompareIC(base1CD->gettable(j)->getname()) == 0)
		{
			k++;
			mess->AddMessage_(L"Найден дубль имен таблиц", msWarning,
				L"Имя таблицы", n);
		}
	}

	mess->AddMessage_(L"Поиск дублей таблиц завершен", msSuccesfull,
		L"Найдено дублей", k);

}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionFindAndSaveLostObjectsExecute(TObject *Sender)
{
#ifndef PublicRelease
	if(!base1CD->is_open()) return;
	base1CD->find_and_save_lost_objects();
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionCommandLineKeysExecute(TObject *Sender)
{
	//MessageBoxW(Handle, CommandParse::gethelpstring().c_str(), L"Ключи командной строки", MB_OK);
	FormCommandLineDescription->Show();
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionTestStreamFormatExternalExecute(TObject *Sender)
{
	// Тест формата потока внешнего файла

	TFileStream* f;
	bool r;

	if(OpenDialogExt->Execute())
	{
		f = new TFileStream(OpenDialogExt->FileName, fmOpenRead | fmShareDenyWrite);
		r = test_parse_1Ctext(f, f->FileName);
		if(r) mess->AddMessage_(L"Тест формата потока завершен без ошибок", msSuccesfull,
			L"Файл", f->FileName);
		else mess->AddMessage_(L"Тест формата потока завершен c ошибками", msWarning,
			L"Файл", f->FileName);
		delete f;
	}

}

//---------------------------------------------------------------------------
void __fastcall TFormMain::StringGridPhysicalViewKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift)
{
	if(Key == VK_DELETE) Key = 0;
//	mess->AddMessage_(L"Клавиша нажата", msSuccesfull,
//		L"Код", Key);

}

//---------------------------------------------------------------------------
void __fastcall TFormMain::StringGridPhysicalViewKeyUp(TObject *Sender, WORD &Key,
		  TShiftState Shift)
{
	if(Key == VK_DELETE)
	{
		Key = 0;
		ActionEditTableDelete->Execute();
	}
//	mess->AddMessage_(L"Клавиша отпущена", msSuccesfull,
//		L"Код", Key);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::MaskEditCommonEdit_MouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	if(Button == mbRight) PopupEditMenu(X, Y);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ButtonedEditCommonEditRightButtonClick(TObject *Sender)
{
	PopupEditMenu();
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ButtonedEditCommonEditChange(TObject *Sender)
{
	field* f;
	type_fields tf;
	int l, i, j, s, sl;
	wchar_t sym;
//	int lm, l1, lp, l2; // длина минуса, знаков до точки, точки и знаков после точки
	int length, precision;
	String str;
	String tmpl;
	bool m, point, guid;

	if(!tab) return;
	if(!tab->get_edit()) return;
	if(!edit_tab) return;

	f = tab->getfield(edit_numfield);
	tf = f->gettype();

	str = curedit->Text;
	l = str.Length();
	s = curedit->SelStart;
	sl = curedit->SelLength;

	length = f->getlength();
	precision = f->getprecision();

	switch(tf)
	{
		case tf_binary:
			guid = length == 16 && field::showGUID; // признак редактирования GUID
			l = guid ? 36 : length * 2;
			for(i = 1; i <= str.Length();)
			{
				m = false;
				if(i <= l)
				{
					sym = str[i];
					if(guid && (i == 9 || i == 14 || i == 19 || i == 24))
					{
						if(sym == L'-') m = true;
					}
					else
					{

						switch(sym)
						{
							case L'A':
							case L'a':
							case L'Ф':
							case L'ф':
								sym = L'a';
								m = true;
								break;
							case L'B':
							case L'b':
							case L'И':
							case L'и':
								sym = L'b';
								m = true;
								break;
							case L'C':
							case L'c':
							case L'С':
							case L'с':
								sym = L'c';
								m = true;
								break;
							case L'D':
							case L'd':
							case L'В':
							case L'в':
								sym = L'd';
								m = true;
								break;
							case L'E':
							case L'e':
							case L'У':
							case L'у':
								sym = L'e';
								m = true;
								break;
							case L'F':
							case L'f':
							case L'А':
							case L'а':
								sym = L'f';
								m = true;
								break;
							case L'0':
							case L'1':
							case L'2':
							case L'3':
							case L'4':
							case L'5':
							case L'6':
							case L'7':
							case L'8':
							case L'9':
							case L' ':
								m = true;
								break;
						}
					}
				}
				if(m)
				{
					str[i] = sym;
					i++;
				}
				else
				{
					str = str.SubString(1, i - 1) + str.SubString(i + 1, str.Length() - i);
					if(sl) if(s + sl >= i && s < i) sl--;
					if(s >= i) s--;
				}
			}

			if(str.Length() < l)
			{
				if(guid) tmpl = L"        -    -    -    -            ";
				else
				{
					tmpl = L"";
					for(i = 0; i < length; i++) tmpl += "  ";
				}
				str += tmpl.SubString(str.Length() + 1, l - str.Length());
			}
			break;
		case tf_bool:
			for(i = 1; i <= str.Length(); i++)
			{
				sym = str[i];
				if(sym == L'T' || sym == L't' || sym == L'Д' || sym == L'д' || sym == L'И' || sym == L'и' || sym == L'Y' || sym == L'y')
				{
					str = L"true";
					s = 0;
					sl = 4;
					break;
				}
				if(sym == L'F' || sym == L'f' || sym == L'Н' || sym == L'н' || sym == L'Л' || sym == L'л' || sym == L'N' || sym == L'n')
				{
					str = L"false";
					s = 0;
					sl = 5;
					break;
				}
			}
			break;
		case tf_datetime:
			break;
		case tf_numeric:
			for(i = 1; i <= str.Length();)
			{
				sym = str[i];
				m = false; // признак валидного символа
				point = false; // признак, что точка уже была
				switch(sym)
				{
					case L'-':
						if(i == 1) m = true;
						break;
					case L'0':
					case L'1':
					case L'2':
					case L'3':
					case L'4':
					case L'5':
					case L'6':
					case L'7':
					case L'8':
					case L'9':
						m = true;
						break;
					case L'.':
					case L',':
						if(!point)
						{
							m = true;
							point = true;
						}
						break;
				}
				if(m) i++;
				else
				{
					str = str.SubString(1, i - 1) + str.SubString(i + 1, str.Length() - i);
					if(sl) if(s + sl >= i && s < i) sl--;
					if(s >= i) s--;

				}
			}

			validateNumber(str, length, precision, s, sl);
			break;
		default:
			return;
	}

	curedit->Text = str;
	curedit->SelStart = s;
	curedit->SelLength = sl;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionWriteBlobToFileExecute(TObject *Sender)
{
	int ARow;
	int ACol;
	String fname;
	String under(L"_");
	if(!base1CD) return;
	if(!tab) return;
	ACol = StringGridPhysicalView->Selection.Left;
	ARow = StringGridPhysicalView->Selection.Top;
	if(!ARow || !ACol) return;

//	fname = L"";
//	if(cat)
//	{
//		TTreeNode* cur = TreeViewContainer->Selected;
//		while(cur)
//		{
//			if(fname.Length()) fname = under + fname;
//			fname = cur->Text + fname;
//			cur = cur->Parent;
//		}
//	}
//
//	if(fname.Length()) fname = under + fname;
//	fname = tab->get_file_name_for_field(ACol - 1, tab->get_edit_record(get_phys_numrec(ARow), tabrec)) + fname;

	fname = tab->get_file_name_for_field(ACol - 1, tab->get_edit_record(get_phys_numrec(ARow), tabrec));

	SaveDialogFile->Title = L"Сохранение BLOB/MEMO";
	SaveDialogFile->FileName = fname;
	if(SaveDialogFile->Execute())
	{
		if(cat)
		{
			cat->SaveToDir(SaveDialogFile->FileName);
		}
		else
		{
			TFileStream* fs = new TFileStream(SaveDialogFile->FileName, fmCreate | fmShareDenyNone);
			fs->CopyFrom(temp_stream, 0);
			delete fs;
		}

//		mess->AddMessage_(L"Сохранен BLOB/MEMO.", msInfo,
//			L"Таблица", tab->getname(),
//			L"Файл", SaveDialogFile->FileName);
	}
}

//---------------------------------------------------------------------------
void exchangeRefs(void** Ref1, void** Ref2)
{
	void* Ref = *Ref1;
	*Ref1 = *Ref2;
	*Ref2 = Ref;
}

#define exchRefs(r1,r2) exchangeRefs(&(void*)(r1), &(void*)(r2))

//---------------------------------------------------------------------------
void __fastcall TFormMain::AddDirectoryToContainer(String DirName, v8catalog* cat)
{
	TSearchRec sr;
	TFileStream* fs;
	v8file* f;
	v8catalog* subcat;
	FILETIME create, modify;

	if(FindFirst(DirName +L"\\*.*", faDirectory | faAnyFile, sr) == 0)
	{
		do
		{
			if(sr.Name == L".") continue;
			if(sr.Name == L"..") continue;
			if(sr.Attr & faDirectory)
			{
				subcat = cat->CreateCatalog(sr.Name);
				AddDirectoryToContainer(DirName + L"\\" + sr.Name, subcat);
			}
			else{
				f = cat->createFile(sr.Name);
				try
				{
					fs = new TFileStream(DirName + L"\\" + sr.Name, fmOpenRead | fmShareDenyWrite);
				}
				catch(...)
				{
					if(mess) mess->AddError(L"Ошибка открытия файла",
						L"Файл", DirName + L"\\" + sr.Name);
					continue;
				}
				f->Write(fs);
				GetFileTime((HANDLE)fs->Handle, &create, NULL, &modify);
				f->SetTimeCreate(&create);
				f->SetTimeModify(&modify);
				delete fs;
			}
		}while (FindNext(sr) == 0);
		FindClose(sr);
	}
}

//---------------------------------------------------------------------------
String __fastcall GetFileTimePresentation(const FILETIME* ft)
{
	SYSTEMTIME st;
	FILETIME lt;
	String ret;
	FileTimeToLocalFileTime(ft, &lt);
	FileTimeToSystemTime(&lt, &st);
	return ret.sprintf(L"%.2hu.%.2hu.%.4hu %.2hu:%.2hu:%.2hu", st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionReadBlobFromFileExecute(TObject *Sender)
{
#ifndef PublicRelease
	TGridRect coord;
	int ARow;
	int ACol;
	bool tmp;
	TFileStream* fst = NULL;
	TStream* st1 = NULL;
	TStream* st2 = NULL;
	char mask[] = "Tool_1CD was here";
	int mask_size = sizeof(mask);
	char t;
	int i;
	type_fields tf;
	field* fld;
	unsigned int physnr;
	WIN32_FILE_ATTRIBUTE_DATA atr;
	//FILETIME create, modify;

	if(!base1CD) return;
	if(!tab) return;
	if(!tab->get_edit()) return;
	coord = StringGridPhysicalView->Selection;
	ACol = coord.Left;
	ARow = coord.Top;
	if(!ARow || !ACol) return;

	physnr = get_phys_numrec(ARow);
	fld = tab->getfield(ACol - 1);
	tf = fld->gettype();
	if(tf == tf_string || tf == tf_text)
	{
		if(!OpenDialogExt->Execute()) return;
		fst = new TFileStream(OpenDialogExt->FileName, fmOpenRead | fmShareDenyWrite);
		st1 = new TMemoryStream;
		st1->CopyFrom(fst, 0);
		delete fst;
		tab->set_edit_value(physnr, ACol - 1, false, L"", st1);
	}
	else if(tf == tf_image)
	{
		if(tab->getname().CompareIC(L"V8USERS") == 0 || (tab->getname().CompareIC(L"PARAMS") == 0 && fld->getname().CompareIC(L"users.usr") == 0)) FormOpenBLOB->obMode = obmUserUsr;
		else if(tab->getname().CompareIC(L"CONFIG") == 0 || tab->getname().CompareIC(L"CONFIGSAVE") == 0 || tab->getname().CompareIC(L"PARAMS") == 0 || tab->getname().CompareIC(L"FILES") == 0) FormOpenBLOB->obMode = obmFiles;
		else if(tab->get_issystem()) FormOpenBLOB->obMode = obmSystem;
		else FormOpenBLOB->obMode = obmDefault;

		if(mrOk != FormOpenBLOB->ShowModal()) return;
		{
			st1 = new TMemoryStream;
			st2 = new TMemoryStream;
			if(FormOpenBLOB->CheckBoxBlobParseContainer->Checked)
			{
				// создаем новый контейнер
				v8catalog* cat = new v8catalog(st1, FormOpenBLOB->CheckBoxBlobParseUnzip2->Checked, true);
				AddDirectoryToContainer(FormOpenBLOB->DirectoryEdit->Text, cat);
				delete cat;
				GetFileAttributesEx(FormOpenBLOB->DirectoryEdit->Text.c_str(), GetFileExInfoStandard, &atr);
			}
			else
			{
				fst = new TFileStream(FormOpenBLOB->FileNameEdit->Text, fmOpenRead | fmShareDenyWrite);
				//GetFileTime((HANDLE)fst->Handle, &create, NULL, &modify);
				st1->CopyFrom(fst, 0);
				delete fst;
				GetFileAttributesEx(FormOpenBLOB->FileNameEdit->Text.c_str(), GetFileExInfoStandard, &atr);
			}
			if(FormOpenBLOB->CheckBoxBlobParseUsersUsr->Checked)
			{
				exchRefs(st1, st2);
				st1->Size = 0;
				st2->Seek(0, soFromBeginning);
				st1->Write(&mask_size, 1);
				st1->Write(mask, mask_size);
				i = 0;
				while(st2->Read(&t, 1))
				{
					t ^= mask[i++];
					if(i >= mask_size) i = 0;
					st1->Write(&t, 1);
				}
			}
			if(FormOpenBLOB->CheckBoxBlobParseUnzip->Checked)
			{
				exchRefs(st1, st2);
				st1->Size = 0;
				st2->Seek(0, soFromBeginning);
				DeflateStream(st2, st1);
			}
			if(FormOpenBLOB->CheckBoxBlobParseUnzip2->Checked)
			{
				exchRefs(st1, st2);
				st1->Size = 0;
				st2->Seek(0, soFromBeginning);
				DeflateStream(st2, st1);
			}
			if(FormOpenBLOB->CheckBoxChangeRelatedFields->Checked)
			{
				for(i = 0; i < tab->get_numfields(); i++)
				{
					fld = tab->getfield(i);
					//if(fld->getname().CompareIC(L"FILENAME")
					if(fld->getname().CompareIC(L"CREATION") == 0) tab->set_edit_value(physnr, i, false, GetFileTimePresentation(&atr.ftCreationTime));
					else if(fld->getname().CompareIC(L"MODIFIED") == 0) tab->set_edit_value(physnr, i, false, GetFileTimePresentation(&atr.ftLastWriteTime));
					else if(fld->getname().CompareIC(L"DATASIZE") == 0) tab->set_edit_value(physnr, i, false, st1->Size);
				}
			}

			tab->set_edit_value(physnr, ACol - 1, false, L"", st1);
			delete st2;
		}
	}
	else return;

//	StringGridPhysicalView->InvalidateGridRect(coord);
	coord.Left = 0;
	coord.Right = StringGridPhysicalView->ColCount - 1;
	StringGridPhysicalView->InvalidateGridRect(coord);
	StringGridPhysicalViewSelectCell(Sender, ACol, ARow, tmp);
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionBlobClearExecute(TObject *Sender)
{
#ifndef PublicRelease
	TGridRect coord;
	int ARow;
	int ACol;
	bool tmp;

	if(!base1CD) return;
	if(!tab) return;
	if(!tab->get_edit()) return;
	coord = StringGridPhysicalView->Selection;
	ACol = coord.Left;
	ARow = coord.Top;
	if(!ARow || !ACol) return;

	tab->set_edit_value(get_phys_numrec(ARow), ACol - 1, false, L"", NULL);

	StringGridPhysicalView->InvalidateGridRect(coord);
	coord.Left = 0;
	coord.Right = 0;
	StringGridPhysicalView->InvalidateGridRect(coord);
	StringGridPhysicalViewSelectCell(Sender, ACol, ARow, tmp);
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ActionBlobRestoreExecute(TObject *Sender)
{
#ifndef PublicRelease
	TGridRect coord;
	int ARow;
	int ACol;
	bool tmp;

	if(!base1CD) return;
	if(!tab) return;
	if(!tab->get_edit()) return;
	coord = StringGridPhysicalView->Selection;
	ACol = coord.Left;
	ARow = coord.Top;
	if(!ARow || !ACol) return;

	tab->restore_edit_value(get_phys_numrec(ARow), ACol - 1);

	StringGridPhysicalView->InvalidateGridRect(coord);
	coord.Left = 0;
	coord.Right = 0;
	StringGridPhysicalView->InvalidateGridRect(coord);
	StringGridPhysicalViewSelectCell(Sender, ACol, ARow, tmp);
#endif //#ifdef PublicRelease
}

//---------------------------------------------------------------------------
// Тестовая процедура (удалить!)
void __fastcall TFormMain::Button1Click(TObject *Sender)
{
	// Тест вывода дерева во внешний файл

	TFileStream* f;
	tree* t;
	wchar_t* buf;
	int i;
	String s;

	if(OpenDialogExt->Execute())
	{
		f = new TFileStream(OpenDialogExt->FileName, fmOpenRead | fmShareDenyWrite);
		i = f->Size / 2 - 1;
		buf = new wchar_t[i + 1];
		f->Read(buf, 2);
		f->Read(buf, i * 2);
		delete f;
		buf[i] = 0;
		s = buf;
		delete[] buf;
		t = parse_1Ctext(s, OpenDialogExt->FileName);
		if(t)
		{
			s = outtext(t);
			f = new TFileStream(OpenDialogExt->FileName + L"_test", fmCreate | fmShareDenyNone);
			f->Write(sUTF16LEBOM, 2);
			f->Write(s.c_str(), s.Length() * 2);
			delete f;
		}
		delete t;
	}


}

//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionSaveLastDepotConfigExecute(TObject *Sender)
{
	String s;
	int v;

	if(!base1CD->is_open()) return;
	v = base1CD->get_ver_depot_config(0);
	if(!v) return;
	s = L"v";
	s += v;
	s += L".cf";
	SaveDialogCF->FileName = s;
	if(SaveDialogCF->Execute())
	{
		if(base1CD->save_depot_config(SaveDialogCF->FileName, v))
			mess->AddMessage_(L"Сохранение последней конфигурации хранилища завершено.", msSuccesfull,
				L"Файл", SaveDialogCF->FileName);
		else
			mess->AddMessage_(L"Не удалось сохранить последнюю конфигурацию хранилища.", msError,
				L"Файл", SaveDialogCF->FileName);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionSaveDepotConfigExecute(TObject *Sender)
{
	char* rec;
	field* fld;
	bool ok;
	unsigned int i;
	int v, j;
	String s;

	if(!base1CD->is_open()) return;

	if(!base1CD->is_depot)
	{
		if(mess) mess->AddError(L"База не является хранилищем конфигурации.");
		return;
	}

	if(tab != base1CD->table_versions) return;

	ok = false;
	for(j = 0; j < tab->get_numfields(); j++)
	{
		fld = tab->getfield(j);
		if(fld->getname().CompareIC(L"VERNUM") == 0)
		{
			ok = true;
			break;
		}
	}
	if(!ok)
	{
		if(mess) mess->AddError(L"В базе хранилища в таблице VERSIONS не найдено поле VERNUM.");
		return;
	}

	i = get_phys_numrec(StringGridPhysicalView->Selection.Top);
	rec = tab->get_edit_record(i, tabrec);
	s = fld->get_presentation(rec, true);
	v = s.ToIntDef(0);
	if(!v)
	{
		if(mess) mess->AddError(L"Не удалось получить текущий номер версии конфигурации.");
		return;
	}

	s = L"v";
	s += v;
	s += L".cf";
	SaveDialogCF->FileName = s;
	if(SaveDialogCF->Execute())
	{
		if(base1CD->save_depot_config(SaveDialogCF->FileName, v))
			mess->AddMessage_(L"Сохранение конфигурации хранилища завершено.", msSuccesfull,
				L"Файл", SaveDialogCF->FileName);
		else
			mess->AddMessage_(L"Не удалось сохранить конфигурацию хранилища.", msError,
				L"Файл", SaveDialogCF->FileName);
	}

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionRestoreDATAAllocationTableExecute(TObject *Sender)
{
#ifndef PublicRelease
	if(!base1CD->is_open()) return;
	if(!tab) return;

	base1CD->restore_DATA_allocation_table(tab);
#endif //#ifdef PublicRelease

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionSaveConfigExtensionExecute(TObject *Sender)
{
	char* rec;
	field* fld;
	bool ok;
	unsigned int i;
	int j;
	String hashname, s;
	TGUID g;

	if(!base1CD->is_open()) return;

	if(!base1CD->is_infobase)
	{
		if(mess) mess->AddError(L"База не является информационной базой.");
		return;
	}

	if(tab != base1CD->table__extensionsinfo) return;

	ok = false;
	for(j = 0; j < tab->get_numfields(); j++)
	{
		fld = tab->getfield(j);
		if(fld->getname().CompareIC(L"_CONFIGVERSION") == 0)
		{
			ok = true;
			break;
		}
	}
	if(!ok)
	{
		if(mess) mess->AddError(L"В таблице _EXTENSIONSINFO не найдено поле _CONFIGVERSION.");
		return;
	}

	i = get_phys_numrec(StringGridPhysicalView->Selection.Top);
	rec = tab->get_edit_record(i, tabrec);
	hashname = fld->get_presentation(rec, true);

	ok = false;
	for(j = 0; j < tab->get_numfields(); j++)
	{
		fld = tab->getfield(j);
		if(fld->getname().CompareIC(L"_IDRREF") == 0)
		{
			ok = true;
			break;
		}
	}
	if(!ok)
	{
		if(mess) mess->AddError(L"В таблице _EXTENSIONSINFO не найдено поле _IDRREF.");
		return;
	}
	g = *(TGUID*)(rec + fld->getoffset());

	ok = false;
	for(j = 0; j < tab->get_numfields(); j++)
	{
		fld = tab->getfield(j);
		if(fld->getname().CompareIC(L"_EXTNAME") == 0)
		{
			ok = true;
			break;
		}
	}
	if(!ok)
	{
		if(mess) mess->AddError(L"В таблице _EXTENSIONSINFO не найдено поле _EXTNAME.");
		return;
	}

	s = fld->get_presentation(rec, true);

	s += L".cfe";
	SaveDialogCFE->FileName = s;
	if(SaveDialogCFE->Execute())
	{
		if(base1CD->save_config_ext(SaveDialogCFE->FileName, g, hashname))
			mess->AddMessage_(L"Сохранение расширения конфигурации (основное) завершено.", msSuccesfull,
				L"Файл", SaveDialogCFE->FileName);
		else
			mess->AddMessage_(L"Не удалось сохранить расширение конфигурации (основное).", msError,
				L"Файл", SaveDialogCFE->FileName);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ActionSaveConfigExtensionDbExecute(TObject *Sender)
{
	char* rec;
	field* fld;
	bool ok;
	unsigned int i;
	int j;
	String hashname, s;

	if(!base1CD->is_open()) return;

	if(!base1CD->is_infobase)
	{
		if(mess) mess->AddError(L"База не является информационной базой.");
		return;
	}

	if(tab != base1CD->table__extensionsinfo) return;

	ok = false;
	for(j = 0; j < tab->get_numfields(); j++)
	{
		fld = tab->getfield(j);
		if(fld->getname().CompareIC(L"_CONFIGVERSION") == 0)
		{
			ok = true;
			break;
		}
	}
	if(!ok)
	{
		if(mess) mess->AddError(L"В таблице _EXTENSIONSINFO не найдено поле _CONFIGVERSION.");
		return;
	}

	i = get_phys_numrec(StringGridPhysicalView->Selection.Top);
	rec = tab->get_edit_record(i, tabrec);
	hashname = fld->get_presentation(rec, true);

	ok = false;
	for(j = 0; j < tab->get_numfields(); j++)
	{
		fld = tab->getfield(j);
		if(fld->getname().CompareIC(L"_EXTNAME") == 0)
		{
			ok = true;
			break;
		}
	}
	if(!ok)
	{
		if(mess) mess->AddError(L"В таблице _EXTENSIONSINFO не найдено поле _EXTNAME.");
		return;
	}

	s = fld->get_presentation(rec, true);

	s += L".db.cfe";
	SaveDialogCFE->FileName = s;
	if(SaveDialogCFE->Execute())
	{
		if(base1CD->save_config_ext_db(SaveDialogCFE->FileName, hashname))
			mess->AddMessage_(L"Сохранение расширения конфигурации (базы данных) завершено.", msSuccesfull,
				L"Файл", SaveDialogCFE->FileName);
		else
			mess->AddMessage_(L"Не удалось сохранить расширение конфигурации (базы данных).", msError,
				L"Файл", SaveDialogCFE->FileName);
	}

}
//---------------------------------------------------------------------------


