#include <memory>

#include "ConfigStorage.h"
#include "Common.h"
#include "Base64.h"
#include "TempStream.h"
#include "cfapi/V8File.h"

#include "UZLib.h"
#pragma comment (lib, "zlibstatic.lib")

using namespace std;

const string spoint = ".";
const string sdynupdate = "_dynupdate_";
const int lsdynupdate = sdynupdate.size();

extern Registrator msreg_g;

std::vector<BinaryGuid> parse_dynamically_updated_tree(unique_ptr<Tree> tt);

//********************************************************
// Класс ConfigStorageDirectory

//---------------------------------------------------------------------------
ConfigStorageDirectory::ConfigStorageDirectory(const std::string &_dir)
	: fdir(_dir)
{
}

//---------------------------------------------------------------------------
ConfigFile* ConfigStorageDirectory::readfile(const std::string &path)
{
	ConfigFile* cf;

	boost::filesystem::path filename = fdir / path;

	if (boost::filesystem::exists(filename)) {
		cf = new ConfigFile;
		try
		{
			cf->str = new TFileStream(filename, fmOpenRead);
			cf->addin = nullptr;
			return cf;
		}
		catch(...)
		{
			// TODO: сообщить об ошибке и записать в log
			delete cf;
			return nullptr;
		}

	}

	return nullptr;
}

//---------------------------------------------------------------------------
bool ConfigStorageDirectory::writefile(const std::string &path, TStream *str)
{
	boost::filesystem::path filename = fdir / path;
	TFileStream f(filename, fmCreate);
	f.CopyFrom(str, 0);

	return true;
}

//---------------------------------------------------------------------------
std::string ConfigStorageDirectory::presentation() const
{
	return fdir.string();
}

//---------------------------------------------------------------------------
bool ConfigStorageDirectory::fileexists(const std::string &path) const
{
	return boost::filesystem::exists(fdir / path);
}


//********************************************************
// Класс ConfigStorageCFFile

//---------------------------------------------------------------------------
ConfigStorageCFFile::ConfigStorageCFFile(const std::string &fname)
{
	filename = fname;
	cat = new V8Catalog(filename);
}

//---------------------------------------------------------------------------
ConfigStorageCFFile::~ConfigStorageCFFile()
{
	delete cat;
}

//---------------------------------------------------------------------------
ConfigFile* ConfigStorageCFFile::readfile(const std::string &path)
{
	ConfigFile* cf;

	if(!cat->isOpen()) return nullptr;

	string fname = StringReplace(path, "/", "\\", rfReplaceAll);
	V8Catalog* c = cat;
	for (auto i = fname.find("\\"); i != string::npos; i = fname.find("\\"))
	{
		V8File *f = c->get_file(fname.substr(0, i - 1));
		if(!f) {
			return nullptr;
		}
		c = f->get_catalog();
		if(!c) {
			return nullptr;
		}
		fname = fname.substr(i + 1, fname.size() - i - 1);
	}
	V8File *f = c->get_file(fname);
	if (!f) {
		return nullptr;
	}
	if (!f->open()) {
		return nullptr;
	}
	cf = new ConfigFile;
	cf->str = f->get_stream();
	cf->str->Seek(0l, soBeginning);
	cf->addin = f;
	return cf;

}

//---------------------------------------------------------------------------
bool ConfigStorageCFFile::writefile(const std::string &path, TStream *str)
{
	V8Catalog* c;
	V8File* f;
	int i;

	if(!cat->isOpen()) return false;

	string fname = StringReplace(path, "/", "\\", rfReplaceAll);
	c = cat;
	for (auto i = fname.find("\\"); i != string::npos; i = fname.find("\\"))
	{
		c = c->CreateCatalog(fname.substr(0, i - 1));
		fname = fname.substr(i + 1, fname.size() - i - 1);
	}
	f = c->createFile(fname);
	f->write(str);
	return true;
}

//---------------------------------------------------------------------------
std::string ConfigStorageCFFile::presentation() const
{
	return filename;
}

