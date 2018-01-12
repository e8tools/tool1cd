#include <algorithm>
#include <iterator>
#include <sstream>
#include <iomanip>
#include <array>
#include <stdexcept>

#include "APIcfBase.h"
#include "Common.h"

#ifdef _MSC_VER

	#include <sys/utime.h>

#else

	#include <sys/types.h>
	#include <utime.h>

#endif // _MSC_VER

using namespace System;

#pragma comment (lib, "zlibstatic.lib")

constexpr uint32_t HEX_INT_LEN = sizeof(int32_t) * 2;

enum class block_header: int {
	doc_len = 2,
	block_len = 11,
	nextblock = 20
};

//---------------------------------------------------------------------------
// возвращает секунды от эпохи UNIX
// https://stackoverflow.com/questions/6161776/convert-windows-filetime-to-second-in-unix-linux
//
unsigned WindowsTickToUnixSeconds(long long windowsTicks)
{
	return (unsigned)(windowsTicks / WINDOWS_TICK - SEC_TO_UNIX_EPOCH);
}

//---------------------------------------------------------------------------
// читает блок из потока каталога stream_from, собирая его по страницам
TStream* read_block(TStream* stream_from, int start, TStream* stream_to = nullptr)
{
	std::array<char, BLOCK_HEADER_LEN> temp_buf;
	int len, curlen, pos, readlen;

	if(!stream_to)
		stream_to = new TMemoryStream;
	stream_to->Seek(0, soFromBeginning);
	stream_to->SetSize(0);

	if(start < 0 || start == LAST_BLOCK || start > stream_from->GetSize())
		return stream_to;

	stream_from->Seek(start, soFromBeginning);
	stream_from->Read(temp_buf.data(), temp_buf.size() - 1);

	std::string hex_len("0x");
	std::copy_n(temp_buf.begin() + (int)block_header::doc_len,
				HEX_INT_LEN,
				std::back_inserter(hex_len));
	len = std::stoi(hex_len, nullptr, 16);

	if(!len)
		return stream_to;

	std::string hex_curlen("0x");
	std::copy_n(temp_buf.begin() + (int)block_header::block_len,
				HEX_INT_LEN,
				std::back_inserter(hex_curlen));
	curlen =std::stoi(hex_curlen, nullptr, 16);

	std::string hex_start("0x");
	std::copy_n(temp_buf.begin() + (int)block_header::nextblock,
				HEX_INT_LEN,
				std::back_inserter(hex_start));
	start  = std::stoi(hex_start, nullptr, 16);

	readlen = std::min(len, curlen);
	stream_to->CopyFrom(stream_from, readlen);

	pos = readlen;

	while(start != LAST_BLOCK){

		stream_from->Seek(start, soFromBeginning);
		stream_from->Read(temp_buf.data(), temp_buf.size() - 1);

		std::string hex_curlen("0x");
		std::copy_n(temp_buf.begin() + (int)block_header::block_len,
					HEX_INT_LEN,
					std::back_inserter(hex_curlen));
		curlen = std::stoi(hex_curlen, nullptr, 16);

		std::string hex_start("0x");
		std::copy_n(temp_buf.begin() + (int)block_header::nextblock,
					HEX_INT_LEN,
					std::back_inserter(hex_start));
		start  = std::stoi(hex_start, nullptr, 16);

		readlen = std::min(len - pos, curlen);
		stream_to->CopyFrom(stream_from, readlen);
		pos += readlen;

	}

	return stream_to;

}

//---------------------------------------------------------------------------
//преобразование времени
void V8timeToFileTime(const int64_t* v8t, FILETIME* ft){

	FILETIME lft;

	int64_t t = *v8t;
	t -= EPOCH_START_WIN; //504911232000000 = ((365 * 4 + 1) * 100 - 3) * 4 * 24 * 60 * 60 * 10000
	t *= 1000;
	*(int64_t*)&lft = t;
	LocalFileTimeToFileTime(&lft, ft);

}

//---------------------------------------------------------------------------
// обратное преобразование времени
void FileTimeToV8time(const FILETIME* ft, int64_t* v8t){

	FILETIME lft;

	FileTimeToLocalFileTime(ft, &lft);
	int64_t t = *(int64_t*)&lft;
	t /= 1000;
	t += EPOCH_START_WIN; //504911232000000 = ((365 * 4 + 1) * 100 - 3) * 4 * 24 * 60 * 60 * 10000
	*v8t = t;
}

