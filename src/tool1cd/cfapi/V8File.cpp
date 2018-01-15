#include"APIcfBase.h"
#include "V8File.h"

using namespace System;

v8file::v8file(v8catalog* _parent, const String& _name, v8file* _previous, int _start_data, int _start_header, int64_t* _time_create, int64_t* _time_modify)
{
	Lock = new TCriticalSection();
	is_destructed = false;
	flushed  = false;
	parent   = _parent;
	name     = _name;
	previous = _previous;
	next     = nullptr;
	data     = nullptr;
	start_data        = _start_data;
	start_header      = _start_header;
	is_datamodified   = !start_data;
	is_headermodified = !start_header;
	if(previous)
		previous->next = this;
	else
		parent->first = this;
	iscatalog = FileIsCatalog::unknown;
	self = nullptr;
	is_opened = false;
	time_create = *_time_create;
	time_modify = *_time_modify;
	selfzipped = false;
	if(parent)
		parent->files[name.UpperCase()] = this;
}

//---------------------------------------------------------------------------
// получить время создания
void v8file::GetTimeCreate(FILETIME* ft)
{
	V8timeToFileTime(&time_create, ft);
}

//---------------------------------------------------------------------------
// получить время модификации
void v8file::GetTimeModify(FILETIME* ft)
{
	V8timeToFileTime(&time_modify, ft);
}

//---------------------------------------------------------------------------
// установить время создания
void v8file::SetTimeCreate(FILETIME* ft)
{
	FileTimeToV8time(ft, &time_create);
}

//---------------------------------------------------------------------------
// установить время модификации
void v8file::SetTimeModify(FILETIME* ft)
{
	FileTimeToV8time(ft, &time_modify);
}

//---------------------------------------------------------------------------
// сохранить в файл
void v8file::SaveToFile(const String& FileName)
{
	FILETIME create, modify;

#ifdef _MSC_VER

		struct _utimbuf ut;

#else

		struct utimbuf ut;

#endif // _MSC_VER

	if (!try_open()){
		return;
	}

	TFileStream* fs = new TFileStream(FileName, fmCreate);
	Lock->Acquire();
	fs->CopyFrom(data, 0);
	Lock->Release();

	GetTimeCreate(&create);
	GetTimeModify(&modify);

	time_t RawtimeCreate = FileTime_to_POSIX(&create);
	struct tm * ptm_create = localtime(&RawtimeCreate);
	ut.actime = mktime(ptm_create);

	time_t RawtimeModified = FileTime_to_POSIX(&create);
	struct tm * ptm_modified = localtime(&RawtimeModified);
	ut.modtime = mktime(ptm_modified);

	#ifdef _MSC_VER

		_utime(FileName.c_str(), &ut);

	#else

		utime(FileName.c_str(), &ut);

	#endif // _MSC_VER

	delete fs;
}

//---------------------------------------------------------------------------
// сохранить в поток
void v8file::SaveToStream(TStream* stream)
{
	Lock->Acquire();
	if (!try_open()) {
		return;
	}
	stream->CopyFrom(data, 0);
	Lock->Release();
}

int64_t v8file::GetFileLength()
{
	int64_t ret = 0l;
	Lock->Acquire();
	if (!try_open()) {
		return ret;
	}
	ret = data->GetSize();
	Lock->Release();
	return ret;
}

//---------------------------------------------------------------------------
// чтение
int64_t v8file::Read(void* Buffer, int Start, int Length)
{
	int64_t ret = 0l;
	Lock->Acquire();
	if (!try_open()){
		return ret;
	}
	data->Seek(Start, soFromBeginning);
	ret = data->Read(Buffer, Length);
	Lock->Release();
	return ret;
}

//---------------------------------------------------------------------------
// чтение
int64_t v8file::Read(std::vector<t::Byte> Buffer, int Start, int Length)
{
	int64_t ret = 0l;
	Lock->Acquire();
	if (!try_open()){
		return ret;
	}
	data->Seek(Start, soFromBeginning);
	ret = data->Read(Buffer, Length);
	Lock->Release();
	return ret;
}

//---------------------------------------------------------------------------
// получить поток
TV8FileStream* v8file::get_stream(bool own)
{
	return new TV8FileStream(this, own);
}

