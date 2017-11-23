#include <stdio.h>
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <iostream>
#include <System.IOUtils.hpp>
#include <vector>

#include "cTool_1CD_Main.h"
#include "ParseCommandLine.h"
#include "ErrorCode.h"
#include "Messenger.h"
#include "APIcfBase.h"

using namespace std;

TMultiReadExclusiveWriteSynchronizer* tr_syn = new TMultiReadExclusiveWriteSynchronizer();

//---------------------------------------------------------------------------
const int TEMP_BUFFER_SIZE = 4096;
char temp[TEMP_BUFFER_SIZE];

Registrator msreg_g;

//---------------------------------------------------------------------------
bool IsTrueString(const String &str)
{
	String s = str.LowerCase();
	return s.Compare("1") == 0 || s.Compare("y") == 0 || s.Compare("yes") == 0 || s.Compare("д") == 0 || s.Compare("да") == 0;
}

//---------------------------------------------------------------------------
//cmd_export_all_to_xml
void T1CD_cmd_export_all_to_xml(T_1CD& base1CD, const ParsedCommand& pc, Messenger& mess, bool ActionXMLSaveBLOBToFileChecked, bool ActionXMLUnpackBLOBChecked)
{
	Table* tbl = nullptr;

	if (base1CD.is_open())
	{
		boost::filesystem::path root_path(static_cast<string>(pc.param1));
		if(!directory_exists(root_path)) {
			return;
		}

		for (int j = 0; j < base1CD.get_numtables(); j++)
		{
			tbl = base1CD.gettable(j);

			if (!tbl->get_numindexes())
			{
				tbl->fillrecordsindex();
			}

			boost::filesystem::path filetable = root_path / static_cast<string>(tbl->getname() + ".xml");

			tbl->export_to_xml(filetable.string(), ActionXMLSaveBLOBToFileChecked, ActionXMLUnpackBLOBChecked);

			mess.AddMessage_("Выполнен экспорт таблицы в файл.", MessageState::Succesfull, "Таблица", tbl->getname(), "Файл", filetable.string());
		}
	}
	else
		mess.AddError("Попытка выгрузки всех таблиц в XML без открытой базы.");
} // T1CD_cmd_export_all_to_xml

//---------------------------------------------------------------------------
//cmd_export_to_xml
void T1CD_cmd_export_to_xml(T_1CD& base1CD, const ParsedCommand& pc, Messenger& mess, bool ActionXMLSaveBLOBToFileChecked, bool ActionXMLUnpackBLOBChecked)
{
	boost::regex* expr = nullptr;
	Table* tbl = nullptr;

	if (base1CD.is_open())
	{
		boost::filesystem::path root_path(static_cast<string>(pc.param1));
		if(!directory_exists(root_path)) {
			return;
		}

		Sysutils::TStringBuilder filter(pc.param2);
		filter.Replace("*", ".*");
		filter.Replace("?", ".");
		filter.Replace(" ", "\n");
		filter.Replace("\t", "\n");
		filter.Replace(",", "\n");
		filter.Replace(";", "\n");

		TStringList filters;
		filters.SetText(filter.ToString());
		for (int m = filters.Count() - 1; m >= 0; m--)
		{
			if (filters[m].Length() == 0)
				filters.Delete(m);
			else
				filters[m] = String("^") + filters[m].UpperCase() + "$";
		}

		int k = filters.Count();
		if (k == 0)
		{
			mess.AddError("Список таблиц для выгрузки в XML пуст.");
			return;
		}

		expr = new boost::regex[k];
		for (int m = 0; m < k; m++)
			expr[m] = boost::regex(static_cast<string>(filters[m]));

		for (int j = 0; j < base1CD.get_numtables(); j++)
		{
			tbl = base1CD.gettable(j);

			bool b = false;

			for (int m = 0; m < k; m++)
			{
				if (regex_match(static_cast<string>(tbl->getname().UpperCase()), expr[m]))
				{
					b = true;
					break;
				}
			}

			if (b)
			{
				if (!tbl->get_numindexes())
				{
					tbl->fillrecordsindex();
				}

				boost::filesystem::path filetable = root_path / static_cast<string>(tbl->getname() + ".xml");

				tbl->export_to_xml(filetable.string(), ActionXMLSaveBLOBToFileChecked, ActionXMLUnpackBLOBChecked);
				mess.AddMessage_("Выполнен экспорт таблицы в файл.", MessageState::Succesfull, "Таблица", tbl->getname(), "Файл", filetable.string());
			}
		}

		delete[] expr;
	}
	else
		mess.AddError("Попытка выгрузки таблиц в XML без открытой базы.");
} //T1CD_cmd_export_to_xml