//---------------------------------------------------------------------------
// установка текущего времени
void setCurrentTime(int64_t* v8t)
{
	SYSTEMTIME st;
	FILETIME ft;

	GetSystemTime(&st);
	SystemTimeToFileTime(&st, &ft);
	FileTimeToV8time(&ft, v8t);

}

//---------------------------------------------------------------------------

//********************************************************
// Класс v8file

//---------------------------------------------------------------------------
// конструктор
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

//********************************************************
// Класс v8catalog

//---------------------------------------------------------------------------
// определение каталога
bool v8catalog::IsCatalog()
{
	int64_t _filelen;
	uint32_t _startempty = (uint32_t)(-1);
	char _t[BLOCK_HEADER_LEN];

	Lock->Acquire();
	if(iscatalogdefined)
	{
		Lock->Release();
		return iscatalog;
	}
	iscatalogdefined = true;
	iscatalog = false;

	// эмпирический метод?
	_filelen = data->GetSize();
	if(_filelen == CATALOG_HEADER_LEN)
	{
		data->Seek(0, soFromBeginning);
		data->Read(_t, CATALOG_HEADER_LEN);
		if(memcmp(_t, _EMPTY_CATALOG_TEMPLATE, CATALOG_HEADER_LEN) != 0)
		{
			Lock->Release();
			return false;
		}
		else
		{
			iscatalog = true;
			Lock->Release();
			return true;
		}
	}

	data->Seek(0, soFromBeginning);
	data->Read(&_startempty, 4);
	if(_startempty != LAST_BLOCK){
		if(_startempty + 31 >= _filelen)
		{
			Lock->Release();
			return false;
		}
		data->Seek(_startempty, soFromBeginning);
		data->Read(_t, 31);
		if(_t[0] != 0xd || _t[1] != 0xa || _t[10] != 0x20 || _t[19] != 0x20 || _t[28] != 0x20 || _t[29] != 0xd || _t[30] != 0xa)
		{
			Lock->Release();
			return false;
		}
	}
	if(_filelen < (BLOCK_HEADER_LEN - 1 + CATALOG_HEADER_LEN) )
	{
		Lock->Release();
		return false;
	}
	data->Seek(CATALOG_HEADER_LEN, soFromBeginning);
	data->Read(_t, 31);
	if(_t[0] != 0xd || _t[1] != 0xa || _t[10] != 0x20 || _t[19] != 0x20 || _t[28] != 0x20 || _t[29] != 0xd || _t[30] != 0xa)
	{
		Lock->Release();
		return false;
	}
	iscatalog = true;
	Lock->Release();
	return true;
}

//---------------------------------------------------------------------------
// конструктор
v8catalog::v8catalog(String name) // создать каталог из физического файла .cf
{
	Lock = new TCriticalSection();
	iscatalogdefined = false;

	String ext = ExtractFileExt(name).LowerCase();
	if(ext == str_cfu)
	{
		is_cfu = true;
		zipped = false;
		data = new TMemoryStream();

		if(!FileExists(name))
		{
			data->WriteBuffer(_EMPTY_CATALOG_TEMPLATE, CATALOG_HEADER_LEN);
			cfu = new TFileStream(name, fmCreate);
		}
		else
		{
			cfu = new TFileStream(name, fmOpenReadWrite);
			ZInflateStream(cfu, data);
		}
	}
	else
	{
		zipped = ext == str_cf || ext == str_epf || ext == str_erf || ext == str_cfe;
		is_cfu = false;

		if(!FileExists(name))
		{
			data = new TFileStream(name, fmCreate);
			data->WriteBuffer(_EMPTY_CATALOG_TEMPLATE, CATALOG_HEADER_LEN);
			delete data;
		}
		data = new TFileStream(name, fmOpenReadWrite);
	}

	file = nullptr;
	if(IsCatalog()) initialize();
	else
	{
		first = nullptr;
		last = nullptr;
		start_empty = 0;
		page_size = 0;
		version = 0;
		zipped = false;

		is_fatmodified = false;
		is_emptymodified = false;
		is_modified = false;
		is_destructed = false;
		flushed = false;
		leave_data = false;
	}
}

