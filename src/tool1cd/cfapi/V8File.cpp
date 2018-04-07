#include <memory>

#include"APIcfBase.h"
#include "V8File.h"
#include "../MessageRegistration.h"
#include "../DetailedException.h"

extern Registrator msreg_g;

using namespace System;
using namespace std;

V8File::V8File(V8Catalog *_parent, const std::string &_name, V8File *_previous, int _start_data, int _start_header,
			   int64_t time_create, int64_t time_modify)
{
	Lock = new TCriticalSection();
	_destructed = false;
	flushed  = false;
	parent   = _parent;
	name     = _name;
	previous = _previous;
	next     = nullptr;
	data     = nullptr;
	start_data        = _start_data;
	start_header      = _start_header;
	_datamodified   = !start_data;
	is_headermodified = !start_header;
	if(previous)
		previous->next = this;
	else
		parent->first_file(this);
	iscatalog = FileIsCatalog::unknown;
	self = nullptr;
	is_opened = false;
	_time_create = V8Time(time_create);
	_time_modify = V8Time(time_modify);
	_selfzipped = false;
	if(parent) {
		V8Catalog::V8Files& files = parent->v8files();
		files[LowerCase(name)] = this;
	}
}

// получить время создания
System::FILETIME V8File::get_time_create() const {
	return _time_create.to_file_time();
}

// получить время модификации
System::FILETIME V8File::get_time_modify() const {
	return _time_modify.to_file_time();
}

// установить время создания
void V8File::set_time_create(const System::FILETIME &file_time) {
	_time_create.from_file_time(file_time);
}

void V8File::set_time_create(const V8Time &time) {
	_time_create = time;
}

// установить время модификации
void V8File::set_time_modify(const System::FILETIME &file_time) {
	_time_modify.from_file_time(file_time);
}

void V8File::set_time_modify(const V8Time &time) {
	_time_modify = time;
}

//---------------------------------------------------------------------------
// сохранить в файл
void V8File::save_to_file(const boost::filesystem::path &FileName)
{
	if (!try_open()){
		return;
	}

	TFileStream fs(FileName, fmCreate);
	Lock->Acquire();
	fs.CopyFrom(data, 0);
	fs.Close();
	Lock->Release();

	#ifdef _MSC_VER

	auto file_times = V8Time::to_file_times(_time_create, _time_modify);
	_utime(FileName.string().c_str(), &file_times);

	#else

	auto file_times = V8Time::to_file_times(_time_create, _time_modify);
	utime(FileName.string().c_str(), &file_times);

	#endif // _MSC_VER
}

//---------------------------------------------------------------------------
// сохранить в поток
void V8File::save_to_stream(TStream* stream)
{
	Lock->Acquire();
	if (!try_open()) {
		return;
	}
	stream->CopyFrom(data, 0);
	Lock->Release();
}

int64_t V8File::get_file_length()
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
int64_t V8File::read(void* Buffer, int Start, int Length)
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
int64_t V8File::read(std::vector<uint8_t> Buffer, int Start, int Length)
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
TV8FileStream* V8File::get_stream(bool own)
{
	return new TV8FileStream(this, own);
}

//---------------------------------------------------------------------------
// записать
int64_t V8File::write(const void* Buffer, int Start, int Length) // дозапись/перезапись частично
{
	int64_t ret = 0l;
	Lock->Acquire();
	if (!try_open()){
		return ret;
	}
	_time_modify = V8Time::current_time();

	is_headermodified = true;
	_datamodified   = true;
	data->Seek(Start, soFromBeginning);
	ret = data->Write(Buffer, Length);
	Lock->Release();

	return ret;
}

//---------------------------------------------------------------------------
// записать
int64_t V8File::write(std::vector<uint8_t> Buffer, int Start, int Length) // дозапись/перезапись частично
{
	int64_t ret = 0l;
	Lock->Acquire();
	if (!try_open()){
		return ret;
	}
	_time_modify = V8Time::current_time();

	is_headermodified = true;
	_datamodified   = true;
	data->Seek(Start, soFromBeginning);
	ret = data->Write(Buffer, Length);
	Lock->Release();

	return ret;
}

//---------------------------------------------------------------------------
// записать
int64_t V8File::write(const void* Buffer, int Length) // перезапись целиком
{
	int64_t ret = 0l;
	Lock->Acquire();
	if (!try_open()) {
		return ret;
	}
	_time_modify = V8Time::current_time();

	is_headermodified = true;
	_datamodified = true;
	if (data->GetSize() > Length) data->SetSize(Length);
	data->Seek(0, soFromBeginning);
	ret = data->Write(Buffer, Length);
	Lock->Release();

	return ret;
}