void T1CD_cmd_export_to_binary(T_1CD& base1CD, const ParsedCommand& pc, Messenger& mess)
{
	if (!base1CD.is_open()) {
		mess.AddError("Попытка выгрузки таблиц без открытой базы.");
		return;
	}

	boost::filesystem::path root_path(static_cast<string>(pc.param1));
	if(!directory_exists(root_path)) {
		return;
	}

	Sysutils::TStringBuilder filter(pc.param2);
	filter.Replace("*", ".*");
	filter.Replace("?", ".");
	filter.Replace(" ", "\n");
	filter.Replace("\t", "\n");
	filter.Replace(",", "\n");
	filter.Replace(";", "\n");

	TStringList filters;
	filters.SetText(filter.ToString());

	for (int m = filters.Count() - 1; m >= 0; m--) {
		if (filters[m].Length() == 0) {
			filters.Delete(m);
		} else {
			filters[m] = String("^") + filters[m].UpperCase() + "$";
		}
	}

	int k = filters.Count();
	if (k == 0) {
		mess.AddError("Список таблиц для выгрузки пуст.");
		return;
	}

	vector<boost::regex> expr(k);
	for (int m = 0; m < k; m++) {
		expr[m] = boost::regex(static_cast<string>(filters[m]));
	}

	for (int j = 0; j < base1CD.get_numtables(); j++) {

		Table *tbl = base1CD.gettable(j);

		bool found = false;

		for (int m = 0; m < k; m++) {
			if (regex_match(static_cast<string>(tbl->getname().UpperCase()), expr[m])) {
				found = true;
				break;
			}
		}

		if (found) {
			if (!tbl->get_numindexes()) {
				tbl->fillrecordsindex();
			}
			tbl->export_table(root_path.string());
			mess.AddMessage_("Выполнен экспорт таблицы в файл.", MessageState::Succesfull, "Таблица", tbl->getname(), "Каталог", root_path.string());
		}
	}

} // T1CD_cmd_export_to_binary

void T1CD_cmd_import_from_binary(T_1CD& base1CD, const ParsedCommand& pc, Messenger& mess)
{
	if (!base1CD.is_open()) {
		mess.AddError("Попытка загрузки таблиц без открытой базы.");
		return;
	}

	boost::filesystem::path root_path(static_cast<string>(pc.param1));
	if(!directory_exists(root_path)) {
		return;
	}

	Sysutils::TStringBuilder filter(pc.param2);
	filter.Replace("*", ".*");
	filter.Replace("?", ".");
	filter.Replace(" ", "\n");
	filter.Replace("\t", "\n");
	filter.Replace(",", "\n");
	filter.Replace(";", "\n");

	TStringList filters;
	filters.SetText(filter.ToString());

	for (int m = filters.Count() - 1; m >= 0; m--) {
		if (filters[m].Length() == 0) {
			filters.Delete(m);
		} else {
			filters[m] = String("^") + filters[m].UpperCase() + "$";
		}
	}

	int k = filters.Count();
	if (k == 0) {
		mess.AddError("Список таблиц для загрузки пуст.");
		return;
	}

	vector<boost::regex> expr(k);
	for (int m = 0; m < k; m++) {
		expr[m] = boost::regex(static_cast<string>(filters[m]));
	}

	for (int j = 0; j < base1CD.get_numtables(); j++) {

		Table *tbl = base1CD.gettable(j);

		bool found = false;

		for (int m = 0; m < k; m++) {
			if (regex_match(static_cast<string>(tbl->getname().UpperCase()), expr[m])) {
				found = true;
				break;
			}
		}

		if (found) {
			if (!tbl->get_numindexes()) {
				tbl->fillrecordsindex();
			}
			tbl->import_table(root_path.string());
			mess.AddMessage_("Выполнен импорт таблицы из файла.", MessageState::Succesfull, "Таблица", tbl->getname(), "Каталог", root_path.string());
		}
	}

} // T1CD_cmd_export_to_binary


//---------------------------------------------------------------------------
//cmd_save_config
void T1CD_cmd_save_config(T_1CD& base1CD, const ParsedCommand& pc, Messenger& mess)
{
	if (base1CD.is_open())
	{
		boost::filesystem::path cfpath(static_cast<string>(pc.param1));
		if (!boost::iequals(cfpath.extension().string(), str_cf))
		{
			if (!directory_exists(cfpath)) {
				return;
			}
			cfpath /= "dbcf.cf"; // FIXME: заменить "dbcf.cf" константой
		}
		if (base1CD.save_config(cfpath.string()))
		{
			mess.AddMessage_("Сохранение конфигурации базы данных завершено.", MessageState::Succesfull, "Файл", cfpath.string());
		}
		else
		{
			mess.AddMessage_("Не удалось сохранить конфигурацию базы данных.", MessageState::Error, "Файл", cfpath.string());
		}
	}
	else
	{
		mess.AddError("Попытка выгрузки конфигурации базы данных без открытой базы.");
	}

} // T1CD_cmd_save_config