//---------------------------------------------------------------------------
// конструктор
v8catalog::v8catalog(String name, bool _zipped) // создать каталог из физического файла
{
	Lock = new TCriticalSection();
	iscatalogdefined = false;
	is_cfu = false;
	zipped = _zipped;

	if(!FileExists(name))
	{
		data = new TFileStream(name, fmCreate);
		data->WriteBuffer(_EMPTY_CATALOG_TEMPLATE, CATALOG_HEADER_LEN);
		delete data;
	}
	data = new TFileStream(name, fmOpenReadWrite);
	file = nullptr;
	if(IsCatalog()) initialize();
	else
	{
		first = nullptr;
		last = nullptr;
		start_empty = 0;
		page_size = 0;
		version = 0;
		zipped = false;

		is_fatmodified = false;
		is_emptymodified = false;
		is_modified = false;
		is_destructed = false;
		flushed = false;
		leave_data = false;
	}
}

//---------------------------------------------------------------------------
// конструктор
v8catalog::v8catalog(TStream* stream, bool _zipped, bool leave_stream) // создать каталог из потока
{
	Lock = new TCriticalSection();
	is_cfu = false;
	iscatalogdefined = false;
	zipped = _zipped;
	data = stream;
	file = nullptr;

	if(!data->GetSize())
		data->WriteBuffer(_EMPTY_CATALOG_TEMPLATE, CATALOG_HEADER_LEN);

	if(IsCatalog())
		initialize();
	else
	{
		first = nullptr;
		last = nullptr;
		start_empty = 0;
		page_size = 0;
		version = 0;
		zipped = false;

		is_fatmodified = false;
		is_emptymodified = false;
		is_modified = false;
		is_destructed = false;
		flushed = false;
	}
	leave_data = leave_stream;
}

//---------------------------------------------------------------------------
// конструктор
v8catalog::v8catalog(v8file* f) // создать каталог из файла
{
	is_cfu = false;
	iscatalogdefined = false;
	file = f;
	Lock = file->Lock;
	Lock->Acquire();
	file->Open();
	data = file->data;
	zipped = false;

	if(IsCatalog()) initialize();
	else
	{
		first = nullptr;
		last = nullptr;
		start_empty = 0;
		page_size = 0;
		version = 0;
		zipped = false;

		is_fatmodified = false;
		is_emptymodified = false;
		is_modified = false;
		is_destructed = false;
		flushed = false;
		leave_data = false;
	}
	Lock->Release();
}

//---------------------------------------------------------------------------
void v8catalog::initialize()
{
	is_destructed = false;
	catalog_header _ch;
	String _name;
	fat_item _fi;
	char* _temp_buf;
	TMemoryStream* _file_header;
	TStream* _fat;
	v8file* _prev;
	v8file* _file;
	v8file* f;
	int64_t _countfiles;

	data->Seek(0, soFromBeginning);
	data->ReadBuffer(&_ch, CATALOG_HEADER_LEN);
	start_empty = _ch.start_empty;
	page_size = _ch.page_size;
	version = _ch.version;

	first = nullptr;

	_file_header = new TMemoryStream;
	_prev = nullptr;
	try
	{
		if(data->GetSize() > CATALOG_HEADER_LEN)
		{
			_fat = read_block(data, CATALOG_HEADER_LEN);
			_fat->Seek(0, soFromBeginning);
			_countfiles = _fat->GetSize() / 12;
			for(int i = 0; i < _countfiles; i++){
				_fat->Read(&_fi, 12);
				read_block(data, _fi.header_start, _file_header);
				_file_header->Seek(0, soFromBeginning);
				_temp_buf = new char[_file_header->GetSize()];
				_file_header->Read(_temp_buf, _file_header->GetSize());
				_name = (WCHART*)(_temp_buf + 20);
				_file = new v8file(this, _name, _prev, _fi.data_start, _fi.header_start, (int64_t*)_temp_buf, (int64_t*)(_temp_buf + 8));
				delete[] _temp_buf;
				if(!_prev) first = _file;
				_prev = _file;
			}
			delete _file_header;
			delete _fat;
		}
	}
	catch(...)
	{
		f = first;
		while(f)
		{
			f->Close();
			f = f->next;
		}
		while(first) delete first;

		iscatalog = false;
		iscatalogdefined = true;

		first = nullptr;
		last = nullptr;
		start_empty = 0;
		page_size = 0;
		version = 0;
		zipped = false;

	}

	last = _prev;

	is_fatmodified = false;
	is_emptymodified = false;
	is_modified = false;
	is_destructed = false;
	flushed = false;
	leave_data = false;
}

