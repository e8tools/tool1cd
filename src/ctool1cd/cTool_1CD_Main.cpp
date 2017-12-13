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
#include "Parse_tree.h"

using namespace std;
using namespace System;

TMultiReadExclusiveWriteSynchronizer* tr_syn = new TMultiReadExclusiveWriteSynchronizer();

Registrator msreg_g;


App::App(char **szArglist, int nArgs, Messenger &mess)
	: comm(szArglist, nArgs), mess(mess)
{}

App::~App() {}

bool App::IsTrueString(const String &str) const
{
	String s = str.LowerCase();
	return s.Compare("1") == 0 || s.Compare("y") == 0 || s.Compare("yes") == 0 || s.Compare("д") == 0 || s.Compare("да") == 0;
}

// export_all_to_xml
void App::export_all_to_xml(const ParsedCommand& pc)
{
	Table* tbl = nullptr;

	boost::filesystem::path root_path(static_cast<string>(pc.param1));
	if(!directory_exists(root_path)) {
		return;
	}

	for (int j = 0; j < base1CD->get_numtables(); j++)
	{
		tbl = base1CD->gettable(j);

		if (!tbl->get_numindexes())
		{
			tbl->fillrecordsindex();
		}

		boost::filesystem::path filetable = root_path / static_cast<string>(tbl->getname() + ".xml");

		tbl->export_to_xml(filetable.string(), ActionXMLSaveBLOBToFileChecked, ActionXMLUnpackBLOBChecked);

		msreg_g.AddMessage_("Выполнен экспорт таблицы в файл.", MessageState::Succesfull, "Таблица", tbl->getname(), "Файл", filetable.string());
	}
} // export_all_to_xml

//---------------------------------------------------------------------------
// export_to_xml
void App::export_to_xml(const ParsedCommand& pc)
{
	boost::regex* expr = nullptr;
	Table* tbl = nullptr;

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
		}
		else {
			filters[m] = String("^") + filters[m].UpperCase() + "$";
		}
	}

	int k = filters.Count();
	if (k == 0) {
		msreg_g.AddError("Список таблиц для выгрузки в XML пуст.");
		return;
	}

	expr = new boost::regex[k];
	for (int m = 0; m < k; m++) {
		expr[m] = boost::regex(static_cast<string>(filters[m]));
	}

	for (int j = 0; j < base1CD->get_numtables(); j++) {
		tbl = base1CD->gettable(j);

		bool b = false;

		for (int m = 0; m < k; m++) {
			if (regex_match(static_cast<string>(tbl->getname().UpperCase()), expr[m])) {
				b = true;
				break;
			}
		}

		if (b) {
			if (!tbl->get_numindexes()) {
					tbl->fillrecordsindex();
			}

			boost::filesystem::path filetable = root_path / static_cast<string>(tbl->getname() + ".xml");
			tbl->export_to_xml(filetable.string(), ActionXMLSaveBLOBToFileChecked, ActionXMLUnpackBLOBChecked);
			msreg_g.AddMessage_("Выполнен экспорт таблицы в файл.", MessageState::Succesfull, "Таблица", tbl->getname(), "Файл", filetable.string());
		}
	}

	delete[] expr;
} // export_to_xml

void App::export_to_binary(const ParsedCommand& pc)
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

	for (int m = filters.Count() - 1; m >= 0; m--) {
		if (filters[m].Length() == 0) {
			filters.Delete(m);
		} else {
			filters[m] = String("^") + filters[m].UpperCase() + "$";
		}
	}

	int k = filters.Count();
	if (k == 0) {
		msreg_g.AddError("Список таблиц для выгрузки пуст.");
		return;
	}

	vector<boost::regex> expr(k);
	for (int m = 0; m < k; m++) {
		expr[m] = boost::regex(static_cast<string>(filters[m]));
	}

	for (int j = 0; j < base1CD->get_numtables(); j++) {

		Table *tbl = base1CD->gettable(j);

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
			msreg_g.AddMessage_("Выполнен экспорт таблицы в файл.", MessageState::Succesfull, "Таблица", tbl->getname(), "Каталог", root_path.string());
		}
	}

} // export_to_binary

void App::import_from_binary(const ParsedCommand& pc)
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

	for (int m = filters.Count() - 1; m >= 0; m--) {
		if (filters[m].Length() == 0) {
			filters.Delete(m);
		} else {
			filters[m] = String("^") + filters[m].UpperCase() + "$";
		}
	}

	int k = filters.Count();
	if (k == 0) {
		msreg_g.AddError("Список таблиц для загрузки пуст.");
		return;
	}

	vector<boost::regex> expr(k);
	for (int m = 0; m < k; m++) {
		expr[m] = boost::regex(static_cast<string>(filters[m]));
	}

	for (int j = 0; j < base1CD->get_numtables(); j++) {

		Table *tbl = base1CD->gettable(j);

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
			msreg_g.AddMessage_("Выполнен импорт таблицы из файла.", MessageState::Succesfull, "Таблица", tbl->getname(), "Каталог", root_path.string());
		}
	}

} // import_from_binary

