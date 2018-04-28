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

#ifndef CTOOL1CD_APPH
#define CTOOL1CD_APPH

#include <boost/filesystem.hpp>
#include <memory>

#include "Class_1CD.h"
#include "ParseCommandLine.h"
#include <string>
#include "Messenger.h"
#include "cfapi/APIcfBase.h"

class App {
public:
	App(char **szArglist, int nArgs, Messenger &mess);
	~App();

	int Run();

	App(const App &app) = delete;

private:
	std::unique_ptr<T_1CD> base1CD;
	CommandParse comm;
	Messenger& mess;

	bool ActionOpenBaseNotMonopolyChecked{ false };
	bool ActionXMLSaveBLOBToFileChecked{ false };
	bool ActionXMLUnpackBLOBChecked{ true };

	bool IsTrueString(const std::string &str) const;
	void export_all_to_xml(const ParsedCommand& pc);
	void export_to_xml(const ParsedCommand& pc);

	void export_to_binary(const ParsedCommand& pc);
	void import_from_binary(const ParsedCommand& pc);

	void save_config(const boost::filesystem::path& param_path);
	void save_config(const ParsedCommand& pc);

	void save_configsave(const boost::filesystem::path& param_path);
	void save_configsave(const ParsedCommand& pc);

	void save_vendors_configs(const boost::filesystem::path& param_path);
	void save_vendors_configs(const ParsedCommand& pc);
	void save_all_configs(const ParsedCommand& pc);

	void save_depot_config(const ParsedCommand& pc);
	void save_depot_config_part(const ParsedCommand& pc);

	void find_and_save_lost_objects(const ParsedCommand& pc);

	inline bool is_infobase() const;

};

#endif