//---------------------------------------------------------------------------
// удалить файл
void v8catalog::DeleteFile(const String& FileName)
{
	Lock->Acquire();
	v8file* f = first;
	while(f)
	{
		if(!f->name.CompareIC(FileName))
		{
			f->DeleteFile();
			delete f;
		}
		f = f->next;
	}
	Lock->Release();
}

//---------------------------------------------------------------------------
// получить файл
v8file* v8catalog::GetFile(const String& FileName)
{
	v8file* ret;
	Lock->Acquire();
	std::map<String,v8file*>::const_iterator it;
	it = files.find(FileName.UpperCase());
	if(it == files.end()) ret = nullptr;
	else ret = it->second;
	Lock->Release();
	return ret;
}

//---------------------------------------------------------------------------
// получить первого
v8file* v8catalog::GetFirst(){
	return first;
}

//---------------------------------------------------------------------------
// создать файл
v8file* v8catalog::createFile(const String& FileName, bool _selfzipped){
	int64_t v8t;
	v8file* f;

	Lock->Acquire();
	f = GetFile(FileName);
	if(!f)
	{
		setCurrentTime(&v8t);
		f = new v8file(this, FileName, last, 0, 0, &v8t, &v8t);
		f->selfzipped = _selfzipped;
		last = f;
		is_fatmodified = true;
	}
	Lock->Release();
	return f;
}

//---------------------------------------------------------------------------
// получить родительский каталог
v8catalog* v8catalog::GetParentCatalog()
{
	if(!file) return nullptr;
	return file->parent;
}

//---------------------------------------------------------------------------
// чтение блока данных
TStream* v8catalog::read_datablock(int start)
{
	TStream* stream;
	TStream* stream2;
	if(!start) return new TMemoryStream;
	Lock->Acquire();
	stream = read_block(data, start);
	Lock->Release();

	if(zipped)
	{
		stream2 = new TMemoryStream;
		stream->Seek(0, soFromBeginning);
		ZInflateStream(stream, stream2);
		delete stream;
	}
	else stream2 = stream;

	return stream2;
}

//---------------------------------------------------------------------------
// освобождение блока
void v8catalog::free_block(int start){
	std::array<char, BLOCK_HEADER_LEN> temp_buf;
	int nextstart;
	int prevempty;

	if(!start) return;
	if(start == LAST_BLOCK) return;

	Lock->Acquire();
	prevempty = start_empty;
	start_empty = start;

	do
	{
		data->Seek(start, soFromBeginning);
		data->ReadBuffer(temp_buf.data(), temp_buf.size() - 1);

		std::string hex_nextstart("0x");
		std::copy_n(temp_buf.begin() + (int)block_header::nextblock,
					HEX_INT_LEN,
					std::back_inserter(hex_nextstart));
		nextstart = std::stoi(hex_nextstart, nullptr, 16);

		std::string hex_int = to_hex_string(LAST_BLOCK, false);
		std::copy(hex_int.begin(),
				  hex_int.end(),
				  temp_buf.begin() + (int)block_header::doc_len);
		if (nextstart == LAST_BLOCK) {
			std::string hex_prevempty = to_hex_string(prevempty, false);
			std::copy(hex_prevempty.begin(),
					  hex_prevempty.end(),
					  temp_buf.begin() + (int)block_header::nextblock);
		}
		data->Seek(start, soFromBeginning);
		data->WriteBuffer(temp_buf.data(), temp_buf.size() - 1);
		start = nextstart;
	}
	while(start != LAST_BLOCK);

	is_emptymodified = true;
	is_modified = true;
	Lock->Release();
}

