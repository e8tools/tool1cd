/*
 * Packdata.cpp
 *
 *  Created on: 2 окт. 2017 г.
 */

#include <regex>

#include "Packdata.h"
#include "MessageRegistration.h"

extern Registrator msreg_g;

Packdata::Packdata(boost::filesystem::path& file_path) {

	std::unique_ptr<TFileStream> in(new TFileStream(file_path.string(), fmOpenRead | fmShareDenyNone));

	in->Seek(8, soFromBeginning);
	uint32_t count;
	in->Read(&count, sizeof(count));
	datahashes.resize(count);
	for(auto &record: datahashes) {
		in->Read(&record.datahash, DATAHASH_FIELD_LENGTH);
		in->Read(&record.offset, sizeof(int64_t));
	}

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

	for(const auto &record: datahashes) {
		if(memcmp(datahash, record.datahash, DATAHASH_FIELD_LENGTH) == 0) {
			pack->Seek(record.offset, soBeginning);
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
