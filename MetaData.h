//---------------------------------------------------------------------------

#ifndef MetaDataH
#define MetaDataH

#include "vector"
#include "map"

#include "NodeTypes.h"
#include "Parse_tree.h"
#include "Common.h"
#include "ConfigStorage.h"
#include "TempStream.h"

//---------------------------------------------------------------------------
extern TGUID EmptyUID;

//---------------------------------------------------------------------------
class MetaBase;
class MetaValue;
class MetaProperty;
struct MetaPropertyLess;
struct GeneratedType;
class Class;
class MetaType;
class MetaTypeSet;
class Value1C;
class Value1C_obj;
class Value1C_metaobj;
class MetaContainer;
class ConfigStorage;
struct VarValidValue;
class Value1C_obj_ExportThread;

//---------------------------------------------------------------------------
// Версии контейнера 1С
// значения версий должны обязательно располагаться по возрастанию, чтобы можно было сравнивать версии на >, < и =
enum ContainerVer
{
	cv_2_0 = 1,
	cv_5_0 = 2,
	cv_6_0 = 3,
	cv_106_0 = 4,
	cv_200_0 = 5,
	cv_202_2 = 6,
	cv_216_0 = 7
};

//---------------------------------------------------------------------------
// Версии 1С
// значения версий должны обязательно располагаться по возрастанию, чтобы можно было сравнивать версии на >, < и =
enum Version1C
{
	v1C_min = 0,

	v1C_8_0 = 1,
	v1C_8_1 = 2,
	v1C_8_2 = 3,
	v1C_8_2_14 = 4,
	v1C_8_3_1 = 5,
	v1C_8_3_2 = 6,
	v1C_8_3_3 = 7,
	v1C_8_3_4 = 8,
	v1C_8_3_5 = 9,
	v1C_8_3_6 = 10
};

//---------------------------------------------------------------------------
// Способы выгрузки
enum ExportType
{
	et_default = 0,
	et_catalog = 1,
	et_file = 2
};

//---------------------------------------------------------------------------
// Базовый класс метаданных 1С
class MetaBase
{
protected:
	String fname;
	String fename;
public:
	__fastcall MetaBase(){};
	__fastcall MetaBase(
		const String& _name
		,const String& _ename
	) : fname(_name), fename(_ename){};
	~MetaBase(){};
	void setname(const String& _name){fname = _name;};
	void setename(const String& _ename){fename = _ename;};
	__property String name = {read = fname};
	__property String ename = {read = fename};
	String getname(bool english = false)
	{
		if(english) return fename;
		else return fname;
	};
};

//---------------------------------------------------------------------------
// Предопределенное значение метаданных
class MetaValue : public MetaBase
{
protected:
	MetaType* owner;
	int fvalue;
	TGUID fvalueUID;
public:
	__fastcall MetaValue(
		MetaType* _owner,
		const String& _name,
		const String& _ename,
		const int _value
	) : MetaBase(_name, _ename), fvalue(_value), owner(_owner){};
	__fastcall MetaValue(MetaType* _owner, tree* tr);
	__fastcall ~MetaValue(){};
	__property int value = {read = fvalue};
	__property TGUID valueUID = {read = fvalueUID};
	MetaType* __fastcall getowner(){return owner;};
};

//---------------------------------------------------------------------------
// Виды значений по умолчанию
enum DefaultValueType
{
	dvt_novalue = 0, // Нет значения по умолчанию
	dvt_bool = 1, // Булево
	dvt_number = 2, // Число
	dvt_string = 3, // Строка
	dvt_date = 4, // Дата
	dvt_undef = 5, // Неопределено
	dvt_null = 6, // Null
	dvt_type = 7, // Тип
	dvt_enum = 8 // Значение системного перечисления
};

//---------------------------------------------------------------------------
// Свойство метаданных
class MetaProperty : public MetaBase
{
protected:
	std::vector<MetaType*> ftypes;
	std::vector<String> fstypes;
	MetaType* owner;
	bool fpredefined;
	ExportType fexporttype;
	Class* f_class;
public:
	DefaultValueType defaultvaluetype;
	union
	{
		bool dv_bool;
		int dv_number;
		String* dv_string;
		char dv_date[7];
		MetaType* dv_type;
		MetaValue* dv_enum;
	};

	__fastcall MetaProperty(
		MetaType* _owner,
		const String& _name,
		const String& _ename
	) : MetaBase(_name, _ename), owner(_owner){};
	__fastcall MetaProperty(MetaType* _owner, tree* tr);
	__fastcall ~MetaProperty();
	void __fastcall filltypes();
	__property std::vector<MetaType*> types = {read = ftypes};
	MetaType* __fastcall getowner(){return owner;};
	__property bool predefined = {read = fpredefined};
	__property ExportType exporttype = {read = fexporttype};
	__property Class* _class = {read = f_class};

};

//---------------------------------------------------------------------------
// Сравнение свойств метаданных
struct MetaPropertyLess
{
	bool operator()(MetaProperty* const l, MetaProperty* const r) const
	{
		return l->name < r->name;
	}
};

//---------------------------------------------------------------------------
// Объект метаданных
class MetaObject : public MetaBase
{
protected:
	String ffullname;
	String fefullname;
	TGUID fuid;
	Value1C_metaobj* fvalue;
public:
	static std::map<TGUID, MetaObject*> map;
	static std::map<String, MetaObject*> smap;

