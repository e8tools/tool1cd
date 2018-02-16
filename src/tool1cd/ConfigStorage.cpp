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
std::string ConfigStorageDirectory::presentation()
{
	return fdir.string();
}

//---------------------------------------------------------------------------
bool ConfigStorageDirectory::fileexists(const std::string &path)
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

	string fname = TStringBuilder(path).Replace('/', '\\')->ToString();
	V8Catalog* c = cat;
	for (auto i = fname.find("\\"); i != string::npos; i = fname.find("\\"))
	{
		V8File *f = c->GetFile(fname.substr(0, i - 1));
		if(!f) {
			return nullptr;
		}
		c = f->GetCatalog();
		if(!c) {
			return nullptr;
		}
		fname = fname.substr(i + 1, fname.size() - i - 1);
	}
	V8File *f = c->GetFile(fname);
	if (!f) {
		return nullptr;
	}
	if (!f->Open()) {
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

	string fname = TStringBuilder(path).Replace('/', '\\')->ToString();
	c = cat;
	for (auto i = fname.find("\\"); i != string::npos; i = fname.find("\\"))
	{
		c = c->CreateCatalog(fname.substr(0, i - 1));
		fname = fname.substr(i + 1, fname.size() - i - 1);
	}
	f = c->createFile(fname);
	f->Write(str);
	return true;
}

//---------------------------------------------------------------------------
std::string ConfigStorageCFFile::presentation()
{
	return filename;
}

//---------------------------------------------------------------------------
void ConfigStorageCFFile::close(ConfigFile* cf)
{
	V8File* f;

	f = (V8File*)cf->addin;
	f->Close();
	delete cf;
}

