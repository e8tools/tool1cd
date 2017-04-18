//---------------------------------------------------------------------------

#include <vcl.h>
#include <IOUtils.hpp>
#include <mshtml.h>
#pragma hdrstop

#include "MainForm_MetaRead.h"
#include "FileFormat.h"
#include "TempStream.h"

//---------------------------------------------------------------------------
//#pragma package(smart_init)
#pragma link "AdvEdBtn"
#pragma link "AdvEdit"
#pragma link "AdvFileNameEdit"
#pragma link "TreeList"
#pragma link "AdvDirectoryEdit"
#pragma link "AdvGrid"
#pragma link "AdvObj"
#pragma link "BaseGrid"
#pragma link "acAlphaImageList"
#pragma link "AdvMemo"
#pragma link "AdvCombo"
#pragma link "AdvmWS"
#pragma link "Advmxml"
#pragma link "AdvPanel"
#pragma link "HTMLText"
#pragma link "HTMLabel"
#pragma link "HTMLForm"
#pragma link "AdvOfficeImage"
#pragma link "AdvReflectionImage"
#pragma link "AdvReflectionImage"
#pragma link "WebImage"
#pragma resource "*.dfm"

TFormMain *FormMain;
MessageRegistrator* msreg = NULL;
extern __declspec(thread) TThread* cur_thread;

//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner) : TForm(Owner)
{
	TStream* rstr;

	TabSheetPicture->TabVisible = false;
	TabSheetHTML->TabVisible = false;
	aboutblank = true;

	mess = new Messager(L"", MemoLog);
	msreg = mess;

	String s = Application->ExeName;
	s = s.SubString(1, s.LastDelimiter(L"\\")) + L"MetaTree.txt";
	if(FileExists(s)) rstr = new TFileStream(s, fmOpenRead);
	else rstr = new TResourceStream(NULL, L"MetaTree", RT_RCDATA);

	MetaTypeSet::staticTypesLoad(rstr);
	delete rstr;
	if(MemoLog->Lines->Count > 0) PageControlMain->ActivePage = TabSheetLog;

	Container = NULL;

	VirtualStringTreeValue1C->NodeDataSize = sizeof(VirtualTreeData);

	s_1cv8fv = NULL;
	wchar_t szPath[MAX_PATH];
	if(SUCCEEDED(SHGetFolderPath(NULL, CSIDL_PROGRAM_FILES, NULL, SHGFP_TYPE_CURRENT, szPath)))
	{
		s_1cv8fv = szPath;
		s_1cv8fv += L"\\1cv82fv\\bin\\1cv8fv.exe ";
		if(!FileExists(s_1cv8fv)) s_1cv8fv = NULL;
	}

}

//---------------------------------------------------------------------------
__fastcall TFormMain::~TFormMain()
{
	delete Container;
}

