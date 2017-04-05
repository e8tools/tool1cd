//---------------------------------------------------------------------------

#pragma hdrstop

#include "FileStorage.h"
//---------------------------------------------------------------------------
//#pragma package(smart_init)

//********************************************************
// Класс FileStorageStream

//---------------------------------------------------------------------------
__fastcall FileStorageStream::FileStorageStream(FileStorage* _s, TStream* _d, const String& _n)
{
	_data = _d;
	_storage = _s;
	_name = _n;
	_storage->openfiles[_name] = this;
}

//---------------------------------------------------------------------------
__fastcall FileStorageStream::~FileStorageStream()
{
	_storage->openfiles.erase(_name);
	delete _data;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс FileStorage

//---------------------------------------------------------------------------
__fastcall FileStorage::~FileStorage()
{
	while(openfiles.begin() != openfiles.end()) delete openfiles.begin()->second;
}

//---------------------------------------------------------------------------
String __fastcall FileStorage::normalizepath(const String& path)
{
	std::auto_ptr<TStringBuilder> b(new TStringBuilder(path.UpperCase()));
	b->Replace(L"/", L"\\");
	b->Replace(L"\\\\", L"\\");
	return b->ToString();
}

//---------------------------------------------------------------------------
FileStorageStream* __fastcall FileStorage::getstream(const String& path)
{
	std::map<String, FileStorageStream*>::iterator i;

	i = openfiles.find(normalizepath(path));
	if(i == openfiles.end()) return NULL;
	return i->second;

}

//---------------------------------------------------------------------------

//********************************************************
// Класс FileStorageDirectory

//---------------------------------------------------------------------------
__fastcall FileStorageDirectory::FileStorageDirectory(const String& _dir) : FileStorage()
{
	TSearchRec sr;
	int res;

	dir = _dir;
	if(*dir.LastChar() != L'\\') dir += L'\\';

	res = FindFirst(dir + L"*.*", faAnyFile, sr);
	while(res == 0)
	{
		if(sr.Name.Compare(L".") != 0 && sr.Name.Compare(L"..") != 0) names.push_back(sr.Name);
		res = FindNext(sr);
	}
	FindClose(sr);

}

//---------------------------------------------------------------------------
__fastcall FileStorageDirectory::~FileStorageDirectory()
{
}

//---------------------------------------------------------------------------
String __fastcall FileStorageDirectory::presentation()
{
	return dir.SubString(1, dir.Length() - 1);
}

//---------------------------------------------------------------------------
FileStorageStream* __fastcall FileStorageDirectory::open(const String& path)
{
	TFileStream* fs;
	FileStorageStream* fss;

	String normname = normalizepath(path);
	fss = getstream(normname);
	if(fss) return fss;

	String filename = dir + normname;

	if(FileExists(filename))
	{
		try
		{
			fs = new TFileStream(filename, fmOpenReadWrite);
		}
		catch(...)
		{
			// Здесь надо бы что-нибудь сообщить об ошибке
			return NULL;
		}

		return new FileStorageStream(this, fs, normname);
	}
	else return NULL;
}

//---------------------------------------------------------------------------
bool __fastcall FileStorageDirectory::write(const String& path, TStream* str)
{
	String filename = dir + normalizepath(path);
	try
	{
		TFileStream* f = new TFileStream(filename, fmCreate);
		f->CopyFrom(str, 0);
		delete f;
	}
	catch(...)
	{
		// Здесь надо бы что-нибудь сообщить об ошибке
		return false;
	}

	return true;
}

//---------------------------------------------------------------------------
bool __fastcall FileStorageDirectory::fileexists(const String& path)
{
	String filename;
	filename = dir + normalizepath(path);
	return FileExists(filename);
}

//---------------------------------------------------------------------------
String __fastcall FileStorageDirectory::getname(unsigned int index)
{
	if(index >= names.size()) return L"";
	return names[index];
}

//---------------------------------------------------------------------------

//********************************************************
// Класс FileStorageCFFile

//---------------------------------------------------------------------------
__fastcall FileStorageCFFile::FileStorageCFFile(const String& fname)
{
	v8file* f;

	filename = fname;
	cat = new v8catalog(filename);

	f = cat->GetFirst();
	while(f)
	{
		names.push_back(f->GetFileName());
		f = f->GetNext();
	}
}

//---------------------------------------------------------------------------
__fastcall FileStorageCFFile::~FileStorageCFFile()
{
	delete cat;
}

//---------------------------------------------------------------------------
String __fastcall FileStorageCFFile::presentation()
{
	return filename;
}

//---------------------------------------------------------------------------
FileStorageStream* __fastcall FileStorageCFFile::open(const String& path)
{
	v8catalog* c;
	v8file* f;
	int i;

	FileStorageStream* fss;

	String normname = normalizepath(path);
	fss = getstream(normname);
	if(fss) return fss;

	if(!cat->isOpen()) return NULL;

	//status(path);

	String fname = normname;
	c = cat;
	for(i = fname.Pos(L"\\"); i; i = fname.Pos(L"\\"))
	{
		f = c->GetFile(fname.SubString(1, i - 1));
		if(!f) return NULL;
		c = f->GetCatalog();
		if(!c) return NULL;
		fname = fname.SubString(i + 1, fname.Length() - i);
	}
	f = c->GetFile(fname);
	if(!f) return NULL;
	return new FileStorageStream(this, f->get_stream(true), normname);
}

//---------------------------------------------------------------------------
bool __fastcall FileStorageCFFile::write(const String& path, TStream* str)
{
	v8catalog* c;
	v8file* f;
	int i;

	if(!cat->isOpen()) return false;

	String fname = normalizepath(path);
	c = cat;
	for(i = fname.Pos(L"\\"); i; i = fname.Pos(L"\\"))
	{
		c = c->CreateCatalog(fname.SubString(1, i - 1));
		fname = fname.SubString(i + 1, fname.Length() - i);
	}
	f = c->createFile(fname);
	f->Write(str);
	return true;
}

//---------------------------------------------------------------------------
bool __fastcall FileStorageCFFile::fileexists(const String& path)
{
	// По сути, проверяется существование только каталога (файла верхнего уровня)
	// Это неправильно для формата 8.0 с файлом каталогом metadata. Но метод fileexists используется только для внешних файлов,
	// поэтому такой проверки достаточно

	v8file* f;
	int i;

	if(!cat->isOpen()) return false;

	String fname = normalizepath(path);
	i = fname.Pos(L"\\");
	if(i) fname = fname.SubString(1, i - 1);
	f = cat->GetFile(fname);
	if(!f) return false;
	return true;
}

//---------------------------------------------------------------------------
String __fastcall FileStorageCFFile::getname(unsigned int index)
{
	if(index >= names.size()) return L"";
	return names[index];
}

//---------------------------------------------------------------------------

//********************************************************
// Класс FileStorageTable

//---------------------------------------------------------------------------

//---------------------------------------------------------------------------

