/*
 *
 *
 */

#include "V8Object.h"
#include "Common.h"
#include "Constants.h"
#include "DetailedException.h"

using namespace std;

v8object* v8object::first = nullptr;
v8object* v8object::last = nullptr;

extern Registrator msreg_g;

//---------------------------------------------------------------------------
void v8object::garbage()
{
	uint32_t curt = GetTickCount();
	v8object* ob = first;

	while(ob)
	{
		if(!ob->lockinmemory) if(ob->data) if(curt - ob->lastdataget > LIVE_CASH * 60 * 1000)
		{
			delete[] ob->data;
			ob->data = nullptr;
		}
		ob = ob->next;
	}
}

//---------------------------------------------------------------------------
void v8object::set_lockinmemory(bool _lock)
{
	lockinmemory = _lock;
	lastdataget = GetTickCount();
}

//---------------------------------------------------------------------------
void v8object::init()
{
	len = 0;
	version.version_1 = 0;
	version.version_2 = 0;
	version_rec.version_1 = 0;
	version_rec.version_2 = 0;
	new_version_recorded = false;
	numblocks = 0;
	real_numblocks = 0;
	blocks = nullptr;
	block = -1;
	data = nullptr;
	lockinmemory = false;
	type = v8objtype::unknown;
	fatlevel = 0;
}

//---------------------------------------------------------------------------
void v8object::init(T_1CD* _base, int32_t blockNum)
{
	base = _base;
	lockinmemory = false;

	prev = last;
	next = nullptr;
	if(last) last->next = this;
	else first = this;
	last = this;

	if(blockNum == 1)
	{
		if(base->version < db_ver::ver8_3_8_0) type = v8objtype::free80;
		else type = v8objtype::free838;
	}
	else
	{
		if(base->version < db_ver::ver8_3_8_0) type = v8objtype::data80;
		else type = v8objtype::data838;
	}

	if(type == v8objtype::data80 || type == v8objtype::free80)
	{
		fatlevel = 1;
		v8ob* t = new v8ob;
		base->getblock(t, blockNum);
		if(memcmp(&(t->sig), SIG_OBJ, 8) != 0)
		{
			delete t;
			init();
			throw DetailedException("Ошибка получения объекта из блока. Блок не является объектом.")
				.add_detail("Блок", to_hex_string(blockNum));
		}

		len = t->len;
		version.version_1 = t->version.version_1;
		version.version_2 = t->version.version_2;
		version.version_3 = t->version.version_3;
		version_rec.version_1 = version.version_1 + 1;
		version_rec.version_2 = 0;
		new_version_recorded = false;
		block = blockNum;
		real_numblocks = 0;
		data = nullptr;

		if(type == v8objtype::free80)
		{
			if(len) numblocks = (len - 1) / 0x400 + 1;
			else numblocks = 0;

			// в таблице свободных блоков в разделе blocks может быть больше блоков, чем numblocks
			// numblocks - кол-во блоков с реальными данными
			// оставшиеся real_numblocks - numblocks блоки принадлежат объекту, но не содержат данных
			while(t->blocks[real_numblocks]) real_numblocks++;
			if(real_numblocks)
			{
				blocks = new uint32_t[real_numblocks];
				memcpy(blocks, t->blocks, real_numblocks * sizeof(*blocks));
			}
			else blocks = nullptr;
		}
		else
		{
			if(len) numblocks = (len - 1) / 0x3ff000 + 1;
			else numblocks = 0;
			if(numblocks)
			{
				blocks = new uint32_t[numblocks];
				memcpy(blocks, t->blocks, numblocks * sizeof(*blocks));
			}
			else blocks = nullptr;
		}

		delete t;
	}
	else if(type == v8objtype::data838)
	{
		char* b = new char[base->pagesize];
		v838ob_data* t = (v838ob_data*)b;
		base->getblock(t, blockNum);

		if(t->sig[0] != 0x1c || t->sig[1] != 0xfd)
		{
			delete[] b;
			init();
			throw DetailedException("Ошибка получения файла из страницы. Страница не является заголовочной страницей файла данных.")
				.add_detail("Блок", to_hex_string(blockNum));
		}

		len = t->len;
		fatlevel = t->fatlevel;
		if(fatlevel == 0 && len > ((base->pagesize / 4 - 6) * base->pagesize))
		{
			delete[] b;
			init();
			throw DetailedException("Ошибка получения файла из страницы. Длина файла больше допустимой при одноуровневой таблице размещения.")
				.add_detail("Блок", to_hex_string(blockNum))
				.add_detail("Длина файла", len);
		}
		version.version_1 = t->version.version_1;
		version.version_2 = t->version.version_2;
		version.version_3 = t->version.version_3;
		version_rec.version_1 = version.version_1 + 1;
		version_rec.version_2 = 0;
		new_version_recorded = false;
		block = blockNum;
		real_numblocks = 0;
		data = nullptr;

		if(len)
		{
			if(fatlevel == 0) {
				numblocks = (len - 1) / base->pagesize + 1;
			}
			else {
				numblocks = (len - 1) / (base->pagesize / 4 * base->pagesize) + 1;
			}
		}
		else numblocks = 0;
		if(numblocks)
		{
			blocks = new uint32_t[numblocks];
			memcpy(blocks, t->blocks, numblocks * sizeof(*blocks));
		}
		else blocks = nullptr;

		delete[] b;
	}
	else
	{
		char* b = new char[base->pagesize];
		v838ob_free* t = (v838ob_free*)b;
		base->getblock(t, blockNum);

		if(t->sig[0] != 0x1c || t->sig[1] != 0xff)
		{
			delete[] b;
			init();
			throw DetailedException("Ошибка получения файла из страницы. Страница не является заголовочной страницей файла свободных блоков.")
				.add_detail("Блок", to_hex_string(blockNum));
		}

		len = 0; // ВРЕМЕННО! Пока не понятна структура файла свободных страниц

		version.version_1 = t->version;
		version_rec.version_1 = version.version_1 + 1;
		version_rec.version_2 = 0;
		new_version_recorded = false;
		block = blockNum;
		real_numblocks = 0;
		data = nullptr;

		if(len) numblocks = (len - 1) / 0x400 + 1;
		else numblocks = 0;

		// в таблице свободных блоков в разделе blocks может быть больше блоков, чем numblocks
		// numblocks - кол-во блоков с реальными данными
		// оставшиеся real_numblocks - numblocks блоки принадлежат объекту, но не содержат данных
		while(t->blocks[real_numblocks]) real_numblocks++;
		if(real_numblocks)
		{
			blocks = new uint32_t[real_numblocks];
			memcpy(blocks, t->blocks, real_numblocks * sizeof(*blocks));
		}
		else blocks = nullptr;

		delete[] b;
	}


	msreg_g.AddDebugMessage("Создан объект", MessageState::Info)
			.with("Номер блока", to_hex_string(blockNum))
			.with("Длина", len)
			.with("Версия данных", to_string(version.version_1).append(":").append(to_string(version.version_2)));

}

