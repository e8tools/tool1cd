#include <array>
#include <algorithm>

#include "V8Catalog.h"
#include "APIcfBase.h"
#include "../UZLib.h"
#include "../Common.h"

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

// определение каталога
bool V8Catalog::IsCatalog() const
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
V8Catalog::V8Catalog(String name) // создать каталог из физического файла .cf
{
	Lock = new TCriticalSection();
	iscatalogdefined = false;

	String ext = ExtractFileExt(name).LowerCase();
	if(ext == CFU_STR)
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
		zipped = ext == CF_STR || ext == EPF_STR || ext == ERF_STR || ext == CFE_STR;
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
V8Catalog::V8Catalog(String name, bool _zipped) // создать каталог из физического файла
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
	file->Open();
	data = file->get_data();
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
	catalog_header _ch;
	String _name;
	fat_item _fi;
	char* _temp_buf;
	TMemoryStream* _file_header;
	TStream* _fat;
	V8File* _prev;
	V8File* _file;
	V8File* f;
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
				int64_t time_create = *(int64_t*)_temp_buf;
				int64_t time_modify = *(int64_t*)_temp_buf +8;
				_file = new V8File(this, _name, _prev, _fi.data_start, _fi.header_start, time_create, time_modify);
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
			f = f->GetNext();
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

	_fatmodified = false;
	is_emptymodified = false;
	is_modified = false;
	_destructed = false;
	flushed = false;
	leave_data = false;
}

//---------------------------------------------------------------------------
// удалить файл
void V8Catalog::DeleteFile(const String& FileName)
{
	Lock->Acquire();
	V8File* f = first;
	while(f)
	{
		if(!f->GetFileName().CompareIC(FileName))
		{
			f->DeleteFile();
			delete f;
		}
		f = f->GetNext();
	}
	Lock->Release();
}

//---------------------------------------------------------------------------
// получить файл
V8File* V8Catalog::GetFile(const String& FileName)
{
	V8File* ret;
	Lock->Acquire();
	std::map<String,V8File*>::const_iterator it;
	it = files.find(FileName.UpperCase());
	if(it == files.end()) ret = nullptr;
	else ret = it->second;
	Lock->Release();
	return ret;
}

//---------------------------------------------------------------------------
// получить первого
V8File* V8Catalog::GetFirst(){
	return first;
}

//---------------------------------------------------------------------------
// создать файл
V8File* V8Catalog::createFile(const String& FileName, bool _selfzipped){
	int64_t v8t;
	V8File* f;

	Lock->Acquire();
	f = GetFile(FileName);
	if(!f)
	{
		V8Time v8t = V8Time::current_time();

		f = new V8File(this, FileName, last, 0, 0, 0, 0);
		f->time_create(v8t);
		f->time_modify(v8t);
		f->self_zipped(_selfzipped);
		last = f;
		_fatmodified = true;
	}
	Lock->Release();
	return f;
}

//---------------------------------------------------------------------------
// получить родительский каталог
V8Catalog* V8Catalog::GetParentCatalog()
{
	if(!file) {
		return nullptr;
	}
	return file->GetParentCatalog();
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
		start = start_empty;
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
V8Catalog::~V8Catalog()
{
	fat_item fi;
	V8File* f;
	TMemoryStream* fat = nullptr;

	Lock->Acquire();
	_destructed = true;

	f = first;
	while(f)
	{
		f->Close();
		f = f->GetNext();
	}

	if(data)
	{
		if(_fatmodified)
		{
			try
			{
				fat = new TMemoryStream;
				fi.ff = LAST_BLOCK;
				f = first;
				while(f)
				{
					fi.header_start = f->get_start_header();
					fi.data_start = f->get_start_data();
					fat->WriteBuffer(&fi, 12);
					f = f->GetNext();
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
			file->datamodified(true);
		}
		if(!file->is_destructed()) file->Close();
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
V8File* V8Catalog::GetSelfFile()
{
	return file;
}

//---------------------------------------------------------------------------
// создать каталог
V8Catalog* V8Catalog::CreateCatalog(const String& FileName, bool _selfzipped)
{
	V8Catalog* ret;
	Lock->Acquire();
	V8File* f = createFile(FileName, _selfzipped);
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
void V8Catalog::SaveToDir(const boost::filesystem::path &dir) const
{
	if (!boost::filesystem::exists(dir)) {
		boost::filesystem::create_directories(dir);
	}
	Lock->Acquire();
	V8File* f = first;
	while(f)
	{
		if(f->IsCatalog()) {
			f->GetCatalog()->SaveToDir(dir / static_cast<std::string>(f->GetFileName()));
		}
		else {
			f->SaveToFile(dir / static_cast<std::string>(f->GetFileName()));
		}
		f->Close();
		f = f->GetNext();
	}
	Lock->Release();
}

//---------------------------------------------------------------------------
// возвращает признак открытости
bool V8Catalog::isOpen() const
{
	return IsCatalog();
}

//---------------------------------------------------------------------------
// сбросить
void V8Catalog::Flush()
{
	fat_item fi;
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
		f->Flush();
		f = f->GetNext();
	}

	if(data)
	{
		if(_fatmodified)
		{
			TMemoryStream* fat = new TMemoryStream;
			fi.ff = LAST_BLOCK;
			f = first;
			while(f)
			{
				fi.header_start = f->get_start_header();
				fi.data_start = f->get_start_data();
				fat->WriteBuffer(&fi, 12);
				f = f->GetNext();
			}
			write_block(fat, CATALOG_HEADER_LEN, true);
			_fatmodified = false;
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
			file->datamodified(true);
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
void V8Catalog::HalfOpen(const String& name)
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

String V8Catalog::get_full_name() {
	String result;

	if(file != nullptr) {
		result = file->GetFullName();
	}
	return result;
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