//---------------------------------------------------------------------------
// записать
int64_t v8file::Write(const void* Buffer, int Start, int Length) // дозапись/перезапись частично
{
	int64_t ret = 0l;
	Lock->Acquire();
	if (!try_open()){
		return ret;
	}
	setCurrentTime(&time_modify);
	is_headermodified = true;
	is_datamodified   = true;
	data->Seek(Start, soFromBeginning);
	ret = data->Write(Buffer, Length);
	Lock->Release();

	return ret;
}

//---------------------------------------------------------------------------
// записать
int64_t v8file::Write(std::vector<t::Byte> Buffer, int Start, int Length) // дозапись/перезапись частично
{
	int64_t ret = 0l;
	Lock->Acquire();
	if (!try_open()){
		return ret;
	}
	setCurrentTime(&time_modify);
	is_headermodified = true;
	is_datamodified   = true;
	data->Seek(Start, soFromBeginning);
	ret = data->Write(Buffer, Length);
	Lock->Release();

	return ret;
}

//---------------------------------------------------------------------------
// записать
int64_t v8file::Write(const void* Buffer, int Length) // перезапись целиком
{
	int64_t ret = 0l;
	Lock->Acquire();
	if (!try_open()) {
		return ret;
	}
	setCurrentTime(&time_modify);
	is_headermodified = true;
	is_datamodified = true;
	if (data->GetSize() > Length) data->SetSize(Length);
	data->Seek(0, soFromBeginning);
	ret = data->Write(Buffer, Length);
	Lock->Release();

	return ret;
}

//---------------------------------------------------------------------------
// записать
int64_t v8file::Write(TStream* Stream, int Start, int Length) // дозапись/перезапись частично
{
	int64_t ret = 0l;
	Lock->Acquire();
	if (!try_open()) {
		return ret;
	}
	setCurrentTime(&time_modify);
	is_headermodified = true;
	is_datamodified   = true;
	data->Seek(Start, soFromBeginning);
	ret = data->CopyFrom(Stream, Length);
	Lock->Release();

	return ret;
}

//---------------------------------------------------------------------------
// записать
int64_t v8file::Write(TStream* Stream) // перезапись целиком
{
	int64_t ret = 0l;
	Lock->Acquire();
	if (!try_open()) {
		return ret;
	}
	setCurrentTime(&time_modify);
	is_headermodified = true;
	is_datamodified   = true;
	if (data->GetSize() > Stream->GetSize()) data->SetSize(Stream->GetSize());
	data->Seek(0, soFromBeginning);
	ret = data->CopyFrom(Stream, 0);
	Lock->Release();

	return ret;
}


//---------------------------------------------------------------------------
// возвращает имя
String v8file::GetFileName()
{
	return name;
}

//---------------------------------------------------------------------------
// возвращает полное имя
String v8file::GetFullName()
{
	if(parent) if(parent->file)
	{
		String fulln = parent->file->GetFullName();
		if(!fulln.IsEmpty())
		{
			fulln += "\\";
			fulln += name;
			return fulln;
		}
	}
	return name;
}

//---------------------------------------------------------------------------
// устанавливает имя
void v8file::SetFileName(const String& _name)
{
	name = _name;
	is_headermodified = true;
}