//---------------------------------------------------------------------------
//cmd_save_configsave
void T1CD_cmd_save_configsave(T_1CD& base1CD, const ParsedCommand& pc, Messenger& mess)
{
	if (base1CD.is_open())
	{
		boost::filesystem::path cfpath(static_cast<string>(pc.param1));
		if (!boost::iequals(cfpath.extension().string(), str_cf))
		{
			if (!directory_exists(cfpath)) {
				return;
			}
			cfpath /= "cf.cf"; // FIXME: заменить "cf.cf" константой
		}
		if (base1CD.save_configsave(cfpath.string()))
			mess.AddMessage_("Сохранение основной конфигурации завершено.", MessageState::Succesfull, "Файл", cfpath.string());
		else
			mess.AddMessage_("Не удалось сохранить основную конфигурацию.", MessageState::Error, "Файл", cfpath.string());
	}
	else
		mess.AddError("Попытка выгрузки основной конфигурации без открытой базы.");

} // T1CD_cmd_save_configsave

//---------------------------------------------------------------------------
//cmd_save_vendors_configs
void T1CD_cmd_save_vendors_configs(T_1CD& base1CD, const ParsedCommand& pc, Messenger& mess)
{
	if (base1CD.is_open()) {
		boost::filesystem::path param_path(static_cast<string>(pc.param1));
		if(!directory_exists(param_path)) {
			return;
		}

		base1CD.find_supplier_configs();
		for (size_t n = 0; n < base1CD.supplier_configs.size(); n++) {
			String file_name = base1CD.supplier_configs[n].name + " " + base1CD.supplier_configs[n].version + str_cf;
			boost::filesystem::path cfpath = param_path / static_cast<string>(file_name);
			if (base1CD.save_supplier_configs(n, cfpath.string())) {
				mess.AddMessage_("Сохранение конфигурации поставщика завершено.", MessageState::Succesfull, "Файл", cfpath.string());
			}
			else {
				mess.AddMessage_("Не удалось сохранить конфигурацию поставщика.", MessageState::Error, "Файл", cfpath.string());
			}
		}
	}
	else {
		mess.AddError("Попытка выгрузки конфигураций поставщиков без открытой базы.");
	}
} // T1CD_cmd_save_vendors_configs

//---------------------------------------------------------------------------
//cmd_save_all_configs
void T1CD_cmd_save_all_configs(T_1CD& base1CD, const ParsedCommand& pc, Messenger& mess)
{
	if (base1CD.is_open())
	{
		boost::filesystem::path param_path(static_cast<string>(pc.param1));
		if(!directory_exists(param_path)) {
			return;
		}

		boost::filesystem::path dbpath = param_path / "dbcf.cf"; // FIXME: заменить "dbcf.cf" константой
		if (base1CD.save_config(dbpath.string())) {
			mess.AddMessage_("Сохранение конфигурации базы данных завершено.", MessageState::Succesfull, "Файл", dbpath.string());
		}
		else {
			mess.AddMessage_("Не удалось сохранить конфигурацию базы данных.", MessageState::Error, "Файл", dbpath.string());
		}

		boost::filesystem::path cfpath = param_path / "cf.cf"; // FIXME: заменить "cf.cf" константой
		if (base1CD.save_configsave(cfpath.string())) {
			mess.AddMessage_("Сохранение основной конфигурации завершено.", MessageState::Succesfull, "Файл", cfpath.string());
		}
		else {
			mess.AddMessage_("Не удалось сохранить основную конфигурацию.", MessageState::Error, "Файл", cfpath.string());
		}

		base1CD.find_supplier_configs();
		for (size_t n = 0; n < base1CD.supplier_configs.size(); n++) {
			String file_name = base1CD.supplier_configs[n].name + " " + base1CD.supplier_configs[n].version + str_cf;
			boost::filesystem::path supplier_path = param_path / static_cast<string>(file_name);
			if (base1CD.save_supplier_configs(n, supplier_path.string())) {
				mess.AddMessage_("Сохранение конфигурации поставщика завершено.", MessageState::Succesfull, "Файл", supplier_path.string());
			}
			else {
				mess.AddMessage_("Не удалось сохранить конфигурацию поставщика.", MessageState::Error, "Файл", supplier_path.string());
			}
		}
	}
	else {
		mess.AddError("Попытка выгрузки всех конфигураций без открытой базы.");
	}

} // T1CD_cmd_save_all_configs

