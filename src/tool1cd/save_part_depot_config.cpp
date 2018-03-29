#include <vector>
#include <System.IOUtils.hpp>
#include <boost/filesystem.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <memory>

#include "UZLib.h"
#include "Class_1CD.h"
#include "Base64.h"
#include "Common.h"
#include "TempStream.h"
#include "ConfigStorage.h"
#include "Constants.h"
#include "CRC32.h"
#include "PackDirectory.h"

using namespace std;

//---------------------------------------------------------------------------
// Сохранение файлов конфигурации в каталог из хранилища конфигураций
// ver_begin - начальный номер диапазона версий сохраняемых файлов конфигурации
// ver_end - конечный номер диапазона версий сохраняемых файлов конфигурации
// ver_begin > 0, ver_end > 0 - используется переданный номер версии
// ver_begin <= 0, ver_end <= 0 - номер версии от последней конфигурации. 0 - последняя конфигурация, -1 - предпоследняя и т.д., т.е. Номер версии определяется как номер последней + ver
bool T_1CD::save_part_depot_config(const string &_filename, int32_t ver_begin, int32_t ver_end)
{
	Field* fldv_vernum;
	Field* fldh_datahash;
	bool hasrech1;
	bool hasrech2;

	BinaryGuid curobj;

	Field* flde_datahash;

	bool ok;
	bool datapacked;
	bool deletesobj;
	bool iscatalog;
	bool hasext;
	char emptyimage[8];

	uint32_t i;
	int32_t v, lastver, n;
	depot_ver depotVer;
	uint32_t configVerMajor, configVerMinor;
	TMemoryStream* in;
	TMemoryStream* out;
	TStream* sobj;
	TStreamWriter* sw;
	TMemoryStream* sd;
	bool hasdeleted;
	PackDirectory pack_directory;
	V8Catalog* cat;
	TFileStream* f;

	union
	{
		char b[2];
		unsigned short s;
	} cv;


	try {

		assert_i_am_a_repository();

	} catch (DetailedException &exc) {
		exc.show();
		return false;
	}

	TableRecord *rec = nullptr;
	for(i = 0; i < table_depot->get_phys_numrecords(); i++)
	{
		rec = table_depot->get_record(i);
		if(!rec->is_removed()) {
			break;
		}
		delete rec;
		rec = nullptr;
	}
	if (!rec) {
		throw DetailedException("Не удалось прочитать запись в таблице DEPOT.");
	}

	depotVer = get_depot_version(*rec);

	delete rec;

	// "Нормализуем" версию конфигурации
	ver_begin = get_ver_depot_config(ver_begin);
	ver_end = get_ver_depot_config(ver_end);
	if(ver_end < ver_begin)
	{
		v = ver_begin;
		ver_begin = ver_end;
		ver_end = v;
	}

	// Ищем строку с номером версии конфигурации
	fldv_vernum = table_versions->get_field("VERNUM");

	rec = nullptr;
	n = 0;
	for (uint32_t i = 0; i < table_versions->get_phys_numrecords(); i++)
	{
		rec = table_versions->get_record(i);
		if (rec->is_removed()) {
			delete rec;
			rec = nullptr;
			continue;
		}
		int version = ToIntDef(rec->get_string(fldv_vernum), 0);
		if (version == ver_begin) {
			n++;
		}
		if (version == ver_end) {
			n++;
		}
		if (n >= 2) {
			break;
		}
	}

	if (n < 2) {
		throw DetailedException("В хранилище не найдены запрошенные версии конфигурации")
				.add_detail("Версия с", ver_begin)
				.add_detail("Версия по", ver_end);
	}

	boost::filesystem::path filepath = boost::filesystem::path(_filename);
	boost::filesystem::path root_path(filename); // путь к 1cd

	// Определяем версию структуры конфигурации (для файла version)
	if (depotVer >= depot_ver::Ver5) {
		const char *frec = rec->get_raw("CVERSION");
		cv.b[0] = frec[1];
		cv.b[1] = frec[0];
		configVerMajor = cv.s;
		frec += 2;
		cv.b[0] = frec[1];
		cv.b[1] = frec[0];
		configVerMinor = cv.s;
	} else {
		configVerMinor = 0;
		if(version == db_ver::ver8_0_3_0 || version == db_ver::ver8_0_5_0) {
			configVerMajor = 6;
		}
		else if(version == db_ver::ver8_1_0_0) {
			configVerMajor = 106;
		}
		else {
			configVerMajor = 216;
		}
	}

	delete rec;

	Field *fldh_objid      = table_history->get_field("OBJID");
	Field *fldh_vernum     = table_history->get_field("VERNUM");
	Field *fldh_removed    = table_history->get_field("REMOVED");
	Field *fldh_datapacked = table_history->get_field("DATAPACKED");
	Field *fldh_objdata    = table_history->get_field("OBJDATA");

	Field *flde_objid      = table_externals->get_field("OBJID");
	Field *flde_vernum     = table_externals->get_field("VERNUM");
	Field *flde_extname    = table_externals->get_field("EXTNAME");
	Field *flde_extverid   = table_externals->get_field("EXTVERID");
	Field *flde_datapacked = table_externals->get_field("DATAPACKED");
	Field *flde_extdata    = table_externals->get_field("EXTDATA");

	if(depotVer >= depot_ver::Ver6)
	{
		fldh_datahash = table_history->get_field("DATAHASH");
		flde_datahash = table_externals->get_field("DATAHASH");

		boost::filesystem::path root_dir = root_path.parent_path();
		pack_directory.init(root_dir);
	}
	else
	{
		fldh_datahash = nullptr;
		flde_datahash = nullptr;
	}

	Index *history_pk = table_history->get_index("PK");
	Index *externals_pk = table_externals->get_index("PK");

	TableRecord *rech = nullptr;
	TableRecord *rech1 = nullptr;
	TableRecord *rech2 = nullptr;
	TableRecord *rece = nullptr;

	uint32_t history_records = history_pk->get_numrecords();
	uint32_t external_records = externals_pk->get_numrecords();

	boost::filesystem::path cath (filepath);

	if(configVerMajor < 100)
	{
		cath /= "metadata";
	}

	if (!boost::filesystem::exists(cath)) {
		boost::filesystem::create_directories(cath);
	}

	lastver = -1;
	memset(emptyimage, 0, 8);

	sd = new TMemoryStream;
	sw = new TStreamWriter(sd, TEncoding::UTF8, 4096);
	hasdeleted = false;

	in = new TMemoryStream;
	out = new TMemoryStream;
	cat = nullptr;
	hasrech1 = false;
	hasrech2 = false;
	for (uint32_t history_iterator = 0, external_iterator = 0; history_iterator <= history_records; history_iterator++) {
		uint32_t i;
		if (history_iterator < history_records) {
			i = history_pk->get_numrec(history_iterator);
			rech = table_history->get_record(i);
		}

		if (rech->get<BinaryGuid>(fldh_objid) != curobj || history_iterator == history_records)
		{ // это новый объект или конец таблицы
			if(history_iterator)
				if(hasrech2)
				{
					lastver = ToIntDef(rech2->get_string(fldh_vernum), numeric_limits<int32_t>::max());
					string sObjId = rech2->get_string(fldh_objid);

					hasext = true;
					bool removed = rech2->get<bool>(fldh_removed);
					if (removed) {
						if(hasrech1)
						{
							sw->Write(sObjId + "\r\n");
							hasdeleted = true;
						}
						else hasext = false;
					}
					else
					{
						datapacked = false;
						if (!rech2->is_null_value(fldh_datapacked)) {
							if (rech2->get<bool>(fldh_datapacked)) {
								datapacked = true;
							}
						}

						if(datapacked)
						{
							bool changed = true;
							bool inreaded = false;
							if(hasrech1)
							{
								datapacked = false;
								if (!rech1->is_null_value(fldh_datapacked)) {
									if (rech1->get<bool>(fldh_datapacked)) {
										datapacked = true;
									}
								}
								if(datapacked)
								{
									auto b = rech1->get<table_blob_file>(fldh_objdata);
									if (!rech1->is_null_value(fldh_objdata) && (b.blob_start != 0 || b.blob_length != 0)) {

										table_history->readBlob(out, b.blob_start, b.blob_length);

										auto b2 = rech2->get<table_blob_file>(fldh_objdata);
										if (!rech2->is_null_value(fldh_objdata) && (b2.blob_start != 0 || b2.blob_length != 0)) {
										{
											table_history->readBlob(out, b2.blob_start, b2.blob_length);
											inreaded = true;
											if (in->GetSize() == out->GetSize()) {
												if(memcmp(in->GetMemory(), out->GetMemory(), in->GetSize()) == 0) {
													changed = false;
												}
											}
										}
									}
									else if(depotVer >= depot_ver::Ver6)
									{
										// TODO: тип под Datahash
									}
										if (memcmp(rech2->get_data(fldh_datahash)
												, rech1->get_data(fldh_datahash)
												, fldh_datahash->get_length()) == 0) {
											changed = false;
										}
									}
								}
							}

							if(changed)
							{
								ok = false;
								deletesobj = false;
								auto b = rech2->get<table_blob_file>(fldh_objdata);
								if(inreaded)
								{
									sobj = in;
									ok = true;
								}
								else if (!rech2->is_null_value(fldh_objdata) && (b.blob_start != 0 || b.blob_length != 0)) {
									table_history->readBlob(in, b.blob_start, b.blob_length);
									sobj = in;
									ok = true;
								}
								else if(depotVer >= depot_ver::Ver6)
								{
									sobj = pack_directory.get_data(rech2->get_string(fldh_datahash), ok);
								}

								if(!ok)
								{
									throw DetailedException("Ошибка чтения объекта конфигурации")
														.add_detail("Таблица", "HISTORY")
														.add_detail("Объект", sObjId)
														.add_detail("Версия", lastver);
								}
								TFileStream f(cath / sObjId, fmCreate);
								sobj->Seek(0, soFromBeginning);
								ZInflateStream(sobj, &f);
								if(deletesobj) delete sobj;
							}
						}
					}

					// Вот тут идем по EXTERNALS
					while(hasext)
					{
						if (external_iterator > external_records) {
							break;
						}

						BinaryGuid current_record_guid;
						if (rece != nullptr) {
							current_record_guid = rece->get<BinaryGuid>(flde_objid);
							if (current_record_guid > curobj) {
								break;
							}
						}
						if (rece != nullptr && current_record_guid == curobj)
						{
							v = ToIntDef(rece->get_string(flde_vernum), std::numeric_limits<int32_t>::max());
							if(v == lastver)
							{
								string ext_name = rece->get_string(flde_extname);
								if(removed)
								{
									sw->Write(ext_name + "\r\n");
									hasdeleted = true;
								}
								else
								{
									datapacked = rece->get<bool>(flde_datapacked);

									// ==> Поиск записи о файле
									// В случае отсутствия данных (datapacked = false) в этой записи пытаемся найти предыдущую запись с данными
									// (с тем же objid, extname и extverid), но в пределах ver_begin <= vernum < lastver
									BinaryGuid verid = rece->get<BinaryGuid>(flde_extverid);
									uint32_t je = external_iterator;
									while(!datapacked && v > ver_begin && je)
									{
										i = externals_pk->get_numrec(--je);
										rece = table_externals->get_record(i);
										if (rece->get<BinaryGuid>(flde_objid) != curobj) {
											break;
										}
										if (CompareIC(rece->get_string(flde_extname), ext_name)) {
											continue;
										}

										if (verid == rece->get<BinaryGuid>(flde_extverid))
										{
											v = ToIntDef(rece->get_string(flde_vernum), numeric_limits<int32_t>::max());
											if (v < ver_begin) {
												break;
											}
											datapacked = rece->get<bool>(flde_datapacked);
										}
									}
									// <== Поиск записи о файле

									if(datapacked)
									{
										ok = false;
										deletesobj = false;
										auto b = rece->get<table_blob_file>(flde_extdata);
										if (b.blob_start != 0 || b.blob_length != 0)
										{
											table_externals->readBlob(in, b.blob_start, b.blob_length);
											sobj = in;
											ok = true;
										}
										else if(depotVer >= depot_ver::Ver6)
										{
											sobj = pack_directory.get_data(rece->get_string(flde_datahash), ok);
										}
										if(!ok)
										{
											throw DetailedException("Ошибка чтения объекта конфигурации")
																.add_detail("Таблица", "EXTERNALS")
																.add_detail("Объект", sObjId)
																.add_detail("Файл конфигурации", ext_name)
																.add_detail("Версия", v);
										}
										{
											out->SetSize(0);
											sobj->Seek(0, soFromBeginning);
											ZInflateOrCopy(sobj, out);
											iscatalog = false;
											if(out->GetSize() > 0)
											{
												cat = new V8Catalog(out, false, true);
												iscatalog = cat->is_catalog();
											}
											if(iscatalog) cat->SaveToDir((cath / ext_name).string());
											else
											{
												TFileStream f(cath / ext_name, fmCreate);
												f.CopyFrom(out, 0);
											}
											delete cat;
											cat = nullptr;
											if(deletesobj) delete sobj;

										}

									}

								}
							}

						}
						if(external_iterator == external_records)
						{
							external_iterator++;
							break;
						}
						i = externals_pk->get_numrec(external_iterator++);
						rece = table_externals->get_record(i);
					}
				}

			curobj = rech->get<BinaryGuid>(fldh_objid);
			hasrech1 = false;
			hasrech2 = false;
		}

		if(history_iterator < history_records)
		{
			int v = ToIntDef(rech->get_string(fldh_vernum), numeric_limits<int32_t>::max());
			if (v < ver_begin)
			{
				rech1 = rech;
				hasrech1 = true;
			}
			else if (v <= ver_end)
			{
				rech2 = rech;
				hasrech2 = true;
			}
		}
	}

	delete sw;
	if(hasdeleted)
	{
		f = new TFileStream(root_path / "deleted", fmCreate);
		f->CopyFrom(sd, 0);
		delete f;
	}
	delete sd;

	return true;
}
