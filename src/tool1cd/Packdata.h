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

#ifndef SRC_CTOOL1CD_PACKDATA_H_
#define SRC_CTOOL1CD_PACKDATA_H_

#include "SystemClasses/TStream.hpp"
#include <boost/filesystem.hpp>
#include <array>
#include <memory>

#include "TempStream.h"
#include "Constants.h"

// Структура одной записи индекса упакованных объектов хранилища 8.3 (из файла *.ind)

#pragma pack(push)
#pragma pack(1)
struct record_data_hash
{
	char datahash[DATAHASH_FIELD_LENGTH];	// хэш файла
	int64_t offset;		// смещение файла в файле *.pck
};
#pragma pack(pop)

// Класс для одной пары файлов *.ind и *.pck в каталоге data\pack\ хранилища 8.3

class Packdata {
public:

	explicit Packdata(boost::filesystem::path& file_path);
	~Packdata() = default;

	TStream* get_data(const char* datahash, bool &found);
private:
	std::unique_ptr<TFileStream> pack;	// открытый на чтение файл *.pck
	std::vector<record_data_hash> datahashes;

};

#endif /* SRC_CTOOL1CD_PACKDATA_H_ */
