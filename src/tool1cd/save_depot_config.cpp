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
// Сохранение конфигурации в файл из хранилища конфигураций
// ver - номер версии сохраняемой конфигурации
// ver > 0 - используется переданный номер версии
// ver <= 0 - номер версии от последней конфигурации. 0 - последняя конфигурация, -1 - предпоследняя и т.д., т.е. Номер версии определяется как номер последней + ver
bool T_1CD::save_depot_config(const String& _filename, int32_t ver)
{
	char* rec;
	char* frec;
	Field* fldd_rootobjid;

	Field* fldv_vernum;
	Field* fldv_snapshotcrc;
	Field* fldv_snapshotmaker;

	Field* fldh_datahash;
	Index* index_history;
	char* rech1;
	char* rech2;

	char rootobj[16];
	char curobj[16];
	uint32_t ih, nh;

	Field* flde_datahash;
	Index* index_externals;
	char* rece;
	vector<char*> reces;
	vector<String> extnames;
	int32_t nreces;
	uint32_t ie, ne;

	bool ok;
	bool lastremoved;
	bool removed;
	bool datapacked;
	bool deletesobj;
	char emptyimage[8];
	int32_t lastver;
	depot_ver depotVer;
	uint32_t configVerMajor, configVerMinor;
	TStream* in;
	TStream* out;
	PackDirectory pack_directory;
	v8catalog* cat;
	v8catalog* cath;
	bool oldformat;
	tree* t;
	tree* tc;
	tree* tv; // корень дерева файла versions
	tree* tvc; // тек. элемент дерева файла versions
	tree* tr; // корень дерева файла root
	tree* trc; // тек. элемент дерева файла root
	tree* tcountv; // узел, содержащий счетчик в файле versions
	tree* tcountr; // узел, содержащий счетчик в файле root

	boost::uuids::random_generator uuid_gen;

	union
	{
		char cv_b[2];
		unsigned short cv_s;
	};

	try {

		assert_i_am_a_repository();

	} catch (DetailedException &exc) {
		exc.show();
		return false;
	}

	// Получаем версию хранилища
	fldd_rootobjid = table_depot->get_field("ROOTOBJID");

	rec = new char[table_depot->get_recordlen()];
	ok = false;
	for(uint32_t i = 0; i < table_depot->get_phys_numrecords(); i++)
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

	memcpy(rootobj, rec + fldd_rootobjid->offset, 16);
	delete[] rec;

	// "Нормализуем" версию конфигурации
	ver = get_ver_depot_config(ver);

	// Ищем строку с номером версии конфигурации

	fldv_vernum = table_versions->get_field("VERNUM");
	fldv_snapshotcrc = table_versions->get_field("SNAPSHOTCRC");
	fldv_snapshotmaker = table_versions->get_field("SNAPSHOTMAKER");

	rec = new char[table_versions->get_recordlen()];
	ok = false;
	for(uint32_t i = 0; i < table_versions->get_phys_numrecords(); i++)
	{
		table_versions->getrecord(i, rec);
		if(*rec) {
			continue;
		}
		int32_t vernum = fldv_vernum->get_presentation(rec, true).ToIntDef(0);
		if(vernum == ver)
		{
			ok = true;
			break;
		}
	}

	if(!ok)
	{
		msreg_m.AddMessage_("В хранилище не найдена версия конфигурации", MessageState::Error,
			"Требуемая версия", ver);
		delete[] rec;
		return false;
	}

	boost::filesystem::path filepath = boost::filesystem::path(static_cast<std::string>(_filename));
	boost::filesystem::path root_path(static_cast<std::string>(filename)); // путь к 1cd

	// Проверяем, нет ли снэпшота нужной версии конфигурации
	if( fldv_snapshotmaker->get_presentation(rec, true).Compare(EMPTY_GUID) != 0 )
	{
		String name_snap = "ddb";
		String ver_part  = "00000";
		ver_part  += ver;
		name_snap += ver_part.SubString(ver_part.GetLength() - 4, 5);
		name_snap += ".snp";

		boost::filesystem::path file_snap = root_path.parent_path() / "cache" / static_cast<std::string>(name_snap);

		msreg_m.AddMessage_("Попытка открытия файла снэпшота", MessageState::Info,
				"Файл", file_snap.string());

		if(boost::filesystem::exists(file_snap)) {
			try {
				in = new TFileStream(file_snap, fmOpenRead | fmShareDenyNone);
			}
			catch(...) {
				msreg_m.AddMessage_("Не удалось открыть файл снэпшота", MessageState::Warning,
					"Имя файла", file_snap.string(),
					"Требуемая версия", ver);
				in = nullptr;
			}
			try
			{
				out = new TFileStream(_filename, fmCreate | fmShareDenyWrite);
			}
			catch(...) {
				msreg_m.AddMessage_("Не удалось создать файл конфигурации", MessageState::Warning,
						"Имя файла", _filename);
				delete[] rec;
				return false;
			}
			if(in) {
				snapshot_version snap_ver = snapshot_version::Ver1;

				if( memcmp(rootobj, rec + fldv_snapshotmaker->offset + 1, 16) == 0 ||
					fldv_snapshotmaker->get_presentation(rec, true).Compare(SNAPSHOT_VER1) == 0 ) {
					snap_ver = snapshot_version::Ver1;
				}
				else if (fldv_snapshotmaker->get_presentation(rec, true).Compare(SNAPSHOT_VER2) == 0) {
					snap_ver = snapshot_version::Ver2;
				};

				switch (snap_ver) {
				case snapshot_version::Ver1:
				{
					try {
						ZInflateStream(in, out);
					}
					catch(...) {
						msreg_m.AddMessage_("Не удалось распаковать файл снэпшота", MessageState::Warning,
							"Имя файла", file_snap.string(),
							"Требуемая версия", ver);
						delete out;
						out = nullptr;
					}
					break;
				}
				case snapshot_version::Ver2:
				{
					out->CopyFrom(in, 0);
					break;
				}
				default:
					{
						msreg_m.AddMessage_("Неизвестная версия снэпшота", MessageState::Warning,
								"Имя файла", file_snap.string());
						delete out;
						out = nullptr;
					}
				};

				delete in;
				in = nullptr;
				if(out) {
					uint32_t snapshot_crc = *(uint32_t*)(rec + fldv_snapshotcrc->offset + 1);
					uint32_t calc_crc = _crc32(out);
					if(calc_crc == snapshot_crc) {
						delete out;
						delete[] rec;
						return true;
					}
					msreg_m.AddMessage_("Файл снэпшота испорчен (не совпала контрольная сумма)", MessageState::Warning,
							"Имя файла", file_snap.string(),
							"Требуемая версия", ver,
							"Должен быть CRC32", to_hex_string(snapshot_crc),
							"Получился CRC32", to_hex_string(calc_crc));
					delete out;
				}
			}
		}
		else {
			msreg_m.AddMessage_("Не найден файл снэпшота", MessageState::Warning,
					"Имя файла", file_snap.string(),
					"Требуемая версия", ver);
		}
	}

	// Определяем версию структуры конфигурации (для файла version)
	if(depotVer >= depot_ver::Ver5)
	{
		Field *fldv_cversion = table_versions->get_field("CVERSION");
		frec = rec + fldv_cversion->offset;
		cv_b[0] = frec[1];
		cv_b[1] = frec[0];
		configVerMajor = cv_s;
		frec += 2;
		cv_b[0] = frec[1];
		cv_b[1] = frec[0];
		configVerMinor = cv_s;
	}
	else
	{
		configVerMinor = 0;
		if(version == db_ver::ver8_0_3_0 || version == db_ver::ver8_0_5_0) configVerMajor = 6;
		else if(version == db_ver::ver8_1_0_0) configVerMajor = 106;
		else configVerMajor = 216;
	}

	delete[] rec;

	// Инициализируем таблицы HISTORY и EXTERNALS

	Field *fldh_objid      = table_history->get_field("OBJID");
	Field *fldh_vernum     = table_history->get_field("VERNUM");
	Field *fldh_objverid   = table_history->get_field("OBJVERID");
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

	if (depotVer >= depot_ver::Ver6)
	{
		fldh_datahash = table_history->get_field("DATAHASH");
		flde_datahash = table_externals->get_field("DATAHASH");

		boost::filesystem::path root_dir = root_path.parent_path();
		try {
			pack_directory.init(root_dir);
		}
		catch (...) {
			DetailedException error("Ошибка обработки файлов");
			error.add_detail("Каталог", root_dir.string());
			throw error;
		}

		objects_path = root_path.parent_path() / "data" / "objects";
	}
	else
	{
		fldh_datahash = nullptr;
		flde_datahash = nullptr;
	}

	index_history = table_history->get_index("PK");
	index_externals = table_externals->get_index("PK");

	rech1 = new char[table_history->get_recordlen()];
	rech2 = new char[table_history->get_recordlen()];
	rece = new char[table_externals->get_recordlen()];
	memset(rece, 0, table_externals->get_recordlen());
	nreces = 0;
	reces.resize(0);

	nh = index_history->get_numrecords();
	ne = index_externals->get_numrecords();
	memset(curobj, 0, 16);

	if (boost::filesystem::exists(filepath)) {
		boost::filesystem::remove(filepath);
	}
	cat = new v8catalog(filepath.string(), false);

	// root, versions

	std::map<String,String> vermap; // контейнер для сортировки versions
	std::map<String,String> rootmap; // контейнер для сортировки root
	std::map<String,TStream*> extmap; // контейнер для сортировки файлов в корне
	std::map<String,TStream*> metamap; // контейнер для сортировки файлов в metadata

	tv = new tree("",  node_type::nd_list, nullptr); // корень дерева файла versions
	tvc = new tree("", node_type::nd_list, tv); // тек. элемент дерева файла versions
	tr = new tree("",  node_type::nd_list, nullptr); // корень дерева файла root
	trc = new tree("", node_type::nd_list, tr); // тек. элемент дерева файла root

	tvc->add_child("1", node_type::nd_number);
	tcountv = tvc->add_child("0", node_type::nd_number); // узел, содержащий счетчик в файле versions

	vermap[""] = GUIDasMS(uuid_gen().data);

	String sversion;
	{// Создаем и записываем файл version
	String s;
	t = new tree("",  node_type::nd_list, nullptr);
	tc = new tree("", node_type::nd_list, t);
	tc = new tree("", node_type::nd_list, tc);
	s = configVerMajor;
	tc->add_child(s, node_type::nd_number);
	s = configVerMinor;
	tc->add_child(s, node_type::nd_number);
	sversion = outtext(t);
	delete t;
	}

	in = new TMemoryStream;
	in->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().size());
	{
		TStreamWriter sw(in, TEncoding::UTF8, 1024);
		sw.Write(sversion);
	}
	out = new TMemoryStream;
	in->Seek(0, soFromBeginning);
	ZDeflateStream(in, out);
	delete in;
	extmap["version"] = out;

	vermap["version"] = GUIDasMS(uuid_gen().data);

	if(configVerMajor < 100)
	{
		trc->add_child("1", node_type::nd_number);
		trc->add_child(GUIDasMS((unsigned char*)rootobj), node_type::nd_guid);
		tcountr = trc->add_child("0", node_type::nd_number); // узел, содержащий счетчик в файле root
		oldformat = true;
	}
	else
	{
		trc->add_child("2", node_type::nd_number);
		trc->add_child(GUIDasMS((unsigned char*)rootobj), node_type::nd_guid);
		tcountr = nullptr;
		oldformat = false;
	}

	lastver = -1;
	lastremoved = true;
	memset(emptyimage, 0, 8);

	in = new TMemoryStream;
	for(ih = 0, ie = 0; ih <= nh; ih++)
	{
		if(ih < nh)
		{
			uint32_t num_rec = index_history->get_numrec(ih);
			table_history->getrecord(num_rec, rech2);
		}

		if(memcmp(curobj, rech2 + fldh_objid->offset, 16) != 0 || ih == nh)
		{ // это новый объект или конец таблицы
			if(!lastremoved)
			{
				ok = false;
				deletesobj = false;
				rec = rech1 + fldh_objdata->offset + 1;
				if(*(rech1 + fldh_objdata->offset) && memcmp(emptyimage, rec, 8))
				{
					out = new TTempStream;
					if(oldformat)
					{
						table_history->readBlob(in, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
						in->Seek(0, soFromBeginning);
						ZInflateStream(in, out);
					}
					else table_history->readBlob(out, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
					out->Close();
					ok = true;
				}
				else if(depotVer >= depot_ver::Ver6)
				{
					rec = rech1 + fldh_datahash->offset + (fldh_datahash->getnull_exists() ? 1 : 0);
					out = pack_directory.get_data(rec, ok);

					if(!ok)
					{
						String ss = fldh_datahash->get_presentation(rech1, true);
						boost::filesystem::path current_object_path = objects_path / static_cast<std::string>(ss.SubString(1, 2)) / static_cast<std::string>(ss.SubString(3, ss.GetLength() - 2));
						if(boost::filesystem::exists(current_object_path))
						{
							try
							{
								out = new TFileStream(current_object_path, fmOpenRead | fmShareDenyNone);
								ok = true;
							}
							catch(...)
							{
								msreg_m.AddMessage_("Ошибка открытия файла", MessageState::Error,
									"Файл", current_object_path.string(),
									"Таблица", "HISTORY",
									"Объект", fldh_objid->get_presentation(rech1, false, L'.', true),
									"Версия", fldh_vernum->get_presentation(rech1, false));
							}
						}
						else
						{
							msreg_m.AddMessage_("Не найден файл", MessageState::Error,
								"Файл", current_object_path.string(),
								"Таблица", "HISTORY",
								"Объект", fldh_objid->get_presentation(rech1, false, L'.', true),
								"Версия", fldh_vernum->get_presentation(rech1, false));
						}
					}
				}
				String s = fldh_objid->get_presentation(rech1, false, L'.', true);
				if(!ok)
				{
					msreg_m.AddMessage_("Ошибка чтения объекта конфигурации", MessageState::Error,
						"Таблица", "HISTORY",
						"Объект", s,
						"Версия", fldh_vernum->get_presentation(rech1, false));
				}
				else
				{
					if(oldformat)
					{
						rootmap[s] = GUIDasMS((unsigned char*)rech1 + fldh_objverid->offset);
						metamap[s] = out;
					}
					else
					{
						vermap[s] = GUIDasMS((unsigned char*)rech1 + fldh_objverid->offset);
						extmap[s] = out;
					}

					// Вот тут идем по EXTERNALS
					while(true) {
						if(ie > ne) break;
						int32_t res = memcmp(rece + flde_objid->offset, curobj, 16);
						if(res > 0) break;
						if(!res)
						{
							int32_t vernum = flde_vernum->get_presentation(rece, false).ToIntDef(std::numeric_limits<int32_t>::max());
							s = flde_extname->get_presentation(rece);
							if(vernum <= ver && *(rece + flde_datapacked->offset)) {
								int32_t j;
								for(j = 0; j < nreces; j++) if(s.CompareIC(flde_extname->get_presentation(reces[j])) == 0) break;
								if(j == reces.size()){
									reces.resize(reces.size() + 1);
									reces[j] = new char[table_externals->get_recordlen()];
								}
								if(j == nreces) nreces++;
								memcpy(reces[j], rece, table_externals->get_recordlen());
							}
							if(vernum == lastver)
							{
								extnames.resize(extnames.size() + 1);
								extnames[extnames.size() - 1] = s;
							}
						}
						if(ie == ne)
						{
							ie++;
							break;
						}
						uint32_t num_rec = index_externals->get_numrec(ie++);
						table_externals->getrecord(num_rec, rece);
					}
					for(int32_t j = 0; j < nreces; j++)
					{
						rec = reces[j];
						String ext_name = flde_extname->get_presentation(rec);
						ok = false;
						for( const auto& name: extnames ) {
							if(ext_name.CompareIC(name) == 0) {
								ok = true;
								break;
							}
						}
						if(!ok) continue;

						ok = false;
						deletesobj = false;
						frec = rec + flde_extdata->offset;
						if(memcmp(emptyimage, frec, 8))
						{
							out = new TTempStream;
							table_externals->readBlob(out, *(uint32_t*)frec, *(uint32_t*)(frec + 4));
							out->Close();
							ok = true;
						}
						else if(depotVer >= depot_ver::Ver6)
						{
							frec = rec + flde_datahash->offset + (flde_datahash->getnull_exists() ? 1 : 0);
							out = pack_directory.get_data(frec, ok);

							if(!ok)
							{
								String ss = flde_datahash->get_presentation(rec, true);
								boost::filesystem::path current_object_path = objects_path / static_cast<std::string>(ss.SubString(1, 2)) / static_cast<std::string>(ss.SubString(3, ss.GetLength() - 2));
								if (boost::filesystem::exists(current_object_path))
								{
									try
									{
										out = new TFileStream(current_object_path, fmOpenRead | fmShareDenyNone);
										ok = true;
									}
									catch(...)
									{
										msreg_m.AddMessage_("Ошибка открытия файла", MessageState::Error,
											"Файл", current_object_path.string(),
											"Таблица", "EXTERNALS",
											"Объект", flde_extname->get_presentation(rec),
											"Версия", flde_vernum->get_presentation(rec));
									}
								}
								else
								{
									msreg_m.AddMessage_("Не найден файл", MessageState::Error,
										"Файл", current_object_path.string(),
										"Таблица", "EXTERNALS",
										"Объект", flde_extname->get_presentation(rec),
										"Версия", flde_vernum->get_presentation(rec));
								}
							}
						}
						if(!ok)
						{
							msreg_m.AddMessage_("Ошибка чтения объекта конфигурации", MessageState::Error,
								"Таблица", "EXTERNALS",
								"Объект", ext_name,
								"Версия", flde_vernum->get_presentation(rec));
						}
						else
						{
							vermap[ext_name] = GUIDasMS((unsigned char*)rec + flde_extverid->offset);
							extmap[ext_name] = out;
						}

					}
					nreces = 0;
					extnames.resize(0);
				}
			}

			memcpy(curobj, rech2 + fldh_objid->offset, 16);
			lastremoved = true;
		}

		if(ih < nh)
		{
			int32_t vernum = fldh_vernum->get_presentation(rech2, false).ToIntDef(std::numeric_limits<int32_t>::max());
			if(vernum <= ver)
			{
				removed = *(rech2 + fldh_removed->offset);
				if(removed)
				{
					lastremoved = true;
				}
				else
				{
					datapacked = false;
					if(*(rech2 + fldh_datapacked->offset)) if(*(rech2 + fldh_datapacked->offset + 1)) datapacked = true;
					if(datapacked)
					{
						memcpy(rech1, rech2, table_history->get_recordlen());
						lastremoved = false;
						lastver = vernum;
					}
				}
			}
		}
	}

	delete[] rech1;
	delete[] rech2;
	delete[] rece;
	for(size_t j = 0; j < reces.size(); j++) {
		delete[] reces[j];
	}


	// Завершаем формирование списков версий

	vermap["versions"] = GUIDasMS(uuid_gen().data);

	if(oldformat)
	{
		tcountv->set_value(vermap.size(), node_type::nd_number);
		tcountr->set_value(rootmap.size(), node_type::nd_number);
	}
	else
	{
		vermap["root"] = GUIDasMS(uuid_gen().data);
		tcountv->set_value(vermap.size(), node_type::nd_number);
	}

	// Запись root
	for( auto& pmap: rootmap )
	{
		trc->add_child(pmap.first, node_type::nd_string);
		trc->add_child(pmap.second, node_type::nd_guid);
	}
	String tree_text = outtext(tr);
	delete tr;
	in->SetSize(0);
	in->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().size());
	{
		TStreamWriter sw(in, TEncoding::UTF8, 1024);
		sw.Write(tree_text);
	}
	in->Seek(0, soFromBeginning);
	out = new TTempStream;
	if(oldformat)
	{
		out->CopyFrom(in, 0);
		metamap["root"] = out;
	}
	else
	{
		ZDeflateStream(in, out);
		extmap["root"] = out;
	}

	// Запись versions

	for( auto& pmap: vermap )
	{
		tvc->add_child(pmap.first, node_type::nd_string);
		tvc->add_child(pmap.second, node_type::nd_guid);
	}

	String tv_text = outtext(tv);
	delete tv;
	in->SetSize(0);
	in->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().size());
	{
		TStreamWriter sw(in, TEncoding::UTF8, 1024);
		sw.Write(tv_text);
	}
	out = new TTempStream;
	in->Seek(0, soFromBeginning);
	ZDeflateStream(in, out);
	extmap["versions"] = out;

	if(oldformat)
	{
		cath = cat->CreateCatalog("metadata", true);
		for( auto& psmap: metamap )
		{
			cath->createFile(psmap.first)->WriteAndClose(psmap.second);
			delete psmap.second;
		}
	}
	for( auto& psmap: extmap )
	{
		cat->createFile(psmap.first)->WriteAndClose(psmap.second);
		delete psmap.second;
	}

	delete in;

	delete cat;
	return true;
}

