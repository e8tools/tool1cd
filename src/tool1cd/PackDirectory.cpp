/*
 * PackDirectory.cpp
 *
 *  Created on: 8 окт. 2017 г.
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

TStream* PackDirectory::get_data(const std::string &datahash, bool &found)
{
	found = false;

	for(auto& packdata: packdates) {
		TStream *buffer = packdata->get_data(datahash.c_str(), found);
		if(found) {
			return buffer;
		}
	}

	auto data_path = object_path(objects_path, datahash);
	if (!boost::filesystem::exists(data_path)) {
		/*
			msreg_m.AddMessage_("Не найден файл", MessageState::Error,
					"Файл", s,
					"Таблица", "HISTORY",
					"Объект", sObjId,
					"Версия", lastver);
	 */
		return nullptr;
	}

	try
	{
		return new TFileStream(data_path, fmOpenRead | fmShareDenyNone);
	}
	catch(...)
	{
		/*
		msreg_m.AddMessage_("Ошибка открытия файла", MessageState::Error,
							"Файл", current_object_path.string(),
							"Таблица", "HISTORY",
							"Объект", sObjId,
							"Версия", lastver);
							*/
	}
	return nullptr;
}