//---------------------------------------------------------------------------
//cmd_save_depot_config
void T1CD_cmd_save_depot_config(T_1CD& base1CD, const ParsedCommand& pc, Messenger& mess)
{
	if (!base1CD.is_open())
	{
		mess.AddError("Попытка выгрузки конфигурации хранилища без открытой базы.");
		return;
	}
	if (!base1CD.is_depot)
	{
		mess.AddError("Попытка выгрузки конфигурации хранилища из базы, не являющейся хранилищем конфигурации.");
		return;
	}
	int version_number;
	String version_number_param = pc.param1;

	if (version_number_param.Compare("0") == 0)
	{
		version_number = 0;
	}
	else
	{
		version_number = version_number_param.ToIntDef(0);
	}

	version_number = base1CD.get_ver_depot_config(version_number);

	if (!version_number)
	{
		mess.AddError("Запрошенной версии конфигурации нет в хранилище конфигурации.");
		return;
	}
	boost::filesystem::path cfpath(static_cast<string>(pc.param2));

	cfpath = boost::filesystem::absolute(cfpath);

	if (!boost::iequals(cfpath.extension().string(), str_cf))
	{
		if (!directory_exists(cfpath)) {
			return;
		}
		cfpath /= static_cast<string>(String(string("v") + version_number + string(str_cf)));
	}
	if (base1CD.save_depot_config(cfpath.string(), version_number))
		mess.AddMessage_("Сохранение конфигурации хранилища завершено.", MessageState::Succesfull, "Файл", cfpath.string());
	else
		mess.AddMessage_("Не удалось сохранить конфигурацию хранилища.", MessageState::Error, "Файл", cfpath.string());

} // T1CD_cmd_save_depot_config

//---------------------------------------------------------------------------
// cmd_save_depot_config_part
void T1CD_cmd_save_depot_config_part(T_1CD& base1CD, const ParsedCommand& pc, Messenger& mess)
{
	if (!base1CD.is_open()) {
		mess.AddError("Попытка выгрузки файлов конфигурации хранилища без открытой базы.");
		return;
	}
	if (!base1CD.is_depot) {
		mess.AddError("Попытка выгрузки файлов конфигурации хранилища из базы, не являющейся хранилищем конфигурации.");
		return;
	}

	String version_number_param = pc.param1;

	int32_t begin_version = 0;
	int32_t end_version = 0;

	int32_t splitter = version_number_param.Pos(":");
	if (splitter) {
		end_version = version_number_param
				.SubString(splitter + 1, version_number_param.Length() - splitter)
				.ToIntDef(0);
		begin_version = version_number_param
				.SubString(1, splitter - 1)
				.ToIntDef(0);
	}
	else {
		begin_version = version_number_param.ToIntDef(0);
		end_version = begin_version;
	}

	auto version_exists = [&] (int32_t ver) {
		if (!ver) {
			mess.AddError("Запрошенной версии конфигурации нет в хранилище конфигурации.", "Версия", ver);
			return false;
		}
		return true;
	};

	begin_version = base1CD.get_ver_depot_config(begin_version);
	if(!version_exists(begin_version)) { return; }

	end_version = base1CD.get_ver_depot_config(end_version);
	if(!version_exists(end_version)) { return; }

	boost::filesystem::path save_path(static_cast<string>(pc.param2));
	if(!directory_exists(save_path, true)) {
		return;
	}

	if (base1CD.save_part_depot_config(save_path.string(), begin_version, end_version)) {
		mess.AddMessage_("Сохранение файлов конфигурации хранилища завершено.", MessageState::Succesfull, "Файл", save_path.string());
	}
	else {
		mess.AddMessage_("Не удалось сохранить файлы конфигурации хранилища.", MessageState::Error, "Файл", save_path.string());
	}
} // T1CD_cmd_save_depot_config_part

void T1CD_cmd_find_and_save_lost_objects(T_1CD& base1CD, const ParsedCommand& pc, Messenger& mess) {

	boost::filesystem::path lost_objects(static_cast<string>(pc.param1));
	if (!directory_exists(lost_objects)) {
		return;
	}

	base1CD.find_and_save_lost_objects(lost_objects);
} // T1CD_cmd_find_and_save_lost_objects

//***************************************************************************

