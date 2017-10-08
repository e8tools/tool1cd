/*
 * PackDirectory.cpp
 *
 *  Created on: 8 окт. 2017 г.
 */

#include <regex>

#include "PackDirectory.h"

PackDirectory::PackDirectory(boost::filesystem::path& root_path) {
	boost::filesystem::path subpath = root_path / "data" / "pack";
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

PackDirectory::~PackDirectory() {
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