// save_config
void App::save_config(const ParsedCommand& pc)
{
	boost::filesystem::path cfpath(static_cast<string>(pc.param1));
	if (!boost::iequals(cfpath.extension().string(), str_cf))
	{
		if (!directory_exists(cfpath)) {
			return;
		}
		cfpath /= "dbcf.cf"; // FIXME: заменить "dbcf.cf" константой
	}
	if (base1CD->save_config(cfpath.string()))
	{
		msreg_g.AddMessage_("Сохранение конфигурации базы данных завершено.", MessageState::Succesfull, "Файл", cfpath.string());
	}
	else
	{
		msreg_g.AddMessage_("Не удалось сохранить конфигурацию базы данных.", MessageState::Error, "Файл", cfpath.string());
	}

} // save_config

// save_configsave
void App::save_configsave(const ParsedCommand& pc)
{
	boost::filesystem::path cfpath(static_cast<string>(pc.param1));
	if (!boost::iequals(cfpath.extension().string(), str_cf))
	{
		if (!directory_exists(cfpath)) {
			return;
		}
		cfpath /= "cf.cf"; // FIXME: заменить "cf.cf" константой
	}
	if (base1CD->save_configsave(cfpath.string()))
		msreg_g.AddMessage_("Сохранение основной конфигурации завершено.", MessageState::Succesfull, "Файл", cfpath.string());
	else
		msreg_g.AddMessage_("Не удалось сохранить основную конфигурацию.", MessageState::Error, "Файл", cfpath.string());

} // save_configsave

// save_vendors_configs
void App::save_vendors_configs(const ParsedCommand& pc)
{
	boost::filesystem::path param_path(static_cast<string>(pc.param1));
	if(!directory_exists(param_path)) {
		return;
	}

	base1CD->find_supplier_configs();
	for (size_t n = 0; n < base1CD->supplier_configs.size(); n++) {
		String file_name = base1CD->supplier_configs[n].name + " " + base1CD->supplier_configs[n].version + str_cf;
		boost::filesystem::path cfpath = param_path / static_cast<string>(file_name);
		if (base1CD->save_supplier_configs(n, cfpath.string())) {
			msreg_g.AddMessage_("Сохранение конфигурации поставщика завершено.", MessageState::Succesfull, "Файл", cfpath.string());
		}
		else {
			msreg_g.AddMessage_("Не удалось сохранить конфигурацию поставщика.", MessageState::Error, "Файл", cfpath.string());
		}
	}

} // save_vendors_configs

// save_all_configs
void App::save_all_configs(const ParsedCommand& pc)
{
	boost::filesystem::path param_path(static_cast<string>(pc.param1));
	if(!directory_exists(param_path)) {
		return;
	}

	boost::filesystem::path dbpath = param_path / "dbcf.cf"; // FIXME: заменить "dbcf.cf" константой
	if (base1CD->save_config(dbpath.string())) {
		msreg_g.AddMessage_("Сохранение конфигурации базы данных завершено.", MessageState::Succesfull, "Файл", dbpath.string());
	}
	else {
		msreg_g.AddMessage_("Не удалось сохранить конфигурацию базы данных.", MessageState::Error, "Файл", dbpath.string());
	}

	boost::filesystem::path cfpath = param_path / "cf.cf"; // FIXME: заменить "cf.cf" константой
	if (base1CD->save_configsave(cfpath.string())) {
		msreg_g.AddMessage_("Сохранение основной конфигурации завершено.", MessageState::Succesfull, "Файл", cfpath.string());
	}
	else {
		msreg_g.AddMessage_("Не удалось сохранить основную конфигурацию.", MessageState::Error, "Файл", cfpath.string());
	}

	base1CD->find_supplier_configs();
	for (size_t n = 0; n < base1CD->supplier_configs.size(); n++) {
		String file_name = base1CD->supplier_configs[n].name + " " + base1CD->supplier_configs[n].version + str_cf;
		boost::filesystem::path supplier_path = param_path / static_cast<string>(file_name);
		if (base1CD->save_supplier_configs(n, supplier_path.string())) {
			msreg_g.AddMessage_("Сохранение конфигурации поставщика завершено.", MessageState::Succesfull, "Файл", supplier_path.string());
		}
		else {
			msreg_g.AddMessage_("Не удалось сохранить конфигурацию поставщика.", MessageState::Error, "Файл", supplier_path.string());
		}
	}

} // save_all_configs

