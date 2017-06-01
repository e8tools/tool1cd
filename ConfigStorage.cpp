//---------------------------------------------------------------------------

#pragma hdrstop

#include "ConfigStorage.h"
#include "MessageRegistration.h"
#include "Common.h"
#include "Base64.h"
#include "TempStream.h"

//#pragma package(smart_init)

#include "UZLib.h"
#pragma comment (lib, "zlibstatic.lib")

#define CHUNK 65536

//---------------------------------------------------------------------------
#define error if(msreg) msreg->AddError
#define status if(msreg) msreg->Status
extern MessageRegistrator* msreg;
const String spoint = ".";
const String sdynupdate = "_dynupdate_";
const int lsdynupdate = sdynupdate.Length();

//********************************************************
// Класс ConfigStorageDirectory

//---------------------------------------------------------------------------
ConfigStorageDirectory::ConfigStorageDirectory(const String& _dir)
{
	fdir = _dir;
	if(*fdir.LastChar() != '\\') fdir += '\\';
}

//---------------------------------------------------------------------------
ConfigFile* ConfigStorageDirectory::readfile(const String& path)
{
	ConfigFile* cf;
	String filename;

	filename = fdir + TStringBuilder(path).Replace('/', '\\')->ToString();

	//status(path);

	if(FileExists(filename))
	{
		cf = new ConfigFile;
		try
		{
			cf->str = new TFileStream(filename, fmOpenRead);
			cf->addin = NULL;
			return cf;
		}
		catch(...)
		{
			// Здесь надо бы что-нибудь сообщить об ошибке
			delete cf;
			return NULL;
		}

	}
	else return NULL;

}

//---------------------------------------------------------------------------
bool ConfigStorageDirectory::writefile(const String& path, TStream* str)
{
	String filename = fdir + TStringBuilder(path).Replace('/', '\\')->ToString();
	TFileStream* f = new TFileStream(filename, fmCreate);
	f->CopyFrom(str, 0);
	delete f;

	return true;
}

//---------------------------------------------------------------------------
String ConfigStorageDirectory::presentation()
{
	return fdir.SubString(1, fdir.Length() - 1);
}

//---------------------------------------------------------------------------
bool ConfigStorageDirectory::fileexists(const String& path)
{
	String filename;
	filename = fdir + TStringBuilder(path).Replace('/', '\\')->ToString();
	return FileExists(filename);
}

//---------------------------------------------------------------------------

//********************************************************
// Класс ConfigStorageCFFile

//---------------------------------------------------------------------------
ConfigStorageCFFile::ConfigStorageCFFile(const String& fname)
{
	filename = fname;
	cat = new v8catalog(filename);
}

//---------------------------------------------------------------------------
ConfigStorageCFFile::~ConfigStorageCFFile()
{
	delete cat;
}

//---------------------------------------------------------------------------
ConfigFile* ConfigStorageCFFile::readfile(const String& path)
{
	v8catalog* c;
	v8file* f;
	int i;
	//TStream* s;
	ConfigFile* cf;

	if(!cat->isOpen()) return NULL;

	//status(path);

	String fname = TStringBuilder(path).Replace('/', '\\')->ToString();
	c = cat;
	for(i = fname.Pos("\\"); i; i = fname.Pos("\\"))
	{
		f = c->GetFile(fname.SubString(1, i - 1));
		if(!f) return NULL;
		c = f->GetCatalog();
		if(!c) return NULL;
		fname = fname.SubString(i + 1, fname.Length() - i);
	}
	f = c->GetFile(fname);
	if(!f) return NULL;
	if(!f->Open()) return NULL;
	cf = new ConfigFile;
	//cf->str = f->get_data();
	cf->str = f->get_stream();
	cf->str->Seek(0l, soBeginning);
	cf->addin = f;
	return cf;

}