//---------------------------------------------------------------------------
// определение "каталога"
bool v8file::IsCatalog()
{
	int64_t _filelen;
	uint32_t _startempty = (uint32_t)(-1);
	char _t[BLOCK_HEADER_LEN];

	Lock->Acquire();
	if(iscatalog == FileIsCatalog::unknown){
		// эмпирический метод?
		if (!try_open())
		{
			Lock->Release();
			return false;
		}
		_filelen = data->GetSize();
		if(_filelen == CATALOG_HEADER_LEN)
		{
			data->Seek(0, soFromBeginning);
			data->Read(_t, CATALOG_HEADER_LEN);
			if(memcmp(_t, _EMPTY_CATALOG_TEMPLATE, CATALOG_HEADER_LEN) != 0)
			{
				iscatalog = FileIsCatalog::no;
				Lock->Release();
				return false;
			}
			else
			{
				iscatalog = FileIsCatalog::yes;
				Lock->Release();
				return true;
			}
		}

		data->Seek(0, soFromBeginning);
		data->Read(&_startempty, 4);
		if(_startempty != LAST_BLOCK){
			if(_startempty + 31 >= _filelen){
				iscatalog = FileIsCatalog::no;
				Lock->Release();
				return false;
			}
			data->Seek(_startempty, soFromBeginning);
			data->Read(_t, 31);
			if(_t[0] != 0xd || _t[1] != 0xa || _t[10] != 0x20 || _t[19] != 0x20 || _t[28] != 0x20 || _t[29] != 0xd || _t[30] != 0xa){
				iscatalog = FileIsCatalog::no;
				Lock->Release();
				return false;
			}
		}
		if(_filelen < (BLOCK_HEADER_LEN - 1 + CATALOG_HEADER_LEN) ){
			iscatalog = FileIsCatalog::no;
			Lock->Release();
			return false;
		}
		data->Seek(CATALOG_HEADER_LEN, soFromBeginning);
		data->Read(_t, 31);
		if(_t[0] != 0xd || _t[1] != 0xa || _t[10] != 0x20 || _t[19] != 0x20 || _t[28] != 0x20 || _t[29] != 0xd || _t[30] != 0xa){
			iscatalog = FileIsCatalog::no;
			Lock->Release();
			return false;
		}
		iscatalog = FileIsCatalog::yes;
		Lock->Release();
		return true;
	}
	Lock->Release();
	return iscatalog == FileIsCatalog::yes;
}

//---------------------------------------------------------------------------
// получение "каталога"
v8catalog* v8file::GetCatalog(){
	v8catalog* ret;

	Lock->Acquire();
	if(IsCatalog())
	{
		if(!self)
		{
			self = new v8catalog(this);
		}
		ret = self;
	}
	else ret = nullptr;
	Lock->Release();
	return ret;
}

//---------------------------------------------------------------------------
// получение родительского контейнера
v8catalog* v8file::GetParentCatalog()
{
	return parent;
}

//---------------------------------------------------------------------------
// удалить файл
void v8file::DeleteFile()
{
	Lock->Acquire();
	if(parent)
	{
		parent->Lock->Acquire();
		if(next)
		{
			next->Lock->Acquire();
			next->previous = previous;
			next->Lock->Release();
		}
		else parent->last = previous;
		if(previous)
		{
			previous->Lock->Acquire();
			previous->next = next;
			previous->Lock->Release();
		}
		else parent->first = next;
		parent->is_fatmodified = true;
		parent->free_block(start_data);
		parent->free_block(start_header);
		parent->files.erase(name.UpperCase());
		parent->Lock->Release();
		parent = nullptr;
	}
	delete data;
	data = nullptr;
	if(self)
	{
		self->data = nullptr;
		delete self;
		self = nullptr;
	}
	iscatalog = FileIsCatalog::no;
	next = nullptr;
	previous = nullptr;
	is_opened = false;
	start_data = 0;
	start_header = 0;
	is_datamodified = false;
	is_headermodified = false;
}

//---------------------------------------------------------------------------
// получить следующий
v8file* v8file::GetNext()
{
	return next;
}

//---------------------------------------------------------------------------
// открыть файл
bool v8file::Open(){
	if(!parent) return false;
	Lock->Acquire();
	if(is_opened)
	{
		Lock->Release();
		return true;
	}
	data = parent->read_datablock(start_data);
	is_opened = true;
	Lock->Release();
	return true;
}

//---------------------------------------------------------------------------
// закрыть файл
void v8file::Close(){
	int _t = 0;

	if(!parent) return;
	Lock->Acquire();
	if(!is_opened) return;

	if(self) if(!self->is_destructed)
	{
		delete self;
	}
	self = nullptr;

	if(parent->data)
	{
		if(is_datamodified || is_headermodified)
		{
			parent->Lock->Acquire();
			if(is_datamodified)
			{
				start_data = parent->write_datablock(data, start_data, selfzipped);
			}
			if(is_headermodified)
			{
				TMemoryStream* hs = new TMemoryStream();
				hs->Write(&time_create, 8);
				hs->Write(&time_modify, 8);
				hs->Write(&_t, 4);
				#ifndef _DELPHI_STRING_UNICODE // FIXME: определится используем WCHART или char
				int ws = name.WideCharBufSize();
				char* tb = new char[ws];
				name.WideChar((WCHART*)tb, ws);
				hs->Write((char*)tb, ws);
				delete[] tb;
				#else
				hs->Write(name.c_str(), name.Length() * 2);
				#endif
				hs->Write(&_t, 4);

				start_header = parent->write_block(hs, start_header, false);
				delete hs;
			}
			parent->Lock->Release();
		}
	}
	delete data;
	data = nullptr;
	iscatalog = FileIsCatalog::unknown;
	is_opened = false;
	is_datamodified = false;
	is_headermodified = false;
	Lock->Release();
}