	__fastcall MetaObject(const TGUID& _uid, Value1C_metaobj* _value) : fuid(_uid), fvalue(_value){};
	__fastcall MetaObject(const TGUID& _uid, Value1C_metaobj* _value, const String _name, const String _ename) : MetaBase(_name, _ename), fuid(_uid), fvalue(_value){};
	void setfullname(const String& _fullname){ffullname = _fullname;};
	void setefullname(const String& _efullname){fefullname = _efullname;};
	__property String fullname = {read = ffullname};
	__property String efullname = {read = fefullname};
	__property TGUID uid = {read = fuid};
	__property Value1C_metaobj* value = {read = fvalue};
	String getfullname(bool english = false)
	{
		if(english) return fefullname;
		else return ffullname;
	};
};

//---------------------------------------------------------------------------
// Генерируемый тип
class MetaGeneratedType : public MetaBase
{
protected:
	bool fwoprefix; // Признак "Без префикса"
public:
	__property bool woprefix = {read = fwoprefix};

	__fastcall MetaGeneratedType(
		const String& _name,
		const String& _ename
	) : MetaBase(_name, _ename), fwoprefix(false)
	{};
	__fastcall MetaGeneratedType(tree* tr);

};

//---------------------------------------------------------------------------
// Право
class MetaRight : public MetaBase
{
protected:
	TGUID fuid;
	Version1C fver1C;
public:
	static std::map<TGUID, MetaRight*> map;
	static std::map<String, MetaRight*> smap;

	//__fastcall MetaRight(const TGUID& _uid) : fuid(_uid){map[fuid] = this;};
	//__fastcall MetaRight(const TGUID& _uid, const String _name, const String _ename) : MetaBase(_name, _ename), fuid(_uid){
	//	map[fuid] = this;
	//	smap[fname] = this;
	//	smap[fename] = this;
	//};
	__fastcall MetaRight(tree* tr);
	static MetaRight* __fastcall getright(const TGUID& _uid);
	static MetaRight* __fastcall getright(const String& _name);
	__property TGUID uid = {read = fuid};
	__property Version1C ver1C = {read = fver1C};
};



//---------------------------------------------------------------------------
// Стандартный реквизит
class MetaStandartAttribute : public MetaBase
{
protected:
	int fvalue;
	bool fcount;
	int fvaluemax;
	TGUID fuid;
public:
	__fastcall MetaStandartAttribute(
		const String& _name,
		const String& _ename
	) : MetaBase(_name, _ename)
	{fcount = false;};
	__fastcall MetaStandartAttribute(tree* tr);
	__property int value = {read = fvalue};
	__property bool count = {read = fcount};
	__property int valuemax = {read = fvaluemax};
	__property TGUID uid = {read = fuid};
};

//---------------------------------------------------------------------------
// Стандартная табличная часть
class MetaStandartTabularSection : public MetaBase
{
protected:
	int fvalue;
	Class* f_class;
public:
	TGUID class_uid;
	static std::vector<MetaStandartTabularSection*> list;

	__fastcall MetaStandartTabularSection(
		const String& _name,
		const String& _ename
	) : MetaBase(_name, _ename)
	{
		f_class = NULL;
		class_uid = EmptyUID;
	};
	__fastcall MetaStandartTabularSection(tree* tr);
	__property int value = {read = fvalue};
	__property Class* _class = {read = f_class, write = f_class};
};

//---------------------------------------------------------------------------
// Параметры классов
class ClassParameter
{
private:
	String fname;
	static std::map<String, ClassParameter*> map;
public:
	//__fastcall ClassParameter(String _name) : fname(_name){map[fname] = this;};
	__fastcall ClassParameter(tree* tr);
	__property String name = {read = fname};
	static ClassParameter* __fastcall getparam(const String& paramname);
};

//---------------------------------------------------------------------------
// Классы
class Class
{
private:
	TGUID fuid;
	std::vector<VarValidValue> fvervalidvalues;
	std::map<ClassParameter*, int> fparamvalues;
	static std::map<TGUID, Class*> map;
	std::vector<MetaStandartAttribute*> fstandartattributes; // Стандартные реквизиты
	std::vector<MetaStandartTabularSection*> fstandarttabularsections; // Стандартные табличные части

public:
	__fastcall Class(tree* tr);
	__fastcall ~Class();

	__property TGUID uid = {read = fuid};
	__property std::vector<VarValidValue> vervalidvalues = {read = fvervalidvalues};
	__property std::map<ClassParameter*, int> paramvalues = {read = fparamvalues};
	__property std::vector<MetaStandartAttribute*> standartattributes = {read = fstandartattributes};
	__property std::vector<MetaStandartTabularSection*> standarttabularsections = {read = fstandarttabularsections};

	int __fastcall getparamvalue(ClassParameter* p);
	static Class* __fastcall getclass(const TGUID& id);

};

//---------------------------------------------------------------------------
// Экземпляр класса
class ClassItem
{
private:
	Class* fcl;
	bool fversionisset;
	int fversion;
	int __fastcall setversion(int v);
	int __fastcall getversion();
public:
	__fastcall ClassItem(Class* _cl) : fcl(_cl){fversionisset = false;};
	__property Class* cl = {read = fcl};
	__property int version = {read = getversion, write = setversion};
};

//---------------------------------------------------------------------------
// Допустимое значение переменной дерева сериализации
struct VarValidValue
{
	int value;
	Version1C ver1C;
	int globalvalue;
};

//---------------------------------------------------------------------------
// Переменная дерева сериализации
class SerializationTreeVar
{
private:
	String fname;
	bool fcolcount;
	bool fisglobal;
	bool fisfix;
	int ffixvalue;
	std::vector<VarValidValue> fvalidvalues;
public:
	__fastcall SerializationTreeVar(tree* tr);
	__property String name = {read = fname};
	__property bool colcount = {read = fcolcount};
	__property bool isglobal = {read = fisglobal};
	__property bool isfix = {read = fisfix};
	__property int fixvalue = {read = ffixvalue};
	__property std::vector<VarValidValue> validvalues = {read = fvalidvalues};
};


