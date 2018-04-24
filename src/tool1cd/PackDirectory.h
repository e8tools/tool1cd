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