//---------------------------------------------------------------------------
void ConfigStorageCFFile::close(ConfigFile* cf)
{
	V8File* f;

	f = (V8File*)cf->addin;
	f->close();
	delete cf;
}

//---------------------------------------------------------------------------
bool ConfigStorageCFFile::fileexists(const std::string &path) const
{
	// По сути, проверяется существование только каталога (файла верхнего уровня)
	// Это неправильно для формата 8.0 с файлом каталогом metadata. Но метод fileexists используется только для внешних файлов,
	// поэтому такой проверки достаточно

	if (!cat->isOpen()) {
		return false;
	}

	string fname = StringReplace(path, "/", "\\", rfReplaceAll);
	auto i = fname.find("\\");
	if (i != string::npos) {
		fname = fname.substr(0, i - 1);
	}
	V8File *f = cat->get_file(fname);
	return f != nullptr;
}


//********************************************************
// Класс ContainerFile

//---------------------------------------------------------------------------
ContainerFile::ContainerFile(TableFile *_f, const std::string &_name)
{


	file    = _f;
	name    = _name;
	stream  = nullptr;
	rstream = nullptr;
	cat     = nullptr;
	packed  = table_file_packed::unknown;
	dynno   = -3;
}

//---------------------------------------------------------------------------
ContainerFile::~ContainerFile()
{
	close();
}

//---------------------------------------------------------------------------
bool ContainerFile::open()
{
	TStream* ts;

	unsigned int i;
	Table* t;
	table_blob_file* addr;
	unsigned int maxpartno;

	if(stream)
	{
		stream->Seek(0l, soBeginning);
		return true;
	}
	t = file->t;
	addr = file->addr;
	maxpartno = file->maxpartno;

	if(maxpartno > 0) stream = new TTempStream;
	else stream = new TMemoryStream;

	if (packed == table_file_packed::unknown) {
		packed = isPacked() ? table_file_packed::yes : table_file_packed::no;
	}

	if(rstream)
	{
		if(packed == table_file_packed::yes) ts = rstream;
		else
		{
			stream = rstream;
			stream->Seek(0l, soBeginning);
			return true;
		}
	}
	else
	{
		if(packed == table_file_packed::yes)
		{
			if(maxpartno > 0) ts = new TTempStream;
			else ts = new TMemoryStream;
		}
		else ts = stream;

		for (int i = 0; i <= maxpartno; ++i) {
			t->readBlob(ts, addr[i].blob_start, addr[i].blob_length, false);
		}
	}

	if(packed == table_file_packed::yes)
	{
		ts->Seek(0l, soBeginning);
		ZInflateStream(ts, stream);
		if(!rstream)
		{
			delete ts;
		}
	}

	stream->Seek(0l, soBeginning);
	return true;

}

//---------------------------------------------------------------------------
bool ContainerFile::ropen()
{

	unsigned int i;
	Table* t;
	table_blob_file* addr;
	unsigned int maxpartno;

	if(rstream)
	{
		rstream->Seek(0l, soBeginning);
		return true;
	}
	t = file->t;
	addr = file->addr;
	maxpartno = file->maxpartno;
	if(packed == table_file_packed::unknown) packed = isPacked() ? table_file_packed::yes : table_file_packed::no;
	if(packed == table_file_packed::no && stream)
	{
		rstream = stream;
		rstream->Seek(0l, soBeginning);
		return true;
	}

	if(maxpartno > 0) rstream = new TTempStream;
	else rstream = new TMemoryStream;

	for(i = 0; i <= maxpartno; ++i) t->readBlob(rstream, addr[i].blob_start, addr[i].blob_length, false);

	rstream->Seek(0l, soBeginning);
	return true;

}

//---------------------------------------------------------------------------
void ContainerFile::close()
{
	delete cat;
	cat = nullptr;
	if(stream != rstream)
	{
		delete stream;
		delete rstream;
	}
	else delete stream;
	stream = nullptr;
	rstream = nullptr;
}

