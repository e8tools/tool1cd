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

#include "Packdata.h"
#include "MessageRegistration.h"
#include "DetailedException.h"

extern Registrator msreg_g;

Packdata::Packdata(boost::filesystem::path& file_path) {

	try {
		std::unique_ptr<TFileStream> in(new TFileStream(file_path, fmOpenRead | fmShareDenyNone));
		in->Seek(8, soFromBeginning);
		uint32_t count;
		in->Read(&count, sizeof(count));
		datahashes.resize(count);
		in->Read(datahashes.data(), count * sizeof(record_data_hash));
	}
	catch (...) {
		throw DetailedException("Ошибка открытия файла")
				.add_detail("Файл", file_path.string());
	}

	boost::filesystem::path pack_item = file_path;
	pack_item.replace_extension("pck");
	try {
		std::unique_ptr<TFileStream> tmp(new TFileStream(pack_item, fmOpenRead | fmShareDenyNone)); // в TWrapperStream есть метод reset
		pack = std::move(tmp);
	}
	catch (...) {
		throw DetailedException("Ошибка открытия файла")
				.add_detail("Файл", pack_item.string());
	}
}

TStream* Packdata::get_data(const char* datahash, bool &found) {
	TStream *buffer = nullptr;
	found = false;

	for(const auto &record: datahashes) {
		if(memcmp(datahash, record.datahash, DATAHASH_FIELD_LENGTH) == 0) {
			pack->Seek(record.offset, soBeginning);
			int64_t packlen = 0;
			pack->Read(&packlen, sizeof(packlen));
			buffer = new TTempStream;
			buffer->CopyFrom(pack.get(), packlen);
			buffer->Close();
			found = true;
		}
	}
	return buffer;
}