//---------------------------------------------------------------------------
// Типы узлов дерева сериализации
enum SerializationTreeNodeType
{
	stt_min = 0, // Минимум, для проверки

	stt_const = 1, // Константа
	stt_var = 2, // Переменная
	stt_list = 3, // Список
	stt_prop = 4, // Свойство
	stt_elcol = 5, // ЭлементКоллекции
	stt_gentype = 6, // ГенерируемыйТип
	stt_cond = 7, // Условие
	stt_metaid = 8, // МетаИД, идентификатор объекта метаданных
	stt_classcol = 9, // Коллекция классов
	stt_class = 10, // Класс
	stt_idcol = 11, // Коллекция ИД-элементов
	stt_idel = 12, // ИД-элемент

	stt_max // Максимум, для проверки
};

//---------------------------------------------------------------------------
// Типы значений дерева сериализации
enum SerializationTreeValueType
{
	//stv_min = 0, // Минимум, для проверки

	stv_string = 1, // Строка
	stv_number = 2, // Число
	stv_uid = 3, // УникальныйИдентификатор
	stv_value = 4, // Значение (MetaValue)
	stv_var = 5, // Переменная (SerializationTreeVar)
	stv_prop = 6, // Свойство (MetaProperty)
	stv_vercon = 7, // Версия контейнера
	stv_ver1C = 8, // Версия 1C
	stv_classpar = 9, // Параметр класса
	stv_globalvar = 10, // Глобальная переменная (SerializationTreeVar)
	stv_none = 11 // Нет значения

	//stv_max // Максимум, для проверки
};

//---------------------------------------------------------------------------
// Виды условий дерева сериализации
enum SerializationTreeCondition
{
	stc_min = 0, // Минимум, для проверки

	stc_e = 1, // Равно
	stc_ne = 2, // НеРавно
	stc_l = 3, // Меньше
	stc_g = 4, // Больше
	stc_le = 5, // МеньшеИлиРавно
	stc_ge = 6, // БольшеИлиРавно
	stc_bs = 7, // УстановленБит
	stc_bn = 8, // НеУстановленБит

	stc_max // Максимум, для проверки
};

//---------------------------------------------------------------------------
// Виды коллекций классов дерева сериализации
enum SerializationTreeClassType
{
	stct_min = 0, // Минимум, для проверки

	stct_inlist = 1, // Классы в списке
	stct_notinlist = 2, // Классы не в списке

	stct_max // Максимум, для проверки
};

//---------------------------------------------------------------------------
// Форматы внешних файлов типа (форматы двоичных данных)
enum ExternalFileFormat
{
	eff_min = 0,

	eff_servalue = 1, // СериализованноеЗначение
	eff_text = 2, // Текст (ИсходныйТекст)
	eff_tabdoc = 3, // ТабличныйДокумент
	eff_binary = 4, // ДвоичныеДанные
	eff_activedoc = 5, // ActiveДокумент
	eff_htmldoc = 6, // HTMLДокумент
	eff_textdoc = 7, // ТекстовыйДокумент
	eff_geo = 8, // ГеографическаяСхема
	eff_kd = 9, // СхемаКомпоновкиДанных
	eff_mkd = 10, // МакетОформленияКомпоновкиДанных
	eff_graf = 11, // ГрафическаяСхема
	eff_xml = 12, // XML
	eff_wsdl = 13, // WSDL
	eff_picture = 14, // Картинка
	eff_string = 15, // Строка (строка длиной > maxStringLength)

	eff_max
};

//---------------------------------------------------------------------------
// Типы сериализации двоичных данных
enum BinarySerializationType
{
	bst_min = 0,

	bst_empty = 1, // БезПрефикса
	bst_base64 = 2, // Префикс base64
	bst_data = 3, // Префикс data
	bst_base64_or_data = 4, // Префикс base64 или data (Преимущество base64)

	bst_max
};

//---------------------------------------------------------------------------
// Узел дерева сериализации
struct SerializationTreeNode
{
	SerializationTreeNode* next; // следующий на этом уровне
	SerializationTreeNode* first; // первый на подчиненном уровне
	SerializationTreeNode* parent; // родитель
	unsigned int index; // индекс на текущем уровне
	MetaType* owner;

	SerializationTreeNodeType type;
	SerializationTreeCondition condition; // (type == stt_cond)

	SerializationTreeValueType typeval1; // Тип значения 1 (type == stt_const ИЛИ type == stt_cond ИЛИ type == stt_elcol)
	String str1; // ((type == stt_const ИЛИ type == stt_cond ИЛИ type == stt_var ИЛИ type == stt_elcol) И typeval1 = stv_string ИЛИ typeval1 = stv_var ИЛИ typeval1 = stv_globalvar)
	union
	{
		int num1; // ((type == stt_const ИЛИ type == stt_cond ИЛИ type == stt_elcol) И typeval1 = stv_number)
		TGUID uid1; // (((type == stt_const ИЛИ type == stt_cond) И typeval1 = stv_uid) ИЛИ type == stt_class ИЛИ type == stt_idel)
		MetaValue* val1; // значение (type == stt_cond И typeval1 = stv_value)
		MetaProperty* prop1; // свойство (type == stt_prop ИЛИ ((type == stt_cond ИЛИ type == stt_elcol) И typeval1 = stv_prop))
		MetaGeneratedType* gentype; // генерируемый тип (type == stt_gentype)
		ContainerVer vercon1; // версия контейнера (type == stt_cond И typeval1 = stv_vercon)
		Version1C ver1C1; // версия 1С (type == stt_cond И typeval1 = stv_ver1С)
		SerializationTreeClassType classtype; // вид коллекции классов ((type == stt_classcol)
		ClassParameter* classpar1; // параметр класса (type == stt_cond И typeval1 = stv_classpar)
	};
	MetaType* typeprop; // тип свойства (type == stt_prop ИЛИ type == stt_elcol ИЛИ type == stt_idel)

