/*
 * PackDirectory.h
 *
 *  Created on: 8 окт. 2017 г.
 */

#ifndef SRC_CTOOL1CD_PACKDIRECTORY_H_
#define SRC_CTOOL1CD_PACKDIRECTORY_H_

#include <vector>
#include <memory>
#include <boost/filesystem.hpp>

#include "Packdata.h"

class PackDirectory {
public:
	PackDirectory();
	explicit PackDirectory(boost::filesystem::path& init_path);
	virtual ~PackDirectory();

	void init(boost::filesystem::path& init_path);
	TStream* get_data(const std::string &datahash, bool &found);
private:
	std::vector<std::shared_ptr<Packdata>> packdates;
	boost::filesystem::path objects_path;
};

#endif /* SRC_CTOOL1CD_PACKDIRECTORY_H_ */