//---------------------------------------------------------------------------
bool ContainerFile::isPacked() const
{

	if (EqualIC(name, "DynamicallyUpdated")
	 || EqualIC(name, "SprScndInfo")
	 || EqualIC(name, "DBNamesVersion")
	 || EqualIC(name, "siVersions")
	 || name.front() == '{'
	 || LowerCase(name).find("fo_version") != std::string::npos) {
		return false;
	}
	auto i = name.rfind(".");
	if (i == std::string::npos) {
		return true;
	}
	std::string ext = name.substr(i + 1, name.size() - i - 1);
	if (EqualIC(ext, "INF")) {
		return false;
	}
	if (EqualIC(ext, "PF")) {
		return false;
	}
	return true;
}


//********************************************************
// Структура дополнительных данных открытого файла класса ConfigStorageTable

//---------------------------------------------------------------------------
enum class ConfigStorageTableAddinVariant
{
	ContainerFile,
	V8File
};

//---------------------------------------------------------------------------
struct ConfigStorageTable_addin
{
	ConfigStorageTableAddinVariant variant;
	union
	{
		ContainerFile* tf;
		V8File* f;
	};
};

//********************************************************
// Класс ConfigStorageTable

//---------------------------------------------------------------------------
ConfigStorageTable::~ConfigStorageTable()
{
	for (auto &pfile: files) {
		delete pfile.second;
	}

	delete base;
}

//---------------------------------------------------------------------------
ConfigFile* ConfigStorageTable::readfile(const std::string &path)
{
	V8Catalog* c;
	ConfigFile* cf;
	ConfigStorageTable_addin* cfa;

	if(!ready) return nullptr;

	string fname = StringReplace(path, "/", "\\", rfReplaceAll);
	string top_level_name;
	{
		auto i = fname.find("\\");
		if (i != string::npos) {
			top_level_name = fname.substr(0, i - 1);
			fname = fname.substr(i + 1, fname.size() - i - 1);
		} else {
			top_level_name = fname;
			fname = "";
		}
	}

	auto pfiles = files.find(LowerCase(top_level_name));
	if (pfiles == files.end()) {
		return nullptr;
	}
	ContainerFile *top_file = pfiles->second;

	top_file->open();
	if (!fname.empty()) {
		if (!top_file->cat) {
			top_file->cat = new V8Catalog(top_file->stream, false, true);
		}
		V8Catalog *parent_cat = top_file->cat;
		for (auto i = fname.find("\\"); i != string::npos; i = fname.find("\\")) {
			V8File *f = parent_cat->get_file(fname.substr(0, i - 1));
			if (!f) {
				// TODO: бросить исключение???
				return nullptr;
			}
			parent_cat = f->get_catalog();
			if (!parent_cat) {
				// TODO: бросить исключение???
				return nullptr;
			}
			fname = fname.substr(i + 1, fname.size() - i - 1);
		}
		V8File *f = parent_cat->get_file(fname);
		if (!f) {
			// TODO: бросить исключение???
			return nullptr;
		}
		if (!f->open()) {
			// TODO: бросить исключение???
			return nullptr;
		}

		cf = new ConfigFile;
		cfa = new ConfigStorageTable_addin;
		cfa->variant = ConfigStorageTableAddinVariant::V8File;
		cfa->f = f;
		cf->str = f->get_stream();
		cf->str->Seek(0l, soBeginning);
		cf->addin = cfa;
	}
	else
	{
		cf = new ConfigFile;
		cfa = new ConfigStorageTable_addin;
		cfa->variant = ConfigStorageTableAddinVariant::ContainerFile;
		cfa->tf = top_file;
		cf->str = top_file->stream;
		cf->str->Seek(0l, soBeginning);
		cf->addin = cfa;
	}

	return cf;

}

//---------------------------------------------------------------------------
bool ConfigStorageTable::writefile(const std::string &path, TStream *str)
{
	return false; // TODO Запись в таблицы пока не поддерживается
}