	SerializationTreeValueType typeval2; // Тип значения 2 (type == stt_cond)
	String str2; // (type == stt_cond И (typeval2 = stv_string ИЛИ typeval2 = stv_var ИЛИ typeval2 = stv_globalvar))
	union
	{
		int num2; // (type == stt_cond И typeval2 = stv_number)
		TGUID uid2; // (type == stt_cond И typeval2 = stv_uid)
		MetaValue* val2; // значение (type == stt_cond И typeval2 = stv_value)
		MetaProperty* prop2; // свойство (type == stt_prop ИЛИ (type == stt_cond И typeval2 = stv_prop))
		ContainerVer vercon2; // версия контейнера (type == stt_cond И typeval2 = stv_vercon)
		Version1C ver1C2; // версия 1С ((type == stt_cond И typeval2 = stv_ver1С) ИЛИ type == stt_class ИЛИ type == stt_idel)
		ClassParameter* classpar2; // параметр класса (type == stt_cond И typeval2 = stv_classpar)
	};

	bool nomove; //
	bool isref; // Признак ссылки на объект метаданных (т.е. тут есть только UID, самого определения объекта метаданных тут нет)
	bool isrefpre; // Признак ссылки на предопределенный элемент (т.е. тут есть только UID, самого предопределенного элемента тут нет)
	bool isright; // Это право
	bool exnernal; // Объект находится во внешнем файле, тут только UID, который является именем внешнего файла
	BinarySerializationType binsertype;
	ExternalFileFormat binformat;

	__fastcall SerializationTreeNode()
	{
		next = NULL;
		first = NULL;
		nomove = false;
	};
	__fastcall SerializationTreeNode(MetaType* _owner, tree* tr, SerializationTreeNode* _parent, unsigned int _index);
	__fastcall ~SerializationTreeNode();
	String __fastcall path() const;
	static SerializationTreeNode* __fastcall SerializationTree(MetaType* _owner, tree* tr, SerializationTreeNode* parent);
	static String __fastcall typevalpresentation(SerializationTreeValueType typeval);
	String __fastcall typeval1presentation() const{return typevalpresentation(typeval1);};
	String __fastcall typeval2presentation() const{return typevalpresentation(typeval2);};
};

//---------------------------------------------------------------------------
// Внешний файл типа
struct ExternalFile
{
	MetaType* owner;
	MetaProperty* prop; // Свойство
	bool relativepath; // признак относительного пути файла
	String name; // Имя
	String ext; // Расширение (без точки впереди)
	bool catalog; // признак каталога (если catalog == true, то это файл <name+"."+ext+"/"+filename>, иначе <name+"."+ext>)
	String filename; // Имя файла (только если catalog == true)
	ExternalFileFormat format; // Формат файла
	MetaType* type; // Тип свойства
	bool optional; // признак необязательности
	Version1C ver1C; // Минимальная версия 1С
	bool havecondition; // признак наличия условия
	SerializationTreeCondition condition; // (type == stt_cond)

	SerializationTreeValueType typeval1; // Тип значения 1 (havecondition == true)
	String str1; // typeval1 = stv_string ИЛИ typeval1 = stv_var ИЛИ typeval1 = stv_globalvar
	union
	{
		int num1; // число (typeval1 = stv_number)
		TGUID uid1; // УИД (typeval1 = stv_uid)
		MetaValue* val1; // значение (typeval1 = stv_value)
		MetaProperty* prop1; // свойство (typeval1 = stv_prop)
		ContainerVer vercon1; // версия контейнера (typeval1 = stv_vercon)
		Version1C ver1C1; // версия 1С (typeval1 = stv_ver1С)
		ClassParameter* classpar1; // параметр класса (typeval1 = stv_classpar)
	};

	SerializationTreeValueType typeval2; // Тип значения 2 (havecondition == true)
	String str2; // typeval2 = stv_string ИЛИ typeval2 = stv_var ИЛИ typeval2 = stv_globalvar
	union
	{
		int num2; // число (typeval2 = stv_number)
		TGUID uid2; // УИД (typeval2 = stv_uid)
		MetaValue* val2; // значение (typeval2 = stv_value)
		MetaProperty* prop2; // свойство (typeval2 = stv_prop)
		ContainerVer vercon2; // версия контейнера (typeval2 = stv_vercon)
		Version1C ver1C2; // версия 1С (typeval2 = stv_ver1С)
		ClassParameter* classpar2; // параметр класса (typeval2 = stv_classpar)
	};

	__fastcall ExternalFile(MetaType* _owner, tree* tr);
	String __fastcall typeval1presentation() const{return SerializationTreeNode::typevalpresentation(typeval1);};
	String __fastcall typeval2presentation() const{return SerializationTreeNode::typevalpresentation(typeval2);};
};

//---------------------------------------------------------------------------
// Тип метаданных
class MetaType : public MetaBase
{
private:
	void __fastcall init();
protected:
	MetaTypeSet* typeSet; // набор типов, которому принадлежит этот тип