//---------------------------------------------------------------------------
v8object::v8object(T_1CD* _base, int32_t blockNum)
{
	init(_base, blockNum);
}

v8object::v8object(T_1CD* _base)
{
	int32_t blockNum;
	char* b;

	blockNum = _base->get_free_block();
	b = _base->getblock_for_write(blockNum, false);
	memset(b, 0, _base->pagesize);
	if(_base->version < db_ver::ver8_3_8_0)	memcpy(((v8ob*)b)->sig, SIG_OBJ, 8);
	else
	{
		b[0] = 0x1c;
		b[1] = 0xfd;
	}
	init(_base, blockNum);
}


//---------------------------------------------------------------------------
v8object::~v8object()
{
	delete[] blocks;
	delete[] data;

	if(prev) prev->next = next;
	else first = next;
	if(next) next->prev = prev;
	else last = prev;
}

//---------------------------------------------------------------------------
char* v8object::getdata()
{
	char* tt;
	objtab* b;
	objtab838* bb;
	uint32_t i, l;
	int32_t j, pagesize, blocksperpage;
	uint64_t ll;
	uint32_t curlen = 0;

	lastdataget = GetTickCount();
	if(!len) return nullptr;
	if(data) return data;

	if(type == v8objtype::free80)
	{
		l = len * 4;
		data = new char[l];
		tt = data;
		i = 0;
		while(l > DEFAULT_PAGE_SIZE)
		{
			base->getblock(tt, blocks[i++]);
			tt += DEFAULT_PAGE_SIZE;
			l -= DEFAULT_PAGE_SIZE;
		}
		base->getblock(tt, blocks[i], l);
	}
	else if(type == v8objtype::data80)
	{
		l = len;
		data = new char[l];
		tt = data;
		for(i = 0; i < numblocks; i++)
		{
			b = (objtab*)base->getblock(blocks[i]);
			for(j = 0; j < b->numblocks; j++)
			{
				curlen = std::min(DEFAULT_PAGE_SIZE, l);
				base->getblock(tt, b->blocks[j], curlen);
				if(l <= curlen) break;
				l -= curlen;
				tt += DEFAULT_PAGE_SIZE;
			}
			if(l <= curlen) break;
		}
	}
	else if(type == v8objtype::data838)
	{
		pagesize = base->pagesize;
		blocksperpage = pagesize / 4;
		ll = len;
		data = new char[ll];
		tt = data;
		if(fatlevel)
		{
			for(i = 0; i < numblocks; i++)
			{
				bb = (objtab838*)base->getblock(blocks[i]);
				for(j = 0; j < blocksperpage; j++)
				{
					curlen = ll > pagesize ? pagesize : ll;
					base->getblock(tt, bb->blocks[j], curlen);
					if(ll <= curlen) break;
					ll -= curlen;
					tt += pagesize;
				}
				if(ll <= curlen) break;
			}
		}
		else
		{
			for(i = 0; i < numblocks; i++)
			{
				curlen = ll > pagesize ? pagesize : ll;
				base->getblock(tt, blocks[i], curlen);
				if(ll <= curlen) break;
				ll -= curlen;
				tt += pagesize;
			}
		}
	}
	else if(type == v8objtype::free838)
	{
		// TODO: реализовать v8object::getdata() для файла свободных страниц формата 8.3.8
	}
	return data;
}