//---------------------------------------------------------------------------
// запись блока данных
int v8catalog::write_datablock(TStream* block, int start, bool _zipped, int len)
{
	TMemoryStream* stream2;
	TMemoryStream* stream;
	int ret;

	if(zipped || _zipped)
	{
		if(len == -1)
		{
			stream2 = new TMemoryStream;
			block->Seek(0, soFromBeginning);
			ZDeflateStream(block, stream2);
			Lock->Acquire();
			start = write_block(stream2, start, false);
			ret = start;
			Lock->Release();
			delete stream2;
		}
		else
		{
			stream = new TMemoryStream;
			stream->CopyFrom(block, len);
			stream2 = new TMemoryStream;
			stream->Seek(0, soFromBeginning);
			ZDeflateStream(stream, stream2);
			delete stream;
			Lock->Acquire();
			start = write_block(stream2, start, false);
			ret = start;
			Lock->Release();
			delete stream2;
		}
	}
	else
	{
		Lock->Acquire();
		start = write_block(block, start, false, len);
		ret = start;
		Lock->Release();
	}
	return ret;
}

//---------------------------------------------------------------------------
// получить следующий блок
int64_t v8catalog::get_nextblock(int64_t start)
{
	int64_t ret;

	Lock->Acquire();
	if(start == 0 || start == LAST_BLOCK)
	{
		start = start_empty;
		if(start == LAST_BLOCK) start = data->GetSize();
	}
	ret = start;
	Lock->Release();
	return ret;
}

//---------------------------------------------------------------------------
// записать блок
int v8catalog::write_block(TStream* block, int start, bool use_page_size, int len)
{
	std::array<char, BLOCK_HEADER_LEN> temp_buf;
	char* _t;
	int firststart, nextstart, blocklen, curlen;
	bool isfirstblock = true;
	bool addwrite = false; // признак, что надо дозаписать файл при использовании размера страницы по умолчанию

	Lock->Acquire();
	if(data->GetSize() == CATALOG_HEADER_LEN && start != CATALOG_HEADER_LEN) // если каталог пустой, надо выделить первую страницу!!!
	{
		TMemoryStream* _ts = new TMemoryStream;
		write_block(_ts, CATALOG_HEADER_LEN, true);
	}

	if(len == -1)
	{
		len = block->GetSize();
		block->Seek(0, soFromBeginning);
	}
	start = get_nextblock(start);

	do
	{
		if(start == start_empty)
		{// пишем в свободный блок
			data->Seek(start, soFromBeginning);
			data->ReadBuffer(temp_buf.data(), temp_buf.size() - 1);

			std::string hex_blocklen("0x");
			std::copy_n(temp_buf.begin() + (int)block_header::block_len,
						HEX_INT_LEN,
						std::back_inserter(hex_blocklen));
			blocklen = std::stoi(hex_blocklen, nullptr, 16);

			std::string hex_nextstart("0x");
			std::copy_n(temp_buf.begin() + (int)block_header::nextblock,
						HEX_INT_LEN,
						std::back_inserter(hex_nextstart));
			nextstart = std::stoi(hex_nextstart, nullptr, 16);

			start_empty = nextstart;
			is_emptymodified = true;
		}
		else if(start == data->GetSize())
		{// пишем в новый блок
			memcpy(temp_buf.data(), _BLOCK_HEADER_TEMPLATE, temp_buf.size() - 1);
			blocklen = use_page_size ? len > page_size ? len : page_size : len;
			std::string hex_blocklen = to_hex_string(blocklen, false);
			std::copy(hex_blocklen.begin(),
					  hex_blocklen.end(),
					  temp_buf.begin() + (int)block_header::block_len);
			nextstart = 0;
			if(blocklen > len) addwrite = true;
		}
		else
		{// пишем в существующий блок
			data->Seek(start, soFromBeginning);
			data->ReadBuffer(temp_buf.data(), temp_buf.size() - 1);

			std::string hex_blocklen("0x");
			std::copy_n(temp_buf.begin() + (int)block_header::block_len,
						HEX_INT_LEN,
						std::back_inserter(hex_blocklen));
			blocklen = std::stoi(hex_blocklen, nullptr, 16);

			std::string hex_nextstart("0x");
			std::copy_n(temp_buf.begin() + (int)block_header::nextblock,
						HEX_INT_LEN,
						std::back_inserter(hex_nextstart));
			nextstart = std::stoi(hex_nextstart, nullptr, 16);
		}

		std::string hex_first_block_len = to_hex_string(isfirstblock ? len : 0, false);
		std::copy(hex_first_block_len.begin(),
				  hex_first_block_len.end(),
				  temp_buf.begin() + (int)block_header::doc_len);
		curlen = std::min(blocklen, len);
		if(!nextstart) nextstart = data->GetSize() + 31 + blocklen;
		else nextstart = get_nextblock(nextstart);

		std::string hex_block = to_hex_string(len <= blocklen ? LAST_BLOCK : nextstart, false);
		std::copy(hex_block.begin(),
				  hex_block.end(),
				  temp_buf.begin() + (int)block_header::nextblock);

		data->Seek(start, soFromBeginning);
		data->WriteBuffer(temp_buf.data(), temp_buf.size() - 1);
		data->CopyFrom(block, curlen);
		if(addwrite)
		{
			_t = new char [blocklen - len];
			memset(_t, 0, blocklen - len);
			data->WriteBuffer(_t, blocklen - len);
			addwrite = false;
		}

		len -= curlen;

		if(isfirstblock)
		{
			firststart = start;
			isfirstblock = false;
		}
		start = nextstart;

	}while(len > 0);

	if(start < data->GetSize() && start != start_empty) free_block(start);

	is_modified = true;
	Lock->Release();
	return firststart;
}