// save_depot_config
void App::save_depot_config(const ParsedCommand& pc)
{
	if (!base1CD->is_depot)
	{
		msreg_g.AddError("Попытка выгрузки конфигурации хранилища из базы, не являющейся хранилищем конфигурации.");
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

	version_number = base1CD->get_ver_depot_config(version_number);

	if (!version_number)
	{
		msreg_g.AddError("Запрошенной версии конфигурации нет в хранилище конфигурации.");
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
	if (base1CD->save_depot_config(cfpath.string(), version_number))
		msreg_g.AddMessage_("Сохранение конфигурации хранилища завершено.", MessageState::Succesfull, "Файл", cfpath.string());
	else
		msreg_g.AddMessage_("Не удалось сохранить конфигурацию хранилища.", MessageState::Error, "Файл", cfpath.string());

} // save_depot_config

// save_depot_config_part
void App::save_depot_config_part(const ParsedCommand& pc)
{
	if (!base1CD->is_depot) {
		msreg_g.AddError("Попытка выгрузки файлов конфигурации хранилища из базы, не являющейся хранилищем конфигурации.");
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
			msreg_g.AddError("Запрошенной версии конфигурации нет в хранилище конфигурации.", "Версия", ver);
			return false;
		}
		return true;
	};

	begin_version = base1CD->get_ver_depot_config(begin_version);
	if(!version_exists(begin_version)) { return; }

	end_version = base1CD->get_ver_depot_config(end_version);
	if(!version_exists(end_version)) { return; }

	boost::filesystem::path save_path(static_cast<string>(pc.param2));
	if(!directory_exists(save_path, true)) {
		return;
	}

	if (base1CD->save_part_depot_config(save_path.string(), begin_version, end_version)) {
		msreg_g.AddMessage_("Сохранение файлов конфигурации хранилища завершено.", MessageState::Succesfull, "Файл", save_path.string());
	}
	else {
		msreg_g.AddMessage_("Не удалось сохранить файлы конфигурации хранилища.", MessageState::Error, "Файл", save_path.string());
	}
} // save_depot_config_part

// find_and_save_lost_objects
void App::find_and_save_lost_objects(const ParsedCommand& pc) {

	boost::filesystem::path lost_objects(static_cast<string>(pc.param1));
	if (!directory_exists(lost_objects)) {
		return;
	}

	base1CD->find_and_save_lost_objects(lost_objects);
}

int App::Run() {

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

	String &f = comm.getfilename();
	if(f.IsEmpty())
	{
		msreg_g.AddMessage("В командной строке не найден файл базы 1CD", MessageState::Error);
		return CTOOL_1CD_INVALID_ARGUMENT;
	}

	boost::filesystem::path dbpath(static_cast<string>(f));
	dbpath = boost::filesystem::absolute(dbpath);
	if (!boost::filesystem::exists(dbpath))
	{
		msreg_g.AddMessage("Указанный файл базы 1CD не существует", MessageState::Error);
		return CTOOL_1CD_FILE_NOT_EXISTS;
	}

	base1CD.reset(new T_1CD(dbpath.string(), &mess, !ActionOpenBaseNotMonopolyChecked));

	if (base1CD->is_open())
	{
		msreg_g.AddMessage_("База данных 1CD открыта", MessageState::Succesfull,
			"Файл", dbpath.string(),
			"Версия базы", base1CD->ver,
			"Locale", base1CD->locale,
			"Режим", base1CD->get_readonly() ? "Только чтение" : "Редактирование",
			"Количество таблиц", base1CD->get_numtables());
	}
	else {
		msreg_g.AddError("Попытка выгрузки файлов конфигурации хранилища без открытой базы.");
		return CTOOL_1CD_FILE_NOT_OPEN;
	}

	for( const auto& pc: commands )	{
		try {
			switch(pc.command) {
				case Command::export_all_to_xml: {
					export_all_to_xml(pc);
					break;
				}
				case Command::export_to_xml: {
					export_to_xml(pc);
					break;
				}
				case Command::save_config: {
					save_config(pc);
					break;
				}
				case Command::save_configsave: {
					save_configsave(pc);
					break;
				}
				case Command::save_vendors_configs: {
					save_vendors_configs(pc);
					break;
				}
				case Command::save_all_configs: {
					save_all_configs(pc);
					break;
				}
				case Command::save_depot_config: {
					save_depot_config(pc);
					break;
				}
				case Command::save_depot_config_part:{
					save_depot_config_part(pc);
					break;
				}
				case Command::export_to_binary: {
					export_to_binary(pc);
					break;
				}
				case Command::import_from_binary: {
					import_from_binary(pc);
					break;
				}
				case Command::find_and_save_lost_objects: {
					find_and_save_lost_objects(pc);
					break;
				}
			}
		}
		catch(String& s)
		{
			msreg_g.AddError(s);
		}
		catch(Exception& ex)
		{
			msreg_g.AddError(ex.Message());
		}
		catch(...)
		{
			msreg_g.AddError("Неизвестная ошибка.");
		}
	}

	return 0;
}

//***************************************************************************

//---------------------------------------------------------------------------
// основная точка входа утилиты
int main(int argc, char* argv[])
{
	Messenger mess; // регистратор сообщений

	msreg_g.AddMessageRegistrator(&mess);

	App app(argv, argc, mess);

	bool result = app.Run();
	if (!result) {
		return result;
	}

	if(mess.has_error)
	{
		return CTOOL_1CD_ERROR;
	}

	return 0;
}