	TGUID fuid; // УИД типа
	bool fhasuid; // Признак наличия УИД
	String fmetaname;
	String femetaname;
	String fgentypeprefix;
	String fegentypeprefix;
	unsigned int fserialization_ver; // Вариант сериализации
	int fimageindex; // индекс картинки
	unsigned int fprenameindex; // Индекс колонки имя предопределенного
	unsigned int fpreidindex; // ИндексКолонкиИДПредопределенного
	std::vector<MetaProperty*> fproperties; // Свойства
	std::map<String, MetaProperty*> fproperties_by_name; // Соответствие имен (русских и английских) свойствам
	std::vector<MetaValue*> fvalues; // Предопределенные значения типа
	std::map<int, MetaValue*> fvalues_by_value; // Соответствие числовых значений предопределенным значениям
	std::map<String, MetaValue*> fvalues_by_name; // Соответствие имен (русских и английских) предопределенным значениям
	std::vector<MetaType*> fcollectiontypes; // Типы элементов коллекции
	std::vector<String> fscollectiontypes; // Имена типов элементов коллекции
	std::vector<MetaGeneratedType*> fgeneratedtypes; // Генерируемые типы
	bool fmeta; // Признак объекта метаданных
	ExportType fexporttype;
	Class* fdefaultclass;

	// Дерево сериализации
	std::vector<SerializationTreeVar*> fserializationvars;
	SerializationTreeNode* fserializationtree; // Если NULL - дерева сериализации нет
	std::vector<ExternalFile*> fexternalfiles;

public:
	__fastcall MetaType(
		MetaTypeSet* _typeSet,
		const String& _name,
		const String& _ename,
		const String& _metaname,
		const String& _emetaname,
		const String& _uid
	);
	__fastcall MetaType(
		MetaTypeSet* _typeSet,
		const String& _name,
		const String& _ename,
		const String& _metaname,
		const String& _emetaname,
		const TGUID& _uid
	);
	MetaGeneratedType* gentypeRef; // генерируемый тип Ссылка
	__fastcall MetaType(MetaTypeSet* _typeSet, tree* tr);
	__fastcall ~MetaType();
	void __fastcall setSerializationTree(tree* tr);
	__property String metaname = {read = fmetaname};
	__property String emetaname = {read = femetaname};
	__property String gentypeprefix = {read = fgentypeprefix};
	__property String egentypeprefix = {read = fegentypeprefix};
	__property bool hasuid = {read = fhasuid};
	__property TGUID uid = {read = fuid};
	__property unsigned int serialization_ver = {read = fserialization_ver};
	__property int imageindex = {read = fimageindex};
	__property unsigned int prenameindex = {read = fprenameindex};
	__property unsigned int preidindex = {read = fpreidindex};
	__property std::vector<MetaProperty*> properties = {read = fproperties};
	__property std::vector<MetaValue*> values = {read = fvalues};
	__property std::vector<MetaType*> collectiontypes = {read = fcollectiontypes};
	__property std::vector<MetaGeneratedType*> generatedtypes = {read = fgeneratedtypes};
	__property MetaTypeSet* TypeSet = {read = typeSet};
	__property std::vector<SerializationTreeVar*> serializationvars = {read = fserializationvars};
	__property SerializationTreeNode* serializationtree = {read = fserializationtree};
	__property std::vector<ExternalFile*> externalfiles = {read = fexternalfiles};
	__property bool meta = {read = fmeta};
	__property ExportType exporttype = {read = fexporttype};
	__property Class* defaultclass = {read = fdefaultclass};

	MetaProperty* __fastcall getProperty(const String& n);
	MetaProperty* __fastcall getProperty(int index);
	MetaValue* __fastcall getValue(const String& n);
	MetaValue* __fastcall getValue(int value);
	int __fastcall numberOfProperties();
	void __fastcall fillcollectiontypes(); // Заполнить типы элементов коллекции по их именам (по fscollectiontypes заполнить fcollectiontypes)
	String getmetaname(bool english = false)
	{
		if(english) return femetaname;
		else return fmetaname;
	}

};

//---------------------------------------------------------------------------
// Набор типов метаданных (статических или генерируемых)
class MetaTypeSet
{
private:
	std::map<String, MetaType*> mapname; // соответствие имен (русских и английских) типам
	std::map<TGUID, MetaType*> mapuid; // соответствие идентификаторов типам
	std::vector<MetaType*> alltype; // массив всех типов

public:
	static MetaTypeSet* staticTypes; // Cтатические типы
	// Пустой тип
	static MetaType* mt_empty;
	// Примитивные типы
	static MetaType* mt_string;
	static MetaType* mt_number;
	static MetaType* mt_bool;
	static MetaType* mt_date;
	static MetaType* mt_undef;
	static MetaType* mt_null;
	static MetaType* mt_type;
	// УникальныйИдентификатор
	static MetaType* mt_uid;
	// ОписаниеТипаВнутр
	static MetaType* mt_typedescrinternal;
	// Двоичные данные
	static MetaType* mt_binarydata;
	// Произвольный тип
	static MetaType* mt_arbitrary;
	// Корневой тип
	static MetaType* mt_container;
	static MetaType* mt_config;
	// Псевдо-тип Стандартный атрибут
	static MetaType* mt_standart_attribute;
	// Псевдо-тип Стандартная табличная часть
	static MetaType* mt_standart_tabular_section;
	// Значения частей даты для квалификатора даты
	static MetaValue* mv_datefractionsdate;
	static MetaValue* mv_datefractionstime;
	// Тип ЧастиДаты
	static MetaType* mt_datefractions;
	// Свойство ЧастиДаты типа КвалификаторыДаты
	static MetaProperty* mp_datefractions;
	// ОбъектМетаданных: ТабличнаяЧасть
	static MetaType* mt_tabularsection;
	// ОбъектМетаданных: Реквизит
	static MetaType* mt_attribute;
	// ОбъектМетаданныхСсылка
	static MetaType* mt_metaobjref;
	// ОбъектМетаданныхСсылкаВнутр
	static MetaType* mt_metarefint; // специальный тип для свойств с галочкой Ссылка в деревьях сериализации
	// ТабличнаяЧасть
	static MetaType* mt_tabsection;
	// МетаСсылка
	static MetaType* mt_metaref;