//---------------------------------------------------------------------------
void ConfigStorageTable::close(ConfigFile* cf)
{
	ConfigStorageTable_addin* cfa;

	cfa = (ConfigStorageTable_addin*)cf->addin;
	if(cfa->variant == ConfigStorageTableAddinVariant::ContainerFile)
	{
		cfa->tf->close();
	}
	else if(cfa->variant == ConfigStorageTableAddinVariant::V8File)
	{
		cfa->f->close();
	}
	delete cfa;
	delete cf;
}

//---------------------------------------------------------------------------
bool ConfigStorageTable::save_config(const boost::filesystem::path &file_name)
{
	if(!ready) {
		return false;
	}

	if(boost::filesystem::exists(file_name)) {
		boost::filesystem::remove_all(file_name);
	}

	size_t prevj = 101;
	size_t i = 1;

	std::unique_ptr<V8Catalog> catalog (new V8Catalog(file_name.string(), false));
	for(auto pfiles: files)
	{
		++i;
		size_t j = i * 100 / files.size();
		if(j != prevj)
		{
			prevj = j;
			msreg_g.Status(to_string(j) + string("%"));
		}

		ContainerFile* top_file = pfiles.second;
		if(top_file->ropen())
		{
			V8File* file = catalog->createFile(top_file->name);
			file->set_time_create(top_file->file->ft_create);
			file->set_time_modify(top_file->file->ft_modify);
			file->write_and_close(top_file->rstream);

			top_file->close();
		}
	}

	msreg_g.Status("");
	return true;
}

//---------------------------------------------------------------------------
bool ConfigStorageTable::fileexists(const std::string &path) const
{
	// По сути, проверяется существование только каталога (файла записи верхнего уровня)
	// Это неправильно для формата 8.0 с файлом каталогом metadata. Но метод fileexists используется только для внешних файлов, поэтому такой проверки достаточно

	if(!ready) return false;

	string fname = StringReplace(path, "/", "\\", rfReplaceAll);
	auto i = fname.find("\\");
	if (i != string::npos) {
		fname = fname.substr(0, i - 1);
	}

	return files.find(LowerCase(fname)) != files.end();
}

//********************************************************
// Класс ConfigStorageTableConfig

//---------------------------------------------------------------------------
ConfigStorageTableConfig::ConfigStorageTableConfig(TableFiles* tabf, T_1CD* _base) : ConfigStorageTable(_base)
{
	string name, ext;
	TableFile* tf;
	TableFile* _DynamicallyUpdated;
	ContainerFile* DynamicallyUpdated;
	Tree* tt;
	Tree* ct;
	std::vector<BinaryGuid> dynup;
	BinaryGuid g;
	Table* tab;
	int dynno;
	ContainerFile* pcf;

	ready = tabf->get_ready();
	if(!ready) return;

	present = tabf->get_table()->get_base()->get_filename() + "\\config";

	tab = tabf->get_table();
	_DynamicallyUpdated = tabf->get_file("DynamicallyUpdated");

	if(_DynamicallyUpdated)
	{
		ContainerFile DynamicallyUpdated(_DynamicallyUpdated, _DynamicallyUpdated->name);
		DynamicallyUpdated.open();
		string detail_path_name = tab->get_base()->get_filename()
								  + "\\" + tab->get_name()
								  + "\\" + DynamicallyUpdated.name;
		auto tt = parse_1Cstream(DynamicallyUpdated.stream, detail_path_name);
		try {
			dynup = parse_dynamically_updated_tree(std::move(tt));
		} catch (DetailedException &ex) {
			ex.add_detail("Путь", detail_path_name);
			throw ex;
		}
	}

	for (auto &pfilesmap : tabf->files()) {
		TableFile *tf = pfilesmap.second;
		if (tf == _DynamicallyUpdated) {
			continue;
		}
		if (tf->addr->blob_length == 0) {
			continue;
		}
		string file_name = tf->name;
		{
			auto m = file_name.rfind(spoint);
			if (m != string::npos) {
				name = file_name.substr(0, m);
				ext = file_name.substr(m + 1, file_name.size() - m - 1);
			} else {
				name = file_name;
				ext = "";
			}
		}

		if (EqualIC(ext, "new")) {
			ext = "";
			dynno = - 2;
		}
		else dynno = -1;

		{
			auto m = name.rfind(sdynupdate);
			if (m != string::npos) {
				string update_id = name.substr(m + lsdynupdate - 1, name.size() - m - lsdynupdate);
				name = name.substr(0, m);
				BinaryGuid update_guid = BinaryGuid(update_id);
				if (!dynup.empty()) {

					auto found = std::find(dynup.begin(), dynup.end(), update_guid);

					if (found == dynup.end()) {
						dynno = -2;
					} else {
						dynno = found - dynup.begin();
					}
				} else dynno = -2;
			}
		}

		if(!ext.empty()) {
			name += spoint;
			name += ext;
		}

		auto pfiles = files.find(LowerCase(name));
		if (pfiles == files.end())
		{
			auto pcf = new ContainerFile(tf, name);
			files[LowerCase(name)] = pcf;
			pcf->dynno = dynno;
		}
		else
		{
			auto pcf = pfiles->second;
			if(pcf->dynno < dynno)
			{
				pcf->file = tf;
				pcf->dynno = dynno;
			}
		}
	}

}