//---------------------------------------------------------------------------
// деструктор
v8catalog::~v8catalog()
{
	fat_item fi;
	v8file* f;
	TMemoryStream* fat = nullptr;

	Lock->Acquire();
	is_destructed = true;

	f = first;
	while(f)
	{
		f->Close();
		f = f->next;
	}

	if(data)
	{
		if(is_fatmodified)
		{
			try
			{
				fat = new TMemoryStream;
				fi.ff = LAST_BLOCK;
				f = first;
				while(f)
				{
					fi.header_start = f->start_header;
					fi.data_start = f->start_data;
					fat->WriteBuffer(&fi, 12);
					f = f->next;
				}
				write_block(fat, CATALOG_HEADER_LEN, true);
			}
			catch(...)
			{
			}
			delete fat;
		}
	}

	while(first) delete first;

	if(data)
	{
		if(is_emptymodified)
		{
			data->Seek(0, soFromBeginning);
			data->WriteBuffer(&start_empty, 4);
		}
		if(is_modified)
		{
			version++;
			data->Seek(8, soFromBeginning);
			data->WriteBuffer(&version, 4);
		}
	}

	if(file){
		if(is_modified)
		{
			file->is_datamodified = true;
		}
		if(!file->is_destructed) file->Close();
	}
	else
	{
		if(is_cfu)
		{
			if(data && cfu && is_modified)
			{
				data->Seek(0, soFromBeginning);
				cfu->Seek(0, soFromBeginning);
				ZDeflateStream(data, cfu);
			}
			delete data;
			data = nullptr;
			if(cfu && !leave_data){
				delete cfu;
				cfu = nullptr;
			}
		}
		if(data && !leave_data)
		{
			delete data;
			data = nullptr;
		}

	}
	if(!file) delete Lock;
}

//---------------------------------------------------------------------------
// получить файл собственный
v8file* v8catalog::GetSelfFile()
{
	return file;
}

//---------------------------------------------------------------------------
// создать каталог
v8catalog* v8catalog::CreateCatalog(const String& FileName, bool _selfzipped)
{
	v8catalog* ret;
	Lock->Acquire();
	v8file* f = createFile(FileName, _selfzipped);
	if(f->GetFileLength() > 0)
	{
		if(f->IsCatalog()) ret = f->GetCatalog();
		else ret = nullptr;
	}
	else
	{
		f->Write(_EMPTY_CATALOG_TEMPLATE, CATALOG_HEADER_LEN);
		ret = f->GetCatalog();
	}
	Lock->Release();
	return ret;
}

//---------------------------------------------------------------------------
// сохранить в файловую систему
void v8catalog::SaveToDir(String DirName)
{
	CreateDir(DirName);
	if(DirName.SubString(DirName.Length(), 1) != str_backslash) DirName += str_backslash;
	Lock->Acquire();
	v8file* f = first;
	while(f)
	{
		if(f->IsCatalog()) f->GetCatalog()->SaveToDir(DirName + f->name);
		else f->SaveToFile(DirName + f->name);
		f->Close();
		f = f->next;
	}
	Lock->Release();
}

//---------------------------------------------------------------------------
// возвращает признак открытости
bool v8catalog::isOpen()
{
	return IsCatalog();
}