	__fastcall MetaTypeSet(){};
	__fastcall ~MetaTypeSet();
	MetaType* __fastcall getTypeByName(const String& n); // Получить тип по имени
	MetaType* __fastcall getTypeByUID(const TGUID& u);  // Получить тип по УИД
	void __fastcall fillall(); //
	void __fastcall add(MetaType* t);
	static void __fastcall staticTypesLoad(TStream* str);
	int __fastcall number();
	MetaType* getType(int index);
};

//---------------------------------------------------------------------------
// Виды значений 1С (для Value1C)
enum KindOfValue1C
{
	kv_unknown,   //Неинициализированное значение
	kv_bool,      //Булево
	kv_string,    //Строка
	kv_number,    //Целое число
	kv_number_exp,//Число с плавающей запятой
	kv_date,      //Дата
	kv_null,      //Null
	kv_undef,     //Неопределено
	kv_type,      //Тип
	kv_uid,       //Уникальный идентификатор
	kv_enum,      //Системное перечисление
	kv_stdattr,   //Стандартный реквизит
	kv_stdtabsec, //Стандартная табличная часть
	kv_obj,       //Объект
	kv_metaobj,   //Объект, являющийся объектом метаданных
	kv_refobj,    //Ссылка на объект, являющийся объектом метаданных
	kv_refpre,    //Ссылка на предопределенный элемент
	kv_right,     //Право
	kv_binary,    //Двоичные данные
	kv_extobj	  //Внешний объект
};

//---------------------------------------------------------------------------
// Значение генерируемого типа
struct GeneratedType
{
	TGUID typeuid; // УИД типа
	TGUID valueuid; // УИД значения
	__fastcall GeneratedType(tree** ptr, String path);
};

//---------------------------------------------------------------------------
// Предопределенный элемент
struct PredefinedValue
{
	String name; // Имя
	TGUID ref; // Ссылка
	Value1C_metaobj* owner; // Владелец предопределенного значения
	__fastcall PredefinedValue(const String& n, const TGUID& r, Value1C_metaobj* o) : name(n), ref(r), owner(o){};
	__fastcall ~PredefinedValue(){};
	String _fastcall getfullname(bool english = false);
};


//---------------------------------------------------------------------------
// Базовый класс значения 1С
class Value1C
{
protected:
	static wchar_t indentstring[31];
public:
	Value1C_obj* parent;
	int index; // Индекс в родителе (-1 - еще не определен, от 0 до (parent->v_objpropv.size() - 1) - именованое свойство, от parent->v_objpropv.size() - элемент коллекции)
	MetaType* type; // Тип значения 1С
	KindOfValue1C kind; // Вид хранимого значения

	__fastcall Value1C(Value1C_obj* _parent);
	virtual __fastcall ~Value1C(){};
	virtual String __fastcall presentation(bool english = false) = 0;
	virtual String __fastcall valuepresentation(bool english = false) = 0;
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false) = 0;
	virtual bool __fastcall isempty() = 0;
	String __fastcall path(MetaContainer* mc, bool english = false);
	String __fastcall fullpath(MetaContainer* mc, bool english = false);
};

//---------------------------------------------------------------------------
// Значение 1С типа Булево
class Value1C_bool : public Value1C
{
public:
	bool v_bool;    //Булево

	__fastcall Value1C_bool(Value1C_obj* _parent);
	virtual __fastcall ~Value1C_bool(){};
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа Строка
class Value1C_string : public Value1C
{
public:
	String v_string;

	__fastcall Value1C_string(Value1C_obj* _parent);
	virtual __fastcall ~Value1C_string(){};
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа Число
class Value1C_number : public Value1C
{
public:
	String v_string;
	int v_number;

	__fastcall Value1C_number(Value1C_obj* _parent);
	virtual __fastcall ~Value1C_number(){};
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа Число с плавающей запятой
class Value1C_number_exp : public Value1C
{
public:
	String v_string;
	double v_number;

	__fastcall Value1C_number_exp(Value1C_obj* _parent);
	virtual __fastcall ~Value1C_number_exp(){};
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа Дата
class Value1C_date : public Value1C
{
private:
	static char emptydate[7];
public:
	unsigned char v_date[7];    //Дата

	__fastcall Value1C_date(Value1C_obj* _parent);
	virtual __fastcall ~Value1C_date(){};
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа NULL
class Value1C_null : public Value1C
{
public:
	__fastcall Value1C_null(Value1C_obj* _parent);
	virtual __fastcall ~Value1C_null(){};
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа Неопределено
class Value1C_undef : public Value1C
{
public:
	__fastcall Value1C_undef(Value1C_obj* _parent);
	virtual __fastcall ~Value1C_undef(){};
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа Тип
class Value1C_type : public Value1C
{
public:
	MetaType* v_type;
	TGUID v_uid;

	__fastcall Value1C_type(Value1C_obj* _parent);
	virtual __fastcall ~Value1C_type(){};
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа УникальныйИдентификатор
class Value1C_uid : public Value1C
{
public:
	TGUID v_uid;

	__fastcall Value1C_uid(Value1C_obj* _parent);
	virtual __fastcall ~Value1C_uid(){};
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
};

//---------------------------------------------------------------------------
// Значение 1С - системное перечисление
class Value1C_enum : public Value1C
{
public:
	MetaValue* v_enum; // Значение системного перечисления