//---------------------------------------------------------------------------
bool ConfigStorageCFFile::fileexists(const std::string &path)
{
	// По сути, проверяется существование только каталога (файла верхнего уровня)
	// Это неправильно для формата 8.0 с файлом каталогом metadata. Но метод fileexists используется только для внешних файлов,
	// поэтому такой проверки достаточно

	if (!cat->isOpen()) {
		return false;
	}

	string fname = TStringBuilder(path).Replace('/', '\\')->ToString();
	auto i = fname.find("\\");
	if (i != string::npos) {
		fname = fname.substr(0, i - 1);
	}
	V8File *f = cat->GetFile(fname);
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
	String tn;

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

	if(packed == table_file_packed::unknown) packed = isPacked() ? table_file_packed::yes : table_file_packed::no;

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

		for(i = 0; i <= maxpartno; ++i) t->readBlob(ts, addr[i].blob_start, addr[i].blob_length, false);
	}

	if(packed == table_file_packed::yes)
	{
		ts->Seek(0l, soBeginning);
		ZInflateStream(ts, stream);
		if(!rstream)
		{
			delete ts;
			DeleteFile(tn);
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
bool ContainerFile::isPacked()
{

	if (EqualIC(name, "DynamicallyUpdated")
	 || EqualIC(name, "SprScndInfo")
	 || EqualIC(name, "DBNamesVersion")
	 || EqualIC(name, "siVersions")
	 || name.front() == '{'
	 || LowerCase(name).find("fo_version") != std::string::npos) {
		return false;
	}
	auto i = name.find_last_of(".");
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
	String r_name;
	ConfigFile* cf;
	ConfigStorageTable_addin* cfa;

	if(!ready) return nullptr;

	string fname = TStringBuilder(path).Replace('/', '\\')->ToString();
	auto i = fname.find("\\");
	if (i != string::npos) {
		r_name = fname.substr(0, i - 1);
		fname = fname.substr(i + 1, fname.size() - i - 1);
	}
	else
	{
		r_name = fname;
		fname = "";
	}

	auto pfiles = files.find(LowerCase(r_name));
	if (pfiles == files.end()) {
		return nullptr;
	}
	ContainerFile *tf = pfiles->second;

	tf->open();
	if (!fname.empty()) {
		if(!tf->cat) tf->cat = new V8Catalog(tf->stream, false, true);
		c = tf->cat;
		for (auto i = fname.find("\\"); i != string::npos; i = fname.find("\\")) {
			V8File *f = c->GetFile(fname.substr(0, i - 1));
			if (!f) {
				return nullptr;
			}
			c = f->GetCatalog();
			if (!c) {
				return nullptr;
			}
			fname = fname.substr(i + 1, fname.size() - i - 1);
		}
		V8File *f = c->GetFile(fname);
		if(!f) return nullptr;
		if(!f->Open()) return nullptr;
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
		cfa->tf = tf;
		cf->str = tf->stream;
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
		cfa->f->Close();
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
			msreg_g.Status(String(j) + "%");
		}

		ContainerFile* tf = pfiles.second;
		if(tf->ropen())
		{
			V8File* f = catalog->createFile(tf->name);
			f->time_create(tf->file->ft_create);
			f->time_modify(tf->file->ft_modify);
			f->WriteAndClose(tf->rstream);

			tf->close();
		}
	}

	msreg_g.Status("");
	return true;
}

//---------------------------------------------------------------------------
bool ConfigStorageTable::fileexists(const std::string &path)
{
	// По сути, проверяется существование только каталога (файла записи верхнего уровня)
	// Это неправильно для формата 8.0 с файлом каталогом metadata. Но метод fileexists используется только для внешних файлов, поэтому такой проверки достаточно

	if(!ready) return false;

	string fname = TStringBuilder(path).Replace('/', '\\')->ToString();
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
	int m;
	String s;
	string name, ext;
	TableFile* tf;
	std::map<String,TableFile*>::iterator pfilesmap;
	TableFile* _DynamicallyUpdated;
	ContainerFile* DynamicallyUpdated;
	tree* tt;
	tree* ct;
	std::vector<BinaryGuid> dynup;
	BinaryGuid g;
	Table* tab;
	int dynno;
	ContainerFile* pcf;

	ready = tabf->getready();
	if(!ready) return;

	present = tabf->gettable()->getbase()->getfilename() + "\\CONFIG";

	tab = tabf->gettable();
	_DynamicallyUpdated = tabf->getfile("DynamicallyUpdated");

	if(_DynamicallyUpdated)
	{
		ContainerFile DynamicallyUpdated(_DynamicallyUpdated, _DynamicallyUpdated->name);
		DynamicallyUpdated.open();
		string s = tab->getbase()->getfilename() + "\\" + tab->getname() + "\\" + DynamicallyUpdated.name;
		tree *tt = parse_1Cstream(DynamicallyUpdated.stream, s);
		if(!tt)
		{
			msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
				, "Путь", s);
		}
		else
		{
			ct = tt->get_first();
			if(!ct)
			{
				msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
					, "Путь", s);
			}
			else
			{
				ct = ct->get_first();
				if(!ct)
				{
					msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
						, "Путь", s);
				}
				else
				{
					ct = ct->get_next();
					if(!ct)
					{
						msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
							, "Путь", s);
					}
					else
					{
						if(ct->get_type() != node_type::nd_number)
						{
							msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
								, "Путь", s);
						}
						else
						{
							int ndynup = ToIntDef(ct->get_value(), 0);
							if(ndynup > 0)
							{
								dynup.reserve(ndynup);
								for(m = 0; m < ndynup; ++m)
								{
									ct = ct->get_next();
									dynup.emplace_back(BinaryGuid(ct->get_value()));
								}
							}
							else ndynup = 0;
						}

					}
				}
			}
		}
	}

	for (pfilesmap = tabf->files().begin(); pfilesmap != tabf->files().end(); ++pfilesmap)
	{
		tf = pfilesmap->second;
		if(tf == _DynamicallyUpdated) continue;
		if(tf->addr->blob_length == 0) continue;
		string s = tf->name;
		m = s.find_last_of(spoint);
		if (m != string::npos) {
			name = s.substr(0, m);
			ext = s.substr(m + 1, s.size() - m - 1);
		}
		else
		{
			name = s;
			ext = "";
		}

		if (CompareIC(ext, "new") == 0) {
			ext = "";
			dynno = - 2;
		}
		else dynno = -1;

		auto m = name.find_last_of(sdynupdate);
		if (m != string::npos) {
			string s = name.substr(m + lsdynupdate - 1, name.size() - m - lsdynupdate);
			name = name.substr(0, m);
			g = BinaryGuid(s);
			if(!dynup.empty())
			{
				auto found = std::find(dynup.begin(), dynup.end(), g);
				if (found == dynup.end()) {
					dynno = -2;
				} else {
					dynno = found - dynup.begin();
				}
			}
			else dynno = -2;
		}

		if(!ext.empty()) {
			name += spoint;
			name += ext;
		}

		auto pfiles = files.find(LowerCase(name));
		if (pfiles == files.end())
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
std::string ConfigStorageTableConfig::presentation()
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
	std::map<String,TableFile*>::iterator pfilesmap;
	std::map<String,ContainerFile*>::iterator pfiles;
	TableFile* _DynamicallyUpdated;
	TableFile* _deleted;
	tree* tt;
	tree* ct;
	BinaryGuid* dynup;
	int ndynup = 0;
	BinaryGuid g;
	Table* tab;
	int dynno;
	ContainerFile* pcf;
	set<string> del;
	set<string>::iterator pdel;

	ready = tabc->getready();
	if(!ready) return;
	ready = tabcs->getready();
	if(!ready) return;

	present = tabc->gettable()->getbase()->getfilename() + "\\configsave";

	tab = tabcs->gettable();
	_deleted = tabcs->getfile("deleted");
	if(_deleted)
	{
		ContainerFile deleted(_deleted, _deleted->name);
		deleted.open();
		string s = tab->getbase()->getfilename()
				   + "\\" + tab->getname()
				   + "\\" + deleted.name;
		tt = parse_1Cstream(deleted.stream, s);
		if(!tt)
		{
			msreg_g.AddError("Ошибка разбора файла deleted"
				, "Путь", s);
		}
		else
		{
			ct = tt->get_first();
			if(!ct)
			{
				msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
					, "Путь", s);
			}
			{
				for(m = ToIntDef(ct->get_value(), 0); m; --m)
				{
					ct = ct->get_next();
					if(!ct)
					{
						msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
							, "Путь", s);
						break;
					}
					del.insert(LowerCase(ct->get_value()));
					ct = ct->get_next();
					if(!ct)
					{
						msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
							, "Путь", s);
						break;
					}
				}
			}

		}
	}

	tab = tabc->gettable();
	_DynamicallyUpdated = tabc->getfile("DynamicallyUpdated");

	dynup = nullptr;
	if(_DynamicallyUpdated)
	{
		ContainerFile DynamicallyUpdated(_DynamicallyUpdated, _DynamicallyUpdated->name);
		DynamicallyUpdated.open();
		string s = tab->getbase()->getfilename()
				+ "\\" + tab->getname()
				+ "\\" + DynamicallyUpdated.name;
		tree *tt = parse_1Cstream(DynamicallyUpdated.stream, s);
		if(!tt)
		{
			msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
				, "Путь", s);
		}
		else
		{
			ct = tt->get_first();
			if(!ct)
			{
				msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
					, "Путь", s);
			}
			else
			{
				ct = ct->get_first();
				if(!ct)
				{
					msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
						, "Путь", s);
				}
				else
				{
					ct = ct->get_next();
					if(!ct)
					{
						msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
							, "Путь", s);
					}
					else
					{
						if(ct->get_type() != node_type::nd_number)
						{
							msreg_g.AddError("Ошибка разбора файла DynamicallyUpdated"
								, "Путь", s);
						}
						else
						{
							ndynup = ToIntDef(ct->get_value(), 0);
							if(ndynup > 0)
							{
								dynup = new BinaryGuid[ndynup];
								for(m = 0; m < ndynup; ++m)
								{
									ct = ct->get_next();
									string_to_GUID(ct->get_value(), &dynup[m]);
								}
							}
							else ndynup = 0;
						}

					}
				}
			}
		}
	}

	m = ndynup + 2;

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
			msreg_g.AddError("Ошибка чтения CONFIGSAVE. Повторяющееся имя файла"
				, "Имя файла", name);
		}
	}

	for (auto &pfilesmap : tabc->files()) {
		tf = pfilesmap.second;
		if(tf == _DynamicallyUpdated) continue;
		if(tf->addr->blob_length == 0) continue;
		string s = tf->name;

		pdel = del.find(LowerCase(s));
		if(pdel != del.end()) continue;

		m = s.find_last_of(spoint);
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
			dynno = ndynup + 1;
		}
		else dynno = -1;

		m = name.find(sdynupdate);
		if (m != string::npos) {
			string s = name.substr(m + lsdynupdate, name.size() - m - lsdynupdate + 1);
			name = name.substr(0, m - 1);
			string_to_GUID(s, &g);
			if(dynup)
			{
				for(m = 0; m < ndynup; ++m) if(g == dynup[m]) break;
				if(m >= ndynup) dynno = -2;
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

	if(dynup) delete[] dynup;

}

//---------------------------------------------------------------------------
std::string ConfigStorageTableConfigSave::presentation()
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
	tree* ct;
	TableFile* tf;
	ContainerFile* pcf;
	TMemoryStream* stream;

	ready = tabc->getready();
	if(!ready) return;

	present = tabc->gettable()->getbase()->getfilename() + "\\configcas";
	string filepath = present + "\\" + configver;

	_configinfo = tabc->getfile(configver);
	if(!_configinfo)
	{
		msreg_g.AddError("Ошибка поиска файла"
			, "Путь", filepath
			, "Имя", configver
			, "Имя файла", "configinfo");
		return;
	}

	configinfo = new ContainerFile(_configinfo, "configinfo");
	files["configinfo"] = configinfo;
	configinfo->open();
	tree *tt = parse_1Cstream(configinfo->stream, filepath);
	if(!tt)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", filepath);
		delete tt;
		return;
	}
	ct = tt->get_first();
	if(!ct)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", filepath);
		delete tt;
		return;
	}
	ct = ct->get_next();
	if(!ct)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", filepath);
		delete tt;
		return;
	}
	ct = ct->get_next();
	if(!ct)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", filepath);
		delete tt;
		return;
	}
	ct = ct->get_first();
	if(!ct)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", filepath);
		delete tt;
		return;
	}

	stream = new TMemoryStream;
	for(m = ToIntDef(ct->get_value(), 0); m; --m)
	{
		ct = ct->get_next();
		if(!ct)
		{
			msreg_g.AddError("Ошибка разбора файла configinfo"
				, "Путь", filepath);
			delete tt;
			delete stream;
			return;
		}
		if(ct->get_type() != node_type::nd_string)
		{
			msreg_g.AddError("Ошибка разбора файла configinfo"
				, "Путь", filepath);
			delete tt;
			delete stream;
			return;
		}
		string name = ct->get_value();

		ct = ct->get_next();
		if(!ct)
		{
			msreg_g.AddError("Ошибка разбора файла configinfo"
				, "Путь", filepath);
			delete tt;
			delete stream;
			return;
		}
		if(ct->get_type() != node_type::nd_binary2)
		{
			msreg_g.AddError("Ошибка разбора файла configinfo"
				, "Путь", filepath);
			delete tt;
			delete stream;
			return;
		}
		stream->Seek(0l, soBeginning);
		base64_decode(ct->get_value(), stream);
		stream->Seek(0l, soBeginning);
		string hashname = hexstring(stream);

		tf = tabc->getfile(hashname);
		if(tf)
		{
			pcf = new ContainerFile(tf, name);
			files[LowerCase(name)] = pcf;
		}
		else
		{
			msreg_g.AddError("Ошибка поиска файла"
				, "Путь", filepath
				, "Имя", hashname
				, "Имя файла", name);
			delete tt;
			delete stream;
			return;
		}
	}

	delete tt;
	delete stream;

}