//---------------------------------------------------------------------------
std::string ConfigStorageTableConfig::presentation() const
{
	return present;
}

//********************************************************
// Класс ConfigStorageTableConfigSave

//---------------------------------------------------------------------------
ConfigStorageTableConfigSave::ConfigStorageTableConfigSave(TableFiles* tabc, TableFiles* tabcs, T_1CD* _base) : ConfigStorageTable(_base)
{
	int m;
	string name, ext;
	TableFile* tf;
	std::map<string,TableFile*>::iterator pfilesmap;
	std::map<string,ContainerFile*>::iterator pfiles;
	TableFile* _DynamicallyUpdated;
	TableFile* _deleted;
	Tree* tt;
	Tree* ct;
	BinaryGuid g;
	Table* tab;
	int dynno;
	ContainerFile* pcf;
	set<string> del;
	set<string>::iterator pdel;

	ready = tabc->get_ready();
	if(!ready) return;
	ready = tabcs->get_ready();
	if(!ready) return;

	present = tabc->get_table()->get_base()->get_filename() + "\\configsave";

	tab = tabcs->get_table();
	_deleted = tabcs->get_file("deleted");
	if(_deleted)
	{
		ContainerFile deleted(_deleted, _deleted->name);
		deleted.open();
		string detail_path_name = tab->get_base()->get_filename()
				   + "\\" + tab->get_name()
				   + "\\" + deleted.name;
		auto tt = parse_1Cstream(deleted.stream, detail_path_name);
		std::vector<BinaryGuid> dynup;
		try {
			dynup = parse_dynamically_updated_tree(std::move(tt));
		} catch (DetailedException &ex) {
			ex.add_detail("Путь", detail_path_name);
			throw ex;
		}
		for (auto &guid : dynup) {
			del.insert(LowerCase(guid.as_MS()));
		}
	}

	tab = tabc->get_table();
	_DynamicallyUpdated = tabc->get_file("DynamicallyUpdated");

	std::vector<BinaryGuid> dynup;
	if(_DynamicallyUpdated)
	{
		ContainerFile DynamicallyUpdated(_DynamicallyUpdated, _DynamicallyUpdated->name);
		DynamicallyUpdated.open();
		string detail_path_name = tab->get_base()->get_filename()
				+ "\\" + tab->get_name()
				+ "\\" + DynamicallyUpdated.name;
		auto tt = parse_1Cstream(DynamicallyUpdated.stream, detail_path_name);
		try {
			dynup = parse_dynamically_updated_tree(std::move(tt));
		} catch (DetailedException &ex) {
			ex.add_detail("Путь", detail_path_name);
			throw ex;
		}
	}

	m = dynup.size() + 2;

	for (auto &pfilesmap : tabcs->files()) {
		tf = pfilesmap.second;
		if(tf == _deleted) continue;
		if(tf->addr->blob_length == 0) continue;
		name = tf->name;
		auto pfiles = files.find(LowerCase(name));
		if (pfiles == files.end())
		{
			pcf = new ContainerFile(tf, name);
			files[LowerCase(name)] = pcf;
			pcf->dynno = m;
		}
		else
		{
			throw DetailedException("Ошибка чтения CONFIGSAVE. Повторяющееся имя файла")
				.add_detail("Имя файла", name);
		}
	}

	for (auto &pfilesmap : tabc->files()) {
		tf = pfilesmap.second;
		if(tf == _DynamicallyUpdated) continue;
		if(tf->addr->blob_length == 0) continue;
		string s = tf->name;

		pdel = del.find(LowerCase(s));
		if(pdel != del.end()) continue;

		m = s.rfind(spoint);
		if (m != string::npos) {
			name = s.substr(0, m - 1);
			ext = s.substr(m + 1, s.size() - m);
		}
		else
		{
			name = s;
			ext = "";
		}
		if (EqualIC(ext, "new")) {
			ext = "";
			dynno = dynup.size() + 1;
		}
		else dynno = -1;

		m = name.find(sdynupdate);
		if (m != string::npos) {
			string s = name.substr(m + lsdynupdate, name.size() - m - lsdynupdate + 1);
			name = name.substr(0, m - 1);
			string_to_GUID(s, &g);
			if (!dynup.empty())
			{
				for(m = 0; m < dynup.size(); ++m) if(g == dynup[m]) break;
				if(m >= dynup.size()) dynno = -2;
				else dynno = m;
			}
			else dynno = -2;
		}

		if (!ext.empty()) {
			name.append(spoint).append(ext);
		}

		auto pfiles = files.find(LowerCase(name));
		if(pfiles == files.end())
		{
			pcf = new ContainerFile(tf, name);
			files[LowerCase(name)] = pcf;
			pcf->dynno = dynno;
		}
		else
		{
			pcf = pfiles->second;
			if(pcf->dynno < dynno)
			{
				pcf->file = tf;
				pcf->dynno = dynno;
			}
		}
	}

}