	__fastcall Value1C_enum(Value1C_obj* _parent);
	virtual __fastcall ~Value1C_enum(){};
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
};

//---------------------------------------------------------------------------
// Значение 1С - стандартный реквизит
class Value1C_stdattr : public Value1C
{
public:
	MetaObject* v_metaobj; // Объект метаданных
	MetaStandartAttribute* v_stdattr;
	int v_value;

	__fastcall Value1C_stdattr(Value1C_obj* _parent);
	virtual __fastcall ~Value1C_stdattr(){};
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
};

//---------------------------------------------------------------------------
// Значение 1С - стандартная табличная часть
class Value1C_stdtabsec : public Value1C
{
public:
	MetaObject* v_metaobj; // Объект метаданных
	MetaStandartTabularSection* v_stdtabsec;
	int v_value;

	__fastcall Value1C_stdtabsec(Value1C_obj* _parent);
	virtual __fastcall ~Value1C_stdtabsec(){};
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
};

//---------------------------------------------------------------------------
// Значение 1С объектного типа
class Value1C_obj : public Value1C
{
protected:
	static bool __fastcall compare(MetaProperty* p, Value1C* v);
public:
	MetaContainer* owner;
	std::map<MetaProperty*, Value1C*, MetaPropertyLess> v_objprop; //Объект - коллекция свойств
	std::vector<std::pair<MetaProperty*, Value1C*> > v_objpropv; // Коллекция свойств в векторе
	std::vector<Value1C*> v_objcol; //Объект (kv_obj или kv_metaobj???) - коллекция упорядоченных элементов
	std::map<String, int> globalvars; // Коллекция глобальных переменных со значениями
	void __fastcall fillpropv();

	__fastcall Value1C_obj(Value1C_obj* _parent, MetaContainer* _owner);
	virtual __fastcall ~Value1C_obj();
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
	virtual Value1C* __fastcall getproperty(MetaProperty* mp);
	virtual Value1C* __fastcall getproperty(const String& prop);
//	String __fastcall path(MetaContainer& mc);
//	String __fastcall fullpath(MetaContainer& mc);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
	Class* __fastcall getclass(bool immediately = false);
};

//---------------------------------------------------------------------------
// Значение 1С - объект метаданных
class Value1C_metaobj : public Value1C_obj
{
public:
	MetaObject* v_metaobj;
	std::map<MetaGeneratedType*, GeneratedType*> v_objgentypes; //коллекция генерируемых типов
	std::vector<PredefinedValue*> v_prevalues; // Коллекция предопределенных элементов

	__fastcall Value1C_metaobj(Value1C_obj* _parent, MetaContainer* _owner);
	virtual __fastcall ~Value1C_metaobj();
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
//	String __fastcall path(MetaContainer& mc);
//	String __fastcall fullpath(MetaContainer& mc);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
};

//---------------------------------------------------------------------------
// Значение 1С - ссылка на объект метаданных
class Value1C_refobj : public Value1C
{
public:
	MetaObject* v_metaobj;
	TGUID v_uid;

	__fastcall Value1C_refobj(Value1C_obj* _parent);
	virtual __fastcall ~Value1C_refobj(){};
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
};

//---------------------------------------------------------------------------
// Значение 1С - ссылка на предопределенное значение
class Value1C_refpre : public Value1C
{
public:
	PredefinedValue* v_prevalue; // Предопределенное значение (kv_refpre)

	__fastcall Value1C_refpre(Value1C_obj* _parent);
	virtual __fastcall ~Value1C_refpre(){};
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
};

//---------------------------------------------------------------------------
// Значение 1С - ссылка на предопределенное значение
class Value1C_right : public Value1C
{
public:
	MetaRight* v_right; // Право

	__fastcall Value1C_right(Value1C_obj* _parent);
	virtual __fastcall ~Value1C_right(){};
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
};

//---------------------------------------------------------------------------
// Значение 1С типа ДвоичныеДанные
class Value1C_binary : public Value1C
{
public:
	//String v_string; //Имя временного файла
	ExternalFileFormat v_binformat; // Формат двоичных данных
	TTempStream* v_binary; // Двоичные данные

	__fastcall Value1C_binary(Value1C_obj* _parent);
	virtual __fastcall ~Value1C_binary();
	virtual String __fastcall presentation(bool english = false);
	virtual String __fastcall valuepresentation(bool english = false);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	String get_file_extension();
	virtual bool __fastcall isempty();
};

//---------------------------------------------------------------------------
// Значение 1С - внешний объект
class Value1C_extobj : public Value1C_obj
{
public:
	bool opened; // признак, открыт ли внешний объект (т.е. распознан, или нет)
	String path; // путь внешнего файла
	TGUID metauid;

	__fastcall Value1C_extobj(Value1C_obj* _parent, MetaContainer* _owner, const String& _path, MetaType* _type, TGUID _metauid);
	virtual __fastcall ~Value1C_extobj();

