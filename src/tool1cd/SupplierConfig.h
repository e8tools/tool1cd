/*
    Tool1CD library provides access to 1CD database files.
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of Tool1CD Library.

    Tool1CD Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tool1CD Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Tool1CD Library.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef SUPPLIERCONFIG_H
#define SUPPLIERCONFIG_H

#include <boost/filesystem.hpp>

#include "TableFiles.h"

struct TableFile;

// класс конфигурации поставщика
class SupplierConfig
{
public:
	SupplierConfig(TableFile *file,
				   const std::string &name,
				   const std::string &supplier,
				   const std::string &version);
	~SupplierConfig() = default;

	bool save_to_file(const boost::filesystem::path& file_name);
	std::string name() const;
	std::string supplier() const;
	std::string version() const;

	static std::shared_ptr<SupplierConfig> create_supplier_config(TableFile* table_file);

private:
	TableFile* _file {nullptr};
	std::string _name;     // имя конфигурация поставщика
	std::string _supplier; // синоним конфигурация поставщика
	std::string _version;  // версия конфигурация поставщика
};

#endif // SUPPLIERCONFIG_H