//---------------------------------------------------------------------------
// сбросить
void v8catalog::Flush()
{
	fat_item fi;
	v8file* f;

	Lock->Acquire();
	if(flushed)
	{
		Lock->Release();
		return;
	}
	flushed = true;

	f = first;
	while(f)
	{
		f->Flush();
		f = f->next;
	}

	if(data)
	{
		if(is_fatmodified)
		{
			TMemoryStream* fat = new TMemoryStream;
			fi.ff = LAST_BLOCK;
			f = first;
			while(f)
			{
				fi.header_start = f->start_header;
				fi.data_start = f->start_data;
				fat->WriteBuffer(&fi, 12);
				f = f->next;
			}
			write_block(fat, CATALOG_HEADER_LEN, true);
			is_fatmodified = false;
		}

		if(is_emptymodified)
		{
			data->Seek(0, soFromBeginning);
			data->WriteBuffer(&start_empty, 4);
			is_emptymodified = false;
		}
		if(is_modified)
		{
			version++;
			data->Seek(8, soFromBeginning);
			data->WriteBuffer(&version, 4);
		}
	}

	if(file){
		if(is_modified)
		{
			file->is_datamodified = true;
		}
		file->Flush();
	}
	else
	{
		if(is_cfu)
		{
			if(data && cfu && is_modified)
			{
				data->Seek(0, soFromBeginning);
				cfu->Seek(0, soFromBeginning);
				ZDeflateStream(data, cfu);
			}
		}
	}

	is_modified = false;
	flushed = false;
	Lock->Release();
}

//---------------------------------------------------------------------------
// закрыть наполовину
void v8catalog::HalfClose()
{
	Lock->Acquire();
	Flush();
	if(is_cfu)
	{
		delete cfu;
		cfu = nullptr;
	}
	else
	{
		delete data;
		data = nullptr;
	}
	Lock->Release();
}

//---------------------------------------------------------------------------
// половину открыть
void v8catalog::HalfOpen(const String& name)
{
	Lock->Acquire();
	if(is_cfu) cfu = new TFileStream(name, fmOpenReadWrite);
	else data = new TFileStream(name, fmOpenReadWrite);
	Lock->Release();
}


//********************************************************
// Класс TV8FileStream

//---------------------------------------------------------------------------
// конструктор
TV8FileStream::TV8FileStream(v8file* f, bool ownfile) : TStream(), file(f), own(ownfile)
{
	pos = 0l;
	file->streams.insert(this);
}

//---------------------------------------------------------------------------
// декструктор
TV8FileStream::~TV8FileStream()
{
	if(own) delete file;
	else file->streams.erase(this);
}

//---------------------------------------------------------------------------
// чтение буфера
int64_t TV8FileStream::Read(void *Buffer, int64_t Count)
{
	int r = file->Read(Buffer, pos, Count);
	pos += r;
	return r;
}

//---------------------------------------------------------------------------
// чтение буфера
int TV8FileStream::Read(std::vector<t::Byte> Buffer, int Offset, int Count)
{
	int r = file->Read(Buffer, pos, Count);
	pos += r;
	return r;
}

//---------------------------------------------------------------------------
// запись буфера
int64_t TV8FileStream::Write(const void *Buffer, int64_t Count)
{
	int r = file->Write(Buffer, pos, Count);
	pos += r;
	return r;
}

//---------------------------------------------------------------------------
// запись буфера
int TV8FileStream::Write(const std::vector<t::Byte> Buffer, int Offset, int Count)
{
	int r = file->Write(Buffer, pos, Count);
	pos += r;
	return r;
}

//---------------------------------------------------------------------------
// позиционирование
int64_t TV8FileStream::Seek(const int64_t Offset, TSeekOrigin Origin)
{
	int64_t len = file->GetFileLength();
	switch(Origin)
	{
		case soFromBeginning:
			if(Offset >= 0)	{
				if(Offset <= len) {
					pos = Offset;
				}
				else {
					pos = len;
				}
			}
			break;
		case soFromCurrent:
			if(pos + Offset < len) {
				pos += Offset;
			}
			else {
				pos = len;
			}
			break;
		case soFromEnd:
			if(Offset <= 0) {
				if(Offset <= len) {
					pos = len - Offset;
				}
				else {
					pos = 0;
				}
			}
			break;
	}
	return pos;
}
