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
bool T_1CD::save_part_depot_config(const String& _filename, int32_t ver_begin, int32_t ver_end)
{
	char* rec;
	char* frec;
	Field* fldv_vernum;
	Field* fldh_datahash;
	char* rech; // текущая запись HISTORY
	char* rech1; // запись с версией < ver_begin
	bool hasrech1;
	char* rech2; // запись с версией <= ver_end
	bool hasrech2;

	char curobj[16];

	Field* flde_datahash;
	char* rece;

	bool ok;
	bool removed;
	bool datapacked;
	bool deletesobj;
	bool iscatalog;
	bool inreaded;
	bool hasext;
	char emptyimage[8];
	char verid[16];
	uint32_t i;
	int32_t v, res, lastver, n;
	String s, ss, sn, se;
	depot_ver depotVer;
	uint32_t configVerMajor, configVerMinor;
	TMemoryStream* in;
	TMemoryStream* out;
	TStream* sobj;
	TStreamWriter* sw;
	TMemoryStream* sd;
	bool hasdeleted;
	PackDirectory pack_directory;
	v8catalog* cat;
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

	rec = new char[table_depot->get_recordlen()];
	ok = false;
	for(i = 0; i < table_depot->get_phys_numrecords(); i++)
	{
		table_depot->getrecord(i, rec);
		if(!*rec)
		{
			ok = true;
			break;
		}
	}
	if(!ok)
	{
		msreg_m.AddError("Не удалось прочитать запись в таблице DEPOT.");
		delete[] rec;
		return false;
	}

	depotVer = get_depot_version(rec);

	delete[] rec;

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

	rec = new char[table_versions->get_recordlen()];
	n = 0;
	for(i = 0; i < table_versions->get_phys_numrecords(); i++)
	{
		table_versions->getrecord(i, rec);
		if(*rec) continue;
		s = fldv_vernum->get_presentation(rec, true);
		v = s.ToIntDef(0);
		if(v == ver_begin) n++;
		if(v == ver_end) n++;
		if(n >= 2) break;
	}

	if(n < 2)
	{
		msreg_m.AddMessage_("В хранилище не найдены запрошенные версии конфигурации", MessageState::Error
				, "Версия с", ver_begin
				, "Версия по", ver_end);
		delete[] rec;
		return false;
	}

	boost::filesystem::path filepath = boost::filesystem::path(static_cast<std::string>(_filename));
	boost::filesystem::path root_path(static_cast<std::string>(filename)); // путь к 1cd

	// Определяем версию структуры конфигурации (для файла version)
	if (depotVer >= depot_ver::Ver5) {
		Field *fldv_cversion = table_versions->get_field("CVERSION");
		frec = rec + fldv_cversion->offset;
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

	delete[] rec;

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

	boost::filesystem::path objects_path;

	if(depotVer >= depot_ver::Ver6)
	{
		fldh_datahash = table_history->get_field("DATAHASH");
		flde_datahash = table_externals->get_field("DATAHASH");

		boost::filesystem::path root_dir = root_path.parent_path();
		pack_directory.init(root_dir);
		objects_path = root_path.parent_path() / "data" / "objects";
	}
	else
	{
		fldh_datahash = nullptr;
		flde_datahash = nullptr;
	}

	Index *history_pk = table_history->get_index("PK");
	Index *externals_pk = table_externals->get_index("PK");

	rech = new char[table_history->get_recordlen()];
	rech1 = new char[table_history->get_recordlen()];
	rech2 = new char[table_history->get_recordlen()];
	rece = new char[table_externals->get_recordlen()];
	memset(rece, 0, table_externals->get_recordlen());

	uint32_t history_records = history_pk->get_numrecords();
	uint32_t external_records = externals_pk->get_numrecords();
	memset(curobj, 0, 16);

	boost::filesystem::path cath (filepath);

	if(configVerMajor < 100)
	{
		cath /= "metadata";
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
			table_history->getrecord(i, rech);
		}

		if(memcmp(curobj, rech + fldh_objid->offset, 16) != 0 || history_iterator == history_records)
		{ // это новый объект или конец таблицы
			if(history_iterator)
				if(hasrech2)
				{
					s = fldh_vernum->get_presentation(rech2, false);
					lastver = s.ToIntDef(std::numeric_limits<int32_t>::max());
					sn = fldh_objid->get_presentation(rech2, false, L'.', true);

					hasext = true;
					removed = *(rech2 + fldh_removed->offset);
					if(removed)
					{
						if(hasrech1)
						{
							sw->Write(sn + "\r\n");
							hasdeleted = true;
						}
						else hasext = false;
					}
					else
					{
						datapacked = false;
						if(*(rech2 + fldh_datapacked->offset)) if(*(rech2 + fldh_datapacked->offset + 1)) datapacked = true;

						if(datapacked)
						{
							bool changed = true;
							bool inreaded = false;
							if(hasrech1)
							{
								datapacked = false;
								if(*(rech1 + fldh_datapacked->offset)) if(*(rech1 + fldh_datapacked->offset + 1)) datapacked = true;
								if(datapacked)
								{
									rec = rech1 + fldh_objdata->offset + 1;
									if(*(rech1 + fldh_objdata->offset) && memcmp(emptyimage, rec, 8))
									{
										table_history->readBlob(out, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
										rec = rech2 + fldh_objdata->offset + 1;
										if(*(rech2 + fldh_objdata->offset) && memcmp(emptyimage, rec, 8))
										{
											table_history->readBlob(in, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
											inreaded = true;
											if(in->GetSize() == out->GetSize()) if(memcmp(in->GetMemory(), out->GetMemory(), in->GetSize()) == 0) changed = false;
										}
									}
									else if(depotVer >= depot_ver::Ver6)
									{
										if(memcmp(rech2 + fldh_datahash->offset + (fldh_datahash->getnull_exists() ? 1 : 0)
												, rech1 + fldh_datahash->offset + (fldh_datahash->getnull_exists() ? 1 : 0)
												, fldh_datahash->getlength()) == 0) changed = false;
									}
								}
							}

							if(changed)
							{
								ok = false;
								deletesobj = false;
								rec = rech2 + fldh_objdata->offset + 1;
								if(inreaded)
								{
									sobj = in;
									ok = true;
								}
								else if(*(rech2 + fldh_objdata->offset) && memcmp(emptyimage, rec, 8))
								{
									table_history->readBlob(in, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
									sobj = in;
									ok = true;
								}
								else if(depotVer >= depot_ver::Ver6)
								{
									rec = rech2 + fldh_datahash->offset + (fldh_datahash->getnull_exists() ? 1 : 0);
									sobj = pack_directory.get_data(rec, ok);

									if(!ok)
									{
										ss = fldh_datahash->get_presentation(rech2, true);
										boost::filesystem::path current_object_path = objects_path / static_cast<std::string>(ss.SubString(1, 2)) / static_cast<std::string>(ss.SubString(3, ss.GetLength() - 2));
										if (boost::filesystem::exists(current_object_path))
										{
											try
											{
												sobj = new TFileStream(current_object_path, fmOpenRead | fmShareDenyNone);
												deletesobj = true;
												ok = true;
											}
											catch(...)
											{
												msreg_m.AddMessage_("Ошибка открытия файла", MessageState::Error,
																	"Файл", current_object_path.string(),
																	"Таблица", "HISTORY",
																	"Объект", sn,
																	"Версия", lastver);
											}
										}
										else
										{
											msreg_m.AddMessage_("Не найден файл", MessageState::Error,
																"Файл", s,
																"Таблица", "HISTORY",
																"Объект", sn,
																"Версия", lastver);
										}
									}
								}

								if(!ok)
								{
									msreg_m.AddMessage_("Ошибка чтения объекта конфигурации", MessageState::Error,
														"Таблица", "HISTORY",
														"Объект", sn,
														"Версия", lastver);
								}
								else
								{
									f = new TFileStream(cath / static_cast<string>(sn), fmCreate);
									sobj->Seek(0, soFromBeginning);
									ZInflateStream(sobj, f);
									delete f;
									if(deletesobj) delete sobj;

								}
							}
						}
					}

					// Вот тут идем по EXTERNALS
					while(hasext)
					{
						if(external_iterator > external_records) break;
						res = memcmp(rece + flde_objid->offset, curobj, 16);
						if(res > 0) break;
						if(!res)
						{
							s = flde_vernum->get_presentation(rece, false);
							v = s.ToIntDef(std::numeric_limits<int32_t>::max());
							if(v == lastver)
							{
								se = flde_extname->get_presentation(rece);
								if(removed)
								{
									sw->Write(se + "\r\n");
									hasdeleted = true;
								}
								else
								{
									datapacked = *(rece + flde_datapacked->offset);

									// ==> Поиск записи о файле
									// В случае отсутствия данных (datapacked = false) в этой записи пытаемся найти предыдущую запись с данными
									// (с тем же objid, extname и extverid), но в пределах ver_begin <= vernum < lastver
									memcpy(verid, rece + flde_extverid->offset, 16);
									uint32_t je = external_iterator;
									while(!datapacked && v > ver_begin && je)
									{
										i = externals_pk->get_numrec(--je);
										table_externals->getrecord(i, rece);
										res = memcmp(rece + flde_objid->offset, curobj, 16);
										if(res) break;
										s = flde_extname->get_presentation(rece);
										if(s.CompareIC(se)) continue;
										if(memcmp(verid, rece + flde_extverid->offset, 16) == 0)
										{
											s = flde_vernum->get_presentation(rece, false);
											v = s.ToIntDef(std::numeric_limits<int32_t>::max());
											if(v < ver_begin) break;
											datapacked = *(rece + flde_datapacked->offset);
										}
									}
									// <== Поиск записи о файле

									if(datapacked)
									{
										ok = false;
										deletesobj = false;
										frec = rece + flde_extdata->offset;
										if(memcmp(emptyimage, frec, 8))
										{
											table_externals->readBlob(in, *(uint32_t*)frec, *(uint32_t*)(frec + 4));
											sobj = in;
											ok = true;
										}
										else if(depotVer >= depot_ver::Ver6)
										{
											frec = rece + flde_datahash->offset + (flde_datahash->getnull_exists() ? 1 : 0);
											sobj = pack_directory.get_data(frec, ok);

											if(!ok)
											{
												ss = flde_datahash->get_presentation(rece, true);
												boost::filesystem::path current_object_path = objects_path / static_cast<std::string>(ss.SubString(1, 2)) / static_cast<std::string>(ss.SubString(3, ss.GetLength() - 2));
												if (boost::filesystem::exists(current_object_path))
												{
													try
													{
														sobj = new TFileStream(current_object_path, fmOpenRead | fmShareDenyNone);
														deletesobj = true;
														ok = true;
													}
													catch(...)
													{
														msreg_m.AddMessage_("Ошибка открытия файла", MessageState::Error,
																			"Файл", current_object_path.string(),
																			"Таблица", "EXTERNALS",
																			"Объект", sn,
																			"Файл конфигурации", se,
																			"Версия", v);
													}
												}
												else
												{
													msreg_m.AddMessage_("Не найден файл", MessageState::Error,
																		"Файл", current_object_path.string(),
																		"Таблица", "EXTERNALS",
																		"Объект", sn,
																		"Файл конфигурации", se,
																		"Версия", v);
												}
											}
										}
										if(!ok)
										{
											msreg_m.AddMessage_("Ошибка чтения объекта конфигурации", MessageState::Error,
																"Таблица", "EXTERNALS",
																"Объект", sn,
																"Файл конфигурации", se,
																"Версия", v);
										}
										else
										{
											out->SetSize(0);
											sobj->Seek(0, soFromBeginning);
											ZInflateStream(sobj, out);
											iscatalog = false;
											if(out->GetSize() > 0)
											{
												cat = new v8catalog(out, false, true);
												iscatalog = cat->IsCatalog();
											}
											if(iscatalog) cat->SaveToDir((cath / static_cast<string>(se)).string());
											else
											{
												f = new TFileStream(cath / static_cast<string>(se), fmCreate);
												f->CopyFrom(out, 0);
												delete f;
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
						table_externals->getrecord(i, rece);
					}
				}

			memcpy(curobj, rech + fldh_objid->offset, 16);
			hasrech1 = false;
			hasrech2 = false;
		}

		if(history_iterator < history_records)
		{
			s = fldh_vernum->get_presentation(rech, false);
			v = s.ToIntDef(std::numeric_limits<int32_t>::max());
			if(v < ver_begin)
			{
				memcpy(rech1, rech, table_history->get_recordlen());
				hasrech1 = true;
			}
			else if(v <= ver_end)
			{
				memcpy(rech2, rech, table_history->get_recordlen());
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

	delete[] rech;
	delete[] rech1;
	delete[] rech2;
	delete[] rece;

	return true;
}