//---------------------------------------------------------------------------
std::string ConfigStorageTableConfigSave::presentation() const
{
	return present;
}

//********************************************************
// Класс ConfigStorageTableConfigCas

//---------------------------------------------------------------------------
ConfigStorageTableConfigCas::ConfigStorageTableConfigCas(TableFiles *tabc, const std::string &configver, T_1CD *_base) : ConfigStorageTable(_base)
{
	int m;
	TableFile* _configinfo;
	ContainerFile* configinfo;
	Tree* ct;
	TableFile* tf;
	ContainerFile* pcf;
	TMemoryStream* stream;

	ready = tabc->get_ready();
	if(!ready) return;

	present = tabc->get_table()->get_base()->get_filename() + "\\configcas";
	string filepath = present + "\\" + configver;

	_configinfo = tabc->get_file(configver);
	if(!_configinfo)
	{
		throw DetailedException("Ошибка поиска файла")
			.add_detail("Путь", filepath)
			.add_detail("Имя", configver)
			.add_detail("Имя файла", "configinfo");
	}

	configinfo = new ContainerFile(_configinfo, "configinfo");
	files["configinfo"] = configinfo;
	configinfo->open();

	auto tt = parse_1Cstream(configinfo->stream, filepath);
	if(tt == nullptr)
	{
		throw DetailedException("Ошибка разбора файла configinfo")
			.add_detail("Путь", filepath);
	}
	ct = tt->get_first();
	if(!ct)
	{
		throw DetailedException("Ошибка разбора файла configinfo")
			.add_detail("Путь", filepath);
	}
	ct = ct->get_next();
	if(!ct)
	{
		throw DetailedException("Ошибка разбора файла configinfo")
			.add_detail("Путь", filepath);
	}
	ct = ct->get_next();
	if(!ct)
	{
		throw DetailedException("Ошибка разбора файла configinfo")
			.add_detail("Путь", filepath);
	}
	ct = ct->get_first();
	if(!ct)
	{
		throw DetailedException("Ошибка разбора файла configinfo")
			.add_detail("Путь", filepath);
	}

	stream = new TMemoryStream;
	for(m = ToIntDef(ct->get_value(), 0); m; --m)
	{
		ct = ct->get_next();
		if(!ct)
		{
			throw DetailedException("Ошибка разбора файла configinfo")
				.add_detail("Путь", filepath);
		}
		if(ct->get_type() != node_type::nd_string)
		{
			throw DetailedException("Ошибка разбора файла configinfo")
				.add_detail("Путь", filepath);
		}
		string name = ct->get_value();

		ct = ct->get_next();
		if(!ct)
		{
			throw DetailedException("Ошибка разбора файла configinfo")
				.add_detail("Путь", filepath);
		}
		if(ct->get_type() != node_type::nd_binary2)
		{
			throw DetailedException("Ошибка разбора файла configinfo")
				.add_detail("Путь", filepath);
		}
		stream->Seek(0l, soBeginning);
		base64_decode(ct->get_value(), stream);
		stream->Seek(0l, soBeginning);
		string hashname = hexstring(stream);

		tf = tabc->get_file(hashname);
		if(tf)
		{
			pcf = new ContainerFile(tf, name);
			files[LowerCase(name)] = pcf;
		}
		else
		{
			throw DetailedException("Ошибка поиска файла")
				.add_detail("Путь", filepath)
				.add_detail("Имя", hashname)
				.add_detail("Имя файла", name);
		}
	}

	delete stream;

}

