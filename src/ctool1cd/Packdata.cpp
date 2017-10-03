/*
 * Packdata.cpp
 *
 *  Created on: 2 окт. 2017 г.
 */

#include <regex>

#include "Packdata.h"
#include "MessageRegistration.h"
#include "Constants.h"

extern Registrator msreg_g;

Packdata::Packdata(boost::filesystem::path& file_path) {

	std::unique_ptr<TFileStream> in(new TFileStream(file_path.string(), fmOpenRead | fmShareDenyNone));

	in->Seek(8, soFromBeginning);
	in->Read(&count, sizeof(count));
	datahashes.reset(new record_data_hash[count]);
	in->Read(datahashes.get(), count * sizeof(record_data_hash));

	boost::filesystem::path pack_item = file_path;
	pack_item.replace_extension("pck");
	std::unique_ptr<TFileStream> tmp(new TFileStream(pack_item.string(), fmOpenRead | fmShareDenyNone)); // в TWrapperStream есть метод reset
	pack = std::move(tmp);
}

Packdata::~Packdata() {
}

TStream* Packdata::get_data(const char* datahash, bool &find) {
	TStream *buffer = nullptr;
	find = false;

	for(uint32_t k = 0; k < count; k++) {
		if(memcmp(datahash, datahashes[k].datahash, DATAHASH_FIELD_LENGTH) == 0) {
			pack->Seek(datahashes[k].offset, soBeginning);
			int64_t packlen = 0;
			pack->Read(&packlen, sizeof(packlen));
			buffer = new TTempStream;
			buffer->CopyFrom(pack.get(), packlen);
			buffer->Close();
			find = true;
		}
	}
	return buffer;
}
