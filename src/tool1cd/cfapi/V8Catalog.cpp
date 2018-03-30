#include <array>
#include <algorithm>

#include "V8Catalog.h"
#include "../UZLib.h"
#include "../Common.h"
using namespace std;

//---------------------------------------------------------------------------
// читает блок из потока каталога stream_from, собирая его по страницам
TStream* read_block(TStream* stream_from, int start, TStream* stream_to = nullptr)
{
	stBlockHeader block_header;

	if(!stream_to) {
		stream_to = new TMemoryStream;
	}

	stream_to->Seek(0, soFromBeginning);
	stream_to->SetSize(0);

	if( start < 0
			|| start == LAST_BLOCK
			|| start > stream_from->GetSize() ) {
		return stream_to;
	}

	stream_from->Seek(start, soFromBeginning);
	stream_from->Read(&block_header, stBlockHeader::size());

	int32_t len = block_header.get_data_size();

	if(len == 0) {
		return stream_to;
	}

	int32_t curlen = block_header.get_page_size();
	start = block_header.get_next_page_addr();

	int32_t readlen = std::min(len, curlen);
	stream_to->CopyFrom(stream_from, readlen);

	int32_t pos = readlen;

	while(start != LAST_BLOCK) {
		stream_from->Seek(start, soFromBeginning);
		stream_from->Read(&block_header, block_header.size());

		curlen = block_header.get_page_size();

		start  = block_header.get_next_page_addr();

		readlen = std::min(len - pos, curlen);
		stream_to->CopyFrom(stream_from, readlen);
		pos += readlen;
	}

	return stream_to;

}

