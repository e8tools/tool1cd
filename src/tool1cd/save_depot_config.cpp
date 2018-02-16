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
#include "TableIterator.h"

using namespace std;

/* возвращает текст вида
{
{216,0}
}*/
String serialize_version(int configVerMajor, int configVerMinor)
{
	auto t = std::make_shared<tree>("", node_type::nd_list, nullptr);
	auto tc = new tree("", node_type::nd_list, t.get());
	tc = new tree("", node_type::nd_list, tc);
	tc->add_child(String(configVerMajor), node_type::nd_number);
	tc->add_child(String(configVerMinor), node_type::nd_number);
	return outtext(t.get());
}

bool try_store_blob_data(const TableRecord &record,
						 const Field *data_field,
						 bool inflate_stream,
						 const Field *hash_field,
						 PackDirectory &pack_directory,
						 TStream *&out);


bool contains_ic(const vector<string> &vector, const string &string_to_find)
{
	for (auto &value : vector) {
		if (CompareIC(value, string_to_find) == 0) {
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------
// Сохранение конфигурации в файл из хранилища конфигураций
// ver - номер версии сохраняемой конфигурации
// ver > 0 - используется переданный номер версии
// ver <= 0 - номер версии от последней конфигурации. 0 - последняя конфигурация, -1 - предпоследняя и т.д., т.е. Номер версии определяется как номер последней + ver
bool T_1CD::save_depot_config(const string &_filename, int32_t ver)
{
	Field* fldd_rootobjid;

	Field* fldh_datahash;
	BinaryGuid rootobj;

	Field* flde_datahash;
	vector<TableRecord*> reces;
	vector<string> extnames;

	depot_ver depotVer;
	uint32_t configVerMajor, configVerMinor;
	PackDirectory pack_directory;
	V8Catalog* cat;
	V8Catalog* cath;
	bool oldformat;
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

	TableIterator depot_iterator(table_depot);
	if (depot_iterator.eof()) {
		throw DetailedException("Не удалось прочитать запись в таблице DEPOT.");
	}

	depotVer = get_depot_version(depot_iterator.current());
	rootobj = depot_iterator.current().get<BinaryGuid>(fldd_rootobjid);

	// "Нормализуем" версию конфигурации
	ver = get_ver_depot_config(ver);

	// Ищем строку с номером версии конфигурации

	Field *fldv_vernum = table_versions->get_field("VERNUM");

	TableIterator versions_iterator(table_versions);
	while (!versions_iterator.eof()) {
		int32_t vernum = ToIntDef(versions_iterator.current().get_string(fldv_vernum), 0);
		if (vernum == ver) {
			break;
		}
		versions_iterator.next();
	}

	if (versions_iterator.eof()) {
		throw DetailedException("В хранилище не найдена версия конфигурации")
				.add_detail("Требуемая версия", ver);
	}

	boost::filesystem::path filepath = boost::filesystem::path(_filename);
	boost::filesystem::path root_path(filename); // путь к 1cd


	if (try_save_snapshot(versions_iterator.current(), ver, rootobj, root_path, filepath)) {
		return true;
	}

	auto vrec = versions_iterator.current();

	// Определяем версию структуры конфигурации (для файла version)
	if(depotVer >= depot_ver::Ver5)
	{
		const char *frec = vrec.get_raw("CVERSION");
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

	if (depotVer >= depot_ver::Ver6)
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

	Index *index_history = table_history->get_index("PK");

	TableRecord *rech1 = nullptr;
	TableRecord *rech2 = nullptr;
	reces.resize(0);

	auto HistoryIndex_numrec = index_history->get_numrecords();
	IndexedTableIterator externals_iterator(table_externals, "PK");

	if (boost::filesystem::exists(filepath)) {
		boost::filesystem::remove(filepath);
	}
	cat = new V8Catalog(filepath.string(), false);

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

	{
		TStream *in = new TMemoryStream;
		in->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().size());
		in->WriteString(serialize_version(configVerMajor, configVerMinor));
		TStream *out = new TMemoryStream;
		in->Seek(0, soFromBeginning);
		ZDeflateStream(in, out);
		delete in;
		extmap["version"] = out;
	}
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

	int lastver = -1;
	bool lastremoved = true;

	BinaryGuid curobj;
	for (uint32_t iHistory_Index = 0; iHistory_Index <= HistoryIndex_numrec; iHistory_Index++)
	{
		if(iHistory_Index < HistoryIndex_numrec)
		{
			uint32_t num_rec = index_history->get_numrec(iHistory_Index);
			rech2 = table_history->getrecord(num_rec);
		}

		if (rech2->get<BinaryGuid>(fldh_objid) != curobj || iHistory_Index == HistoryIndex_numrec) {
			// это новый объект или конец таблицы
			if(!lastremoved)
			{
				TStream *out;

				string sObjId = rech1->get<BinaryGuid>(fldh_objid).as_MS();
				if (!try_store_blob_data(*rech1, fldh_objdata, oldformat, fldh_datahash, pack_directory, out)) {
					msreg_m.AddMessage_("Ошибка чтения объекта конфигурации", MessageState::Error,
						"Таблица", "HISTORY",
						"Объект", sObjId,
						"Версия", rech1->get_string(fldh_vernum));
				}
				else
				{
					if(oldformat)
					{
						rootmap[sObjId] = rech1->get<BinaryGuid>(fldh_objverid).as_MS();
						metamap[sObjId] = out;
					}
					else
					{
						vermap[sObjId] = rech1->get<BinaryGuid>(fldh_objverid).as_MS();
						extmap[sObjId] = out;
					}

					// Вот тут идем по EXTERNALS
					while (!externals_iterator.eof()) {
						const auto &rece = externals_iterator.current();
						BinaryGuid current_external_guid = rece.get<BinaryGuid>(flde_objid);
						if (current_external_guid > curobj) {
							break;
						}

						if (current_external_guid == curobj) {
							int32_t vernum = ToIntDef(rece.get_string(flde_vernum), numeric_limits<int32_t>::max());
							string ext_name = rece.get_string(flde_extname);
							if (vernum <= ver && rece.get<bool>(flde_datapacked)) {
								size_t j;
								bool found = false;
								for (j = 0; j < reces.size(); j++) {
									if (CompareIC(ext_name, reces[j]->get_string(flde_extname)) == 0) {
										reces[j] = new TableRecord(rece);
										found = true;
										break;
									}
								}
								if (!found) {
									reces.push_back(new TableRecord(rece));
								}
							}
							if (vernum == lastver) {
								extnames.push_back(ext_name);
							}
						}
						externals_iterator.next();
					}
					for (auto rec : reces) {
						String ext_name = rec->get_string(flde_extname);
						if (!contains_ic(extnames, ext_name)) {
							continue;
						}

						TStream *out;
						if (!try_store_blob_data(*rec, flde_extdata, false, flde_datahash, pack_directory, out)) {
							msreg_m.AddMessage_("Ошибка чтения объекта конфигурации", MessageState::Error,
								"Таблица", "EXTERNALS",
								"Объект", ext_name,
								"Версия", rec->get_string(flde_vernum));
						}
						else
						{
							vermap[ext_name] = rec->get<BinaryGuid>(flde_extverid).as_MS();
							extmap[ext_name] = out;
						}

					}
					reces.clear();
					extnames.clear();
				}
			}

			curobj = rech2->get<BinaryGuid>(fldh_objid);
			lastremoved = true;
		}

		if(iHistory_Index < HistoryIndex_numrec)
		{
			int32_t vernum = ToIntDef(rech2->get_string(fldh_vernum), numeric_limits<int32_t>::max());
			if(vernum <= ver)
			{
				if (rech2->get<bool>(fldh_removed)) {
					lastremoved = true;
				}
				else
				{
					bool datapacked = false;
					if (!rech2->is_null_value(fldh_datapacked)) {
						if (rech2->get<bool>(fldh_datapacked)) {
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
	{
		TStream *in = new TMemoryStream;
		in->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().size());
		in->WriteString(tree_text);
		in->Seek(0, soFromBeginning);
		TStream *out = new TTempStream;
		if (oldformat) {
			out->CopyFrom(in, 0);
			metamap["root"] = out;
		} else {
			ZDeflateStream(in, out);
			extmap["root"] = out;
		}
		delete in;
	}
	// Запись versions

	for( auto& pmap: vermap )
	{
		tvc->add_child(pmap.first, node_type::nd_string);
		tvc->add_child(pmap.second, node_type::nd_guid);
	}

	String tv_text = outtext(tv);
	delete tv;
	{
		TStream *in = new TMemoryStream;
		in->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().size());
		{
			TStreamWriter sw(in, TEncoding::UTF8, 1024);
			sw.Write(tv_text);
		}
		TStream *out = new TTempStream;
		in->Seek(0, soFromBeginning);
		ZDeflateStream(in, out);
		extmap["versions"] = out;
	}

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

	delete cat;
	return true;
}


bool T_1CD::try_save_snapshot(const TableRecord &version_record,
							  int ver,
							  const BinaryGuid &rootobj,
							  const boost::filesystem::path &root_path,
							  const boost::filesystem::path &target_file_path) const
{
	if (version_record.get<BinaryGuid>("SNAPSHOTMAKER").is_empty()) {
		return false;
	}
	string ver_part  = string("00000").append(to_string(ver));
	string name_snap = string("ddb").append(ver_part.substr(ver_part.size() - 5, 5)).append(".snp");

	boost::filesystem::path file_snap = root_path.parent_path() / "cache" / name_snap;

	msreg_m.AddMessage_("Попытка открытия файла снэпшота", MessageState::Info,
						"Файл", file_snap.string());

	if (!boost::filesystem::exists(file_snap)){
		msreg_m.AddMessage_("Не найден файл снэпшота", MessageState::Warning,
							"Имя файла", file_snap.string(),
							"Требуемая версия", ver);
		return false;
	}

	std::shared_ptr<TStream> in;
	try {
		in.reset(new TFileStream(file_snap, fmOpenRead | fmShareDenyNone));
	}
	catch (...) {
		msreg_m.AddMessage_("Не удалось открыть файл снэпшота", MessageState::Warning,
							"Имя файла", file_snap.string(),
							"Требуемая версия", ver);
		return false;
	}

	std::shared_ptr<TStream>out;
	try
	{
		out.reset(new TFileStream(target_file_path, fmCreate | fmShareDenyWrite));
	}
	catch(...) {
		throw DetailedException("Не удалось создать файл конфигурации")
				.add_detail("Имя файла", target_file_path.string());
		return false;
	}

	snapshot_version snap_ver = snapshot_version::Ver1;
	BinaryGuid snapshot_maker = version_record.get<BinaryGuid>("SNAPSHOTMAKER");

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
				ZInflateStream(in.get(), out.get());
			}
			catch(...) {
				msreg_m.AddMessage_("Не удалось распаковать файл снэпшота", MessageState::Warning,
									"Имя файла", file_snap.string(),
									"Требуемая версия", ver);
				return false;
			}
			break;
		}
		case snapshot_version::Ver2:
		{
			out->CopyFrom(in.get(), 0);
			break;
		}
		default:
		{
			msreg_m.AddMessage_("Неизвестная версия снэпшота", MessageState::Warning,
								"Имя файла", file_snap.string());
			return false;
		}
	};

	uint32_t snapshot_crc = version_record.get<uint32_t>("SNAPSHOTCRC");
	uint32_t calc_crc = _crc32(out.get());
	if (calc_crc == snapshot_crc) {
		return true;
	}
	msreg_m.AddMessage_("Файл снэпшота испорчен (не совпала контрольная сумма)", MessageState::Warning,
						"Имя файла", file_snap.string(),
						"Требуемая версия", ver,
						"Должен быть CRC32", to_hex_string(snapshot_crc),
						"Получился CRC32", to_hex_string(calc_crc));
	return false;
}

bool try_store_blob_data(const TableRecord &record,
						 const Field *data_field,
						 bool inflate_stream,
						 const Field *hash_field,
						 PackDirectory &pack_directory,
						 TStream *&out)
{
	if (record.try_store_blob_data(data_field, out, inflate_stream)) {
		return true;
	}

	if (!hash_field) {
		return false;
	}

	bool found = false;
	out = pack_directory.get_data(record.get_string(hash_field), found);
	return found;
}