//---------------------------------------------------------------------------
// основная точка входа утилиты
int main(int argc, char* argv[])
{
	Messenger mess; // регистратор сообщений
	String f, v;

	bool ActionOpenBaseNotMonopolyChecked = false;
	bool ActionXMLSaveBLOBToFileChecked   = false;
	bool ActionXMLUnpackBLOBChecked       = true;

	msreg_g.AddMessageRegistrator(&mess);

	CommandParse comm(argv, argc, &mess);

	vector<ParsedCommand>& commands = comm.getcommands();

	if(commands.size() == 0)
	{
		cout << "cTool_1CD (c) awa 2009 - 2017" << endl << "Запусти cTool_1CD -h для справки" << endl;
		return 0;
	}

	mess.setlogfile("tool1cd.log");

	// Первый цикл просмотра команд для определения ключей параметров
	for( const auto& pc: commands ) {
		switch(pc.command) {
			case Command::help:
				cout << comm.gethelpstring() << endl;
				return 0;
				break;
			case Command::no_verbose:
				mess.setnoverbose(true);
				break;
			case Command::quit:
				//quit = true;
				break;
			case Command::logfile:
				mess.setlogfile(pc.param1);
				break;
			case Command::not_exclusively:
				ActionOpenBaseNotMonopolyChecked = true;
				break;
			case Command::xml_blob_to_file:
				ActionXMLSaveBLOBToFileChecked = IsTrueString(pc.param1);
				break;
			case Command::xml_parse_blob:
				ActionXMLUnpackBLOBChecked = IsTrueString(pc.param1);
				break;
		}
	}

	f = comm.getfilename();
	if(f.IsEmpty())
	{
		mess.AddMessage("В командной строке не найден файл базы 1CD", MessageState::Error);
		return CTOOL_1CD_INVALID_ARGUMENT;
	}

	boost::filesystem::path dbpath(static_cast<string>(f));
	dbpath = boost::filesystem::absolute(dbpath);
	if (!boost::filesystem::exists(dbpath))
	{
		mess.AddMessage("Указанный файл базы 1CD не существует", MessageState::Error);
		return CTOOL_1CD_FILE_NOT_EXISTS;
	}

	T_1CD base1CD(dbpath.string(), &mess, !ActionOpenBaseNotMonopolyChecked);
	if (base1CD.is_open())
	{
		mess.AddMessage_("База данных 1CD открыта", MessageState::Succesfull,
			"Файл", dbpath.string(),
			"Версия базы", base1CD.ver,
			"Locale", base1CD.locale,
			"Режим", base1CD.get_readonly() ? "Только чтение" : "Редактирование",
			"Количество таблиц", base1CD.get_numtables());
	}
	else {
		return CTOOL_1CD_FILE_NOT_OPEN;
	}

	for( const auto& pc: commands )	{
		try {
			switch(pc.command) {
				case Command::export_all_to_xml: {
					T1CD_cmd_export_all_to_xml(base1CD, pc, mess, ActionXMLSaveBLOBToFileChecked, ActionXMLUnpackBLOBChecked);
					break;
				}
				case Command::export_to_xml: {
					T1CD_cmd_export_to_xml(base1CD, pc, mess, ActionXMLSaveBLOBToFileChecked, ActionXMLUnpackBLOBChecked);
					break;
				}
				case Command::save_config: {
					T1CD_cmd_save_config(base1CD, pc, mess);
					break;
				}
				case Command::save_configsave: {
					T1CD_cmd_save_configsave(base1CD, pc, mess);
					break;
				}
				case Command::save_vendors_configs: {
					T1CD_cmd_save_vendors_configs(base1CD, pc, mess);
					break;
				}
				case Command::save_all_configs: {
					T1CD_cmd_save_all_configs(base1CD, pc, mess);
					break;
				}
				case Command::save_depot_config: {
					T1CD_cmd_save_depot_config(base1CD, pc, mess);
					break;
				}
				case Command::save_depot_config_part:{
					T1CD_cmd_save_depot_config_part(base1CD, pc, mess);
					break;
				}
				case Command::export_to_binary: {
					T1CD_cmd_export_to_binary(base1CD, pc, mess);
					break;
				}
				case Command::import_from_binary: {
					T1CD_cmd_import_from_binary(base1CD, pc, mess);
					break;
				}
				case Command::find_and_save_lost_objects: {
					T1CD_cmd_find_and_save_lost_objects(base1CD, pc, mess);
					break;
				}
			}
		}
		catch(String& s)
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

	if(mess.has_error)
	{
		return CTOOL_1CD_ERROR;
	}

	return 0;
}

