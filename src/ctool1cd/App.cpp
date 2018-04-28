/*
    CTool1CD provides console front end to Tool1CD library
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of CTool1CD.

    CTool1CD is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    CTool1CD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with CTool1CD.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <boost/regex.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <iosfwd>
#include <vector>

#include "App.h"
#include "ParseCommandLine.h"
#include "ErrorCode.h"
#include "Messenger.h"

extern Registrator msreg_g;

using namespace std;
using namespace System;

// Константы

const string GENERAL_CONFIG_DEFAULT_NAME() {
	return string("cf") + CF_STR;
}

const string DATABASE_CONFIG_DEFAULT_NAME() {
	return string("dbcf") + CF_STR;
}

App::App(char **szArglist, int nArgs, Messenger &mess)
		: comm(szArglist, nArgs), mess(mess)
{}

App::~App()
{}

bool App::IsTrueString(const string &str) const
{
	string s = LowerCase(str);
	return Equal(s, "1") || Equal(s, "y") || Equal(s, "yes") || Equal(s, "д") ||
			Equal(s, "да");
}

// export_all_to_xml
void App::export_all_to_xml(const ParsedCommand &pc)
{
	Table *tbl = nullptr;

	boost::filesystem::path root_path(pc.param1);
	if (!directory_exists(root_path)) {
		return;
	}

	for (int j = 0; j < base1CD->get_numtables(); j++) {
		tbl = base1CD->get_table(j);

		if (!tbl->get_num_indexes()) {
			tbl->fill_records_index();
		}

		boost::filesystem::path filetable = root_path / (tbl->get_name() + ".xml");

		tbl->export_to_xml(filetable.string(), ActionXMLSaveBLOBToFileChecked, ActionXMLUnpackBLOBChecked);

		msreg_g.AddMessage("Выполнен экспорт таблицы в файл.", MessageState::Succesfull)
				.with("Таблица", tbl->get_name())
				.with("Файл", filetable.string());
	}
} // export_all_to_xml

//---------------------------------------------------------------------------
// export_to_xml
void App::export_to_xml(const ParsedCommand &pc)
{
	boost::regex *expr = nullptr;

	boost::filesystem::path root_path(pc.param1);
	if (!directory_exists(root_path)) {
		return;
	}

	string filter(pc.param2);
	filter = StringReplace(filter, "*", ".*", rfReplaceAll);
	filter = StringReplace(filter, "?", ".", rfReplaceAll);
	filter = StringReplace(filter, " ", "\n", rfReplaceAll);
	filter = StringReplace(filter, "\t", "\n", rfReplaceAll);
	filter = StringReplace(filter, ",", "\n", rfReplaceAll);
	filter = StringReplace(filter, ";", "\n", rfReplaceAll);

	TStringList filters;
	filters.SetText(filter);

	for (int m = filters.Count() - 1; m >= 0; m--) {
		if (filters[m].empty()) {
			filters.Delete(m);
		} else {
			filters[m] = string("^").append(LowerCase(filters[m])).append("$");
		}
	}

	int k = filters.Count();
	if (k == 0) {
		msreg_g.AddError("Список таблиц для выгрузки в XML пуст.");
		return;
	}

	expr = new boost::regex[k];
	for (int m = 0; m < k; m++) {
		expr[m] = boost::regex(filters[m]);
	}

	for (int j = 0; j < base1CD->get_numtables(); j++) {
		Table *tbl = base1CD->get_table(j);

		bool b = false;

		for (int m = 0; m < k; m++) {
			if (regex_match(LowerCase(tbl->get_name()), expr[m])) {
				b = true;
				break;
			}
		}

		if (b) {
			tbl->fill_records_index();

			boost::filesystem::path filetable = root_path / (tbl->get_name() + ".xml");
			tbl->export_to_xml(filetable.string(), ActionXMLSaveBLOBToFileChecked, ActionXMLUnpackBLOBChecked);
			msreg_g.AddMessage("Выполнен экспорт таблицы в файл.", MessageState::Succesfull)
				.with("Таблица", tbl->get_name())
				.with("Файл", filetable.string());
		}
	}

	delete[] expr;
} // export_to_xml

void App::export_to_binary(const ParsedCommand &pc)
{
	boost::filesystem::path root_path(pc.param1);
	if (!directory_exists(root_path)) {
		return;
	}

	string filter(pc.param2);
	filter = StringReplace(filter, "*", ".*", rfReplaceAll);
	filter = StringReplace(filter, "?", ".", rfReplaceAll);
	filter = StringReplace(filter, " ", "\n", rfReplaceAll);
	filter = StringReplace(filter, "\t", "\n", rfReplaceAll);
	filter = StringReplace(filter, ",", "\n", rfReplaceAll);
	filter = StringReplace(filter, ";", "\n", rfReplaceAll);

	TStringList filters;
	filters.SetText(filter);

	for (int m = filters.Count() - 1; m >= 0; m--) {
		if (filters[m].empty()) {
			filters.Delete(m);
		} else {
			filters[m] = string("^").append(LowerCase(filters[m])).append("$");
		}
	}

	int k = filters.Count();
	if (k == 0) {
		msreg_g.AddError("Список таблиц для выгрузки пуст.");
		return;
	}

	vector<boost::regex> expr(k);
	for (int m = 0; m < k; m++) {
		expr[m] = boost::regex(filters[m]);
	}

	for (int j = 0; j < base1CD->get_numtables(); j++) {

		Table *tbl = base1CD->get_table(j);

		bool found = false;

		for (int m = 0; m < k; m++) {
			if (regex_match(LowerCase(tbl->get_name()), expr[m])) {
				found = true;
				break;
			}
		}

		if (found) {
			if (!tbl->get_num_indexes()) {
				tbl->fill_records_index();
			}
			tbl->export_table(root_path.string());
			msreg_g.AddMessage("Выполнен экспорт таблицы в файл.", MessageState::Succesfull)
					.with("Таблица", tbl->get_name())
					.with("Каталог", root_path.string());
		}
	}

} // export_to_binary

void App::import_from_binary(const ParsedCommand &pc)
{
	boost::filesystem::path root_path(pc.param1);
	if (!directory_exists(root_path)) {
		return;
	}

	string filter(pc.param2);
	filter = StringReplace(filter, "*", ".*", rfReplaceAll);
	filter = StringReplace(filter, "?", ".", rfReplaceAll);
	filter = StringReplace(filter, " ", "\n", rfReplaceAll);
	filter = StringReplace(filter, "\t", "\n", rfReplaceAll);
	filter = StringReplace(filter, ",", "\n", rfReplaceAll);
	filter = StringReplace(filter, ";", "\n", rfReplaceAll);

	TStringList filters;
	filters.SetText(filter);

	for (int m = filters.Count() - 1; m >= 0; m--) {
		if (filters[m].empty()) {
			filters.Delete(m);
		} else {
			filters[m] = string("^").append(LowerCase(filters[m])).append("$");
		}
	}

	int k = filters.Count();
	if (k == 0) {
		msreg_g.AddError("Список таблиц для загрузки пуст.");
		return;
	}

	vector<boost::regex> expr(k);
	for (int m = 0; m < k; m++) {
		expr[m] = boost::regex(filters[m]);
	}

	for (int j = 0; j < base1CD->get_numtables(); j++) {

		Table *tbl = base1CD->get_table(j);

		bool found = false;

		for (int m = 0; m < k; m++) {
			if (regex_match(LowerCase(tbl->get_name()), expr[m])) {
				found = true;
				break;
			}
		}

		if (found) {
			if (!tbl->get_num_indexes()) {
				tbl->fill_records_index();
			}
			tbl->import_table(root_path.string());
			msreg_g.AddMessage("Выполнен импорт таблицы из файла.", MessageState::Succesfull)
					.with("Таблица", tbl->get_name())
					.with("Каталог", root_path.string());
		}
	}

} // import_from_binary

void App::save_config(const boost::filesystem::path& param_path)
{
	if(!is_infobase()) {
		return;
	}

	boost::filesystem::path cfpath = param_path;

	if (!boost::iequals(cfpath.extension().string(), CF_STR)) {
		if (!directory_exists(cfpath)) {
			return;
		}
		cfpath /= DATABASE_CONFIG_DEFAULT_NAME();
	}

	if (base1CD->save_config(cfpath)) {
		msreg_g.AddMessage("Сохранение конфигурации базы данных завершено.", MessageState::Succesfull)
				.with("Файл", cfpath.string());
	} else {
		msreg_g.AddMessage("Не удалось сохранить конфигурацию базы данных.", MessageState::Error)
				.with("Файл", cfpath.string());
	}

}

// save_config
void App::save_config(const ParsedCommand &pc)
{
	boost::filesystem::path cfpath(pc.param1);

	save_config(cfpath);

} // save_config

void App::save_configsave(const boost::filesystem::path& param_path)
{
	if(!is_infobase()) {
		return;
	}

	boost::filesystem::path cfpath = param_path;
	if (!boost::iequals(cfpath.extension().string(), CF_STR)) {
		if (!directory_exists(cfpath)) {
			return;
		}
		cfpath /= GENERAL_CONFIG_DEFAULT_NAME();
	}
	if (base1CD->save_configsave(cfpath)) {
		msreg_g.AddMessage("Сохранение основной конфигурации завершено.", MessageState::Succesfull)
				.with("Файл", cfpath.string());
	} else {
		msreg_g.AddMessage("Не удалось сохранить основную конфигурацию.", MessageState::Error)
				.with("Файл", cfpath.string());
	}
}

// save_configsave
void App::save_configsave(const ParsedCommand &pc)
{
	boost::filesystem::path cfpath(pc.param1);
	save_configsave(cfpath);

} // save_configsave

void App::save_vendors_configs(const boost::filesystem::path& param_path) {

	if(!is_infobase()) {
		return;
	}

	for (auto& supplier_config : base1CD->supplier_configs()) {
		string file_name = supplier_config->name() + " " + supplier_config->version() + CF_STR;
		boost::filesystem::path cfpath = param_path / file_name;
		if ( supplier_config->save_to_file(cfpath) ) {
			msreg_g.AddMessage("Сохранение конфигурации поставщика завершено.", MessageState::Succesfull)
					.with("Файл", cfpath.string());
		} else {
			msreg_g.AddMessage("Не удалось сохранить конфигурацию поставщика.", MessageState::Error)
					.with("Файл", cfpath.string());
		}
	}
}

// save_vendors_configs
void App::save_vendors_configs(const ParsedCommand &pc)
{
	boost::filesystem::path param_path(pc.param1);
	if (!directory_exists(param_path)) {
		return;
	}

	save_vendors_configs(param_path);

} // save_vendors_configs

// save_all_configs
void App::save_all_configs(const ParsedCommand &pc)
{
	boost::filesystem::path param_path(pc.param1);
	if (!directory_exists(param_path)) {
		return;
	}

	if(!is_infobase()) {
		return;
	}

	boost::filesystem::path dbpath = param_path / DATABASE_CONFIG_DEFAULT_NAME();
	save_config(dbpath);

	boost::filesystem::path cfpath = param_path / GENERAL_CONFIG_DEFAULT_NAME();
	save_configsave(cfpath);

	save_vendors_configs(param_path);
} // save_all_configs

// save_depot_config
void App::save_depot_config(const ParsedCommand &pc)
{
	if (!base1CD->is_depot) {
		msreg_g.AddError("Попытка выгрузки конфигурации хранилища из базы, не являющейся хранилищем конфигурации.");
		return;
	}
	int version_number;
	string version_number_param = pc.param1;

	if (Equal(version_number_param, "0")) {
		version_number = 0;
	} else {
		version_number = ToIntDef(version_number_param, 0);
	}

	version_number = base1CD->get_ver_depot_config(version_number);

	if (!version_number) {
		msreg_g.AddError("Запрошенной версии конфигурации нет в хранилище конфигурации.");
		return;
	}
	boost::filesystem::path cfpath(pc.param2);

	cfpath = boost::filesystem::absolute(cfpath);

	if (!boost::iequals(cfpath.extension().string(), CF_STR)) {
		if (!directory_exists(cfpath)) {
			return;
		}
		cfpath /= string("v").append(std::to_string(version_number)).append(CF_STR);
	}
	if (base1CD->save_depot_config(cfpath.string(), version_number)) {
		msreg_g.AddMessage("Сохранение конфигурации хранилища завершено.", MessageState::Succesfull)
				.with("Файл", cfpath.string());
	} else {
		msreg_g.AddMessage("Не удалось сохранить конфигурацию хранилища.", MessageState::Error)
				.with("Файл", cfpath.string());
	}
} // save_depot_config

// save_depot_config_part
void App::save_depot_config_part(const ParsedCommand &pc)
{
	if (!base1CD->is_depot) {
		msreg_g.AddError(
				"Попытка выгрузки файлов конфигурации хранилища из базы, не являющейся хранилищем конфигурации.");
		return;
	}

	std::string version_number_param = pc.param1;

	int32_t begin_version = 0;
	int32_t end_version = 0;

	auto splitter = version_number_param.find(':');
	if (splitter != string::npos) {
		end_version = ToIntDef(version_number_param
				.substr(splitter + 1, version_number_param.size() - splitter - 1)
				, 0);
		begin_version = ToIntDef(version_number_param
				.substr(0, splitter - 1)
				, 0);
	} else {
		begin_version = ToIntDef(version_number_param, 0);
		end_version = begin_version;
	}

	auto version_exists = [&](int32_t ver) {
		if (!ver) {
			msreg_g.AddError("Запрошенной версии конфигурации нет в хранилище конфигурации.").with("Версия", ver);
			return false;
		}
		return true;
	};

	begin_version = base1CD->get_ver_depot_config(begin_version);
	if (!version_exists(begin_version)) { return; }

	end_version = base1CD->get_ver_depot_config(end_version);
	if (!version_exists(end_version)) { return; }

	boost::filesystem::path save_path(pc.param2);
	if (!directory_exists(save_path, true)) {
		return;
	}

	if (base1CD->save_part_depot_config(save_path.string(), begin_version, end_version)) {
		msreg_g.AddMessage("Сохранение файлов конфигурации хранилища завершено.", MessageState::Succesfull)
				.with("Файл", save_path.string());
	} else {
		msreg_g.AddMessage("Не удалось сохранить файлы конфигурации хранилища.", MessageState::Error)
				.with("Файл", save_path.string());
	}
} // save_depot_config_part

// find_and_save_lost_objects
void App::find_and_save_lost_objects(const ParsedCommand &pc)
{

	boost::filesystem::path lost_objects(pc.param1);
	if (!directory_exists(lost_objects)) {
		return;
	}

	base1CD->find_and_save_lost_objects(lost_objects);
}

void out_gpl_header()
{
	cout << "cTool_1CD  Copyright 2009-2017 awa, Copyright 2017-2018 E8 Tools Contributors"
		 << endl
		 << "This program comes with ABSOLUTELY NO WARRANTY; "
				 "This is free software, and you are welcome to redistribute it under certain conditions."
		 << endl;
}

int App::Run()
{

	vector<ParsedCommand> &commands = comm.getcommands();

	if (commands.empty()) {
		out_gpl_header();
		cout << "Запусти cTool_1CD -h для справки" << endl;
		return 0;
	}

	// Первый цикл просмотра команд для определения ключей параметров
	for (const auto &pc : commands) {
		switch (pc.command) {
			case Command::help:
				cout << comm.gethelpstring() << endl;
				return 0;
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

	string f = comm.getfilename();
	if (f.empty()) {
		msreg_g.AddMessage("В командной строке не найден файл базы 1CD", MessageState::Error);
		return CTOOL_1CD_INVALID_ARGUMENT;
	}

	boost::filesystem::path dbpath(f);
	dbpath = boost::filesystem::absolute(dbpath);
	if (!boost::filesystem::exists(dbpath)) {
		msreg_g.AddMessage("Указанный файл базы 1CD не существует", MessageState::Error);
		return CTOOL_1CD_FILE_NOT_EXISTS;
	}

	base1CD.reset(new T_1CD(dbpath.string(), &mess, !ActionOpenBaseNotMonopolyChecked));

	if (base1CD->is_open()) {
		msreg_g.AddMessage ("База данных 1CD открыта", MessageState::Succesfull)
							.with("Файл", dbpath.string())
							.with("Версия базы", base1CD->ver)
							.with("Locale", string(base1CD->locale))
							.with("Режим", base1CD->get_readonly() ? string("Только чтение") : string("Редактирование"))
							.with("Количество таблиц", base1CD->get_numtables());
	} else {
		msreg_g.AddError("Попытка выгрузки файлов конфигурации хранилища без открытой базы.");
		return CTOOL_1CD_FILE_NOT_OPEN;
	}

	for (const auto &pc : commands) {
		try {
			switch (pc.command) {
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
				case Command::save_depot_config_part: {
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
		catch (string &s) {
			msreg_g.AddError(s);
		}
		catch (DetailedException &ex) {
			ex.show();
		}
		catch (Exception &ex) {
			msreg_g.AddError(ex.Message());
		}
		catch (...) {
			msreg_g.AddError("Неизвестная ошибка.");
		}
	}

	return 0;
}

inline bool App::is_infobase() const {
	if(!base1CD->is_infobase()) {
		msreg_g.AddError("Попытка выгрузки конфигурации из файла не являющегося информационной базой!");
		return false;
	}

	return true;
}
