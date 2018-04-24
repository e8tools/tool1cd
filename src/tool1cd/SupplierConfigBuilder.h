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
#ifndef SUPPLIERCONFIGBUILDER_H
#define SUPPLIERCONFIGBUILDER_H

#include <memory>

#include "SupplierConfig.h"
#include "TableFiles.h"
#include "cfapi/V8Catalog.h"


class SupplierConfigBuilder
{
public:
	SupplierConfigBuilder() = delete;
	explicit SupplierConfigBuilder(TableFile *table_file);

	~SupplierConfigBuilder() = default;

	std::shared_ptr<SupplierConfig> build();

private:
	TableFile *_table_file {nullptr};
	std::unique_ptr<V8Catalog> main_catalog;

	int32_t get_version() const;
	V8File* get_file(const std::string &file_name) const;


};

#endif // SUPPLIERCONFIGBUILDER_H
