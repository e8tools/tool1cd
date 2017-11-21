/*
 * PackDirectory.cpp
 *
 *  Created on: 8 окт. 2017 г.
 */

#include <regex>
#include <exception>

#include "PackDirectory.h"
#include "Common.h"

PackDirectory::PackDirectory() {

}

PackDirectory::PackDirectory(boost::filesystem::path& init_path) {
	init(init_path);
}

PackDirectory::~PackDirectory() {
}

void PackDirectory::init(boost::filesystem::path& init_path) {
	boost::filesystem::path subpath = init_path / "data" / "pack";
	if(!directory_exists(subpath)){
		throw std::exception(); // FIXME: реализовать свой класс исключения
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
}

TStream* PackDirectory::get_data(const char* datahash, bool &find) {
	TStream *buffer = nullptr;
	find = false;

	for(auto& packdata: packdates) {
		buffer = packdata->get_data(datahash, find);
		if(find) {
			break;
		}
	}

	return buffer;
}