//---------------------------------------------------------------------------
// записать и закрыть
int64_t v8file::WriteAndClose(TStream* Stream, int Length)
{
	int32_t _4bzero = 0;

	Lock->Acquire();
	if (!try_open())
	{
		Lock->Release();
		return 0;
	}

	if (!parent)
	{
		Lock->Release();
		return 0;
	}

	if (self) delete self;
	self = nullptr;

	delete data;
	data = nullptr;

	if (parent->data)
	{
		int name_size = name.WideCharBufSize();
		WCHART *wname = new WCHART[name_size];
		name.WideChar(wname, name.Length());

		parent->Lock->Acquire();
		start_data = parent->write_datablock(Stream, start_data, selfzipped, Length);
		TMemoryStream hs;
		hs.Write(&time_create, 8);
		hs.Write(&time_modify, 8);
		hs.Write(&_4bzero, 4);
		hs.Write(wname, name.Length() * sizeof(WCHART));
		hs.Write(&_4bzero, 4);
		start_header = parent->write_block(&hs, start_header, false);
		parent->Lock->Release();
		delete[]wname;
	}
	iscatalog = FileIsCatalog::unknown;
	is_opened = false;
	is_datamodified = false;
	is_headermodified = false;
	Lock->Release();

	if (Length == -1) return Stream->GetSize();
	return Length;
}

//---------------------------------------------------------------------------
// деструктор
v8file::~v8file()
{
	std::set<TV8FileStream*>::iterator istreams;

	Lock->Acquire();
	is_destructed = true;
	for(istreams = streams.begin(); istreams != streams.end(); ++istreams) delete *istreams;
	streams.clear();

	Close();

	if(parent)
	{
		if(next)
		{
			next->Lock->Acquire();
			next->previous = previous;
			next->Lock->Release();
		}
		else
		{
			parent->Lock->Acquire();
			parent->last = previous;
			parent->Lock->Release();
		}
		if(previous)
		{
			previous->Lock->Acquire();
			previous->next = next;
			previous->Lock->Release();
		}
		else
		{
			parent->Lock->Acquire();
			parent->first = next;
			parent->Lock->Release();
		}
	}
	delete Lock;
}

//---------------------------------------------------------------------------
// сброс
void v8file::Flush()
{
	int _t = 0;

	Lock->Acquire();
	if(flushed)
	{
		Lock->Release();
		return;
	}
	if(!parent)
	{
		Lock->Release();
		return;
	}
	if(!is_opened)
	{
		Lock->Release();
		return;
	}

	flushed = true;
	if(self) self->Flush();

	if(parent->data)
	{
		if(is_datamodified || is_headermodified)
		{
			parent->Lock->Acquire();
			if(is_datamodified)
			{
				start_data = parent->write_datablock(data, start_data, selfzipped);
				is_datamodified = false;
			}
			if(is_headermodified)
			{
				TMemoryStream* hs = new TMemoryStream();
				hs->Write(&time_create, 8);
				hs->Write(&time_modify, 8);
				hs->Write(&_t, 4);
				#ifndef _DELPHI_STRING_UNICODE
				int ws = name.WideCharBufSize();
				char* tb = new char[ws];
				name.WideChar((WCHART*)tb, ws);
				hs->Write((char*)tb, ws);
				delete[] tb;
				#else
				hs->Write(name.c_str(), name.Length() * 2);
				#endif
				hs->Write(&_t, 4);

				start_header = parent->write_block(hs, start_header, false);
				delete hs;
				is_headermodified = false;
			}
			parent->Lock->Release();
		}
	}
	flushed = false;
	Lock->Release();
}