//---------------------------------------------------------------------------
bool ConfigStorageCFFile::writefile(const String& path, TStream* str)
{
	v8catalog* c;
	v8file* f;
	int i;

	if(!cat->isOpen()) return false;

	String fname = TStringBuilder(path).Replace('/', '\\')->ToString();
	c = cat;
	for(i = fname.Pos("\\"); i; i = fname.Pos("\\"))
	{
		c = c->CreateCatalog(fname.SubString(1, i - 1));
		fname = fname.SubString(i + 1, fname.Length() - i);
	}
	f = c->createFile(fname);
	f->Write(str);
	return true;
}

//---------------------------------------------------------------------------
String ConfigStorageCFFile::presentation()
{
	return filename;
}

//---------------------------------------------------------------------------
void ConfigStorageCFFile::close(ConfigFile* cf)
{
	v8file* f;

	f = (v8file*)cf->addin;
	f->Close();
	delete cf;
}

//---------------------------------------------------------------------------
bool ConfigStorageCFFile::fileexists(const String& path)
{
	// По сути, проверяется существование только каталога (файла верхнего уровня)
	// Это неправильно для формата 8.0 с файлом каталогом metadata. Но метод fileexists используется только для внешних файлов,
	// поэтому такой проверки достаточно

	v8file* f;
	int i;

	if(!cat->isOpen()) return false;

	String fname = TStringBuilder(path).Replace('/', '\\')->ToString();
	i = fname.Pos("\\");
	if(i) fname = fname.SubString(1, i - 1);
	f = cat->GetFile(fname);
	if(!f) return false;
	return true;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс container_file

//---------------------------------------------------------------------------
container_file::container_file(table_file* _f, const String& _name)
{
	unsigned int i;

	file = _f;
	name = _name;
	stream = NULL;
	rstream = NULL;
	cat = NULL;
	packed = tfp_unknown;
	dynno = -3;
}

//---------------------------------------------------------------------------
container_file::~container_file()
{
	close();
}

//---------------------------------------------------------------------------
bool container_file::open()
{
	TStream* ts;
	String tn;
	wchar_t tempfile[MAX_PATH];
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

	if(packed == tfp_unknown) packed = isPacked() ? tfp_yes : tfp_no;

	if(rstream)
	{
		if(packed == tfp_yes) ts = rstream;
		else
		{
			stream = rstream;
			stream->Seek(0l, soBeginning);
			return true;
		}
	}
	else
	{
		if(packed == tfp_yes)
		{
			if(maxpartno > 0) ts = new TTempStream;
			else ts = new TMemoryStream;
		}
		else ts = stream;

		for(i = 0; i <= maxpartno; ++i) t->readBlob(ts, addr[i].blob_start, addr[i].blob_length, false);
	}

	if(packed == tfp_yes)
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
bool container_file::ropen()
{
	wchar_t tempfile[MAX_PATH];
	unsigned int i;
	Table* t;
	table_blob_file* addr;
	unsigned int maxpartno;

	//status(name);

	if(rstream)
	{
		rstream->Seek(0l, soBeginning);
		return true;
	}
	t = file->t;
	addr = file->addr;
	maxpartno = file->maxpartno;
	if(packed == tfp_unknown) packed = isPacked() ? tfp_yes : tfp_no;
	if(packed == tfp_no && stream)
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
void container_file::close()
{
	delete cat;
	cat = NULL;
	if(stream != rstream)
	{
		delete stream;
		delete rstream;
	}
	else delete stream;
	stream = NULL;
	rstream = NULL;
}

//---------------------------------------------------------------------------
bool container_file::isPacked()
{
	int i;
	String ext;

	if(name.CompareIC("DynamicallyUpdated") == 0) return false;
	if(name.CompareIC("SprScndInfo") == 0) return false;
	if(name.CompareIC("DBNamesVersion") == 0) return false;
	if(name.CompareIC("siVersions") == 0) return false;
	if(name.UpperCase().Pos("FO_VERSION") > 0) return false;
	if(name[1] == '{') return false;
	i = name.LastDelimiter(spoint);
	if(i)
	{
		ext = name.SubString(i + 1, name.Length() - i).UpperCase();
		if(ext.Compare("INF") == 0) return false;
		if(ext.Compare("PF") == 0) return false;
	}
	return true;

}

//---------------------------------------------------------------------------

//********************************************************
// Структура дополнительных данных открытого файла класса ConfigStorageTable

//---------------------------------------------------------------------------
enum ConfigStorageTableAddinVariant
{
	cstav_container_file,
	cstav_v8file
};

//---------------------------------------------------------------------------
struct ConfigStorageTable_addin
{
	ConfigStorageTableAddinVariant variant;
	union
	{
		container_file* tf;
		v8file* f;
	};
};

//********************************************************
// Класс ConfigStorageTable

//---------------------------------------------------------------------------
ConfigStorageTable::~ConfigStorageTable()
{
	std::map<String,container_file*>::iterator pfiles;

	for(pfiles = files.begin(); pfiles != files.end(); ++pfiles)
	{
		delete pfiles->second;
	}

	delete base;
}

//---------------------------------------------------------------------------
ConfigFile* ConfigStorageTable::readfile(const String& path)
{
	container_file* tf;
	std::map<String,container_file*>::iterator pfiles;
	v8catalog* c;
	v8file* f;
	int i;
	String fname;
	String r_name;
	//TStream* s;
	ConfigFile* cf;
	ConfigStorageTable_addin* cfa;

	if(!ready) return NULL;

	//status(path);

	fname = TStringBuilder(path).Replace('/', '\\')->ToString();
	i = fname.Pos("\\");
	if(i)
	{
		r_name = fname.SubString(1, i - 1);
		fname = fname.SubString(i + 1, fname.Length() - i);
	}
	else
	{
		r_name = fname;
		fname = "";
	}

	pfiles = files.find(r_name.UpperCase());
	if(pfiles == files.end()) return NULL;
	tf = pfiles->second;

	tf->open();
	if(!fname.IsEmpty())
	{
		if(!tf->cat) tf->cat = new v8catalog(tf->stream, false, true);
		c = tf->cat;
		for(i = fname.Pos("\\"); i; i = fname.Pos("\\"))
		{
			f = c->GetFile(fname.SubString(1, i - 1));
			if(!f) return NULL;
			c = f->GetCatalog();
			if(!c) return NULL;
			fname = fname.SubString(i + 1, fname.Length() - i);
		}
		f = c->GetFile(fname);
		if(!f) return NULL;
		if(!f->Open()) return NULL;
		cf = new ConfigFile;
		cfa = new ConfigStorageTable_addin;
		cfa->variant = cstav_v8file;
		cfa->f = f;
		//cf->str = f->get_data();
		cf->str = f->get_stream();
		cf->str->Seek(0l, soBeginning);
		cf->addin = cfa;
	}
	else
	{
		cf = new ConfigFile;
		cfa = new ConfigStorageTable_addin;
		cfa->variant = cstav_container_file;
		cfa->tf = tf;
		cf->str = tf->stream;
		cf->str->Seek(0l, soBeginning);
		cf->addin = cfa;
	}

	return cf;

}

//---------------------------------------------------------------------------
bool ConfigStorageTable::writefile(const String& path, TStream* str)
{
	return false; // Запись в таблицы пока не поддерживается
}

//---------------------------------------------------------------------------
void ConfigStorageTable::close(ConfigFile* cf)
{
	ConfigStorageTable_addin* cfa;

	cfa = (ConfigStorageTable_addin*)cf->addin;
	if(cfa->variant == cstav_container_file)
	{
		cfa->tf->close();
	}
	else if(cfa->variant == cstav_v8file)
	{
		cfa->f->Close();
	}
	delete cfa;
	delete cf;
}

//---------------------------------------------------------------------------
bool ConfigStorageTable::save_config(String _filename)
{
	v8catalog* c;
	v8file* f;
	std::map<String,container_file*>::iterator pfiles;
	container_file* tf;
	int i, j, prevj, size;

	if(!ready) return false;

	size = files.size();
	prevj = 101;

	if(FileExists(_filename)) DeleteFile(_filename);
	c = new v8catalog(_filename, false);
	for(pfiles = files.begin(), i = 1; pfiles != files.end(); ++pfiles, ++i)
	{
		j = i * 100 / size;
		if(j != prevj)
		{
			prevj = j;
			status(String(j) + "%");
		}

		tf = pfiles->second;
		if(tf->ropen())
		{
			f = c->createFile(tf->name);
			f->SetTimeCreate(&tf->file->ft_create);
			f->SetTimeModify(&tf->file->ft_modify);
			f->WriteAndClose(tf->rstream);

			tf->close();
		}

	}

	delete c;
	status("");
	return true;
}

//---------------------------------------------------------------------------
bool ConfigStorageTable::fileexists(const String& path)
{
	// По сути, проверяется существование только каталога (файла записи верхнего уровня)
	// Это неправильно для формата 8.0 с файлом каталогом metadata. Но метод fileexists используется только для внешних файлов, поэтому такой проверки достаточно

	std::map<String,container_file*>::iterator pfiles;
	int i;
	String fname;

	if(!ready) return false;

	fname = TStringBuilder(path).Replace('/', '\\')->ToString();
	i = fname.Pos("\\");
	if(i) fname = fname.SubString(1, i - 1);

	pfiles = files.find(fname.UpperCase());
	if(pfiles == files.end()) return false;
	return true;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс ConfigStorageTableConfig

//---------------------------------------------------------------------------
ConfigStorageTableConfig::ConfigStorageTableConfig(TableFiles* tabf, T_1CD* _base) : ConfigStorageTable(_base)
{
	int m;
	String s;
	String name, ext;
	table_file* tf;
	std::map<String,table_file*>::iterator pfilesmap;
	std::map<String,container_file*>::iterator pfiles;
	table_file* _DynamicallyUpdated;
	container_file* DynamicallyUpdated;
	tree* tt;
	tree* ct;
	TGUID* dynup;
	int ndynup;
	TGUID g;
	Table* tab;
	int dynno;
	container_file* pcf;

	ready = tabf->getready();
	if(!ready) return;

	present = tabf->gettable()->getbase()->getfilename() + "\\CONFIG";

	tab = tabf->gettable();
	_DynamicallyUpdated = tabf->getfile("DynamicallyUpdated");

	dynup = NULL;
	if(_DynamicallyUpdated)
	{
		DynamicallyUpdated = new container_file(_DynamicallyUpdated, _DynamicallyUpdated->name);
		DynamicallyUpdated->open();
		s = tab->getbase()->getfilename() + "\\" + tab->getname() + "\\" + DynamicallyUpdated->name;
		tt = parse_1Cstream(DynamicallyUpdated->stream, s);
		if(!tt)
		{
			error("Ошибка разбора файла DynamicallyUpdated"
				, "Путь", s);
		}
		else
		{
			ct = tt->get_first();
			if(!ct)
			{
				error("Ошибка разбора файла DynamicallyUpdated"
					, "Путь", s);
			}
			else
			{
				ct = ct->get_first();
				if(!ct)
				{
					error("Ошибка разбора файла DynamicallyUpdated"
						, "Путь", s);
				}
				else
				{
					ct = ct->get_next();
					if(!ct)
					{
						error("Ошибка разбора файла DynamicallyUpdated"
							, "Путь", s);
					}
					else
					{
						if(ct->get_type() != nd_number)
						{
							error("Ошибка разбора файла DynamicallyUpdated"
								, "Путь", s);
						}
						else
						{
							ndynup = ct->get_value().ToIntDef(0);
							if(ndynup > 0)
							{
								dynup = new TGUID[ndynup];
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
		delete DynamicallyUpdated;
	}

	for (pfilesmap = tabf->files().begin(); pfilesmap != tabf->files().end(); ++pfilesmap)
	{
		tf = pfilesmap->second;
		if(tf == _DynamicallyUpdated) continue;
		if(tf->addr->blob_length == 0) continue;
		s = tf->name;
		m = s.LastDelimiter(spoint);
		if(m)
		{
			name = s.SubString(1, m - 1);
			ext = s.SubString(m + 1, s.Length() - m);
		}
		else
		{
			name = s;
			ext = "";
		}

		if(ext.CompareIC("new") == 0)
		{
			ext = "";
			dynno = - 2;
		}
		else dynno = -1;

		m = name.Pos(sdynupdate);
		if(m)
		{
			s = name.SubString(m + lsdynupdate, name.Length() - m - lsdynupdate + 1);
			name = name.SubString(1, m - 1);
			string_to_GUID(s, &g);
			if(dynup)
			{
				for(m = 0; m < ndynup; ++m) if(g == dynup[m]) break;
				if(m >= ndynup) dynno = -2;
				else dynno = m;
			}
			else dynno = -2;
		}

		if(!ext.IsEmpty()) name = name + spoint + ext;
		s = name.UpperCase();

		pfiles = files.find(s);
		if(pfiles == files.end())
		{
			pcf = new container_file(tf, name);
			files[s] = pcf;
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
String ConfigStorageTableConfig::presentation()
{
	return present;
}

//********************************************************
// Класс ConfigStorageTableConfigSave

//---------------------------------------------------------------------------
ConfigStorageTableConfigSave::ConfigStorageTableConfigSave(TableFiles* tabc, TableFiles* tabcs, T_1CD* _base) : ConfigStorageTable(_base)
{
	int m;
	String s;
	String name, ext;
	table_file* tf;
	std::map<String,table_file*>::iterator pfilesmap;
	std::map<String,container_file*>::iterator pfiles;
	table_file* _DynamicallyUpdated;
	container_file* DynamicallyUpdated;
	table_file* _deleted;
	container_file* deleted;
	tree* tt;
	tree* ct;
	TGUID* dynup;
	int ndynup = 0;
	TGUID g;
	Table* tab;
	int dynno;
	container_file* pcf;
	std::set<String> del;
	std::set<String>::iterator pdel;

	ready = tabc->getready();
	if(!ready) return;
	ready = tabcs->getready();
	if(!ready) return;

	present = tabc->gettable()->getbase()->getfilename() + "\\CONFIGSAVE";

	tab = tabcs->gettable();
	_deleted = tabcs->getfile("deleted");
	if(_deleted)
	{
		deleted = new container_file(_deleted, _deleted->name);
		deleted->open();
		s = tab->getbase()->getfilename() + "\\" + tab->getname() + "\\" + deleted->name;
		tt = parse_1Cstream(deleted->stream, s);
		if(!tt)
		{
			error("Ошибка разбора файла deleted"
				, "Путь", s);
		}
		else
		{
			ct = tt->get_first();
			if(!ct)
			{
				error("Ошибка разбора файла DynamicallyUpdated"
					, "Путь", s);
			}
			{
				for(m = ct->get_value().ToIntDef(0); m; --m)
				{
					ct = ct->get_next();
					if(!ct)
					{
						error("Ошибка разбора файла DynamicallyUpdated"
							, "Путь", s);
						break;
					}
					del.insert(ct->get_value().UpperCase());
					ct = ct->get_next();
					if(!ct)
					{
						error("Ошибка разбора файла DynamicallyUpdated"
							, "Путь", s);
						break;
					}
				}
			}

		}
		delete deleted;
	}

	tab = tabc->gettable();
	_DynamicallyUpdated = tabc->getfile("DynamicallyUpdated");

	dynup = NULL;
	if(_DynamicallyUpdated)
	{
		DynamicallyUpdated = new container_file(_DynamicallyUpdated, _DynamicallyUpdated->name);
		DynamicallyUpdated->open();
		s = tab->getbase()->getfilename() + "\\" + tab->getname() + "\\" + DynamicallyUpdated->name;
		tt = parse_1Cstream(DynamicallyUpdated->stream, s);
		if(!tt)
		{
			error("Ошибка разбора файла DynamicallyUpdated"
				, "Путь", s);
		}
		else
		{
			ct = tt->get_first();
			if(!ct)
			{
				error("Ошибка разбора файла DynamicallyUpdated"
					, "Путь", s);
			}
			else
			{
				ct = ct->get_first();
				if(!ct)
				{
					error("Ошибка разбора файла DynamicallyUpdated"
						, "Путь", s);
				}
				else
				{
					ct = ct->get_next();
					if(!ct)
					{
						error("Ошибка разбора файла DynamicallyUpdated"
							, "Путь", s);
					}
					else
					{
						if(ct->get_type() != nd_number)
						{
							error("Ошибка разбора файла DynamicallyUpdated"
								, "Путь", s);
						}
						else
						{
							ndynup = ct->get_value().ToIntDef(0);
							if(ndynup > 0)
							{
								dynup = new TGUID[ndynup];
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
		delete DynamicallyUpdated;
	}

	m = ndynup + 2;

	for(pfilesmap = tabcs->files().begin(); pfilesmap != tabcs->files().end(); ++pfilesmap)
	{
		tf = pfilesmap->second;
		if(tf == _deleted) continue;
		if(tf->addr->blob_length == 0) continue;
		name = tf->name;
		s = name.UpperCase();

		pfiles = files.find(s);
		if(pfiles == files.end())
		{
			pcf = new container_file(tf, name);
			files[s] = pcf;
			pcf->dynno = m;
		}
		else
		{
			error("Ошибка чтения CONFIGSAVE. Повторяющееся имя файла"
				, "Имя файла", s);
		}
	}

	for(pfilesmap = tabc->files().begin(); pfilesmap != tabc->files().end(); ++pfilesmap)
	{
		tf = pfilesmap->second;
		if(tf == _DynamicallyUpdated) continue;
		if(tf->addr->blob_length == 0) continue;
		s = tf->name;

		pdel = del.find(s.UpperCase());
		if(pdel != del.end()) continue;

		m = s.LastDelimiter(spoint);
		if(m)
		{
			name = s.SubString(1, m - 1);
			ext = s.SubString(m + 1, s.Length() - m);
		}
		else
		{
			name = s;
			ext = "";
		}
		if(ext.CompareIC("new") == 0)
		{
			ext = "";
			dynno = ndynup + 1;
		}
		else dynno = -1;

		m = name.Pos(sdynupdate);
		if(m)
		{
			s = name.SubString(m + lsdynupdate, name.Length() - m - lsdynupdate + 1);
			name = name.SubString(1, m - 1);
			string_to_GUID(s, &g);
			if(dynup)
			{
				for(m = 0; m < ndynup; ++m) if(g == dynup[m]) break;
				if(m >= ndynup) dynno = -2;
				else dynno = m;
			}
			else dynno = -2;
		}

		if(!ext.IsEmpty()) name = name + spoint + ext;
		s = name.UpperCase();

		pfiles = files.find(s);
		if(pfiles == files.end())
		{
			pcf = new container_file(tf, name);
			files[s] = pcf;
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
String ConfigStorageTableConfigSave::presentation()
{
	return present;
}

//********************************************************
// Класс ConfigStorageTableConfigCas

//---------------------------------------------------------------------------
ConfigStorageTableConfigCas::ConfigStorageTableConfigCas(TableFiles* tabc, const String& configver, T_1CD* _base) : ConfigStorageTable(_base)
{
	int m;
	String s, name, hashname;
	table_file* _configinfo;
	container_file* configinfo;
	tree* tt;
	tree* ct;
	table_file* tf;
	container_file* pcf;
	TMemoryStream* stream;

	ready = tabc->getready();
	if(!ready) return;

	present = tabc->gettable()->getbase()->getfilename() + "\\CONFIGCAS";
	s = present + "\\" + configver;

	_configinfo = tabc->getfile(configver);
	if(!_configinfo)
	{
		error("Ошибка поиска файла"
			, "Путь", s
			, "Имя", configver
			, "Имя файла", "configinfo");
		return;
	}

	configinfo = new container_file(_configinfo, "configinfo");
	files["CONFIGINFO"] = configinfo;
	configinfo->open();
	tt = parse_1Cstream(configinfo->stream, s);
	if(!tt)
	{
		error("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = tt->get_first();
	if(!ct)
	{
		error("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = ct->get_next();
	if(!ct)
	{
		error("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = ct->get_next();
	if(!ct)
	{
		error("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = ct->get_first();
	if(!ct)
	{
		error("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}

	stream = new TMemoryStream;
	for(m = ct->get_value().ToIntDef(0); m; --m)
	{
		ct = ct->get_next();
		if(!ct)
		{
			error("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		if(ct->get_type() != nd_string)
		{
			error("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		name = ct->get_value();

		ct = ct->get_next();
		if(!ct)
		{
			error("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		if(ct->get_type() != nd_binary2)
		{
			error("Ошибка разбора файла configinfo"
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
			pcf = new container_file(tf, name);
			files[name.UpperCase()] = pcf;
		}
		else
		{
			error("Ошибка поиска файла"
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
String ConfigStorageTableConfigCas::presentation()
{
	return present;
}

//********************************************************
// Класс ConfigStorageTableConfigCasSave

//---------------------------------------------------------------------------
ConfigStorageTableConfigCasSave::ConfigStorageTableConfigCasSave(TableFiles* tabc, TableFiles* tabcs, const TGUID& uid, const String& configver, T_1CD* _base) : ConfigStorageTable(_base)
{
	int m;
	String s, name, hashname;
	table_file* _configinfo;
	container_file* configinfo;
	tree* tt;
	tree* ct;
	table_file* tf;
	container_file* pcf;
	TMemoryStream* stream;
	String g;
	int gl;
	std::map<String,table_file*>::iterator ptf;


	ready = tabc->getready();
	if(!ready) return;
	ready = tabcs->getready();
	if(!ready) return;

	configinfo = NULL;
	present = tabcs->gettable()->getbase()->getfilename() + "\\CONFIGCASSAVE";

	g = GUID_to_string(uid) + "__";
	gl = g.Length();
	for(ptf = tabcs->files().begin(); ptf != tabcs->files().end(); ++ptf)
	{
		if(ptf->first.SubString(1, gl).CompareIC(g) == 0)
		{
			tf = ptf->second;
			pcf = new container_file(tf, tf->name.SubString(gl + 1, tf->name.Length() - gl));
			if(pcf->name.CompareIC("configinfo") == 0) configinfo = pcf;
			files[pcf->name.UpperCase()] = pcf;
		}
	}

	if(!configinfo)
	{
		if(files.size())
		{
			error("Ошибка поиска файла"
				, "Путь", present
				, "Имя", g + "configinfo"
				, "Имя файла", "configinfo");
			return;
		}

		s = tabc->gettable()->getbase()->getfilename() + "\\CONFIGCAS\\" + configver;
		_configinfo = tabc->getfile(configver);
		if(!_configinfo)
		{
			error("Ошибка поиска файла"
				, "Путь", s
				, "Имя", configver
				, "Имя файла", "configinfo");
			return;
		}

		configinfo = new container_file(_configinfo, "configinfo");
		files["CONFIGINFO"] = configinfo;
	}
	else s = present + "\\" + gl + "configinfo";

	configinfo->open();
	tt = parse_1Cstream(configinfo->stream, s);
	if(!tt)
	{
		error("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = tt->get_first();
	if(!ct)
	{
		error("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = ct->get_next();
	if(!ct)
	{
		error("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = ct->get_next();
	if(!ct)
	{
		error("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}
	ct = ct->get_first();
	if(!ct)
	{
		error("Ошибка разбора файла configinfo"
			, "Путь", s);
		delete tt;
		return;
	}

	stream = new TMemoryStream;
	for(m = ct->get_value().ToIntDef(0); m; --m)
	{
		ct = ct->get_next();
		if(!ct)
		{
			error("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		if(ct->get_type() != nd_string)
		{
			error("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		name = ct->get_value();

		ct = ct->get_next();
		if(files.find(name.UpperCase()) != files.end()) continue;

		if(!ct)
		{
			error("Ошибка разбора файла configinfo"
				, "Путь", s);
			delete tt;
			delete stream;
			return;
		}
		if(ct->get_type() != nd_binary2)
		{
			error("Ошибка разбора файла configinfo"
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
			pcf = new container_file(tf, name);
			files[name.UpperCase()] = pcf;
		}
		else
		{
			error("Ошибка поиска файла"
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
String ConfigStorageTableConfigCasSave::presentation()
{
	return present;
}


//---------------------------------------------------------------------------