//---------------------------------------------------------------------------
void AddTreeSerialization(TTreeNodes* nodes, TTreeNode* ct, SerializationTreeNode* stn)
{
	System::UnicodeString s;
	TTreeNode* nt;

	while(stn)
	{
		s = L"";
		if(stn->nomove) s = L"!";
		switch(stn->type)
		{
			case stt_const:
				s += L"Константа: ";
				if(stn->typeval1 == stv_string)
				{
					s += L"\"";
					s += stn->str1;
					s += L"\"";
				}
				else if(stn->typeval1 == stv_number) s += stn->num1;
				else if(stn->typeval1 == stv_uid) s += GUID_to_string(stn->uid1);
				else s += L"ОШИБКА";
				break;
			case stt_var:
				s += L"Переменная: ";
				s += stn->str1;
				break;
			case stt_list:
				s += L"Список";
				break;
			case stt_prop:
				s += L"Свойство: ";
				s += stn->prop1->name;
				if(stn->typeprop)
				{
					s += L" (тип ";
					s += stn->typeprop->name;
					s += L")";
				}
				break;
			case stt_elcol:
				s += L"ЭлементКоллекции ";
				if(stn->typeval1 != stv_none)
				{
					s += L" (Счетчик ";
					if(stn->typeval1 == stv_number) s += stn->num1;
					else if(stn->typeval1 == stv_var)
					{
						s += L"Переменная ";
						s += stn->str1;
					}
					else if(stn->typeval1 == stv_prop)
					{
						s += L"Свойство ";
						s += stn->prop1->name;
					}
					else s += L"ОШИБКА";
					s += L")";
				}
				break;
			case stt_gentype:
				s += L"ГенерируемыйТип: ";
				s += stn->gentype->name;
				break;
			case stt_cond:
				s += L"Условие: ";

				if(stn->typeval1 == stv_string)
				{
					s += L"\"";
					s += stn->str1;
					s += L"\"";
				}
				else if(stn->typeval1 == stv_number) s += stn->num1;
				else if(stn->typeval1 == stv_uid) s += GUID_to_string(stn->uid1);
				else if(stn->typeval1 == stv_var)
				{
					s += L"Переменная ";
					s += stn->str1;
				}
				else if(stn->typeval1 == stv_prop)
				{
					s += L"Свойство ";
					s += stn->prop1->name;
				}
				else if(stn->typeval1 == stv_value)
				{
					s += L"Значение ";
					s += stn->val1->getowner()->name;
					s += L".";
					s += stn->val1->name;
				}
				else s += L"ОШИБКА";

				if(stn->condition == stc_e) s += L" = ";
				else if(stn->condition == stc_ne) s += L" <> ";
				else if(stn->condition == stc_g) s += L" > ";
				else if(stn->condition == stc_l) s += L" < ";
				else if(stn->condition == stc_ge) s += L" >= ";
				else if(stn->condition == stc_le) s += L" <= ";
				else s += L"ОШИБКА";

				if(stn->typeval2 == stv_string)
				{
					s += L"\"";
					s += stn->str2;
					s += L"\"";
				}
				else if(stn->typeval2 == stv_number) s += stn->num2;
				else if(stn->typeval2 == stv_uid) s += GUID_to_string(stn->uid2);
				else if(stn->typeval2 == stv_var)
				{
					s += L"Переменная ";
					s += stn->str2;
				}
				else if(stn->typeval2 == stv_prop)
				{
					s += L"Свойство ";
					s += stn->prop2->name;
				}
				else if(stn->typeval2 == stv_value)
				{
					s += L"Значение ";
					s += stn->val2->getowner()->name;
					s += L".";
					s += stn->val2->name;
				}
				else s += L"ОШИБКА";

				break;
			case stt_metaid:
				s += L"МетаИД";
				break;
			default:
				s += L"ОШИБКА";
		}

		nt = nodes->AddChild(ct, s);
		AddTreeSerialization(nodes, nt, stn->first);
		stn = stn->next;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ButtonReadTypesClick(TObject *Sender)
{
	std::vector<MetaType*>::iterator t;
	std::vector<MetaProperty*>::iterator p;
	std::vector<MetaValue*>::iterator v;
	unsigned int j;
	System::UnicodeString s;
	TTreeNodes* nodes;
	TTreeNode* nt;
	TTreeNode* npp;
	TTreeNode* np;

	MetaType* tt;
	int ii;

	nodes = TreeViewTypes->Items;
	for(ii = 0; ii < MetaTypeSet::staticTypes->number(); ++ii)
	{
		tt = MetaTypeSet::staticTypes->getType(ii);
		nt = nodes->AddChild(NULL, tt->name);

		// UID
		if(tt->hasuid) nodes->AddChild(nt, GUID_to_string(tt->uid));

		// Свойства
		if(tt->properties.size() > 0)
		{

			s = L"Свойства (";
			s += tt->properties.size();
			s += L")";
			npp = nodes->AddChild(nt, s);
			for(p = tt->properties.begin(); p != tt->properties.end(); ++p)
			{
				s = (*p)->name;
				np = nodes->AddChild(npp, s);
				for(t = (*p)->types.begin(); t != (*p)->types.end(); ++t)
				{
					s = (*t)->name;
					nodes->AddChild(np, s);
				}
			}

		}

		// Элементы коллекции
		if(tt->collectiontypes.size() > 0)
		{
			s = L"Элементы коллекции (";
			s += tt->collectiontypes.size();
			s += L")";
			npp = nodes->AddChild(nt, s);
			for(t = tt->collectiontypes.begin(); t != tt->collectiontypes.end(); ++t)
			{
				s = (*t)->name;
				nodes->AddChild(npp, s);
			}

		}

		// Значения
		if(tt->values.size() > 0)
		{
			s = L"Значения (";
			s += tt->values.size();
			s += L")";
			npp = nodes->AddChild(nt, s);
			for(v = tt->values.begin(); v != tt->values.end(); ++v)
			{
				s = (*v)->name;
				s += L" = ";
				s += (*v)->value;
				nodes->AddChild(npp, s);
			}

		}

		// Генерируемые типы
		if(tt->generatedtypes.size() > 0)
		{
			s = L"Генерируемые типы (";
			s += tt->generatedtypes.size();
			s += L")";
			npp = nodes->AddChild(nt, s);
			for(j = 0; j < tt->generatedtypes.size(); ++j)
			{
				s = tt->generatedtypes[j]->name;
				nodes->AddChild(npp, s);
			}

		}

		// Дерево сериализации
		if(tt->serializationtree)
		{
			s = L"Переменные дерева сериализации (";
			s += tt->serializationvars.size();
			s += L")";
			npp = nodes->AddChild(nt, s);
			for(j = 0; j < tt->serializationvars.size(); ++j)
			{
				s = tt->serializationvars[j]->name;
				nodes->AddChild(npp, s);
			}

			npp = nodes->AddChild(nt, L"Дерево сериализации");
			AddTreeSerialization(nodes, npp, tt->serializationtree);
		}
	}
}

//---------------------------------------------------------------------------

__fastcall Messager::Messager()
{
	initFormatSettings();
	Lock = new TCriticalSection();

}

//---------------------------------------------------------------------------
__fastcall Messager::Messager(const String& _logfile, TMemo* _MemoLog)
{
	initFormatSettings();
	setlogfile(_logfile);
	MemoLog = _MemoLog;
	Lock = new TCriticalSection();
}

//---------------------------------------------------------------------------
__fastcall Messager::~Messager()
{
    delete Lock;
}

//---------------------------------------------------------------------------
void __fastcall Messager::initFormatSettings()
{
	FormatSettings.DateSeparator = L'.';
	FormatSettings.TimeSeparator = L':';
	//FormatSettings.LongDateFormat = L"dd.mm.yyyy";
	FormatSettings.ShortDateFormat = L"dd.mm.yyyy";
	FormatSettings.LongTimeFormat = L"hh:mm:ss:zzz";
}

//---------------------------------------------------------------------------
void __fastcall Messager::setlogfile(const String& _logfile)
{
	if(_logfile.Length() > 0)
	{
		logfile = System::Ioutils::TPath::GetFullPath(_logfile);
		if(FileExists(logfile)) DeleteFile(logfile);
	}
	else logfile = _logfile;
}

//---------------------------------------------------------------------------
void __fastcall Messager::Status(const String& message)
{
	Lock->Acquire();
	_s = &message;
	if(cur_thread) TThread::Synchronize(cur_thread, mainthreadStatus);
	else mainthreadStatus();
	Lock->Release();
}

//---------------------------------------------------------------------------
void __fastcall Messager::mainthreadStatus()
{
	FormMain->PanelСurConfPath->Caption = *_s;
	FormMain->PanelСurConfPath->Update();
}

//---------------------------------------------------------------------------
void __fastcall Messager::AddMessage(const String& message, const MessageState mstate, TStringList* param)
{
	String s;

	s = DateTimeToStr(Now(), FormatSettings);
	s += L" ";
	switch(mstate)
	{
		case msEmpty: s += L"<>"; break;
		case msSuccesfull: s += L"<ok>"; break;
		case msWarning: s += L"<warning>"; break;
		case msInfo: s += L"<info>"; break;
		case msError: s += L"<error>"; break;
		case msWait: s += L"<wait>"; break;
		case msHint: s += L"<hint>"; break;
		default: s += L"<>";
	}
	s += L" ";
	s += message;
	if(param) for(int i = 0; i < param->Count; i++)
	{
		s += L"\r\n\t";
		s += (*param)[i];
	}
	s += L"\r\n";

	Lock->Acquire();
	_s = &s;
	if(cur_thread) TThread::Synchronize(cur_thread, mainthreadAddMessage);
	else mainthreadAddMessage();
	Lock->Release();

}

//---------------------------------------------------------------------------
void __fastcall Messager::mainthreadAddMessage()
{
	TFileStream* log = NULL;
	TStreamWriter* sw;
	String s;

	if(MemoLog) MemoLog->Lines->Add(*_s);

	if(logfile.Length() > 0)
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
		s = *_s + L"\r\n";
		sw->Write(s);
		delete sw;
		delete log;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::showValue1C()
{
	Value1C* rootvalue;

	VirtualStringTreeValue1C->RootNodeCount = 0;
	if(!Container) return;
	rootvalue = Container->root;
	if(!rootvalue) return;

	VirtualStringTreeValue1C->RootNodeCount = 1;
	VirtualStringTreeValue1C->Invalidate();
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::showGenTypes()
{
	MetaTypeSet* ts;
	MetaType* t;
	int i, j;

	ts = Container->types;
	j = ts->number();
	AdvStringGridGenTypes->RowCount = j + 1;
	AdvStringGridGenTypes->Cells[0][0] = L"UID";
	AdvStringGridGenTypes->Cells[1][0] = L"Имя";

	for(i = 0; i < j; ++i)
	{
		t = ts->getType(i);
		AdvStringGridGenTypes->Cells[0][i + 1] = GUID_to_string(t->uid);
		AdvStringGridGenTypes->Cells[1][i + 1] = t->name;
	}

}

//---------------------------------------------------------------------------
void __fastcall TFormMain::showMeta()
{
	std::map<TGUID, MetaObject*>::const_iterator it;
	int i;

	AdvStringGridMeta->RowCount = Container->metamap.size() + 1;
	AdvStringGridMeta->Cells[0][0] = L"UID";
	AdvStringGridMeta->Cells[1][0] = L"Имя";

	for(it = Container->metamap.begin(), i = 1; it != Container->metamap.end(); ++it, ++i)
	{
		AdvStringGridMeta->Cells[0][i] = GUID_to_string(it->first);
		AdvStringGridMeta->Cells[1][i] = it->second->fullname;
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::ButtonLoadStorageClick(TObject *Sender)
{
	ConfigStorage* Storage;
	T_1CD* base;

	switch(AdvComboBoxVariant->ItemIndex)
	{
		case 0: //Файл конфигурации
			if(AdvFileNameEditConfigFile->Text.Length() == 0) return;
			MemoLog->Lines->Clear();

			Storage = new ConfigStorageCFFile(AdvFileNameEditConfigFile->Text);
			Container = new MetaContainer(Storage, CheckBoxUseExternal->Checked);
			if(!CheckBoxUseExternal->Checked) delete Storage;
			break;
		case 1: //Каталог конфигурации
			if(AdvDirectoryEditStorageDirectory->Text.Length() == 0) return;
			MemoLog->Lines->Clear();

			Storage = new ConfigStorageDirectory(AdvDirectoryEditStorageDirectory->Text);
			Container = new MetaContainer(Storage, CheckBoxUseExternal->Checked);
			if(!CheckBoxUseExternal->Checked) delete Storage;

			break;
		case 2: //Основная конфигурация
			if(AdvFileNameEditBaseFile->Text.Length() == 0) return;
			MemoLog->Lines->Clear();

			base = new T_1CD(AdvFileNameEditBaseFile->Text, msreg, false);
			if(!base->is_open())
			{
				delete base;
				return;
			}
			if(!base->table_config)
			{
				delete base;
				return;
			}
			if(!base->table_configsave)
			{
				delete base;
				return;
			}

			Storage = new ConfigStorageTableConfigSave(base->files_config, base->files_configsave, base);
			Container = new MetaContainer(Storage, CheckBoxUseExternal->Checked);
			if(!CheckBoxUseExternal->Checked) delete Storage;

			break;
		case 3: //Конфигурация базы данных
			if(AdvFileNameEditBaseFile->Text.Length() == 0) return;
			MemoLog->Lines->Clear();

			base = new T_1CD(AdvFileNameEditBaseFile->Text, msreg, false);
			if(!base->is_open())
			{
				delete base;
				return;
			}
			if(!base->table_config)
			{
				delete base;
				return;
			}

			Storage = new ConfigStorageTableConfig(base->files_config, base);
			Container = new MetaContainer(Storage, CheckBoxUseExternal->Checked);
			if(!CheckBoxUseExternal->Checked) delete Storage;

			break;
		default:
			return;
	}

	msreg->Status(L"");
	showValue1C();
	showGenTypes();
	showMeta();
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::VirtualStringTreeValue1CInitChildren(TBaseVirtualTree *Sender, PVirtualNode Node, DWORD &ChildCount)
{
	VirtualTreeData* d;
	Value1C* v;

	d = (VirtualTreeData*)(Sender->GetNodeData(Node));
	v = d->v;
	if(v)
	{
		if(v->kind == kv_extobj) ((Value1C_extobj*)v)->open();
		if(v->kind == kv_obj || v->kind == kv_metaobj || v->kind == kv_extobj) ChildCount = ((Value1C_obj*)v)->v_objpropv.size() + ((Value1C_obj*)v)->v_objcol.size();
	}

}

//---------------------------------------------------------------------------
String __fastcall TFormMain::getNodeName(PVirtualNode Node)
{
	VirtualTreeData* d;
	Value1C* v;
	Value1C_obj* vo;
	Value1C_obj* vstr;
	Value1C_obj* vcol; // Значение коллекции (ДеревоЗначений или ТаблицаЗначений)
	Value1C_obj* vmap; // Значение СоответствиеКолонокИЗначений
	Value1C_obj* vc; // Значение Колонки
	MetaProperty* p;
	String s = L"";
	MetaType* t;
	int vi, ci, nc; // индекс значения, индекс колонки, уникальный номер колонки
	unsigned int i;
	bool ok;

	d = (VirtualTreeData*)(VirtualStringTreeValue1C->GetNodeData(Node));
	v = d->v;
	p = d->p;
	if(p) s = p->getname(CheckBoxEnglish->Checked);
	else
	{
		if(d->q > 1)
		{
			vi = (int)Node->Index;
			vstr = (Value1C_obj*)v->parent;
			ok = false;
			for(i = 0; i < vstr->v_objcol.size(); ++i)
			{
				if(vstr->v_objcol[i] == v)
				{
					ok = true;
					vi = (int)i;
					break;
				}
			}
			if(ok)
			{
				if(d->q == 2) vcol = (Value1C_obj*)vstr->parent->parent;
				else //d->q == 3
				{
					for(vcol = (Value1C_obj*)vstr->parent->parent; vcol->type->getname(false).CompareIC(L"ДеревоЗначений"); vcol = (Value1C_obj*)vcol->parent->parent);
				}
				vmap = (Value1C_obj*)vcol->getproperty(L"СоответствиеКолонокИЗначений");
				ok = false;
				for(i = 0; i < vmap->v_objcol.size(); ++i)
				{
					vo = (Value1C_obj*)vmap->v_objcol[i];
					if(((Value1C_number*)vo->getproperty(L"ИндексЗначения"))->v_number == vi)
					{
						nc = ((Value1C_number*)vo->getproperty(L"УникальныйНомерКолонки"))->v_number;
						ok = true;
						break;
					}
				}
			}
			if(ok)
			{
				vc = (Value1C_obj*)vcol->getproperty(L"Колонки");
				ok = false;
				for(i = 0; i < vc->v_objcol.size(); ++i)
				{
					vo = (Value1C_obj*)vc->v_objcol[i];
					if(((Value1C_number*)vo->getproperty(L"УникальныйНомер"))->v_number == nc)
					{
						s = ((Value1C_string*)vo->getproperty(L"Имя"))->v_string;
						if(s.IsEmpty()) s = String(CheckBoxEnglish->Checked ? L"<Column_" : L"<Колонка_") + i + L">";
						ok = true;
						break;
					}
				}
			}
			if(!ok) s = String(L"[") + vi + L"]";
		}
		else if(v) s = v->presentation(CheckBoxEnglish->Checked);
	}
	if(s.IsEmpty()) s = String(L"[") + Node->Index + L"]";

	return s;
}

//---------------------------------------------------------------------------
String __fastcall TFormMain::getNodeValue(PVirtualNode Node)
{
	VirtualTreeData* d;
	Value1C* v;
	MetaProperty* p;
	String s = L"";

	d = (VirtualTreeData*)(VirtualStringTreeValue1C->GetNodeData(Node));
	v = d->v;
	p = d->p;

	if(p || d->q > 1)
	{
		if(v) s = v->presentation(CheckBoxEnglish->Checked);
	}
	else if(v) s = v->valuepresentation(CheckBoxEnglish->Checked);

	return s;
}

//---------------------------------------------------------------------------
String __fastcall TFormMain::getNodeType(PVirtualNode Node)
{
	VirtualTreeData* d;
	Value1C* v;
	String s = L"";

	d = (VirtualTreeData*)(VirtualStringTreeValue1C->GetNodeData(Node));
	v = d->v;

	if(v) if(v->type) s = v->type->getname(CheckBoxEnglish->Checked);

	return s;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::VirtualStringTreeValue1CGetText(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column,
		  TVSTTextType TextType, UnicodeString &CellText)
{
	CellText = L"";
	if(Column == 0) CellText = getNodeName(Node);
	else if(Column == 1) CellText = getNodeValue(Node);
	else if(Column == 2) CellText = getNodeType(Node);
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::VirtualStringTreeValue1CInitNode(TBaseVirtualTree *Sender, PVirtualNode ParentNode, PVirtualNode Node, TVirtualNodeInitStates &InitialStates)
{
	VirtualTreeData* d;
	Value1C* v;
	Value1C_obj* vo;
	unsigned int cp;
	String s;
	MetaType* t;

	if(!ParentNode)
	{
		d = (VirtualTreeData*)(Sender->GetNodeData(Node));
		v = Container->root->getproperty(L"Значение");
		d->v = v;
		d->p = NULL;
		d->q = 1;
	}
	else
	{
		d = (VirtualTreeData*)(Sender->GetNodeData(ParentNode));
		vo = (Value1C_obj*)(d->v);
		cp = vo->v_objpropv.size();
		d = (VirtualTreeData*)(Sender->GetNodeData(Node));
		if(Node->Index < cp)
		{
			d->v = vo->v_objpropv[Node->Index].second;
			d->p = vo->v_objpropv[Node->Index].first;
		}
		else
		{
			d->v = vo->v_objcol[Node->Index - cp];
			d->p = NULL;
		}
		d->q = 1;
		t = vo->type;
		if(t)
		{
			s = t->getname(false);
			if(s.CompareIC(L"СтрокаТаблицыЗначений") == 0)
			{
				d->q = 2;
			}
			else if(s.CompareIC(L"СтрокаДереваЗначений") == 0)
			{
				d->q = 3;
			}
		}
		v = d->v;
	}

	if(v)
	{
		if(v->kind == kv_obj || v->kind == kv_metaobj)
		{
			vo = (Value1C_obj*)v;
			if(vo->v_objpropv.size() + vo->v_objcol.size()) InitialStates << ivsHasChildren;
		}
		else if(v->kind == kv_extobj) InitialStates << ivsHasChildren;
	}


}

//---------------------------------------------------------------------------
void __fastcall TFormMain::VirtualStringTreeValue1CFocusChanged(TBaseVirtualTree *Sender, PVirtualNode Node, TColumnIndex Column)
{
	VirtualTreeData* d;
	Value1C* v;
	Value1C_binary* vb;
	MetaProperty* p;
	String CurPath;
	String s, ext;
	file_format ff;
	wchar_t temppath[MAX_PATH];
	wchar_t tempfile[MAX_PATH];
	TFileStream* fstr;
	IHTMLDocument2 *html;
	Variant var;
	IHTMLStyleSheet* stylesheet;

	TabSheetPicture->TabVisible = false;
	TabSheetHTML->TabVisible = false;
	MemoValue->SyntaxStyles = NULL;
	//TabSheetText->Visible = true;

	d = (VirtualTreeData*)(Sender->GetNodeData(Node));
	if(!d)
	{
		PanelСurConfPath->Caption = L"";
		MemoValue->Lines->Text = L"";
		PanelViewIn1C->Visible = false;
		return;
	}

	v = d->v;
	p = d->p;
	if(!v)
	{
		MemoValue->Lines->Text = L"";
		PanelViewIn1C->Visible = false;
	}
	else if(v->kind == kv_binary)
	{
		vb = (Value1C_binary*)v;
		switch(vb->v_binformat)
		{
			case eff_servalue:
			case eff_text:
			case eff_textdoc:
			case eff_string:
				vb->v_binary->Seek(0, soFromBeginning);
				MemoValue->Lines->LoadFromStream(vb->v_binary);
				break;
			case eff_htmldoc:
				vb->v_binary->Seek(0, soFromBeginning);
				MemoValue->Lines->LoadFromStream(vb->v_binary);
				MemoValue->SyntaxStyles = AdvHTMLMemoStyler1;

				if(aboutblank)
				{
					aboutblank = false;
					WebBrowserHTML->Navigate(L"about:blank");
					while(WebBrowserHTML->Busy) Application->DoApplicationIdle();
				}
				WebBrowserHTML->Document->QueryInterface<IHTMLDocument2>(&html);
				var = VarArrayCreate(OPENARRAY(int, (0, 0)), varVariant);
				var.PutElement(MemoValue->Lines->Text, 0);
				html->close();
				html->write(PSafeArray(TVarData(var).VArray));
				html->createStyleSheet(L"", 0, &stylesheet);
				stylesheet->put_cssText(L"body\r\n\
				{\r\n\
					margin: 0px 0px 0px 0px;\r\n\
					padding: 0px 0px 0px 0px;\r\n\
					background: #ffffff;\r\n\
					color: #000000;\r\n\
					font-family: Tahoma, Arial, Helvetica, sans-serif;\r\n\
					font-size: 70%;\r\n\
					width: 100%;\r\n\
				}\r\n\
				\r\n\
				table\r\n\
				{\r\n\
					font-size: 100%;\r\n\
				}\r\n\
				\r\n\
				p\r\n\
				{\r\n\
					margin-top: 0em;\r\n\
				}\r\n\
				\r\n\
				.V8SH_pagetitle     { display: none }\r\n\
				.V8SH_title     { background: gainsboro; font-weight: bold }\r\n\
				.V8SH_textarea     { padding: 5px 5px 5px 5px; }\r\n\
				.V8SH_heading     { text-decoration: underline; font-weight: bold }\r\n\
				.V8SH_chapter     { font-weight: bold }\r\n\
				.V8SH_rubric      { text-decoration: underline }\r\n\
				.V8SH_codesample  { font-family: Courier New; background: #e6e6e6 }\r\n\
				");
				html->Release();

				TabSheetHTML->TabVisible = true;
				PageControlFileFormats->ActivePage = TabSheetHTML;
				break;
			case eff_kd:
			case eff_mkd:
			case eff_xml:
			case eff_wsdl:
				vb->v_binary->Seek(0, soFromBeginning);
				MemoValue->Lines->LoadFromStream(vb->v_binary, TEncoding::UTF8);
				MemoValue->SyntaxStyles = AdvXMLMemoStyler1;
				break;
			case eff_picture:
				MemoValue->Lines->Text = CheckBoxEnglish->Checked ? L"<Picture>" : L"<Картинка>";
				ff = get_file_format(vb->v_binary);
				ext = L"";
				switch(ff)
				{
					case ff_gif:
						ext = L"gif";
						break;
					case ff_pcx:
						ext = L"pcx";
						break;
					case ff_bmp:
						ext = L"bmp";
						break;
					case ff_jpg:
						ext = L"jpg";
						break;
					case ff_png:
						ext = L"png";
						break;
					case ff_tiff:
						ext = L"tif";
						break;
					case ff_ico:
						ext = L"ico";
						break;
					case ff_wmf:
						ext = L"wmf";
						break;
					case ff_emf:
						ext = L"emf";
						break;
				}
				if(!ext.IsEmpty())
				{
					GetTempPath(MAX_PATH - 1, temppath);
					GetTempFileName(temppath, L"mtr", 0, tempfile);
					s = tempfile;
					s += L".";
					s += ext;
					fstr = new TFileStream(s, fmCreate);
					fstr->CopyFrom(vb->v_binary, 0);
					delete fstr;
					AdvReflectionImagePicture->Picture->LoadFromFile(s);
					DeleteFile(s);
					TabSheetPicture->TabVisible = true;
					PageControlFileFormats->ActivePage = TabSheetPicture;
				}
				break;
			default:
				MemoValue->Lines->Text = CheckBoxEnglish->Checked ? L"<BinaryData>" : L"<ДвоичныеДанные>";
		}
		PanelViewIn1C->Visible = !s_1cv8fv.IsEmpty();
	}
	else
	{
		if(p) MemoValue->Lines->Text = v->presentation(CheckBoxEnglish->Checked);
		else MemoValue->Lines->Text = v->valuepresentation(CheckBoxEnglish->Checked);
		PanelViewIn1C->Visible = false;
	}

	CurPath = L"";
	while(Node && Node != Sender->RootNode)
	{
		s = getNodeName(Node);
		CurPath = s + (CurPath.IsEmpty() ? L"" : (CurPath[1] == L'[' ? L"" : L".")) + CurPath;
		Node = Node->Parent;
	}
	PanelСurConfPath->Caption = CurPath;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::VirtualStringTreeValue1CGetImageIndex(TBaseVirtualTree *Sender, PVirtualNode Node, TVTImageKind Kind,
		  TColumnIndex Column, bool &Ghosted, int &ImageIndex)
{
	VirtualTreeData* d;
	MetaType* t;

	if(Column || !CheckBoxUseImages->Checked)
	{
		ImageIndex = -1;
		return;
	}
	d = (VirtualTreeData*)(Sender->GetNodeData(Node));
	t = d->v->type;
	if(t) ImageIndex = t->imageindex;
	else ImageIndex = -1;

	if(ImageIndex == 0) ImageIndex = -1;
}

//---------------------------------------------------------------------------

void __fastcall TFormMain::CheckBoxUseImagesClick(TObject *Sender)
{
	VirtualStringTreeValue1C->Invalidate();
	if(VirtualStringTreeValue1C->FocusedNode) VirtualStringTreeValue1CFocusChanged(VirtualStringTreeValue1C, VirtualStringTreeValue1C->FocusedNode, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonViewIn1CClick(TObject *Sender)
{
	VirtualTreeData* d;
	Value1C* v;
	Value1C_binary* vb;
	String s, ext;
	wchar_t temppath[MAX_PATH];
	wchar_t tempfile[MAX_PATH];
	TFileStream* fstr;
	STARTUPINFO si;
	PROCESS_INFORMATION pi;
	file_format ff;

	if(s_1cv8fv.IsEmpty()) return;
	if(VirtualStringTreeValue1C->FocusedNode)
	{
		d = (VirtualTreeData*)(VirtualStringTreeValue1C->GetNodeData(VirtualStringTreeValue1C->FocusedNode));
		if(!d) return;

		v = d->v;
		if(v->kind == kv_binary)
		{
			vb = (Value1C_binary*)v;
			ext = NULL;
			switch(vb->v_binformat)
			{
				case eff_servalue:
				case eff_text:
				case eff_textdoc:
				case eff_string:
					ext = L"txt";
					break;
				case eff_htmldoc:
					ext = L"html";
					break;
				case eff_tabdoc:
					ext = L"mxl";
					break;
				case eff_binary:
					ext = L"bin";
					break;
				case eff_activedoc:
					ext = L"ad";
					break;
				case eff_geo:
					ext = L"geo";
					break;
				case eff_kd:
				case eff_mkd:
				case eff_xml:
					ext = L"xml";
					break;
				case eff_wsdl:
					ext = L"wsdl";
					break;
				case eff_graf:
					ext = L"grs";
					break;
				case eff_picture:
					ff = get_file_format(vb->v_binary);
					switch(ff)
					{
						case ff_unknown:
							MessageBox(Handle, L"Не удалось определить формат файла картинки", L"Ошибка!", MB_OK|MB_ICONSTOP);
							return;
						case ff_gif:
							ext = L"gif";
							break;
						case ff_utf8: // Очень странно встретить в картинке текст!
							ext = L"txt";
							break;
						case ff_pcx:
							ext = L"pcx";
							break;
						case ff_bmp:
							ext = L"bmp";
							break;
						case ff_jpg:
							ext = L"jpg";
							break;
						case ff_png:
							ext = L"png";
							break;
						case ff_tiff:
							ext = L"tif";
							break;
						case ff_ico:
							ext = L"ico";
							break;
						case ff_wmf:
							ext = L"wmf";
							break;
						case ff_emf:
							ext = L"emf";
							break;
						default:
							MessageBox(Handle, L"Ошибка определения формата файла картинки", L"Ошибка!", MB_OK|MB_ICONSTOP);
							return;
					}
					break;
				default:
					MessageBox(Handle, L"Неизвестный формат файла", L"Ошибка!", MB_OK|MB_ICONSTOP);
					return;
			}
			GetTempPath(MAX_PATH - 1, temppath);
			GetTempFileName(temppath, L"mtr", 0, tempfile);
			s = tempfile;
			s += L".";
			s += ext;
			fstr = new TFileStream(s, fmCreate);
			//v->v_binary->Seek(0, soFromBeginning);
			fstr->CopyFrom(vb->v_binary, 0);
			delete fstr;
			s = String(L"\"") + s_1cv8fv + L"\" \"" + s + L"\"";

			ZeroMemory( &si, sizeof(si) );
			si.cb = sizeof(si);
			ZeroMemory( &pi, sizeof(pi) );
			CreateProcess(NULL, s.c_str(), NULL, NULL, false, 0, NULL, NULL, &si, &pi);

		}
	}
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::AdvComboBoxVariantChange(TObject *Sender)
{
	AdvDirectoryEditStorageDirectory->Visible = false;
	AdvFileNameEditConfigFile->Visible = false;
	AdvFileNameEditBaseFile->Visible = false;

	switch(AdvComboBoxVariant->ItemIndex)
	{
		case 0: //Файл конфигурации
			AdvFileNameEditConfigFile->Visible = true;
			break;
		case 1: //Каталог конфигурации
			AdvDirectoryEditStorageDirectory->Visible = true;
			break;
		case 2: //Основная конфигурация
		case 3: //Конфигурация базы данных
			AdvFileNameEditBaseFile->Visible = true;
			break;
	}

}

//---------------------------------------------------------------------------

void __fastcall TFormMain::CheckBoxLogToFileClick(TObject *Sender)
{
	if(CheckBoxLogToFile->Checked) mess->setlogfile(AdvFileNameEditLogFile->Text);
	else mess->setlogfile(L"");
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonExportClick(TObject *Sender)
{
	if(!Container) return;
	if(AdvDirectoryEditExport->Text.IsEmpty()) return;

	Container->Export(AdvDirectoryEditExport->Text, CheckBoxEnglish->Checked, AdvEditExportThreadCount->Text.ToIntDef(0));
}
//---------------------------------------------------------------------------

