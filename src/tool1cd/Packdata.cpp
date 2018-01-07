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

	try {
		std::unique_ptr<TFileStream> in(new TFileStream(file_path, fmOpenRead | fmShareDenyNone));
		in->Seek(8, soFromBeginning);
		uint32_t count;
		in->Read(&count, sizeof(count));
		datahashes.resize(count);
		in->Read(datahashes.data(), count * sizeof(record_data_hash));
	}
	catch (...) {
		msreg_g.AddMessage_("Ошибка открытия файла", MessageState::Error,
				"Файл", file_path.string());
		throw;
	}

	boost::filesystem::path pack_item = file_path;
	pack_item.replace_extension("pck");
	try {
		std::unique_ptr<TFileStream> tmp(new TFileStream(pack_item, fmOpenRead | fmShareDenyNone)); // в TWrapperStream есть метод reset
		pack = std::move(tmp);
	}
	catch (...) {
		msreg_g.AddMessage_("Ошибка открытия файла", MessageState::Error,
				"Файл", pack_item.string());
		throw;
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