// определение каталога
bool V8Catalog::is_catalog() const
{
	int64_t _filelen;
	uint32_t _startempty = (uint32_t)(-1);
	char _t[stBlockHeader::size()];

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
	if(_filelen == stFileHeader::size())
	{
		data->Seek(0, soFromBeginning);
		data->Read(_t, stFileHeader::size());
		if(memcmp(_t, _EMPTY_CATALOG_TEMPLATE, stFileHeader::size()) != 0)
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
	if(_filelen < (stBlockHeader::size() + stFileHeader::size()) )
	{
		Lock->Release();
		return false;
	}
	data->Seek(stFileHeader::size(), soFromBeginning);
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
V8Catalog::V8Catalog(const string &name) // создать каталог из физического файла .cf
{
	Lock = new TCriticalSection();
	iscatalogdefined = false;

	string ext = LowerCase(ExtractFileExt(name));
	if (ext == CFU_STR)
	{
		is_cfu = true;
		zipped = false;
		data = new TMemoryStream();

		if(!FileExists(name))
		{
			data->WriteBuffer(_EMPTY_CATALOG_TEMPLATE, stFileHeader::size());
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
		zipped = ext == CF_STR || ext == EPF_STR || ext == ERF_STR || ext == CFE_STR;
		is_cfu = false;

		if(!FileExists(name))
		{
			data = new TFileStream(name, fmCreate);
			data->WriteBuffer(_EMPTY_CATALOG_TEMPLATE, stFileHeader::size());
			delete data;
		}
		data = new TFileStream(name, fmOpenReadWrite);
	}

	file = nullptr;
	if(is_catalog()) initialize();
	else
	{
		first = nullptr;
		last = nullptr;
		zipped = false;

		_fatmodified = false;
		is_emptymodified = false;
		is_modified = false;
		_destructed = false;
		flushed = false;
		leave_data = false;
	}
}

//---------------------------------------------------------------------------
// конструктор
V8Catalog::V8Catalog(const string &name, bool _zipped) // создать каталог из физического файла
{
	Lock = new TCriticalSection();
	iscatalogdefined = false;
	is_cfu = false;
	zipped = _zipped;

	if(!FileExists(name))
	{
		data = new TFileStream(name, fmCreate);
		data->WriteBuffer(_EMPTY_CATALOG_TEMPLATE, stFileHeader::size());
		delete data;
	}
	data = new TFileStream(name, fmOpenReadWrite);
	file = nullptr;
	if(is_catalog()) initialize();
	else
	{
		first = nullptr;
		last = nullptr;
		zipped = false;

		_fatmodified = false;
		is_emptymodified = false;
		is_modified = false;
		_destructed = false;
		flushed = false;
		leave_data = false;
	}
}

//---------------------------------------------------------------------------
// конструктор
V8Catalog::V8Catalog(TStream* stream, bool _zipped, bool leave_stream) // создать каталог из потока
{
	Lock = new TCriticalSection();
	is_cfu = false;
	iscatalogdefined = false;
	zipped = _zipped;
	data = stream;
	file = nullptr;

	if(!data->GetSize())
		data->WriteBuffer(_EMPTY_CATALOG_TEMPLATE, stFileHeader::size());

	if(is_catalog())
		initialize();
	else
	{
		first = nullptr;
		last = nullptr;
		zipped = false;

		_fatmodified = false;
		is_emptymodified = false;
		is_modified = false;
		_destructed = false;
		flushed = false;
	}
	leave_data = leave_stream;
}

//---------------------------------------------------------------------------
// конструктор
V8Catalog::V8Catalog(V8File* f) // создать каталог из файла
{
	is_cfu = false;
	iscatalogdefined = false;
	file = f;
	Lock = file->get_lock();
	Lock->Acquire();
	file->open();
	data = file->get_data();
	zipped = false;

	if(is_catalog()) initialize();
	else
	{
		first = nullptr;
		last = nullptr;
		zipped = false;

		_fatmodified = false;
		is_emptymodified = false;
		is_modified = false;
		_destructed = false;
		flushed = false;
		leave_data = false;
	}
	Lock->Release();
}

//---------------------------------------------------------------------------
void V8Catalog::initialize()
{
	_destructed = false;
	char* _temp_buf;

	V8File* _prev;
	V8File* _file;
	V8File* f;

	first = nullptr;
	_prev = nullptr;

	data->Seek(0, soFromBeginning);
	data->ReadBuffer(&file_header, stFileHeader::size());

	unique_ptr<TMemoryStream> file_data( new TMemoryStream );

	try {
		if(data->GetSize() > stFileHeader::size()) {
			// TODO: как это работает?
			unique_ptr<TStream> _fat( read_block(data, stFileHeader::size()) );
			_fat->Seek(0, soFromBeginning);
			int64_t count_files = _fat->GetSize() / stElemAddr::size();

			for(int i = 0; i < count_files; i++) {
				stElemAddr elem_addr;

				_fat->Read(&elem_addr, stElemAddr::size());
				read_block(data, elem_addr.header_addr, file_data.get());
				file_data->Seek(0, soFromBeginning);
				auto header_size = file_data->GetSize();
				_temp_buf = new char[header_size];
				file_data->Read(_temp_buf, header_size);
				// TODO: константы 20, 8

				string _name = TEncoding::Unicode->toUtf8(vector<uint8_t>(_temp_buf + 20, _temp_buf + header_size));
				int64_t time_create = *(int64_t*)_temp_buf;
				int64_t time_modify = *(int64_t*)_temp_buf + 8;

				_file = new V8File(this, _name, _prev, elem_addr.data_addr, elem_addr.header_addr, time_create, time_modify);
				delete[] _temp_buf;

				if(!_prev) {
					first = _file;
				}

				_prev = _file;
			}
		}
	}
	catch(...)
	{
		f = first;
		while(f)
		{
			f->close();
			f = f->get_next();
		}
		while(first) delete first;

		iscatalog = false;
		iscatalogdefined = true;

		first = nullptr;
		last = nullptr;
		zipped = false;

	}

	last = _prev;

	_fatmodified = false;
	is_emptymodified = false;
	is_modified = false;
	_destructed = false;
	flushed = false;
	leave_data = false;
}

//---------------------------------------------------------------------------
// удалить файл
void V8Catalog::delete_file(const string &FileName)
{
	Lock->Acquire();
	V8File* f = first;
	while(f)
	{
		if (EqualIC(f->get_file_name(), FileName)) {
			f->delete_file();
			delete f;
		}
		f = f->get_next();
	}
	Lock->Release();
}

//---------------------------------------------------------------------------
// получить файл
V8File* V8Catalog::get_file(const std::string &FileName)
{
	V8File* ret;
	Lock->Acquire();
	auto it = files.find(LowerCase(FileName));
	if(it == files.end()) ret = nullptr;
	else ret = it->second;
	Lock->Release();
	return ret;
}

//---------------------------------------------------------------------------
// получить первого
V8File* V8Catalog::get_first(){
	return first;
}

//---------------------------------------------------------------------------
// создать файл
V8File* V8Catalog::createFile(const string &FileName, bool _selfzipped){
	int64_t v8t;
	V8File* f;

	Lock->Acquire();
	f = get_file(FileName);
	if(!f)
	{
		V8Time v8t = V8Time::current_time();

		f = new V8File(this, FileName, last, 0, 0, 0, 0);
		f->set_time_create(v8t);
		f->set_time_modify(v8t);
		f->set_self_zipped(_selfzipped);
		last = f;
		_fatmodified = true;
	}
	Lock->Release();
	return f;
}

//---------------------------------------------------------------------------
// получить родительский каталог
V8Catalog* V8Catalog::get_parent_catalog()
{
	if(!file) {
		return nullptr;
	}
	return file->get_parent_catalog();
}

//---------------------------------------------------------------------------
// чтение блока данных
TStream* V8Catalog::read_datablock(int start)
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
void V8Catalog::free_block(int start){
	stBlockHeader block_header;
	int nextstart;
	int prevempty;

	if(start == 0 || start == LAST_BLOCK) {
		return;
	}

	Lock->Acquire();
	prevempty = file_header.next_page_addr;
	file_header.next_page_addr = start;

	do {
		data->Seek(start, soFromBeginning);
		data->ReadBuffer(&block_header, stBlockHeader::size());

		nextstart = block_header.get_next_page_addr();

		block_header.set_data_size(LAST_BLOCK);

		if (nextstart == LAST_BLOCK) {
			block_header.set_next_page_addr(prevempty);
		}
		data->Seek(start, soFromBeginning);
		data->WriteBuffer(&block_header, stBlockHeader::size());
		start = nextstart;
	}
	while(start != LAST_BLOCK);

	is_emptymodified = true;
	is_modified = true;
	Lock->Release();
}

//---------------------------------------------------------------------------
// запись блока данных
int V8Catalog::write_datablock(TStream* block, int start, bool _zipped, int len)
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
int64_t V8Catalog::get_nextblock(int64_t start)
{
	int64_t ret;

	Lock->Acquire();
	if(start == 0 || start == LAST_BLOCK)
	{
		start = file_header.next_page_addr;
		if(start == LAST_BLOCK) start = data->GetSize();
	}
	ret = start;
	Lock->Release();
	return ret;
}

//---------------------------------------------------------------------------
// записать блок
int V8Catalog::write_block(TStream* block, int start, bool use_page_size, int len)
{
	stBlockHeader block_header;
	char* _t;
	int firststart, nextstart, blocklen, curlen;
	bool isfirstblock = true;
	bool addwrite = false; // признак, что надо дозаписать файл при использовании размера страницы по умолчанию

	Lock->Acquire();
	if(data->GetSize() == stFileHeader::size() && start != stFileHeader::size()) // если каталог пустой, надо выделить первую страницу!!!
	{
		TMemoryStream* _ts = new TMemoryStream;
		write_block(_ts, stFileHeader::size(), true);
	}

	if(len == -1)
	{
		len = block->GetSize();
		block->Seek(0, soFromBeginning);
	}
	start = get_nextblock(start);

	do
	{
		if(start == file_header.next_page_addr)
		{// пишем в свободный блок
			data->Seek(start, soFromBeginning);
			data->ReadBuffer(&block_header, stBlockHeader::size());

			blocklen = block_header.get_page_size();
			nextstart = block_header.get_next_page_addr();

			file_header.next_page_addr = nextstart;
			is_emptymodified = true;
		}
		else if(start == data->GetSize())
		{// пишем в новый блок
			block_header = stBlockHeader::create(0, 0, 0);
			blocklen = use_page_size ? len > file_header.page_size ? len : file_header.page_size : len;
			block_header.set_page_size(blocklen);

			nextstart = 0;
			if(blocklen > len) addwrite = true;
		}
		else
		{// пишем в существующий блок
			data->Seek(start, soFromBeginning);
			data->ReadBuffer(&block_header, stBlockHeader::size());

			blocklen = block_header.get_page_size();
			nextstart = block_header.get_next_page_addr();
		}

		block_header.set_data_size(isfirstblock ? len : 0);

		curlen = std::min(blocklen, len);

		if(!nextstart) {
			nextstart = data->GetSize() + 31 + blocklen; // TODO: 31 в константу
		}
		else {
			nextstart = get_nextblock(nextstart);
		}

		block_header.set_next_page_addr(len <= blocklen ? LAST_BLOCK : nextstart);

		data->Seek(start, soFromBeginning);
		data->WriteBuffer(&block_header, stBlockHeader::size());
		data->CopyFrom(block, curlen);
		if(addwrite) {
			_t = new char [blocklen - len];
			memset(_t, 0, blocklen - len);
			data->WriteBuffer(_t, blocklen - len);
			addwrite = false;
		}

		len -= curlen;

		if(isfirstblock) {
			firststart = start;
			isfirstblock = false;
		}
		start = nextstart;

	} while(len > 0);

	if(start < data->GetSize() && start != file_header.next_page_addr) free_block(start);

	is_modified = true;
	Lock->Release();
	return firststart;
}

//---------------------------------------------------------------------------
// деструктор
V8Catalog::~V8Catalog()
{
	stElemAddr elem_addr;
	V8File* f;
	TMemoryStream* fat = nullptr;

	Lock->Acquire();
	_destructed = true;

	f = first;
	while(f)
	{
		f->close();
		f = f->get_next();
	}

	if(data)
	{
		if(_fatmodified)
		{
			try
			{
				fat = new TMemoryStream;
				elem_addr.fffffff = LAST_BLOCK;
				f = first;
				while(f)
				{
					elem_addr.header_addr = f->get_start_header();
					elem_addr.data_addr = f->get_start_data();
					fat->WriteBuffer(&elem_addr, stElemAddr::size());
					f = f->get_next();
				}
				write_block(fat, stFileHeader::size(), true);
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
			data->WriteBuffer(&file_header.next_page_addr, sizeof(file_header.next_page_addr));
		}
		if(is_modified)
		{
			file_header.storage_ver++;
			data->Seek(8, soFromBeginning);
			data->WriteBuffer(&file_header.storage_ver, sizeof(file_header.storage_ver));
		}
	}

	if(file){
		if(is_modified)
		{
			file->set_data_modified(true);
		}
		if(!file->get_destructed()) file->close();
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
V8File* V8Catalog::get_self_file()
{
	return file;
}

//---------------------------------------------------------------------------
// создать каталог
V8Catalog* V8Catalog::CreateCatalog(const string &FileName, bool _selfzipped)
{
	V8Catalog* ret;
	Lock->Acquire();
	V8File* f = createFile(FileName, _selfzipped);
	if(f->get_file_length() > 0)
	{
		if(f->is_catalog()) ret = f->get_catalog();
		else ret = nullptr;
	}
	else
	{
		f->write(_EMPTY_CATALOG_TEMPLATE, stFileHeader::size());
		ret = f->get_catalog();
	}
	Lock->Release();
	return ret;
}

//---------------------------------------------------------------------------
// сохранить в файловую систему
void V8Catalog::SaveToDir(const boost::filesystem::path &dir) const
{
	if (!boost::filesystem::exists(dir)) {
		boost::filesystem::create_directories(dir);
	}
	Lock->Acquire();
	V8File* f = first;
	while(f)
	{
		if(f->is_catalog()) {
			f->get_catalog()->SaveToDir(dir / f->get_file_name());
		}
		else {
			f->save_to_file(dir / f->get_file_name());
		}
		f->close();
		f = f->get_next();
	}
	Lock->Release();
}

//---------------------------------------------------------------------------
// возвращает признак открытости
bool V8Catalog::isOpen() const
{
	return is_catalog();
}

//---------------------------------------------------------------------------
// сбросить
void V8Catalog::Flush()
{
	stElemAddr elem_addr;
	V8File* f;

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
		f->flush();
		f = f->get_next();
	}

	if(data)
	{
		if(_fatmodified)
		{
			TMemoryStream* fat = new TMemoryStream;
			elem_addr.fffffff = LAST_BLOCK;
			f = first;
			while(f)
			{
				elem_addr.header_addr = f->get_start_header();
				elem_addr.data_addr = f->get_start_data();
				fat->WriteBuffer(&elem_addr, stElemAddr::size());
				f = f->get_next();
			}
			write_block(fat, stFileHeader::size(), true);
			_fatmodified = false;
		}

		if(is_emptymodified)
		{
			data->Seek(0, soFromBeginning);
			data->WriteBuffer(&file_header.next_page_addr, sizeof(file_header.next_page_addr));
			is_emptymodified = false;
		}
		if(is_modified)
		{
			file_header.storage_ver++;
			data->Seek(8, soFromBeginning);
			data->WriteBuffer(&file_header.storage_ver, sizeof(file_header.storage_ver));
		}
	}

	if(file){
		if(is_modified)
		{
			file->set_data_modified(true);
		}
		file->flush();
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
void V8Catalog::HalfClose()
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
void V8Catalog::HalfOpen(const string &name)
{
	Lock->Acquire();
	if(is_cfu) cfu = new TFileStream(name, fmOpenReadWrite);
	else data = new TFileStream(name, fmOpenReadWrite);
	Lock->Release();
}

V8File* V8Catalog::get_first_file()
{
	return first;
}

void V8Catalog::first_file(V8File* value)
{
	first = value;
}

V8File* V8Catalog::get_last_file()
{
	return last;
}

void V8Catalog::last_file(V8File *value)
{
	last = value;
}

V8Catalog::V8Files& V8Catalog::v8files() {
	return files;
}

string V8Catalog::get_full_name() {
	if (file != nullptr) {
		return file->get_full_name();
	}
	return "";
}

bool V8Catalog::data_empty() const {
	return (data == nullptr);
}

void V8Catalog::erase_data() {
	data = nullptr;
}

bool V8Catalog::is_fatmodified() const {
	return _fatmodified;
}

void V8Catalog::fatmodified(const bool value) {
	_fatmodified = value;
}

bool V8Catalog::is_destructed() const {
	return _destructed;
}