	void __fastcall open();
	void __fastcall close();
	virtual Value1C* __fastcall getproperty(MetaProperty* mp);
	virtual Value1C* __fastcall getproperty(const String& prop);
	virtual bool __fastcall Export(const String& path, TStreamWriter* str, int indent, bool english = false);
	virtual bool __fastcall isempty();
};


//---------------------------------------------------------------------------
// Значение переменной дерева сериализации
struct VarValue
{
	int value;
	bool isset;
	__fastcall VarValue(){value = 0; isset = false;};
};

//---------------------------------------------------------------------------
// Структура неинициализированного значения
struct UninitValue1C
{
	Value1C* value; // Неинициализированное значение
	TGUID uid; // УИД значения
	String path; // Путь
	TGUID sauid; // УИД стандартного реквизита
	Value1C_stdtabsec* metats; // Значение стандартной табличной части для стандартного реквизита
	__fastcall UninitValue1C(Value1C* v, const String& p, const TGUID& u) : value(v), path(p), uid(u){};
	__fastcall UninitValue1C(Value1C* v, const String& p, const TGUID& u, const TGUID& su, Value1C_stdtabsec* mts) : value(v), path(p), uid(u), sauid(su), metats(mts){};
};

//---------------------------------------------------------------------------
// Контейнер метаданных (конфигурация, внешняя обработка, внешний отчет)
class MetaContainer
{
private:
	Value1C_obj* froot; // корневой объект контейнера
	MetaTypeSet* ftypes; // набор генерируемых типов
	std::map<TGUID, MetaObject*> fmetamap; // Соответствие УИД объектам метаданных
	std::map<String, MetaObject*> fsmetamap; // Соответствие полного имени объектам метаданных (на двух языках)
	std::map<TGUID, PredefinedValue*> fpredefinedvalues; // Соответствие УИД предопределенным элементам
	ContainerVer containerver;
	Version1C ver1C;
	bool useExternal; // Использовать отложенную загрузку внешних файлов

	// ==> Переменные времени загрузки
	String metaprefix;
	std::vector<UninitValue1C> uninitvalues;
	// <== Переменные времени загрузки

	// ==> Переменные времени выгрузки
	unsigned int export_thread_count; // количество потоков экспорта
	Value1C_obj_ExportThread** export_threads; // массив потоков экспорта
	// <== Переменные времени выгрузки

	static TGUID sig_standart_attribute; // сигнатура стандартного реквизита
	static TGUID sig_standart_table_sec; // сигнатура стандартной табличной части
	static TGUID sig_ext_dimension; // сигнатура стандартного реквизита Субконто
	static TGUID sig_ext_dimension_type; // сигнатура стандартного реквизита ВидСубконто

	Value1C* __fastcall readValue1C(tree** ptr, MetaType* t, Value1C_obj* valparent, const TGUID& metauid, Value1C_stdtabsec*& metats, ClassItem* clitem, String path, bool checkend = false);
	Value1C* __fastcall readValue1C(MetaType* nt, const SerializationTreeNode* tn, tree*& tr, Value1C_obj* valparent, const TGUID& metauid, Value1C_stdtabsec*& metats, ClassItem* clitem, String& path);
	void __fastcall recursiveLoadValue1C(Value1C_obj* v, VarValue* varvalues, tree** ptr, const SerializationTreeNode* tn, TGUID& metauid, Value1C_stdtabsec*& metats, ClassItem* clitem, String& path, bool checkend = false);

	int __fastcall getVarValue(const String& vname, MetaType* t, VarValue* varvalues, ClassItem* clitem, String& path);
	void __fastcall setVarValue(const String& vname, MetaType* t, Value1C_obj* v, VarValue* varvalues, ClassItem* clitem, int value, String& path);

	void __fastcall readPredefinedValues(Value1C_metaobj* v, const int ni, const int ui, const Value1C_obj* vStrings, const String& spath);
public:
	ConfigStorage* stor;
	String storpresent;
	static tree* __fastcall gettree(ConfigStorage* stor, const String& path, bool reporterror = true);
	void __fastcall loadValue1C(Value1C_obj* v, tree** ptr, const SerializationTreeNode* tn, TGUID metauid, Value1C_stdtabsec* metats, ClassItem* clitem, String& path, bool checkend = false);
	void __fastcall inituninitvalues();

	__fastcall MetaContainer(ConfigStorage* _stor, bool _useExternal = false); // Если _useExternal истина, _stor принадлежит MetaContainer и удаляется в деструкторе. Иначе _stor принадлежит вызывающей стороне, и может быть удален сразу после выполнения конструктора
	__fastcall ~MetaContainer();

	MetaObject* __fastcall getMetaObject(const String& n); // Получить объект метаданных по имени
	MetaObject* __fastcall getMetaObject(const TGUID& u);  // Получить объект метаданных по УИД
	PredefinedValue* __fastcall getPreValue(const TGUID& u);  // Получить предопределенный элемент по УИД
	bool __fastcall Export(const String& path, bool english = false, unsigned int thread_count = 0);
	bool __fastcall ExportThread(Value1C_obj* v, const String& path, bool english);
	//bool __fastcall ExportThread(Value1C_obj* v, String path, bool english);

	__property Value1C_obj* root = {read = froot};
	__property MetaTypeSet* types = {read = ftypes};
	__property std::map<TGUID, MetaObject*> metamap = {read = fmetamap};
	__property std::map<String, MetaObject*> smetamap = {read = fsmetamap};
	__property std::map<TGUID, PredefinedValue*> predefinedvalues = {read = fpredefinedvalues};

	// ==> Переменные времени выгрузки
	long export_work_count; // количество заданий выгрузки
	// <== Переменные времени выгрузки

};

//---------------------------------------------------------------------------

class Value1C_obj_ExportThread : public TThread
{
private:
	MetaContainer* owner;
protected:
	void __fastcall Execute();
public:
	String path;
	bool english;
	Value1C_obj* v;
	long busy; // Признак занятости потока (0 - свободен, 1 - занят)
	TEvent* work; // Сигнал, означающий, что свойства (v, path, english) установлены и нужно сделать экспорт
	bool finish; // признак окончания всех экспортов.
	int singlethread; // счетчик-признак, что текущее задание нужно выполнить в один поток

	__fastcall Value1C_obj_ExportThread(MetaContainer* _owner);
	__fastcall ~Value1C_obj_ExportThread();

	// Процессно-зависимые свойства MetaContainer
	std::vector<UninitValue1C> uninitvalues;

};

//---------------------------------------------------------------------------

#endif

