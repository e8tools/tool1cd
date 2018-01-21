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
	Field* fldd_rootobjid;

	Field* fldv_snapshotcrc;
	Field* fldv_snapshotmaker;

	Field* fldh_datahash;
	Index* index_history;

	BinaryGuid rootobj;
	BinaryGuid curobj;
	uint32_t ih, nh;

	Field* flde_datahash;
	Index* index_externals;
	vector<TableRecord*> reces;
	vector<String> extnames;
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
		char b[2];
		unsigned short s;
	} cv;

	try {

		assert_i_am_a_repository();

	} catch (DetailedException &exc) {
		exc.show();
		return false;
	}

	// Получаем версию хранилища
	fldd_rootobjid = table_depot->get_field("ROOTOBJID");

	TableRecord *rec = nullptr;
	for(uint32_t i = 0; i < table_depot->get_phys_numrecords(); i++)
	{
		rec = table_depot->getrecord(i);
		if (!rec->is_removed()) {
			break;
		}
		delete rec; // TODO: придумать схему без постоянной перегонки памяти
		rec = nullptr;
	}

	if (!rec) {
		throw DetailedException("Не удалось прочитать запись в таблице DEPOT.");
	}

	depotVer = get_depot_version(rec);

	rootobj = rec->get_guid(fldd_rootobjid);
	delete rec;

	// "Нормализуем" версию конфигурации
	ver = get_ver_depot_config(ver);

	// Ищем строку с номером версии конфигурации

	Field *fldv_vernum = table_versions->get_field("VERNUM");
	fldv_snapshotcrc = table_versions->get_field("SNAPSHOTCRC");
	fldv_snapshotmaker = table_versions->get_field("SNAPSHOTMAKER");

	rec = nullptr;
	ok = false;
	for(uint32_t i = 0; i < table_versions->get_phys_numrecords(); i++)
	{
		rec = table_versions->getrecord(i);
		if (rec->is_removed()) {
			delete rec; // TODO: вариант без перегонок памяти
			continue;
		}
		int32_t vernum = rec->get_string(fldv_vernum).ToIntDef(0);
		if (vernum == ver) {
			break;
		}
		delete rec; // TODO: вариант без перегонок памяти
		rec = nullptr;
	}

	if (!rec) {
		throw DetailedException("В хранилище не найдена версия конфигурации")
				.add_detail("Требуемая версия", ver);
	}

	boost::filesystem::path filepath = boost::filesystem::path(static_cast<std::string>(_filename));
	boost::filesystem::path root_path(static_cast<std::string>(filename)); // путь к 1cd

	// Проверяем, нет ли снэпшота нужной версии конфигурации
	if (!rec->get_guid(fldv_snapshotmaker).is_empty()) {
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
				throw DetailedException("Не удалось создать файл конфигурации")
						.add_detail("Имя файла", _filename);
			}
			if(in) {
				snapshot_version snap_ver = snapshot_version::Ver1;
				BinaryGuid snapshot_maker = rec->get_guid(fldv_snapshotmaker);

				if (rootobj == snapshot_maker || snapshot_maker == SNAPSHOT_VER1) {
					snap_ver = snapshot_version::Ver1;
				}
				else if (snapshot_maker == SNAPSHOT_VER2) {
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
		const char *frec = rec->get_raw("CVERSION");
		cv.b[0] = frec[1];
		cv.b[1] = frec[0];
		configVerMajor = cv.s;
		frec += 2;
		cv.b[0] = frec[1];
		cv.b[1] = frec[0];
		configVerMinor = cv.s;
	}
	else
	{
		configVerMinor = 0;
		if(version == db_ver::ver8_0_3_0 || version == db_ver::ver8_0_5_0) configVerMajor = 6;
		else if(version == db_ver::ver8_1_0_0) configVerMajor = 106;
		else configVerMajor = 216;
	}

	delete rec;

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

	TableRecord *rech1 = nullptr;
	TableRecord *rech2 = nullptr;
	TableRecord *rece = nullptr;
	reces.resize(0);

	nh = index_history->get_numrecords();
	ne = index_externals->get_numrecords();
	curobj = BinaryGuid();

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
		trc->add_child(rootobj.as_MS(), node_type::nd_guid);
		tcountr = trc->add_child("0", node_type::nd_number); // узел, содержащий счетчик в файле root
		oldformat = true;
	}
	else
	{
		trc->add_child("2", node_type::nd_number);
		trc->add_child(rootobj.as_MS(), node_type::nd_guid);
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
			rech2 = table_history->getrecord(num_rec);
		}

		if (rech2->get_guid(fldh_objid) != curobj || ih == nh) {
			// это новый объект или конец таблицы
			if(!lastremoved)
			{
				ok = false;
				deletesobj = false;
				auto *b = (const BlobPointer *)rech1->get_data(fldh_objdata);
				if (!rech1->is_null_value(fldh_objdata) && (b->start != 0 || b->length != 0)) {
					out = new TTempStream;
					if(oldformat)
					{
						table_history->readBlob(in, b->start, b->length);
						in->Seek(0, soFromBeginning);
						ZInflateStream(in, out);
					}
					else table_history->readBlob(out, b->start, b->length);
					out->Close();
					ok = true;
				}
				else if(depotVer >= depot_ver::Ver6)
				{
					const char* hash_data = rech1->get_raw(fldh_datahash);
					out = pack_directory.get_data(hash_data, ok);

					if(!ok)
					{
						String ss = rech1->get_string(fldh_datahash);
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
									"Объект", rech1->get_string(fldd_rootobjid),
									"Версия", rech1->get_string(fldh_vernum));
							}
						}
						else
						{
							msreg_m.AddMessage_("Не найден файл", MessageState::Error,
								"Файл", current_object_path.string(),
								"Таблица", "HISTORY",
								"Объект", rech1->get_string(fldd_rootobjid),
								"Версия", rech1->get_string(fldh_vernum));
						}
					}
				}
				String s = rech1->get_string(fldh_objid);
				if(!ok)
				{
					msreg_m.AddMessage_("Ошибка чтения объекта конфигурации", MessageState::Error,
						"Таблица", "HISTORY",
						"Объект", s,
						"Версия", rech1->get_string(fldh_vernum));
				}
				else
				{
					if(oldformat)
					{
						rootmap[s] = rech1->get_guid(fldh_objverid).as_MS();
						metamap[s] = out;
					}
					else
					{
						vermap[s] = rech1->get_guid(fldh_objverid).as_MS();
						extmap[s] = out;
					}

					// Вот тут идем по EXTERNALS
					while(true) {
						if(ie > ne) break;
						BinaryGuid current_external_guid = rece->get_guid(flde_objid);
						if (current_external_guid > curobj) {
							break;
						}
						if (current_external_guid != curobj) {
							int32_t vernum = rece->get_string(flde_vernum).ToIntDef(std::numeric_limits<int32_t>::max());
							String s = rece->get_string(flde_extname);
							if(vernum <= ver && rece->get_bool(flde_datapacked)) {
								int32_t j;
								bool found = false;
								for (j = 0; j < reces.size(); j++) {
									if (s.CompareIC(reces[j]->get_string(flde_extname)) == 0) {
										reces[j] = rece;
										found = true;
										break;
									}
								}
								if (!found){
									reces.push_back(rece);
								}
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
						table_externals->getrecord(num_rec);
					}
					for(int32_t j = 0; j < reces.size(); j++)
					{
						rec = reces[j];
						String ext_name = rec->get_string(flde_extname);
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
						const BlobPointer *bp = (const BlobPointer *)rec->get_data(flde_extdata);
						if (bp->length != 0 || bp->start != 0) {
							out = new TTempStream;
							table_externals->readBlob(out, bp->start, bp->length);
							out->Close();
							ok = true;
						}
						else if(depotVer >= depot_ver::Ver6)
						{
							const char *hashdata = rec->get_data(flde_datahash);
							out = pack_directory.get_data(hashdata, ok);

							if(!ok)
							{
								String ss = rec->get_string(flde_datahash);
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
											"Объект", rec->get_string(flde_extname),
											"Версия", rec->get_string(flde_vernum));
									}
								}
								else
								{
									msreg_m.AddMessage_("Не найден файл", MessageState::Error,
										"Файл", current_object_path.string(),
										"Таблица", "EXTERNALS",
										"Объект", rec->get_string(flde_extname),
										"Версия", rec->get_string(flde_vernum));
								}
							}
						}
						if(!ok)
						{
							msreg_m.AddMessage_("Ошибка чтения объекта конфигурации", MessageState::Error,
								"Таблица", "EXTERNALS",
								"Объект", ext_name,
								"Версия", rec->get_string(flde_vernum));
						}
						else
						{
							vermap[ext_name] = rec->get_guid(flde_extverid).as_MS();
							extmap[ext_name] = out;
						}

					}
					reces.clear();
					extnames.clear();
				}
			}

			curobj = rech2->get_guid(fldh_objid);
			lastremoved = true;
		}

		if(ih < nh)
		{
			int32_t vernum = rech2->get_string(fldh_vernum).ToIntDef(std::numeric_limits<int32_t>::max());
			if(vernum <= ver)
			{
				removed = rech2->get_bool(fldh_removed);
				if(removed)
				{
					lastremoved = true;
				}
				else
				{
					datapacked = false;
					if (!rech2->is_null_value(fldh_datapacked)) {
						if (!rech2->get_bool(fldh_datapacked)) {
							datapacked = true;
						}
					}
					if(datapacked)
					{
						rech1 = rech2;
						lastremoved = false;
						lastver = vernum;
					}
				}
			}
		}
	}

	// TODO: Полечить дичайшие утечки памяти
	for(size_t j = 0; j < reces.size(); j++) {
		delete reces[j];
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