//---------------------------------------------------------------------------
char* v8object::getdata(void* buf, uint64_t _start, uint64_t _length)
{
	uint32_t curblock;
	uint32_t curoffblock;
	char* _buf;
	char* _bu;
	uint32_t curlen;
	uint32_t offsperpage;

	objtab* b;
	objtab838* bb;
	uint32_t curobjblock;
	uint32_t curoffobjblock;

	lastdataget = GetTickCount();

	if(data) memcpy(buf, data + _start, _length);
	else
	{
		if(type == v8objtype::free80)
		{
			if(_start + _length > len * 4)
			{
				throw DetailedException("Попытка чтения данных за пределами объекта")
					.add_detail("Номер блока объекта", to_hex_string(block))
					.add_detail("Длина объекта", len * 4)
					.add_detail("Начало читаемых данных", _start)
					.add_detail("Длина читаемых данных", _length);
			}

			curblock = _start >> 12;
			_buf = (char*)buf;
			curoffblock = _start - (curblock << 12);
			curlen = std::min(static_cast<uint64_t>(DEFAULT_PAGE_SIZE - curoffblock), _length);

			while(_length)
			{
				_bu = base->getblock(blocks[curblock++]);
				if(!_bu) {
					return nullptr;
				}
				memcpy(_buf, _bu + curoffblock, curlen);
				_buf += curlen;
				_length -= curlen;
				curoffblock = 0;
				curlen = std::min(static_cast<uint64_t>(DEFAULT_PAGE_SIZE), _length);
			}

		}
		else if(type == v8objtype::data80)
		{
			if(_start + _length > len)
			{
				throw DetailedException("Попытка чтения данных за пределами объекта")
					.add_detail("Номер блока объекта", to_hex_string(block))
					.add_detail("Длина объекта", len)
					.add_detail("Начало читаемых данных", _start)
					.add_detail("Длина читаемых данных", _length);
			}

			curblock = _start >> 12;
			_buf = (char*)buf;
			curoffblock = _start - (curblock << 12);
			curlen = std::min(static_cast<uint64_t>(DEFAULT_PAGE_SIZE - curoffblock), _length);

			curobjblock = curblock / 1023;
			curoffobjblock = curblock - curobjblock * 1023;

			b = (objtab*) base->getblock(blocks[curobjblock++]);
			if(!b) {
				return nullptr;
			}
			while(_length)
			{
				_bu = base->getblock(b->blocks[curoffobjblock++]);
				if(!_bu) {
					return nullptr;
				}
				memcpy(_buf, _bu + curoffblock, curlen);
				_buf += curlen;
				_length -= curlen;
				curoffblock = 0;
				curlen = std::min(static_cast<uint64_t>(DEFAULT_PAGE_SIZE), _length);
				if(_length > 0) {
					if(curoffobjblock >= 1023)
					{
						curoffobjblock = 0;
						b = (objtab*) base->getblock(blocks[curobjblock++]);
					}
				}
			}
		}
		else if(type == v8objtype::data838)
		{
			if(_start + _length > len)
			{
				throw DetailedException("Попытка чтения данных за пределами объекта")
					.add_detail("Номер блока объекта", to_hex_string(block))
					.add_detail("Длина объекта", len)
					.add_detail("Начало читаемых данных", _start)
					.add_detail("Длина читаемых данных", _length);
			}

			curblock = _start / base->pagesize;
			_buf = (char*)buf;
			offsperpage = base->pagesize / 4;
			curoffblock = _start - (curblock * base->pagesize);
			curlen = std::min(static_cast<uint64_t>(base->pagesize - curoffblock), _length);
			if(fatlevel)
			{
				curobjblock = curblock / offsperpage;
				curoffobjblock = curblock - curobjblock * offsperpage;

				bb = (objtab838*) base->getblock(blocks[curobjblock++]);
				if(!bb) {
					return nullptr;
				}
				while(_length)
				{
					_bu = base->getblock(bb->blocks[curoffobjblock++]);
					if(!_bu) {
						return nullptr;
					}
					memcpy(_buf, _bu + curoffblock, curlen);
					_buf += curlen;
					_length -= curlen;
					curoffblock = 0;
					curlen = std::min(static_cast<uint64_t>(base->pagesize), _length);
					if(_length > 0) {
						if(curoffobjblock >= offsperpage)
						{
							curoffobjblock = 0;
							bb = (objtab838*) base->getblock(blocks[curobjblock++]);
						}
					}
				}
			}
			else
			{
				while(_length)
				{
					_bu = base->getblock(blocks[curblock++]);
					if(!_bu) {
						return nullptr;
					}
					memcpy(_buf, _bu + curoffblock, curlen);
					_buf += curlen;
					_length -= curlen;
					curoffblock = 0;
					curlen = std::min(static_cast<uint64_t>(base->pagesize), _length);
				}
			}
		}
		else if(type == v8objtype::free838)
		{
			// TODO: реализовать v8object::getdata для файла свободных страниц формата 8.3.8
		}

	}
	return (char*)buf;
}

//---------------------------------------------------------------------------
uint64_t v8object::getlen()
{
	if(type == v8objtype::free80) return len * 4;
	else return len;
}

//---------------------------------------------------------------------------
void v8object::savetofile(const std::string &_filename)
{
	uint64_t pagesize = base->pagesize;
	TFileStream fs(_filename, fmCreate);
	char *buf = new char[pagesize];
	uint64_t total_size = getlen();
	uint64_t remain_size = total_size;
	for (uint64_t offset = 0; offset < total_size; offset += pagesize)
	{
		unsigned size_of_block = std::min(remain_size, pagesize);
		getdata(buf, offset, size_of_block);
		fs.Write(buf, size_of_block);
		remain_size -= pagesize;
	}
	delete[] buf;
}

