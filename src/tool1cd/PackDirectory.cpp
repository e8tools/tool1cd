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

#include <regex>

#include "PackDirectory.h"
#include "DetailedException.h"
#include "Common.h"
#include "DetailedException.h"

boost::filesystem::path object_path(const boost::filesystem::path &rootpath, const std::string &datahash)
{
	// aabbccddeeff -> ./data/objects/aa/bbccddeeff
	return rootpath / datahash.substr(0, 2) / datahash.substr(2, datahash.size() - 2);
}


PackDirectory::PackDirectory() {

}

PackDirectory::PackDirectory(boost::filesystem::path& init_path) {
	init(init_path);
}

PackDirectory::~PackDirectory() {
}

void PackDirectory::init(boost::filesystem::path& init_path) {
	boost::filesystem::path subpath = init_path / "data" / "pack";
	if(!directory_exists(subpath)) {
		throw PackDirectoryDoesNotExistException("Не найден каталог хранилища. Обязательный с версии >= 6")
				.add_detail("Каталог", subpath.string());
	}
	std::regex pack_mask("pack-.*\\.ind");
	boost::filesystem::directory_iterator dit(subpath), dend;
	for (; dit != dend; dit++) {
		boost::filesystem::path current_path = dit->path();
		if (!std::regex_match(current_path.filename().string(), pack_mask)) {
			continue;
		}
		std::shared_ptr<Packdata> pd = std::make_shared<Packdata>(current_path);
		packdates.push_back(pd);
	}
	objects_path = init_path / "data" / "objects";
}

void binary_datahash(const std::string &datahash, void *buf)
{
	auto bytebuf = (uint8_t*)buf;
	for (int i = 0; i < DATAHASH_FIELD_LENGTH; ++i) {
		bytebuf[i] = from_hex_digit(datahash[i*2]) << 4 | from_hex_digit(datahash[i*2 + 1]);
	}
}

TStream* PackDirectory::get_data(const std::string &datahash, bool &found)
{
	found = false;

	char binary_hash[DATAHASH_FIELD_LENGTH];
	binary_datahash(datahash, binary_hash);

	for(auto& packdata: packdates) {
		TStream *buffer = packdata->get_data(binary_hash, found);
		if(found) {
			return buffer;
		}
	}

	auto data_path = object_path(objects_path, datahash);
	if (!boost::filesystem::exists(data_path)) {
		return nullptr;
	}

	try
	{
		TStream *buffer = new TFileStream(data_path, fmOpenRead | fmShareDenyNone);
		found = true;
		buffer->Close();
		return buffer;
	} catch (...) {
	}
	return nullptr;
}