//---------------------------------------------------------------------------
std::string ConfigStorageTableConfigCas::presentation() const
{
	return present;
}

//********************************************************
// Класс ConfigStorageTableConfigCasSave

//---------------------------------------------------------------------------
ConfigStorageTableConfigCasSave::ConfigStorageTableConfigCasSave(TableFiles *tabc, TableFiles *tabcs,
																 const BinaryGuid &uid, const std::string &configver,
																 T_1CD *_base) : ConfigStorageTable(_base)
{
	int m;
	TableFile* _configinfo;
	ContainerFile* configinfo;
	Tree* ct;
	TMemoryStream* stream;
	std::map<string,TableFile*>::iterator ptf;


	ready = tabc->get_ready();
	if(!ready) return;
	ready = tabcs->get_ready();
	if(!ready) return;

	configinfo = nullptr;
	present = tabcs->get_table()->get_base()->get_filename() + "\\configcassave";

	string struid = uid.as_1C() + "__";
	auto struid_len = struid.size();
	for (auto &ptf : tabcs->files())
	{
		if (EqualIC(ptf.first.substr(0, struid_len), struid)) {
			TableFile *tf = ptf.second;
			auto pcf = new ContainerFile(tf, tf->name.substr(struid_len, tf->name.size() - struid_len - 1));
			if (EqualIC(pcf->name, "configinfo")) {
				configinfo = pcf;
			}
			files[LowerCase(pcf->name)] = pcf;
		}
	}

	string config_info_path;
	if(!configinfo)
	{
		if(!files.empty()) {
			throw DetailedException("Ошибка поиска файла")
				.add_detail("Путь", present)
				.add_detail("Имя", struid + "configinfo")
				.add_detail("Имя файла", "configinfo");
		}

		config_info_path = tabc->get_table()->get_base()->get_filename() + "\\configcas\\" + configver;
		_configinfo = tabc->get_file(configver);
		if(!_configinfo)
		{
			throw DetailedException("Ошибка поиска файла")
				.add_detail("Путь", config_info_path)
				.add_detail("Имя", configver)
				.add_detail("Имя файла", "configinfo");
		}

		configinfo = new ContainerFile(_configinfo, "configinfo");
		files["configinfo"] = configinfo;
	}
	else {
		config_info_path = present + "\\" + struid_len + "configinfo";
	}

	configinfo->open();

	auto tt = parse_1Cstream(configinfo->stream, config_info_path);
	if(tt == nullptr)
	{
		throw DetailedException("Ошибка разбора файла configinfo")
			.add_detail("Путь", config_info_path);
	}
	ct = tt->get_first();
	if(!ct)
	{
		throw DetailedException("Ошибка разбора файла configinfo")
			.add_detail("Путь", config_info_path);
	}
	ct = ct->get_next();
	if(!ct)
	{
		throw DetailedException("Ошибка разбора файла configinfo")
			.add_detail("Путь", config_info_path);
	}
	ct = ct->get_next();
	if(!ct)
	{
		throw DetailedException("Ошибка разбора файла configinfo")
			.add_detail("Путь", config_info_path);
	}
	ct = ct->get_first();
	if(!ct)
	{
		throw DetailedException("Ошибка разбора файла configinfo")
			.add_detail("Путь", config_info_path);
	}

	stream = new TMemoryStream;
	for (auto m = ToIntDef(ct->get_value(), 0); m; --m)
	{
		ct = ct->get_next();
		if(!ct)
		{
			throw DetailedException("Ошибка разбора файла configinfo")
				.add_detail("Путь", config_info_path);
		}
		if(ct->get_type() != node_type::nd_string)
		{
			throw DetailedException("Ошибка разбора файла configinfo")
				.add_detail("Путь", config_info_path);
		}
		string name = ct->get_value();

		ct = ct->get_next();
		if (files.find(LowerCase(name)) != files.end()) {
			continue;
		}

		if(!ct)
		{
			throw DetailedException("Ошибка разбора файла configinfo")
				.add_detail("Путь", config_info_path);
		}
		if(ct->get_type() != node_type::nd_binary2)
		{
			throw DetailedException("Ошибка разбора файла configinfo")
				.add_detail("Путь", config_info_path);
		}
		stream->Seek(0l, soBeginning);
		base64_decode(ct->get_value(), stream);
		stream->Seek(0l, soBeginning);
		string hashname = hexstring(stream);

		TableFile *tf = tabc->get_file(hashname);
		if(tf)
		{
			auto pcf = new ContainerFile(tf, name);
			files[LowerCase(name)] = pcf;
		}
		else
		{
			throw DetailedException("Ошибка поиска файла")
				.add_detail("Путь", config_info_path)
				.add_detail("Имя", hashname)
				.add_detail("Имя файла", name);
		}
	}

	delete stream;

}