//---------------------------------------------------------------------------
std::string ConfigStorageTableConfigCas::presentation()
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
	String s, name, hashname;
	TableFile* _configinfo;
	ContainerFile* configinfo;
	tree* tt;
	tree* ct;
	TableFile* tf;
	ContainerFile* pcf;
	TMemoryStream* stream;
	int gl;
	std::map<String,TableFile*>::iterator ptf;


	ready = tabc->getready();
	if(!ready) return;
	ready = tabcs->getready();
	if(!ready) return;

	configinfo = nullptr;
	present = tabcs->gettable()->getbase()->getfilename() + "\\CONFIGCASSAVE";

	string g = uid.as_1C() + "__";
	gl = g.size();
	for(ptf = tabcs->files().begin(); ptf != tabcs->files().end(); ++ptf)
	{
		if (EqualIC(ptf->first.substr(0, gl), g)) {
			tf = ptf->second;
			pcf = new ContainerFile(tf, tf->name.substr(gl, tf->name.size() - gl - 1));
			if (EqualIC(pcf->name, "configinfo")) {
				configinfo = pcf;
			}
			files[LowerCase(pcf->name)] = pcf;
		}
	}

	if(!configinfo)
	{
		if(files.size())
		{
			msreg_g.AddError("Ошибка поиска файла"
				, "Путь", present
				, "Имя", g + "configinfo"
				, "Имя файла", "configinfo");
			return;
		}

		s = tabc->gettable()->getbase()->getfilename() + "\\CONFIGCAS\\" + configver;
		_configinfo = tabc->getfile(configver);
		if(!_configinfo)
		{
			msreg_g.AddError("Ошибка поиска файла"
				, "Путь", s
				, "Имя", configver
				, "Имя файла", "configinfo");
			return;
		}

		configinfo = new ContainerFile(_configinfo, "configinfo");
		files["configinfo"] = configinfo;
	}
	else s = present + "\\" + gl + "configinfo";

	configinfo->open();
	tt = parse_1Cstream(configinfo->stream, s);
	if(!tt)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = tt->get_first();
	if(!ct)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = ct->get_next();
	if(!ct)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = ct->get_next();
	if(!ct)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = ct->get_first();
	if(!ct)
	{
		msreg_g.AddError("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}

	stream = new TMemoryStream;
	for(m = ToIntDef(ct->get_value(), 0); m; --m)
	{
		ct = ct->get_next();
		if(!ct)
		{
			msreg_g.AddError("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		if(ct->get_type() != node_type::nd_string)
		{
			msreg_g.AddError("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		name = ct->get_value();

		ct = ct->get_next();
		if (files.find(LowerCase(name)) != files.end()) {
			continue;
		}

		if(!ct)
		{
			msreg_g.AddError("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		if(ct->get_type() != node_type::nd_binary2)
		{
			msreg_g.AddError("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		stream->Seek(0l, soBeginning);
		base64_decode(ct->get_value(), stream);
		stream->Seek(0l, soBeginning);
		hashname = hexstring(stream);

		tf = tabc->getfile(hashname);
		if(tf)
		{
			pcf = new ContainerFile(tf, name);
			files[LowerCase(name)] = pcf;
		}
		else
		{
			msreg_g.AddError("Ошибка поиска файла"
				, "Путь", s
				, "Имя", hashname
				, "Имя файла", name);
			delete tt;
			delete stream;
			return;
		}
	}

	delete tt;
	delete stream;

}

//---------------------------------------------------------------------------
std::string ConfigStorageTableConfigCasSave::presentation()
{
	return present;
}
