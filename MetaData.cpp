//---------------------------------------------------------------------------

#include <typeinfo.h>

#pragma hdrstop

#include "MetaData.h"
#include "Base64.h"
#include "Parse_tree.h"
#include "MessageRegistration.h"
#include "FileFormat.h"
#include "CRC32.h"

//---------------------------------------------------------------------------

//#pragma package(smart_init)


//---------------------------------------------------------------------------
TGUID EmptyUID = {0,0,0,{0,0,0,0,0,0,0,0}};

MetaTypeSet* MetaTypeSet::staticTypes = NULL; // Набор статических типов 1С

// Пустой тип
MetaType* MetaTypeSet::mt_empty;
// Примитивные типы
MetaType* MetaTypeSet::mt_string;
MetaType* MetaTypeSet::mt_number;
MetaType* MetaTypeSet::mt_bool;
MetaType* MetaTypeSet::mt_date;
MetaType* MetaTypeSet::mt_undef;
MetaType* MetaTypeSet::mt_null;
MetaType* MetaTypeSet::mt_type;
// УникальныйИдентификатор
MetaType* MetaTypeSet::mt_uid;
// ОписаниеТипаВнутр
MetaType* MetaTypeSet::mt_typedescrinternal;
// Двоичные данные
MetaType* MetaTypeSet::mt_binarydata;
// Произвольный тип
MetaType* MetaTypeSet::mt_arbitrary;
// Корневые типы
MetaType* MetaTypeSet::mt_container;
MetaType* MetaTypeSet::mt_config;
// Псевдо-тип Стандартный атрибут
MetaType* MetaTypeSet::mt_standart_attribute;
// Псевдо-тип Стандартная табличная часть
MetaType* MetaTypeSet::mt_standart_tabular_section;
// Значения частей даты для квалификатора даты
MetaValue* MetaTypeSet::mv_datefractionsdate;
MetaValue* MetaTypeSet::mv_datefractionstime;
// Тип ЧастиДаты
MetaType* MetaTypeSet::mt_datefractions;
// Свойство ЧастиДаты типа КвалификаторыДаты
MetaProperty* MetaTypeSet::mp_datefractions;
// ОбъектМетаданных: ТабличнаяЧасть
MetaType* MetaTypeSet::mt_tabularsection;
// ОбъектМетаданных: Реквизит
MetaType* MetaTypeSet::mt_attribute;
// ОбъектМетаданныхСсылка
MetaType* MetaTypeSet::mt_metaobjref;
// ОбъектМетаданныхСсылкаВнутр
MetaType* MetaTypeSet::mt_metarefint;
// ТабличнаяЧасть
MetaType* MetaTypeSet::mt_tabsection;
// МетаСсылка
MetaType* MetaTypeSet::mt_metaref;

std::map<String, ClassParameter*> ClassParameter::map;
std::map<TGUID, Class*> Class::map;

std::map<TGUID, MetaObject*> MetaObject::map;
std::map<String, MetaObject*> MetaObject::smap;

extern MessageRegistrator* msreg;

const int maxStringLength = 4096;

wchar_t Value1C::indentstring[31] = L"\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
char Value1C_date::emptydate[7] = {0,1,1,1,0,0,0};

std::vector<MetaStandartTabularSection*> MetaStandartTabularSection::list;

std::map<TGUID, MetaRight*> MetaRight::map;
std::map<String, MetaRight*> MetaRight::smap;

TGUID MetaContainer::sig_standart_attribute; // сигнатура стандартного реквизита
TGUID MetaContainer::sig_standart_table_sec; // сигнатура стандартной табличной части
TGUID MetaContainer::sig_ext_dimension; // сигнатура стандартного реквизита Субконто
TGUID MetaContainer::sig_ext_dimension_type; // сигнатура стандартного реквизита ВидСубконто

__declspec(thread) TThread* cur_thread = NULL;
__declspec(thread) Value1C_obj_ExportThread* cur_export_thread = NULL;
__declspec(thread) std::vector<UninitValue1C>* puninitvalues = NULL;

//********************************************************
// Перегрузка операторов

bool __fastcall operator<(const TGUID& l, const TGUID& r)
{
	return memcmp(&l, &r, sizeof(TGUID)) < 0;
}

//********************************************************
// Функции

#define error if(msreg) msreg->AddError

//---------------------------------------------------------------------------

Version1C stringtover1C(const String& s)
{
	if(s.IsEmpty()) return v1C_min;
	if(s == L"8.0") return v1C_8_0;
	if(s == L"8.1") return v1C_8_1;
	if(s == L"8.2") return v1C_8_2;
	if(s == L"8.2.14") return v1C_8_2_14;
	if(s == L"8.3.1") return v1C_8_3_1;
	if(s == L"8.3.2") return v1C_8_3_2;
	if(s == L"8.3.3") return v1C_8_3_3;
	if(s == L"8.3.4") return v1C_8_3_4;
	if(s == L"8.3.5") return v1C_8_3_5;
	if(s == L"8.3.6") return v1C_8_3_6;
	return v1C_min;
}

//---------------------------------------------------------------------------
String __fastcall KindOfValue1C_presantation(KindOfValue1C kv)
{
	switch(kv)
	{
		case kv_unknown: return L"Неинициализированное значение";
		case kv_bool: return L"Булево";
		case kv_string: return L"Строка";
		case kv_number: return L"Число";
		case kv_number_exp: return L"Число с плавающей запятой";
		case kv_date: return L"Дата";
		case kv_null: return L"Null";
		case kv_undef: return L"Неопределено";
		case kv_type: return L"Тип";
		case kv_uid: return L"Уникальный идентификатор";
		case kv_enum: return L"Системное перечисление";
		case kv_stdattr: return L"Стандартный реквизит";
		case kv_stdtabsec: return L"Стандартная табличная часть";
		case kv_obj: return L"Объект";
		case kv_metaobj: return L"Объект, являющийся объектом метаданных";
		case kv_refobj: return L"Ссылка на объект, являющийся объектом метаданных";
		case kv_refpre: return L"Ссылка на предопределенный элемент";
		case kv_right: return L"Право";
		case kv_binary: return L"Двоичные данные";
	}
	return L"?Неизвестный тип значения 1С";
}

//---------------------------------------------------------------------------
void __fastcall LoadValidValues(tree* tr, std::vector<VarValidValue>& validvalues, bool haveglobal = false)
{
	tree* tt;
	int i, count;
	VarValidValue vvv;
	String s;

	tt = tr->get_first();
	count = tt->get_value().ToIntDef(0);
	for(i = 0; i < count; ++i)
	{
		tt = tt->get_next();
		vvv.value = tt->get_value().ToIntDef(0);
		tt = tt->get_next();
		s = tt->get_value();
		if(s.IsEmpty()) vvv.ver1C = v1C_min;
		else
		{
			vvv.ver1C = stringtover1C(s);
			if(vvv.ver1C == v1C_min)
			{
				error(L"Ошибка загрузки статических типов. Некорректное значение версии 1C в допустимых значениях переменной дерева сериализации"
					//, L"Переменная", fname
					, L"Значение", s);
			}
		}
		if(haveglobal)
		{
			tt = tt->get_next();
			vvv.globalvalue = tt->get_value().ToIntDef(0);
		}
		validvalues.push_back(vvv);
	}
}

//---------------------------------------------------------------------------

//********************************************************
// Класс MetaValue

//---------------------------------------------------------------------------
__fastcall MetaValue::MetaValue(MetaType* _owner, tree* tr) : owner(_owner)
{
	tree* tt;

	tt = tr->get_first();
	fname = tt->get_value();
	tt = tt->get_next();
	fename = tt->get_value();
	tt = tt->get_next();
	fvalue = tt->get_value().ToIntDef(0);
	tt = tt->get_next();
	string_to_GUID(tt->get_value(), &fvalueUID);
}

//********************************************************
// Класс MetaProperty

//---------------------------------------------------------------------------
__fastcall MetaProperty::MetaProperty(MetaType* _owner, tree* tr) : owner(_owner)
{
	tree* tt;
	tree* t;
	int num, i;
	TGUID guid;

	tt = tr->get_first();
	fname = tt->get_value();
	tt = tt->get_next();
	fename = tt->get_value();
	tt = tt->get_next();
	fpredefined = tt->get_value().Compare(L"1") == 0 ? true : false;
	tt = tt->get_next();
	fexporttype = (ExportType)(tt->get_value().ToIntDef(0));
	tt = tt->get_next();
	string_to_GUID(tt->get_value(), &guid);
	f_class = Class::getclass(guid);

	// Типы
	tt = tt->get_next();
	t = tt->get_first();
	num = t->get_value().ToIntDef(0);
	for(i = 0; i < num; ++i)
	{
		t = t->get_next();
		fstypes.push_back(t->get_value());
	}

	//fdefaultvalue = NULL;
	defaultvaluetype = dvt_novalue;
}

//---------------------------------------------------------------------------
__fastcall MetaProperty::~MetaProperty()
{
	if(defaultvaluetype == dvt_string) delete dv_string;
}

//---------------------------------------------------------------------------
void __fastcall MetaProperty::filltypes()
{
	std::vector<String>::iterator i;
	ftypes.erase(ftypes.begin(), ftypes.end());
	for(i = fstypes.begin(); i != fstypes.end(); ++i) ftypes.push_back(owner->TypeSet->getTypeByName(*i));
}

////---------------------------------------------------------------------------
//void __fastcall MetaProperty::setdefaultvalue(MetaType* typ, const String& valuename)
//{
//	fdefaultvalue = typ->getValue(valuename);
//}
////---------------------------------------------------------------------------

//********************************************************
// Класс MetaGeneratedType

//---------------------------------------------------------------------------
__fastcall MetaGeneratedType::MetaGeneratedType(tree* tr)
{
	tree* tt;

	tt = tr->get_first();
	fname = tt->get_value();
	tt = tt->get_next();
	fename = tt->get_value();
	tt = tt->get_next();
	fwoprefix = tt->get_value().Compare(L"1") == 0 ? true : false;

}

//---------------------------------------------------------------------------

//********************************************************
// Класс MetaRight

//---------------------------------------------------------------------------
__fastcall MetaRight::MetaRight(tree* tr)
{
	tree* tt;

	tt = tr->get_first();
	fname = tt->get_value();
	tt = tt->get_next();
	fename = tt->get_value();
	tt = tt->get_next();
	string_to_GUID(tt->get_value(), &fuid);
	tt = tt->get_next();
	fver1C = stringtover1C(tt->get_value());
	if(fver1C == v1C_min)
	{
		error(L"Ошибка загрузки статических типов. Некорректное значение версии 1C в описании права"
			, L"Право", fname
			, L"Значение", tt->get_value());
	}

	map[fuid] = this;
	smap[fname] = this;
	smap[fename] = this;

}

//---------------------------------------------------------------------------
MetaRight* __fastcall MetaRight::getright(const TGUID& _uid)
{
	std::map<TGUID, MetaRight*>::iterator i;
	i = map.find(_uid);
	if(i == map.end()) return NULL;
	return i->second;
}