//---------------------------------------------------------------------------
uint64_t v8object::get_fileoffset(uint64_t offset)
{
	uint32_t _start = offset;
	objtab* b;
	objtab838* bb;
	uint32_t curblock;
	uint32_t curoffblock;
	uint32_t curobjblock;
	uint32_t curoffobjblock;
	uint32_t offsperpage;

	if(type == v8objtype::free80)
	{
		curblock = _start >> 12;
		curoffblock = _start - (curblock << 12);
		return (((uint64_t)(blocks[curblock])) << 12) + curoffblock;
	}
	else if(type == v8objtype::data80)
	{
		curblock = _start >> 12;
		curoffblock = _start - (curblock << 12);

		curobjblock = curblock / 1023;
		curoffobjblock = curblock - curobjblock * 1023;

		b = (objtab*) base->getblock(blocks[curobjblock]);

		return (((uint64_t)(b->blocks[curoffobjblock])) << 12) + curoffblock;
	}
	else if(type == v8objtype::data838)
	{
		curblock = _start / base->pagesize;
		curoffblock = _start - (curblock * base->pagesize);
		if(fatlevel)
		{
			offsperpage = base->pagesize / 4;
			curobjblock = curblock / offsperpage;
			curoffobjblock = curblock - curobjblock * offsperpage;
			bb = (objtab838*)base->getblock(blocks[curobjblock]);
			return (((uint64_t)(bb->blocks[curoffobjblock])) * base->pagesize) + curoffblock;
		}
		else
		{
			return (((uint64_t)(blocks[curblock])) * base->pagesize) + curoffblock;
		}
	}

	else if(type == v8objtype::free838)
	{
		// TODO: реализовать v8object::get_fileoffset для файла свободных страниц формата 8.3.8
		return 0;
	}

	return 0;

}