//---------------------------------------------------------------------------
// записать
int64_t V8File::write(TStream* Stream, int Start, int Length) // дозапись/перезапись частично
{
	int64_t ret = 0l;
	Lock->Acquire();
	if (!try_open()) {
		return ret;
	}
	_time_modify = V8Time::current_time();

	is_headermodified = true;
	_datamodified   = true;
	data->Seek(Start, soFromBeginning);
	ret = data->CopyFrom(Stream, Length);
	Lock->Release();

	return ret;
}

//---------------------------------------------------------------------------
// записать
int64_t V8File::write(TStream* Stream) // перезапись целиком
{
	int64_t ret = 0l;
	Lock->Acquire();
	if (!try_open()) {
		return ret;
	}
	_time_modify = V8Time::current_time();

	is_headermodified = true;
	_datamodified   = true;
	if (data->GetSize() > Stream->GetSize()) data->SetSize(Stream->GetSize());
	data->Seek(0, soFromBeginning);
	ret = data->CopyFrom(Stream, 0);
	Lock->Release();

	return ret;
}


//---------------------------------------------------------------------------
// возвращает имя
std::string V8File::get_file_name()
{
	return name;
}

//---------------------------------------------------------------------------
// возвращает полное имя
std::string V8File::get_full_name()
{
	if(parent != nullptr) {
		string fulln = parent->get_full_name();
		if(!fulln.empty())
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
void V8File::set_file_name(const string &_name)
{
	name = _name;
	is_headermodified = true;
}

//---------------------------------------------------------------------------
// определение "каталога"
bool V8File::is_catalog()
{
	int64_t _filelen;
	uint32_t _startempty = (uint32_t)(-1);
	char _t[stBlockHeader::size()];

	Lock->Acquire();
	if(iscatalog == FileIsCatalog::unknown){
		// эмпирический метод?
		if (!try_open())
		{
			Lock->Release();
			return false;
		}
		_filelen = data->GetSize();
		if(_filelen == stFileHeader::size())
		{
			data->Seek(0, soFromBeginning);
			data->Read(_t, stFileHeader::size());
			if(memcmp(_t, _EMPTY_CATALOG_TEMPLATE, stFileHeader::size()) != 0)
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
		if(_filelen < (stBlockHeader::size() + stFileHeader::size()) ){
			iscatalog = FileIsCatalog::no;
			Lock->Release();
			return false;
		}
		data->Seek(stFileHeader::size(), soFromBeginning);
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
V8Catalog* V8File::get_catalog(){
	V8Catalog* ret;

	Lock->Acquire();
	if(is_catalog())
	{
		if(!self)
		{
			self = new V8Catalog(this);
		}
		ret = self;
	}
	else ret = nullptr;
	Lock->Release();
	return ret;
}

//---------------------------------------------------------------------------
// получение родительского контейнера
V8Catalog* V8File::get_parent_catalog()
{
	return parent;
}

//---------------------------------------------------------------------------
// удалить файл
void V8File::delete_file()
{
	Lock->Acquire();
	if(parent)
	{
		parent->Acquire();
		if(next)
		{
			next->Lock->Acquire();
			next->previous = previous;
			next->Lock->Release();
		}
		else parent->last_file(previous);
		if(previous) {
			previous->Lock->Acquire();
			previous->next = next;
			previous->Lock->Release();
		}
		else {
			parent->first_file(next);
		}

		parent->fatmodified(true);
		parent->free_block(start_data);
		parent->free_block(start_header);

		V8Catalog::V8Files& files = parent->v8files();
		files.erase(LowerCase(name));

		parent->Release();
		parent = nullptr;
	}
	delete data;
	data = nullptr;
	if(self)
	{
		self->erase_data(); // TODO: разобраться, зачем обнуляется указатель
		delete self;
		self = nullptr;
	}
	iscatalog = FileIsCatalog::no;
	next = nullptr;
	previous = nullptr;
	is_opened = false;
	start_data = 0;
	start_header = 0;
	_datamodified = false;
	is_headermodified = false;
}

//---------------------------------------------------------------------------
// получить следующий
V8File* V8File::get_next()
{
	return next;
}

//---------------------------------------------------------------------------
// открыть файл
bool V8File::open(){
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
void V8File::close(){
	uint32_t zero4b = 0;

	if(!parent) return;
	Lock->Acquire();
	if(!is_opened) return;

	if( self && !self->is_destructed() ) {
		delete self;
	}
	self = nullptr;

	if(!parent->data_empty()) {
		if(_datamodified || is_headermodified)
		{
			parent->Acquire();
			if(_datamodified)
			{
				start_data = parent->write_datablock(data, start_data, _selfzipped);
			}
			if(is_headermodified)
			{
				TMemoryStream* hs = new TMemoryStream();
				_time_create.write_to_stream(hs);
				_time_modify.write_to_stream(hs);
				hs->Write(&zero4b, 4);
				auto tb = SysUtils::TEncoding::Unicode->fromUtf8(name);
				hs->Write(tb.data(), tb.size());
				hs->Write(&zero4b, 4);

				start_header = parent->write_block(hs, start_header, false);
				delete hs;
			}
			parent->Release();
		}
	}
	delete data;
	data = nullptr;
	iscatalog = FileIsCatalog::unknown;
	is_opened = false;
	_datamodified = false;
	is_headermodified = false;
	Lock->Release();
}

//---------------------------------------------------------------------------
// записать и закрыть
int64_t V8File::write_and_close(TStream* Stream, int Length)
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

	if (!parent->data_empty()) {
		auto wname = SysUtils::TEncoding::Unicode->fromUtf8(name);

		parent->Acquire();
		start_data = parent->write_datablock(Stream, start_data, _selfzipped, Length);
		TMemoryStream hs;
		_time_create.write_to_stream(&hs);
		_time_modify.write_to_stream(&hs);
		hs.Write(&_4bzero, 4);
		hs.Write(wname.data(), wname.size());
		hs.Write(&_4bzero, 4);
		start_header = parent->write_block(&hs, start_header, false);
		parent->Release();
	}
	iscatalog = FileIsCatalog::unknown;
	is_opened = false;
	_datamodified = false;
	is_headermodified = false;
	Lock->Release();

	if (Length == -1) return Stream->GetSize();
	return Length;
}

//---------------------------------------------------------------------------
// деструктор
V8File::~V8File()
{
	std::set<TV8FileStream*>::iterator istreams;

	Lock->Acquire();
	_destructed = true;
	for(istreams = _streams.begin(); istreams != _streams.end(); ++istreams) {
		delete *istreams;
	}
	_streams.clear();

	close();

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
			parent->Acquire();
			parent->last_file(previous);
			parent->Release();
		}
		if(previous)
		{
			previous->Lock->Acquire();
			previous->next = next;
			previous->Lock->Release();
		}
		else
		{
			parent->Acquire();
			parent->first_file(next);
			parent->Release();
		}
	}
	delete Lock;
}

//---------------------------------------------------------------------------
// сброс
void V8File::flush()
{
	uint32_t zero4b = 0;

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

	if( !parent->data_empty() )	{
		if(_datamodified || is_headermodified)
		{
			parent->Acquire();
			if(_datamodified)
			{
				start_data = parent->write_datablock(data, start_data, _selfzipped);
				_datamodified = false;
			}
			if(is_headermodified)
			{
				TMemoryStream* hs = new TMemoryStream();
				_time_create.write_to_stream(hs);
				_time_modify.write_to_stream(hs);
				hs->Write(&zero4b, 4);
				auto tb = SysUtils::TEncoding::Unicode->fromUtf8(name);
				hs->Write(tb.data(), tb.size());
				hs->Write(&zero4b, 4);

				start_header = parent->write_block(hs, start_header, false);
				delete hs;
				is_headermodified = false;
			}
			parent->Release();
		}
	}
	flushed = false;
	Lock->Release();
}

unique_ptr<Tree> V8File::get_tree()
{
	std::vector<uint8_t> bytes;
	std::unique_ptr<TBytesStream> bytes_stream( new TBytesStream(bytes) );
	save_to_stream(bytes_stream.get());

	TEncoding *enc = nullptr;
	int32_t offset = TEncoding::GetBufferEncoding(bytes_stream->GetBytes(), enc);
	if(offset == 0 || enc == nullptr) {
		throw DetailedException("Ошибка определения кодировки файла контейнера")
			.add_detail("Файл",  get_full_name());
	}

	string text = enc->toUtf8(bytes_stream->GetBytes(), offset);

	auto rt = parse_1Ctext(text, get_full_name());

	return rt;
}

TCriticalSection* V8File::get_lock() {
	return Lock;
}

TStream* V8File::get_data() {
	return data;
}

int V8File::get_start_data() const {
	return start_data;
}

int V8File::get_start_header() const {
	return start_header;
}

bool V8File::get_data_modified() const {
	return _datamodified;
}

void V8File::set_data_modified(const bool value) {
	_datamodified = value;
}

bool V8File::get_destructed() const {
	return _destructed;
}

void V8File::set_destructed(const bool value) {
	_destructed = value;
}

bool V8File::get_self_zipped() const {
	return _selfzipped;
}

void V8File::set_self_zipped(const bool value) {
	_selfzipped = value;
}

V8File::TV8FileStreams& V8File::streams() {
	return _streams;
}