//---------------------------------------------------------------------------
std::string ConfigStorageTableConfigCasSave::presentation() const
{
	return present;
}

std::vector<BinaryGuid> parse_dynamically_updated_tree(unique_ptr<Tree> tt)
{
	if (!tt) {
		throw DetailedException("Ошибка разбора файла DynamicallyUpdated");
	}

	Tree *ct = tt->get_first();
	if (!ct) {
		throw DetailedException("Ошибка разбора файла DynamicallyUpdated");
	}
	ct = ct->get_first();
	if (!ct) {
		throw DetailedException("Ошибка разбора файла DynamicallyUpdated");
	}

	ct = ct->get_next();
	if (!ct) {
		throw DetailedException("Ошибка разбора файла DynamicallyUpdated");
	}

	if (ct->get_type() != node_type::nd_number) {
		throw DetailedException("Ошибка разбора файла DynamicallyUpdated");
	}

	int ndynup = ToIntDef(ct->get_value(), 0);
	std::vector<BinaryGuid> dynup;
	dynup.reserve(ndynup);
	if (ndynup > 0) {
		dynup.reserve(ndynup);
		while (ndynup--) {
			ct = ct->get_next();
			dynup.emplace_back(BinaryGuid(ct->get_value()));
		}
	}
	return dynup;
}