//---------------------------------------------------------------------------
bool v8object::setdata(const void* buf, uint64_t _start, uint64_t _length)
{
	uint32_t curblock;
	uint32_t curoffblock;
	char* _buf;
	uint32_t curlen;

	uint32_t curobjblock;
	uint32_t curoffobjblock;
	uint32_t offsperpage;

	if(base->get_readonly())
	{
		throw DetailedException("Попытка записи в файл в режиме \"Только чтение\"")
			.add_detail("Номер страницы файла", to_hex_string(block));
	}

	if(type == v8objtype::free80 || type == v8objtype::free838)
	{
		throw DetailedException("Попытка прямой записи в файл свободных страниц")
			.add_detail("Номер страницы файла", to_hex_string(block));
	}

	lastdataget = GetTickCount();

	delete[] data;
	data = nullptr;
	if(_start + _length > len) {
		set_len(_start + _length);
	}

	if(type == v8objtype::data80)
	{
		curblock = _start >> 12;
		_buf = (char*)buf;
		curoffblock = _start - (curblock << 12);
		curlen = std::min(static_cast<uint64_t>(DEFAULT_PAGE_SIZE - curoffblock), _length);

		curobjblock = curblock / 1023;
		curoffobjblock = curblock - curobjblock * 1023;

		objtab *b = (objtab*) base->getblock(blocks[curobjblock++]);
		while(_length)
		{
			memcpy((char*)(base->getblock_for_write(b->blocks[curoffobjblock++], curlen != DEFAULT_PAGE_SIZE)) + curoffblock, _buf, curlen);
			_buf += curlen;
			_length -= curlen;
			curoffblock = 0;
			curlen = std::min(static_cast<uint64_t>(DEFAULT_PAGE_SIZE), _length);
			if(_length > 0) {
				if(curoffobjblock >= 1023)
				{
					curoffobjblock = 0;
					b = (objtab*) base->getblock(blocks[curobjblock++]);
				}
			}
		}

		write_new_version();
		return true;
	}
	else if(type == v8objtype::data838)
	{
		curblock = _start / base->pagesize;
		_buf = (char*)buf;
		curoffblock = _start - (curblock * base->pagesize);
		curlen = std::min(static_cast<uint64_t>(base->pagesize - curoffblock), _length);

		if(fatlevel)
		{
			offsperpage = base->pagesize / 4;
			curobjblock = curblock / offsperpage;
			curoffobjblock = curblock - curobjblock * offsperpage;

			objtab838 *bb = (objtab838*) base->getblock(blocks[curobjblock++]);
			while(_length)
			{
				memcpy((char*)(base->getblock_for_write(bb->blocks[curoffobjblock++], curlen != base->pagesize)) + curoffblock, _buf, curlen);
				_buf += curlen;
				_length -= curlen;
				curoffblock = 0;
				curlen = std::min(static_cast<uint64_t>(base->pagesize), _length);
				if(_length > 0) {
					if(curoffobjblock >= offsperpage)
					{
						curoffobjblock = 0;
						bb = (objtab838*) base->getblock(blocks[curobjblock++]);
					}
				}
			}
		}
		else
		{
			while(_length)
			{
				memcpy((char*)(base->getblock_for_write(blocks[curblock++], curlen != base->pagesize)) + curoffblock, _buf, curlen);
				_buf += curlen;
				_length -= curlen;
				curoffblock = 0;
				curlen = std::min(static_cast<uint64_t>(base->pagesize), _length);
			}
		}

		write_new_version();
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
bool v8object::setdata(const void* _buf, uint64_t _length)
{
	uint32_t curlen = 0;
	char* buf;
	uint32_t offsperpage;
	uint32_t curblock;
	uint32_t curobjblock;
	uint32_t curoffobjblock;

	if(base->get_readonly())
	{
		throw DetailedException("Попытка записи в файл в режиме \"Только чтение\"")
			.add_detail("Номер страницы файла", to_hex_string(block));
	}

	if(type == v8objtype::free80 || type == v8objtype::free838)
	{
		throw DetailedException("Попытка прямой записи в файл свободных страниц")
			.add_detail("Номер страницы файла", to_hex_string(block));
	}

	delete[] data;
	data = nullptr;
	set_len(_length);

	buf = (char*)_buf;

	if(type == v8objtype::data80)
	{
		for (unsigned i = 0; i < numblocks; i++)
		{
			objtab *b = (objtab*)base->getblock(blocks[i]);
			for(int32_t j = 0; j < b->numblocks; j++)
			{
				curlen = std::min(static_cast<uint64_t>(DEFAULT_PAGE_SIZE),_length);
				char *tt = base->getblock_for_write(b->blocks[j], false);
				memcpy(tt, buf, curlen);
				buf += DEFAULT_PAGE_SIZE;
				if(_length <= curlen) {
					break;
				}
				_length -= curlen;
			}
			if(_length <= curlen) {
				break;
			}
		}

		write_new_version();
		return true;
	}
	else if(type == v8objtype::data838)
	{
		curblock = 0;
		curlen = std::min(static_cast<uint64_t>(base->pagesize), _length);

		if(fatlevel)
		{
			offsperpage = base->pagesize / 4;
			curobjblock = 0;
			curoffobjblock = 0;

			objtab838 *bb = (objtab838*) base->getblock(blocks[curobjblock++]);
			while(_length)
			{
				memcpy((char*)(base->getblock_for_write(bb->blocks[curoffobjblock++], false)), buf, curlen);
				buf += curlen;
				_length -= curlen;
				curlen = std::min(static_cast<uint64_t>(base->pagesize), _length);
				if(_length > 0) {
					if(curoffobjblock >= offsperpage)
					{
						curoffobjblock = 0;
						bb = (objtab838*) base->getblock(blocks[curobjblock++]);
					}
				}
			}
		}
		else
		{
			while(_length)
			{
				memcpy((char*)(base->getblock_for_write(blocks[curblock++], false)), buf, curlen);
				buf += curlen;
				_length -= curlen;
				curlen = std::min(static_cast<uint64_t>(base->pagesize), _length);
			}
		}

		write_new_version();
		return true;
	}

	return false;
}

//---------------------------------------------------------------------------
bool v8object::setdata(TStream* stream)
{
	uint32_t curlen;
	uint64_t _length;
	uint32_t offsperpage;
	uint32_t curblock;
	uint32_t curobjblock;
	uint32_t curoffobjblock;

	if(base->get_readonly())
	{
		throw DetailedException("Попытка записи в файл в режиме \"Только чтение\"")
			.add_detail("Номер страницы файла", to_hex_string(block));
	}

	if(type == v8objtype::free80 || type == v8objtype::free838)
	{
		throw DetailedException("Попытка прямой записи в файл свободных страниц")
			.add_detail("Номер страницы файла", to_hex_string(block));
	}

	delete[] data;
	data = nullptr;
	_length = stream->GetSize();
	set_len(_length);

	stream->Seek(0, soFromBeginning);

	if(type == v8objtype::data80)
	{
		for (unsigned i = 0; i < numblocks; i++)
		{
			objtab *b = (objtab*)base->getblock(blocks[i]);
			for(int32_t j = 0; j < b->numblocks; j++)
			{
				curlen = std::min(static_cast<uint64_t>(DEFAULT_PAGE_SIZE),_length);
				char *tt = base->getblock_for_write(b->blocks[j], false);
				stream->ReadBuffer(tt, curlen);
				if(_length <= curlen) {
					break;
				}
				_length -= curlen;
			}
			if(_length <= curlen) {
				break;
			}
		}

		write_new_version();
		return true;
	}
	else if(type == v8objtype::data838)
	{
		curblock = 0;
		curlen = std::min(static_cast<uint64_t>(base->pagesize), _length);

		if(fatlevel)
		{
			offsperpage = base->pagesize / 4;
			curobjblock = 0;
			curoffobjblock = 0;

			objtab838 *bb = (objtab838*) base->getblock(blocks[curobjblock++]);
			while(_length)
			{
				stream->ReadBuffer(base->getblock_for_write(bb->blocks[curoffobjblock++], false), curlen);
				_length -= curlen;
				curlen = std::min(static_cast<uint64_t>(base->pagesize), _length);
				if(_length > 0) {
					if(curoffobjblock >= offsperpage)
					{
						curoffobjblock = 0;
						bb = (objtab838*) base->getblock(blocks[curobjblock++]);
					}
				}
			}
		}
		else
		{
			while(_length)
			{
				stream->ReadBuffer(base->getblock_for_write(blocks[curblock++], false), curlen);
				_length -= curlen;
				curlen = std::min(static_cast<uint64_t>(base->pagesize), _length);
			}
		}

		write_new_version();
		return true;
	}

	return false;

}

//---------------------------------------------------------------------------
bool v8object::setdata(TStream* stream, uint64_t _start, uint64_t _length)
{
	uint32_t curblock;
	uint32_t curoffblock;
	uint32_t curlen;

	uint32_t curobjblock;
	uint32_t curoffobjblock;
	uint32_t offsperpage;

	if(base->get_readonly())
	{
		throw DetailedException("Попытка записи в файл в режиме \"Только чтение\"")
			.add_detail("Номер страницы файла", to_hex_string(block));
	}

	if(type == v8objtype::free80 || type == v8objtype::free838)
	{
		throw DetailedException("Попытка прямой записи в файл свободных страниц")
			.add_detail("Номер страницы файла", to_hex_string(block));
	}

	lastdataget = GetTickCount();

	delete[] data;
	data = nullptr;
	if(_start + _length > len) set_len(_start + _length);

	if(type == v8objtype::data80)
	{
		curblock = _start >> 12;
		curoffblock = _start - (curblock << 12);
		curlen = std::min(static_cast<uint64_t>(DEFAULT_PAGE_SIZE - curoffblock), _length);

		curobjblock = curblock / 1023;
		curoffobjblock = curblock - curobjblock * 1023;

		objtab *b = (objtab*) base->getblock(blocks[curobjblock++]);
		while(_length)
		{
			stream->ReadBuffer((char*)(base->getblock_for_write(b->blocks[curoffobjblock++], curlen != DEFAULT_PAGE_SIZE)) + curoffblock, curlen);
			_length -= curlen;
			curoffblock = 0;
			curlen = std::min(static_cast<uint64_t>(DEFAULT_PAGE_SIZE), _length);
			if(_length > 0) {
				if(curoffobjblock >= 1023)
				{
					curoffobjblock = 0;
					b = (objtab*) base->getblock(blocks[curobjblock++]);
				}
			}
		}

		write_new_version();
		return true;
	}
	else if(type == v8objtype::data838)
	{
		curblock = _start / base->pagesize;
		curoffblock = _start - (curblock * base->pagesize);
		curlen = std::min(static_cast<uint64_t>(base->pagesize - curoffblock), _length);

		if(fatlevel)
		{
			offsperpage = base->pagesize / 4;
			curobjblock = curblock / offsperpage;
			curoffobjblock = curblock - curobjblock * offsperpage;

			objtab838 *bb = (objtab838*) base->getblock(blocks[curobjblock++]);
			while(_length)
			{
				stream->ReadBuffer((char*)(base->getblock_for_write(bb->blocks[curoffobjblock++], curlen != base->pagesize)) + curoffblock, curlen);
				_length -= curlen;
				curoffblock = 0;
				curlen = std::min(static_cast<uint64_t>(base->pagesize), _length);
				if(_length > 0) {
					if(curoffobjblock >= offsperpage)
					{
						curoffobjblock = 0;
						bb = (objtab838*) base->getblock(blocks[curobjblock++]);
					}
				}
			}
		}
		else
		{
			while(_length)
			{
				stream->ReadBuffer((char*)(base->getblock_for_write(blocks[curblock++], curlen != base->pagesize)) + curoffblock, curlen);
				_length -= curlen;
				curoffblock = 0;
				curlen = std::min(static_cast<uint64_t>(base->pagesize), _length);
			}
		}

		write_new_version();
		return true;
	}

	return false;

}

//---------------------------------------------------------------------------
void v8object::set_len(uint64_t _len)
{

	uint32_t num_data_blocks;
	uint32_t num_blocks;
	uint32_t cur_data_blocks;
	uint32_t bl;
	uint32_t i;
	v8ob* b;
	v838ob_data* bd;
	objtab838* bb;
	uint32_t offsperpage;
	uint64_t maxlen;
	int32_t newfatlevel;

	if(len == _len) return;

	if(type == v8objtype::free80 || type == v8objtype::free838)
	{
		// Таблица свободных блоков
		throw DetailedException("Попытка установки длины в файле свободных страниц");
		return;
	}

	delete[] data;
	data = nullptr;

	if(type == v8objtype::data80)
	{
		b = (v8ob*)base->getblock_for_write(block, true);
		b->len = _len;

		num_data_blocks = (_len + 0xfff) >> 12;
		num_blocks = (num_data_blocks + 1022) / 1023;
		cur_data_blocks = (len + 0xfff) >> 12;

		if(numblocks != num_blocks)
		{
			delete[] blocks;
			if(num_blocks) blocks = new uint32_t[num_blocks];
			else blocks = nullptr;
		}

		if(num_data_blocks > cur_data_blocks)
		{
			objtab *ot;
			// Увеличение длины объекта
			if(numblocks) ot = (objtab*)base->getblock_for_write(b->blocks[numblocks - 1], true);
			for(; cur_data_blocks < num_data_blocks; cur_data_blocks++)
			{
				i = cur_data_blocks % 1023;
				if(i == 0)
				{
					bl = base->get_free_block();
					b->blocks[numblocks++] = bl;
					ot = (objtab*)base->getblock_for_write(bl, false);
					ot->numblocks = 0;
				}
				bl = base->get_free_block();
				base->getblock_for_write(bl, false); // получаем блок без чтения, на случай, если блок вдруг в конце файла
				ot->blocks[i] = bl;
				ot->numblocks = i + 1;
			}
		}
		else if(num_data_blocks < cur_data_blocks)
		{
			// Уменьшение длины объекта
			objtab *ot = (objtab*)base->getblock_for_write(b->blocks[numblocks - 1], true);
			for(cur_data_blocks--; cur_data_blocks >= num_data_blocks; cur_data_blocks--)
			{
				i = cur_data_blocks % 1023;
				base->set_block_as_free(ot->blocks[i]);
				ot->blocks[i] = 0;
				ot->numblocks = i;
				if(i == 0)
				{
					base->set_block_as_free(b->blocks[--numblocks]);
					b->blocks[numblocks] = 0;
					if(numblocks) ot = (objtab*)base->getblock_for_write(b->blocks[numblocks - 1], true);
				}
			}

		}

		len = _len;
		if(numblocks) memcpy(blocks, b->blocks, numblocks * 4);

		write_new_version();
	}
	else if(type == v8objtype::data838)
	{
		offsperpage = base->pagesize / 4;
		maxlen = base->pagesize * offsperpage * (offsperpage - 6);
		if(_len > maxlen)
		{
			throw DetailedException("Попытка установки длины файла больше максимальной")
				.add_detail("Номер страницы файла", to_hex_string(block))
				.add_detail("Максимальная длина файла", maxlen)
				.add_detail("Запрошенная длина файла", to_hex_string(_len));
		}

		bd = (v838ob_data*)base->getblock_for_write(block, true);
		bd->len = _len;

		num_data_blocks = (_len + base->pagesize - 1) / base->pagesize;
		if(num_data_blocks > offsperpage - 6)
		{
			num_blocks = (num_data_blocks + offsperpage - 1) / offsperpage;
			newfatlevel = 1;
		}
		else
		{
			num_blocks = num_data_blocks;
			newfatlevel = 0;
		}
		cur_data_blocks = (len + base->pagesize - 1) / base->pagesize;

		if(numblocks != num_blocks)
		{
			delete[] blocks;
			if(num_blocks) blocks = new uint32_t[num_blocks];
			else blocks = nullptr;
		}

		if(num_data_blocks > cur_data_blocks)
		{
			// Увеличение длины объекта
			if(fatlevel == 0 && newfatlevel)
			{
				bl = base->get_free_block();
				bb = (objtab838*)base->getblock_for_write(bl, false);
				memcpy(bb->blocks, bd->blocks, numblocks * 4);
				fatlevel = newfatlevel;
				bd->fatlevel = newfatlevel;
				bd->blocks[0] = bl;
				numblocks = 1;
			}
			else bb = (objtab838*)base->getblock_for_write(bd->blocks[numblocks - 1], true);

			if(fatlevel)
			{
				for(; cur_data_blocks < num_data_blocks; cur_data_blocks++)
				{
					i = cur_data_blocks % offsperpage;
					if(i == 0)
					{
						bl = base->get_free_block();
						bd->blocks[numblocks++] = bl;
						bb = (objtab838*)base->getblock_for_write(bl, false);
					}
					bl = base->get_free_block();
					base->getblock_for_write(bl, false); // получаем блок без чтения, на случай, если блок вдруг в конце файла
					bb->blocks[i] = bl;
				}
			}
			else
			{
				for(; cur_data_blocks < num_data_blocks; cur_data_blocks++)
				{
					bl = base->get_free_block();
					base->getblock_for_write(bl, false); // получаем блок без чтения, на случай, если блок вдруг в конце файла
					bd->blocks[cur_data_blocks] = bl;
				}
			}
		}
		else if(num_data_blocks < cur_data_blocks)
		{
			// Уменьшение длины объекта
			if(fatlevel)
			{
				bb = (objtab838*)base->getblock_for_write(b->blocks[numblocks - 1], true);
				for(cur_data_blocks--; cur_data_blocks >= num_data_blocks; cur_data_blocks--)
				{
					i = cur_data_blocks % offsperpage;
					base->set_block_as_free(bb->blocks[i]);
					bb->blocks[i] = 0;
					if(i == 0)
					{
						base->set_block_as_free(bd->blocks[--numblocks]);
						bd->blocks[numblocks] = 0;
						if(numblocks) bb = (objtab838*)base->getblock_for_write(b->blocks[numblocks - 1], true);
					}
				}
			}
			else
			{
				for(cur_data_blocks--; cur_data_blocks >= num_data_blocks; cur_data_blocks--)
				{
					base->set_block_as_free(bd->blocks[cur_data_blocks]);
					bd->blocks[cur_data_blocks] = 0;
				}
				numblocks = num_data_blocks;
			}

			if(fatlevel && newfatlevel == 0)
			{
				if(numblocks)
				{
					bl = bd->blocks[0];
					memcpy(bd->blocks, bb->blocks, num_data_blocks * 4);
					base->set_block_as_free(bl);
				}
				fatlevel = 0;
				bd->fatlevel = 0;
			}

		}

		len = _len;
		if(numblocks) memcpy(blocks, bd->blocks, numblocks * 4);

		write_new_version();
	}
}

//---------------------------------------------------------------------------
void v8object::set_block_as_free(uint32_t block_number)
{
	if(block != 1)
	{
		// Таблица свободных блоков
		throw DetailedException("Попытка установки свободного блока в объекте, не являющимся таблицей свободных блоков")
			.add_detail("Блок объекта", block);
	}

	uint32_t j = len >> 10; // length / 1024
	int32_t i = len & 0x3ff; // length % 1024

	v8ob *ob = (v8ob*)base->getblock_for_write(block, true);

	if(real_numblocks > j)
	{
		len++;
		ob->len = len;
		uint32_t *b = (uint32_t*)base->getblock_for_write(blocks[j], true);
		b[i] = block_number;
		if(numblocks <= j) numblocks = j + 1;
	}
	else
	{
		ob->blocks[real_numblocks] = block_number;
		delete[] blocks;
		real_numblocks++;
		blocks = new uint32_t[real_numblocks];
		memcpy(blocks, ob->blocks, real_numblocks * 4);
	}

}

//---------------------------------------------------------------------------
uint32_t v8object::get_free_block()
{
	if(block != 1)
	{
		// Таблица свободных блоков
		throw DetailedException("Попытка получения свободного блока в объекте, не являющимся таблицей свободных блоков")
			.add_detail("Блок объекта", block);
	}

	if(len)
	{
		len--;
		unsigned j = len >> 10; // length / 1024
		unsigned i = len & 0x3ff; // length % 1024
		uint32_t *b = (uint32_t*)base->getblock_for_write(blocks[j], true);
		unsigned k = b[i];
		b[i] = 0;
		v8ob *ob = (v8ob*)base->getblock_for_write(block, true);
		ob->len = len;
		return k;
	}
	else
	{
		unsigned i = MemBlock::get_numblocks();
		base->getblock_for_write(i, false);
		return i;
	}

}

//---------------------------------------------------------------------------
void v8object::get_version_rec_and_increase(_version* ver)
{
	ver->version_1 = version_rec.version_1;
	ver->version_2 = version_rec.version_2;

	version_rec.version_2++;
}

//---------------------------------------------------------------------------
void v8object::get_version(_version* ver)
{
	ver->version_1 = version.version_1;
	ver->version_2 = version.version_2;
}

//---------------------------------------------------------------------------
void v8object::write_new_version()
{
	_version new_ver;
	if(new_version_recorded) return;
	int32_t veroffset = type == v8objtype::data80 || type == v8objtype::free80 ? 12 : 4;

	new_ver.version_1 = version.version_1 + 1;
	new_ver.version_2 = version.version_2;
	memcpy(base->getblock_for_write(block, true) + veroffset, &new_ver, 8);
	new_version_recorded = true;
}

//---------------------------------------------------------------------------
v8object* v8object::get_first()
{
	return first;
}

//---------------------------------------------------------------------------
v8object* v8object::get_last()
{
	return last;
}

//---------------------------------------------------------------------------
v8object* v8object::get_next()
{
	return next;
}

//---------------------------------------------------------------------------
uint32_t v8object::get_block_number()
{
	return block;
}

//---------------------------------------------------------------------------
// rewrite - перезаписывать поток _str. Истина - перезаписывать (по умолчанию), Ложь - дописывать
TStream* v8object::readBlob(TStream* _str, uint32_t _startblock, uint32_t _length, bool rewrite)
{
	uint32_t _curblock;
	char* _curb;
	uint32_t _numblock;
	uint32_t startlen;

	if(rewrite) _str->SetSize(0);
	startlen = _str->GetPosition();

	if(!_startblock && _length)
	{
		throw DetailedException("Попытка чтения нулевого блока файла Blob")
			.add_detail("Номер страницы файла", to_hex_string(block));
	}

	_numblock = len >> 8;
	if(_numblock << 8 != len)
	{
		throw DetailedException("Длина файла Blob не кратна 0x100")
			.add_detail("Номер страницы файла", to_hex_string(block))
			.add_detail("Длина файла", to_hex_string(len));
	}

	_curb = new char[0x100];
	_curblock = _startblock;
	while(_curblock)
	{
		if(_curblock >= _numblock)
		{
			throw DetailedException("Попытка чтения блока файла Blob за пределами файла")
				.add_detail("Номер страницы файла", to_hex_string(block))
				.add_detail("Всего блоков", _numblock)
				.add_detail("Читаемый блок", _curblock);
		}
		getdata(_curb, _curblock << 8, 0x100);
		_curblock = *(uint32_t*)_curb;
		uint16_t _curlen = *(uint16_t*)(_curb + 4);
		if(_curlen > 0xfa)
		{
			throw DetailedException("Попытка чтения из блока файла Blob более 0xfa байт")
				.add_detail("Номер страницы файла", to_hex_string(block))
				.add_detail("Индекс блока", _curblock)
				.add_detail("Читаемых байт", _curlen);
		}
		_str->Write(_curb + 6, _curlen);

		if(_str->GetSize() - startlen > _length) break; // аварийный выход из возможного ошибочного зацикливания
	}
	delete[] _curb;

	if(_length != UINT_MAX) if(_str->GetSize() - startlen != _length)
	{
		throw DetailedException("Несовпадение длины Blob-поля, указанного в записи, с длиной практически прочитанных данных")
			.add_detail("Номер страницы файла", to_hex_string(block))
			.add_detail("Длина поля", _length)
			.add_detail("Прочитано", _str->GetSize() - startlen);
	}

	return _str;
}