//---------------------------------------------------------------------------
MetaRight* __fastcall MetaRight::getright(const String& _name)
{
	std::map<String, MetaRight*>::iterator i;
	i = smap.find(_name);
	if(i == smap.end()) return NULL;
	return i->second;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс MetaStandartAttribute

//---------------------------------------------------------------------------
__fastcall MetaStandartAttribute::MetaStandartAttribute(tree* tr)
{
	tree* tt;

	tt = tr->get_first();
	fname = tt->get_value();
	tt = tt->get_next();
	fename = tt->get_value();
	tt = tt->get_next();
	fvalue = tt->get_value().ToIntDef(0);
	tt = tt->get_next();
	fcount = tt->get_value().Compare(L"1") == 0 ? true : false;
	tt = tt->get_next();
	fvaluemax = tt->get_value().ToIntDef(0);
	tt = tt->get_next();
	string_to_GUID(tt->get_value(), &fuid);
}

//---------------------------------------------------------------------------

//********************************************************
// Класс MetaStandartTabularSection

//---------------------------------------------------------------------------
__fastcall MetaStandartTabularSection::MetaStandartTabularSection(tree* tr)
{
	tree* tt;

	tt = tr->get_first();
	fname = tt->get_value();
	tt = tt->get_next();
	fename = tt->get_value();
	tt = tt->get_next();
	fvalue = tt->get_value().ToIntDef(0);
	tt = tt->get_next();
	string_to_GUID(tt->get_value(), &class_uid);

	list.push_back(this);
}

//---------------------------------------------------------------------------

//********************************************************
// Класс ClassParameter

//---------------------------------------------------------------------------
__fastcall ClassParameter::ClassParameter(tree* tr)
{
	tree* tt;

	tt = tr->get_first();
	fname = tt->get_value();
	map[fname] = this;
}

//---------------------------------------------------------------------------

ClassParameter* __fastcall ClassParameter::getparam(const String& paramname)
{
	ClassParameter* p;
	std::map<String, ClassParameter*>::iterator i;

	i = map.find(paramname);
	if(i == map.end()) return NULL;
	return i->second;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Class

//---------------------------------------------------------------------------
__fastcall Class::Class(tree* tr)
{
	tree* tt;
	tree* t;
	int i, count, j;
	String s;
	ClassParameter* p;

	tt = tr->get_first();
	string_to_GUID(tt->get_value(), &fuid);
	tt = tt->get_next();
	LoadValidValues(tt, fvervalidvalues);

	tt = tt->get_next();
	t = tt->get_first();
	count = t->get_value().ToIntDef(0);
	for(i = 0; i < count; ++i)
	{
		t = t->get_next();
		s = t->get_value();
		t = t->get_next();
		p = ClassParameter::getparam(s);
		if(!p)
		{
			error(L"Ошибка загрузки статических типов. Некорректное имя параметра класса"
				, L"Параметр", s);
		}
		else
		{
			j = t->get_value().ToIntDef(0);
			fparamvalues[p] = j;
		}
	}

	// Стандартные реквизиты
	tt = tt->get_next();
	t = tt->get_first();
	count = t->get_value().ToIntDef(0);
	for(i = 0; i < count; ++i)
	{
		t = t->get_next();
		fstandartattributes.push_back(new MetaStandartAttribute(t));
	}

	// Стандартные табличные части
	tt = tt->get_next();
	t = tt->get_first();
	count = t->get_value().ToIntDef(0);
	for(i = 0; i < count; ++i)
	{
		t = t->get_next();
		fstandarttabularsections.push_back(new MetaStandartTabularSection(t));
	}

	map[fuid] = this;
}

//---------------------------------------------------------------------------
__fastcall Class::~Class()
{
	unsigned int j;

	for(j = 0; j < fstandartattributes.size(); ++j) delete fstandartattributes[j];
}

//---------------------------------------------------------------------------
int __fastcall Class::getparamvalue(ClassParameter* p)
{
	std::map<ClassParameter*, int>::iterator i;

	i = fparamvalues.find(p);
	if(i == fparamvalues.end()) return -1;
	return i->second;
}

//---------------------------------------------------------------------------
Class* __fastcall Class::getclass(const TGUID& id)
{
	std::map<TGUID, Class*>::iterator i;

	i = map.find(id);
	if(i == map.end()) return NULL;
	return i->second;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс ClassItem

//---------------------------------------------------------------------------
int __fastcall ClassItem::setversion(int v)
{
	fversion = v;
	fversionisset = true;
	return v;
}

//---------------------------------------------------------------------------
int __fastcall ClassItem::getversion()
{
	if(fversionisset) return fversion;
	error(L"Ошибка формата потока 117. Ошибка получения значения переменной ВерсияКласса. Значение не установлено.");
	return -1;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс ExternalFile

//---------------------------------------------------------------------------
__fastcall ExternalFile::ExternalFile(MetaType* _owner, tree* tr)
{
	tree* tt;
	String sval;
	String str;
	unsigned int i;
	MetaType* typ;

	owner = _owner;

	tt = tr->get_first();
	prop = owner->getProperty(tt->get_value());
	if(!prop)
	{
		error(L"Ошибка загрузки статических типов. Некорректное имя свойства дерева сериализации в описании внешнего файла"
			, L"Тип", owner->name
			, L"Имя свойства", tt->get_value());
	}
	tt = tt->get_next();
	relativepath = tt->get_value().Compare(L"1") == 0 ? true : false;
	tt = tt->get_next();
	name = tt->get_value();
	tt = tt->get_next();
	ext = tt->get_value();
	tt = tt->get_next();
	catalog = tt->get_value().Compare(L"1") == 0 ? true : false;
	tt = tt->get_next();
	filename = tt->get_value();
	tt = tt->get_next();
	format = (ExternalFileFormat)(tt->get_value().ToIntDef(0));
	if(format <= eff_min || format >= eff_max)
	{
		error(L"Ошибка загрузки статических типов. Некорректное значение формата внешнего файла"
			, L"Тип", owner->name
			, L"Значение", tt->get_value());
	}
	tt = tt->get_next();
	str = tt->get_value();
	if(str.IsEmpty()) type = NULL;
	else
	{
		type = MetaTypeSet::staticTypes->getTypeByName(str);
		if(!type) error(L"Ошибка загрузки статических типов. Некорректное имя типа внешнего файла"
			, L"Тип", owner->name
			, L"Имя типа", str);
	}
	tt = tt->get_next();
	optional = tt->get_value().Compare(L"1") == 0 ? true : false;
	tt = tt->get_next();
	ver1C = stringtover1C(tt->get_value());
	if(ver1C == v1C_min)
	{
		error(L"Ошибка загрузки статических типов. Некорректное значение версии 1C в описании внешнего файла"
			, L"Тип", owner->name
			, L"Значение", tt->get_value());
	}
	tt = tt->get_next();
	havecondition = tt->get_value().Compare(L"1") == 0 ? true : false;
	if(havecondition)
	{
		tt = tt->get_next();
		condition = (SerializationTreeCondition)(tt->get_value().ToIntDef(0));
		if(condition <= stc_min || condition >= stc_max)
		{
			error(L"Ошибка загрузки статических типов. Некорректный вид условия в описании внешнего файла"
				, L"Тип", owner->name
				, L"Вид условия", tt->get_value());
		}

		tt = tt->get_next();
		sval = tt->get_value();


		str = sval.SubString(1, 1);
		if(str.Compare(L"%") == 0)
		{
			typeval1 = stv_var;
			str1 = sval.SubString(2, sval.Length() - 1);
		}
		else if(str.Compare(L".") == 0)
		{
			sval = sval.SubString(2, sval.Length() - 1);
			typeval1 = stv_prop;
			prop1 = owner->getProperty(sval);
			if(!prop1)
			{
				error(L"Ошибка загрузки статических типов. Некорректное имя свойства в описании внешнего файла"
					, L"Тип", owner->name
					, L"Имя свойства", sval);
			}
		}
		else if(str.Compare(L"*") == 0)
		{
			sval = sval.SubString(2, sval.Length() - 1);
			typeval1 = stv_value;
			val1 = NULL;
			i = sval.Pos(L".");
			if(i)
			{
				str = sval.SubString(1, i - 1);
				typ = MetaTypeSet::staticTypes->getTypeByName(str);
				if(typ)
				{
					str = sval.SubString(i + 1, sval.Length() - i);
					val1 = typ->getValue(str);
				}
			}

			if(!val1)
			{
				error(L"Ошибка загрузки статических типов. Некорректное значение в описании внешнего файла"
					, L"Тип", owner->name
					, L"Значение", sval);
			}
		}
		else if(str.Compare(L"v") == 0)
		{
			sval = sval.SubString(2, sval.Length() - 1);
			typeval1 = stv_vercon;
			if(sval == L"2.0") vercon1 = cv_2_0;
			else if(sval == L"5.0") vercon1 = cv_5_0;
			else if(sval == L"6.0") vercon1 = cv_6_0;
			else if(sval == L"106.0") vercon1 = cv_106_0;
			else if(sval == L"200.0") vercon1 = cv_200_0;
			else if(sval == L"202.2") vercon1 = cv_202_2;
			else if(sval == L"216.0") vercon1 = cv_216_0;
			else
			{
				error(L"Ошибка загрузки статических типов. Некорректное значение версии контейнера в описании внешнего файла"
					, L"Тип", owner->name
					, L"Значение", sval);
			}
		}
		else if(str.Compare(L":") == 0)
		{
			sval = sval.SubString(2, sval.Length() - 1);
			typeval1 = stv_ver1C;
			ver1C1 = stringtover1C(sval);
			if(ver1C1 == v1C_min)
			{
				error(L"Ошибка загрузки статических типов. Некорректное значение версии 1C в описании внешнего файла"
					, L"Тип", owner->name
					, L"Значение", sval);
			}
		}
		else if(str.Compare(L"&") == 0)
		{
			sval = sval.SubString(2, sval.Length() - 1);
			typeval1 = stv_classpar;
			classpar1 = ClassParameter::getparam(sval);
			if(!classpar1)
			{
				error(L"Ошибка загрузки статических типов. Некорректное имя параметра класса в описании внешнего файла"
					, L"Тип", owner->name
					, L"Значение", sval);
			}
		}
		else if(str.Compare(L"~") == 0)
		{
			typeval1 = stv_globalvar;
			str1 = sval.SubString(2, sval.Length() - 1);
		}
		else
		{
			str = sval.SubString(1, 2);
			if(str.CompareIC(L"S'") == 0)
			{
				typeval1 = stv_string;
				str1 = sval.SubString(3, sval.Length() - 2);
			}
			else if(str.CompareIC(L"N'") == 0)
			{
				typeval1 = stv_number;
				num1 = sval.SubString(3, sval.Length() - 2).ToIntDef(0);
			}
			else if(str.CompareIC(L"U'") == 0)
			{
				typeval1 = stv_uid;
				if(!string_to_GUID(sval.SubString(3, sval.Length() - 2), &uid1))
				{
					error(L"Ошибка загрузки статических типов. Ошибка преобразования УИД в условии в описании внешнего файла"
						, L"Тип", owner->name
						, L"УИД", sval);
				}
			}
			else
			{
				error(L"Ошибка загрузки статических типов. Ошибка разбора значения в условии в описании внешнего файла"
					, L"Тип", owner->name
					, L"Значение", sval);
			}
		}

		tt = tt->get_next();
		sval = tt->get_value();
		str = sval.SubString(1, 1);
		if(str.Compare(L"%") == 0)
		{
			typeval2 = stv_var;
			str2 = sval.SubString(2, sval.Length() - 1);
		}
		else if(str.Compare(L".") == 0)
		{
			sval = sval.SubString(2, sval.Length() - 1);
			typeval2 = stv_prop;
			prop2 = owner->getProperty(sval);
			if(!prop2)
			{
				error(L"Ошибка загрузки статических типов. Некорректное имя свойства в описании внешнего файла"
					, L"Тип", owner->name
					, L"Имя свойства", sval);
			}
		}
		else if(str.Compare(L"*") == 0)
		{
			sval = sval.SubString(2, sval.Length() - 1);
			typeval2 = stv_value;
			val2 = NULL;
			i = sval.Pos(L".");
			if(i)
			{
				str = sval.SubString(1, i - 1);
				typ = MetaTypeSet::staticTypes->getTypeByName(str);
				if(typ)
				{
					str = sval.SubString(i + 1, sval.Length() - i);
					val2 = typ->getValue(str);
				}
			}

			if(!val2)
			{
				error(L"Ошибка загрузки статических типов. Некорректное значение в описании внешнего файла"
					, L"Тип", owner->name
					, L"Значение", sval);
			}
		}
		else if(str.Compare(L"v") == 0)
		{
			sval = sval.SubString(2, sval.Length() - 1);
			typeval2 = stv_vercon;
			if(sval == L"2.0") vercon2 = cv_2_0;
			else if(sval == L"5.0") vercon2 = cv_5_0;
			else if(sval == L"6.0") vercon2 = cv_6_0;
			else if(sval == L"106.0") vercon2 = cv_106_0;
			else if(sval == L"200.0") vercon2 = cv_200_0;
			else if(sval == L"202.2") vercon2 = cv_202_2;
			else if(sval == L"216.0") vercon2 = cv_216_0;
			else
			{
				error(L"Ошибка загрузки статических типов. Некорректное значение версии контейнера в описании внешнего файла"
					, L"Тип", owner->name
					, L"Значение", sval);
			}
		}
		else if(str.Compare(L":") == 0)
		{
			sval = sval.SubString(2, sval.Length() - 1);
			typeval2 = stv_ver1C;
			ver1C2 = stringtover1C(sval);
			if(ver1C2 == v1C_min)
			{
				error(L"Ошибка загрузки статических типов. Некорректное значение версии 1C в описании внешнего файла"
					, L"Тип", owner->name
					, L"Значение", sval);
			}
		}
		else if(str.Compare(L"&") == 0)
		{
			sval = sval.SubString(2, sval.Length() - 1);
			typeval2 = stv_classpar;
			classpar2 = ClassParameter::getparam(sval);
			if(!classpar2)
			{
				error(L"Ошибка загрузки статических типов. Некорректное имя параметра класса в описании внешнего файла"
					, L"Тип", owner->name
					, L"Значение", sval);
			}
		}
		else if(str.Compare(L"~") == 0)
		{
			typeval2 = stv_globalvar;
			str2 = sval.SubString(2, sval.Length() - 1);
		}
		else
		{
			str = sval.SubString(1, 2);
			if(str.CompareIC(L"S'") == 0)
			{
				typeval2 = stv_string;
				str2 = sval.SubString(3, sval.Length() - 2);
			}
			else if(str.CompareIC(L"N'") == 0)
			{
				typeval2 = stv_number;
				num2 = sval.SubString(3, sval.Length() - 2).ToIntDef(0);
			}
			else if(str.CompareIC(L"U'") == 0)
			{
				typeval2 = stv_uid;
				if(!string_to_GUID(sval.SubString(3, sval.Length() - 2), &uid2))
				{
					error(L"Ошибка загрузки статических типов. Ошибка преобразования УИД в условии в описании внешнего файла"
						, L"Тип", owner->name
						, L"УИД", sval);
				}
			}
			else
			{
				error(L"Ошибка загрузки статических типов. Ошибка разбора значения в условии в описании внешнего файла"
					, L"Тип", owner->name
					, L"Значение", sval);
			}
		}

	}

}

//---------------------------------------------------------------------------

//********************************************************
// Класс SerializationTreeVar

//---------------------------------------------------------------------------
__fastcall SerializationTreeVar::SerializationTreeVar(tree* tr)
{
	tree* tt;

	tt = tr->get_first();
	fname = tt->get_value();
	tt = tt->get_next();
	fcolcount = tt->get_value().Compare(L"1") == 0 ? true : false;
	tt = tt->get_next();
	fisglobal = tt->get_value().Compare(L"1") == 0 ? true : false;
	tt = tt->get_next();
	fisfix = tt->get_value().Compare(L"1") == 0 ? true : false;
	tt = tt->get_next();
	ffixvalue = tt->get_value().ToIntDef(0);
	tt = tt->get_next();
	LoadValidValues(tt, fvalidvalues, true);
}

//---------------------------------------------------------------------------

//********************************************************
// Класс SerializationTreeNode

//---------------------------------------------------------------------------
__fastcall SerializationTreeNode::~SerializationTreeNode()
{
	SerializationTreeNode* sn;
	while(first)
	{
		sn = first->next;
		delete first;
		first = sn;
	}
}

//---------------------------------------------------------------------------
__fastcall SerializationTreeNode::SerializationTreeNode(MetaType* _owner, tree* tr, SerializationTreeNode* _parent, unsigned int _index)
{
	unsigned int i;
	String str;
	String stval;
	String sval1;
	String sval2;
	MetaType* typ;

	owner = _owner;
	parent = _parent;
	index = _index;

	type = (SerializationTreeNodeType)(tr->get_value().ToIntDef(0));
	tr = tr->get_next();
	stval = tr->get_value();
	tr = tr->get_next();
	sval1 = tr->get_value();
	tr = tr->get_next();
	sval2 = tr->get_value();

	if(type == stt_class)
	{
		if(!parent)
		{
			error(L"Ошибка загрузки статических типов. Узел типа Класс находится на верхнем уровне"
				, L"Тип", owner->name
				, L"Путь", path());
		}
		else if(parent->type != stt_classcol)
		{
			error(L"Ошибка загрузки статических типов. Узел типа Класс находится не в узле типа Коллекция классов"
				, L"Тип", owner->name
				, L"Путь", path());
		}
	}
	else if(parent) if(parent->type == stt_classcol)
	{
		error(L"Ошибка загрузки статических типов. Узел не типа Класс находится в узле типа Коллекция классов"
			, L"Тип", owner->name
			, L"Путь", path());
	}

	if(type == stt_idel)
	{
		if(!parent)
		{
			error(L"Ошибка загрузки статических типов. Узел типа ИД-элемент находится на верхнем уровне"
				, L"Тип", owner->name
				, L"Путь", path());
		}
		else if(parent->type != stt_idcol)
		{
			error(L"Ошибка загрузки статических типов. Узел типа ИД-элемент находится не в узле типа Коллекция ИД-элементов"
				, L"Тип", owner->name
				, L"Путь", path());
		}
	}
	else if(parent) if(parent->type == stt_idcol)
	{
		error(L"Ошибка загрузки статических типов. Узел не типа ИД-элемент находится в узле типа Коллекция ИД-элементов"
			, L"Тип", owner->name
			, L"Путь", path());
	}

	switch(type)
	{
		case stt_const:
			typeval1 = (SerializationTreeValueType)(stval.ToIntDef(0));
			switch(typeval1)
			{
				case stv_string:
					str1 = sval1;
					break;
				case stv_number:
					num1 = sval1.ToIntDef(0);
					break;
				case stv_uid:
					if(!string_to_GUID(sval1, &uid1))
					{
						error(L"Ошибка загрузки статических типов. Ошибка преобразования УИД в константе дерева сериализации"
							, L"Тип", owner->name
							, L"Путь", path()
							, L"УИД", sval1);
					}
					break;
				default:
					error(L"Ошибка загрузки статических типов. Некорректный тип значения константы"
						, L"Тип", owner->name
						, L"Путь", path()
						, L"Тип значения", (int)typeval1);
			}
			break;
		case stt_var:
			str1 = sval1;
			break;
		case stt_list:
			break;
		case stt_prop:
			prop1 = owner->getProperty(sval1);
			if(!prop1)
			{
				error(L"Ошибка загрузки статических типов. Некорректное имя свойства дерева сериализации"
					, L"Тип", owner->name
					, L"Путь", path()
					, L"Имя свойства", sval1);
			}
			typeprop = NULL;
			if(!stval.IsEmpty())
			{
				typeprop = MetaTypeSet::staticTypes->getTypeByName(stval);
				if(!typeprop) error(L"Ошибка загрузки статических типов. Некорректное имя типа свойства"
					, L"Тип", owner->name
					, L"Путь", path()
					, L"Имя свойства", sval1
					, L"Имя типа", stval);
			}

			prop2 = NULL;
			if(sval2.Length() > 0)
			{
				prop2 = owner->getProperty(sval2);
				if(!prop2)
				{
					error(L"Ошибка загрузки статических типов. Некорректное имя свойства дерева сериализации"
						, L"Тип", owner->name
						, L"Путь", path()
						, L"Имя свойства", sval2);
				}
			}
			break;
		case stt_elcol:
			str = sval1.SubString(1, 1);
			if(str.IsEmpty()) typeval1 = stv_none;
			else if(str.Compare(L"%") == 0)
			{
				typeval1 = stv_var;
				str1 = sval1.SubString(2, sval1.Length() - 1);
			}
			else if(str.Compare(L".") == 0)
			{
				sval1 = sval1.SubString(2, sval1.Length() - 1);
				typeval1 = stv_prop;
				prop1 = owner->getProperty(sval1);
				if(!prop1)
				{
					error(L"Ошибка загрузки статических типов. Некорректное имя свойства дерева сериализации"
						, L"Тип", owner->name
						, L"Путь", path()
						, L"Имя свойства", sval1);
				}
			}
			else
			{
				str = sval1.SubString(1, 2);
				if(str.CompareIC(L"N'") == 0)
				{
					typeval1 = stv_number;
					num1 = sval1.SubString(3, sval1.Length() - 2).ToIntDef(0);
				}
				else
				{
					error(L"Ошибка загрузки статических типов. Ошибка разбора значения в элементе коллекции дерева сериализации"
						, L"Тип", owner->name
						, L"Путь", path()
						, L"Значение", sval1);
				}
			}
			typeprop = NULL;
			if(!stval.IsEmpty())
			{
				typeprop = MetaTypeSet::staticTypes->getTypeByName(stval);
				if(!typeprop) error(L"Ошибка загрузки статических типов. Некорректное имя типа элемента коллекции"
					, L"Тип", owner->name
					, L"Путь", path()
					, L"Имя типа", stval);
			}
			break;
		case stt_gentype:
			gentype = NULL;
			for(i = 0; i < owner->generatedtypes.size(); ++i) if(sval1.CompareIC(owner->generatedtypes[i]->name) == 0)
			{
				gentype = owner->generatedtypes[i];
			}
			if(!gentype)
			{
				error(L"Ошибка загрузки статических типов. Некорректное имя генерируемого типа"
					, L"Тип", owner->name
					, L"Путь", path()
					, L"Имя генерируемого типа", sval1);
			}
			break;
		case stt_cond:
			condition = (SerializationTreeCondition)(stval.ToIntDef(0));
			if(condition <= stc_min || condition >= stc_max)
			{
				error(L"Ошибка загрузки статических типов. Некорректный вид условия"
					, L"Тип", owner->name
					, L"Путь", path()
					, L"Вид условия", stval);
			}

			str = sval1.SubString(1, 1);
			if(str.Compare(L"%") == 0)
			{
				typeval1 = stv_var;
				str1 = sval1.SubString(2, sval1.Length() - 1);
			}
			else if(str.Compare(L".") == 0)
			{
				sval1 = sval1.SubString(2, sval1.Length() - 1);
				typeval1 = stv_prop;
				prop1 = owner->getProperty(sval1);
				if(!prop1)
				{
					error(L"Ошибка загрузки статических типов. Некорректное имя свойства дерева сериализации"
						, L"Тип", owner->name
						, L"Путь", path()
						, L"Имя свойства", sval1);
				}
			}
			else if(str.Compare(L"*") == 0)
			{
				sval1 = sval1.SubString(2, sval1.Length() - 1);
				typeval1 = stv_value;
				val1 = NULL;
				i = sval1.Pos(L".");
				if(i)
				{
					str = sval1.SubString(1, i - 1);
					typ = MetaTypeSet::staticTypes->getTypeByName(str);
					if(typ)
					{
						str = sval1.SubString(i + 1, sval1.Length() - i);
						val1 = typ->getValue(str);
					}
				}

				if(!val1)
				{
					error(L"Ошибка загрузки статических типов. Некорректное значение дерева сериализации"
						, L"Тип", owner->name
						, L"Путь", path()
						, L"Значение", sval1);
				}
			}
			else if(str.Compare(L"v") == 0)
			{
				sval1 = sval1.SubString(2, sval1.Length() - 1);
				typeval1 = stv_vercon;
				if(sval1 == L"2.0") vercon1 = cv_2_0;
				else if(sval1 == L"5.0") vercon1 = cv_5_0;
				else if(sval1 == L"6.0") vercon1 = cv_6_0;
				else if(sval1 == L"106.0") vercon1 = cv_106_0;
				else if(sval1 == L"200.0") vercon1 = cv_200_0;
				else if(sval1 == L"202.2") vercon1 = cv_202_2;
				else if(sval1 == L"216.0") vercon1 = cv_216_0;
				else
				{
					error(L"Ошибка загрузки статических типов. Некорректное значение версии контейнера в дереве сериализации"
						, L"Тип", owner->name
						, L"Путь", path()
						, L"Значение", sval1);
				}
			}
			else if(str.Compare(L":") == 0)
			{
				sval1 = sval1.SubString(2, sval1.Length() - 1);
				typeval1 = stv_ver1C;
				ver1C1 = stringtover1C(sval1);
				if(ver1C1 == v1C_min)
				{
					error(L"Ошибка загрузки статических типов. Некорректное значение версии 1C в дереве сериализации"
						, L"Тип", owner->name
						, L"Путь", path()
						, L"Значение", sval1);
				}
			}
			else if(str.Compare(L"&") == 0)
			{
				sval1 = sval1.SubString(2, sval1.Length() - 1);
				typeval1 = stv_classpar;
				classpar1 = ClassParameter::getparam(sval1);
				if(!classpar1)
				{
					error(L"Ошибка загрузки статических типов. Некорректное имя параметра класса в дереве сериализации"
						, L"Тип", owner->name
						, L"Путь", path()
						, L"Значение", sval1);
				}
			}
			else if(str.Compare(L"~") == 0)
			{
				typeval1 = stv_globalvar;
				str1 = sval1.SubString(2, sval1.Length() - 1);
			}
			else
			{
				str = sval1.SubString(1, 2);
				if(str.CompareIC(L"S'") == 0)
				{
					typeval1 = stv_string;
					str1 = sval1.SubString(3, sval1.Length() - 2);
				}
				else if(str.CompareIC(L"N'") == 0)
				{
					typeval1 = stv_number;
					num1 = sval1.SubString(3, sval1.Length() - 2).ToIntDef(0);
				}
				else if(str.CompareIC(L"U'") == 0)
				{
					typeval1 = stv_uid;
					if(!string_to_GUID(sval1.SubString(3, sval1.Length() - 2), &uid1))
					{
						error(L"Ошибка загрузки статических типов. Ошибка преобразования УИД в условии дерева сериализации"
							, L"Тип", owner->name
							, L"Путь", path()
							, L"УИД", sval1);
					}
				}
				else
				{
					error(L"Ошибка загрузки статических типов. Ошибка разбора значения в условии дерева сериализации"
						, L"Тип", owner->name
						, L"Путь", path()
						, L"Значение", sval1);
				}
			}

			str = sval2.SubString(1, 1);
			if(str.Compare(L"%") == 0)
			{
				typeval2 = stv_var;
				str2 = sval2.SubString(2, sval2.Length() - 1);
			}
			else if(str.Compare(L".") == 0)
			{
				sval2 = sval2.SubString(2, sval2.Length() - 1);
				typeval2 = stv_prop;
				prop2 = owner->getProperty(sval2);
				if(!prop2)
				{
					error(L"Ошибка загрузки статических типов. Некорректное имя свойства дерева сериализации"
						, L"Тип", owner->name
						, L"Путь", path()
						, L"Имя свойства", sval2);
				}
			}
			else if(str.Compare(L"*") == 0)
			{
				sval2 = sval2.SubString(2, sval2.Length() - 1);
				typeval2 = stv_value;
				val2 = NULL;
				i = sval2.Pos(L".");
				if(i)
				{
					str = sval2.SubString(1, i - 1);
					typ = MetaTypeSet::staticTypes->getTypeByName(str);
					if(typ)
					{
						str = sval2.SubString(i + 1, sval2.Length() - i);
						val2 = typ->getValue(str);
					}
				}

				if(!val2)
				{
					error(L"Ошибка загрузки статических типов. Некорректное значение дерева сериализации"
						, L"Тип", owner->name
						, L"Путь", path()
						, L"Значение", sval2);
				}
			}
			else if(str.Compare(L"v") == 0)
			{
				sval2 = sval2.SubString(2, sval2.Length() - 1);
				typeval2 = stv_vercon;
				if(sval2 == L"2.0") vercon2 = cv_2_0;
				else if(sval2 == L"5.0") vercon2 = cv_5_0;
				else if(sval2 == L"6.0") vercon2 = cv_6_0;
				else if(sval2 == L"106.0") vercon2 = cv_106_0;
				else if(sval2 == L"200.0") vercon2 = cv_200_0;
				else if(sval2 == L"202.2") vercon2 = cv_202_2;
				else if(sval2 == L"216.0") vercon2 = cv_216_0;
				else
				{
					error(L"Ошибка загрузки статических типов. Некорректное значение версии контейнера в дереве сериализации"
						, L"Тип", owner->name
						, L"Путь", path()
						, L"Значение", sval2);
				}
			}
			else if(str.Compare(L":") == 0)
			{
				sval2 = sval2.SubString(2, sval2.Length() - 1);
				typeval2 = stv_ver1C;
				ver1C2 = stringtover1C(sval2);
				if(ver1C2 == v1C_min)
				{
					error(L"Ошибка загрузки статических типов. Некорректное значение версии 1C в дереве сериализации"
						, L"Тип", owner->name
						, L"Путь", path()
						, L"Значение", sval2);
				}
			}
			else if(str.Compare(L"&") == 0)
			{
				sval2 = sval2.SubString(2, sval2.Length() - 1);
				typeval2 = stv_classpar;
				classpar2 = ClassParameter::getparam(sval2);
				if(!classpar2)
				{
					error(L"Ошибка загрузки статических типов. Некорректное имя параметра класса в дереве сериализации"
						, L"Тип", owner->name
						, L"Путь", path()
						, L"Значение", sval2);
				}
			}
			else if(str.Compare(L"~") == 0)
			{
				typeval2 = stv_globalvar;
				str2 = sval2.SubString(2, sval2.Length() - 1);
			}
			else
			{
				str = sval2.SubString(1, 2);
				if(str.CompareIC(L"S'") == 0)
				{
					typeval2 = stv_string;
					str2 = sval2.SubString(3, sval2.Length() - 2);
				}
				else if(str.CompareIC(L"N'") == 0)
				{
					typeval2 = stv_number;
					num2 = sval2.SubString(3, sval2.Length() - 2).ToIntDef(0);
				}
				else if(str.CompareIC(L"U'") == 0)
				{
					typeval2 = stv_uid;
					if(!string_to_GUID(sval2.SubString(3, sval2.Length() - 2), &uid2))
					{
						error(L"Ошибка загрузки статических типов. Ошибка преобразования УИД в условии дерева сериализации"
							, L"Тип", owner->name
							, L"Путь", path()
							, L"УИД", sval2);
					}
				}
				else
				{
					error(L"Ошибка загрузки статических типов. Ошибка разбора значения в условии дерева сериализации"
						, L"Тип", owner->name
						, L"Путь", path()
						, L"Значение", sval2);
				}
			}

			break;
		case stt_metaid:
			break;
		case stt_classcol:
			if(stval.Compare(L"2") == 0) classtype = stct_notinlist;
			else classtype = stct_inlist;
			break;
		case stt_class:
			typeval1 = stv_uid;
			if(!string_to_GUID(sval1, &uid1))
			{
				error(L"Ошибка загрузки статических типов. Ошибка преобразования УИД в узле дерева сериализации Класс"
					, L"Тип", owner->name
					, L"Путь", path()
					, L"УИД", sval1);
			}

			typeval2 = stv_ver1C;
			ver1C2 = stringtover1C(sval2);
			if(ver1C2 == v1C_min)
			{
				error(L"Ошибка загрузки статических типов. Некорректное значение версии 1C в узле дереве сериализации Класс"
					, L"Тип", owner->name
					, L"Путь", path()
					, L"Значение", sval2);
			}
			break;
		case stt_idcol:
			if(stval.Compare(L"2") == 0) classtype = stct_notinlist;
			else classtype = stct_inlist;
			break;
		case stt_idel:
			typeval1 = stv_uid;
			if(!string_to_GUID(sval1, &uid1))
			{
				error(L"Ошибка загрузки статических типов. Ошибка преобразования УИД в узле дерева сериализации ИД-элемент"
					, L"Тип", owner->name
					, L"Путь", path()
					, L"УИД", sval1);
			}

			typeprop = NULL;
			if(!stval.IsEmpty())
			{
				typeprop = MetaTypeSet::staticTypes->getTypeByName(stval);
				if(!typeprop) error(L"Ошибка загрузки статических типов. Некорректное имя типа в узле дерева сериализации ИД-элемент"
					, L"Тип", owner->name
					, L"Путь", path()
					, L"УИД", sval1
					, L"Имя типа", stval);
			}

			typeval2 = stv_ver1C;
			ver1C2 = stringtover1C(sval2);
			if(ver1C2 == v1C_min)
			{
				error(L"Ошибка загрузки статических типов. Некорректное значение версии 1C в узле дерева сериализации ИД-элемент"
					, L"Тип", owner->name
					, L"Путь", path()
					, L"Значение", sval2);
			}
			break;
		default:
			error(L"Ошибка загрузки статических типов. Некорректный тип узла дерева сериализации"
				, L"Тип", owner->name
				, L"Путь", path()
				, L"Тип узла", (int)type);
	}

	tr = tr->get_next();
	nomove = tr->get_value().Compare(L"1") == 0 ? true : false;

	tr = tr->get_next();
	isref = tr->get_value().Compare(L"1") == 0 ? true : false;

	tr = tr->get_next();
	isrefpre = tr->get_value().Compare(L"1") == 0 ? true : false;

	tr = tr->get_next();
	isright = tr->get_value().Compare(L"1") == 0 ? true : false;

	tr = tr->get_next();
	exnernal = tr->get_value().Compare(L"1") == 0 ? true : false;

	tr = tr->get_next();
	binsertype = (BinarySerializationType)(tr->get_value().ToIntDef(0));

	tr = tr->get_next();
	binformat = (ExternalFileFormat)(tr->get_value().ToIntDef(0));

	tr = tr->get_next();
	first = SerializationTree(_owner, tr, this);
	next = NULL;

}

//---------------------------------------------------------------------------
SerializationTreeNode* __fastcall SerializationTreeNode::SerializationTree(MetaType* _owner, tree* tr, SerializationTreeNode* parent)
{
	int count, i;
	SerializationTreeNode* st;
	SerializationTreeNode* cst;
	SerializationTreeNode* nst;

	count = tr->get_value().ToIntDef(0);

	if(count == 0) return NULL;
	for(i = 0; i < count; ++i)
	{
		tr = tr->get_next();
		nst = new SerializationTreeNode(_owner, tr->get_first(), parent, i);
		if(i) cst->next = nst;
		else st = nst;
		cst = nst;
	}
	return st;
}

//---------------------------------------------------------------------------
String __fastcall SerializationTreeNode::path() const
{
	SerializationTreeNode* stn;
	String path;

	for(stn = parent, path = index; stn; stn = stn->parent)
	{
		path = String(stn->index) + ":" + path;
	}
	return path;
}

//---------------------------------------------------------------------------
String __fastcall SerializationTreeNode::typevalpresentation(SerializationTreeValueType typeval)
{
	switch(typeval)
	{
		case stv_string: return L"Строка";
		case stv_number: return L"Число";
		case stv_uid: return L"УникальныйИдентификатор";
		case stv_value: return L"Значение";
		case stv_var: return L"Переменная";
		case stv_prop: return L"Свойство";
		case stv_vercon: return L"Версия контейнера";
		case stv_ver1C: return L"Версия 1C";
		case stv_classpar: return L"Параметр класса";
		case stv_globalvar: return L"Глобальная переменная";
		case stv_none: return L"<Нет значения>";
	}
	return L"<Неизвестный тип значения дерева сериализации>";
}

//---------------------------------------------------------------------------

//********************************************************
// Класс MetaType

//---------------------------------------------------------------------------
void __fastcall MetaType::init()
{
	fhasuid = fuid != EmptyUID;
	fserializationtree = NULL;
	gentypeRef = NULL;
	fserialization_ver = 0;
	fimageindex = -1;
	fprenameindex = -1;
	fpreidindex = -1;
	fmeta = false;
	fexporttype = et_default;
	fdefaultclass = NULL;
	fserializationtree = NULL;

}

//---------------------------------------------------------------------------
__fastcall MetaType::MetaType(
	MetaTypeSet* _typeSet,
	const String& _name,
	const String& _ename,
	const String& _metaname,
	const String& _emetaname,
	const String& _uid
) : MetaBase(_name, _ename), fmetaname(_metaname), femetaname(_emetaname), typeSet(_typeSet)
{
	string_to_GUID(_uid, &fuid);
	init();
};

//---------------------------------------------------------------------------
__fastcall MetaType::MetaType(
	MetaTypeSet* _typeSet,
	const String& _name,
	const String& _ename,
	const String& _metaname,
	const String& _emetaname,
	const TGUID& _uid
) : MetaBase(_name, _ename), fmetaname(_metaname), femetaname(_emetaname), typeSet(_typeSet), fuid(_uid)
{
	init();
};

//---------------------------------------------------------------------------
__fastcall MetaType::MetaType(MetaTypeSet* _typeSet, tree* tr) : typeSet(_typeSet)
{
	tree* tt;
	tree* t;
	int num, i;
	MetaGeneratedType* gt;
	TGUID guid;

	gentypeRef = NULL;

	tt = tr->get_first();
	fname = tt->get_value();
	tt = tt->get_next();
	fename = tt->get_value();
	tt = tt->get_next();
	fhasuid = string_to_GUID(tt->get_value(), &fuid);

	tt = tt->get_next();
	fserialization_ver = tt->get_value().ToIntDef(0);
	tt = tt->get_next();
	fmetaname = tt->get_value();
	tt = tt->get_next();
	femetaname = tt->get_value();
	tt = tt->get_next();
	fimageindex = tt->get_value().ToIntDef(0);
	tt = tt->get_next();
	fprenameindex = tt->get_value().ToIntDef(0);
	tt = tt->get_next();
	fpreidindex = tt->get_value().ToIntDef(0);
	tt = tt->get_next();
	fmeta = tt->get_value().Compare(L"1") == 0 ? true : false;
	tt = tt->get_next();
	fexporttype = (ExportType)(tt->get_value().ToIntDef(0));
	tt = tt->get_next();
	string_to_GUID(tt->get_value(), &guid);
	fdefaultclass = Class::getclass(guid);

	// Свойства
	tt = tt->get_next();
	t = tt->get_first();
	num = t->get_value().ToIntDef(0);
	for(i = 0; i < num; ++i)
	{
		t = t->get_next();
		MetaProperty* mp = new MetaProperty(this, t);
		fproperties.push_back(mp);
		fproperties_by_name[mp->name.UpperCase()] = mp;
		fproperties_by_name[mp->ename.UpperCase()] = mp;
	}

	// Элементы коллекции
	tt = tt->get_next();
	t = tt->get_first();
	num = t->get_value().ToIntDef(0);
	for(i = 0; i < num; ++i)
	{
		t = t->get_next();
		fscollectiontypes.push_back(t->get_value());
	}

	// Значения
	tt = tt->get_next();
	t = tt->get_first();
	num = t->get_value().ToIntDef(0);
	for(i = 0; i < num; ++i)
	{
		t = t->get_next();
		MetaValue* mv = new MetaValue(this, t);
		fvalues.push_back(mv);
		fvalues_by_name[mv->name.UpperCase()] = mv;
		fvalues_by_name[mv->ename.UpperCase()] = mv;
		fvalues_by_value[mv->value] = mv;
	}

	// Генерируемые типы
	tt = tt->get_next();
	t = tt->get_first();
	fgentypeprefix = t->get_value();
	t = t->get_next();
	fegentypeprefix = t->get_value();
	t = t->get_next();
	num = t->get_value().ToIntDef(0);
	for(i = 0; i < num; ++i)
	{
		t = t->get_next();
		gt = new MetaGeneratedType(t);
		fgeneratedtypes.push_back(gt);
		if(gt->name.CompareIC(L"Ссылка") == 0) gentypeRef = gt;
	}

	fserializationtree = NULL;

	typeSet->add(this);
}

//---------------------------------------------------------------------------
void __fastcall MetaType::setSerializationTree(tree* tr)
{
	tree* t;
	int num, i;

	// Переменные дерева сериализации
	t = tr->get_first();
	num = t->get_value().ToIntDef(0);
	for(i = 0; i < num; ++i)
	{
		t = t->get_next();
		fserializationvars.push_back(new SerializationTreeVar(t));
	}

	// Дерево сериализации
	tr = tr->get_next();
	t = tr->get_first();
	fserializationtree = SerializationTreeNode::SerializationTree(this, t, NULL);

	// Внешние файлы
	tr = tr->get_next();
	t = tr->get_first();
	num = t->get_value().ToIntDef(0);
	for(i = 0; i < num; ++i)
	{
		t = t->get_next();
		fexternalfiles.push_back(new ExternalFile(this, t));
	}

}

//---------------------------------------------------------------------------
__fastcall MetaType::~MetaType()
{
	unsigned int j;
	SerializationTreeNode* sn;

	for(j = 0; j < fproperties.size(); ++j) delete fproperties[j];
	for(j = 0; j < fvalues.size(); ++j) delete fvalues[j];
	for(j = 0; j < fcollectiontypes.size(); ++j) delete fcollectiontypes[j];
	for(j = 0; j < fgeneratedtypes.size(); ++j) delete fgeneratedtypes[j];
	for(j = 0; j < fserializationvars.size(); ++j) delete fserializationvars[j];
	for(j = 0; j < fexternalfiles.size(); ++j) delete fexternalfiles[j];

	while(fserializationtree)
	{
		sn = fserializationtree->next;
		delete fserializationtree;
		fserializationtree = sn;
	}
}

//---------------------------------------------------------------------------
void __fastcall MetaType::fillcollectiontypes()
{
	std::vector<String>::iterator i;
	fcollectiontypes.erase(fcollectiontypes.begin(), fcollectiontypes.end());
	for(i = fscollectiontypes.begin(); i != fscollectiontypes.end(); ++i) fcollectiontypes.push_back(typeSet->getTypeByName(*i));
}

//---------------------------------------------------------------------------
MetaProperty* __fastcall MetaType::getProperty(const String& n)
{
	std::map<String, MetaProperty*>::iterator i;
	i = fproperties_by_name.find(n.UpperCase());
	if(i == fproperties_by_name.end()) return NULL;
	return i->second;
}

//---------------------------------------------------------------------------
MetaProperty* __fastcall MetaType::getProperty(int index)
{
	return fproperties[index];
}

//---------------------------------------------------------------------------
int __fastcall MetaType::numberOfProperties()
{
	return fproperties.size();
}

//---------------------------------------------------------------------------
MetaValue* __fastcall MetaType::getValue(const String& n)
{
	std::map<String, MetaValue*>::iterator i;
	i = fvalues_by_name.find(n.UpperCase());
	if(i == fvalues_by_name.end()) return NULL;
	return i->second;
}

//---------------------------------------------------------------------------
MetaValue* __fastcall MetaType::getValue(int value)
{
	std::map<int, MetaValue*>::iterator i;
	i = fvalues_by_value.find(value);
	if(i == fvalues_by_value.end()) return NULL;
	return i->second;

}

//---------------------------------------------------------------------------

//********************************************************
// Класс MetaTypeSet

//---------------------------------------------------------------------------
__fastcall MetaTypeSet::~MetaTypeSet()
{
	std::vector<MetaType*>::iterator i;
	for(i = alltype.begin(); i != alltype.end(); ++i) delete (*i);
}

//---------------------------------------------------------------------------
void __fastcall MetaTypeSet::add(MetaType* t)
{
	alltype.push_back(t);
	mapname[t->name.UpperCase()] = t;
	mapname[t->ename.UpperCase()] = t;
	if(t->hasuid) mapuid[t->uid] = t;
}

//---------------------------------------------------------------------------
MetaType* __fastcall MetaTypeSet::getTypeByName(const String& n)
{
	std::map<String, MetaType*>::iterator i;
	if(n.IsEmpty()) return NULL;
	i = mapname.find(n.UpperCase());
	if(i == mapname.end())
	{
		if(staticTypes) if(staticTypes != this) return staticTypes->getTypeByName(n);
		return NULL;
	}
	return i->second;
}

//---------------------------------------------------------------------------
MetaType* __fastcall MetaTypeSet::getTypeByUID(const TGUID& u)
{
	std::map<TGUID, MetaType*>::iterator i;
	i = mapuid.find(u);
	if(i == mapuid.end())
	{
		if(staticTypes) if(staticTypes != this) return staticTypes->getTypeByUID(u);
		return NULL;
	}
	return i->second;
}

//---------------------------------------------------------------------------
void __fastcall MetaTypeSet::staticTypesLoad(TStream* str)
{
	int number, i;
	unsigned int j;
	MetaType* mtype;
	MetaType* vtype;
	MetaProperty* prop;
	MetaValue* value;
	tree* tr;
	tree* tt;
	tree* t;
	TGUID uid;
	String sn, sen;
	MetaObject* metaobj;


	delete staticTypes;
	staticTypes = new MetaTypeSet;

	tr = parse_1Cstream(str, msreg, L"static types");

	tt = tr->get_first()->get_first();

	// Параметры классов
	number = tt->get_value().ToIntDef(0);
	for(i = 0; i < number; ++i)
	{
		tt = tt->get_next();
		new ClassParameter(tt);
	}

	// Классы
	tt = tt->get_next();
	number = tt->get_value().ToIntDef(0);
	for(i = 0; i < number; ++i)
	{
		tt = tt->get_next();
		new Class(tt);
	}

	// Типы
	tt = tt->get_next();
	number = tt->get_value().ToIntDef(0);
	for(i = 0; i < number; ++i)
	{
		tt = tt->get_next();
		mtype = new MetaType(staticTypes, tt);
	}

	staticTypes->fillall();

	// Значения по умолчанию
	tt = tt->get_next();
	number = tt->get_value().ToIntDef(0);
	for(i = 0; i < number; ++i)
	{
		tt = tt->get_next();
		t = tt->get_first();
		mtype = staticTypes->getTypeByName(t->get_value());
		t = t->get_next();
		prop = mtype->getProperty(t->get_value());
		t = t->get_next();
		prop->defaultvaluetype = (DefaultValueType)(t->get_value().ToIntDef(0));
		t = t->get_next();
		switch(prop->defaultvaluetype)
		{
			case dvt_bool:
				prop->dv_bool = t->get_value().Compare(L"1") == 0 ? true : false;
				break;
			case dvt_number:
				prop->dv_number = t->get_value().ToIntDef(0);
				break;
			case dvt_string:
				prop->dv_string = new String;
				*prop->dv_string = t->get_value();
				break;
			case dvt_date:
				string1C_to_date(t->get_value(), prop->dv_date);
				break;
			case dvt_type:
				prop->dv_type = staticTypes->getTypeByName(t->get_value());
				break;
			case dvt_enum:
				vtype = staticTypes->getTypeByName(t->get_value());
				break;

		}
		t = t->get_next();
		if(prop->defaultvaluetype == dvt_enum) prop->dv_enum = vtype->getValue(t->get_value());
	}

	// Предопределенные объекты метаданных
	tt = tt->get_next();
	number = tt->get_value().ToIntDef(0);
	for(i = 0; i < number; ++i)
	{
		tt = tt->get_next();
		t = tt->get_first();
		sn = t->get_value();
		t = t->get_next();
		sen = t->get_value();
		t = t->get_next();
		if(!string_to_GUID(t->get_value(), &uid))
		{
			error(L"Ошибка загрузки статических типов. Ошибка преобразования УИД в предопределенном объекте метаданных"
				, L"Имя", sn
				, L"УИД", t->get_value());
				continue;
		}
		metaobj = new MetaObject(uid, NULL, sn, sen);
		metaobj->setfullname(sn);
		metaobj->setefullname(sen);
		MetaObject::map[uid] = metaobj;
		MetaObject::smap[sn.UpperCase()] = metaobj;
		MetaObject::smap[sen.UpperCase()] = metaobj;
	}

	// Права
	tt = tt->get_next();
	number = tt->get_value().ToIntDef(0);
	for(i = 0; i < number; ++i)
	{
		tt = tt->get_next();
		new MetaRight(tt);
	}

	// Деревья сериализации
	tt = tt->get_next();
	number = tt->get_value().ToIntDef(0);
	for(i = 0; i < number; ++i)
	{
		tt = tt->get_next();
		t = tt->get_first();
		mtype = staticTypes->getTypeByName(t->get_value());
		mtype->setSerializationTree(t->get_next());
	}

	delete tr;

	mt_empty = new MetaType(staticTypes, L"<Пустой тип>", L"<Empty type>", L"", L"", L"00000000-0000-0000-0000-000000000000"); // Этот тип нигде не удаляется

	mt_string = staticTypes->getTypeByName(L"Строка");
	mt_number = staticTypes->getTypeByName(L"Число");
	mt_bool = staticTypes->getTypeByName(L"Булево");
	mt_date = staticTypes->getTypeByName(L"Дата");
	mt_undef = staticTypes->getTypeByName(L"Неопределено");
	mt_null = staticTypes->getTypeByName(L"Null");
	mt_type = staticTypes->getTypeByName(L"Тип");
	mt_uid = staticTypes->getTypeByName(L"УникальныйИдентификатор");
	mt_typedescrinternal = staticTypes->getTypeByName(L"ОписаниеТипаВнутр");
	mt_binarydata = staticTypes->getTypeByName(L"ДвоичныеДанные");
	mt_arbitrary = staticTypes->getTypeByName(L"Произвольный");
	mt_container = staticTypes->getTypeByName(L"Контейнер");
	mt_config = staticTypes->getTypeByName(L"ОбъектМетаданныхКонфигурация");
	mt_standart_attribute = staticTypes->getTypeByName(L"СтандартныйРеквизит");
	mt_standart_tabular_section = staticTypes->getTypeByName(L"СтандартнаяТабличнаяЧасть");

	mtype = staticTypes->getTypeByName(L"КвалификаторыДаты");
	mp_datefractions = mtype->getProperty(L"ЧастиДаты");
	mt_datefractions = staticTypes->getTypeByName(L"ЧастиДаты");
	mv_datefractionsdate = mt_datefractions->getValue(L"Дата");
	mv_datefractionstime = mt_datefractions->getValue(L"Время");

	mt_tabularsection = staticTypes->getTypeByName(L"ОбъектМетаданных: ТабличнаяЧасть");
	mt_attribute = staticTypes->getTypeByName(L"ОбъектМетаданных: Реквизит");
	mt_metaobjref = staticTypes->getTypeByName(L"ОбъектМетаданныхСсылка");
	mt_metarefint = staticTypes->getTypeByName(L"ОбъектМетаданныхСсылкаВнутр");
	mt_tabsection = staticTypes->getTypeByName(L"ТабличнаяЧасть");
	mt_metaref = staticTypes->getTypeByName(L"МетаСсылка");

	for(j = 0; j < MetaStandartTabularSection::list.size(); ++j) MetaStandartTabularSection::list[j]->_class = Class::getclass(MetaStandartTabularSection::list[j]->class_uid);

}

//---------------------------------------------------------------------------
void __fastcall MetaTypeSet::fillall()
{
	std::vector<MetaType*>::iterator i;
	int j;
	for(i = alltype.begin(); i != alltype.end(); ++i)
	{
		(*i)->fillcollectiontypes();
		for(j = 0; j < (*i)->numberOfProperties(); ++j) (*i)->getProperty(j)->filltypes();
	}
}

//---------------------------------------------------------------------------
int __fastcall MetaTypeSet::number()
{
	return alltype.size();
}

//---------------------------------------------------------------------------
MetaType* MetaTypeSet::getType(int index)
{
	return alltype[index];
}

//---------------------------------------------------------------------------

//********************************************************
// Класс GeneratedType

//---------------------------------------------------------------------------
__fastcall GeneratedType::GeneratedType(tree** ptr, String path)
{
//	memset(&typeuid, sizeof(TGUID), 0);
//	memset(&valueuid, sizeof(TGUID), 0);
	typeuid = EmptyUID;
	valueuid = EmptyUID;

	if(!*ptr)
	{
		error(L"Ошибка формата потока 44. Ожидается значение UID генерируемого типа"
			, L"Путь", path);
		return;
	}

	if((*ptr)->get_type() == nd_guid)
	{
		if(!string_to_GUID((*ptr)->get_value(), &typeuid))
		{
			error(L"Ошибка формата потока 45. Ошибка преобразования UID генерируемого типа"
				, L"UID", (*ptr)->get_value()
				, L"Путь", path + (*ptr)->path());
		}
	}
	else
	{
		error(L"Ошибка формата потока 46. Тип значения не UID"
			, L"Значение", (*ptr)->get_value()
			, L"Путь", path + (*ptr)->path());
	}

	*ptr = (*ptr)->get_next();

	if(!*ptr)
	{
		error(L"Ошибка формата потока 47. Ожидается значение UID значения генерируемого типа"
			, L"Путь", path);
		return;
	}

	if((*ptr)->get_type() == nd_guid)
	{
		if(!string_to_GUID((*ptr)->get_value(), &valueuid))
		{
			error(L"Ошибка формата потока 48. Ошибка преобразования UID значения генерируемого типа"
				, L"UID", (*ptr)->get_value()
				, L"Путь", path + (*ptr)->path());
		}
	}
	else
	{
		error(L"Ошибка формата потока 49. Тип значения не UID"
			, L"Значение", (*ptr)->get_value()
			, L"Путь", path + (*ptr)->path());
	}

	*ptr = (*ptr)->get_next();
}

//---------------------------------------------------------------------------

//********************************************************
// Класс PredefinedValue

//---------------------------------------------------------------------------
String _fastcall PredefinedValue::getfullname(bool english)
{
	String s;
	std::map<MetaGeneratedType*, GeneratedType*>::iterator i;
	MetaType* t;

	s = L"";
	if(owner) if(owner->type) if(owner->type->gentypeRef)
	{
		i = owner->v_objgentypes.find(owner->type->gentypeRef);
		if(i != owner->v_objgentypes.end())
		{
			t = owner->owner->types->getTypeByUID( i->second->typeuid);
			if(english) s = t->ename;
			else s = t->name;
			s += L".";
		}
	}
	s += name;
	return s;
}

//---------------------------------------------------------------------------


//********************************************************
// Класс Value1C

//---------------------------------------------------------------------------
__fastcall Value1C::Value1C(Value1C_obj* _parent) : parent(_parent)
{
	type = NULL;
	kind = kv_unknown;
	index = -1;
}

//---------------------------------------------------------------------------
String __fastcall Value1C::path(MetaContainer* mc, bool english)
{
	unsigned int i, j;

	if(!parent) return L"";
	if(type == mc->types->mt_config) return L"";
	j = parent->v_objpropv.size();
	if(index < 0)
	{
		for(i = 0; i < j; ++i)
		{
			if(parent->v_objpropv[i].second == this) return parent->v_objpropv[i].first->getname(english);
		}
		for(i = 0; i < parent->v_objcol.size(); ++i) if(parent->v_objcol[i] == this)
		{
			String s = presentation(english);
			if(s.IsEmpty()) return String(L"[") + i + L"]";
			else return s;
		}
	}
	else
	{
		i = index;
		if(i < j) return parent->v_objpropv[i].first->getname(english);
		else
		{
			String s = presentation(english);
			if(s.IsEmpty()) return String(L"[") + (i - j) + L"]";
			else return s;
		}
	}
	return L"???";
}

//---------------------------------------------------------------------------
String __fastcall Value1C::fullpath(MetaContainer* mc, bool english)
{
	String s = path(mc, english);
	for(Value1C_obj* v = parent; v; v = v->parent) s = v->path(mc, english) + L"\\" + s;
	return s;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_bool

//---------------------------------------------------------------------------
__fastcall Value1C_bool::Value1C_bool(Value1C_obj* _parent) : Value1C(_parent){
	kind = kv_bool;
	type = MetaTypeSet::mt_bool;
};

//---------------------------------------------------------------------------
String __fastcall Value1C_bool::presentation(bool english)
{
	if(english) return v_bool ? L"Yes" : L"No";
	else return v_bool ? L"Да" : L"Нет";
}

//---------------------------------------------------------------------------
String __fastcall Value1C_bool::valuepresentation(bool english)
{
	return L"";
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_bool::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	String s;
	if(english) s = v_bool ? L"Yes" : L"No";
	else s = v_bool ? L"Да" : L"Нет";
	str->Write(s);
	return true;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_bool::isempty()
{
	if(!this) return true;
	return false;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_string

//---------------------------------------------------------------------------
__fastcall Value1C_string::Value1C_string(Value1C_obj* _parent) : Value1C(_parent){
	kind = kv_string;
	type = MetaTypeSet::mt_string;
};

//---------------------------------------------------------------------------
String __fastcall Value1C_string::presentation(bool english)
{
	return v_string;
}

//---------------------------------------------------------------------------
String __fastcall Value1C_string::valuepresentation(bool english)
{
	return L"";
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_string::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	TStringBuilder* sb;

	sb = new TStringBuilder(v_string);
	sb->Replace(L"\"", L"\"\"");
	str->Write(String(L"\""));
	str->Write(sb->ToString());
	str->Write(String(L"\""));
	delete sb;
	return true;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_string::isempty()
{
	if(!this) return true;
	return v_string.IsEmpty();
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_number

//---------------------------------------------------------------------------
__fastcall Value1C_number::Value1C_number(Value1C_obj* _parent) : Value1C(_parent){
	kind = kv_number;
	type = MetaTypeSet::mt_number;
};

//---------------------------------------------------------------------------
String __fastcall Value1C_number::presentation(bool english)
{
	return v_string;
}

//---------------------------------------------------------------------------
String __fastcall Value1C_number::valuepresentation(bool english)
{
	return L"";
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_number::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	str->Write(v_string);
	return true;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_number::isempty()
{
	if(!this) return true;
	//return v_number == 0;
	return false;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_number_exp

//---------------------------------------------------------------------------
__fastcall Value1C_number_exp::Value1C_number_exp(Value1C_obj* _parent) : Value1C(_parent){
	kind = kv_number_exp;
	type = MetaTypeSet::mt_number;
};

//---------------------------------------------------------------------------
String __fastcall Value1C_number_exp::presentation(bool english)
{
	return v_string;
}

//---------------------------------------------------------------------------
String __fastcall Value1C_number_exp::valuepresentation(bool english)
{
	return L"";
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_number_exp::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	str->Write(v_string);
	return true;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_number_exp::isempty()
{
	if(!this) return true;
	//return v_number == 0;
	return false;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_date

//---------------------------------------------------------------------------
__fastcall Value1C_date::Value1C_date(Value1C_obj* _parent) : Value1C(_parent){
	kind = kv_date;
	type = MetaTypeSet::mt_date;
	memcpy(v_date, emptydate, 7);
};

//---------------------------------------------------------------------------
String __fastcall Value1C_date::presentation(bool english)
{
	return date_to_string(v_date);
}

//---------------------------------------------------------------------------
String __fastcall Value1C_date::valuepresentation(bool english)
{
	return L"";
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_date::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	String s;
	s = L"'";
	s += date_to_string1C(v_date);
	s += L"'";
	str->Write(s);
	return true;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_date::isempty()
{
	if(!this) return true;
	return memcmp(v_date, emptydate, 7) == 0;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_null

//---------------------------------------------------------------------------
__fastcall Value1C_null::Value1C_null(Value1C_obj* _parent) : Value1C(_parent){
	kind = kv_null;
	type = MetaTypeSet::mt_null;
};

//---------------------------------------------------------------------------
String __fastcall Value1C_null::presentation(bool english)
{
	return L"<Null>";
}

//---------------------------------------------------------------------------
String __fastcall Value1C_null::valuepresentation(bool english)
{
	return L"";
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_null::isempty()
{
	if(!this) return true;
	return false;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_null::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	String s;
	s = L"NULL";
	str->Write(s);
	return true;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_undef

//---------------------------------------------------------------------------
__fastcall Value1C_undef::Value1C_undef(Value1C_obj* _parent) : Value1C(_parent){
	kind = kv_undef;
	type = MetaTypeSet::mt_undef;
};

//---------------------------------------------------------------------------
String __fastcall Value1C_undef::presentation(bool english)
{
	if(english) return L"<Undefined>";
	else return L"<Неопределено>";
}

//---------------------------------------------------------------------------
String __fastcall Value1C_undef::valuepresentation(bool english)
{
	return L"";
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_undef::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	String s;
	s = english ? L"Undef" : L"Неопределено";
	str->Write(s);
	return true;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_undef::isempty()
{
	if(!this) return true;
	return false;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_type

//---------------------------------------------------------------------------
__fastcall Value1C_type::Value1C_type(Value1C_obj* _parent) : Value1C(_parent){
	kind = kv_type;
	type = MetaTypeSet::mt_type;
	v_type = NULL;
	v_uid = EmptyUID;
};

//---------------------------------------------------------------------------
String __fastcall Value1C_type::presentation(bool english)
{
	if(v_type) return v_type->getname(english);
	if(english) return L"<??? Unknown type>";
	else return L"<??? Неизвестный тип>";
}

//---------------------------------------------------------------------------
String __fastcall Value1C_type::valuepresentation(bool english)
{
	return L"";
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_type::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	String s;
	if(!v_type) return false;
	s = english ? v_type->ename : v_type->name;
	str->Write(s);
	return true;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_type::isempty()
{
	if(!this) return true;
	return !v_type;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_uid

//---------------------------------------------------------------------------
__fastcall Value1C_uid::Value1C_uid(Value1C_obj* _parent) : Value1C(_parent){
	kind = kv_uid;
	type = MetaTypeSet::mt_uid;
	v_uid = EmptyUID;
};

//---------------------------------------------------------------------------
String __fastcall Value1C_uid::presentation(bool english)
{
	return GUID_to_string(v_uid);
}

//---------------------------------------------------------------------------
String __fastcall Value1C_uid::valuepresentation(bool english)
{
	return L"";
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_uid::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	str->Write(GUID_to_string(v_uid));
	return true;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_uid::isempty()
{
	if(!this) return true;
	return v_uid == EmptyUID;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_enum

//---------------------------------------------------------------------------
__fastcall Value1C_enum::Value1C_enum(Value1C_obj* _parent) : Value1C(_parent){
	kind = kv_enum;
	v_enum = NULL;
};

//---------------------------------------------------------------------------
String __fastcall Value1C_enum::presentation(bool english)
{
	return v_enum->getname(english);
}

//---------------------------------------------------------------------------
String __fastcall Value1C_enum::valuepresentation(bool english)
{
	return L"";
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_enum::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	String s;
	if(!v_enum) return false;
	s = english ? v_enum->ename : v_enum->name;
	str->Write(s);
	return true;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_enum::isempty()
{
	if(!this) return true;
	return !v_enum;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_stdattr

//---------------------------------------------------------------------------
__fastcall Value1C_stdattr::Value1C_stdattr(Value1C_obj* _parent) : Value1C(_parent){
	kind = kv_stdattr;
	type = MetaTypeSet::mt_standart_attribute;
	v_metaobj = NULL;
	v_stdattr = NULL;
};

//---------------------------------------------------------------------------
String __fastcall Value1C_stdattr::presentation(bool english)
{
	if(v_stdattr)
	{
		if(v_stdattr->count) return v_stdattr->getname(english) + (v_value + 1);
		else return v_stdattr->getname(english);
	}
	if(english) return L"Unknown_standard_attribute";
	else return L"Неизвестный_стандартный_реквизит";
}

//---------------------------------------------------------------------------
String __fastcall Value1C_stdattr::valuepresentation(bool english)
{
	return L"";
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_stdattr::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	String s;
	if(!v_stdattr) return false;
	str->Write(v_stdattr->getname(english));
	return true;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_stdattr::isempty()
{
	if(!this) return true;
	return !v_metaobj && !v_stdattr;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_stdtabsec

//---------------------------------------------------------------------------
__fastcall Value1C_stdtabsec::Value1C_stdtabsec(Value1C_obj* _parent) : Value1C(_parent){
	kind = kv_stdtabsec;
	type = MetaTypeSet::mt_standart_tabular_section;
	v_metaobj = NULL;
	v_stdtabsec = NULL;
};

//---------------------------------------------------------------------------
String __fastcall Value1C_stdtabsec::presentation(bool english)
{
	if(v_stdtabsec) return v_stdtabsec->getname(english);
	if(english) return L"Unknown_standard_tabular_section";
	else return L"Неизвестная_стандартная_табличная_часть";
}

//---------------------------------------------------------------------------
String __fastcall Value1C_stdtabsec::valuepresentation(bool english)
{
	return L"";
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_stdtabsec::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	String s;
	if(!v_stdtabsec) return false;
	str->Write(v_stdtabsec->getname(english));
	return true;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_stdtabsec::isempty()
{
	if(!this) return true;
	return !v_metaobj && !v_stdtabsec;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_obj

//---------------------------------------------------------------------------
__fastcall Value1C_obj::Value1C_obj(Value1C_obj* _parent, MetaContainer* _owner) : Value1C(_parent), owner(_owner){
	kind = kv_obj;
};

//---------------------------------------------------------------------------
String __fastcall Value1C_obj::presentation(bool english)
{
	Value1C_obj* vo;
	Value1C* v;
	Value1C_enum* ve;
	Value1C_number* vn;
	Value1C* v2;
	unsigned int i;
	String s;
	String s2;

	if(type)
	{
		if(type->name.CompareIC(L"МногоязычнаяСтрока") == 0 || type->name.CompareIC(L"МногоязычнаяСтрокаВнутр") == 0)
		{
			if(v_objcol.size() > 0)
			{
				vo = (Value1C_obj*)v_objcol[0];
				v = vo->getproperty(L"Строка");
				if(v) return v->presentation(english);
			}
		}
		else if(type->name.CompareIC(L"Реквизит") == 0)
		{
			v = getproperty(L"Реквизит");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ТабличнаяЧасть") == 0)
		{
			v = getproperty(L"ТабличнаяЧасть");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"СтрокаНаЯзыке") == 0 || type->name.CompareIC(L"СтрокаНаЯзыкеВнутр") == 0)
		{
			v = getproperty(L"КодЯзыка");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ВариантHTMLДокумента") == 0)
		{
			v = getproperty(L"КодЯзыка");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"КлючИЗначение") == 0)
		{
			v = getproperty(L"Ключ");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ЭлементПрав") == 0)
		{
			v = getproperty(L"Право");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ОграничениеДоступаКДанным") == 0)
		{
			v = getproperty(L"Право");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ПраваНаОбъект") == 0)
		{
			v = getproperty(L"Объект");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ОбъектМетаданныхСсылка") == 0)
		{
			v = getproperty(L"ОбъектМетаданных");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ОписаниеСтандартногоРеквизита") == 0)
		{
			v = getproperty(L"СтандартныйРеквизит");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ОписаниеСтандартнойТабличнойЧасти") == 0)
		{
			v = getproperty(L"СтандартнаяТабличнаяЧасть");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ОписаниеТипа") == 0)
		{
			v = getproperty(L"Тип");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ФайлКартинки") == 0)
		{
			v = getproperty(L"ИмяФайла");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ГруппаКоманд") == 0)
		{
			v = getproperty(L"Группа");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ОписаниеТипов") == 0)
		{
			//s = L"";
			for(i = 0; i < v_objcol.size(); ++i)
			{
				v = v_objcol[i];
				if(v)
				{
					s += s.Length() == 0 ? L"" : L", ";
					s += v->presentation(english);
				}
			}
			return s;
		}
		else if(type->name.CompareIC(L"РасширенноеИмяXML") == 0)
		{
			v = getproperty(L"URIПространстваИмен");
			v2 = getproperty(L"ЛокальноеИмя");
			if(v && v2)
			{
				s = L"{";
				s += v->presentation(english);
				s += L"}";
				s += v2->presentation(english);
				return s;
			}
		}
		else if(type->name.CompareIC(L"СочетаниеКлавиш") == 0)
		{
			v = getproperty(L"Модификатор");
			v2 = getproperty(L"Клавиша");
			if(v && v2)
			{
				s = v2->presentation(english);
				if(s.Length() == 2) if(s[1] == L'_') s = s.SubString(2, 1);
				return v->presentation(english) + s;
			}
		}
		else if(type->name.CompareIC(L"ЭлементСоставаОбщегоРеквизита") == 0)
		{
			v = getproperty(L"Метаданные");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ЭлементСоставаФункциональнойОпции") == 0)
		{
			v = getproperty(L"Объект");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"Ссылка") == 0)
		{
			v = getproperty(L"Ссылка");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ПредопределенноеЗначение") == 0 || type->name.CompareIC(L"ПредопределенноеЗначение2") == 0)
		{
			v = getproperty(L"Тип");
			v2 = getproperty(L"Значение");
			if(v) s = v->presentation(english);
			else s = L"";
			if(v2) s2 = v2->presentation(english);
			else s2 = L"";
			if(s.IsEmpty())
			{
				if(english) s = L"<Empty type>";
				else s = L"<Пустой тип>";
			}
			if(s2.IsEmpty())
			{
				if(english) s2 = L"<Empty ref>";
				else s2 = L"<Пустая ссылка>";
			}
			return s + L"." + s2;
		}
		else if(type->name.CompareIC(L"Цвет") == 0 || type->name.CompareIC(L"ЦветВнутр") == 0)
		{
			ve = (Value1C_enum*)getproperty(L"Вид");
			if(ve) if(ve->kind == kv_enum)
			{
				if(ve->v_enum->getname(false).CompareIC(L"АвтоЦвет") == 0) return english ? L"Auto" : L"Авто";
				v2 = getproperty(L"Значение");
				if(v2) return v2->presentation(english);
				return ve->presentation(english);
			}
		}
		else if(type->name.CompareIC(L"Шрифт") == 0 || type->name.CompareIC(L"ШрифтВнутр") == 0)
		{
			ve = (Value1C_enum*)getproperty(L"Вид");
			if(ve) if(ve->kind == kv_enum)
			{
				if(ve->v_enum->getname(false).CompareIC(L"АвтоШрифт") == 0) return english ? L"Auto" : L"Авто";
				if(ve->v_enum->getname(false).CompareIC(L"ЭлементСтиля") == 0)
				{
					v = getproperty(L"Значение");
					if(v) return v->presentation(english);
				}
			}
			v = getproperty(L"Имя");
			if(v)
			{
				s = v->presentation(english);
				vn = (Value1C_number*)getproperty(L"Размер");
				if(vn) if(vn->kind == kv_number)
				{
					s += L",";
					s += (vn->v_number / 10);
				}
				return s;
			}
			v = getproperty(L"Значение");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ЭлементСоставаПланаОбмена") == 0)
		{
			v = getproperty(L"Метаданные");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ЗначениеРоли") == 0)
		{
			v = getproperty(L"Роль");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ОграничениеПолей") == 0)
		{
			v = getproperty(L"Поля");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"СтандартнаяПанель") == 0)
		{
			v = getproperty(L"СтандартнаяПанель");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"ПоложениеИнтерфейса") == 0)
		{
			v = getproperty(L"Положение");
			if(v) return v->presentation(english);
		}
		else if(type->name.CompareIC(L"МетаСсылка") == 0)
		{
			v = getproperty(L"Объект");
			if(v) s = v->presentation(english);
			else s = "";
			v = getproperty(L"СтандартнаяТабличнаяЧасть");
			if(v)
			{
				s += L".";
				s += v->presentation(english);
			}
			v = getproperty(L"СтандартныйРеквизит");
			if(v)
			{
				s += L".";
				s += v->presentation(english);
			}
			return s;
		}
		else if(type->name.CompareIC(L"ЭлементИнтерфейса") == 0)
		{
			v = getproperty(L"Вид");
			if(v) if(v->kind == kv_enum)
			{
				if(((Value1C_enum*)v)->v_enum->name.CompareIC(L"Группа") == 0) return v->presentation(english);
				v = getproperty(L"СтандартнаяПанель");
				s = v->presentation(english);
				if(s.IsEmpty()) return english ? L"<StandardPanel>" : L"<СтандартнаяПанель>";
				return s;
			}
		}
		else if(type->name.CompareIC(L"ПолеСОграничением") == 0)
		{
			v = getproperty(L"Реквизит");
			if(v) s = v->presentation(english);
			else s = "";
			if(s.Pos(L".") == 0)
			{
				v = getproperty(L"ТабличнаяЧасть");
				if(v)
				{
					s2 = v->presentation(english);
					if(!s2.IsEmpty())
					{
						if(!s.IsEmpty()){
							s2 += L".";
							s2 += s;
						}
						return s2;
					}
				}
			}
			return s;
		}
		else if(type->name.CompareIC(L"ПоляСОграничением") == 0)
		{
			if(!v_objcol.size()) return english ? L"<Other fields>" : L"<Прочие поля>";
			for(i = 0; i < v_objcol.size(); ++i)
			{
				v = v_objcol[i];
				if(v)
				{
					s += s.Length() == 0 ? L"" : L", ";
					s += v->presentation(english);
				}
			}
			return s;
		}
		v = getproperty(L"Имя");
		if(v) return v->presentation(english);
		v = getproperty(L"НомерПараметра");
		if(v) return v->presentation(english);
	}
	return L"";
}

//---------------------------------------------------------------------------
String __fastcall Value1C_obj::valuepresentation(bool english)
{
	Value1C* v;

	if(!type) return L"";

	if(type->name.CompareIC(L"СтрокаНаЯзыке") == 0 || type->name.CompareIC(L"СтрокаНаЯзыкеВнутр") == 0)
	{
		v = getproperty(L"Строка");
		if(v) return v->presentation(english);
	}
	else if(type->name.CompareIC(L"КлючИЗначение") == 0)
	{
		v = getproperty(L"Значение");
		if(v) return v->presentation(english);
	}
	else if(type->name.CompareIC(L"ЭлементПрав") == 0)
	{
		v = getproperty(L"Доступность");
		if(v) return v->presentation(english);
	}
	else if(type->name.CompareIC(L"СвязьПараметраВыбора") == 0)
	{
		v = getproperty(L"Реквизит");
		if(v) return v->presentation(english);
	}
	else if(type->name.CompareIC(L"КолонкаДереваЗначений") == 0)
	{
		v = getproperty(L"ТипЗначения");
		if(v) return v->presentation(english);
	}
	else if(type->name.CompareIC(L"КолонкаТаблицыЗначений") == 0)
	{
		v = getproperty(L"ТипЗначения");
		if(v) return v->presentation(english);
	}
	else if(type->name.CompareIC(L"ЭлементСоставаПланаОбмена") == 0)
	{
		v = getproperty(L"АвтоРегистрация");
		if(v) return v->presentation(english);
	}
	else if(type->name.CompareIC(L"ЗначениеРоли") == 0)
	{
		v = getproperty(L"Значение");
		if(v) return v->presentation(english);
	}
	else if(type->name.CompareIC(L"ПараметрВыбора") == 0)
	{
		v = getproperty(L"Значение");
		if(v) return v->presentation(english);
	}
	else if(type->name.CompareIC(L"ОграничениеПолей") == 0)
	{
		v = getproperty(L"Ограничение");
		if(v) return v->presentation(english);
	}

	return L"";

}

//---------------------------------------------------------------------------
Value1C* __fastcall Value1C_obj::getproperty(MetaProperty* mp)
{
	std::map<MetaProperty*, Value1C*, MetaPropertyLess>:: const_iterator i;
	i = v_objprop.find(mp);
	if(i == v_objprop.end()) return NULL;
	return i->second;
}

//---------------------------------------------------------------------------
Value1C* __fastcall Value1C_obj::getproperty(const String& prop)
{
	MetaProperty* pn;

	if(!type) return NULL;
	pn = type->getProperty(prop);
	if(pn) return getproperty(pn);
	return NULL;
}

//---------------------------------------------------------------------------
void __fastcall Value1C_obj::fillpropv()
{
	std::map<MetaProperty*, Value1C*, MetaPropertyLess>::const_iterator pi;
	unsigned int i;
	int count;

	//if(v_objprop.size() == 0) return;
	//if(v_objpropv.size() == v_objprop.size()) return;
	if(v_objpropv.size() > 0) v_objpropv.clear();
	v_objpropv.reserve(v_objprop.size());

	for(count = 0, pi = v_objprop.begin(); pi != v_objprop.end(); ++pi, ++count)
	{
		v_objpropv.push_back(std::pair<MetaProperty*, Value1C*>(pi->first, pi->second));
		if(pi->second) pi->second->index = count;
	}

	//for(i = 0; i < v_objcol.size(); ++i) v_objcol[i]->index = count++;
	//for(i = 0; i < v_objcol.size(); ++i) if(v_objcol[i]) v_objcol[i]->index = count++;
	for(i = 0; i < v_objcol.size(); ++i, count++) if(v_objcol[i]) v_objcol[i]->index = count;
}

//---------------------------------------------------------------------------
__fastcall Value1C_obj::~Value1C_obj()
{
	std::map<MetaProperty*, Value1C*, MetaPropertyLess>::const_iterator pi;
	unsigned int i;

	for(pi = v_objprop.begin(); pi != v_objprop.end(); ++pi) delete pi->second;
	for(i = 0; i < v_objcol.size(); ++i) delete v_objcol[i];
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_obj::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	String is;
	String npath;
	String _npath;
	String pname;
	String fname;
	TStreamWriter* strp; // свойства
	TStreamWriter* strc; // коллекция
	TFileStream* fs;
	TFileStream* fsb;
	unsigned int i, j;
	Value1C* v;
	MetaProperty* p;
	bool needtype;
	MetaType* ct;
	ExportType exporttype;
	Value1C_binary* vb;
	bool isfirst; // Признак, что файл "свой", т.е. выводятся на первом уровне, а не вложенно.
	Value1C_obj_ExportThread* thr;
	std::vector<Value1C_obj_ExportThread*> thrs;

	if(indent < 0)
	{
		isfirst = true;
		indent = 0;
	}
	else
	{
		if(str) isfirst = false;
		else isfirst = true;
	}

	// Свойства
	if(v_objpropv.size())
	{
		if(str)
		{
			if(!isfirst)
			{
				indent++;
				if(kind == kv_obj || kind == kv_extobj) str->Write(String(L"{\r\n"));
			}
			strp = str;
		}
		else
		{
			fs = new TFileStream(path + (english ? L"\\!Properties" : L"\\!Свойства"), fmCreate);
			fs->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().Length);
			strp = new TStreamWriter(fs, TEncoding::UTF8, 1024);
		}

		is = String(indentstring, indent);
		for(i = 0; i < v_objpropv.size(); ++i)
		{
			v = v_objpropv[i].second;
			if(!v) continue;
			if(v->isempty()) continue;
			p = v_objpropv[i].first;
			if(compare(p, v)) continue;
			pname = p->getname(english);
			exporttype = p->exporttype;
			if(exporttype == et_default) if(v->type) exporttype = v->type->exporttype;
			if(exporttype == et_default) if(v->kind == kv_binary) if(((Value1C_binary*)v)->v_binformat != eff_servalue) exporttype = et_file;
			needtype = false;
			if(v->type)
			{
				if(p->types.size() != 1) needtype = true;
				else if(p->types[0] == MetaTypeSet::mt_typedescrinternal && v->type == MetaTypeSet::mt_type);
				else if(p->types[0] != v->type) needtype = true;
			}
			if(exporttype == et_catalog)
			{
				npath = path + L"\\" + pname;
				CreateDir(npath);

				if(v->kind == kv_obj || v->kind == kv_metaobj || v->kind == kv_extobj)
				{
					//thr = new Value1C_obj_ExportThread((Value1C_obj*)v, npath, english);
					//thr->Start();
					//thrs.push_back(thr);
					((Value1C_obj*)v)->owner->ExportThread((Value1C_obj*)v, npath, english);
				}
				else
					v->Export(npath, NULL, 0, english);

				if(needtype || str)
				{
					strp->Write(is);
					strp->Write(pname);
					strp->Write(String(L" = ("));
					strp->Write(v->type->getname(english));
					strp->Write(String(L")@\""));
					strp->Write(pname);
					strp->Write(String(L"\"\r\n"));
				}
			}
			else if(exporttype == et_file)
			{
				if(v->kind == kv_binary)
				{
					vb = (Value1C_binary*)v;
					if(type->getname(false).CompareIC(L"ШаблонОграничений") == 0)
					{
						fname = ((Value1C_string*)(getproperty(String(L"Имя"))))->v_string + L".rt";
					}
					else
					{
						if(str && vb->v_binformat == eff_picture)
						{
							if(vb->parent->type->name.CompareIC(L"ФайлКартинки") == 0) fname = pname + L"." + ((Value1C_string*)vb->parent->getproperty(L"ИмяФайла"))->v_string;
							else fname = String::IntToHex(_crc32(vb->v_binary), 8) + L"." + vb->get_file_extension();
						}
						else fname = pname + L"." + vb->get_file_extension();
					}
					npath = path + L"\\" + fname;
					fsb = new TFileStream(npath, fmCreate);
					fsb->CopyFrom(vb->v_binary, 0);
					delete fsb;
				}
				else
				{
					if(type->getname(false).CompareIC(L"ШаблонОграничений") == 0)
					{
						fname = ((Value1C_string*)(getproperty(L"Имя")))->v_string + L".rt";
					}
					else
					{
						fname = pname;
						if(v->kind == kv_string) fname += L".bsl"; // Костыль для модулей управляемых форм
					}
					npath = path + L"\\" + fname;
					fsb = new TFileStream(npath, fmCreate);
					fsb->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().Length);
					strc = new TStreamWriter(fsb, TEncoding::UTF8, 1024);
					if(v->kind == kv_string) strc->Write(((Value1C_string*)v)->v_string);
					else v->Export(path, strc, 0, english);
					delete strc;
					delete fsb;
				}
				strp->Write(is);
				strp->Write(pname);
				strp->Write(String(L" = "));
				if(needtype)
				{
					strp->Write(String(L"("));
					strp->Write(v->type->getname(english));
					strp->Write(String(L")"));
				}
				strp->Write(String(L"%\""));
				strp->Write(fname);
				strp->Write(String(L"\"\r\n"));
			}
			else
			{
				strp->Write(is);
				strp->Write(pname);
				strp->Write(String(L" = "));
				if(needtype)
				{
					strp->Write(String(L"("));
					strp->Write(v->type->getname(english));
					strp->Write(String(L")"));
				}
				v->Export(path, strp, indent, english);
				strp->Write(String(L"\r\n"));
			}
		}
		if(str)
		{
			if(!isfirst)
			{
				indent--;
				str->Write(String(indentstring, indent));
				str->Write(String(L"}"));
			}
		}
		else
		{
			delete strp;
			delete fs;
		}
	}

	// Коллекция
	if(v_objcol.size())
	{
		ct = NULL;
		if(type) if(type->collectiontypes.size() == 1) ct = type->collectiontypes[0];
		if(!isfirst)
		{
			if(v_objpropv.size())
			{
				str->Write(String(L"\r\n"));
				str->Write(String(indentstring, indent));
			}
			indent++;
			str->Write(String(L"[\r\n"));
			is = String(indentstring, indent);
			for(i = 0; i < v_objcol.size(); ++i)
			{
				v = v_objcol[i];
				if(!v) continue; // нехорошо, но пока приходится
				//if(v->isempty()) continue; // Неизвестно пока, можно ли так жестко
				str->Write(is);
				needtype = false;
				if(v->type)
				{
					if(!ct) needtype = true;
					else if(v->type != ct) needtype = true;
				}
				if(needtype)
				{
					str->Write(String(L"("));
					str->Write(v->type->getname(english));
					str->Write(String(L")"));
				}
				v->Export(path, str, indent, english);
				str->Write(String(L"\r\n"));
			}
			indent--;
			str->Write(String(indentstring, indent));
			str->Write(String(L"]"));
		}
		else
		{
			fs = new TFileStream(path + (english ? L"\\!Order" : L"\\!Порядок"), fmCreate);
			fs->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().Length);
			strc = new TStreamWriter(fs, TEncoding::UTF8, 1024);
			for(i = 0; i < v_objcol.size(); ++i)
			{
				v = v_objcol[i];
				pname = v->presentation(english);
				if(pname.IsEmpty()) pname = String(L"[") + i + L"]";
				npath = path + L"\\" + pname;
				CreateDir(npath);
				needtype = false;
				if(v->type)
				{
					if(!ct) needtype = true;
					else if(v->type != ct) needtype = true;
				}

				if(v->kind == kv_obj || v->kind == kv_metaobj || v->kind == kv_extobj)
				{
					//thr = new Value1C_obj_ExportThread((Value1C_obj*)v, npath, english);
					//thr->Start();
					//thrs.push_back(thr);
					((Value1C_obj*)v)->owner->ExportThread((Value1C_obj*)v, npath, english);
				}
				else
					v->Export(npath, NULL, 0, english);

				if(needtype)
				{
					strc->Write(String(L"("));
					strc->Write(v->type->getname(english));
					strc->Write(String(L")"));
				}
				strc->Write(pname);
				strc->Write(String(L"\r\n"));
			}
			delete strc;
			delete fs;
		}
	}

	j = thrs.size();

	for(i = 0; i < j; ++i)
	{
		thrs[i]->WaitFor();
		delete thrs[i];
	}

//	if(j > 0)
//	{
//		Handles = new THandle[j];
//		for(i = 0; i < j; ++i)
//		{
//			Handles[i] = thrs[i]->Handle;
//		}
//		WaitForMultipleObjects(j, (void* const *)&Handles, true, INFINITE);
//		delete[] Handles;
//		for(i = 0; i < j; ++i)
//		{
//			delete thrs[i];
//		}
//	}

	return true;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_obj::isempty()
{
	unsigned int i;
	Value1C* v;
	MetaProperty* p;

	if(!this) return true;
	for(i = 0; i < v_objpropv.size(); ++i)
	{
		v = v_objpropv[i].second;
		if(!v) continue;
		if(!v->isempty())
		{
			p = v_objpropv[i].first;
			if(!compare(p, v)) return false;
		}
	}
	for(i = 0; i < v_objcol.size(); ++i)
	{
		v = v_objcol[i];
		if(!v) continue;
		if(!v->isempty()) return false;
	}
	return true;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_obj::compare(MetaProperty* p, Value1C* v)
{
	switch(p->defaultvaluetype)
	{
		case dvt_novalue:
			return false;
		case dvt_bool:
			if(v->kind != kv_bool) return false;
			if(((Value1C_bool*)v)->v_bool != p->dv_bool) return false;
			break;
		case dvt_number:
			if(v->kind != kv_number) return false;
			if(((Value1C_number*)v)->v_number != p->dv_number) return false;
			break;
		case dvt_string:
			if(v->kind != kv_string) return false;
			if(((Value1C_string*)v)->v_string.Compare(*p->dv_string) != 0) return false;
			break;
		case dvt_date:
			if(v->kind != kv_date) return false;
			if(memcmp(((Value1C_date*)v)->v_date, p->dv_date, 7) != 0) return false;
			break;
		case dvt_undef:
			if(v->kind != kv_undef) return false;
			break;
		case dvt_null:
			if(v->kind != kv_null) return false;
			break;
		case dvt_type:
			if(v->kind != kv_type) return false;
			if(((Value1C_type*)v)->v_type != p->dv_type) return false;
			break;
		case dvt_enum:
			if(v->kind != kv_enum) return false;
			if(((Value1C_enum*)v)->v_enum != p->dv_enum) return false;
			break;
	}
	return true;
}

//---------------------------------------------------------------------------
Class* __fastcall Value1C_obj::getclass(bool immediately)
{
	unsigned int i;
	Class* cl;

	for(Value1C_obj* v = this; v->parent; v = v->parent)
	{
		if(v->index >= 0)
		{
			i = (unsigned int)(v->index);
			if(i < v->parent->v_objpropv.size())
			{
				cl = v->parent->v_objpropv[i].first->_class;
				if(immediately) return cl;
				if(cl) return cl;
			}
		}
	}

	return NULL;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_metaobj

//---------------------------------------------------------------------------
__fastcall Value1C_metaobj::Value1C_metaobj(Value1C_obj* _parent, MetaContainer* _owner) : Value1C_obj(_parent, _owner){
	kind = kv_metaobj;
	v_metaobj = NULL;
};

//---------------------------------------------------------------------------
String __fastcall Value1C_metaobj::presentation(bool english)
{
	if(v_metaobj) return v_metaobj->getname(english);
	return L"";
}

//---------------------------------------------------------------------------
String __fastcall Value1C_metaobj::valuepresentation(bool english)
{
	Value1C* v;

	if(!type) return L"";

	if(type->name.CompareIC(L"ОбъектМетаданных: Реквизит") == 0)
	{
		v = getproperty(L"Тип");
		if(v) return v->presentation(english);
	}

	return L"";

}

//---------------------------------------------------------------------------
__fastcall Value1C_metaobj::~Value1C_metaobj()
{
	std::map<MetaGeneratedType*, GeneratedType*>::const_iterator gi;
	unsigned int i;

	for(gi = v_objgentypes.begin(); gi != v_objgentypes.end(); ++gi) delete gi->second;
	for(i = 0; i < v_prevalues.size(); ++i) delete v_prevalues[i];
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_metaobj::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	String is;
	String is2;
	String is3;
	TStreamWriter* strp;
	TFileStream* fs;
	std::map<MetaGeneratedType*, GeneratedType*>::const_iterator it;
	bool res;

	if(str)
	{
		indent++;
		str->Write(String(L"{\r\n"));
		strp = str;
	}
	else
	{
		fs = new TFileStream(path + (english ? L"\\!Properties" : L"\\!Свойства"), fmCreate);
		fs->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().Length);
		strp = new TStreamWriter(fs, TEncoding::UTF8, 1024);
	}

	is = String(indentstring, indent);
	strp->Write(is);
	strp->Write(english ? String(L"!Meta = {\r\n") : String(L"!Мета = {\r\n"));

	is2 = String(indentstring, indent + 1);
	is3 = String(indentstring, indent + 2);

	strp->Write(is2);
	if(english) strp->Write(String(L"ID = "));
	else strp->Write(String(L"ИД = "));
	strp->Write(GUID_to_string(v_metaobj->uid));
	strp->Write(String(L"\r\n"));
	if(v_objgentypes.size())
	{
		strp->Write(is2);
		if(english) strp->Write(String(L"GeneratedTypes = {\r\n"));
		else strp->Write(String(L"ГенерирумыеТипы = {\r\n"));
		for(it = v_objgentypes.begin(); it != v_objgentypes.end(); ++it)
		{
			strp->Write(is3);
			strp->Write(it->first->getname(english));
			strp->Write(String(L" = "));
			strp->Write(GUID_to_string(it->second->typeuid));
			strp->Write(String(L", "));
			strp->Write(GUID_to_string(it->second->valueuid));
			strp->Write(String(L"\r\n"));
		}
		strp->Write(is2);
		strp->Write(String(L"}\r\n"));
	}

	strp->Write(is);
	strp->Write(String(L"}\r\n"));

	res = Value1C_obj::Export(path, strp, indent - 1, english);

	if(!str)
	{
		delete strp;
		delete fs;
	}

	return res;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_metaobj::isempty()
{
	if(!this) return true;
	return false;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_refobj

//---------------------------------------------------------------------------
__fastcall Value1C_refobj::Value1C_refobj(Value1C_obj* _parent) : Value1C(_parent){
	kind = kv_refobj;
	v_metaobj = NULL;
	v_uid = EmptyUID;
};

//---------------------------------------------------------------------------
String __fastcall Value1C_refobj::presentation(bool english)
{
	if(v_metaobj) return v_metaobj->getfullname(english);
	return L"";
}

//---------------------------------------------------------------------------
String __fastcall Value1C_refobj::valuepresentation(bool english)
{
	return L"";
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_refobj::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	if(!v_metaobj) return false;
	str->Write(v_metaobj->getfullname(english));
	return true;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_refobj::isempty()
{
	if(!this) return true;
	return !v_metaobj;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_refpre

//---------------------------------------------------------------------------
__fastcall Value1C_refpre::Value1C_refpre(Value1C_obj* _parent) : Value1C(_parent){
	kind = kv_refpre;
	v_prevalue = NULL;
};

//---------------------------------------------------------------------------
String __fastcall Value1C_refpre::presentation(bool english)
{
	if(v_prevalue) return v_prevalue->name;
	return L"";
}

//---------------------------------------------------------------------------
String __fastcall Value1C_refpre::valuepresentation(bool english)
{
	return L"";
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_refpre::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	if(!v_prevalue) return false;
	str->Write(v_prevalue->getfullname(english));
	return true;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_refpre::isempty()
{
	if(!this) return true;
	return !v_prevalue;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_right

//---------------------------------------------------------------------------
__fastcall Value1C_right::Value1C_right(Value1C_obj* _parent) : Value1C(_parent){
	kind = kv_right;
	v_right = NULL;
};

//---------------------------------------------------------------------------
String __fastcall Value1C_right::presentation(bool english)
{
	if(v_right) return v_right->getname(english);
	return L"";
}

//---------------------------------------------------------------------------
String __fastcall Value1C_right::valuepresentation(bool english)
{
	return L"";
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_right::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	if(!v_right) return false;
	str->Write(v_right->getname(english));
	return true;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_right::isempty()
{
	if(!this) return true;
	return !v_right;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_binary

//---------------------------------------------------------------------------
__fastcall Value1C_binary::Value1C_binary(Value1C_obj* _parent) : Value1C(_parent){
	kind = kv_binary;
	v_binary = NULL;
};

//---------------------------------------------------------------------------
String __fastcall Value1C_binary::presentation(bool english)
{
	if(v_binformat == eff_string) if(v_binary)
	{
		v_binary->Seek(0, soFromBeginning);
		TStreamReader* reader = new TStreamReader(v_binary, true);
		String s = reader->ReadLine();
		delete reader;
		return s;
	}

	if(english) switch(v_binformat)
	{
		case eff_servalue: return L"SerializedValue";
		case eff_text: return L"SourceCode";
		case eff_tabdoc: return L"SpreadsheetDocument";
		case eff_binary: return L"BinaryData";
		case eff_activedoc: return L"ActiveDocument";
		case eff_htmldoc: return L"HTMLDocument";
		case eff_textdoc: return L"TextDocument";
		case eff_geo: return L"GeographicalSchema";
		case eff_kd: return L"DataCompositionSchema";
		case eff_mkd: return L"DataCompositionAppearanceTemplate";
		case eff_graf: return L"GraphicalSchema";
		case eff_xml: return L"XML";
		case eff_wsdl: return L"WSDL";
		case eff_picture: return L"Picture";
		case eff_string: return L"String";
		default: return L"??? Unknown binary format";
	}
	switch(v_binformat)
	{
		case eff_servalue: return L"СериализованноеЗначение";
		case eff_text: return L"ИсходныйТекст";
		case eff_tabdoc: return L"ТабличныйДокумент";
		case eff_binary: return L"ДвоичныеДанные";
		case eff_activedoc: return L"ActiveДокумент";
		case eff_htmldoc: return L"HTMLДокумент";
		case eff_textdoc: return L"ТекстовыйДокумент";
		case eff_geo: return L"ГеографическаяСхема";
		case eff_kd: return L"СхемаКомпоновкиДанных";
		case eff_mkd: return L"МакетОформленияКомпоновкиДанных";
		case eff_graf: return L"ГрафическаяСхема";
		case eff_xml: return L"XML";
		case eff_wsdl: return L"WSDL";
		case eff_picture: return L"Картинка";
		case eff_string: return L"Строка";
		default: return L"??? Неизвестный формат двоичных данных";
	}
}

//---------------------------------------------------------------------------
String __fastcall Value1C_binary::valuepresentation(bool english)
{
	return L"";
}

//---------------------------------------------------------------------------
__fastcall Value1C_binary::~Value1C_binary()
{
	delete v_binary;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_binary::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	TStringBuilder* sb;
	String s;

	if(v_binformat == eff_string)
	{
		s = presentation(english);
		sb = new TStringBuilder(s);
		sb->Replace(L"\"", L"\"\"");
		str->Write(String(L"\""));
		str->Write(sb->ToString());
		str->Write(String(L"\""));
		delete sb;
	}
	else str->Write(String(L"<binary>"));
	return true;
}

//---------------------------------------------------------------------------
String Value1C_binary::get_file_extension()
{
	file_format ff;
	switch(v_binformat)
	{
		case eff_servalue:
			return L"err";
		case eff_tabdoc:
			return L"mxl";
		case eff_geo:
			return L"geo";
		case eff_graf:
			return L"grs";
		case eff_text:
			return L"bsl";
		case eff_textdoc:
			return L"txt";
		case eff_string:
			return L"bsl";
		case eff_htmldoc:
			return L"htm";
		case eff_kd:
		case eff_mkd:
			return L"bin";
		case eff_xml:
			return L"xml";
		case eff_wsdl:
			return L"wsdl";
		case eff_picture:
			ff = get_file_format(v_binary);
			switch(ff)
			{
				case ff_gif:
					return L"gif";
				case ff_pcx:
					return L"pcx";
				case ff_bmp:
					return L"bmp";
				case ff_jpg:
					return L"jpg";
				case ff_png:
					return L"png";
				case ff_tiff:
					return L"tif";
				case ff_ico:
					return L"ico";
				case ff_wmf:
					return L"wmf";
				case ff_emf:
					return L"emf";
				case ff_zip:
					return L"zip";
			}
	}
	return L"bin";
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_binary::isempty()
{
	if(!this) return true;
	if(!v_binary) return true;
	if(v_binary->Size == 0l) return true;
	return false;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс Value1C_extobj

//---------------------------------------------------------------------------
__fastcall Value1C_extobj::Value1C_extobj(Value1C_obj* _parent, MetaContainer* _owner, const String& _path, MetaType* _type, TGUID _metauid)
	: Value1C_obj(_parent, _owner)
{
	path = _path;
	type = _type;
	metauid = _metauid;
	kind = kv_extobj;
	opened = false;
};

//---------------------------------------------------------------------------
__fastcall Value1C_extobj::~Value1C_extobj()
{
	//close();
}

//---------------------------------------------------------------------------
void __fastcall Value1C_extobj::open()
{
	tree *tt, *tte;

	if(opened) return;

	if(!type->serializationtree)
	{
		error(L"Ошибка формата потока 202. Не задано дерево сериализации типа при разборе внешнего файла"
			, L"Загружаемый тип", type->name
			, L"Путь", owner->storpresent + path);
	}
	else
	{
		tt = owner->gettree(owner->stor, path, false);
		if(!tt)
		{
			error(L"Ошибка формата потока 201. Не найден или пустой внешний файл"
				, L"Загружаемый тип", type->name
				, L"Путь", owner->storpresent + path);
		}
		else
		{
			tte = tt->get_first();
			owner->loadValue1C(this, &tte, type->serializationtree, metauid, NULL, NULL, path, true);
			delete tt;
			fillpropv();
			owner->inituninitvalues();
		}
	}

	opened = true;
}

//---------------------------------------------------------------------------
void __fastcall Value1C_extobj::close()
{
	std::map<MetaProperty*, Value1C*, MetaPropertyLess>::const_iterator pi;
	unsigned int i;
	if(!opened) return;

	for(pi = v_objprop.begin(); pi != v_objprop.end(); ++pi) delete pi->second;
	for(i = 0; i < v_objcol.size(); ++i) delete v_objcol[i];

	v_objprop.clear();
	v_objcol.clear();
	opened = false;
}

//---------------------------------------------------------------------------
Value1C* __fastcall Value1C_extobj::getproperty(MetaProperty* mp)
{
	open();
	return Value1C_obj::getproperty(mp);
}

//---------------------------------------------------------------------------
Value1C* __fastcall Value1C_extobj::getproperty(const String& prop)
{
	open();
	return Value1C_obj::getproperty(prop);
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_extobj::Export(const String& path, TStreamWriter* str, int indent, bool english)
{
	bool needclose;
	bool ret;

	needclose = false;
	if(!opened)
	{
		needclose = true;
		if(cur_export_thread) cur_export_thread->singlethread++;
		open();
	}
	ret = Value1C_obj::Export(path, str, indent, english);
	if(needclose)
	{
		close();
		if(cur_export_thread) cur_export_thread->singlethread--;
	}
	return ret;
}

//---------------------------------------------------------------------------
bool __fastcall Value1C_extobj::isempty()
{
	if(!opened) return false;
	return Value1C_obj::isempty();
}

//---------------------------------------------------------------------------

//********************************************************
// Класс MetaContainer

//---------------------------------------------------------------------------
tree* __fastcall MetaContainer::gettree(ConfigStorage* stor, const String& path, bool reporterror)
{
	CongigFile* cf;
	tree* t;
	String fullpath;

	fullpath = stor->presentation() + L"\\" + path;
	cf = stor->readfile(path);
	if(!cf)
	{
		if(reporterror)
		{
			error(L"Ошибка чтения контейнера. Не найден файл."
				, L"Путь", fullpath);
		}
		return NULL;
	}

	t = parse_1Cstream(cf->str, msreg, fullpath);
	stor->close(cf);

	if(!t)
	{
		error(L"Ошибка формата потока 140. Ошибка разбора файла."
			, L"Путь", fullpath);
	}
	return t;
}

//---------------------------------------------------------------------------
__fastcall MetaContainer::MetaContainer(ConfigStorage* _stor, bool _useExternal)
{
	tree* tr;
	tree* t;
	String version_version;
	String s;
	String metaname;
	String emetaname;
	String typepref;
	String etypepref;
	String mtypename;
	int i,j;
	MetaGeneratedType* gt;
	std::map<MetaGeneratedType*, GeneratedType*>::iterator git;
	Value1C_metaobj *vo, *vvo;
	MetaObject* mo;
	std::map<TGUID, MetaObject*>::iterator mit;
	Value1C_stdtabsec* metats;

	string_to_GUID(L"03f171e8-326f-41c6-9fa5-932a0b12cddf", &sig_standart_attribute);
	string_to_GUID(L"28db313d-dbc2-4b83-8c4a-d2aeee708062", &sig_standart_table_sec);
	string_to_GUID(L"91162600-3161-4326-89a0-4a7cecd5092a", &sig_ext_dimension);
	string_to_GUID(L"b3b48b29-d652-47ab-9d21-7e06768c31b5", &sig_ext_dimension_type);
	FormatSettings.DecimalSeparator = L'.';

	ftypes = new MetaTypeSet;
	froot = NULL;

	useExternal = _useExternal;
	stor = _stor;
	storpresent = stor->presentation() + L"\\";

	puninitvalues = &uninitvalues;

	// Файл version
	tr = gettree(stor, L"version");
	if(!tr)
	{
		stor = NULL;
		return;
	}

	try
	{
		t = tr->get_first()->get_first()->get_first();
		i = t->get_value().ToInt();
		t = t->get_next();
		j = t->get_value().ToInt();
	}
	catch(...)
	{
		error(L"Ошибка формата потока 82. Ошибка разбора файла version"
			, L"Путь", storpresent + L"version");
		delete tr;
		stor = NULL;
		return;
	}
	delete tr;

	version_version = String(i) + L"." + j;
	if(i == 2 && j == 0)
	{
		containerver = cv_2_0;
		ver1C = v1C_8_0;
	}
	else if(i == 5 && j == 0)
	{
		containerver = cv_5_0;
		ver1C = v1C_8_0;
	}
	else if(i == 6 && j == 0)
	{
		containerver = cv_6_0;
		ver1C = v1C_8_0;
	}
	else if(i == 106 && j == 0)
	{
		containerver = cv_106_0;
		ver1C = v1C_8_1;
	}
	else if(i == 200 && j == 0)
	{
		containerver = cv_200_0;
		ver1C = v1C_8_2;
	}
	else if(i == 202 && j == 2)
	{
		containerver = cv_202_2;
		ver1C = v1C_8_2;
	}
	else if(i == 216 && j == 0)
	{
		containerver = cv_216_0;
		ver1C = v1C_8_2_14;
	}
	else
	{
		error(L"Ошибка формата потока 81. Неизвестная версия в файле version"
			, L"Путь", storpresent + L"version"
			, L"Версия", version_version);
		stor = NULL;
		return;
	}

	if(containerver < cv_106_0) metaprefix = L"metadata\\";
	else metaprefix = L"";

	// Файл versions
	// пока не разбираем!

	// Файл root
	tr = gettree(stor, metaprefix + L"root");
	if(!tr)
	{
		stor = NULL;
		return;
	}

	try
	{
		t = tr->get_first()->get_first();
		i = t->get_value().ToInt();
		t = t->get_next();
		s = t->get_value();
	}
	catch(...)
	{
		error(L"Ошибка формата потока 80. Ошибка разбора файла root"
			, L"Путь", storpresent + metaprefix + L"version");
		delete tr;
		stor = NULL;
		return;
	}
	delete tr;

	if(i == 1)
	{
		if(containerver >= cv_106_0)
		{
			error(L"Ошибка формата потока 79. Версия root не соответствует версии version"
				, L"Версия root", i
				, L"Версия version", version_version
				, L"Путь", storpresent);
			stor = NULL;
			return;
		}
	}
	else if(i == 2)
	{
		if(containerver < cv_106_0)
		{
			error(L"Ошибка формата потока 78. Версия root не соответствует версии version"
				, L"Версия root", i
				, L"Версия version", version_version
				, L"Путь", storpresent);
			stor = NULL;
			return;
		}
	}
	else
	{
		error(L"Ошибка формата потока 77. Неизвестная версия root"
			, L"Версия root", i
			, L"Путь", storpresent);
		stor = NULL;
		return;
	}

	tr = gettree(stor, metaprefix + s);
	if(!tr) return;
	t = tr->get_first();
	metats = NULL;
	froot = (Value1C_obj*)readValue1C(&t, MetaTypeSet::mt_container, NULL, EmptyUID, metats, NULL, metaprefix + s, true);
	delete tr;

	// Заполнение имен объектов метаданных
	for(mit = fmetamap.begin(); mit != fmetamap.end(); ++mit)
	{
		mo = mit->second;
		vo = mo->value;
		if(vo->type == ftypes->mt_container)
		{
			mo->setname(L"<Контейнер>");
			mo->setename(L"<Container>");
		}
		else
		{
			//s = vo->getproperty(vo->type->getProperty(L"Имя"))->v_string;
			//s = ((Value1C_string*)vo->getproperty(L"Имя"))->v_string;
			s = vo->getproperty(L"Имя")->presentation();
			mo->setname(s);
			mo->setename(s);
		}
	}
	for(mit = fmetamap.begin(); mit != fmetamap.end(); ++mit)
	{
		mo = mit->second;
		vvo = mo->value;
		//if(v->type == ftypes->mt_container) continue;
		metaname = mo->name;
		emetaname = mo->ename;
		typepref = L"";
		etypepref = L"";
		mtypename = mo->name;
		if(vvo->type != ftypes->mt_config) for(vo = (Value1C_metaobj*)vvo->parent; vo; vo = (Value1C_metaobj*)vo->parent)
		{
			if(vo->type == ftypes->mt_config) break;
			if(vo->kind == kv_metaobj)
			{
				metaname = vo->v_metaobj->name + L"." + metaname; //-V525
				emetaname = vo->v_metaobj->ename + L"." + emetaname;
				mtypename = vo->v_metaobj->ename + L"." + mtypename;
				typepref = vo->type->gentypeprefix + typepref;
				etypepref = vo->type->egentypeprefix + etypepref;
			}
			else if(vo->type->metaname.Length() > 0)
			{
				metaname = vo->type->metaname + L"." + metaname;
				emetaname = vo->type->emetaname + L"." + emetaname;
			}
		}
		mo->setfullname(metaname);
		mo->setefullname(emetaname);
		fsmetamap[metaname] = mo;
		fsmetamap[emetaname] = mo;
		for(git = vvo->v_objgentypes.begin(); git != vvo->v_objgentypes.end(); ++git)
		{
			gt = git->first;
			if(gt->woprefix)
			{
				metaname = typepref + gt->name + L"." + mtypename;
				emetaname = etypepref + gt->ename + L"." + mtypename;
			}
			else
			{
				metaname = typepref + vvo->type->gentypeprefix + gt->name + L"." + mtypename;
				emetaname = etypepref + vvo->type->egentypeprefix + gt->ename + L"." + mtypename;
			}
			ftypes->add(new MetaType(ftypes, metaname, emetaname, L"", L"", git->second->typeuid));
		}
	}

	// Инициализация неинициализированных занчений
	inituninitvalues();

	if(!useExternal)
	{
		stor = NULL;
	}
}

//---------------------------------------------------------------------------
void __fastcall MetaContainer::inituninitvalues()
{
	int j;
	unsigned int in;
	Value1C *v;
	Value1C_obj *vo;
	Value1C_metaobj *vvo;
	Value1C_refpre *vrp;
	Value1C_type *vt;
	Value1C_stdattr *vsa;
	Value1C_stdtabsec* vst;
	Value1C_refobj* vro;
	MetaObject* mo;
	MetaType* typ;
	TGUID uid;
	MetaStandartAttribute* sa;
	MetaStandartTabularSection* st;
	Class* cl;
	bool flag;
	std::vector<UninitValue1C>* _puninitvalues;

	_puninitvalues = puninitvalues;
	for(std::vector<UninitValue1C>::iterator ui = _puninitvalues->begin(); ui != _puninitvalues->end(); ++ui)
	{
		v = ui->value;
		uid = ui->uid;
		if(v->kind == kv_refobj)
		{
			vro = (Value1C_refobj*)v;
			uid = vro->v_uid;
			vro->v_metaobj = getMetaObject(uid);
			if(!vro->v_metaobj)
			{
#ifndef VikaD
				error(L"Ошибка формата потока 74. Не определен объект метаданных по UID"
					, L"UID", GUID_to_string(uid)
					, L"Путь", ui->path
					, L"Мета", v->fullpath(this, false));
#endif //VikaD
			}
		}
		else if(v->kind == kv_refpre)
		{
			vrp = (Value1C_refpre*)v;
			vrp->v_prevalue = getPreValue(uid);
			if(!vrp->v_prevalue)
			{
				error(L"Ошибка формата потока 162. Не определен предопределенный элемент по UID"
					, L"UID", GUID_to_string(uid)
					, L"Путь", ui->path
					, L"Мета", v->fullpath(this, false));
			}
		}
		else if(v->kind == kv_type)
		{
			vt = (Value1C_type*)v;
			uid = vt->v_uid;
			if(uid == EmptyUID) vt->v_type = MetaTypeSet::mt_empty;
			else vt->v_type = ftypes->getTypeByUID(uid);
			if(!vt->v_type)
			{
#ifndef VikaD
				error(L"Ошибка формата потока 75. Не определен тип по UID"
					, L"UID", GUID_to_string(uid)
					, L"Путь", ui->path
					, L"Мета", v->fullpath(this, false));
#endif //VikaD
			}
		}
		else if(v->kind == kv_stdattr)
		{
			vsa = (Value1C_stdattr*)v;
			mo = getMetaObject(uid);
			vsa->v_stdattr = NULL;
			if(!mo)
			{
				error(L"Ошибка формата потока 106. Ошибка загрузки стандартного реквизита. Не определен объект метаданных по UID"
					, L"UID", GUID_to_string(uid)
					, L"Путь", ui->path
					, L"Мета", v->fullpath(this, false));
			}
			else
			{
				j = vsa->v_value;

				if(mo->value->type == MetaTypeSet::mt_attribute)
				{
					vt = NULL;
					typ = NULL;
					vo = mo->value;
					vo = (Value1C_obj*)vo->getproperty(L"Тип"); // ОписаниеТипов
					if(vo) if(vo->v_objcol.size() > 0) vo = (Value1C_obj*)vo->v_objcol[0]; // ОписаниеТипа
					if(vo) vt = (Value1C_type*)vo->getproperty(L"Тип"); // ОписаниеТипаВнутр
					if(vt)
					{
						typ = vt->v_type;
						if(!typ) typ = ftypes->getTypeByUID(vt->v_uid);
					}
					if(typ) if(typ->defaultclass)
					{
						cl = typ->defaultclass;
						for(in = 0; in < cl->standartattributes.size(); ++in)
						{
							sa = cl->standartattributes[in];
							if(j == sa->value)
							{
								vsa->v_stdattr = sa;
								break;
							}
						}
					}
				}
				if(!vsa->v_stdattr) if(ui->metats)
				{
					cl = ui->metats->v_stdtabsec->_class;
					for(in = 0; in < cl->standartattributes.size(); ++in)
					{
						sa = cl->standartattributes[in];
						if(sa->count)
						{
							if(sa->uid == ui->sauid)
							{
								vsa->v_stdattr = sa;
								if(j < sa->value || j > sa->valuemax)
								{
									error(L"Ошибка формата потока 212. Ошибка загрузки стандартного реквизита. Значение за пределами допустимого."
										, L"Класс", GUID_to_string(cl->uid)
										, L"Значение", j
										, L"Минимальное значение", sa->value
										, L"Максимальное значение", sa->valuemax
										, L"Путь", ui->path
										, L"Мета", v->fullpath(this, false));
								}
								break;
							}
						}
						else if(j == sa->value)
						{
							vsa->v_stdattr = sa;
							break;
						}
					}
				}
				if(!vsa->v_stdattr)
				{
					//if(mo->value->type->standartattributes.size() == 0)
					cl = mo->value->getclass(true);
					flag = false;
					if(!cl) flag = true;
					else if(cl->standartattributes.size() == 0) flag = true;
					if(flag)
					{
						for(vo = mo->value->parent, mo = NULL; vo; vo = vo->parent)
						{
							if(vo->kind != kv_metaobj) continue;
							vvo = (Value1C_metaobj*)vo;
							if(!vvo->type) continue;
							if(vvo->type == MetaTypeSet::mt_tabularsection) continue;
							cl = vvo->getclass(true);
							if(cl) if(cl->standartattributes.size() > 0)
							{
								mo = vvo->v_metaobj;
								break;
							}
						}
					}
					if(!mo)
					{
						error(L"Ошибка формата потока 116. Не определен объект метаданных стандартного реквизита по UID"
							, L"UID", GUID_to_string(uid)
							, L"Путь", ui->path
							, L"Мета", v->fullpath(this, false));
					}
					else
					{
						cl = mo->value->getclass();
						if(!cl)
						{
							error(L"Ошибка формата потока 107. Не определен класс объекта метаданных"
								, L"Объект метаданных", mo->fullname
								, L"Путь", ui->path
								, L"Мета", v->fullpath(this, false));
						}
						else
						{
							for(in = 0; in < cl->standartattributes.size(); ++in)
							{
								sa = cl->standartattributes[in];
								if(sa->count)
								{
									if(sa->uid == ui->sauid)
									{
										vsa->v_stdattr = sa;
										if(j < sa->value || j > sa->valuemax)
										{
											error(L"Ошибка формата потока 206. Ошибка загрузки стандартного реквизита. Значение за пределами допустимого."
												, L"Класс", GUID_to_string(cl->uid)
												, L"Значение", j
												, L"Минимальное значение", sa->value
												, L"Максимальное значение", sa->valuemax
												, L"Путь", ui->path
												, L"Мета", v->fullpath(this, false));
										}
										break;
									}
								}
								else if(j == sa->value)
								{
									vsa->v_stdattr = sa;
									break;
								}
							}
							if(!vsa->v_stdattr)
							{
								error(L"Ошибка формата потока 108. Не найден стандартный реквизит по значению"
									, L"Класс", GUID_to_string(cl->uid)
									, L"Значение", j
									, L"Путь", ui->path
									, L"Мета", v->fullpath(this, false));
							}
						}
					}
				}
			}
			vsa->v_metaobj = mo;
		}
		else if(v->kind == kv_stdtabsec)
		{
			vst = (Value1C_stdtabsec*)v;
			vst->v_stdtabsec = NULL;
			mo = getMetaObject(uid);
			vst->v_metaobj = mo;
			if(!mo)
			{
				error(L"Ошибка формата потока 209. Ошибка загрузки стандартной табличной части.  Не определен объект метаданных по metauid."
					, L"UID", GUID_to_string(uid)
					, L"Путь", ui->path
					, L"Мета", v->fullpath(this, false));
			}
			else
			{
				cl = mo->value->getclass();
				if(!cl)
				{
					error(L"Ошибка формата потока 210. Не определен класс объекта метаданных"
						, L"Объект метаданных", mo->fullname
						, L"Путь", ui->path
						, L"Мета", v->fullpath(this, false));
				}
				else
				{
					j = vst->v_value;
					for(in = 0; in < cl->standarttabularsections.size(); ++in)
					{
						st = cl->standarttabularsections[in];
						if(j == st->value)
						{
							vst->v_stdtabsec = st;
							break;
						}
					}
					if(!vst->v_stdtabsec)
					{
						error(L"Ошибка формата потока 211. Не найдена стандартная табличная часть по значению"
							, L"Класс", GUID_to_string(cl->uid)
							, L"Значение", j
							, L"Путь", ui->path
							, L"Мета", v->fullpath(this, false));
					}
				}

			}

		}
	}

	_puninitvalues->clear();

}

//---------------------------------------------------------------------------
__fastcall MetaContainer::~MetaContainer()
{
	delete stor;

	delete root;
	delete types;
}

//---------------------------------------------------------------------------
Value1C* __fastcall MetaContainer::readValue1C(tree** ptr, MetaType* t, Value1C_obj* valparent, const TGUID& metauid, Value1C_stdtabsec*& metats, ClassItem* clitem, String path, bool checkend)
{
	tree* tr;
	tree* tt;
	tree* ttt;
	tree* tx;
	Value1C* v;
	Value1C* vv;
	Value1C_bool* vb;
	Value1C_string* vs;
	Value1C_number* vn;
	Value1C_number_exp* vne;
	Value1C_date* vd;
	Value1C_type* vt;
	Value1C_uid* vu;
	Value1C_binary* vbn;
	Value1C_enum* ve;
	Value1C_stdattr* vsa;
	Value1C_stdtabsec* vst;
	Value1C_obj* vo;
	Value1C_refobj* vro;
	String s;
	TGUID uid, ouid;
	int i, k, n;
	unsigned int j;
	String spath;
	TStreamWriter* sw;
	MetaProperty* prop;
	bool b;
	Value1C_stdtabsec* _metats;
	//HANDLE handle;
	String sn;
	THandle handle;

	spath = storpresent + path;
	v = NULL;
	vo = NULL;
	if(t == MetaTypeSet::mt_arbitrary) t = NULL; // Если тип Произвольный, то тип не задан!

	if(*ptr == NULL)
	{
		if(t)
		{
			v = new Value1C_obj(valparent, this);
			v->type = t;
			if(t->serialization_ver == 0)
			{
				if(t->serializationtree)
				{
					error(L"Ошибка формата потока 73. Ожидается значение."
						, L"Загружаемый тип", t->name
						, L"Путь", spath);
				}
				else
				{
					error(L"Ошибка формата потока 17. Не определен алгоритм загрузки типа."
						, L"Загружаемый тип", t->name
						, L"Путь", spath);
				}

			}
			else if(t->serialization_ver > 1)
			{
				error(L"Ошибка формата потока 18. Ожидается значение."
					, L"Загружаемый тип", t->name
					, L"Путь", spath);

			}
		}
		else
		{
			//v = new Value1C(valparent);
			error(L"Ошибка формата потока 1. Ожидается значение."
				, L"Путь", spath);
		}
	}
	else if(!t) // Тип не задан
	{
		tr = *ptr;
		*ptr = tr->get_next();
		if(tr->get_type() == nd_list)
		{
			tr = tr->get_first();
			if(tr->get_type() == nd_string)
			{
				s = tr->get_value();
				if(s.CompareIC(L"S") == 0)
				{
					tr = tr->get_next();
					v = readValue1C(&tr, MetaTypeSet::mt_string, valparent, metauid, metats, clitem, path);
				}
				else if(s.CompareIC(L"N") == 0)
				{
					tr = tr->get_next();
					v = readValue1C(&tr, MetaTypeSet::mt_number, valparent, metauid, metats, clitem, path);
				}
				else if(s.CompareIC(L"B") == 0)
				{
					tr = tr->get_next();
					v = readValue1C(&tr, MetaTypeSet::mt_bool, valparent, metauid, metats, clitem, path);
				}
				else if(s.CompareIC(L"D") == 0)
				{
					tr = tr->get_next();
					v = readValue1C(&tr, MetaTypeSet::mt_date, valparent, metauid, metats, clitem, path);
				}
				else if(s.CompareIC(L"U") == 0)
				{
					tr = tr->get_next();
					v = new Value1C_undef(valparent);
				}
				else if(s.CompareIC(L"L") == 0)
				{
					tr = tr->get_next();
					v = new Value1C_null(valparent);
				}
				else if(s.CompareIC(L"T") == 0)
				{
					tr = tr->get_next();
					v = readValue1C(&tr, MetaTypeSet::mt_type, valparent, metauid, metats, clitem, path);
				}
				else if(s.CompareIC(L"#") == 0)
				{
					tr = tr->get_next();
					if(tr)
					{
						if(tr->get_type() == nd_guid)
						{
							if(string_to_GUID(tr->get_value(), &uid))
							{
								t = MetaTypeSet::staticTypes->getTypeByUID(uid);
								if(t != NULL)
								{
									tr = tr->get_next();
									v = readValue1C(&tr, t, valparent, metauid, metats, clitem, path);
								}
								else
								{
									//v = new Value1C(valparent);
									error(L"Ошибка формата потока 2. Не найден тип по UID"
										, L"UID", tr->get_value()
										, L"Путь", spath + tr->path()
										, L"Мета", valparent->fullpath(this, false));
								}
							}
							else
							{
								//v = new Value1C(valparent);
								error(L"Ошибка формата потока 3. Ошибка преобразования UID"
									, L"UID", tr->get_value()
									, L"Путь", spath + tr->path());
							}
						}
						else
						{
							//v = new Value1C(valparent);
							error(L"Ошибка формата потока 4. Ожидается значение UID"
								, L"Тип значения", get_node_type_presentation(tr->get_type())
								, L"Значение", tr->get_value()
								, L"Путь", spath + tr->path());
						}
					}
					else
					{
						//v = new Value1C(valparent);
						error(L"Ошибка формата потока 5. Отсутствует UID типа значения"
							, L"Путь", spath);
					}
				}
				else
				{
					//v = new Value1C(valparent);
					error(L"Ошибка формата потока 6. Неизвестный символ типа значения"
						, L"Символ значения", s
						, L"Путь", spath + tr->path());
				}
			}
			else
			{
				//v = new Value1C(valparent);
				error(L"Ошибка формата потока 7. Ожидается строковое представление типа значения"
					, L"Тип значения", get_node_type_presentation(tr->get_type())
					, L"Значение", tr->get_value()
					, L"Путь", spath + tr->path());
			}
		}
		else
		{
			//v = new Value1C(valparent);
			error(L"Ошибка формата потока 8. Ожидается значение типа список"
				, "Тип значения", get_node_type_presentation(tr->get_type())
				, L"Путь", spath + tr->path());
		}
	}
	else  // Тип задан
	{
//		v = new Value1C(valparent);
//		v->type = t;
		tr = *ptr;

		if(t == MetaTypeSet::mt_string)
		{
			if(tr->get_type() == nd_string)
			{
				s = tr->get_value();
				if(s.Length() > maxStringLength)
				{
					vbn = new Value1C_binary(valparent);
					v = vbn;
					vbn->v_binformat = eff_string;
					vbn->v_binary = new TTempStream();
					vbn->v_binary->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().Length);
					sw = new TStreamWriter(vbn->v_binary, TEncoding::UTF8, 1024);
					sw->Write(s);
					delete sw;
				}
				else
				{
					vs = new Value1C_string(valparent);
					v = vs;
					vs->v_string = s;
				}

			}
			else
			{
				vs = new Value1C_string(valparent);
				vs->v_string = L"";
				v = vs;
				error(L"Ошибка формата потока 113. Ошибка получения строки. Тип значения не Строка"
					, L"Тип значения", get_node_type_presentation(tr->get_type())
					, L"Значение", tr->get_value()
					, L"Путь", spath + tr->path());
			}
			*ptr = tr->get_next();
		}
		else if(t == MetaTypeSet::mt_number)
		{
			if(tr->get_type() == nd_number)
			{
				vn = new Value1C_number(valparent);
				v = vn;
				vn->v_string = tr->get_value();
				vn->v_number = vn->v_string.ToIntDef(0);
			}
			else if(tr->get_type() == nd_number_exp)
			{
				vne = new Value1C_number_exp(valparent);
				v = vne;
				vne->v_string = tr->get_value();
				try
				{
					vne->v_number = vne->v_string.ToDouble();
				}
				catch(...)
				{
					error(L"Ошибка формата потока 234. Ошибка получения числа с плавающей запятой."
						, L"Тип значения", get_node_type_presentation(tr->get_type())
						, L"Значение", tr->get_value()
						, L"Путь", spath + tr->path());
				}
			}
			else
			{
				error(L"Ошибка формата потока 114. Ошибка получения числа. Тип значения не Число"
					, L"Тип значения", get_node_type_presentation(tr->get_type())
					, L"Значение", tr->get_value()
					, L"Путь", spath + tr->path());
			}
			*ptr = tr->get_next();
		}
		else if(t == MetaTypeSet::mt_bool)
		{
			vb = new Value1C_bool(valparent);
			v = vb;
			s = tr->get_value();
			if(tr->get_type() != nd_number)
			{
				error(L"Ошибка формата потока 115. Ошибка получения значения Булево. Тип значения не Число"
					, L"Тип значения", get_node_type_presentation(tr->get_type())
					, L"Значение", s
					, L"Путь", spath + tr->path());
			}
			else if(s.Compare(L"0") != 0 && s.Compare(L"1") != 0)
			{
				error(L"Ошибка формата потока 204. Ошибка получения значения типа Булево."
					, L"Значение", s
					, L"Путь", spath + tr->path());
			}
			vb->v_bool = s.Compare(L"0") != 0;
			*ptr = tr->get_next();
		}
		else if(t == MetaTypeSet::mt_date)
		{
			vd = new Value1C_date(valparent);
			v = vd;
			if(!string1C_to_date(tr->get_value(), vd->v_date))
			{
				error(L"Ошибка формата потока 9. Ошибка разбора значения даты"
					, L"Значение", tr->get_value()
					, L"Путь", spath + tr->path());
			}
			*ptr = tr->get_next();
		}
		else if(t == MetaTypeSet::mt_undef)
		{
			v = new Value1C_undef(valparent);
			*ptr = tr->get_next();
		}
		else if(t == MetaTypeSet::mt_null)
		{
			v = new Value1C_null(valparent);
			*ptr = tr->get_next();
		}
		else if(t == MetaTypeSet::mt_type)
		{
			vt = new Value1C_type(valparent);
			v = vt;
			if(tr->get_type() == nd_guid)
			{
				if(string_to_GUID(tr->get_value(), &uid))
				{
					puninitvalues->push_back(UninitValue1C(v, spath + tr->path(), uid));
					vt->v_uid = uid;
				}
				else
				{
					error(L"Ошибка формата потока 185. Ошибка преобразования UID"
						, L"UID", tr->get_value()
						, L"Путь", spath + tr->path());
				}
			}
			else
			{
				error(L"Ошибка формата потока 186. Ожидается значение UID"
					, L"Путь", spath + tr->path());
			}

			*ptr = tr->get_next();
		}
		else if(t == MetaTypeSet::mt_typedescrinternal)
		{
			vt = new Value1C_type(valparent);
			v = vt;
			uid = EmptyUID;
			if(tr->get_type() == nd_string)
			{
				s = tr->get_value();
				if(s.CompareIC(L"S") == 0)
				{
					vt->v_type = MetaTypeSet::mt_string;
					vt->v_uid = MetaTypeSet::mt_string->uid;
				}
				else if(s.CompareIC(L"N") == 0)
				{
					vt->v_type = MetaTypeSet::mt_number;
					vt->v_uid = MetaTypeSet::mt_number->uid;
				}
				else if(s.CompareIC(L"B") == 0)
				{
					vt->v_type = MetaTypeSet::mt_bool;
					vt->v_uid = MetaTypeSet::mt_bool->uid;
				}
				else if(s.CompareIC(L"D") == 0)
				{
					vt->v_type = MetaTypeSet::mt_date;
					vt->v_uid = MetaTypeSet::mt_date->uid;
				}
				else if(s.CompareIC(L"U") == 0)
				{
					vt->v_type = MetaTypeSet::mt_undef;
					vt->v_uid = MetaTypeSet::mt_undef->uid;
				}
				else if(s.CompareIC(L"L") == 0)
				{
					vt->v_type = MetaTypeSet::mt_null;
					vt->v_uid = MetaTypeSet::mt_null->uid;
				}
				else if(s.CompareIC(L"T") == 0)
				{
					vt->v_type = MetaTypeSet::mt_type;
					vt->v_uid = MetaTypeSet::mt_type->uid;
				}
				else if(s.CompareIC(L"R") == 0)
				{
					vt->v_type = MetaTypeSet::mt_binarydata;
					vt->v_uid = MetaTypeSet::mt_binarydata->uid;
				}
				else if(s.CompareIC(L"#") == 0)
				{
					tr = tr->get_next();
					if(tr)
					{
						if(tr->get_type() == nd_guid)
						{
							if(!string_to_GUID(tr->get_value(), &uid))
							{
								uid = EmptyUID;
								error(L"Ошибка формата потока 11. Ошибка преобразования UID"
									, L"UID", tr->get_value()
									, L"Путь", spath + tr->path());
							}
						}
						else
						{
							error(L"Ошибка формата потока 12. Ожидается значение UID"
								, L"Путь", spath + tr->path());
						}
					}
					else
					{
						error(L"Ошибка формата потока 10. Отсутствует значение"
						, L"Загружаемый тип", t->name
						, L"Путь", spath);
					}
				}
				else
				{
					error(L"Ошибка формата потока 110. Неизвестный символ типа значения"
						, L"Загружаемый тип", t->name
						, L"Символ значения", s
						, L"Путь", spath + tr->path());
				}
				if(uid != EmptyUID)
				{
					puninitvalues->push_back(UninitValue1C(v, spath + tr->path(), uid));
					vt->v_uid = uid;
				}
			}
			else
			{
				error(L"Ошибка формата потока 109. Ожидается значение типа Строка"
					, L"Загружаемый тип", t->name
					, L"Тип значения", get_node_type_presentation(tr->get_type())
					, L"Значение", tr->get_value()
					, L"Путь", spath + tr->path());
			}

			*ptr = tr->get_next();
		}
		else
		{
			switch(t->serialization_ver)
			{
				case 0:
					if(t->meta) vo = new Value1C_metaobj(valparent, this);
					else vo = new Value1C_obj(valparent, this);
					vo->type = t;
					v = vo;
					if(!t->serializationtree)
					{
#ifndef VikaD
						error(L"Ошибка формата потока 13. Не определен алгоритм загрузки типа."
							, L"Загружаемый тип", t->name
							, L"Путь", spath + tr->path());
#endif //VikaD
						*ptr = tr->get_next();
						break;
					}
					loadValue1C(vo, &tr, t->serializationtree, metauid, metats, clitem, path, checkend);
					*ptr = tr;
					break;
				case 1: // Без значения. В реальности, должно было бы обработаться выше в блоке if(*ptr == NULL)
					//v->kind = kv_obj;
					error(L"Ошибка формата потока 19. Не ожидаемое значение."
						, L"Загружаемый тип", t->name
						, L"Тип значения", get_node_type_presentation(tr->get_type())
						, L"Значение", tr->get_value()
						, L"Путь", spath + tr->path());

					*ptr = tr;
					break;
				case 2:
					if(tr->get_type() == nd_number)
					{
						i = tr->get_value().ToIntDef(0);
						for(j = 0; j < t->values.size(); ++j) if(t->values[j]->value == i)
						{
							ve = new Value1C_enum(valparent);
							ve->type = t;
							v = ve;
							ve->v_enum = t->values[j];
							break;
						}
						if(!v)
						{
							error(L"Ошибка формата потока 15. Не найдено значение системного перечисления по числовому значению."
								, L"Загружаемый тип", t->name
								, L"Значение", i
								, L"Путь", spath + tr->path());
						}
					}
					else if(tr->get_type() == nd_guid)
					{
						if(!string_to_GUID(tr->get_value(), &uid))
						{
							error(L"Ошибка формата потока 20. Ошибка преобразвания UID."
								, L"Загружаемый тип", t->name
								, L"Значение", tr->get_value()
								, L"Путь", spath + tr->path());
						}
						else
						{
							for(j = 0; j < t->values.size(); ++j) if(t->values[j]->valueUID == uid)
							{
								ve = new Value1C_enum(valparent);
								ve->type = t;
								v = ve;
								ve->v_enum = t->values[j];
								break;
							}
							if(!v)
							{
								error(L"Ошибка формата потока 21. Не найдено значение системного перечисления по UID."
									, L"Загружаемый тип", t->name
									, L"Значение", tr->get_value()
									, L"Путь", spath + tr->path());
							}
						}
					}
					else
					{
						error(L"Ошибка формата потока 14. Ожидается значение типа Число или UID."
							, L"Загружаемый тип", t->name
							, L"Тип значения", get_node_type_presentation(tr->get_type())
							, L"Путь", spath + tr->path());
					}
					*ptr = tr->get_next();
					break;
				case 3:
					if(tr->get_type() == nd_number)
					{
						i = tr->get_value().ToIntDef(0);
						for(j = 0; j < t->values.size(); ++j) if(t->values[j]->value == i)
						{
							ve = new Value1C_enum(valparent);
							ve->type = t;
							v = ve;
							ve->v_enum = t->values[j];
							break;
						}
						if(!v)
						{
							error(L"Ошибка формата потока 65. Не найдено значение системного перечисления по числовому значению."
								, L"Загружаемый тип", t->name
								, L"Значение", i
								, L"Путь", spath + tr->path());
						}
					}
					else if(tr->get_type() == nd_list)
					{
						tt = tr->get_first();
						*ptr = tr->get_next();
						if(!tt)
						{
							error(L"Ошибка формата потока 66. Ожидается UID типа системного перечисления."
								, L"Загружаемый тип", t->name
								, L"Путь", spath + tr->path());
							break;
						}
						if(tt->get_type() != nd_guid)
						{
							error(L"Ошибка формата потока 67. Тип значения не UID, ожидается UID типа системного перечисления."
								, L"Загружаемый тип", t->name
								, L"Тип значения", tt->get_type()
								, L"Путь", spath + tt->path());
							break;
						}
						if(!string_to_GUID(tt->get_value(), &uid))
						{
							error(L"Ошибка формата потока 68. Ошибка преобразвания UID."
								, L"Загружаемый тип", t->name
								, L"Значение", tt->get_value()
								, L"Путь", spath + tt->path());
							break;
						}
						if(uid != t->uid)
						{
							error(L"Ошибка формата потока 69. UID типа не совпадает с UID загружаемого системного перечисления."
								, L"Загружаемый тип", t->name
								, L"UID типа", GUID_to_string(t->uid)
								, L"Загружаемый UID", tt->get_value()
								, L"Путь", spath + tt->path());
							break;
						}
						tt = tt->get_next();
						if(!tt)
						{
							error(L"Ошибка формата потока 70. Ожидается значение системного перечисления."
								, L"Загружаемый тип", t->name
								, L"Путь", spath + tr->path());
							break;
						}
						if(tt->get_type() != nd_number)
						{
							error(L"Ошибка формата потока 71. Тип значения не Число, ожидается значение системного перечисления."
								, L"Загружаемый тип", t->name
								, L"Тип значения", tt->get_type()
								, L"Путь", spath + tt->path());
							break;
						}
						i = tt->get_value().ToIntDef(0);
						for(j = 0; j < t->values.size(); ++j) if(t->values[j]->value == i)
						{
							ve = new Value1C_enum(valparent);
							ve->type = t;
							v = ve;
							ve->v_enum = t->values[j];
							break;
						}
						if(!v)
						{
							error(L"Ошибка формата потока 72. Не найдено значение системного перечисления по числовому значению."
								, L"Загружаемый тип", t->name
								, L"Значение", i
								, L"Путь", spath + tt->path());
						}
						break;
					}
					else
					{
						error(L"Ошибка формата потока 14. Ожидается значение типа Число или UID."
							, L"Загружаемый тип", t->name
							, L"Тип значения", get_node_type_presentation(tr->get_type())
							, L"Путь", spath + tr->path());
					}
					*ptr = tr->get_next();
					break;
				case 4:
					vu = new Value1C_uid(valparent);
					v = vu;
					if(tr->get_type() != nd_guid)
					{
						error(L"Ошибка формата потока 100. Тип значения не UID."
							, L"Загружаемый тип", t->name
							, L"Тип значения", tr->get_type()
							, L"Путь", spath + tr->path());
					}
					else
					{
						if(!string_to_GUID(tr->get_value(), &uid))
						{
							error(L"Ошибка формата потока 101. Ошибка преобразвания UID."
								, L"Загружаемый тип", t->name
								, L"Значение", tr->get_value()
								, L"Путь", spath + tr->path());
						}
						else vu->v_uid = uid;
					}
					*ptr = tr->get_next();
					break;
				case 5: // Стандартный реквизит
					vsa = new Value1C_stdattr(valparent);
					v = vsa;
					vsa->v_stdattr = NULL;
					if(tr->get_type() != nd_number)
					{
						error(L"Ошибка формата потока 104. Тип значения не Число при загрузке стандартного реквизита."
							, L"Загружаемый тип", t->name
							, L"Тип значения", tr->get_type()
							, L"Значение", tr->get_value()
							, L"Путь", spath + tr->path());
					}
					else
					{
						i = tr->get_value().ToIntDef(0);
						vsa->v_value = i;
						if(metauid == EmptyUID)
						{
							error(L"Ошибка формата потока 105. Ошибка загрузки стандартного реквизита. Не определён metauid."
								, L"Загружаемый тип", t->name
								, L"Значение", tr->get_value()
								, L"Путь", spath + tr->path());
						}
						else
						{
							tt = tr;
							tr = tr->get_next();
							if(tr && i >= 0)
							{
								if(tr->get_type() == nd_guid)
								{
									uid = EmptyUID;
									string_to_GUID(tr->get_value(), &uid);
									puninitvalues->push_back(UninitValue1C(v, spath + tt->path(), metauid, uid, metats));
									tr = tr->get_next();
								}
								else
								{
									error(L"Ошибка формата потока 205. Ошибка загрузки стандартного реквизита. Тип значения не UID."
										, L"Загружаемый тип", t->name
										, L"Тип значения", tr->get_type()
										, L"Значение", tr->get_value()
										, L"Путь", spath + tr->path());
									puninitvalues->push_back(UninitValue1C(v, spath + tt->path(), metauid, EmptyUID, metats));
								}
							}
							else puninitvalues->push_back(UninitValue1C(v, spath + tt->path(), metauid, EmptyUID, metats));
						}
					}
					*ptr = tr;
					break;
				case 6:
					vo = new Value1C_obj(valparent, this);
					v = vo;

					ve = new Value1C_enum(vo);
					ve->type = MetaTypeSet::mt_datefractions;
					ve->v_enum = NULL;
					vo->v_objprop[MetaTypeSet::mp_datefractions] = ve;
					if(tr->get_type() != nd_string)
					{
						error(L"Ошибка формата потока 111. Тип значения не Строка."
							, L"Загружаемый тип", t->name
							, L"Тип значения", tr->get_type()
							, L"Значение", tr->get_value()
							, L"Путь", spath + tr->path());
					}
					else
					{
						s = tr->get_value();
						if(s.CompareIC(L"D") == 0)
						{
							ve->v_enum = MetaTypeSet::mv_datefractionsdate;
						}
						else if(s.CompareIC(L"T") == 0)
						{
							ve->v_enum = MetaTypeSet::mv_datefractionstime;
						}
						else
						{
							error(L"Ошибка формата потока 112. Неизвестное значение частей даты."
								, L"Загружаемый тип", t->name
								, L"Значение", s
								, L"Путь", spath + tr->path());
						}
					}
					*ptr = tr->get_next();
					break;
				case 7: // Стандартная табличная часть
					vst = new Value1C_stdtabsec(valparent);
					v = vst;
					metats = vst;
					vst->v_stdtabsec = NULL;
					if(tr->get_type() != nd_number)
					{
						error(L"Ошибка формата потока 207. Тип значения не Число при загрузке стандартной табличной части."
							, L"Загружаемый тип", t->name
							, L"Тип значения", tr->get_type()
							, L"Значение", tr->get_value()
							, L"Путь", spath + tr->path());
					}
					else
					{
						i = tr->get_value().ToIntDef(0);
						vst->v_value = i;
						if(metauid == EmptyUID)
						{
							error(L"Ошибка формата потока 208. Ошибка загрузки стандартной табличной части. Не определён metauid."
								, L"Загружаемый тип", t->name
								, L"Значение", tr->get_value()
								, L"Путь", spath + tr->path());
						}
						else puninitvalues->push_back(UninitValue1C(v, spath + tr->path(), metauid));
					}
					*ptr = tr->get_next();
					break;
				case 8:
					vo = new Value1C_obj(valparent, this);
					v = vo;
					_metats = NULL;
					vo->type = t;
					if(tr->get_type() != nd_list)
					{
						error(L"Ошибка формата потока 216. Ошибка загрузки типа МетаСсылка. Узел не является списком."
							, L"Загружаемый тип", t->name
							, L"Тип значения", tr->get_type()
							, L"Значение", tr->get_value()
							, L"Путь", spath + tr->path());
					}
					else
					{
						tt = tr->get_first();
						if(!tt)
						{
							error(L"Ошибка формата потока 217. Ошибка загрузки типа МетаСсылка. Нет значений."
								, L"Загружаемый тип", t->name
								, L"Путь", spath + tr->path());
						}
						else
						{
							if(tt->get_type() != nd_number)
							{
								error(L"Ошибка формата потока 218. Ошибка загрузки типа МетаСсылка. Узел не является числом."
									, L"Загружаемый тип", t->name
									, L"Тип значения", tt->get_type()
									, L"Значение", tt->get_value()
									, L"Путь", spath + tt->path());
							}
							else
							{
								if(tt->get_value().Compare(L"1") != 0)
								{
									error(L"Ошибка формата потока 219 Ошибка загрузки типа МетаСсылка. Значение узла не равно 1."
										, L"Загружаемый тип", t->name
										, L"Значение", tt->get_value()
										, L"Путь", spath + tt->path());
								}
								tt = tt->get_next();
								if(tt->get_type() != nd_guid)
								{
									error(L"Ошибка формата потока 220. Ошибка загрузки типа МетаСсылка. Узел не UID."
										, L"Загружаемый тип", t->name
										, L"Тип значения", tt->get_type()
										, L"Значение", tt->get_value()
										, L"Путь", spath + tt->path());
								}
								else
								{
									vro = new Value1C_refobj(valparent);
									vro->type = MetaTypeSet::mt_metarefint;
									if(!string_to_GUID(tt->get_value(), &ouid))
									{
										error(L"Ошибка формата потока 221. Ошибка преобразования UID при загрузке ссылки на объект метаданных."
											, L"Загружаемый тип", t->name
											, L"Значение", tt->get_value()
											, L"Путь", spath + tt->path());
									}
									else if(ouid != EmptyUID)
									{
										puninitvalues->push_back(UninitValue1C(vro, spath + tr->path(), ouid));
										vro->v_uid = ouid;
									}

									prop = t->getProperty(L"Объект");
									vo->v_objprop[prop] = vro;

									tt = tt->get_next();
									if(tt->get_type() != nd_number)
									{
										error(L"Ошибка формата потока 222. Ошибка загрузки типа МетаСсылка. Узел не является числом."
											, L"Загружаемый тип", t->name
											, L"Тип значения", tt->get_type()
											, L"Значение", tt->get_value()
											, L"Путь", spath + tt->path());
									}
									else
									{
										vst = NULL;
										vsa = NULL;
										k = tt->get_value().ToIntDef(0);
										for(i = 0; i < k; ++i)
										{
											tt = tt->get_next();
											if(tt->get_type() != nd_list)
											{
												error(L"Ошибка формата потока 225. Ошибка загрузки типа МетаСсылка. Узел не является списком."
													, L"Загружаемый тип", t->name
													, L"Тип значения", tt->get_type()
													, L"Значение", tt->get_value()
													, L"Путь", spath + tt->path());
											}
											else
											{
												ttt = tt->get_first();
												if(!ttt)
												{
													error(L"Ошибка формата потока 226. Ошибка загрузки типа МетаСсылка. Нет значений."
														, L"Загружаемый тип", t->name
														, L"Путь", spath + tt->path());
												}
												else
												{
													tx = ttt;
													ttt = ttt->get_next();
													if(!ttt)
													{
														error(L"Ошибка формата потока 227. Ошибка загрузки типа МетаСсылка. Нет значений."
															, L"Загружаемый тип", t->name
															, L"Путь", spath + tx->path());
													}
													else
													{
														if(ttt->get_type() != nd_guid)
														{
															error(L"Ошибка формата потока 228. Ошибка загрузки типа МетаСсылка. Узел не UID."
																, L"Загружаемый тип", t->name
																, L"Тип значения", ttt->get_type()
																, L"Значение", ttt->get_value()
																, L"Путь", spath + ttt->path());
														}
														else
														{
															if(!string_to_GUID(ttt->get_value(), &uid))
															{
																error(L"Ошибка формата потока 229. Ошибка преобразования UID при загрузке типа МетаСсылка."
																	, L"Загружаемый тип", t->name
																	, L"Значение", ttt->get_value()
																	, L"Путь", spath + ttt->path());
															}
															else
															{
																if(uid == sig_standart_attribute)
																{
																	if(i == k - 1) // стандартный реквизит читаем только если он последний
																	{
																		vv = readValue1C(&tx, MetaTypeSet::mt_standart_attribute, vo, ouid, _metats, clitem, path, true);
																		prop = t->getProperty(L"СтандартныйРеквизит");
																		vo->v_objprop[prop] = vv;
																	}
																}
																else if(uid == sig_standart_table_sec)
																{
																	vv = readValue1C(&tx, MetaTypeSet::mt_standart_tabular_section, vo, ouid, _metats, clitem, path, true);
																	prop = t->getProperty(L"СтандартнаяТабличнаяЧасть");
																	vo->v_objprop[prop] = vv;
																}
																else if(uid == sig_ext_dimension || uid == sig_ext_dimension_type)
																{
																	vv = readValue1C(&tx, MetaTypeSet::mt_standart_attribute, vo, ouid, _metats, clitem, path, true);
																	prop = t->getProperty(L"СтандартныйРеквизит");
																	vo->v_objprop[prop] = vv;
																}
																else
																{
																	error(L"Ошибка формата потока 230. Неизвестная сигнатура при загрузке типа МетаСсылка."
																		, L"Загружаемый тип", t->name
																		, L"Значение", ttt->get_value()
																		, L"Путь", spath + ttt->path());
																}

															}

														}
													}
												}
											}

										}

										tt = tt->get_next();
										if(tt->get_type() != nd_number)
										{
											error(L"Ошибка формата потока 223. Ошибка загрузки типа МетаСсылка. Узел не является числом."
												, L"Загружаемый тип", t->name
												, L"Тип значения", tt->get_type()
												, L"Значение", tt->get_value()
												, L"Путь", spath + tt->path());
										}
										else
										{
											vb = new Value1C_bool(vo);
											if(tt->get_value().Compare(L"0") == 0) b = false;
											else if(tt->get_value().Compare(L"1") == 0) b = true;
											else
											{
												error(L"Ошибка формата потока 224. Ошибка загрузки типа Булево."
													, L"Загружаемый тип", t->name
													, L"Значение", tt->get_value()
													, L"Путь", spath + tt->path());
												b = false;
											}
											vb->v_bool = b;
											prop = t->getProperty(L"Подчиненный");
											vo->v_objprop[prop] = vb;
										}
									}
								}
							}
						}
					}
					*ptr = tr->get_next();
					break;
				default:
					error(L"Ошибка формата потока 16. Неизвестный вариант сериализации."
						, L"Загружаемый тип", t->name
						, L"Вариант сериализации", t->serialization_ver
						, L"Путь", spath + tr->path());
					*ptr = tr->get_next();
			}
		}
	}

	if(vo) vo->fillpropv();
	return v;
}

//---------------------------------------------------------------------------
void __fastcall MetaContainer::loadValue1C(Value1C_obj* v, tree** ptr, const SerializationTreeNode* tn, TGUID metauid, Value1C_stdtabsec* metats, ClassItem* clitem, String& path, bool checkend)
{
	MetaType* t;
	MetaType* nt;
	VarValue* varvalues;
	unsigned int i, j;
	int k, l;
	std::map<MetaProperty*, Value1C*, MetaPropertyLess>::iterator ip;
	MetaProperty* p;
	MetaGeneratedType* gt;
	SerializationTreeVar* var;
	ExternalFile* ext;

	bool cv, ok;
	SerializationTreeValueType vt1, vt2;
	String sv1, sv2;
	int nv1, nv2;
	TGUID uv1, uv2;
	Value1C* vv;
	Value1C_obj* vo;
	Value1C_number* vn;
	std::map<String, int>::iterator icv;
	String spath, npath;
	tree* tt;
	tree* tte;
	//TStreamReader* reader;
	TStream* str;
	MetaValue* mv;
	DefaultValueType dvt;

	MetaProperty* prop;
	int ni, ui;
	unsigned int ii;
	Value1C_obj* vValues;
	Value1C_obj* vColumns;
	Value1C_obj* vColumnsAndValuesMap;
	Value1C_obj* vStrings;
	Value1C_binary* vb;
	Value1C_extobj* veo;
	bool nok, uok;

	CongigFile* cf;
	CongigFile* cfc;
	String sn;
	THandle handle;

	spath = storpresent + path;
	t = tn->owner;
	varvalues = NULL;
	i = t->serializationvars.size();
	if(i)
	{
		varvalues = new VarValue[i];
		for(j = 0; j < i; ++j)
		{
			var = t->serializationvars[j];
			if(var->isglobal) if(var->isfix) v->globalvars[var->name.UpperCase()] = var->fixvalue;
		}
	}

	recursiveLoadValue1C(v, varvalues, ptr, tn, metauid, metats, clitem, path, checkend);

	if(v->kind == kv_metaobj) if(!((Value1C_metaobj*)v)->v_metaobj)
	{
		error(L"Ошибка формата потока 203. Не загружен UID объекта метаданных."
			, L"Загружаемый тип", t->name
			, L"Путь", spath);
	}

	// ==> Чтение внешних файлов ===========
	for(i = 0; i < t->externalfiles.size(); ++i)
	{
		ext = t->externalfiles[i];
		if(ext->havecondition)
		{
			cv = false;

			//Значение 1
			vt1 = stv_none;
			switch(ext->typeval1)
			{
				case stv_string:
					vt1 = stv_string;
					sv1 = ext->str1;
					break;
				case stv_number:
					vt1 = stv_number;
					nv1 = ext->num1;
					break;
				case stv_uid:
					vt1 = stv_uid;
					uv1 = ext->uid1;
					break;
				case stv_value:
					mv = ext->val1;
					if(mv->valueUID == EmptyUID)
					{
						vt1 = stv_number;
						nv1 = mv->value;
					}
					else
					{
						vt1 = stv_uid;
						uv1 = mv->valueUID;
					}
					break;
				case stv_var:
					vt1 = stv_number;
					nv1 = getVarValue(ext->str1, t, varvalues, clitem, spath);
					break;
				case stv_prop:
					ip = v->v_objprop.find(ext->prop1);
					if(ip == v->v_objprop.end())
					{
						p = ext->prop1;
						dvt = p->defaultvaluetype;
						switch(dvt)
						{
							case dvt_bool:
								vt1 = stv_number;
								nv1 = p->dv_bool ? 1 : 0;
								break;
							case dvt_number:
								vt1 = stv_number;
								nv1 = p->dv_number;
								break;
							case dvt_string:
								vt1 = stv_string;
								sv1 = *(p->dv_string);
								break;
							case dvt_type:
								vt1 = stv_uid;
								uv1 = p->dv_type->uid;
								break;
							case dvt_enum:
								mv = p->dv_enum;
								if(mv->valueUID == EmptyUID)
								{
									vt1 = stv_number;
									nv1 = mv->value;
								}
								else
								{
									vt1 = stv_uid;
									uv1 = mv->valueUID;
								}
								break;
							default:
								error(L"Ошибка формата потока 126. Ошибка вычисления условия внешнего файла. Не найдено значение свойства"
									, L"Загружаемый тип", t->name
									, L"Свойство", p->name
									, L"Путь", spath);
						}
					}
					else
					{
						vv = ip->second;
						if(vv->kind == kv_string)
						{
							vt1 = stv_string;
							sv1 = ((Value1C_string*)vv)->v_string;
						}
						else if(vv->kind == kv_number)
						{
							vt1 = stv_number;
							nv1 = ((Value1C_number*)vv)->v_number;
						}
						else if(vv->kind == kv_number_exp)
						{
							vt1 = stv_number;
							nv1 = (int)((Value1C_number_exp*)vv)->v_number;
						}
						else if(vv->kind == kv_uid)
						{
							vt1 = stv_uid;
							uv1 = ((Value1C_uid*)vv)->v_uid;
						}
						else if(vv->kind == kv_enum)
						{
							mv = ((Value1C_enum*)vv)->v_enum;
							if(mv->valueUID == EmptyUID)
							{
								vt1 = stv_number;
								nv1 = mv->value;
							}
							else
							{
								vt1 = stv_uid;
								uv1 = mv->valueUID;
							}
						}
						else if(vv->kind == kv_bool)
						{
							vt1 = stv_number;
							nv1 = ((Value1C_bool*)vv)->v_bool ? 1 : 0;
						}
						else if(vv->kind == kv_type)
						{
							vt1 = stv_uid;
							uv1 = ((Value1C_type*)vv)->v_uid;
						}
						else if(vv->kind == kv_binary)
						{
							vt1 = stv_string;
							sv1 = vv->presentation();
						}
						else
						{
							error(L"Ошибка формата потока 127. Ошибка вычисления условия внешнего файла. Недопустимый тип значения свойства"
								, L"Загружаемый тип", t->name
								, L"Свойство", ext->prop1->name
								, L"Тип значения", vv->kind
								, L"Путь", spath);
						}
					}
					break;
				case stv_vercon:
					vt1 = stv_number;
					nv1 = (int)ext->vercon1;
					break;
				case stv_ver1C:
					vt1 = stv_number;
					nv1 = (int)ext->ver1C1;
					break;
				case stv_classpar:
					vt1 = stv_number;
					if(clitem) nv1 = clitem->cl->getparamvalue(ext->classpar1);
					else
					{
						error(L"Ошибка формата потока 128. Ошибка вычисления условия внешнего файла. Класс не определён."
							, L"Загружаемый тип", t->name
							, L"Параметр класса", ext->classpar1->name
							, L"Тип значения", vv->kind
							, L"Путь", spath);
						nv1 = -1;
					}
					break;
				case stv_globalvar:
					vt1 = stv_number;
					nv1 = 0;
					ok = false;
					for(vo = v->parent; vo; vo = vo->parent)
					{
						icv = vo->globalvars.find(ext->str1.UpperCase());
						if(icv != vo->globalvars.end())
						{
							nv1 = icv->second;
							ok = true;
							break;
						}
					}
					if(!ok)
					{
						error(L"Ошибка формата потока 129. Ошибка получения первого значения условия внешнего файла. Не удалось получить значение глобальной переменной"
							, L"Загружаемый тип", t->name
							, L"Глобальная переменная", ext->str1
							, L"Путь", spath);
					}
					break;
				default:
					error(L"Ошибка формата потока 130. Ошибка вычисления условия внешнего файла. Недопустимый тип значения 1"
						, L"Загружаемый тип", t->name
						, L"Тип значения", ext->typeval1presentation()
						, L"Путь", spath);
			}

			//Значение 2
			vt2 = stv_none;
			switch(ext->typeval2)
			{
				case stv_string:
					vt2 = stv_string;
					sv2 = ext->str2;
					break;
				case stv_number:
					vt2 = stv_number;
					nv2 = ext->num2;
					break;
				case stv_uid:
					vt2 = stv_uid;
					uv2 = ext->uid2;
					break;
				case stv_value:
					mv = ext->val2;
					if(mv->valueUID == EmptyUID)
					{
						vt2 = stv_number;
						nv2 = mv->value;
					}
					else
					{
						vt2 = stv_uid;
						uv2 = mv->valueUID;
					}
					break;
				case stv_var:
					vt2 = stv_number;
					nv2 = getVarValue(ext->str2, t, varvalues, clitem, spath);
					break;
				case stv_prop:
					ip = v->v_objprop.find(ext->prop2);
					if(ip == v->v_objprop.end())
					{
						p = ext->prop2;
						dvt = p->defaultvaluetype;
						switch(dvt)
						{
							case dvt_bool:
								vt2 = stv_number;
								nv2 = p->dv_bool ? 1 : 0;
								break;
							case dvt_number:
								vt2 = stv_number;
								nv2 = p->dv_number;
								break;
							case dvt_string:
								vt2 = stv_string;
								sv2 = *(p->dv_string);
								break;
							case dvt_type:
								vt2 = stv_uid;
								uv2 = p->dv_type->uid;
								break;
							case dvt_enum:
								mv = p->dv_enum;
								if(mv->valueUID == EmptyUID)
								{
									vt2 = stv_number;
									nv2 = mv->value;
								}
								else
								{
									vt2 = stv_uid;
									uv2 = mv->valueUID;
								}
								break;
							default:
								error(L"Ошибка формата потока 131. Ошибка вычисления условия внешнего файла. Не найдено значение свойства"
									, L"Загружаемый тип", t->name
									, L"Свойство", p->name
									, L"Путь", spath);
						}
					}
					else
					{
						vv = ip->second;
						if(vv->kind == kv_string)
						{
							vt2 = stv_string;
							sv2 = ((Value1C_string*)vv)->v_string;
						}
						else if(vv->kind == kv_number)
						{
							vt2 = stv_number;
							nv2 = ((Value1C_number*)vv)->v_number;
						}
						else if(vv->kind == kv_number_exp)
						{
							vt2 = stv_number;
							nv2 = (int)((Value1C_number_exp*)vv)->v_number;
						}
						else if(vv->kind == kv_uid)
						{
							vt2 = stv_uid;
							uv2 = ((Value1C_uid*)vv)->v_uid;
						}
						else if(vv->kind == kv_enum)
						{
							mv = ((Value1C_enum*)vv)->v_enum;
							if(mv->valueUID == EmptyUID)
							{
								vt2 = stv_number;
								nv2 = mv->value;
							}
							else
							{
								vt2 = stv_uid;
								uv2 = mv->valueUID;
							}
						}
						else if(vv->kind == kv_bool)
						{
							vt2 = stv_number;
							nv2 = ((Value1C_bool*)vv)->v_bool ? 1 : 0;
						}
						else if(vv->kind == kv_type)
						{
							vt2 = stv_uid;
							uv2 = ((Value1C_uid*)vv)->v_uid;
						}
						else if(vv->kind == kv_binary)
						{
							vt2 = stv_string;
							sv2 = vv->presentation();
						}
						else
						{
							error(L"Ошибка формата потока 132. Ошибка вычисления условия внешнего файла. Недопустимый тип значения свойства"
								, L"Загружаемый тип", t->name
								, L"Свойство", ext->prop2->name
								, L"Тип значения", vv->kind
								, L"Путь", spath);
						}
					}
					break;
				case stv_vercon:
					vt2 = stv_number;
					nv2 = (int)ext->vercon2;
					break;
				case stv_ver1C:
					vt2 = stv_number;
					nv2 = (int)ext->ver1C2;
					break;
				case stv_classpar:
					vt2 = stv_number;
					if(clitem) nv2 = clitem->cl->getparamvalue(ext->classpar2);
					else
					{
						error(L"Ошибка формата потока 133. Ошибка вычисления условия внешнего файла. Класс не определён."
							, L"Загружаемый тип", t->name
							, L"Параметр класса", ext->classpar2->name
							, L"Тип значения", vv->kind
							, L"Путь", spath);
						nv2 = -1;
					}
					break;
				case stv_globalvar:
					vt2 = stv_number;
					nv2 = 0;
					ok = false;
					for(vo = v->parent; vo; vo = vo->parent)
					{
						icv = vo->globalvars.find(ext->str2.UpperCase());
						if(icv != vo->globalvars.end())
						{
							nv2 = icv->second;
							ok = true;
							break;
						}
					}
					if(!ok)
					{
						error(L"Ошибка формата потока 134. Ошибка получения второго значения условия внешнего файла. Не удалось получить значение глобальной переменной"
							, L"Загружаемый тип", t->name
							, L"Глобальная переменная", ext->str2
							, L"Путь", spath);
					}
					break;
				default:
					error(L"Ошибка формата потока 135. Ошибка вычисления условия внешнего файла. Недопустимый тип значения 2"
						, L"Загружаемый тип", t->name
						, L"Тип значения", ext->typeval2presentation()
						, L"Путь", spath);
			}
			if(vt1 != stv_none && vt2 != stv_none)
			{
				if(vt1 != vt2)
				{
					error(L"Ошибка формата потока 136. Ошибка вычисления условия внешнего файла. Несовпадающие типы сравниваемых значений"
						, L"Загружаемый тип", t->name
						, L"Тип значения 1", vt1
						, L"Тип значения 2", vt2
						, L"Путь", spath);
				}
				else
				{
					if(vt1 == stv_string)
					{
						switch(ext->condition)
						{
							case stc_e:
								cv = sv1.CompareIC(sv2) == 0;
								break;
							case stc_ne:
								cv = sv1.CompareIC(sv2) != 0;
								break;
							case stc_l:
								cv = sv1.CompareIC(sv2) < 0;
								break;
							case stc_g:
								cv = sv1.CompareIC(sv2) > 0;
								break;
							case stc_le:
								cv = sv1.CompareIC(sv2) <= 0;
								break;
							case stc_ge:
								cv = sv1.CompareIC(sv2) >= 0;
								break;
							default:
								error(L"Ошибка формата потока 137. Ошибка вычисления условия внешнего файла. Некорректное условие при сравнении строк"
									, L"Загружаемый тип", t->name
									, L"Условие", ext->condition
									, L"Путь", spath);
						}
					}
					else if(vt1 == stv_number)
					{
						switch(ext->condition)
						{
							case stc_e:
								cv = nv1 == nv2;
								break;
							case stc_ne:
								cv = nv1 != nv2;
								break;
							case stc_l:
								cv = nv1 < nv2;
								break;
							case stc_g:
								cv = nv1 > nv2;
								break;
							case stc_le:
								cv = nv1 <= nv2;
								break;
							case stc_ge:
								cv = nv1 >= nv2;
								break;
							case stc_bs:
								if(nv2 < 0 || nv2 > 31)
								{
									error(L"Ошибка формата потока 191. Ошибка вычисления условия внешнего файла. Номер бита за пределами 0-31"
										, L"Загружаемый тип", t->name
										, L"Номер бита", nv2
										, L"Путь", spath);
									break;
								}
								cv = (nv1 & (1 << nv2)) != 0;
								break;
							case stc_bn:
								if(nv2 < 0 || nv2 > 31)
								{
									error(L"Ошибка формата потока 192. Ошибка вычисления условия внешнего файла. Номер бита за пределами 0-31"
										, L"Загружаемый тип", t->name
										, L"Номер бита", nv2
										, L"Путь", spath);
									break;
								}
								cv = (nv1 & (1 << nv2)) == 0;
								break;
							default:
								error(L"Ошибка формата потока 138. Ошибка вычисления условия внешнего файла. Некорректное условие при сравнении чисел"
									, L"Загружаемый тип", t->name
									, L"Условие", ext->condition
									, L"Путь", spath);
						}
					}
					else if(vt1 == stv_uid)
					{
						switch(ext->condition)
						{
							case stc_e:
								cv = uv1 == uv2;
								break;
							case stc_ne:
								cv = uv1 != uv2;
								break;
							case stc_l:
								cv = uv1 < uv2;
								break;
							case stc_g:
								cv = uv2 < uv1;
								break;
							case stc_le:
								cv = uv1 < uv2 || uv1 == uv2;
								break;
							case stc_ge:
								cv = uv2 < uv1 || uv1 == uv2;
								break;
							default:
								error(L"Ошибка формата потока 139. Ошибка вычисления условия внешнего файла. Некорректное условие при сравнении UID"
									, L"Загружаемый тип", t->name
									, L"Условие", ext->condition
									, L"Путь", spath);
						}
					}
				}
			}
			if(!cv) continue;
		}

		npath = L"";
		if(ext->relativepath)
		{
			k = path.LastDelimiter(L"\\/");
			npath = path.SubString(1, k);
		}

		if(ext->name.CompareIC(L"<Мета ИД>") == 0)
		{
			if(v->kind == kv_metaobj)
			{
				if(((Value1C_metaobj*)v)->v_metaobj) npath += GUID_to_string(((Value1C_metaobj*)v)->v_metaobj->uid);
				else
				{
					error(L"Ошибка формата потока 144. Ошибка получения <Мета ИД> при определении имени внешнего файла. UID объекта метаданных не загружен"
						, L"Загружаемый тип", t->name
						, L"Путь", spath);
					continue;
				}
			}
			else
			{
				error(L"Ошибка формата потока 141. Ошибка получения <Мета ИД> при определении имени внешнего файла. Значение 1С не является объектом метаданных"
					, L"Загружаемый тип", t->name
					, L"Вид значения 1С", KindOfValue1C_presantation(v->kind)
					, L"Путь", spath);
				continue;
			}
		}
		else npath += ext->name;
		if(ext->ext.Length() > 0)
		{
			npath += L".";
			npath += ext->ext;
		}
		cfc = NULL;
		if(ext->catalog)
		{
			cfc = stor->readfile(npath);
			npath += L"\\";
			npath += ext->filename;
		}

		nt = ext->type;
		if(!nt) if(ext->prop->types.size() == 1) nt = ext->prop->types[0];
		switch(ext->format)
		{
			case eff_servalue:

				prop = ext->prop;
				if(useExternal && !prop->predefined)
				{
					if(stor->fileexists(npath))
					{
						veo = new Value1C_extobj(v, this, npath, nt, metauid);
						vo = veo;
					}
					else
					{
						if(ext->optional) break;
						error(L"Ошибка формата потока 200. Не найден внешний файл"
							, L"Загружаемый тип", t->name
							, L"Свойство", prop->name
							, L"Путь", storpresent + npath);
						vo = NULL;
					}
				}
				else
				{
					tt = gettree(stor, npath, !ext->optional);
					if(!tt)
					{
						if(ext->optional) break;
						error(L"Ошибка формата потока 142. Не найден или пустой внешний файл"
							, L"Загружаемый тип", t->name
							, L"Путь", storpresent + npath);
						vo = NULL;
					}
					else
					{
						tte = tt->get_first();
						vo = (Value1C_obj*)readValue1C(&tte, nt, v, metauid, metats, clitem, npath);
						delete tt;
					}
				}

				v->v_objprop[prop] = vo;
				// ==> Обработка предопределенных элементов
				if(prop->predefined)
				{
					ni = v->type->prenameindex;
					ui = v->type->preidindex;
					ok = true;
					if(ni == ui)
					{
						error(L"Ошибка формата потока 165. Не установлены индексы колонок предопределенных значений"
							, L"Загружаемый тип", t->name
							, L"Путь ДС", tn->path()
							, L"Свойство", prop->name
							, L"Путь", spath);
						ok = false;
					}
					if(ok)
					{
						vValues = (Value1C_obj*)vo->getproperty(L"Значения");
						if(!vValues)
						{
							error(L"Ошибка формата потока 166. Не найдено свойство \"Значения\" при разборое предопределенных значений"
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Свойство", prop->name
								, L"Путь", spath);
							ok = false;
						}
					}
					if(ok)
					{
						vColumns = (Value1C_obj*)vValues->getproperty(L"Колонки");
						if(!vColumns)
						{
							error(L"Ошибка формата потока 167. Не найдено свойство \"Колонки\" при разборое предопределенных значений"
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Свойство", prop->name
								, L"Путь", spath);
							ok = false;
						}
					}
					if(ok)
					{
						vColumnsAndValuesMap = (Value1C_obj*)vValues->getproperty(L"СоответствиеКолонокИЗначений");
						if(!vColumnsAndValuesMap)
						{
							error(L"Ошибка формата потока 168. Не найдено свойство \"СоответствиеКолонокИЗначений\" при разборое предопределенных значений"
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Свойство", prop->name
								, L"Путь", spath);
							ok = false;
						}
					}
					if(ok)
					{
						vStrings = (Value1C_obj*)vValues->getproperty(L"Строки");
						if(!vStrings)
						{
							error(L"Ошибка формата потока 169. Не найдено свойство \"Строки\" при разборое предопределенных значений"
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Свойство", prop->name
								, L"Путь", spath);
							ok = false;
						}
					}
					if(ok)
					{
						if(ni >= (int)vColumns->v_objcol.size())
						{
							error(L"Ошибка формата потока 170. Индекс колонки имени предопределенного элемента больше количества колонок"
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Свойство", prop->name
								, L"Путь", spath);
							ok = false;
						}
					}
					if(ok)
					{
						if(ui >= (int)vColumns->v_objcol.size())
						{
							error(L"Ошибка формата потока 171. Индекс колонки идентификатора предопределенного элемента больше количества колонок"
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Свойство", prop->name
								, L"Путь", spath);
							ok = false;
						}
					}
					if(ok)
					{
						vo = (Value1C_obj*)vColumns->v_objcol[ni];
						if(!vo)
						{
							error(L"Ошибка формата потока 172. Ошибка получения колонки имени предопределенного элемента"
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Свойство", prop->name
								, L"Путь", spath);
							ok = false;
						}
					}
					if(ok)
					{
						vn = (Value1C_number*)vo->getproperty(L"УникальныйНомер");
						if(!vn)
						{
							error(L"Ошибка формата потока 173. Ошибка получения уникального номера колонки имени предопределенного элемента"
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Свойство", prop->name
								, L"Путь", spath);
							ok = false;
						}
					}
					if(ok)
					{
						ni = vn->v_number;
						vo = (Value1C_obj*)vColumns->v_objcol[ui];
						if(!vo)
						{
							error(L"Ошибка формата потока 174. Ошибка получения колонки идентификатора предопределенного элемента"
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Свойство", prop->name
								, L"Путь", spath);
							ok = false;
						}
					}
					if(ok)
					{
						vn = (Value1C_number*)vo->getproperty(L"УникальныйНомер");
						if(!vn)
						{
							error(L"Ошибка формата потока 175. Ошибка получения уникального номера колонки идентификатора предопределенного элемента"
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Свойство", prop->name
								, L"Путь", spath);
							ok = false;
						}
					}
					if(ok)
					{
						ui = vn->v_number;
						nok = uok = false;
						for(ii = 0; ii < vColumnsAndValuesMap->v_objcol.size(); ++ii)
						{
							vo = (Value1C_obj*)vColumnsAndValuesMap->v_objcol[ii];
							if(!vo)
							{
								error(L"Ошибка формата потока 176. Ошибка получения соответствия уникального номера колонки и индекса значения при разборе предопределенных элементов"
									, L"Загружаемый тип", t->name
									, L"Путь ДС", tn->path()
									, L"Свойство", prop->name
									, L"Путь", spath);
								ok = false;
								break;
							}
							vn = (Value1C_number*)vo->getproperty(L"УникальныйНомерКолонки");
							if(!vn)
							{
								error(L"Ошибка формата потока 177. Ошибка получения уникального номера колонки в соответствии колонок и значений при разборе предопределенных элементов"
									, L"Загружаемый тип", t->name
									, L"Путь ДС", tn->path()
									, L"Свойство", prop->name
									, L"Путь", spath);
								ok = false;
								break;
							}
							if(!nok)
							{
								if(ni == vn->v_number)
								{
									vn = (Value1C_number*)vo->getproperty(L"ИндексЗначения");
									if(!vn)
									{
										error(L"Ошибка формата потока 178. Ошибка получения индекса значения в соответствии колонок и значений при разборе предопределенных элементов"
											, L"Загружаемый тип", t->name
											, L"Путь ДС", tn->path()
											, L"Свойство", prop->name
											, L"Путь", spath);
										ok = false;
										break;
									}
									ni = vn->v_number;
									nok = true;
								}
							}
							if(!uok)
							{
								if(ui == vn->v_number)
								{
									vn = (Value1C_number*)vo->getproperty(L"ИндексЗначения");
									if(!vn)
									{
										error(L"Ошибка формата потока 178. Ошибка получения индекса значения в соответствии колонок и значений при разборе предопределенных элементов"
											, L"Загружаемый тип", t->name
											, L"Путь ДС", tn->path()
											, L"Свойство", prop->name
											, L"Путь", spath);
										ok = false;
										break;
									}
									ui = vn->v_number;
									uok = true;
								}
							}
							if(nok && uok) break;
						}
					}
					if(ok)
					{
						readPredefinedValues((Value1C_metaobj*)v, ni, ui, vStrings, spath);
					}
				}
				// <== Обработка предопределенных элементов
				break;
			case eff_text:
			case eff_tabdoc:
			case eff_binary:
			case eff_activedoc:
			case eff_htmldoc:
			case eff_textdoc:
			case eff_geo:
			case eff_kd:
			case eff_mkd:
			case eff_graf:
			case eff_xml:
			case eff_wsdl:
			case eff_picture:
				cf = stor->readfile(npath);
				if(cf)
				{
					vb = new Value1C_binary(v);
					vb->type = nt;
					vb->v_binformat = ext->format;
					vb->v_binary = new TTempStream();
					vb->v_binary->CopyFrom(cf->str, 0);
					stor->close(cf);
				}
				else
				{
					if(ext->optional) break;
					error(L"Ошибка формата потока 145. Не найден внешний файл"
						, L"Загружаемый тип", t->name
						, L"Путь", storpresent + npath);
					vb = new Value1C_binary(v);
					vb->type = nt;
				}
				v->v_objprop[ext->prop] = vb;
				break;
			default:
				error(L"Ошибка формата потока 143. Неизвестное значение формата внешнего файла"
					, L"Загружаемый тип", t->name
					, L"Формат", ext->format
					, L"Путь", storpresent + npath);

		}
		if(cfc) stor->close(cfc);
	}
	// <== Чтение внешних файлов ===========

	delete[] varvalues;
}

//---------------------------------------------------------------------------
void __fastcall MetaContainer::recursiveLoadValue1C(Value1C_obj* v, VarValue* varvalues, tree** ptr, const SerializationTreeNode* tn, TGUID& metauid, Value1C_stdtabsec*& metats, ClassItem* clitem, String& path, bool checkend)
{
	MetaType* t;
	TGUID uid;
	tree* tr;
	tree* tt;
	Value1C* nv;
	Value1C_metaobj* vm;
	Value1C_metaobj* vvm;
	Value1C_obj* vo;
	Value1C_string* vs;
	Value1C_number* vn;
	Value1C* vv;
	Value1C* vvv;
	MetaType* nt;
	int i,j;
	GeneratedType* gt;
	SerializationTreeNode* tnn;

	SerializationTreeValueType vt1, vt2;
	String sv1, sv2;
	int nv1, nv2;
	TGUID uv1, uv2;
	bool cv, ok;
	std::map<MetaProperty*, Value1C*, MetaPropertyLess>::iterator ip;
	MetaProperty* prop;
	Class* cl;
	ClassItem* cli;
	std::map<String, int>::iterator icv;
	String spath;
	MetaValue* mv;
	//Value1C_stdtabsec* _metats;

	unsigned int ii;
	TGUID u;
	String n;
	PredefinedValue* pre;
	MetaProperty* p;
	DefaultValueType dvt;

	spath = storpresent + path;

	tr = *ptr;
	if(!tn)
	{
		if(checkend) if(tr)
		{
			error(L"Ошибка формата потока 63. Остались необработанные значения."
				, L"Путь", spath + tr->path());
		}
		return;
	}

	t = tn->owner;

	for(; tn; tn = tn->next)
	{
		switch(tn->type)
		{
			case stt_const:
				if(!tr)
				{
					error(L"Ошибка формата потока 27. Отсутствует ожидаемое значение константы."
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Путь", spath);
					break;
				}
				switch(tn->typeval1)
				{
					case stv_string:
						if(tr->get_type() == nd_string)
						{
							if(tr->get_value().CompareIC(tn->str1) != 0)
							{
								error(L"Ошибка формата потока 26. Значение не совпадает с константой дерева сериализации."
									, L"Загружаемый тип", t->name
									, L"Путь ДС", tn->path()
									, L"Значение", tr->get_value()
									, L"Значение константы", tn->str1
									, L"Путь", spath + tr->path());
							}

						}
						else
						{
							error(L"Ошибка формата потока 25. Тип значения не совпадает с типом значения константы дерева сериализации."
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Тип значения", get_node_type_presentation(tr->get_type())
								, L"Тип значения константы", tn->typeval1presentation()
								, L"Путь", spath + tr->path());
						}
						break;
					case stv_number:
						if(tr->get_type() == nd_number)
						{
							if(tr->get_value().ToIntDef(0) != tn->num1)
							{
								error(L"Ошибка формата потока 28. Значение не совпадает с константой дерева сериализации."
									, L"Загружаемый тип", t->name
									, L"Путь ДС", tn->path()
									, L"Значение", tr->get_value()
									, L"Значение константы", tn->num1
									, L"Путь", spath + tr->path());
							}

						}
						else if(tr->get_type() == nd_number_exp)
						{
							if(tr->get_value().ToDouble() != (double)tn->num1)
							{
								error(L"Ошибка формата потока 235. Значение не совпадает с константой дерева сериализации."
									, L"Загружаемый тип", t->name
									, L"Путь ДС", tn->path()
									, L"Значение", tr->get_value()
									, L"Значение константы", tn->num1
									, L"Путь", spath + tr->path());
							}

						}
						else
						{
							error(L"Ошибка формата потока 29. Тип значения не совпадает с типом значения константы дерева сериализации."
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Тип значения", get_node_type_presentation(tr->get_type())
								, L"Тип значения константы", tn->typeval1presentation()
								, L"Путь", spath + tr->path());
						}
						break;
					case stv_uid:
						if(tr->get_type() == nd_guid)
						{
							if(!string_to_GUID(tr->get_value(), &uid))
							{
								error(L"Ошибка формата потока 30. Ошибка преобразования UID при загрузке константы."
									, L"Загружаемый тип", t->name
									, L"Путь ДС", tn->path()
									, L"Значение", tr->get_value()
									, L"Значение константы", GUID_to_string(tn->uid1)
									, L"Путь", spath + tr->path());
							}
							else if(uid != tn->uid1)
							{
								error(L"Ошибка формата потока 31. Значение не совпадает с константой дерева сериализации."
									, L"Загружаемый тип", t->name
									, L"Путь ДС", tn->path()
									, L"Значение", tr->get_value()
									, L"Значение константы", GUID_to_string(tn->uid1)
									, L"Путь", spath + tr->path());
							}

						}
						else
						{
							error(L"Ошибка формата потока 32. Тип значения не совпадает с типом значения константы дерева сериализации."
								, L"Загружаемый тип", t->name
									, L"Путь ДС", tn->path()
								, L"Тип значения", get_node_type_presentation(tr->get_type())
								, L"Тип значения константы", tn->typeval1presentation()
								, L"Путь", spath + tr->path());
						}
						break;
					default:
						error(L"Ошибка формата потока 24. Недопустимыйый тип значения константы дерева сериализации."
							, L"Загружаемый тип", t->name
							, L"Путь ДС", tn->path()
							, L"Тип значения", tn->typeval1presentation()
							, L"Путь", spath + tr->path());
				}
				if(!tn->nomove) tr = tr->get_next();
				break;
			case stt_var:
				if(!tr)
				{
					error(L"Ошибка формата потока 94. Отсутствует ожидаемое значение переменной."
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Путь", spath);
					break;
				}
				if(tr->get_type() == nd_number) setVarValue(tn->str1, t, v, varvalues, clitem, tr->get_value().ToIntDef(0), spath);
				else
				{
					error(L"Ошибка формата потока 33. Тип переменной не Число."
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Тип переменной", get_node_type_presentation(tr->get_type())
						, L"Имя переменной", tn->str1
						, L"Путь", spath + tr->path());
				}
				if(!tn->nomove) tr = tr->get_next();
				break;
			case stt_list:
				if(!tr)
				{
					error(L"Ошибка формата потока 95. Отсутствует ожидаемый список."
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Путь", spath);
					break;
				}
				if(tr->get_type() != nd_list)
				{
					error(L"Ошибка формата потока 35. Значение не является списком."
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Путь", spath + tr->path());
				}
				else
				{
					tt = tr->get_first();
					recursiveLoadValue1C(v, varvalues, &tt, tn->first, metauid, metats, clitem, path, true);
				}
				if(!tn->nomove) tr = tr->get_next();
				break;
			case stt_prop:
				if(!tr)
				{
					if(!tn->nomove)
					error(L"Ошибка формата потока 96. Отсутствует ожидаемое значение свойства."
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Путь", spath);
					break;
				}
				if(tn->isref) nt = MetaTypeSet::mt_metarefint; // Для ссылки на объект метаданных жестко задаем тип ОбъектМетаданныхСсылкаВнутр
				else
				{
					nt = tn->typeprop;
					if(!nt) if(tn->prop1->types.size() == 1) nt = tn->prop1->types[0];
				}

				//_metats = metats;
				prop = tn->prop2;
				if(prop)
				{
					vv = v->getproperty(prop);
					if(vv)
					{
						if(vv->kind == kv_obj && vv->type == MetaTypeSet::mt_metaobjref) // ОбъектМетаданныхСсылка
						{
							vv = ((Value1C_obj*)vv)->getproperty(L"ОбъектМетаданных");
						}
					}
					if(vv)
					{
						if(vv->kind == kv_metaobj)
						{
							if(((Value1C_metaobj*)vv)->v_metaobj) uv1 = ((Value1C_metaobj*)vv)->v_metaobj->uid;
							else
							{
								error(L"Ошибка формата потока 199. Ошибка получения metauid из свойства. metauid еще не загружен."
									, L"Загружаемый тип", t->name
									, L"Путь ДС", tn->path()
									, L"Свойство", prop->name
									, L"Путь", spath);
								uv1 = EmptyUID;
							}
						}
						else if(vv->kind == kv_refobj) uv1 = ((Value1C_refobj*)vv)->v_uid;
						else if(vv->kind == kv_obj && vv->type == MetaTypeSet::mt_tabsection)
						{
							vvv = ((Value1C_obj*)vv)->getproperty(L"ТабличнаяЧасть");
							if(vvv)
							{
								if(vvv->kind == kv_refobj) uv1 = ((Value1C_refobj*)vvv)->v_uid;
								//else if(vvv->kind == kv_stdtabsec) _metats = (Value1C_stdtabsec*)vvv;
								else if(vvv->kind == kv_stdtabsec) metats = (Value1C_stdtabsec*)vvv;
							}
						}
						else if(vv->kind == kv_obj && vv->type == MetaTypeSet::mt_metaref)
						{
							vvv = ((Value1C_obj*)vv)->getproperty(L"Объект");
							if(vvv) uv1 = ((Value1C_refobj*)vvv)->v_uid;
							else
							{
								error(L"Ошибка формата потока 231. Ошибка получения свойства Объект из значения типа МетаСсылка."
									, L"Загружаемый тип", t->name
									, L"Путь ДС", tn->path()
									, L"Свойство", prop->name
									, L"Путь", spath);
								uv1 = EmptyUID;
							}
						}
						else
						{
							error(L"Ошибка формата потока 103. Ошибка получения metauid из свойства. Свойство имеет недопустимый тип."
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Свойство", prop->name
								, L"Тип значения", KindOfValue1C_presantation(vv->kind)
								, L"Путь", spath);
							uv1 = EmptyUID;
						}
					}
					else
					{
						error(L"Ошибка формата потока 102. Ошибка получения metauid из свойства. Свойство не определено"
							, L"Загружаемый тип", t->name
							, L"Путь ДС", tn->path()
							, L"Свойство", prop->name
							, L"Путь", spath);
						uv1 = EmptyUID;
					}
				}
				else uv1 = metauid;
				//nv = readValue1C(nt, tn, tr, v, uv1, _metats, clitem, path);
				nv = readValue1C(nt, tn, tr, v, uv1, metats, clitem, path);
				prop = tn->prop1;
				v->v_objprop[prop] = nv;

				//==> Обработка предопределенных значений
				if(prop->predefined)
				{
					if(nv->kind != kv_obj)
					{
						error(L"Ошибка формата потока 195. Ошибка получения значения предопределенного элемента. Значение свойства не является объектом"
							, L"Загружаемый тип", t->name
							, L"Путь ДС", tn->path()
							, L"Свойство", prop->name
							, L"Путь", spath);
					}
					else if(v->kind != kv_metaobj)
					{
						error(L"Ошибка формата потока 196. Ошибка получения значения предопределенного элемента. Значение, которому принадлежит свойство с признаком \"Предопределенные\" не является объектом метаданных"
							, L"Загружаемый тип", t->name
							, L"Путь ДС", tn->path()
							, L"Свойство", prop->name
							, L"Путь", spath
							, L"Тип владельца", KindOfValue1C_presantation(v->kind));
					}
					else
					{
						vm = (Value1C_metaobj*)v;
						vo = (Value1C_obj*)nv;
						for(ii = 0; ii < vo->v_objcol.size(); ++ii)
						{
							vvm = (Value1C_metaobj*)vo->v_objcol[ii];
							if(!vvm)
							{
								error(L"Ошибка формата потока 187. Ошибка получения значения предопределенного элемента"
									, L"Загружаемый тип", t->name
									, L"Путь ДС", tn->path()
									, L"Свойство", prop->name
									, L"Путь", spath);
								continue;
							}
							if(vvm->kind != kv_metaobj)
							{
								error(L"Ошибка формата потока 188. Ошибка получения значения предопределенного элемента. Значение не является объектом метаданных"
									, L"Загружаемый тип", t->name
									, L"Путь ДС", tn->path()
									, L"Свойство", prop->name
									, L"Путь", spath
									, L"Тип значения", KindOfValue1C_presantation(vvm->kind));
								continue;
							}
							u = vvm->v_metaobj->uid;
							if(u != EmptyUID)
							{
								nv = vvm->getproperty(L"Имя");
								if(!nv)
								{
									error(L"Ошибка формата потока 189. Ошибка получения имени предопределенного элемента"
										, L"Загружаемый тип", t->name
										, L"Путь ДС", tn->path()
										, L"Свойство", prop->name
										, L"Путь", spath);
									continue;
								}
								if(nv->kind != kv_string && nv->kind != kv_binary)
								{
									error(L"Ошибка формата потока 190. Ошибка получения имени предопределенного элемента. Тип значения не Строка"
										, L"Загружаемый тип", t->name
										, L"Путь ДС", tn->path()
										, L"Свойство", prop->name
										, L"Путь", spath
										, L"Тип значения", KindOfValue1C_presantation(nv->kind));
									continue;
								}
								n = nv->presentation();
								pre = new PredefinedValue(n, u, vm);
								vm->v_prevalues.push_back(pre);
								fpredefinedvalues[u] = pre;
							}
						}
					}
				}
				//<== Обработка предопределенных значений

				break;
			case stt_elcol:
				nt = tn->typeprop;
				if(!nt) if(t->collectiontypes.size() == 1) nt = t->collectiontypes[0];

				if(tn->typeval1 == stv_none)
				{
					for(tt = tr; tt;)
					{
						nv = readValue1C(nt, tn, tt, v, metauid, metats, clitem, path);
						v->v_objcol.push_back(nv);
					}
				}
				else
				{
					j = 0;
					switch(tn->typeval1)
					{
						case stv_number:
							j = tn->num1;
							break;
						case stv_prop:
							vn = (Value1C_number*)v->getproperty(tn->prop1);
							if(!vn)
							{
								error(L"Ошибка формата потока 40. Не установлено значение свойства, являющегося счетчиком коллекции."
									, L"Загружаемый тип", t->name
									, L"Путь ДС", tn->path()
									, L"Свойство", tn->prop1->name
									, L"Путь", spath + tr->path());
							}
							else if(vn->kind != kv_number)
							{
								error(L"Ошибка формата потока 41. Cвойство, являющееся счетчиком коллекции, имеет значение типа не Число."
									, L"Загружаемый тип", t->name
									, L"Путь ДС", tn->path()
									, L"Свойство", tn->prop1->name
									, L"Путь", spath + tr->path());
							}
							else j = vn->v_number;
							break;
						case stv_var:
							j = getVarValue(tn->str1, t, varvalues, clitem, spath);
							break;
						default:
							error(L"Ошибка формата потока 42. Недопусимый тип значения счетчика коллекции дерева сериализации."
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Тип значения", tn->typeval1presentation()
								, L"Путь", spath + tr->path());
					}
					for(tt = tr, i = 0; i < j; ++i)
					{
						nv = readValue1C(nt, tn, tt, v, metauid, metats, clitem, path);
						v->v_objcol.push_back(nv);
					}
				}
				if(!tn->nomove) tr = tt;
				break;
			case stt_gentype:
				if(!tr)
				{
					error(L"Ошибка формата потока 97. Отсутствует ожидаемое значение генерируемого типа."
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Путь", spath);
					break;
				}
				if(v->kind != kv_metaobj)
				{
					error(L"Ошибка формата потока 197. Попытка загрузки генерируемого типа для значения, не являющегося объектом метаданных."
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Путь", spath + tr->path());
					break;
				}
				gt = new GeneratedType(&tr, spath);
				((Value1C_metaobj*)v)->v_objgentypes[tn->gentype] = gt;
				break;
			case stt_cond:
				cv = false;

				//Значение 1
				vt1 = stv_none;
				switch(tn->typeval1)
				{
					case stv_string:
						vt1 = stv_string;
						sv1 = tn->str1;
						break;
					case stv_number:
						vt1 = stv_number;
						nv1 = tn->num1;
						break;
					case stv_uid:
						vt1 = stv_uid;
						uv1 = tn->uid1;
						break;
					case stv_value:
						mv = tn->val1;
						if(mv->valueUID == EmptyUID)
						{
							vt1 = stv_number;
							nv1 = mv->value;
						}
						else
						{
							vt1 = stv_uid;
							uv1 = mv->valueUID;
						}
						break;
					case stv_var:
						vt1 = stv_number;
						nv1 = getVarValue(tn->str1, t, varvalues, clitem, spath);
						break;
					case stv_prop:
						p = tn->prop1;
						ip = v->v_objprop.find(p);
						if(ip == v->v_objprop.end())
						{
							dvt = p->defaultvaluetype;
							switch(dvt)
							{
								case dvt_bool:
									vt1 = stv_number;
									nv1 = p->dv_bool ? 1 : 0;
									break;
								case dvt_number:
									vt1 = stv_number;
									nv1 = p->dv_number;
									break;
								case dvt_string:
									vt1 = stv_string;
									sv1 = *(p->dv_string);
									break;
								case dvt_type:
									vt1 = stv_uid;
									uv1 = p->dv_type->uid;
									break;
								case dvt_enum:
									mv = p->dv_enum;
									if(mv->valueUID == EmptyUID)
									{
										vt1 = stv_number;
										nv1 = mv->value;
									}
									else
									{
										vt1 = stv_uid;
										uv1 = mv->valueUID;
									}
									break;
								default:
									error(L"Ошибка формата потока 53. Ошибка вычисления условия. Не найдено значение свойства"
										, L"Загружаемый тип", t->name
										, L"Путь ДС", tn->path()
										, L"Свойство", p->name
										, L"Путь", spath + tr->path());
							}
						}
						else
						{
							vv = ip->second;
							if(!vv)
							{
								error(L"Ошибка формата потока 232. Ошибка вычисления условия. Значение свойства не определено"
									, L"Загружаемый тип", t->name
									, L"Путь ДС", tn->path()
									, L"Свойство", p->name
									, L"Путь", spath + tr->path());
							}
							else
							{
								if(vv->kind == kv_string)
								{
									vt1 = stv_string;
									sv1 = ((Value1C_string*)vv)->v_string;
								}
								else if(vv->kind == kv_number)
								{
									vt1 = stv_number;
									nv1 = ((Value1C_number*)vv)->v_number;
								}
								else if(vv->kind == kv_number_exp)
								{
									vt1 = stv_number;
									nv1 = (int)((Value1C_number_exp*)vv)->v_number;
								}
								else if(vv->kind == kv_uid)
								{
									vt1 = stv_uid;
									uv1 = ((Value1C_uid*)vv)->v_uid;
								}
								else if(vv->kind == kv_enum)
								{
									mv = ((Value1C_enum*)vv)->v_enum;
									if(mv->valueUID == EmptyUID)
									{
										vt1 = stv_number;
										nv1 = mv->value;
									}
									else
									{
										vt1 = stv_uid;
										uv1 = mv->valueUID;
									}
								}
								else if(vv->kind == kv_bool)
								{
									vt1 = stv_number;
									nv1 = ((Value1C_bool*)vv)->v_bool ? 1 : 0;
								}
								else if(vv->kind == kv_type)
								{
									vt1 = stv_uid;
									uv1 = ((Value1C_type*)vv)->v_uid;
								}
								else if(vv->kind == kv_binary)
								{
									vt1 = stv_string;
									sv1 = vv->presentation();
								}
								else
								{
									error(L"Ошибка формата потока 54. Ошибка вычисления условия. Недопустимый тип значения свойства"
										, L"Загружаемый тип", t->name
										, L"Путь ДС", tn->path()
										, L"Свойство", p->name
										, L"Тип значения", vv->kind
										, L"Путь", spath + tr->path());
								}
							}
						}
						break;
					case stv_vercon:
						vt1 = stv_number;
						nv1 = (int)tn->vercon1;
						break;
					case stv_ver1C:
						vt1 = stv_number;
						nv1 = (int)tn->ver1C1;
						break;
					case stv_classpar:
						vt1 = stv_number;
						if(clitem) nv1 = clitem->cl->getparamvalue(tn->classpar1);
						else
						{
							error(L"Ошибка формата потока 120. Ошибка вычисления условия. Класс не определён."
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Параметр класса", tn->classpar1->name
								, L"Тип значения", vv->kind
								, L"Путь", spath + tr->path());
							nv1 = -1;
						}
						break;
					case stv_globalvar:
						vt1 = stv_number;
						nv1 = 0;
						ok = false;
						for(vo = v->parent; vo; vo = vo->parent)
						{
							icv = vo->globalvars.find(tn->str1.UpperCase());
							if(icv != vo->globalvars.end())
							{
								nv1 = icv->second;
								ok = true;
								break;
							}
						}
						if(!ok)
						{
							error(L"Ошибка формата потока 123. Ошибка получения первого значения условия. Не удалось получить значение глобальной переменной"
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Глобальная переменная", tn->str1
								, L"Путь", spath + tr->path());
						}
						break;
					default:
						error(L"Ошибка формата потока 55. Ошибка вычисления условия. Недопустимый тип значения 1"
							, L"Загружаемый тип", t->name
							, L"Путь ДС", tn->path()
							, L"Тип значения", tn->typeval1presentation()
							, L"Путь", spath + tr->path());

				}

				//Значение 2
				vt2 = stv_none;
				switch(tn->typeval2)
				{
					case stv_string:
						vt2 = stv_string;
						sv2 = tn->str2;
						break;
					case stv_number:
						vt2 = stv_number;
						nv2 = tn->num2;
						break;
					case stv_uid:
						vt2 = stv_uid;
						uv2 = tn->uid2;
						break;
					case stv_value:
						mv = tn->val2;
						if(mv->valueUID == EmptyUID)
						{
							vt2 = stv_number;
							nv2 = mv->value;
						}
						else
						{
							vt2 = stv_uid;
							uv2 = mv->valueUID;
						}
						break;
					case stv_var:
						vt2 = stv_number;
						nv2 = getVarValue(tn->str2, t, varvalues, clitem, spath);
						break;
					case stv_prop:
						p = tn->prop2;
						ip = v->v_objprop.find(p);
						if(ip == v->v_objprop.end())
						{
							dvt = p->defaultvaluetype;
							switch(dvt)
							{
								case dvt_bool:
									vt2 = stv_number;
									nv2 = p->dv_bool ? 1 : 0;
									break;
								case dvt_number:
									vt2 = stv_number;
									nv2 = p->dv_number;
									break;
								case dvt_string:
									vt2 = stv_string;
									sv2 = *(p->dv_string);
									break;
								case dvt_type:
									vt2 = stv_uid;
									uv2 = p->dv_type->uid;
									break;
								case dvt_enum:
									mv = p->dv_enum;
									if(mv->valueUID == EmptyUID)
									{
										vt2 = stv_number;
										nv2 = mv->value;
									}
									else
									{
										vt2 = stv_uid;
										uv2 = mv->valueUID;
									}
									break;
								default:
									error(L"Ошибка формата потока 56. Ошибка вычисления условия. Не найдено значение свойства"
										, L"Загружаемый тип", t->name
										, L"Путь ДС", tn->path()
										, L"Свойство", p->name
										, L"Путь", spath + tr->path());
							}
						}
						else
						{
							vv = ip->second;
							if(!vv)
							{
								error(L"Ошибка формата потока 233. Ошибка вычисления условия. Значение свойства не определено"
									, L"Загружаемый тип", t->name
									, L"Путь ДС", tn->path()
									, L"Свойство", p->name
									, L"Путь", spath + tr->path());
							}
							else
							{
								if(vv->kind == kv_string)
								{
									vt2 = stv_string;
									sv2 = ((Value1C_string*)vv)->v_string;
								}
								else if(vv->kind == kv_number)
								{
									vt2 = stv_number;
									nv2 = ((Value1C_number*)vv)->v_number;
								}
								else if(vv->kind == kv_number_exp)
								{
									vt2 = stv_number;
									nv2 = (int)((Value1C_number_exp*)vv)->v_number;
								}
								else if(vv->kind == kv_uid)
								{
									vt2 = stv_uid;
									uv2 = ((Value1C_uid*)vv)->v_uid;
								}
								else if(vv->kind == kv_enum)
								{
									mv = ((Value1C_enum*)vv)->v_enum;
									if(mv->valueUID == EmptyUID)
									{
										vt2 = stv_number;
										nv2 = mv->value;
									}
									else
									{
										vt2 = stv_uid;
										uv2 = mv->valueUID;
									}
								}
								else if(vv->kind == kv_bool)
								{
									vt2 = stv_number;
									nv2 = ((Value1C_bool*)vv)->v_bool ? 1 : 0;
								}
								else if(vv->kind == kv_type)
								{
									vt2 = stv_uid;
									uv2 = ((Value1C_type*)vv)->v_uid;
								}
								else if(vv->kind == kv_binary)
								{
									vt2 = stv_string;
									sv2 = vv->presentation();
								}
								else
								{
									error(L"Ошибка формата потока 57. Ошибка вычисления условия. Недопустимый тип значения свойства"
										, L"Загружаемый тип", t->name
										, L"Путь ДС", tn->path()
										, L"Свойство", p->name
										, L"Тип значения", vv->kind
										, L"Путь", spath + tr->path());
								}
							}
						}
						break;
					case stv_vercon:
						vt2 = stv_number;
						nv2 = (int)tn->vercon2;
						break;
					case stv_ver1C:
						vt2 = stv_number;
						nv2 = (int)tn->ver1C2;
						break;
					case stv_classpar:
						vt2 = stv_number;
						if(clitem) nv2 = clitem->cl->getparamvalue(tn->classpar2);
						else
						{
							error(L"Ошибка формата потока 121. Ошибка вычисления условия. Класс не определён."
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Параметр класса", tn->classpar2->name
								, L"Тип значения", vv->kind
								, L"Путь", spath + tr->path());
							nv2 = -1;
						}
						break;
					case stv_globalvar:
						vt2 = stv_number;
						nv2 = 0;
						ok = false;
						for(vo = v->parent; vo; vo = vo->parent)
						{
							icv = vo->globalvars.find(tn->str2.UpperCase());
							if(icv != vo->globalvars.end())
							{
								nv2 = icv->second;
								ok = true;
								break;
							}
						}
						if(!ok)
						{
							error(L"Ошибка формата потока 124. Ошибка получения второго значения условия. Не удалось получить значение глобальной переменной"
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Глобальная переменная", tn->str2
								, L"Путь", spath + tr->path());
						}
						break;
					default:
						error(L"Ошибка формата потока 58. Ошибка вычисления условия. Недопустимый тип значения 2"
							, L"Загружаемый тип", t->name
							, L"Путь ДС", tn->path()
							, L"Тип значения", tn->typeval2presentation()
							, L"Путь", spath + tr->path());

				}
				if(vt1 != stv_none && vt2 != stv_none)
				{
					if(vt1 != vt2)
					{
						error(L"Ошибка формата потока 59. Ошибка вычисления условия. Несовпадающие типы сравниваемых значений"
							, L"Загружаемый тип", t->name
							, L"Путь ДС", tn->path()
							, L"Тип значения 1", vt1
							, L"Тип значения 2", vt2
							, L"Путь", spath + tr->path());
					}
					else
					{
						if(vt1 == stv_string)
						{
							switch(tn->condition)
							{
								case stc_e:
									cv = sv1.CompareIC(sv2) == 0;
									break;
								case stc_ne:
									cv = sv1.CompareIC(sv2) != 0;
									break;
								case stc_l:
									cv = sv1.CompareIC(sv2) < 0;
									break;
								case stc_g:
									cv = sv1.CompareIC(sv2) > 0;
									break;
								case stc_le:
									cv = sv1.CompareIC(sv2) <= 0;
									break;
								case stc_ge:
									cv = sv1.CompareIC(sv2) >= 0;
									break;
								default:
									error(L"Ошибка формата потока 60. Ошибка вычисления условия. Некорректное условие при сравнении строк"
										, L"Загружаемый тип", t->name
										, L"Путь ДС", tn->path()
										, L"Условие", tn->condition
										, L"Путь", spath + tr->path());
							}
						}
						else if(vt1 == stv_number)
						{
							switch(tn->condition)
							{
								case stc_e:
									cv = nv1 == nv2;
									break;
								case stc_ne:
									cv = nv1 != nv2;
									break;
								case stc_l:
									cv = nv1 < nv2;
									break;
								case stc_g:
									cv = nv1 > nv2;
									break;
								case stc_le:
									cv = nv1 <= nv2;
									break;
								case stc_ge:
									cv = nv1 >= nv2;
									break;
								case stc_bs:
									if(nv2 < 0 || nv2 > 31)
									{
										error(L"Ошибка формата потока 193. Ошибка вычисления условия. Номер бита за пределами 0-31"
											, L"Загружаемый тип", t->name
											, L"Номер бита", nv2);
										break;
									}
									cv = (nv1 & (1 << nv2)) != 0;
									break;
								case stc_bn:
									if(nv2 < 0 || nv2 > 31)
									{
										error(L"Ошибка формата потока 194. Ошибка вычисления условия. Номер бита за пределами 0-31"
											, L"Загружаемый тип", t->name
											, L"Номер бита", nv2);
										break;
									}
									cv = (nv1 & (1 << nv2)) == 0;
									break;
								default:
									error(L"Ошибка формата потока 61. Ошибка вычисления условия. Некорректное условие при сравнении чисел"
										, L"Загружаемый тип", t->name
										, L"Путь ДС", tn->path()
										, L"Условие", tn->condition
										, L"Путь", spath + tr->path());
							}
						}
						else if(vt1 == stv_uid)
						{
							switch(tn->condition)
							{
								case stc_e:
									cv = uv1 == uv2;
									break;
								case stc_ne:
									cv = uv1 != uv2;
									break;
								case stc_l:
									cv = uv1 < uv2;
									break;
								case stc_g:
									cv = uv2 < uv1;
									break;
								case stc_le:
									cv = uv1 < uv2 || uv1 == uv2;
									break;
								case stc_ge:
									cv = uv2 < uv1 || uv1 == uv2;
									break;
								default:
									error(L"Ошибка формата потока 62. Ошибка вычисления условия. Некорректное условие при сравнении UID"
										, L"Загружаемый тип", t->name
										, L"Путь ДС", tn->path()
										, L"Условие", tn->condition
										, L"Путь", spath + tr->path());
							}
						}
					}
				}
				if(cv) recursiveLoadValue1C(v, varvalues, &tr, tn->first, metauid, metats, clitem, path);

				break;
			case stt_metaid:
				if(!tr)
				{
					error(L"Ошибка формата потока 98. Отсутствует ожидаемое значение идентификатора объекта метаданных."
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Путь", spath);
					break;
				}
				if(tr->get_type() == nd_guid)
				{
					if(!string_to_GUID(tr->get_value(), &uid))
					{
						error(L"Ошибка формата потока 38. Ошибка преобразования UID при загрузке ИД метаданных."
							, L"Загружаемый тип", t->name
							, L"Путь ДС", tn->path()
							, L"Значение", tr->get_value()
							, L"Путь", spath + tr->path());
					}
					else
					{
						if(v->kind == kv_metaobj)
						{
							vm = (Value1C_metaobj*)v;
							if(vm->v_metaobj)
							{
								if(vm->v_metaobj->uid != uid)
								{
									error(L"Ошибка формата потока 125. Повторное определение ИД метаданных с отличающимся UID."
										, L"Загружаемый тип", t->name
										, L"Путь ДС", tn->path()
										, L"Существующий ИД", GUID_to_string(vm->v_metaobj->uid)
										, L"Загружаемый ИД", GUID_to_string(uid)
										, L"Путь", spath + tr->path());
								}
							}
							else
							{
								vm->v_metaobj = new MetaObject(uid, vm);
								fmetamap[uid] = vm->v_metaobj;
								metauid = uid;
							}
						}
						else
						{
							error(L"Ошибка формата потока 198. Попытка загрузки ИД метаданных для значения, не являющегося объектом метаданных."
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Загружаемый ИД", GUID_to_string(uid)
								, L"Путь", spath + tr->path());
						}
					}
				}
				else
				{
					error(L"Ошибка формата потока 39. Тип значения не UID при загрузке ИД метаданных."
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Тип значения", get_node_type_presentation(tr->get_type())
						, L"Путь", spath + tr->path());
				}
				if(!tn->nomove) tr = tr->get_next();
				break;
			case stt_classcol:
				if(!tr)
				{
					error(L"Ошибка формата потока 99. Отсутствует ожидаемое значение счетчика коллекции классов."
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Путь", spath);
					break;
				}
				if(tr->get_type() != nd_number)
				{
					error(L"Ошибка формата потока 88. Ожидается счетчик коллекции классов. Тип значения не число."
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Тип значения", get_node_type_presentation(tr->get_type())
						, L"Значение", tr->get_value()
						, L"Путь", spath + tr->path());
					if(!tn->nomove) tr = tr->get_next();
					break;
				}
				j = tr->get_value().ToIntDef(0);
				for(i = 0, tr = tr->get_next(); i < j; ++i)
				{
					if(tn->classtype == stct_inlist)
					{
						if(tr->get_type() != nd_list)
						{
							error(L"Ошибка формата потока 89. Ожидается список очередного класса коллекции классов. Тип значения не Список."
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Тип значения", get_node_type_presentation(tr->get_type())
								, L"Значение", tr->get_value()
								, L"Путь", spath + tr->path());
							continue;
						}
						tt = tr->get_first();
					}
					else tt = tr;
					if(!tt)
					{
						error(L"Ошибка формата потока 90. Ожидается идентификатор очередного класса коллекции классов. Значение отсутствует."
							, L"Загружаемый тип", t->name
							, L"Путь ДС", tn->path()
							, L"Путь", spath + tr->path());
						continue;
					}
					if(tt->get_type() != nd_guid)
					{
						error(L"Ошибка формата потока 91. Ожидается идентификатор очередного класса коллекции классов. Тип значения не UID."
							, L"Загружаемый тип", t->name
							, L"Путь ДС", tn->path()
							, L"Тип значения", get_node_type_presentation(tt->get_type())
							, L"Значение", tt->get_value()
							, L"Путь", spath + tt->path());
						continue;
					}
					if(!string_to_GUID(tt->get_value(), &uid))
					{
						error(L"Ошибка формата потока 92. Ожидается идентификатор очередного класса коллекции классов. Ошибка преобразования UID."
							, L"Загружаемый тип", t->name
							, L"Путь ДС", tn->path()
							, L"Значение", tt->get_value()
							, L"Путь", spath + tt->path());
						continue;
					}
					for(tnn = tn->first; tnn; tnn = tnn->next) if(tnn->uid1 == uid) break;
					if(!tnn)
					{
						error(L"Ошибка формата потока 93. Неизвестный идентификатор класса коллекции классов."
							, L"Загружаемый тип", t->name
							, L"Путь ДС", tn->path()
							, L"Значение", tt->get_value()
							, L"Путь", spath + tt->path());
						continue;
					}
					cl = Class::getclass(uid);
					if(cl) cli = new ClassItem(cl);
					else cli = NULL;
					tt = tt->get_next();
					recursiveLoadValue1C(v, varvalues, &tt, tnn->first, metauid, metats, cli, path, tn->classtype == stct_inlist);
					delete cli;
					if(tn->classtype == stct_inlist) tr = tr->get_next();
					else tr = tt;
				}
				break;
			case stt_class:
				error(L"Ошибка формата потока 87. Неожидаемый тип \"Класс\" узла дерева сериализации."
					, L"Загружаемый тип", t->name
					, L"Путь ДС", tn->path()
					, L"Путь", spath + tr->path());
				break;
			case stt_idcol:
				if(!tr)
				{
					error(L"Ошибка формата потока 155. Отсутствует ожидаемое значение счетчика коллекции ИД-элементов."
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Путь", spath);
					break;
				}
				if(tr->get_type() != nd_number)
				{
					error(L"Ошибка формата потока 156. Ожидается счетчик коллекции ИД-элементов. Тип значения не число."
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Тип значения", get_node_type_presentation(tr->get_type())
						, L"Значение", tr->get_value()
						, L"Путь", spath + tr->path());
					if(!tn->nomove) tr = tr->get_next();
					break;
				}
				j = tr->get_value().ToIntDef(0);
				for(i = 0, tr = tr->get_next(); i < j; ++i)
				{
					if(tn->classtype == stct_inlist)
					{
						if(tr->get_type() != nd_list)
						{
							error(L"Ошибка формата потока 157. Ожидается список очередного ИД-элемента. Тип значения не Список."
								, L"Загружаемый тип", t->name
								, L"Путь ДС", tn->path()
								, L"Тип значения", get_node_type_presentation(tr->get_type())
								, L"Значение", tr->get_value()
								, L"Путь", spath + tr->path());
							continue;
						}
						tt = tr->get_first();
					}
					else tt = tr;
					if(!tt)
					{
						error(L"Ошибка формата потока 158. Ожидается идентификатор очередного ИД-элемента. Значение отсутствует."
							, L"Загружаемый тип", t->name
							, L"Путь ДС", tn->path()
							, L"Путь", spath + tr->path());
						continue;
					}
					if(tt->get_type() != nd_guid)
					{
						error(L"Ошибка формата потока 159. Ожидается идентификатор очередного ИД-элемента. Тип значения не UID."
							, L"Загружаемый тип", t->name
							, L"Путь ДС", tn->path()
							, L"Тип значения", get_node_type_presentation(tt->get_type())
							, L"Значение", tt->get_value()
							, L"Путь", spath + tt->path());
						continue;
					}
					if(!string_to_GUID(tt->get_value(), &uid))
					{
						error(L"Ошибка формата потока 160. Ожидается идентификатор очередного ИД-элемента. Ошибка преобразования UID."
							, L"Загружаемый тип", t->name
							, L"Путь ДС", tn->path()
							, L"Значение", tt->get_value()
							, L"Путь", spath + tt->path());
						continue;
					}
					for(tnn = tn->first; tnn; tnn = tnn->next) if(tnn->uid1 == uid) break;
					if(!tnn)
					{
						error(L"Ошибка формата потока 161. Неизвестный идентификатор ИД-элемента."
							, L"Загружаемый тип", t->name
							, L"Путь ДС", tn->path()
							, L"Значение", tt->get_value()
							, L"Путь", spath + tt->path());
						continue;
					}

					nt = tnn->typeprop;
					if(!nt) if(t->collectiontypes.size() == 1) nt = t->collectiontypes[0];

					tt = tt->get_next();
					nv = readValue1C(&tt, nt, v, metauid, metats, clitem, path);
					v->v_objcol.push_back(nv);

					if(tn->classtype == stct_inlist) tr = tr->get_next();
					else tr = tt;
				}
				break;
			case stt_idel:
				error(L"Ошибка формата потока 154. Неожидаемый тип \"ИД-элемент\" узла дерева сериализации."
					, L"Загружаемый тип", t->name
					, L"Путь ДС", tn->path()
					, L"Путь", spath + tr->path());
				break;
			default:
				error(L"Ошибка формата потока 23. Неизвестный тип узла дерева сериализации."
					, L"Загружаемый тип", t->name
					, L"Путь ДС", tn->path()
					, L"Тип узла", tn->type
					, L"Путь", spath + tr->path());
		}
	}

	if(checkend) if(tr)
	{
		error(L"Ошибка формата потока 36. Остались необработанные значения."
			, L"Загружаемый тип", t->name
			, L"Путь", spath + tr->path());
	}


	*ptr = tr;
}

//---------------------------------------------------------------------------
Value1C* __fastcall MetaContainer::readValue1C(MetaType* nt, const SerializationTreeNode* tn, tree*& tr, Value1C_obj* valparent, const TGUID& metauid, Value1C_stdtabsec*& metats, ClassItem* clitem, String& path)
{
	Value1C* nv;
	Value1C_refobj* vro;
	Value1C_refpre* vrp;
	Value1C_binary* vb;
	Value1C_right* vr;
	TGUID uid;
	MetaType* t;
	tree* tt;
	tree* tte;
	String npath;
	String spath;
	String sn;
	THandle handle;

	spath = storpresent + path;
	nv = NULL;
	t = tn->owner;

	if(tn->isref)
	{
		vro = new Value1C_refobj(valparent);
		nv = vro;
		vro->type = nt;
		if(tr->get_type() == nd_guid)
		{
			if(!string_to_GUID(tr->get_value(), &uid))
			{
				error(L"Ошибка формата потока 83. Ошибка преобразования UID при загрузке ссылки на объект метаданных."
					, L"Загружаемый тип", t->name
					, L"Путь ДС", tn->path()
					, L"Значение", tr->get_value()
					, L"Путь", spath + tr->path());
			}
			else if(uid != EmptyUID)
			{
				puninitvalues->push_back(UninitValue1C(nv, spath + tr->path(), uid));
				vro->v_uid = uid;
			}
		}
		else
		{
			error(L"Ошибка формата потока 84. Тип значения не UID при загрузке ссылки на объект метаданных."
				, L"Загружаемый тип", t->name
				, L"Путь ДС", tn->path()
				, L"Значение", tr->get_value()
				, L"Тип значения", get_node_type_presentation(tr->get_type())
				, L"Путь", spath + tr->path());
		}
		//if(!tn->nomove) tr = tr->get_next();
		tr = tr->get_next();
	}
	else if(tn->isrefpre)
	{
		vrp = new Value1C_refpre(valparent);
		nv = vrp;
		vrp->type = nt;
		if(tr->get_type() == nd_guid)
		{
			if(!string_to_GUID(tr->get_value(), &uid))
			{
				error(L"Ошибка формата потока 163. Ошибка преобразования UID при загрузке ссылки на предопределенный элемент."
					, L"Загружаемый тип", t->name
					, L"Путь ДС", tn->path()
					, L"Значение", tr->get_value()
					, L"Путь", spath + tr->path());
			}
			else if(uid != EmptyUID) puninitvalues->push_back(UninitValue1C(nv, spath + tr->path(), uid));
		}
		else
		{
			error(L"Ошибка формата потока 164. Тип значения не UID при загрузке ссылки на предопределенный элемент."
				, L"Загружаемый тип", t->name
				, L"Путь ДС", tn->path()
				, L"Значение", tr->get_value()
				, L"Тип значения", get_node_type_presentation(tr->get_type())
				, L"Путь", spath + tr->path());
		}
		//if(!tn->nomove) tr = tr->get_next();
		tr = tr->get_next();
	}
	else if(tn->isright)
	{
		vr = new Value1C_right(valparent);
		nv = vr;
		vr->type = nt;
		if(tr->get_type() == nd_guid)
		{
			if(!string_to_GUID(tr->get_value(), &uid))
			{
				error(L"Ошибка формата потока 213. Ошибка преобразования UID при загрузке права."
					, L"Загружаемый тип", t->name
					, L"Путь ДС", tn->path()
					, L"Значение", tr->get_value()
					, L"Путь", spath + tr->path());
			}
			else
			{
				vr->v_right = MetaRight::getright(uid);
				if(!vr->v_right)
				{
					error(L"Ошибка формата потока 215. Неизвестное право."
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"UID права", tr->get_value()
						, L"Путь", spath + tr->path());
				}
			}
		}
		else
		{
			error(L"Ошибка формата потока 214. Тип значения не UID при загрузке права."
				, L"Загружаемый тип", t->name
				, L"Путь ДС", tn->path()
				, L"Значение", tr->get_value()
				, L"Тип значения", get_node_type_presentation(tr->get_type())
				, L"Путь", spath + tr->path());
		}
		//if(!tn->nomove) tr = tr->get_next();
		tr = tr->get_next();
	}
	else if(tn->exnernal)
	{
		if(tr->get_type() == nd_guid)
		{
			if(!string_to_GUID(tr->get_value(), &uid))
			{
				error(L"Ошибка формата потока 85. Ошибка преобразования UID при загрузке имени файла."
					, L"Загружаемый тип", t->name
					, L"Путь ДС", tn->path()
					, L"Значение", tr->get_value()
					, L"Путь", spath + tr->path());
				//nv = new Value1C(valparent);
				//nv->type = nt;
			}
			else
			{
				npath = metaprefix + tr->get_value();
				tt = gettree(stor, npath);
				if(!tt)
				{
					error(L"Ошибка формата потока 76. Пустой файл"
						, L"Путь", storpresent + npath);
					//nv = new Value1C(valparent);
					//nv->type = nt;
				}
				else
				{
					tte = tt->get_first();
					nv = readValue1C(&tte, nt, valparent, metauid, metats, clitem, npath);
					delete tt;
				}
			}
		}
		else
		{
			error(L"Ошибка формата потока 86. Тип значения не UID при загрузке имени файла."
				, L"Загружаемый тип", t->name
				, L"Путь ДС", tn->path()
				, L"Значение", tr->get_value()
				, L"Тип значения", get_node_type_presentation(tr->get_type())
				, L"Путь", spath + tr->path());
			//nv = new Value1C(valparent);
			//nv->type = nt;
		}
		//if(!tn->nomove) tr = tr->get_next();
		tr = tr->get_next();
	}
	else if(nt == MetaTypeSet::mt_binarydata)
	{
		vb = new Value1C_binary(valparent);
		vb->type = nt;
		nv = vb;
		switch(tn->binsertype)
		{
			case bst_empty:
				if(tr->get_type() == nd_binary2)
				{
					vb->v_binary = new TTempStream();
					base64_decode(tr->get_value(), vb->v_binary, 0);
				}
				else
				{
					error(L"Ошибка формата потока 146. Несовпадение вида сериализации двоичных данных. Ожидается строка base64 без префикса"
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Значение", tr->get_value()
						, L"Тип значения", get_node_type_presentation(tr->get_type())
						, L"Путь", spath + tr->path());
				}
				break;
			case bst_base64:
				if(tr->get_type() == nd_binary)
				{
					vb->v_binary = new TTempStream();
					base64_decode(tr->get_value(), vb->v_binary, 8);
				}
				else
				{
					error(L"Ошибка формата потока 147. Несовпадение вида сериализации двоичных данных. Ожидается строка base64 c префиксом #base64:"
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Значение", tr->get_value()
						, L"Тип значения", get_node_type_presentation(tr->get_type())
						, L"Путь", spath + tr->path());
				}
				break;
			case bst_data:
				if(tr->get_type() == nd_binary_d)
				{
					vb->v_binary = new TTempStream();
					base64_decode(tr->get_value(), vb->v_binary, 6);
				}
				else
				{
					error(L"Ошибка формата потока 148. Несовпадение вида сериализации двоичных данных. Ожидается строка base64 c префиксом #data:"
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Значение", tr->get_value()
						, L"Тип значения", get_node_type_presentation(tr->get_type())
						, L"Путь", spath + tr->path());
				}
				break;
			case bst_base64_or_data:
				if(tr->get_type() == nd_binary || tr->get_type() == nd_binary_d)
				{
					vb->v_binary = new TTempStream();
					base64_decode(tr->get_value(), vb->v_binary, tr->get_type() == nd_binary ? 8 : 6);
				}
				else
				{
					error(L"Ошибка формата потока 153. Несовпадение вида сериализации двоичных данных. Ожидается строка base64 c префиксом #base64: или #data:"
						, L"Загружаемый тип", t->name
						, L"Путь ДС", tn->path()
						, L"Значение", tr->get_value()
						, L"Тип значения", get_node_type_presentation(tr->get_type())
						, L"Путь", spath + tr->path());
				}
				break;
			case bst_min:
				error(L"Ошибка формата потока 149. Не указан вид сериализации двоичных данных."
					, L"Загружаемый тип", t->name
					, L"Путь ДС", tn->path()
					, L"Значение", tr->get_value()
					, L"Тип значения", get_node_type_presentation(tr->get_type())
					, L"Путь", spath + tr->path());
			default:
				error(L"Ошибка формата потока 150. Неизвестный вид сериализации двоичных данных."
					, L"Загружаемый тип", t->name
					, L"Путь ДС", tn->path()
					, L"Вид сериализации", tn->binsertype
					, L"Тип значения", get_node_type_presentation(tr->get_type())
					, L"Путь", spath + tr->path());
		}
		vb->v_binformat = tn->binformat;
		if(tn->binformat == eff_min)
		{
			error(L"Ошибка формата потока 151. Не указан формат двоичных данных."
				, L"Загружаемый тип", t->name
				, L"Путь ДС", tn->path()
				, L"Путь", spath + tr->path());
		}
		else if(tn->binformat >= eff_max)
		{
			error(L"Ошибка формата потока 152. Неизвестный формат двоичных данных."
				, L"Загружаемый тип", t->name
				, L"Путь ДС", tn->path()
				, L"Формат", tn->binformat
				, L"Путь", spath + tr->path());
			vb->v_binformat = eff_min;
		}

		tr = tr->get_next();
	}
	else nv = readValue1C(&tr, nt, valparent, metauid, metats, clitem, path);

	return nv;
}

//---------------------------------------------------------------------------
void __fastcall MetaContainer::readPredefinedValues(Value1C_metaobj* v, const int ni, const int ui, const Value1C_obj* vStrings, const String& spath)
{
	Value1C_obj* vo;
	Value1C_obj* vvo;
	Value1C_uid* vu;
	Value1C_string* vs;
	Value1C* vv;
	unsigned int i;
	TGUID u;
	String n;
	PredefinedValue* pre;

	for(i = 0; i < vStrings->v_objcol.size(); ++i)
	{
		vo = (Value1C_obj*)vStrings->v_objcol[i];
		if(!vo)
		{
			error(L"Ошибка формата потока 179. Ошибка получения значения предопределенного элемента"
				, L"Путь", spath);
			continue;
		}
		vvo = (Value1C_obj*)vo->v_objcol[ui];
		if(!vvo)
		{
			error(L"Ошибка формата потока 180. Ошибка получения идентификатора предопределенного элемента"
				, L"Путь", spath);
			continue;
		}
		vu = (Value1C_uid*)vvo->getproperty(L"Ссылка");
		if(!vu)
		{
			error(L"Ошибка формата потока 181. Ошибка получения значения идентификатора предопределенного элемента"
				, L"Путь", spath);
			continue;
		}
		if(vu->kind != kv_uid)
		{
			error(L"Ошибка формата потока 182. Ошибка получения значения идентификатора предопределенного элемента. Тип значения не UID"
				, L"Путь", spath
				, L"Тип значения", KindOfValue1C_presantation(vu->kind));
			continue;
		}
		u = vu->v_uid;
		if(u != EmptyUID)
		{
			vv = vo->v_objcol[ni];
			if(!vv)
			{
				error(L"Ошибка формата потока 183. Ошибка получения имени предопределенного элемента"
					, L"Путь", spath);
				continue;
			}
			if(vv->kind != kv_string && vv->kind != kv_binary)
			{
				error(L"Ошибка формата потока 184. Ошибка получения имени предопределенного элемента. Тип значения не Строка"
					, L"Путь", spath
					, L"Тип значения", KindOfValue1C_presantation(vs->kind));
				continue;
			}
			n = vv->presentation();
			pre = new PredefinedValue(n, u, v);
			v->v_prevalues.push_back(pre);
			fpredefinedvalues[u] = pre;
		}

		vvo = (Value1C_obj*)vo->getproperty(L"Строки");
		if(vvo) readPredefinedValues(v, ni, ui, vvo, spath);
	}
}

//---------------------------------------------------------------------------
int __fastcall MetaContainer::getVarValue(const String& vname, MetaType* t, VarValue* varvalues, ClassItem* clitem, String& path)
{
	if(vname.CompareIC(L"ВерсияКонтейнера") == 0) return (int)containerver;
	if(vname.CompareIC(L"Версия1С") == 0) return (int)ver1C;
	if(vname.CompareIC(L"ВерсияКласса") == 0)
	{
		if(clitem) return clitem->version;
		error(L"Ошибка формата потока 119. Ошибка получения значения переменной. Класс не определён."
			, L"Загружаемый тип", t->name
			, L"Имя переменной", vname
			, L"Путь", path);
		return -1;
	}
	if(varvalues)
	{
		for(unsigned int i = 0; i < t->serializationvars.size(); ++i) if(t->serializationvars[i]->name.CompareIC(vname) == 0)
		{
			if(varvalues[i].isset) return varvalues[i].value;
			else
			{
				error(L"Ошибка формата потока 43. Ошибка получения значения переменной. Значение переменной не установлено."
					, L"Загружаемый тип", t->name
					, L"Имя переменной", vname
					, L"Путь", path);
				return 0;
			}
		}
	}
	error(L"Ошибка формата потока 34. Ошибка получения значения переменной. Недопустимое имя переменной."
		, L"Загружаемый тип", t->name
		, L"Имя переменной", vname
		, L"Путь", path);
	return 0;
}

//---------------------------------------------------------------------------
void __fastcall MetaContainer::setVarValue(const String& vname, MetaType* t, Value1C_obj* v, VarValue* varvalues, ClassItem* clitem, int value, String& path)
{
	SerializationTreeVar* var;
	unsigned int i, j;
	VarValidValue* vvv;

	if(vname.CompareIC(L"ВерсияКласса") == 0)
	{
		if(clitem)
		{
			clitem->version = value;
			for(j = 0; j < clitem->cl->vervalidvalues.size(); ++j)
			{
				if(clitem->cl->vervalidvalues[j].value == value) return;
			}
			error(L"Ошибка формата потока 122. Недопустимое значение переменной."
				, L"Загружаемый тип", t->name
				, L"Имя переменной", vname
				, L"Значение", value
				, L"Класс", GUID_to_string(clitem->cl->uid)
				, L"Путь", path);

		}
		else
		{
			error(L"Ошибка формата потока 118. Ошибка установки переменной. Класс не определён."
				, L"Загружаемый тип", t->name
				, L"Имя переменной", vname
				, L"Значение", value
				, L"Путь", path);
		}
		return;
	}

	if(varvalues)
	{
		for(i = 0; i < t->serializationvars.size(); ++i)
		{
			var = t->serializationvars[i];
			if(var->name.CompareIC(vname) == 0)
			{
				varvalues[i].value = value;
				varvalues[i].isset = true;
				if(var->validvalues.size() == 0) return;
				for(j = 0; j < var->validvalues.size(); ++j)
				{
					vvv = &(var->validvalues[j]);
					if(vvv->value == value)
					{
						if(var->isglobal) v->globalvars[vname.UpperCase()] = vvv->globalvalue;
						return;
					}
				}
				error(L"Ошибка формата потока 64. Недопустимое значение переменной."
					, L"Загружаемый тип", t->name
					, L"Имя переменной", vname
					, L"Значение", value
					, L"Путь", path);
				return;
			}
		}
	}
	error(L"Ошибка формата потока 37. Ошибка установки значения переменной. Недопустимое имя переменной."
		, L"Загружаемый тип", t->name
		, L"Имя переменной", vname
		, L"Путь", path);

}

//---------------------------------------------------------------------------
MetaObject* __fastcall MetaContainer::getMetaObject(const String& n)
{
	std::map<String, MetaObject*>::iterator i;
	i = fsmetamap.find(n.UpperCase());
	if(i == fsmetamap.end())
	{
		i = MetaObject::smap.find(n.UpperCase());
		if(i == MetaObject::smap.end()) return NULL;
		return i->second;
	}
	return i->second;
}

//---------------------------------------------------------------------------
MetaObject* __fastcall MetaContainer::getMetaObject(const TGUID& u)
{
	std::map<TGUID, MetaObject*>::iterator i;
	i = fmetamap.find(u);
	if(i == fmetamap.end())
	{
		i = MetaObject::map.find(u);
		if(i == MetaObject::map.end()) return NULL;
		return i->second;
	}
	return i->second;
}

//---------------------------------------------------------------------------
PredefinedValue* __fastcall MetaContainer::getPreValue(const TGUID& u)
{
	std::map<TGUID, PredefinedValue*>::iterator i;
	i = fpredefinedvalues.find(u);
	if(i == fpredefinedvalues.end()) return NULL;
	return i->second;
}

//---------------------------------------------------------------------------
bool __fastcall MetaContainer::Export(const String& path, bool english, unsigned int thread_count)
{
	String npath;
	Value1C_obj_ExportThread* thr;
	unsigned int i;

	npath = String(L"\\\\?\\") + path;
	if(!DirectoryExists(npath)) if(!CreateDir(npath)) return false;
	if(!froot) return false;

	export_thread_count = thread_count;
	export_work_count = 0;
	if(export_thread_count)
	{
		export_threads = new Value1C_obj_ExportThread*[export_thread_count];
		for(i = 0; i < export_thread_count; ++i)
		{
			thr = new Value1C_obj_ExportThread(this);
			export_threads[i] = thr;
		}
	}

	ExportThread(froot, npath, english);

	if(export_thread_count)
	{
		while(export_work_count)
		{
			CheckSynchronize(10);
			//Sleep(0);
		}
		for(i = 0; i < export_thread_count; ++i)
		{
			thr = export_threads[i];
			thr->finish = true;
			thr->work->SetEvent();
		}
		for(i = 0; i < export_thread_count; ++i)
		{
			thr = export_threads[i];
			thr->WaitFor();
			delete thr;
		}
		delete[] export_threads;
	}

	return true;
}

//---------------------------------------------------------------------------

bool __fastcall MetaContainer::ExportThread(Value1C_obj* v, const String& path, bool english)
//bool __fastcall MetaContainer::ExportThread(Value1C_obj* v, String path, bool english)
{
	unsigned int i;
	Value1C_obj_ExportThread* thr;
	bool multithread;

	if(export_thread_count)
	{
		if(cur_export_thread) multithread = !cur_export_thread->singlethread;
		else multithread = true;
	}
	else multithread = false;


	if(multithread)
	{
		for(i = 0; i < export_thread_count; ++i)
		{
			thr = export_threads[i];
			if(!InterlockedExchange(&thr->busy, 1))
			{
				thr->v = v;
				thr->path = path;
				thr->english = english;
				InterlockedIncrement(&export_work_count);
				thr->work->SetEvent();
				return true;
			}
		}
	}
	return v->Export(path, NULL, 0, english);
}


//********************************************************
// Класс Value1C_obj_ExportThread

//---------------------------------------------------------------------------
__fastcall Value1C_obj_ExportThread::Value1C_obj_ExportThread(MetaContainer* _owner) : owner(_owner), TThread(false)
{
	FreeOnTerminate = false;
	busy = 0;
	finish = false;
	singlethread = 0;
	work = new TEvent(NULL, true, false, L"", false);
}

//---------------------------------------------------------------------------
void __fastcall Value1C_obj_ExportThread::Execute()
{
	cur_thread = this;
	cur_export_thread = this;
	puninitvalues = &uninitvalues;

	while(true)
	{
		work->WaitFor(INFINITE);
		work->ResetEvent();
		if(finish) return;
		v->Export(path, NULL, 0, english);
		InterlockedExchange(&busy, 0);
		InterlockedDecrement(&owner->export_work_count);
		//singlethread = 0;
	}
}
//---------------------------------------------------------------------------
__fastcall Value1C_obj_ExportThread::~Value1C_obj_ExportThread()
{
	delete work;
}

//---------------------------------------------------------------------------


