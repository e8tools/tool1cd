//---------------------------------------------------------------------------

//#include <vcl.h>
#include <vector>
#include <System.IOUtils.hpp>

#include "Class_1CD.h"
#include "Base64.h"
#include "Common.h"
#include "TempStream.h"
#include "ConfigStorage.h"
#include <boost/filesystem.hpp>
#include <regex>

#include "UZLib.h"

const unsigned int LIVE_CASH = 5; // время жизни кешированных данных в минутах
extern MessageRegistrator* msreg;
#define error if(msreg) msreg->AddError

//---------------------------------------------------------------------------
char Field::buf[2048 + 16];
char Field::null_index[4096];
bool Field::null_index_initialized = false;
bool Field::showGUID = true;
bool Field::showGUIDasMS = false;

bool T_1CD::recoveryMode = false;

const char SIG_CON[8] = {'1', 'C', 'D', 'B', 'M', 'S', 'V', '8'};
const char SIG_OBJ[8] = {'1', 'C', 'D', 'B', 'O', 'B', 'V', '8'};
const char SIG_MOXCEL[7] = {'M', 'O', 'X', 'C', 'E', 'L', 0};
const char SIG_SKD[8] = {0, 0, 0, 0, 1, 0, 0, 0};
const char SIG_TABDESCR[4] = {'{', 0, '\"', 0};

const unsigned char SIG_ZIP[16] = {0x53,0x4b,0x6f,0xf4,0x88,0x8d,0xc1,0x4e,0xa0,0xd5,0xeb,0xb6,0xbd,0xa0,0xa7,0x0d};

const char NUM_TEST_TEMPLATE[256] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const char DATE1_TEST_TEMPLATE[256] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const char DATE3_TEST_TEMPLATE[256] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const char DATE4_TEST_TEMPLATE[256] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const char DATE5_TEST_TEMPLATE[256] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

const char DATE67_TEST_TEMPLATE[256] = {
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};


v8object* v8object::first = NULL;
v8object* v8object::last = NULL;

memblock* memblock::first = NULL;
memblock* memblock::last = NULL;
uint32_t memblock::count = 0;
uint32_t memblock::maxcount;
memblock** memblock::memblocks = NULL;

uint64_t memblock::numblocks = 0;

uint64_t memblock::array_numblocks = 0;

uint32_t memblock::delta = 128;
uint32_t memblock::pagesize;

#ifndef getcfname
#ifndef console
extern TMultiReadExclusiveWriteSynchronizer* tr_syn;
#endif
#endif

//********************************************************
// Функции


//---------------------------------------------------------------------------
tree* get_treeFromV8file(v8file* f)
{
	TBytesStream* sb;
	TEncoding *enc;
	TBytes bytes;
	int32_t offset;
	tree* rt;

	sb = new TBytesStream(bytes);
	f->SaveToStream(sb);

	enc = NULL;
	offset = TEncoding::GetBufferEncoding(sb->GetBytes(), enc);
	if(offset == 0)
	{
		error("Ошибка определения кодировки файла контейнера",
			"Файл", f->GetFullName());
		delete sb;
		return NULL;
	}
	bytes = TEncoding::Convert(enc, TEncoding::Unicode, sb->GetBytes(), offset, sb->GetSize() - offset);

	rt = parse_1Ctext(String((WCHART*)&bytes[0], bytes.GetLength() / 2), f->GetFullName());
	delete sb;
	return rt;
}

//---------------------------------------------------------------------------
String toXML(String in)
{
	return TStringBuilder(in)
		.Replace("&", "&amp;")
		->Replace("<", "&lt;")
		->Replace(">", "&gt;")
		->Replace("'", "&apos;")
		->Replace("\"", "&quot;")
		->ToString();
}

//---------------------------------------------------------------------------
unsigned char from_hex_digit(char digit)
{
	if(digit >= '0' && digit <= '9') return digit - '0';
	if(digit >= 'a' && digit <= 'f') return digit - 'a' + 10;
	if(digit >= 'A' && digit <= 'F') return digit - 'A' + 10;
	return 0;
}


//********************************************************
// Класс memblock

//---------------------------------------------------------------------------
memblock::memblock(TFileStream* fs, uint32_t _numblock, bool for_write, bool read)
{
	numblock = _numblock;
	if(count >= maxcount) delete first; // если количество кешированных блоков превышает максимальное, удаляем последний, к которому было обращение
	count++;
	prev = last;
	next = NULL;
	if(last) last->next = this;
	else first = this;
	last = this;

	buf = new char[pagesize];
	if(for_write)
	{
		uint32_t fnumblocks = fs->GetSize() / pagesize;
		if(fnumblocks <= numblock)
		{
			memset(buf, 0, pagesize);
			fs->Seek((int64_t)numblock * pagesize, (TSeekOrigin)soFromBeginning);
			fs->WriteBuffer(buf, pagesize);
			fs->Seek(12, (TSeekOrigin)soFromBeginning);
			fs->WriteBuffer(&numblock, 4);
		}
		else
		{
			if(read)
			{
				fs->Seek((int64_t)numblock * pagesize, (TSeekOrigin)soFromBeginning);
				fs->ReadBuffer(buf, pagesize);
			}
			else memset(buf, 0, pagesize);
		}
	}
	else
	{
		fs->Seek((int64_t)numblock * pagesize, (TSeekOrigin)soFromBeginning);
		fs->ReadBuffer(buf, pagesize);
	}

	is_changed = for_write;
	file = fs;

	// регистрируем себя в в массиве блоков
	memblocks[numblock] = this;
}

//---------------------------------------------------------------------------
memblock::~memblock()
{
	if(is_changed) write();

	// удаляем себя из цепочки...
	if(prev) prev->next = next;
	else first = next;
	if(next) next->prev = prev;
	else last = prev;

	// удаляем себя из массива блоков
	memblocks[numblock] = NULL;

	count--;
	delete[] buf;
}

//---------------------------------------------------------------------------
char* memblock::getblock(bool for_write)
{
	lastdataget = GetTickCount();
	// удаляем себя из текущего положения цепочки...
	if(prev) prev->next = next;
	else first = next;
	if(next) next->prev = prev;
	else last = prev;
	// ... и записываем себя в конец цепочки
	prev = last;
	next = NULL;
	if(last) last->next = this;
	else first = this;
	last = this;

	if(for_write) is_changed = true;

	return buf;
}

//---------------------------------------------------------------------------
void memblock::garbage()
{
	uint32_t curt = GetTickCount();
	while(first)
	{
		if(curt - first->lastdataget > LIVE_CASH * 60 * 1000) delete first;
		else break;
	}
}

//---------------------------------------------------------------------------
char* memblock::getblock(TFileStream* fs, uint32_t _numblock)
{
	if(_numblock >= numblocks) return NULL;
	if(!memblocks[_numblock]) new memblock(fs, _numblock, false, true);
	return memblocks[_numblock]->getblock(false);
}

//---------------------------------------------------------------------------
char* memblock::getblock_for_write(TFileStream* fs, uint32_t _numblock, bool read)
{
	if(_numblock > numblocks) return NULL;
	if(_numblock == numblocks) add_block();
	if(!memblocks[_numblock]) new memblock(fs, _numblock, true, read);
	else memblocks[_numblock]->is_changed = true;
	return memblocks[_numblock]->getblock(true);
}

//---------------------------------------------------------------------------
void memblock::create_memblocks(uint64_t _numblocks)
{
	numblocks = _numblocks;
	array_numblocks = (numblocks / delta + 1) * delta;
	memblocks = new memblock*[array_numblocks];
	memset(memblocks, 0, array_numblocks * sizeof(memblock *));
}

//---------------------------------------------------------------------------
void memblock::delete_memblocks()
{
	while(first) delete first;
	delete[] memblocks;
	numblocks = 0;
	array_numblocks = 0;
}

//---------------------------------------------------------------------------
void memblock::add_block()
{
	memblock** mb;

	if(numblocks < array_numblocks) memblocks[numblocks++] = NULL;
	else
	{
		mb = new memblock*[array_numblocks + delta];
		for(unsigned i = 0; i < array_numblocks; i++) mb[i] = memblocks[i];
		for(unsigned i = array_numblocks; i < array_numblocks + delta; i++) mb[i] = NULL;
		array_numblocks += delta;
		delete[] memblocks;
		memblocks = mb;
	}
}

//---------------------------------------------------------------------------
uint64_t memblock::get_numblocks()
{
	return numblocks;
}

//---------------------------------------------------------------------------
void memblock::flush()
{
	memblock* cur;
	for(cur = first; cur; cur = cur->next) if(cur->is_changed) cur->write();
}

//---------------------------------------------------------------------------
void memblock::write()
{
	if(!is_changed) return;
	file->Seek((int64_t)numblock * pagesize, (TSeekOrigin)soFromBeginning);
	file->WriteBuffer(buf, pagesize);
	is_changed = false;
}

//---------------------------------------------------------------------------

//***************************************************************************
// Класс v8object

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
			ob->data = NULL;
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
	blocks = NULL;
	block = -1;
	data = NULL;
	lockinmemory = false;
	type = v8ot_unknown;
	fatlevel = 0;
}

//---------------------------------------------------------------------------
void v8object::init(T_1CD* _base, int32_t blockNum)
{
	base = _base;
	lockinmemory = false;

	prev = last;
	next = NULL;
	if(last) last->next = this;
	else first = this;
	last = this;

	if(blockNum == 1)
	{
		if(base->version < ver8_3_8_0) type = v8ot_free80;
		else type = v8ot_free838;
	}
	else
	{
		if(base->version < ver8_3_8_0) type = v8ot_data80;
		else type = v8ot_data838;
	}

	if(type == v8ot_data80 || type == v8ot_free80)
	{
		fatlevel = 1;
		v8ob* t = new v8ob;
		base->getblock(t, blockNum);
		if(memcmp(&(t->sig), SIG_OBJ, 8) != 0)
		{
			delete t;
			init();
			error("Ошибка получения объекта из блока. Блок не является объектом.",
				"Блок", tohex(blockNum));
			return;
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
		data = NULL;

		if(type == v8ot_free80)
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
			else blocks = NULL;
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
			else blocks = NULL;
		}

		delete t;
	}
	else if(type == v8ot_data838)
	{
		char* b = new char[base->pagesize];
		v838ob_data* t = (v838ob_data*)b;
		base->getblock(t, blockNum);

		if(t->sig[0] != 0x1c || t->sig[1] != 0xfd)
		{
			delete[] b;
			init();
			error("Ошибка получения файла из страницы. Страница не является заголовочной страницей файла данных.",
				"Блок", tohex(blockNum));
			return;
		}

		len = t->len;
		fatlevel = t->fatlevel;
		if(fatlevel == 0 && len > ((base->pagesize / 4 - 6) * base->pagesize))
		{
			delete[] b;
			init();
			error("Ошибка получения файла из страницы. Длина файла больше допустимой при одноуровневой таблице размещения.",
				"Блок", tohex(blockNum),
				"Длина файла", len);
			return;
		}
		version.version_1 = t->version.version_1;
		version.version_2 = t->version.version_2;
		version.version_3 = t->version.version_3;
		version_rec.version_1 = version.version_1 + 1;
		version_rec.version_2 = 0;
		new_version_recorded = false;
		block = blockNum;
		real_numblocks = 0;
		data = NULL;

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
		else blocks = NULL;

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
			error("Ошибка получения файла из страницы. Страница не является заголовочной страницей файла свободных блоков.",
				"Блок", tohex(blockNum));
			return;
		}

		len = 0; // ВРЕМЕННО! Пока не понятна структура файла свободных страниц

		version.version_1 = t->version;
		version_rec.version_1 = version.version_1 + 1;
		version_rec.version_2 = 0;
		new_version_recorded = false;
		block = blockNum;
		real_numblocks = 0;
		data = NULL;

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
		else blocks = NULL;

		delete[] b;
	}


	#ifdef _DEBUG
	if(msreg) msreg->AddDebugMessage("Создан объект", msInfo,
		"Номер блока", tohex(blockNum),
		"Длина", len,
		"Версия данных", String(version.version_1) + ":" + version.version_2);
	#endif

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
	if(_base->version < ver8_3_8_0)	memcpy(((v8ob*)b)->sig, SIG_OBJ, 8);
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
	uint32_t curlen;

	lastdataget = GetTickCount();
	if(!len) return NULL;
	if(data) return data;

	if(type == v8ot_free80)
	{
		l = len * 4;
		data = new char[l];
		tt = data;
		i = 0;
		while(l > 0x1000)
		{
			base->getblock(tt, blocks[i++]);
			tt += 0x1000;
			l -= 0x1000;
		}
		base->getblock(tt, blocks[i], l);
	}
	else if(type == v8ot_data80)
	{
		l = len;
		data = new char[l];
		tt = data;
		for(i = 0; i < numblocks; i++)
		{
			b = (objtab*)base->getblock(blocks[i]);
			for(j = 0; j < b->numblocks; j++)
			{
				curlen = l > 0x1000 ? 0x1000 : l;
				base->getblock(tt, b->blocks[j], curlen);
				if(l <= curlen) break;
				l -= curlen;
				tt += 0x1000;
			}
			if(l <= curlen) break;
		}
	}
	else if(type == v8ot_data838)
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
	else if(type == v8ot_free838)
	{
		// TODO
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
		if(type == v8ot_free80)
		{
			if(_start + _length > len * 4)
			{
				error("Попытка чтения данных за пределами объекта",
					"Номер блока объекта", tohex(block),
					"Длина объекта", len * 4,
					"Начало читаемых данных", _start,
					"Длина читаемых данных", _length);
				return NULL;
			}

			curblock = _start >> 12;
			_buf = (char*)buf;
			curoffblock = _start - (curblock << 12);
			curlen = MIN(0x1000 - curoffblock, _length);

			while(_length)
			{
				_bu = base->getblock(blocks[curblock++]);
				if(!_bu) return NULL;
				memcpy(_buf, _bu + curoffblock, curlen);
				_buf += curlen;
				_length -= curlen;
				curoffblock = 0;
				curlen = MIN(0x1000, _length);
			}

		}
		else if(type == v8ot_data80)
		{
			if(_start + _length > len)
			{
				error("Попытка чтения данных за пределами объекта",
					"Номер блока объекта", tohex(block),
					"Длина объекта", len,
					"Начало читаемых данных", _start,
					"Длина читаемых данных", _length);
				return NULL;
			}

			curblock = _start >> 12;
			_buf = (char*)buf;
			curoffblock = _start - (curblock << 12);
			curlen = MIN(0x1000 - curoffblock, _length);

			curobjblock = curblock / 1023;
			curoffobjblock = curblock - curobjblock * 1023;

			b = (objtab*) base->getblock(blocks[curobjblock++]);
			if(!b) return NULL;
			while(_length)
			{
				_bu = base->getblock(b->blocks[curoffobjblock++]);
				if(!_bu) return NULL;
				memcpy(_buf, _bu + curoffblock, curlen);
				_buf += curlen;
				_length -= curlen;
				curoffblock = 0;
				curlen = MIN(0x1000, _length);
				if(_length > 0) if(curoffobjblock >= 1023)
				{
					curoffobjblock = 0;
					b = (objtab*) base->getblock(blocks[curobjblock++]);
				}
			}
		}
		else if(type == v8ot_data838)
		{
			if(_start + _length > len)
			{
				error("Попытка чтения данных за пределами объекта",
					"Номер блока объекта", tohex(block),
					"Длина объекта", len,
					"Начало читаемых данных", _start,
					"Длина читаемых данных", _length);
				return NULL;
			}

			curblock = _start / base->pagesize;
			_buf = (char*)buf;
			offsperpage = base->pagesize / 4;
			curoffblock = _start - (curblock * base->pagesize);
			curlen = MIN(base->pagesize - curoffblock, _length);
			if(fatlevel)
			{
				curobjblock = curblock / offsperpage;
				curoffobjblock = curblock - curobjblock * offsperpage;

				bb = (objtab838*) base->getblock(blocks[curobjblock++]);
				if(!bb) return NULL;
				while(_length)
				{
					_bu = base->getblock(bb->blocks[curoffobjblock++]);
					if(!_bu) return NULL;
					memcpy(_buf, _bu + curoffblock, curlen);
					_buf += curlen;
					_length -= curlen;
					curoffblock = 0;
					curlen = MIN(base->pagesize, _length);
					if(_length > 0) if(curoffobjblock >= offsperpage)
					{
						curoffobjblock = 0;
						bb = (objtab838*) base->getblock(blocks[curobjblock++]);
					}
				}
			}
			else
			{
				while(_length)
				{
					_bu = base->getblock(blocks[curblock++]);
					if(!_bu) return NULL;
					memcpy(_buf, _bu + curoffblock, curlen);
					_buf += curlen;
					_length -= curlen;
					curoffblock = 0;
					curlen = MIN(base->pagesize, _length);
				}
			}


		}
		else if(type == v8ot_free838)
		{
			// TODO
		}

	}
	return (char*)buf;
}

//---------------------------------------------------------------------------
uint64_t v8object::getlen()
{
	if(type == v8ot_free80) return len * 4;
	else return len;
}

//---------------------------------------------------------------------------
void v8object::savetofile(String _filename)
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

	if(type == v8ot_free80)
	{
		curblock = _start >> 12;
		curoffblock = _start - (curblock << 12);
		return (((uint64_t)(blocks[curblock])) << 12) + curoffblock;
	}
	else if(type == v8ot_data80)
	{
		curblock = _start >> 12;
		curoffblock = _start - (curblock << 12);

		curobjblock = curblock / 1023;
		curoffobjblock = curblock - curobjblock * 1023;

		b = (objtab*) base->getblock(blocks[curobjblock]);

		return (((uint64_t)(b->blocks[curoffobjblock])) << 12) + curoffblock;
	}
	else if(type == v8ot_data838)
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

	else if(type == v8ot_free838)
	{
		// TODO
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
		error("Попытка записи в файл в режиме \"Только чтение\"",
			"Номер страницы файла", tohex(block));
		return false;
	}

	if(type == v8ot_free80 || type == v8ot_free838)
	{
		error("Попытка прямой записи в файл свободных страниц",
			"Номер страницы файла", tohex(block));
		return false;
	}

	lastdataget = GetTickCount();

	delete[] data;
	data = NULL;
	if(_start + _length > len) set_len(_start + _length);

	if(type == v8ot_data80)
	{
		curblock = _start >> 12;
		_buf = (char*)buf;
		curoffblock = _start - (curblock << 12);
		curlen = MIN(0x1000 - curoffblock, _length);

		curobjblock = curblock / 1023;
		curoffobjblock = curblock - curobjblock * 1023;

		objtab *b = (objtab*) base->getblock(blocks[curobjblock++]);
		while(_length)
		{
			memcpy((char*)(base->getblock_for_write(b->blocks[curoffobjblock++], curlen != 0x1000)) + curoffblock, _buf, curlen);
			_buf += curlen;
			_length -= curlen;
			curoffblock = 0;
			curlen = MIN(0x1000, _length);
			if(_length > 0) if(curoffobjblock >= 1023)
			{
				curoffobjblock = 0;
				b = (objtab*) base->getblock(blocks[curobjblock++]);
			}
		}

		write_new_version();
		return true;
	}
	else if(type == v8ot_data838)
	{
		curblock = _start / base->pagesize;
		_buf = (char*)buf;
		curoffblock = _start - (curblock * base->pagesize);
		curlen = MIN(base->pagesize - curoffblock, _length);

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
				curlen = MIN(base->pagesize, _length);
				if(_length > 0) if(curoffobjblock >= offsperpage)
				{
					curoffobjblock = 0;
					bb = (objtab838*) base->getblock(blocks[curobjblock++]);
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
				curlen = MIN(base->pagesize, _length);
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
	uint32_t curlen;
	char* buf;
	uint32_t offsperpage;
	uint32_t curblock;
	uint32_t curobjblock;
	uint32_t curoffobjblock;

	if(base->get_readonly())
	{
		error("Попытка записи в файл в режиме \"Только чтение\"",
			"Номер страницы файла", tohex(block));
		return false;
	}

	if(type == v8ot_free80 || type == v8ot_free838)
	{
		error("Попытка прямой записи в файл свободных страниц",
			"Номер страницы файла", tohex(block));
		return false;
	}

	delete[] data;
	data = NULL;
	set_len(_length);

	buf = (char*)_buf;

	if(type == v8ot_data80)
	{
		for (unsigned i = 0; i < numblocks; i++)
		{
			objtab *b = (objtab*)base->getblock(blocks[i]);
			for(int32_t j = 0; j < b->numblocks; j++)
			{
				curlen = _length > 0x1000 ? 0x1000 : _length;
				char *tt = base->getblock_for_write(b->blocks[j], false);
				memcpy(tt, buf, curlen);
				buf += 0x1000;
				if(_length <= curlen) break;
				_length -= curlen;
			}
			if(_length <= curlen) break;
		}

		write_new_version();
		return true;
	}
	else if(type == v8ot_data838)
	{
		curblock = 0;
		curlen = MIN(base->pagesize, _length);

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
				curlen = MIN(base->pagesize, _length);
				if(_length > 0) if(curoffobjblock >= offsperpage)
				{
					curoffobjblock = 0;
					bb = (objtab838*) base->getblock(blocks[curobjblock++]);
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
				curlen = MIN(base->pagesize, _length);
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
		error("Попытка записи в файл в режиме \"Только чтение\"",
			"Номер страницы файла", tohex(block));
		return false;
	}

	if(type == v8ot_free80 || type == v8ot_free838)
	{
		error("Попытка прямой записи в файл свободных страниц",
			"Номер страницы файла", tohex(block));
		return false;
	}

	delete[] data;
	data = nullptr;
	_length = stream->GetSize();
	set_len(_length);

	stream->Seek(0, soFromBeginning);

	if(type == v8ot_data80)
	{
		for (unsigned i = 0; i < numblocks; i++)
		{
			objtab *b = (objtab*)base->getblock(blocks[i]);
			for(int32_t j = 0; j < b->numblocks; j++)
			{
				curlen = _length > 0x1000 ? 0x1000 : _length;
				char *tt = base->getblock_for_write(b->blocks[j], false);
				stream->ReadBuffer(tt, curlen);
				if(_length <= curlen) break;
				_length -= curlen;
			}
			if(_length <= curlen) break;
		}

		write_new_version();
		return true;
	}
	else if(type == v8ot_data838)
	{
		curblock = 0;
		curlen = MIN(base->pagesize, _length);

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
				curlen = MIN(base->pagesize, _length);
				if(_length > 0) if(curoffobjblock >= offsperpage)
				{
					curoffobjblock = 0;
					bb = (objtab838*) base->getblock(blocks[curobjblock++]);
				}
			}
		}
		else
		{
			while(_length)
			{
				stream->ReadBuffer(base->getblock_for_write(blocks[curblock++], false), curlen);
				_length -= curlen;
				curlen = MIN(base->pagesize, _length);
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
		error("Попытка записи в файл в режиме \"Только чтение\"",
			"Номер страницы файла", tohex(block));
		return false;
	}

	if(type == v8ot_free80 || type == v8ot_free838)
	{
		error("Попытка прямой записи в файл свободных страниц",
			"Номер страницы файла", tohex(block));
		return false;
	}

	lastdataget = GetTickCount();

	delete[] data;
	data = NULL;
	if(_start + _length > len) set_len(_start + _length);

	if(type == v8ot_data80)
	{
		curblock = _start >> 12;
		curoffblock = _start - (curblock << 12);
		curlen = MIN(0x1000 - curoffblock, _length);

		curobjblock = curblock / 1023;
		curoffobjblock = curblock - curobjblock * 1023;

		objtab *b = (objtab*) base->getblock(blocks[curobjblock++]);
		while(_length)
		{
			stream->ReadBuffer((char*)(base->getblock_for_write(b->blocks[curoffobjblock++], curlen != 0x1000)) + curoffblock, curlen);
			_length -= curlen;
			curoffblock = 0;
			curlen = MIN(0x1000, _length);
			if(_length > 0) if(curoffobjblock >= 1023)
			{
				curoffobjblock = 0;
				b = (objtab*) base->getblock(blocks[curobjblock++]);
			}
		}

		write_new_version();
		return true;
	}
	else if(type == v8ot_data838)
	{
		curblock = _start / base->pagesize;
		curoffblock = _start - (curblock * base->pagesize);
		curlen = MIN(base->pagesize - curoffblock, _length);

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
				curlen = MIN(base->pagesize, _length);
				if(_length > 0) if(curoffobjblock >= offsperpage)
				{
					curoffobjblock = 0;
					bb = (objtab838*) base->getblock(blocks[curobjblock++]);
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
				curlen = MIN(base->pagesize, _length);
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

	if(type == v8ot_free80 || type == v8ot_free838)
	{
		// Таблица свободных блоков
		error("Попытка установки длины в файле свободных страниц");
		return;
	}

	delete[] data;
	data = NULL;

	if(type == v8ot_data80)
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
			else blocks = NULL;
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
	else if(type == v8ot_data838)
	{
		offsperpage = base->pagesize / 4;
		maxlen = base->pagesize * offsperpage * (offsperpage - 6);
		if(_len > maxlen)
		{
			error("Попытка установки длины файла больше максимальной",
				"Номер страницы файла", tohex(block),
				"Максимальная длина файла", maxlen,
				"Запрошенная длина файла", tohex(_len));
			_len = maxlen;
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
			else blocks = NULL;
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
		error("Попытка установки свободного блока в объекте, не являющимся таблицей свободных блоков",
			"Блок объекта", block);
		return;
	}

	int32_t j = len >> 10; // length / 1024
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
		error("Попытка получения свободного блока в объекте, не являющимся таблицей свободных блоков",
			"Блок объекта", block);
		return 0;
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
		unsigned i = memblock::get_numblocks();
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
	int32_t veroffset = type == v8ot_data80 || type == v8ot_free80 ? 12 : 4;

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
		error("Попытка чтения нулевого блока файла Blob",
			"Номер страницы файла", tohex(block));
		return _str;
	}

	_numblock = len >> 8;
	if(_numblock << 8 != len)
	{
		error("Длина файла Blob не кратна 0x100",
			"Номер страницы файла", tohex(block),
			"Длина файла", tohex(len));
	}

	_curb = new char[0x100];
	_curblock = _startblock;
	while(_curblock)
	{
		if(_curblock >= _numblock)
		{
			error("Попытка чтения блока файла Blob за пределами файла",
				"Номер страницы файла", tohex(block),
				"Всего блоков", _numblock,
				"Читаемый блок", _curblock);
			return _str;
		}
		getdata(_curb, _curblock << 8, 0x100);
		_curblock = *(uint32_t*)_curb;
		uint16_t _curlen = *(uint16_t*)(_curb + 4);
		if(_curlen > 0xfa)
		{
			error("Попытка чтения из блока файла Blob более 0xfa байт",
				"Номер страницы файла", tohex(block),
				"Индекс блока", _curblock,
				"Читаемых байт", _curlen);
			return _str;
		}
		_str->Write(_curb + 6, _curlen);

		if(_str->GetSize() - startlen > _length) break; // аварийный выход из возможного ошибочного зацикливания
	}
	delete[] _curb;

	if(_length != MAXUINT) if(_str->GetSize() - startlen != _length)
	{
		error("Несовпадение длины Blob-поля, указанного в записи, с длиной практически прочитанных данных",
			"Номер страницы файла", tohex(block),
			"Длина поля", _length,
			"Прочитано", _str->GetSize() - startlen);
	}

	return _str;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс index

//---------------------------------------------------------------------------
index::index(Table* _base)
{
	tbase       = _base;
    err         = nullptr;
	is_primary  = false;
	num_records = 0;
	records     = NULL;
	start       = 0;
	rootblock   = 0;
	length      = 0;
	recordsindex_complete = false;
	pagesize = tbase->base->pagesize;
	version  = tbase->base->version;
}

//---------------------------------------------------------------------------
index::~index()
{
	delete[] records;
}

//---------------------------------------------------------------------------
String index::getname()
{
	return name;
}

//---------------------------------------------------------------------------
bool index::get_is_primary()
{
	return is_primary;
}

//---------------------------------------------------------------------------
int32_t index::get_num_records()
{
	return num_records;
}

//---------------------------------------------------------------------------
index_record* index::get_records()
{
	return records;
}

//---------------------------------------------------------------------------
uint32_t index::get_numrecords()
{
	if(!start) return 0;
	if(!recordsindex_complete) create_recordsindex();
	return recordsindex.GetLength();
}

//---------------------------------------------------------------------------
uint32_t index::get_numrec(uint32_t num_record)
{
	if(!start) return 0;
	if(!recordsindex_complete) create_recordsindex();
	return recordsindex[num_record];
}

//---------------------------------------------------------------------------
void index::create_recordsindex()
{
	char* buf;
	int32_t curlen;
	uint64_t curblock;
	uint32_t mask;
	v8object* file_index;

	if(!start) return;

	String readindex("Чтение индекса ");
	msreg->Status(readindex);

	buf = new char[pagesize];

	file_index = tbase->file_index;
	file_index->getdata(buf, start, 8);

	rootblock = *(uint32_t*)buf;
	if(version >= ver8_3_8_0) rootblock *= pagesize;
	length = *(int16_t*)(buf + 4);

	curblock = rootblock;

	file_index->getdata(buf, curblock, pagesize);
	curlen = *(int16_t*)(buf + 2);
	if(curlen)
	{
		recordsindex.SetLength(tbase->file_data->getlen() / tbase->recordlen);
		bool is_leaf = buf[0] & 0x2;
		while(!is_leaf)
		{
			curblock = *(uint32_t*)(buf + 16 + length);
			curblock = 	reverse_byte_order(curblock);
			if(version >= ver8_3_8_0) curblock *= pagesize;
			file_index->getdata(buf, curblock, pagesize);
			is_leaf = buf[0] & 0x2;
		}

		unsigned curindex = 0;
		while(true)
		{
			curlen = *(int16_t*)(buf + 2);
			curblock = *(uint32_t*)(buf + 8);
			mask = *(uint32_t*)(buf + 14);
			int32_t rlen = *(int16_t*)(buf + 28);
			char *rbuf = buf + 30;
			for (int32_t i = 0; i < curlen; i++)
			{
				recordsindex[curindex++] = *(uint32_t*)rbuf & mask;
				rbuf += rlen;
				if(curindex % 10000 == 0) msreg->Status(readindex + curindex);
			}
			if(curblock == 0xffffffff) break;
			if(version >= ver8_3_8_0) curblock *= pagesize;
			file_index->getdata(buf, curblock, pagesize);
		}
		recordsindex.SetLength(curindex);
	}

	recordsindex_complete = true;
	delete[] buf;
	tbase->log_numrecords = recordsindex.GetLength();
	msreg->Status("");
}

//---------------------------------------------------------------------------
#ifndef PublicRelease
void index::dump_recursive(v8object* file_index, TFileStream* f, int32_t level, uint64_t curblock)
{
	unsigned char bf[3];
	char* buf;
	char* rbuf;
	uint32_t curlen;
	uint32_t lastnumrec;
	String s;

	char* curindex;

	leaf_page_header* lph;
	branch_page_header* bph;

	buf = new char[pagesize];
	lph = (leaf_page_header*)buf;
	bph = (branch_page_header*)buf;
	bf[2] = ' ';
	file_index->getdata(buf, curblock, pagesize);
	curlen = bph->number_indexes;
	if(curlen)
	{
		bool is_leaf = bph->flags & indexpage_is_leaf;
		if(is_leaf)
		{
			curindex = new char[length]; //
			memset(curindex, 0, length);

			unsigned numrecmask = lph->numrecmask;
			unsigned leftmask = lph->leftmask;
			unsigned rightmask = lph->rightmask;
			unsigned numrecbits = lph->numrecbits;
			unsigned leftbits = lph->leftbits;
			//rightbits = lph->rightbits;
			unsigned recbytes = lph->recbytes;
			rbuf = buf + 30;
			char *ibuf = buf + pagesize;
			unsigned previous_right = length;

			s = "=";
			s += level;
			s += " leaf, curblock ";
			s += curblock == 0xffffffff ? -1 : (curblock / pagesize);
			s += ", count ";
			s += curlen;
			s += ", free ";
			s += lph->freebytes;
			s += ", rec ";
			s += numrecbits;
			s += ", left ";
			s += leftbits;
			s += ", right ";
			s += lph->rightbits;
			s += ", bytes ";
			s += recbytes;
			s += "\r\n";
			f->Write(s.c_str(), s.GetLength());

			for (unsigned i = 0; i < curlen; i++)
			{
				int64_t indrec = *(int64_t*)rbuf;
				unsigned numrec = indrec & numrecmask;
				indrec >>= numrecbits;
				unsigned left =  indrec & leftmask;
				indrec >>= leftbits;
				unsigned right =  indrec & rightmask;
				rbuf += recbytes;
				unsigned j = length - left - right;
				ibuf -= j;
				memcpy(curindex + left, ibuf, j);
				if(right > previous_right) memset(curindex + length - right, 0, right - previous_right);
				previous_right = right;

				s = "  -";
				s += level;
				s += ": ";
				f->Write(s.c_str(), s.GetLength());
				for(j = 0; j < length; j++)
				{
					unsigned char b = curindex[j];
					b >>= 4;
					b += '0';
					if(b > '9') b += 'a' - '9' - 1;
					bf[0] = b;
					b = curindex[j];
					b &= 0xf;
					b += '0';
					if(b > '9') b += 'a' - '9' - 1;
					bf[1] = b;
					f->Write(bf, 3);
				}
				s = ": ";
				s += numrec;
				s += "\r\n";
				f->Write(s.c_str(), s.GetLength());
			}
			delete[] curindex;

		}
		else
		{
			s = "*";
			s += level;
			s += " branch, curblock ";
			s += curblock == 0xffffffff ? -1 : (curblock / pagesize);
			s += ", count ";
			s += curlen;
			s += "\r\n";
			f->Write(s.c_str(), s.GetLength());

			rbuf = buf + 12;
			for (unsigned i = 0; i < curlen; i++)
			{
				curindex = rbuf;
				rbuf += length;
				unsigned numrecx = reverse_byte_order(*(uint32_t*)rbuf);
				rbuf += 4;
				uint64_t blockx = reverse_byte_order(*(uint32_t*)rbuf);
				if(version >= ver8_3_8_0) blockx *= pagesize;
				rbuf += 4;

				dump_recursive(file_index, f, level + 1, blockx);

				s = "  +";
				s += level;
				s += ": ";
				f->Write(s.c_str(), s.GetLength());
				for (unsigned j = 0; j < length; j++)
				{
					unsigned char b = curindex[j];
					b >>= 4;
					b += '0';
					if(b > '9') b += 'a' - '9' - 1;
					bf[0] = b;
					b = curindex[j];
					b &= 0xf;
					b += '0';
					if(b > '9') b += 'a' - '9' - 1;
					bf[1] = b;
					f->Write(bf, 3);
				}
				s = ": ";
				s += numrecx;
				s += "\r\n";
				f->Write(s.c_str(), s.GetLength());

			}

		}
	}

	delete[] buf;
}
#endif //#ifdef PublicRelease

//---------------------------------------------------------------------------
uint32_t index::get_rootblock()
{
	if(!start) return 0;

	if(rootblock == 0)
	{
		char buf[8];
		tbase->file_index->getdata(buf, start, 8);
		rootblock = *(uint32_t*)buf;
		if(version >= ver8_3_8_0) rootblock *= pagesize;
		length = *(int16_t*)(buf + 4);
	}
	return rootblock;
}

//---------------------------------------------------------------------------
uint32_t index::get_length()
{
	if(!start) return 0;

	if(rootblock == 0)
	{
		char buf[8];
		tbase->file_index->getdata(buf, start, 8);
		rootblock = *(uint32_t*)buf;
		if(version >= ver8_3_8_0) rootblock *= pagesize;
		length = *(int16_t*)(buf + 4);
	}
	return length;
}

//---------------------------------------------------------------------------
#ifndef PublicRelease
void index::dump(String _filename)
{
	TFileStream* f;
	v8object* file_index;
	String s;

	f = new TFileStream(_filename, fmCreate);

	if(!start)
	{
		delete f;
		return;
	}

	file_index = tbase->file_index;
	if(rootblock == 0)
	{
		char buf[8];
		file_index->getdata(buf, start, 8);
		rootblock = *(uint32_t*)buf;
		if(version >= ver8_3_8_0) rootblock *= pagesize;
		length = *(int16_t*)(buf + 4);
	}

	s = "Index length ";
	s += length;
	s += "\r\n";
	f->Write(s.c_str(), s.GetLength());

	dump_recursive(file_index, f, 0, rootblock);

	delete f;
}
#endif //#ifdef PublicRelease

//---------------------------------------------------------------------------
char* index::unpack_leafpage(uint64_t page_offset, uint32_t& number_indexes)
{
	char* buf;
	char* ret;

	if(!tbase->file_index) return NULL;

	buf = new char[pagesize];
	tbase->file_index->getdata(buf, page_offset, pagesize);
	ret = unpack_leafpage(buf, number_indexes);
	delete[] buf;
	return ret;
}

//---------------------------------------------------------------------------
char* index::unpack_leafpage(char* page, uint32_t& number_indexes)
{
	char* outbuf;
	char* rbuf;
	char* ibuf;
	char* obuf;
	leaf_page_header* header;

	uint32_t i, j, step;

	if(length == 0)
	{
		number_indexes = 0;
		return NULL;
	}

	header = (leaf_page_header*)page;

	if(!(header->flags & indexpage_is_leaf))
	{
		error("Попытка распаковки страницы индекса не являющейся листом.",
			"Таблица", tbase->name,
			"Индекс", name);
		number_indexes = 0;
		return NULL;
	}

	number_indexes = header->number_indexes;
	if(!number_indexes)
	{
		return NULL;
	}

	unsigned numrecmask = header->numrecmask;
	unsigned leftmask = header->leftmask;
	unsigned rightmask = header->rightmask;
	unsigned numrecbits = header->numrecbits;
	unsigned leftbits = header->leftbits;
	unsigned recbytes = header->recbytes;

	step = length + 4;
	outbuf = new char[number_indexes * step];

	rbuf = page + 30;
	ibuf = page + pagesize;
	obuf = outbuf;

	for(i = 0; i < number_indexes; i++)
	{
		int64_t indrec = *(int64_t*)rbuf;
		unsigned numrec = indrec & numrecmask;
		indrec >>= numrecbits;
		unsigned left = indrec & leftmask;
		indrec >>= leftbits;
		unsigned right = indrec & rightmask;
		rbuf += recbytes;
		j = length - left - right;
		ibuf -= j;

		*(uint32_t*)obuf = numrec;
		obuf += 4;

		if(left) memcpy(obuf, obuf - step, left);
		if(j) memcpy(obuf + left, ibuf, j);
		obuf += length;
		if(right) memset(obuf - right, 0, right);

	}

	return outbuf;
}

//---------------------------------------------------------------------------
bool index::pack_leafpage(char* unpack_index, uint32_t number_indexes, char* page_buf)
{
	uint32_t min_numrec_bits;
	uint32_t min_bits;
	uint32_t max_numrec;

	leaf_page_header* header;
	uint32_t freebytes;
	uint32_t numrecmask;
	uint32_t leftmask;
	uint32_t rightmask;
	uint32_t numrecbits;
	uint32_t leftbits;
	uint32_t rightbits;
	uint32_t recbytes;

	uint32_t i, j;
	uint32_t step;
	uint32_t pack_index_len;
	uint32_t left, right;
	uint32_t numrec;
	char* cur;
	char* curp;
	int64_t indrec;
	uint32_t datasize;

	datasize = pagesize - 30;
	for(i = length, rightbits = 0; i; i >>= 1, rightbits++);
	leftbits = rightbits;

	step = length + 4;

	max_numrec = 0;
	cur = unpack_index;
	curp = page_buf + pagesize;
	pack_index_len = 0;

	_pack_index_record* _pack_index_record_array = new _pack_index_record[number_indexes];
	memset(page_buf + 30, 0, datasize);

	for(i = 0; i < number_indexes; i++)
	{
		numrec = *(uint32_t*)cur;
		cur += 4;

		if(i)
		{
			for(j = 0; cur[j] == cur[j - step] && j < length; j++);
			left = j;
		}
		else left = 0;

		for(j = 1; cur[length - j] == 0 && j <= length; j++);
		right = j - 1;


		if(left + right > length)
		{
			if(left < length || is_primary)
			{
				error("Ошибка упаковки индексов на странице-листе. Индекс не уникальный либо неверно отсортирован.",
					"Таблица", tbase->name,
					"Индекс", name);

			}
			right = length - left;
		}
		j = length - left - right;
		curp -= j;
		pack_index_len += j;

		if(pack_index_len >= datasize)
		{
			delete[] _pack_index_record_array;
			return false;
		}

		memcpy(curp, cur + left, j);

		cur += length;

		if(max_numrec < numrec) max_numrec = numrec;

		_pack_index_record_array[i].numrec = numrec;
		_pack_index_record_array[i].left = left;
		_pack_index_record_array[i].right = right;
	}
	for(min_numrec_bits = 0, i = max_numrec; i; i >>= 1, min_numrec_bits++);

	//if(min_numrec_bits < 11) min_numrec_bits = 11; // 11 - эмпиричиски вычислено как 4066 / 2 = 2033 - максимальное кол-во записей на странице. 2033 укладывается в 11 бит
	min_bits = min_numrec_bits + leftbits + rightbits;
	recbytes = (min_bits + 7) >> 3;

	if(recbytes * number_indexes + pack_index_len > datasize)
	{
		delete[] _pack_index_record_array;
		return false;
	}
	freebytes = datasize - recbytes * number_indexes - pack_index_len;

	numrecbits = recbytes * 8 - leftbits - rightbits;

	for(i = 0, numrecmask = 0; i < numrecbits; i++, numrecmask = (numrecmask << 1) | 1);
	for(i = 0, leftmask = 0; i < leftbits; i++, leftmask = (leftmask << 1) | 1);
	for(i = 0, rightmask = 0; i < rightbits; i++, rightmask = (rightmask << 1) | 1);

	header = (leaf_page_header*)page_buf;
	header->flags = indexpage_is_leaf;
	header->number_indexes = number_indexes;
	header->freebytes = freebytes;
	header->numrecmask = numrecmask;
	header->leftmask = leftmask;
	header->rightmask = rightmask;
	header->numrecbits = numrecbits;
	header->leftbits = leftbits;
	header->rightbits = rightbits;
	header->recbytes = recbytes;

	for(i = 0, curp = page_buf + 30; i < number_indexes; i++, curp += recbytes)
	{
		indrec = _pack_index_record_array[i].right;
		indrec <<= leftbits;
		indrec |= _pack_index_record_array[i].left;
		indrec <<= numrecbits;
		indrec |= _pack_index_record_array[i].numrec;

		memcpy(curp, &indrec, recbytes);
	}

	delete[] _pack_index_record_array;
	return true;
}

//---------------------------------------------------------------------------
#ifndef PublicRelease
void index::calcRecordIndex(const char* rec, char* indexBuf)
{
	int32_t i, j, k;

	j = length;
	for(i = 0; i < num_records; i++)
	{
		k = records[i].field->getSortKey(rec, (unsigned char *)indexBuf, j);
		indexBuf += k;
		j -= k;
	}
	if(j) memset(indexBuf, 0, j);
}

//---------------------------------------------------------------------------
void index::delete_index(const char* rec, const uint32_t phys_numrec)
{
	char* index_buf;
	index_buf = new char[length];
	calcRecordIndex(rec, index_buf);
	delete_index_record(index_buf, phys_numrec);
	delete[] index_buf;
}

//---------------------------------------------------------------------------
void index::delete_index_record(const char* index_buf, const uint32_t phys_numrec)
{
	bool is_last_record, page_is_empty; // заглушки для вызова рекурсивной функции
	uint32_t new_last_phys_num; // заглушки для вызова рекурсивной функции
	char* new_last_index_buf = new char[length]; // заглушки для вызова рекурсивной функции
	delete_index_record(index_buf, phys_numrec, rootblock, is_last_record, page_is_empty, new_last_index_buf, new_last_phys_num);
	delete[] new_last_index_buf;
}

//---------------------------------------------------------------------------
void index::delete_index_record(const char* index_buf, const uint32_t phys_numrec, uint64_t block, bool& is_last_record, bool& page_is_empty, char* new_last_index_buf, uint32_t& new_last_phys_num)
{
	char* page;
	branch_page_header* bph;
	leaf_page_header* lph;
	branch_page_header* bph2;
	bool _is_last_record, _page_is_empty;
	uint32_t _new_last_phys_num;
	uint32_t number_indexes;
	char* unpack_indexes_buf;
	int16_t flags;
	uint64_t _block_;

	char* cur_index;
	int32_t i, j, k, delta;

	page = new char[pagesize];
	tbase->file_index->getdata(page, block, pagesize);

	is_last_record = false;
	page_is_empty = false;

	if(*page & indexpage_is_leaf)
	{
		// страница-лист
		lph = (leaf_page_header*)page;
		flags = lph->flags;
		unpack_indexes_buf = unpack_leafpage(page, number_indexes);
		cur_index = unpack_indexes_buf;
		delta = length + 4;
		for(i = 0; i < lph->number_indexes; i++, cur_index += delta)
		{
			j = memcmp(index_buf, cur_index + 4, length);
			if(j == 0 && *(uint32_t*)cur_index == phys_numrec)
			{
				if(i == lph->number_indexes - 1) is_last_record = true;

				lph->number_indexes--;
				for(k = i; k < lph->number_indexes; k++) memcpy(unpack_indexes_buf + k * delta, unpack_indexes_buf + (k + 1) * delta, delta);

				if(lph->number_indexes == 0)
				{
					page_is_empty = true;
					if(lph->prev_page != 0xffffffff)
					{
						tbase->file_index->setdata(&(lph->next_page), (version < ver8_3_8_0 ? lph->prev_page : lph->prev_page * pagesize) + 8, 4);
					}
					if(lph->next_page != 0xffffffff)
					{
						tbase->file_index->setdata(&(lph->prev_page), (version < ver8_3_8_0 ? lph->next_page : lph->next_page * pagesize) + 4, 4);
					}
					// TODO проверить, надо ли номера свободных страниц преобразовывать в смещения для версий от 8.0 до 8.2.14
					tbase->file_index->getdata(&k, 0, 4);
					memset(page, 0, pagesize);
					*(uint32_t*)page = k;
					k = block / pagesize;
					tbase->file_index->setdata(&k, 0, 4);
				}
				else
				{
					if(is_last_record)
					{
						cur_index = unpack_indexes_buf + (lph->number_indexes - 1) * delta;
						memcpy(new_last_index_buf, cur_index + 4, length);
						new_last_phys_num = *(uint32_t*)cur_index;
					}
					pack_leafpage(unpack_indexes_buf, lph->number_indexes, page);
					lph->flags = flags;
				}
				tbase->file_index->setdata(page, block, pagesize);

				break;
			}
		}
		delete[] unpack_indexes_buf;
	}
	else
	{
		// страница-ветка
		bph = (branch_page_header*)page;

		cur_index = page + 12; // 12 = size_of(branch_page_header)
		delta = length + 8;
		for(i = 0; i < bph->number_indexes; i++, cur_index += delta)
		{
			j = memcmp(index_buf, cur_index, length);
			if(j <= 0)
			{
				if(i == bph->number_indexes - 1 && j==0) is_last_record = true;

				_block_ = reverse_byte_order(*(uint32_t*)(cur_index + length + 4));
				if(version >= ver8_3_8_0) _block_ *= pagesize;
				delete_index_record(index_buf, phys_numrec, _block_, _is_last_record, _page_is_empty, new_last_index_buf, _new_last_phys_num);

				if(_page_is_empty)
				{
					bph->number_indexes--;
					if(bph->number_indexes > i) memcpy(cur_index, cur_index + delta, (bph->number_indexes - i) * delta);
					memset(page + 12 + bph->number_indexes * delta, 0, delta);
				}
				else if(_is_last_record)
				{
					memcpy(cur_index, new_last_index_buf, length);
					*(uint32_t*)(cur_index + length) = reverse_byte_order(_new_last_phys_num);
				}

				if(bph->number_indexes == 0)
				{
					page_is_empty = true;
					if(bph->prev_page != 0xffffffff)
					{
						tbase->file_index->setdata(&(bph->next_page), (version < ver8_3_8_0 ? bph->prev_page : bph->prev_page * pagesize) + 8, 4);
					}
					if(bph->next_page != 0xffffffff)
					{
						tbase->file_index->setdata(&(bph->prev_page), (version < ver8_3_8_0 ? bph->next_page : bph->next_page * pagesize) + 4, 4);
					}
					tbase->file_index->getdata(&k, 0, 4);
					memset(page, 0, pagesize);
					*(uint32_t*)page = k;
					k = block / pagesize;
					tbase->file_index->setdata(&k, 0, 4);
				}
				else
				{
					if(is_last_record)
					{
						cur_index = page + 12 + (bph->number_indexes - 1) * delta;
						memcpy(new_last_index_buf, cur_index, length);
						new_last_phys_num = reverse_byte_order(*(uint32_t*)(cur_index + length));
					}
				}
				if(_page_is_empty || _is_last_record || page_is_empty || is_last_record) tbase->file_index->setdata(page, block, pagesize);

				break;
			}
		}
	}

	delete[] page;
}

//---------------------------------------------------------------------------
void index::write_index(const uint32_t phys_numrecord, const char* rec)
{
	char* index_buf;
	index_buf = new char[length];
	calcRecordIndex(rec, index_buf);
	write_index_record(phys_numrecord, index_buf);
	delete[] index_buf;

}

//---------------------------------------------------------------------------
void index::write_index_record(const uint32_t phys_numrecord, const char* index_buf)
{
	int32_t result;
	char* new_last_index_buf = new char[length];
	uint32_t new_last_phys_num;
	char* new_last_index_buf2 = new char[length];
	uint32_t new_last_phys_num2;
	uint64_t new_last_block2;

	char* page;
	branch_page_header* bph;
	uint64_t block;
	uint32_t k;
	char* cur_index;

	write_index_record(phys_numrecord, index_buf, rootblock, result, new_last_index_buf, new_last_phys_num, new_last_index_buf2, new_last_phys_num2, new_last_block2);

	if(result == 2)
	{
		page = new char[pagesize];
		memset(page, 0, pagesize);
		bph = (branch_page_header*)page;

		tbase->file_index->getdata(&k, 0, 4);
		if(k)
		{
			block = k * pagesize;
			tbase->file_index->getdata(&k, block, 4);
			tbase->file_index->setdata(&k, 0, 4);
		}
		else block = tbase->file_index->getlen();

		bph->flags = indexpage_is_root | indexpage_is_leaf;
		bph->number_indexes = 2;
		bph->prev_page = 0xffffffff;
		bph->next_page = 0xffffffff;

		cur_index = page + 12;
		memcpy(cur_index, new_last_index_buf, length);
		cur_index += length;
		*(uint32_t*)cur_index = reverse_byte_order(new_last_phys_num);
		cur_index += 4;
		*(uint32_t*)cur_index = reverse_byte_order(version < ver8_3_8_0 ? rootblock : rootblock / pagesize);
		cur_index += 4;

		memcpy(cur_index, new_last_index_buf2, length);
		cur_index += length;
		*(uint32_t*)cur_index = reverse_byte_order(new_last_phys_num2);
		cur_index += 4;
		*(uint32_t*)cur_index = reverse_byte_order(version < ver8_3_8_0 ? new_last_block2 : new_last_block2 / pagesize);

		tbase->file_index->setdata(page, block, pagesize);

		rootblock = block;
		tbase->file_index->setdata(&rootblock, start, 4);

		delete[] page;
	}

	delete[] new_last_index_buf;
	delete[] new_last_index_buf2;
}

//---------------------------------------------------------------------------
void index::write_index_record(const uint32_t phys_numrecord, const char* index_buf, uint64_t block, int32_t& result, char* new_last_index_buf, uint32_t& new_last_phys_num, char* new_last_index_buf2, uint32_t& new_last_phys_num2, uint64_t& new_last_block2)
{
	// result - результат добавления.
	// 0 - ничего делать не надо.
	// 1 - надо заменить запись на new_last_index_buf.
	// 2 - произошло разбиение на 2 страницы, надо заменить на 2 записи

	char* page;
	branch_page_header* bph;
	leaf_page_header* lph;

	int32_t _result;
	uint32_t _new_last_phys_num;
	uint32_t _new_last_phys_num2;
	uint64_t _new_last_block2;

	char* unpack_indexes_buf;
	char* unpack_indexes_buf_new;

	int16_t flags;
	uint32_t number_indexes;
	uint64_t prev_page;
	uint64_t next_page;

	char* page2;
	branch_page_header* bph2;
	leaf_page_header* lph2;
	uint32_t number_indexes1;
	uint32_t number_indexes2;
	uint64_t block1;
	uint32_t max_num_indexes;

	char* cur_index;
	char* cur_index2;
	uint32_t i, k;
	int32_t j, delta;
	bool ok;

	page = new char[pagesize];
	tbase->file_index->getdata(page, block, pagesize);
	result = 0;

	bph = (branch_page_header*)page;
	flags = bph->flags;
	number_indexes = bph->number_indexes;
	prev_page = bph->prev_page;
	next_page = bph->next_page;

	if(flags & indexpage_is_leaf)
	{
		// страница-лист
		lph = (leaf_page_header*)page;
		unpack_indexes_buf = unpack_leafpage(page, number_indexes);
		cur_index = unpack_indexes_buf;
		delta = length + 4;
		ok = true;
		for(i = 0; i < number_indexes; i++, cur_index += delta)
		{
			j = memcmp(index_buf, cur_index + 4, length);


			if(j == 0)
			{
				if(is_primary || *(uint32_t*)cur_index == phys_numrecord)
				{
					error("Ошибка записи индекса. Индекс уже существует.",
						"Таблица", tbase->name,
						"Индекс", name,
						"Физический номер существующий", *(uint32_t*)cur_index,
						"Физический номер записываемый", phys_numrecord);
					ok = false;
				}
				break;
			}
			if(j < 0) break;
		}
		if(ok)
		{
			if(i == number_indexes) result = 1;

			number_indexes++;
			unpack_indexes_buf_new = new char[delta * number_indexes];

			if(i) memcpy(unpack_indexes_buf_new, unpack_indexes_buf, delta * i);
			*(uint32_t*)(unpack_indexes_buf_new + i * delta) = phys_numrecord;
			memcpy(unpack_indexes_buf_new + i * delta + 4, index_buf, length);
			if(number_indexes - i - 1) memcpy(unpack_indexes_buf_new + (i + 1) * delta, unpack_indexes_buf + i * delta, delta * (number_indexes - i - 1));

			if(pack_leafpage(unpack_indexes_buf_new, number_indexes, page))
			{
				if(result == 1)
				{
					cur_index = unpack_indexes_buf_new + (number_indexes - 1) * delta;
					memcpy(new_last_index_buf, cur_index + 4, length);
					new_last_phys_num = *(uint32_t*)cur_index;
				}
				lph->flags = flags;
				lph->prev_page = prev_page;
				lph->next_page = next_page;
			}
			else
			{
				result = 2;
				char* page2 = new char[pagesize];
				number_indexes1 = number_indexes >> 1;
				number_indexes2 = number_indexes - number_indexes1;
				pack_leafpage(unpack_indexes_buf_new, number_indexes1, page);
				pack_leafpage(unpack_indexes_buf_new + number_indexes1 * delta, number_indexes2, page2);
				lph2 = (leaf_page_header*)page2;

				tbase->file_index->getdata(&k, 0, 4);
				if(k)
				{
					new_last_block2 = k * pagesize;
					tbase->file_index->getdata(&k, new_last_block2, 4);
					tbase->file_index->setdata(&k, 0, 4);
				}
				else new_last_block2 = tbase->file_index->getlen();

				flags &= ~indexpage_is_root;
				lph->flags = flags;
				lph->prev_page = prev_page;
				lph->next_page = version < ver8_3_8_0 ? new_last_block2 : new_last_block2 / pagesize;
				lph2->flags = flags;
				lph2->prev_page = version < ver8_3_8_0 ? block : block / pagesize;
				lph2->next_page = next_page;

				tbase->file_index->setdata(page2, new_last_block2, pagesize);

				cur_index = unpack_indexes_buf_new + (number_indexes1 - 1) * delta;
				memcpy(new_last_index_buf, cur_index + 4, length);
				new_last_phys_num = *(uint32_t*)cur_index;
				cur_index = unpack_indexes_buf_new + (number_indexes - 1) * delta;
				memcpy(new_last_index_buf2, cur_index + 4, length);
				new_last_phys_num2 = *(uint32_t*)cur_index;

				delete[] page2;
			}

			tbase->file_index->setdata(page, block, pagesize);
			delete[] unpack_indexes_buf_new;
		}


		delete[] unpack_indexes_buf;
	}
	else
	{
		// страница-ветка

		cur_index = page + 12; // 12 = size_of(branch_page_header)
		delta = length + 8;
		max_num_indexes = (pagesize - 12) / delta;

		ok = true;
		for(i = 0; i < number_indexes; i++, cur_index += delta)
		{
			j = memcmp(index_buf, cur_index, length);
			if(j == 0)
			{
				if(is_primary)
				{
					error("Ошибка записи индекса. Индекс уже существует.",
						"Таблица", tbase->name,
						"Индекс", name,
						"Физический номер существующий", reverse_byte_order(*(uint32_t*)(cur_index + length)),
						"Физический номер записываемый", phys_numrecord);
					ok = false;
				}
				break;
			}
			if(j < 0) break;
		}

		if(i == number_indexes)
		{
			i--;
			cur_index -= delta;
		}

		if(ok)
		{
			block1 = reverse_byte_order(*(uint32_t*)(cur_index + length + 4));
			if(version >= ver8_3_8_0) block1 *= pagesize;
			write_index_record(phys_numrecord, index_buf, block1, _result, new_last_index_buf, _new_last_phys_num, new_last_index_buf2, _new_last_phys_num2, _new_last_block2);

			if(_result == 1)
			{
				memcpy(cur_index, new_last_index_buf, length);
				*(uint32_t*)(cur_index + length) = reverse_byte_order(_new_last_phys_num);
				if(i == number_indexes - 1)
				{
					result = 1;
					new_last_phys_num = _new_last_phys_num;
					// new_last_index_buf остаётся!
				}
			}
			else if(_result == 2)
			{
			    char* page2 = new char[pagesize];
			    unpack_indexes_buf = new char[delta * number_indexes];

				number_indexes++;
				if(number_indexes > max_num_indexes)
				{
					result = 2;


					tbase->file_index->getdata(&k, 0, 4);
					if(k)
					{
						new_last_block2 = k << 12;
						tbase->file_index->getdata(&k, new_last_block2, 4);
						tbase->file_index->setdata(&k, 0, 4);
					}
					else new_last_block2 = tbase->file_index->getlen();

					flags &= ~indexpage_is_root;

					cur_index = unpack_indexes_buf;
					cur_index2 = page + 12;
					k = i * delta;
					if(k) memcpy(cur_index, cur_index2, k);
					cur_index += k;
					cur_index2 += k + delta;

					memcpy(cur_index, new_last_index_buf, length);
					cur_index += length;
					*(uint32_t*)cur_index = reverse_byte_order(_new_last_phys_num);
					cur_index += 4;
					*(uint32_t*)cur_index = reverse_byte_order(version < ver8_3_8_0 ? block1 : block1 / pagesize);
					cur_index += 4;

					memcpy(cur_index, new_last_index_buf2, length);
					cur_index += length;
					*(uint32_t*)cur_index = reverse_byte_order(_new_last_phys_num2);
					cur_index += 4;
					*(uint32_t*)cur_index = reverse_byte_order(version < ver8_3_8_0 ? _new_last_block2 : _new_last_block2 / pagesize);
					cur_index += 4;

					k = (number_indexes - i - 2) * delta;
					if(k) memcpy(cur_index, cur_index2, k);

					number_indexes1 = number_indexes >> 1;
					number_indexes2 = number_indexes - number_indexes1;

					bph2 = (branch_page_header*)page2;
					memset(page, 0, pagesize);
					memset(page2, 0, pagesize);

					bph->flags = flags;
					bph->number_indexes = number_indexes1;
					bph->prev_page = prev_page;
					bph->next_page = version < ver8_3_8_0 ? new_last_block2 : new_last_block2 / pagesize;
					bph2->flags = flags;
					bph2->number_indexes = number_indexes2;
					bph2->prev_page = version < ver8_3_8_0 ? block : block / pagesize;
					bph2->next_page = next_page;

					memcpy(page + 12, unpack_indexes_buf, number_indexes1 * delta);
					memcpy(page2 + 12, unpack_indexes_buf + number_indexes1 * delta, number_indexes2 * delta);

					tbase->file_index->setdata(page2, new_last_block2, pagesize);

					cur_index = unpack_indexes_buf + (number_indexes1 - 1) * delta;
					memcpy(new_last_index_buf, cur_index, length);
					cur_index += length;
					new_last_phys_num = reverse_byte_order(*(uint32_t*)cur_index);
					cur_index = unpack_indexes_buf + (number_indexes - 1) * delta;
					memcpy(new_last_index_buf2, cur_index + 4, length);
					cur_index += length;
					new_last_phys_num2 = reverse_byte_order(*(uint32_t*)cur_index);

					delete[] unpack_indexes_buf;
					delete[] page2;
				}
				else
				{

					k = (number_indexes - i - 2) * delta;
					if(k)
					{
						cur_index = page + 12 + ((i + 1) * delta);
						cur_index2 = page + 12 + ((i + 2) * delta);
						memcpy(cur_index, cur_index2, k);
					}

					cur_index = page + 12 + (i * delta);
					memcpy(cur_index, new_last_index_buf, length);
					cur_index += length;
					*(uint32_t*)cur_index = reverse_byte_order(_new_last_phys_num);
					cur_index += 4;
					*(uint32_t*)cur_index = reverse_byte_order(version < ver8_3_8_0 ? block1 : block1 / pagesize);
					cur_index += 4;

					memcpy(cur_index, new_last_index_buf2, length);
					cur_index += length;
					*(uint32_t*)cur_index = reverse_byte_order(_new_last_phys_num2);
					cur_index += 4;
					*(uint32_t*)cur_index = reverse_byte_order(version < ver8_3_8_0 ? _new_last_block2 : _new_last_block2 / pagesize);

					if(i == number_indexes - 2)
					{
						result = 1;
						new_last_phys_num = _new_last_phys_num2;
						memcpy(new_last_index_buf, new_last_index_buf2, length);
					}

					delete[] unpack_indexes_buf;
					delete[] page2;
				}
			}

			if(_result) tbase->file_index->setdata(page, block, pagesize);
		}

	}

	delete[] page;

}
#endif //#ifdef PublicRelease

//---------------------------------------------------------------------------

//********************************************************
// Класс field

//---------------------------------------------------------------------------
Field::Field(Table* _parent)
{
	if(!null_index_initialized)
	{
		*null_index = 1;
		memset(null_index + 1, 0, 4095);
		null_index_initialized = true;
	}
	null_exists = false;
	length = 0;
	precision = 0;
	case_sensitive = false;

	parent = _parent;
	len = 0;
	offset = 0;
}

//---------------------------------------------------------------------------
String Field::getname()
{
	return name;
}

//---------------------------------------------------------------------------
int32_t Field::getlen() // возвращает длину поля в байтах
{
	if(len) return len;

	len = null_exists ? 1 : 0;
	switch(type)
	{
		case tf_binary: len += length; break;
		case tf_bool: len += 1; break;
		case tf_numeric: len += (length + 2) >> 1; break;
		case tf_char: len += length * 2; break;
		case tf_varchar: len += length * 2 + 2; break;
		case tf_version: len += 16; break;
		case tf_string: len += 8; break;
		case tf_text: len += 8; break;
		case tf_image: len += 8; break;
		case tf_datetime: len += 7; break;
		case tf_version8: len += 8; break;
		case tf_varbinary: len += length + 2; break;
	}
	return len;
}

//---------------------------------------------------------------------------
// При ignore_showGUID binary16 всегда преобразуется в GUID
String Field::get_presentation(const char* rec, bool EmptyNull, wchar_t Delimiter, bool ignore_showGUID, bool detailed)
{
	char sym;
	int32_t i, j, m;
	bool k;

	unsigned char* fr = (unsigned char*)rec + offset;

	if(null_exists)
	{
		if(fr[0] == 0) return EmptyNull ? "" : "{NULL}";
		fr++;
	}
	switch(type)
	{
		case tf_binary:
			if(length == 16 && (showGUID || ignore_showGUID))
			{
				if(showGUIDasMS) return GUIDasMS(fr);
				else return GUIDas1C(fr);
			}
			else
			{
				for(i = 0; i < length; i++)
				{
					sym = '0' + (fr[i] >> 4);
					if(sym > '9') sym += ('a' - '9' - 1);
					buf[i << 1] = sym;
					sym = '0' + (fr[i] & 0xf);
					if(sym > '9') sym += ('a' - '9' - 1);
					buf[(i << 1) + 1] = sym;
				}
				buf[length << 1] = 0;
			}
			return buf;
		case tf_bool:
			if(fr[0]) return "true";
			return "false";
		case tf_numeric:
			i = 0; // текущий индекс в buf
			k = true; // признак, что значащие цифры еще не начались
			m = length - precision; // позиция десятичной точки слева
			if(fr[0] >> 4 == 0) buf[i++] = '-';
			for(j = 0; j < length; j++)
			{
				if(Delimiter) if(!k) if(m - j > 0) if((m - j) % 3 == 0) buf[i++] = Delimiter;
				if(j == m)
				{
					buf[i++] = '.';
					k = false;
				}
				if(j & 1) sym = fr[(j + 1) >> 1] >> 4;
				else sym = fr[j >> 1] & 0xf;
				if(sym == 0 && k) continue;
				k = false;
				buf[i++] = '0' + sym;
			}

			if(k) buf[i++] = '0';

			buf[i] = 0;
			return buf;
		case tf_char:
			return String((WCHART*)fr, length);
		case tf_varchar:
			i = *(int16_t*)fr;
			return String((WCHART*)(fr + 2), i);
		case tf_version:
			return String(*(int32_t*)fr) + ":" + *(int32_t*)(fr + 4) + ":" + *(int32_t*)(fr + 8) + ":" + *(int32_t*)(fr + 12);
		case tf_version8:
			return String(*(int32_t*)fr) + ":" + *(int32_t*)(fr + 4);
		case tf_string:
			return detailed ? String("{MEMO} [") + tohex(*(int32_t*)fr) + "][" + tohex(*(int32_t*)(fr + 4)) + "]" : String("{MEMO}");
		case tf_text:
			return detailed ? String("{TEXT} [") + tohex(*(int32_t*)fr) + "][" + tohex(*(int32_t*)(fr + 4)) + "]" : String("{TEXT}");
		case tf_image:
			return detailed ? String("{IMAGE} [") + tohex(*(int32_t*)fr) + "][" + tohex(*(int32_t*)(fr + 4)) + "]" : String("{IMAGE}");
		case tf_datetime:
			return date_to_string(fr);
		case tf_varbinary:
			m = *(int16_t*)fr; // длина + смещение
			for(i = 0; i < m; i++)
			{
				sym = '0' + (fr[i + 2] >> 4);
				if(sym > '9') sym += ('a' - '9' - 1);
				buf[i << 1] = sym;
				sym = '0' + (fr[i + 2] & 0xf);
				if(sym > '9') sym += ('a' - '9' - 1);
				buf[(i << 1) + 1] = sym;
			}
			buf[m << 1] = 0;
			return buf;
	}

	return "{?}";
}

//---------------------------------------------------------------------------
bool Field::get_bynary_value(char* binary_value, bool null, String& value)
{
	wchar_t sym;
	int32_t i, j, m, l, p, q;
	bool k, n;
	unsigned char* b;

	unsigned char* fr = (unsigned char*)binary_value;
	memset(fr, 0, len);

	if(null_exists)
	{
		if(null)
		{
			return true;
		}
		*fr = 1;
		fr++;
	}
	switch(type)
	{
		case tf_binary:
			if(value.GetLength() == 0) break;
			j = 1;
			if(length == 16 && showGUID) // TODO Надо доделать для showGUIDasMS
			{
				if(value.GetLength() < 36) break;
				for(i = 12; i < 16; i++) fr[i] = (from_hex_digit(value[j++]) << 4) + from_hex_digit(value[j++]);
				j++;
				for(i = 10; i < 12; i++) fr[i] = (from_hex_digit(value[j++]) << 4) + from_hex_digit(value[j++]);
				j++;
				for(i = 8; i < 10; i++) fr[i] = (from_hex_digit(value[j++]) << 4) + from_hex_digit(value[j++]);
				j++;
				for(i = 0; i < 2; i++) fr[i] = (from_hex_digit(value[j++]) << 4) + from_hex_digit(value[j++]);
				j++;
				for(i = 2; i < 8; i++) fr[i] = (from_hex_digit(value[j++]) << 4) + from_hex_digit(value[j++]);
			}
			else{
				if(value.GetLength() < length * 2) break;
				for(i = 0; i < length; i++) fr[i] = (from_hex_digit(value[j++]) << 4) + from_hex_digit(value[j++]);
			}
			break;
		case tf_bool:
			if(value == "true") *fr = 1;
			else *fr = 0;
			break;
		case tf_numeric:
			l = value.GetLength();
			if(!l) break;

			b = new unsigned char[l];
			k = false; // знак минус
			m = -1; // позиция точки
			n = false; // признак наличия значащих цифр в начале

			for(i = 0, j = 0; i < l; i++)
			{
				sym = value[i + 1];
				if(sym == L'-')
				{
					k = true;
					continue;
				}
				if(sym == L'.')
				{
					m = j;
					n = true;
					continue;
				}
				if(!n) if(sym == L'0')
				{
					continue;
				}
				if(sym >= L'0' || sym <= L'9')
				{
					b[j++] = sym - L'0';
					n = true;
				}
			}
			if(m == -1) m = j;

			// тут имеем:
			// в b значащие цифры
			// k - признак минуса
			// j - всего значащих цифр
			// m - позиция точки (количество цифр до запятой, что одно и то же)

			//     0     1     2     3
			//+-----+-----+-----+-----+
			//I  .  I  .  I  .  I  .  I
			//+-----+-----+-----+-----+
			//  S  0  1  2  3  4  5  6  (номер цифры (полубайта), ниже равен i)

			l = length - precision; // макс. количество цифр до запятой
			if(m > l)
			{
				// значение превышает максимально допустимое, заменяем на все 9ки
				for(i = 0; i < length; i++)
				{
					if(i & 1) fr[(i + 1) >> 1] |= 0x90;
					else fr[(i + 1) >> 1] |= 0x9;
				}
			}
			else
			{
				for(i = l - 1, p = m - 1; p >= 0; i--, p--)
				{
					if(i & 1) fr[(i + 1) >> 1] |= b[p] << 4;
					else fr[(i + 1) >> 1] |= b[p];
				}
				q = MIN(j - m, precision); // количество цифр после запятой
				for(i = l, p = m; p < m + q; i++, p++)
				{
					if(i & 1) fr[(i + 1) >> 1] |= b[p] << 4;
					else fr[(i + 1) >> 1] |= b[p];
				}
			}

			if(!k) *fr |= 0x10; // Знак

			delete[] b;

			break;
		case tf_char:
			l = value.GetLength();
			i = MIN(l, length);
			memcpy(fr, value.c_str(), i << 1);
			while(i < length) ((WCHART*)fr)[i++] = L' ';
			break;
		case tf_varchar:
			l = value.GetLength();
			i = MIN(l, length);
			*(int16_t*)fr = i;
			memcpy(fr + 2, value.c_str(), i * 2);
			while(i < length) ((WCHART*)(fr + 2))[i++] = L' ';
			break;
		case tf_version:
			return false;
		case tf_version8:
			return false;
		case tf_string:
			return false;
		case tf_text:
			return false;
		case tf_image:
			return false;
		case tf_datetime:
			if(value.GetLength() < 19)
			{
				fr[1] = 1;
				fr[2] = 1;
				fr[3] = 1;
			}
			else
			{

				#define correct_spaces(A,B)\
				if(value[B] == L' ')\
				{\
					value[B] = value[A];\
					value[A] = L'0';\
				}\
				if(value[A] == L' ') value[A] = L'0';

				correct_spaces(1,2) // корректируем день
				correct_spaces(4,5) // корректируем месяц
				correct_spaces(12,13) // корректируем часы
				correct_spaces(15,16) // корректируем минуты
				correct_spaces(18,19) // корректируем секунды

				i = 0;
				// корректируем год
				while(value[10] == L' ')
				{
					value[10] = value[9];
					value[9] = value[8];
					value[8] = value[7];
					value[7] = L'0';
					i++;
				}
				while(value[9] == L' ')
				{
					value[9] = value[8];
					value[8] = value[7];
					value[7] = L'0';
					i++;
				}
				while(value[8] == L' ')
				{
					value[8] = value[7];
					value[7] = L'0';
					i++;
				}
				if(value[7] == L' ') value[7] = L'0';

				// дополняем год при необходимости
				switch(i)
				{
					case 1:
						value[7] = L'2';
						break;
					case 2:
						value[7] = L'2';
						value[8] = L'0';
						break;
					case 3:
						value[7] = L'2';
						value[8] = L'0';
						value[9] = L'1';
						break;
				}

				// исправляем день, месяц, год
				i = (value[1] - L'0') * 10 + (value[2] - L'0'); // день
				m = (value[4] - L'0') * 10 + (value[5] - L'0'); // месяц
				j = (value[7] - L'0') * 1000 + (value[8] - L'0') * 100 + (value[9] - L'0') * 10 + (value[10] - L'0'); // год

				if(m > 12)
				{
					m = 12;
					value[4] = L'1';
					value[5] = L'2';
				}
				else if(m == 0)
				{
					m = 1;
					value[4] = L'0';
					value[5] = L'1';
				}

				if(j == 0)
				{
					j = 1;
					value[7] = L'0'; //-V525
					value[8] = L'0';
					value[9] = L'0';
					value[10] = L'1';
				}

				if(i == 0)
				{
					value[1] = L'0';
					value[2] = L'1';
				}
				else if(i > 28) switch(m)
				{
					case 1:
					case 3:
					case 5:
					case 7:
					case 8:
					case 10:
					case 12:
						if(i > 31)
						{
							value[1] = L'3';
							value[2] = L'1';
						}
						break;
					case 4:
					case 6:
					case 9:
					case 11:
						if(i > 30)
						{
							value[1] = L'3';
							value[2] = L'0';
						}
						break;
					case 2:
						if(j % 4 == 0 && (j % 100 != 0 || j % 400 == 0))
						{
							if(i > 29)
							{
								value[1] = L'2';
								value[2] = L'9';
							}
						}
						else
						{
							if(i > 28)
							{
								value[1] = L'2';
								value[2] = L'8';
							}
						}
						break;
				}

				// исправляем часы, минуты, секунды
				i = (value[12] - L'0') * 10 + (value[13] - L'0'); // часы
				if(i > 23)
				{
					value[12] = L'2';
					value[13] = L'3';
				}

				i = (value[15] - L'0') * 10 + (value[16] - L'0'); // минуты
				if(i > 59)
				{
					value[15] = L'5';
					value[16] = L'9';
				}

				i = (value[18] - L'0') * 10 + (value[19] - L'0'); // секунды
				if(i > 59)
				{
					value[18] = L'5';
					value[19] = L'9';
				}


				fr[3] = ((value[1] - L'0') << 4) + (value[2] - L'0');
				fr[2] = ((value[4] - L'0') << 4) + (value[5] - L'0');
				fr[0] = ((value[7] - L'0') << 4) + (value[8] - L'0');
				fr[1] = ((value[9] - L'0') << 4) + (value[10] - L'0');
				fr[4] = ((value[12] - L'0') << 4) + (value[13] - L'0');
				fr[5] = ((value[15] - L'0') << 4) + (value[16] - L'0');
				fr[6] = ((value[18] - L'0') << 4) + (value[19] - L'0');
			}
			break;
		case tf_varbinary:
			return false;
	}

	return true;
}

//---------------------------------------------------------------------------
String Field::get_XML_presentation(char* rec, bool ignore_showGUID)
{
	char sym;
	int32_t i, j, m;
	bool k;

	TMemoryStream* in;
	TMemoryStream* out;
	String s;

	unsigned char* fr = (unsigned char*)rec + offset;

	if(null_exists)
	{
		if(fr[0] == 0) return "";
		fr++;
	}
	switch(type)
	{
		case tf_binary:
			if(length == 16 && (showGUID || ignore_showGUID))
			{
				if(showGUIDasMS) return GUIDasMS(fr);
				else return GUIDas1C(fr);
			}
			else
			{
				for(i = 0; i < length; i++)
				{
					sym = '0' + (fr[i] >> 4);
					if(sym > '9') sym += ('a' - '9' - 1);
					buf[i << 1] = sym;
					sym = '0' + (fr[i] & 0xf);
					if(sym > '9') sym += ('a' - '9' - 1);
					buf[(i << 1) + 1] = sym;
				}
				buf[length << 1] = 0;
			}
			return buf;
		case tf_bool:
			if(fr[0]) return "true";
			return "false";
		case tf_numeric:
			i = 0; // текущий индекс в buf
			k = true; // признак, что значащие цифры еще не начались
			m = length - precision; // позиция десятичной точки слева
			if(fr[0] >> 4 == 0) buf[i++] = '-';
			for(j = 0; j < length; j++)
			{
				if(j == m)
				{
					buf[i++] = '.';
					k = false;
				}
				if(j & 1) sym = fr[(j + 1) >> 1] >> 4;
				else sym = fr[j >> 1] & 0xf;
				if(sym == 0 && k) continue;
				k = false;
				buf[i++] = '0' + sym;
			}
			if(k) return "0";
			buf[i] = 0;
			return buf;
		case tf_char:
			return toXML(String((WCHART*)fr, length));
		case tf_varchar:
			i = *(int16_t*)fr;
			return toXML(String(((WCHART*)fr) + 1, i));
		case tf_version: {
			int32_t *retyped = (int32_t*)fr;
			return String(*(int32_t*)fr) + ":" + retyped[1] + ":" + retyped[2] + ":" + retyped[3];
		}
		case tf_version8: {
			int32_t *retyped = (int32_t*)fr;
			return String(retyped[0] + ":" + retyped[1]);
		}
		case tf_string: {
			uint32_t *retyped = (uint32_t*)fr;
			out = new TMemoryStream();
			parent->readBlob(out, retyped[0], retyped[1]);
			s = toXML(String((WCHART*)(out->GetMemory()), out->GetSize() / 2));
			delete out;
			return s;
		}
		case tf_text: {
			uint32_t *retyped = (uint32_t*)fr;
			out = new TMemoryStream();
			parent->readBlob(out, retyped[0], retyped[1]);
			s = toXML(String((char*)(out->GetMemory()), out->GetSize()));
			delete out;
			return s;
		}
		case tf_image: {
			uint32_t *retyped = (uint32_t*)fr;
			in = new TMemoryStream();
			out = new TMemoryStream();
			parent->readBlob(in, retyped[0], retyped[1]);
			base64_encode(in, out, 72);
			s = String((WCHART*)(out->GetMemory()), out->GetSize() / 2);
			delete in;
			delete out;
			return s;
		}
		case tf_datetime:
			buf[0] = '0' + (fr[0] >> 4);
			buf[1] = '0' + (fr[0] & 0xf);
			buf[2] = '0' + (fr[1] >> 4);
			buf[3] = '0' + (fr[1] & 0xf);
			buf[4] = '-';
			buf[5] = '0' + (fr[2] >> 4);
			buf[6] = '0' + (fr[2] & 0xf);
			buf[7] = '-';
			buf[8] = '0' + (fr[3] >> 4);
			buf[9] = '0' + (fr[3] & 0xf);
			buf[10] = 'T';
			buf[11] = '0' + (fr[4] >> 4);
			buf[12] = '0' + (fr[4] & 0xf);
			buf[13] = ':';
			buf[14] = '0' + (fr[5] >> 4);
			buf[15] = '0' + (fr[5] & 0xf);
			buf[16] = ':';
			buf[17] = '0' + (fr[6] >> 4);
			buf[18] = '0' + (fr[6] & 0xf);
			buf[19] = 0;
			return buf;
		case tf_varbinary:
			m = *(int16_t*)fr; // длина + смещение
			for(i = 0; i < m; i++)
			{
				sym = '0' + (fr[i + 2] >> 4);
				if(sym > '9') sym += ('a' - '9' - 1);
				buf[i << 1] = sym;
				sym = '0' + (fr[i + 2] & 0xf);
				if(sym > '9') sym += ('a' - '9' - 1);
				buf[(i << 1) + 1] = sym;
			}
			buf[m << 1] = 0;
			return buf;
	}

	return "{?}";
}

//---------------------------------------------------------------------------
type_fields Field::gettype()
{
	return type;
}

//---------------------------------------------------------------------------
Table* Field::getparent()
{
	return parent;
}

//---------------------------------------------------------------------------
bool Field::getnull_exists()
{
	return null_exists;
}

//---------------------------------------------------------------------------
int32_t Field::getlength()
{
	return length;
}

//---------------------------------------------------------------------------
int32_t Field::getprecision()
{
	return precision;
}

//---------------------------------------------------------------------------
bool Field::getcase_sensitive()
{
	return case_sensitive;
}

//---------------------------------------------------------------------------
int32_t Field::getoffset()
{
	return offset;
}

//---------------------------------------------------------------------------
String Field::get_presentation_type()
{
	switch(type)
	{
		case tf_binary: return "binary";
		case tf_bool: return "bool";
		case tf_numeric: return "number";
		case tf_char: return "fixed string";
		case tf_varchar: return "string";
		case tf_version: return "version";
		case tf_string: return "memo";
		case tf_text: return "text";
		case tf_image: return "image";
		case tf_datetime: return "datetime";
		case tf_version8: return "hidden version";
		case tf_varbinary: return "var binary";
	}
	return "{?}";
}

//---------------------------------------------------------------------------
String TrimSpacesRight(String s)
{
	while(s.SubString(s.GetLength(), 1) == " ") s.SetLength(s.GetLength() - 1);
	return s;
}

//---------------------------------------------------------------------------
#ifndef PublicRelease
uint32_t Field::getSortKey(const char* rec, unsigned char* SortKey, int32_t maxlen)
{
	T_1CD* base;
	int32_t i, j, m;
	bool k;
	uint32_t addlen = 0;
	int32_t maxl = maxlen;
	bool isnull = false;
	String s;
	unsigned char c;

	unsigned char* fr = (unsigned char *)rec + offset;

	char nbuf[64];

	if(!maxlen)
	{
		error("Ошибка получения ключа сортировки поля. Нулевая длина буфера.",
			"Таблица", parent->name,
			"Поле", name,
			"Значение поля", get_presentation(rec));

		return 0;
	}

	if(null_exists)
	{
		if(*fr == 0)
		{
			*(SortKey++) = 0;
			isnull = true;
		}
		else *(SortKey++) = 1;

		fr++;
		maxl--;
		addlen = 1;
	}

	switch(type)
	{
		case tf_binary:
		case tf_datetime:
		case tf_bool:
			if(len > maxlen)
			{
				error("Ошибка получения ключа сортировки поля. Длина буфера меньше необходимой.",
					"Таблица", parent->name,
					"Поле", name,
					"Значение поля", get_presentation(rec),
					"Длина буфера", maxlen,
					"Необходимая длина буфера", len);

				memcpy(SortKey, isnull ? (void *)null_index : (void *)fr, maxl);
				return maxlen;
			}
			memcpy(SortKey, isnull ? (void *)null_index : (void*)fr, len - addlen);
			return len;

		case tf_numeric:

			if(isnull)
			{
				if(len > maxlen)
				{
					error("Ошибка получения ключа сортировки поля. Длина буфера меньше необходимой.",
						"Таблица", parent->name,
						"Поле", name,
						"Значение поля", get_presentation(rec),
						"Длина буфера", maxlen,
						"Необходимая длина буфера", len);

					memcpy(SortKey, null_index, maxl);
					return maxlen;
				}
				memcpy(SortKey, null_index, len - addlen);
				return len;

			}

			memcpy(nbuf, fr, len - addlen);
			if((*nbuf & 0xf0) == 0) //Отрицательное!
			{
				k = false; // перенос
				for(i = length; i > 0; i--)
				{
					j = i >> 1;
					if(i & 1)
					{
						c = nbuf[j] & 0x0f;
						if(k) c = 9 - c;
						else
						{
							if(c)
							{
								k = true;
								c = 10 - c;
							}
						}
						nbuf[j] = (nbuf[j] & 0xf0) | c;
					}
					else
					{
						c = nbuf[j] >> 4 & 0x0f;
						if(k) c = 9 - c;
						else
						{
							if(c)
							{
								k = true;
								c = 10 - c;
							}
						}
						nbuf[j] = (nbuf[j] & 0x0f) | (c << 4);
					}
				}
				if(!k) *nbuf = 0x10; // если значение оказалось -0 (минус ноль), то должно получиться +0.
			}

			if(len > maxlen)
			{
				error("Ошибка получения ключа сортировки поля. Длина буфера меньше необходимой.",
					"Таблица", parent->name,
					"Поле", name,
					"Значение поля", get_presentation(rec),
					"Длина буфера", maxlen,
					"Необходимая длина буфера", len);

				memcpy(SortKey, nbuf, maxl);
				return maxlen;
			}
			memcpy(SortKey, nbuf, len - addlen);
			return len;

		case tf_char:
					error("Ошибка получения ключа сортировки поля. Неизвестный код возврата.",
						"Таблица", parent->name,
						"Поле", name,
						"Значение поля", get_presentation(rec));
					return addlen;

		case tf_varchar:
					error("Ошибка получения ключа сортировки поля. Неизвестный код возврата.",
						"Таблица", parent->name,
						"Поле", name,
						"Значение поля", get_presentation(rec));
					return addlen;

		case tf_version:
		case tf_version8:
		case tf_string:
		case tf_text:
		case tf_image:
		case tf_varbinary:
			error("Попытка получения ключа сортировки неподдерживаемого типа поля.",
				"Таблица", parent->name,
				"Поле", name,
				"Тип поля", get_presentation_type());
			return 0;
	}

	return addlen;

}
#endif //#ifdef PublicRelease

//---------------------------------------------------------------------------
bool Field::save_blob_to_file(char* rec, String _filename, bool unpack)
{
	TStream* blob_stream;
	bool zippedContainer;
	char _buf[16];
	TStream* _s;
	TStream* _s2;
	TStream* _sx;
	TStream* _sx2;
	int64_t len1C;
	bool is_users_usr;
	uint32_t i, j, k, l;
	char* _xor_mask;
	char* _xor_buf;
	Table* tab;
	TStream* temp_stream;
	bool zipped;
	Field* _f;
	char* _bb;
	v8catalog* cat;
	char* orec;
	bool maybezipped2;

	bool usetemporaryfiles = false;

	orec = rec;
	rec += offset;
	if(null_exists)
	{
		if(*rec == 0) return false;
		rec++;
	}

	if(*(uint32_t*)rec == 0 || *(uint32_t*)(rec + 4) == 0) return false;

	if(!unpack)
	{
		temp_stream = new TFileStream(_filename, fmCreate);
		parent->readBlob(temp_stream, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
		delete temp_stream;
		return true;
	}

	usetemporaryfiles = *(uint32_t*)(rec + 4) > 10 * 1024 * 1024;
	if(usetemporaryfiles) blob_stream = new TTempStream;
	else blob_stream = new TMemoryStream;
	parent->readBlob(blob_stream, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
	if(blob_stream->GetSize() == 0)
	{
		delete blob_stream;
		return false;
	}

	tab = parent;
	if(usetemporaryfiles) _s = new TTempStream;
	else _s = new TMemoryStream;

	if(tab->get_issystem())
	{

		// спецобработка для users.usr
		String tabname = tab->getname();
		is_users_usr = false;
		if(tabname.CompareIC("PARAMS") == 0)
		{
			_f = tab->getfield(0);
			if(_f->get_presentation(orec).CompareIC("users.usr") == 0) is_users_usr = true;
		}
		else if(tabname.CompareIC("V8USERS") == 0) is_users_usr = true;

		maybezipped2 = true;
		if(tabname.CompareIC("CONFIG") == 0 || tabname.CompareIC("CONFIGSAVE") == 0)
		{
			_f = tab->getfield(0);
			maybezipped2 = _f->get_presentation(orec).GetLength() > 72;
		}

		if(is_users_usr)
		{

			i = blob_stream->GetSize();
			_bb = new char[i];
			blob_stream->Seek(0, soFromBeginning);
			blob_stream->Read(_bb, i);

			j = _bb[0];
			_xor_mask = _bb + 1;
			_xor_buf = _xor_mask + j;
			l = i - j - 1;
			for(i = 0, k = 0; i < l; i++, k++)
			{
				if(k >= j) k = 0;
				_xor_buf[i] ^= _xor_mask[k];
			}
			temp_stream = new TFileStream(_filename, fmCreate);
			temp_stream->SetSize(0);
			temp_stream->WriteBuffer(_xor_buf, l);
			delete temp_stream;
			delete[] _bb;
		}
		else
		{
			zippedContainer = false;
			try
			{
				blob_stream->Seek(0, soFromBeginning);
				ZInflateStream(blob_stream, _s);
				zipped = true;
				if(maybezipped2) _sx = _s;
				else _sx2 = _s;
				_s = NULL;
				delete blob_stream;
				blob_stream = NULL;
			}
			catch (...)
			{
				_sx2 = blob_stream;
				delete _s;
				_s = NULL;
				blob_stream = NULL;
				zipped = false;
			}

			if(zipped && maybezipped2)
			{
				if(usetemporaryfiles) _s2 = new TTempStream;
				else _s2 = new TMemoryStream;
				try
				{
					_sx->Seek(0, soFromBeginning);
					ZInflateStream(_sx, _s2);
					zippedContainer = true;
					_sx2 = _s2;
					_s2 = NULL;
					delete _sx;
					_sx = NULL;
				}
				catch (...)
				{
					_sx2 = _sx;
					_sx = NULL;
					delete _s2;
					_s2 = NULL;
				}
			}

			cat = new v8catalog(_sx2, zippedContainer, true);
			if(!cat->GetFirst())
			{
				temp_stream = new TFileStream(_filename, fmCreate);
				temp_stream->CopyFrom(_sx2, 0);
				delete temp_stream;
			}
			else cat->SaveToDir(_filename);
			delete cat;
			delete _sx2;

		}
	}
	else /*if(tab->get_issystem())*/
	{
		_s->CopyFrom(blob_stream, 0);
		blob_stream->Seek(0, soFromBeginning);
		if(blob_stream->Read(_buf, 2) >= 2) if((_buf[0] == 1 || _buf[0] == 2) && _buf[1] == 1)
		{
			if(usetemporaryfiles) _s2 = new TTempStream;
			else _s2 = new TMemoryStream;
			bool isOK = true;
			if(_buf[0] == 1) // неупакованное хранилище
			{
				_s2->CopyFrom(blob_stream, blob_stream->GetSize() - 2);
			}
			else
			{
				if(blob_stream->Read(_buf, 16) < 16) isOK = false;
				else
				{
					if(memcmp(_buf, SIG_ZIP, 16) != 0) isOK = false;
					else
					{
						try
						{
							ZInflateStream(blob_stream, _s2);
						}
						catch(...)
						{
							isOK = false;
						}
					}
				}
			}
			if(isOK)
			{
				_s2->Seek(0, soFromBeginning);
				if(_s2->Read(_buf, 8) < 8) isOK = false;
				else
				{
					_s->SetSize(0);
					_s->CopyFrom(_s2, _s2->GetSize() - 8);
				}

			}

			if(isOK)
			{
				len1C = *(int64_t*)_buf;
				if(_s->GetSize() > len1C)
				{
					_s->Seek(len1C, (TSeekOrigin)soFromBeginning);
					_s2->SetSize(0);
					_s2->CopyFrom(_s, _s->GetSize() - len1C);
					_s2->Seek(0, soFromBeginning);
					if(_s2->Read(_buf, 12) >= 12)
					{
						len1C = *(int64_t*)&_buf[4];
						if(len1C <= _s2->GetSize() - 12)
						{
							_s->SetSize(0);
							_s->CopyFrom(_s2, len1C);
						}
					}
				}
			}
			delete _s2;
		}

		temp_stream = new TFileStream(_filename, fmCreate);
		temp_stream->CopyFrom(_s, 0);
		delete temp_stream;
	}

	delete _s;
	delete blob_stream;

	return true;
}


//********************************************************
// Класс changed_rec

//---------------------------------------------------------------------------
changed_rec::changed_rec(Table* _parent, changed_rec_type crt, uint32_t phys_numrecord)
{
	parent = _parent;
	numrec = phys_numrecord;
	changed_type = crt;
	if(crt == crt_delete)
	{
		fields = NULL;
		rec = NULL;
	}
	else
	{
		fields = new char[parent->num_fields];
		memset(fields, 0, parent->num_fields);
		rec = new char[parent->recordlen];
		memset(rec, 0, parent->recordlen);
	}
	next = parent->ch_rec;
	parent->ch_rec = this;
}
//---------------------------------------------------------------------------
changed_rec::~changed_rec()
{
	clear();
	delete[] fields;
	delete[] rec;
}

//---------------------------------------------------------------------------
void changed_rec::clear()
{
	int32_t i;
	Field* f;
	type_fields tf;
	TStream* b;

	if(rec && fields) for(i = 0; i < parent->num_fields; i++) if(fields[i])
	{
		f = parent->fields[i];
		tf = f->gettype();
		if(tf == tf_image || tf == tf_string || tf == tf_text)
		{
			b = *(TStream**)(rec + f->getoffset() + (f->getnull_exists() ? 1 : 0));
			delete b;
		}
	}

}



//********************************************************
// Класс table

//---------------------------------------------------------------------------
bool Table::get_issystem()
{
	return issystem;
}

//---------------------------------------------------------------------------
void Table::init(int32_t block_descr)
{
	tree* t;
	tree* f;
	tree* ff;
	tree* in;
	tree* rt;
	int32_t i, j, k;
	uint32_t m;
	uint64_t s;
	String ws;
	class index* ind;
	int32_t numrec;
	int32_t blockfile[3];
	Field* fld;
	uint32_t* buf;

	num_fields = 0;
	fields = NULL;
	num_indexes = 0;
	indexes = 0;
	recordlock = false;
	file_data = NULL;
	file_blob = NULL;
	file_index = NULL;
	lockinmemory = 0;

	recordsindex_complete = false;
	numrecords_review = 0;
	numrecords_found = 0;
	recordsindex = NULL;

#ifndef PublicRelease
	edit = false;
#endif //#ifdef PublicRelease
	ch_rec = NULL;
	added_numrecords = 0;

	phys_numrecords = 0;
	log_numrecords = 0;
	bad = true;

	if(description.IsEmpty()) return;

	tree* root = parse_1Ctext(description, String("Блок ") + block_descr);

	if(!root)
	{
		error("Ошибка разбора текста описания таблицы.",
			"Блок", tohex(block_descr));
		init();
		return;
	}

	if(root->get_num_subnode() != 1)
	{
		error("Ошибка разбора текста описания таблицы. Количество узлов не равно 1.",
			"Блок", tohex(block_descr),
			"Узлов", root->get_num_subnode());
		init();
		delete root;
		return;
	}
	rt = root->get_first();

	if(rt->get_num_subnode() != 6)
	{
		error("Ошибка разбора текста описания таблицы. Количество узлов не равно 6.",
			"Блок", tohex(block_descr),
			"Узлов", rt->get_num_subnode());
		init();
		delete root;
		return;
	}

	t = rt->get_first();
	if(t->get_type() != nd_string)
	{
		error("Ошибка получения имени таблицы. Узел не является строкой.",
			"Блок", tohex(block_descr));
		init();
		delete root;
		return;
	}
	name = t->get_value();
	issystem = name[1] != L'_'
		|| name.SubString(name.GetLength() - 6, 7).CompareIC("STORAGE") == 0
		|| name.CompareIC("_SYSTEMSETTINGS") == 0
		|| name.CompareIC("_COMMONSETTINGS") == 0
		|| name.CompareIC("_REPSETTINGS") == 0
		|| name.CompareIC("_REPVARSETTINGS") == 0
		|| name.CompareIC("_FRMDTSETTINGS") == 0
		|| name.CompareIC("_SCHEDULEDJOBS") == 0;

#ifdef getcfname
	if(name.CompareIC("CONFIG"))
	{
		delete root;
		return;
	}
#endif
#ifdef delic
#ifdef delicfiles
	if(name.CompareIC("PARAMS") && name.CompareIC("FILES") && name.CompareIC("CONFIG"))
	{
		delete root;
		return;
	}
#else
	if(name.CompareIC("PARAMS"))
	{
		delete root;
		return;
	}
#endif
#endif

	t = t->get_next();
	// пропускаем узел, так как там всегда содержится "0", и что это такое, неизвестно (версия формата описания таблиц?)
	t = t->get_next();
	if(t->get_type() != nd_list)
	{
		error("Ошибка получения полей таблицы. Узел не является деревом.",
			"Блок", tohex(block_descr),
			"Таблица", name);
		init();
		delete root;
		return;
	}
	if(t->get_num_subnode() < 2)
	{
		error("Ошибка получения полей таблицы. Нет узлов описаня полей.",
			"Блок", tohex(block_descr),
			"Таблица", name);
		init();
		delete root;
		return;
	}

	num_fields = t->get_num_subnode() - 1;
	num_fields2 = num_fields + 1; // добавляем лишнее поле на случай наличия скрытого поля версии
	fields = new Field*[num_fields2];
	bool has_version = false; // признак наличия поля версии
	for(i = 0; i < num_fields2; i++) fields[i] = new Field(this);

	f = t->get_first();
	if(f->get_type() != nd_string)
	{
		error("Ошибка получения полей таблицы. Ожидаемый узел Fields не является строкой.",
			"Блок", tohex(block_descr),
			"Таблица", name);
		deletefields();
		init();
		delete root;
		return;
	}
	if(f->get_value() != "Fields")
	{
		error("Ошибка получения полей таблицы. Узел не Fields.",
			"Блок", tohex(block_descr),
			"Таблица", name,
			"Узел", f->get_value());
		deletefields();
		init();
		delete root;
		return;
	}

	for(i = 0; i < num_fields; i++)
	{
		f = f->get_next();
		if(f->get_num_subnode() != 6)
		{
			error("Ошибка получения узла очередного поля таблицы. Количество узлов поля не равно 6.",
				"Блок", tohex(block_descr),
				"Таблица", name,
				"Номер поля", i + 1,
				"Узлов", f->get_num_subnode());
			deletefields();
			init();
			delete root;
			return;
		}

		ff = f->get_first();
		if(ff->get_type() != nd_string)
		{
			error("Ошибка получения имени поля таблицы. Узел не является строкой.",
				"Блок", tohex(block_descr),
				"Таблица", name,
				"Номер поля", i + 1);
			deletefields();
			init();
			delete root;
			return;
		}
		fld = fields[i];
		fld->name = ff->get_value();

		ff = ff->get_next();
		if(ff->get_type() != nd_string)
		{
			error("Ошибка получения типа поля таблицы. Узел не является строкой.",
				"Блок", tohex(block_descr),
				"Таблица", name,
				"Поле", fld->name);
			deletefields();
			init();
			delete root;
			return;
		}
		ws = ff->get_value();
		if(ws == "B") fld->type = tf_binary;
		else if(ws == "L") fld->type = tf_bool;
		else if(ws == "N") fld->type = tf_numeric;
		else if(ws == "NC") fld->type = tf_char;
		else if(ws == "NVC") fld->type = tf_varchar;
		else if(ws == "RV")
		{
			fld->type = tf_version;
			has_version = true;
		}
		else if(ws == "NT") fld->type = tf_string;
		else if(ws == "T") fld->type = tf_text;
		else if(ws == "I") fld->type = tf_image;
		else if(ws == "DT") fld->type = tf_datetime;
		else if(ws == "VB") fld->type = tf_varbinary;
		else
		{
			error("Неизвестный тип поля таблицы.",
				"Блок", tohex(block_descr),
				"Таблица", name,
				"Поле", fld->name,
				"Тип поля", ws);
			deletefields();
			init();
			delete root;
			return;
		}

		ff = ff->get_next();
		if(ff->get_type() != nd_number)
		{
			error("Ошибка получения признака NULL поля таблицы. Узел не является числом.",
				"Блок", tohex(block_descr),
				"Таблица", name,
				"Поле", fld->name,
				"Тип поля", ws);
			deletefields();
			init();
			delete root;
			return;
		}
		ws = ff->get_value();
		if(ws == "0") fld->null_exists = false;
		else if(ws == "1") fld->null_exists = true;
		else
		{
			error("Неизвестное значение признака NULL поля таблицы.",
				"Блок", tohex(block_descr),
				"Таблица", name,
				"Поле", fld->name,
				"Признак NUL", ws);
			deletefields();
			init();
			delete root;
			return;
		}

		ff = ff->get_next();
		if(ff->get_type() != nd_number)
		{
			error("Ошибка получения длины поля таблицы. Узел не является числом.",
				"Блок", tohex(block_descr),
				"Таблица", name,
				"Поле", fld->name);
			deletefields();
			init();
			delete root;
			return;
		}
		fld->length = StrToInt(ff->get_value());

		ff = ff->get_next();
		if(ff->get_type() != nd_number)
		{
			error("Ошибка получения точности поля таблицы. Узел не является числом.",
				"Блок", tohex(block_descr),
				"Таблица", name,
				"Поле", fld->name);
			deletefields();
			init();
			delete root;
			return;
		}
		fld->precision = StrToInt(ff->get_value());

		ff = ff->get_next();
		if(ff->get_type() != nd_string)
		{
			error("Ошибка получения регистрочувствительности поля таблицы. Узел не является строкой.",
				"Блок", tohex(block_descr),
				"Таблица", name,
				"Поле", fld->name);
			deletefields();
			init();
			delete root;
			return;
		}
		ws = ff->get_value();
		if(ws == "CS") fld->case_sensitive = true;
		else if(ws == "CI") fld->case_sensitive = false;
		else
		{
			error("Неизвестное значение регистрочувствительности поля таблицы.",
				"Блок", tohex(block_descr),
				"Таблица", name,
				"Поле", fld->name,
				"Регистрочувствительность", ws);
			deletefields();
			init();
			delete root;
			return;
		}
	}

	t = t->get_next();
	if(t->get_type() != nd_list)
	{
		error("Ошибка получения индексов таблицы. Узел не является деревом.",
			"Блок", tohex(block_descr),
			"Таблица", name);
		deletefields();
		init();
		delete root;
		return;
	}
	if(t->get_num_subnode() < 1)
	{
		error("Ошибка получения индексов таблицы. Нет узлов описаня индексов.",
			"Блок", tohex(block_descr),
			"Таблица", name);
		deletefields();
		init();
		delete root;
		return;
	}

	num_indexes = t->get_num_subnode() - 1;
	if(num_indexes)
	{
		indexes = new class index*[num_indexes];
		for(i = 0; i < num_indexes; i++) indexes[i] = new class index(this);

		f = t->get_first();
		if(f->get_type() != nd_string)
		{
			error("Ошибка получения индексов таблицы. Ожидаемый узел Indexes не является строкой.",
				"Блок", tohex(block_descr),
				"Таблица", name);
			deletefields();
			deleteindexes();
			init();
			delete root;
			return;
		}
		if(f->get_value() != "Indexes")
		{
			error("Ошибка получения индексов таблицы. Узел не Indexes.",
				"Блок", tohex(block_descr),
				"Таблица", name,
				"Узел", f->get_value());
			deletefields();
			deleteindexes();
			init();
			delete root;
			return;
		}

		for(i = 0; i < num_indexes; i++)
		{
			f = f->get_next();
			numrec = f->get_num_subnode() - 2;
			if(numrec < 1)
			{
				error("Ошибка получения очередного индекса таблицы. Нет узлов описаня полей индекса.",
					"Блок", tohex(block_descr),
					"Таблица", name,
					"Номер индекса", i + 1);
				deletefields();
				deleteindexes();
				init();
				delete root;
				return;
			}
			ind = indexes[i];
			ind->num_records = numrec;

			if(f->get_type() != nd_list)
			{
				error("Ошибка получения очередного индекса таблицы. Узел не является деревом.",
					"Блок", tohex(block_descr),
					"Таблица", name,
					"Номер индекса", i + 1);
				deletefields();
				deleteindexes();
				init();
				delete root;
				return;
			}

			ff = f->get_first();
			if(ff->get_type() != nd_string)
			{
				error("Ошибка получения имени индекса таблицы. Узел не является строкой.",
					"Блок", tohex(block_descr),
					"Таблица", name,
					"Номер индекса", i + 1);
				deletefields();
				deleteindexes();
				init();
				delete root;
				return;
			}
			ind->name = ff->get_value();

			ff = ff->get_next();
			if(ff->get_type() != nd_number)
			{
				error("Ошибка получения типа индекса таблицы. Узел не является числом.",
					"Блок", tohex(block_descr),
					"Таблица", name,
					"Индекс", ind->name);
				deletefields();
				deleteindexes();
				init();
				delete root;
				return;
			}
			ws = ff->get_value();
			if(ws == "0") ind->is_primary = false;
			else if(ws == "1") ind->is_primary = true;
			else
			{
				error("Неизвестный тип индекса таблицы.",
					"Блок", tohex(block_descr),
					"Таблица", name,
					"Индекс", ind->name,
					"Тип индекса", ws);
				deletefields();
				deleteindexes();
				init();
				delete root;
				return;
			}

			ind->records = new index_record[numrec];
			for(j = 0; j < numrec; j++)
			{
				ff = ff->get_next();
				if(ff->get_num_subnode() != 2)
				{
					error("Ошибка получения очередного поля индекса таблицы. Количество узлов поля не равно 2.",
						"Блок", tohex(block_descr),
						"Таблица", name,
						"Индекс", ind->name,
						"Номер поля индекса", j + 1,
						"Узлов", ff->get_num_subnode());
					deletefields();
					deleteindexes();
					init();
					delete root;
					return;
				}

				in = ff->get_first();
				if(in->get_type() != nd_string)
				{
					error("Ошибка получения имени поля индекса таблицы. Узел не является строкой.",
						"Блок", tohex(block_descr),
						"Таблица", name,
						"Индекс", ind->name,
						"Номер поля индекса", j + 1);
					deletefields();
					deleteindexes();
					init();
					delete root;
					return;
				}

				ws = in->get_value();
				for(k = 0; k < num_fields; k++)
				{
					if(fields[k]->name == ws)
					{
						ind->records[j].field = fields[k];
						break;
					}
				}
				if(k >= num_fields)
				{
					error("Ошибка получения индекса таблицы. Не найдено поле таблицы по имени поля индекса.",
						"Блок", tohex(block_descr),
						"Таблица", name,
						"Индекс", ind->name,
						"Поле индекса", ws);
					deletefields();
					deleteindexes();
					init();
					delete root;
					return;
				}

				in = in->get_next();
				if(in->get_type() != nd_number)
				{
					error("Ошибка получения длины поля индекса таблицы. Узел не является числом.",
						"Блок", tohex(block_descr),
						"Таблица", name,
						"Индекс", ind->name,
						"Поле индекса", ws);
					deletefields();
					deleteindexes();
					init();
					delete root;
					return;
				}
				ind->records[j].len = StrToInt(in->get_value());
			}
		}
	}
	else indexes = NULL;

	t = t->get_next();
	if(t->get_num_subnode() != 2)
	{
		error("Ошибка получения типа блокировки таблицы. Количество узлов не равно 2.",
			"Блок", tohex(block_descr),
			"Таблица", name);
		deletefields();
		deleteindexes();
		init();
		delete root;
		return;
	}

	f = t->get_first();
	if(f->get_type() != nd_string)
	{
		error("Ошибка получения типа блокировки таблицы. Ожидаемый узел Recordlock не является строкой.",
			"Блок", tohex(block_descr),
			"Таблица", name);
		deletefields();
		deleteindexes();
		init();
		delete root;
		return;
	}
	if(f->get_value() != "Recordlock")
	{
		error("Ошибка получения типа блокировки таблицы. Узел не Recordlock.",
			"Блок", tohex(block_descr),
			"Таблица", name,
			"Узел", f->get_value());
		deletefields();
		deleteindexes();
		init();
		delete root;
		return;
	}

	f = f->get_next();
	if(f->get_type() != nd_string)
	{
		error("Ошибка получения типа блокировки таблицы. Узел не является строкой.",
			"Блок", tohex(block_descr),
			"Таблица", name);
		deletefields();
		deleteindexes();
		init();
		delete root;
		return;
	}
	ws = f->get_value();
	if(ws == "0") recordlock = false;
	else if(ws == "1") recordlock = true;
	else
	{
		error("Неизвестное значение типа блокировки таблицы.",
			"Блок", tohex(block_descr),
			"Таблица", name,
			"Тип блокировки", ws);
		deletefields();
		deleteindexes();
		init();
		delete root;
		return;
	}

	if(recordlock && !has_version)
	{// добавляем скрытое поле версии
		fld = fields[num_fields++];
		fld->name = "VERSION";
		fld->type = tf_version8;
	}

	t = t->get_next();
	if(t->get_num_subnode() != 4)
	{
		error("Ошибка получения файлов таблицы. Количество узлов не равно 4.",
			"Блок", tohex(block_descr),
			"Таблица", name);
		deletefields();
		deleteindexes();
		init();
		delete root;
		return;
	}

	f = t->get_first();
	if(f->get_type() != nd_string)
	{
		error("Ошибка получения файлов таблицы. Ожидаемый узел Files не является строкой.",
			"Блок", tohex(block_descr),
			"Таблица", name);
		deletefields();
		deleteindexes();
		init();
		delete root;
		return;
	}
	if(f->get_value() != "Files")
	{
		error("Ошибка получения файлов таблицы. Узел не Files.",
			"Блок", tohex(block_descr),
			"Таблица", name,
			"Узел", f->get_value());
		deletefields();
		deleteindexes();
		init();
		delete root;
		return;
	}

	for(i = 0; i < 3; i++)
	{
		f = f->get_next();
		if(f->get_type() != nd_number)
		{
			error("Ошибка получения файла таблицы. Узел не является числом.",
				"Блок", tohex(block_descr),
				"Таблица", name,
				"Номер файла", i + 1);
			deletefields();
			deleteindexes();
			init();
			delete root;
			return;
		}
		blockfile[i] = StrToInt(f->get_value());
	}

	delete root;

	if(blockfile[0]) file_data = new v8object(base, blockfile[0]); else file_data = NULL;
	if(blockfile[1]) file_blob = new v8object(base, blockfile[1]); else file_blob = NULL;
	if(blockfile[2]) file_index = new v8object(base, blockfile[2]); else file_index = NULL;

	if(num_indexes && !file_index)
	{
		error("В таблице есть индексы, однако файл индексов отсутствует.",
			"Блок", tohex(block_descr),
			"Таблица", name,
			"Количество индексов", num_indexes);
	}
	else if(!num_indexes && file_index)
	{
		error("В таблице нет индексов, однако присутствует файл индексов.",
			"Блок", tohex(block_descr),
			"Таблица", name,
			"Блок индексов", tohex(blockfile[2]));
	}
	else if(file_index)
	{
		m = file_index->getlen() / base->pagesize;
		if(file_index->getlen() != m * base->pagesize)
		{
			error("Ошибка чтения индексов. Длина файла индексов не кратна размеру страницы",
				"Таблица", name,
				"Длина файла индексов", tohex(file_index->getlen()));
		}
		else
		{
			int32_t buflen = num_indexes * 4 + 4;
			buf = new uint32_t[num_indexes + 1];
			file_index->getdata(buf, 0, buflen);

//			// Временно, для отладки >>
//			if(buf[0]) if(msreg) msreg->AddMessage_("Существуют свободные страницы в файле индексов", msHint,
//					"Таблица", name,
//					"Индекс свободной страницы", tohex(buf[0]));
//			// Временно, для олтладки <<

			if(buf[0] * base->pagesize >= file_index->getlen())
			{
				error("Ошибка чтения индексов. Индекс первого свободного блока за пределами файла индексов",
					"Таблица", name,
					"Длина файла индексов", tohex(file_index->getlen()),
					"Индекс свободной страницы", tohex(buf[0]));
			}
			else
			{
				for(i = 1; i <= num_indexes; i++)
				{
					if(base->version < ver8_3_8_0)
					{
						if(buf[i] >= file_index->getlen())
						{
							error("Ошибка чтения индексов. Указанное смещение индекса за пределами файла индексов",
								"Таблица", name,
								"Длина файла индексов", tohex(file_index->getlen()),
								"Номер индекса", i,
								"Смещение индекса", tohex(buf[i]));
						}
						else if(buf[i] & 0xfff)
						{
							error("Ошибка чтения индексов. Указанное смещение индекса не кратно 0x1000",
								"Таблица", name,
								"Длина файла индексов", tohex(file_index->getlen()),
								"Номер индекса", i,
								"Смещение индекса", tohex(buf[i]));
						}
						else indexes[i - 1]->start = buf[i];
					}
					else
					{
						s = buf[i];
						s *= base->pagesize;
						if(s >= file_index->getlen())
						{
							error("Ошибка чтения индексов. Указанное смещение индекса за пределами файла индексов",
								"Таблица", name,
								"Длина файла индексов", file_index->getlen(),
								"Номер индекса", i,
								"Смещение индекса", s);
						}
						else indexes[i - 1]->start = s;
					}
				}
			}

			delete[] buf;

		}

	}

	// вычисляем длину записи таблицы как сумму длинн полей и проставим смещения полей в записи
	recordlen = 1; // первый байт записи - признак удаленности
	// сначала идут поля (поле) с типом "версия"
	for(i = 0; i < num_fields; i++) if(fields[i]->type == tf_version || fields[i]->type == tf_version8)
	{
		fields[i]->offset = recordlen;
		recordlen += fields[i]->getlen();
	}
	// затем идут все остальные поля
	for(i = 0; i < num_fields; i++) if(fields[i]->type != tf_version && fields[i]->type != tf_version8)
	{
		fields[i]->offset = recordlen;
		recordlen += fields[i]->getlen();
	}
	if(recordlen < 5) recordlen = 5; // Длина одной записи не может быть меньше 5 байт (1 байт признак, что запись свободна, 4 байт - индекс следующей следующей свободной записи)

	if(!recordlen || !file_data) phys_numrecords = 0;
	else phys_numrecords = file_data->getlen() / recordlen;;

	if(file_data)
	{
		if(phys_numrecords * recordlen != file_data->getlen())
		{
			error("Длина таблицы не кратна длине записи.",
				"Блок", tohex(block_descr),
				"Таблица", name,
				"Длина таблицы", file_data->getlen(),
				"Длина записи", recordlen);
		}
	}
	else
	{
		error("Отсутствует файл данных таблицы.",
			"Блок", tohex(block_descr),
			"Таблица", name);
			return;
	}

	// Инициализация данных индекса
	for(i = 0; i < num_indexes; i++) indexes[i]->get_length();

	#ifdef _DEBUG
	if(msreg) msreg->AddDebugMessage("Создана таблица.", msInfo,
		"Таблица", name,
		"Длина таблицы", file_data->getlen(),
		"Длина записи", recordlen);
	#endif

	bad = false;

}

//---------------------------------------------------------------------------
Table::Table(T_1CD* _base, int32_t block_descr)
{

	base = _base;

	descr_table = new v8object(base, block_descr);
	description = String((WCHART*)descr_table->getdata(), descr_table->getlen() / 2);


	init(block_descr);
}

//---------------------------------------------------------------------------
Table::Table(T_1CD* _base, String _descr, int32_t block_descr)
{

	base = _base;

	descr_table = 0;
	description = _descr;

	init(block_descr);
}

//---------------------------------------------------------------------------
Table::Table()
{
	init();
}

//---------------------------------------------------------------------------
void Table::deletefields()
{
	int32_t i;
	if(fields)
	{
		for(i = 0; i < num_fields2; i++) delete fields[i];
		delete[] fields;
	}
}

//---------------------------------------------------------------------------
void Table::deleteindexes()
{
	int32_t i;
	if(indexes)
	{
		for(i = 0; i < num_indexes; i++) delete indexes[i];
		delete[] indexes;
		indexes = NULL;
	}
}

//---------------------------------------------------------------------------
Table::~Table()
{
	changed_rec* cr;
	changed_rec* cr2;
	for(cr = ch_rec; cr;)
	{
		cr2 = cr->next;
		delete cr;
		cr = cr2;
	}

	deletefields();
	deleteindexes();
	if(file_data)
	{
		delete file_data;
		file_data = NULL;
	}
	if(file_blob)
	{
		delete file_blob;
		file_blob = NULL;
	}
	if(file_index)
	{
		delete file_index;
		file_index = NULL;
	}
	if(descr_table)
	{
		delete descr_table;
		descr_table = NULL;
	}

	delete[] recordsindex;
}

//---------------------------------------------------------------------------
String Table::getname()
{
	return name;
}

//---------------------------------------------------------------------------
String Table::getdescription()
{
	return description;
}

//---------------------------------------------------------------------------
int32_t Table::get_numfields()
{
	return num_fields;
}

//---------------------------------------------------------------------------
int32_t Table::get_numindexes()
{
	return num_indexes;
}

//---------------------------------------------------------------------------
Field* Table::getfield(int32_t numfield)
{
	if(numfield >= num_fields)
	{
		error("Попытка получения поля таблицы по номеру, превышающему количество полей",
			"Таблица", name,
			"Количество полей", num_fields,
			"Номер поля", numfield + 1);
		return NULL;
	}
	return fields[numfield];
}

//---------------------------------------------------------------------------
class index* Table::getindex(int32_t numindex)
{
	if(numindex >= num_indexes)
	{
		error("Попытка получения индекса таблицы по номеру, превышающему количество индексов",
			"Таблица", name,
			"Количество индексов", num_indexes,
			"Номер индекса", numindex + 1);
		return NULL;
	}
	return indexes[numindex];
}

//---------------------------------------------------------------------------
uint32_t Table::get_phys_numrecords()
{
	return phys_numrecords;
}

//---------------------------------------------------------------------------
uint32_t Table::get_log_numrecords()
{
	return log_numrecords;
}

//---------------------------------------------------------------------------
void Table::set_log_numrecords(uint32_t _log_numrecords)
{
	log_numrecords = _log_numrecords;
}

//---------------------------------------------------------------------------
uint32_t Table::get_added_numrecords()
{
	return added_numrecords;
}

//---------------------------------------------------------------------------
char* Table::getrecord(uint32_t phys_numrecord, char* buf)
{
	#ifndef console
	#ifndef getcfname
	tr_syn->BeginWrite();
	#endif
	#endif
	char* b = file_data->getdata(buf, phys_numrecord * recordlen, recordlen);
	#ifndef console
	#ifndef getcfname
	tr_syn->EndWrite();
	#endif
	#endif
	return b;
}

//---------------------------------------------------------------------------
int32_t Table::get_recordlen()
{
	return recordlen;
}

//---------------------------------------------------------------------------
bool Table::get_recordlock()
{
	return recordlock;
}

//---------------------------------------------------------------------------
v8object* Table::get_file_data()
{
	return file_data;
}

//---------------------------------------------------------------------------
v8object* Table::get_file_blob()
{
	return file_blob;
}

//---------------------------------------------------------------------------
v8object* Table::get_file_index()
{
	return file_index;
}

//---------------------------------------------------------------------------
void Table::set_lockinmemory(bool _lock)
{
	if(_lock)
	{
		lockinmemory++;
		if(descr_table) descr_table->set_lockinmemory(true);
		if(file_data) file_data->set_lockinmemory(true);
		if(file_blob) file_blob->set_lockinmemory(true);
		if(file_index) file_index->set_lockinmemory(true);
	}
	else
	{
		if(lockinmemory > 0) lockinmemory--;
		if(!lockinmemory)
		{
			if(descr_table) descr_table->set_lockinmemory(false);
			if(file_data) file_data->set_lockinmemory(false);
			if(file_blob) file_blob->set_lockinmemory(false);
			if(file_index) file_index->set_lockinmemory(false);
		}
	}
}

//---------------------------------------------------------------------------
// rewrite - перезаписывать поток _str. Истина - перезаписывать (по умолчанию), Ложь - дописывать
TStream* Table::readBlob(TStream* _str, uint32_t _startblock, uint32_t _length, bool rewrite)
{
	uint32_t _curblock;
	char* _curb;
	uint16_t _curlen;
	uint32_t _filelen, _numblock;
	uint32_t startlen;

	if(rewrite) _str->SetSize(0);
	startlen = _str->GetPosition();

	if(!_startblock && _length)
	{
		error("Попытка чтения нулевого блока файла Blob",
			"Таблица", name);
		return _str;
	}

	if(file_blob)
	{
		_filelen = file_blob->getlen();
		_numblock = _filelen >> 8;
		if(_numblock << 8 != _filelen)
			error("Длина файла Blob не кратна 0x100",
				"Таблица", name,
				"Длина файла", tohex(_filelen));

		_curb = new char[0x100];
		_curblock = _startblock;
		while(_curblock)
		{
			if(_curblock >= _numblock)
			{
				error("Попытка чтения блока файла Blob за пределами файла",
					"Таблица", name,
					"Всего блоков", _numblock,
					"Читаемый блок", _curblock);
				return _str;
			}
			file_blob->getdata(_curb, _curblock << 8, 0x100);
			_curblock = *(uint32_t*)_curb;
			_curlen = *(uint16_t*)(_curb + 4);
			if(_curlen > 0xfa)
			{
				error("Попытка чтения из блока файла Blob более 0xfa байт",
					"Таблица", name,
					"Индекс блока", _curblock,
					"Читаемых байт", _curlen);
				return _str;
			}
			_str->Write(_curb + 6, _curlen);

			if(_str->GetSize() - startlen > _length) break; // аварийный выход из возможного ошибочного зацикливания
		}
		delete[] _curb;

		if(_str->GetSize() - startlen != _length)
		{
			error("Несовпадение длины Blob-поля, указанного в записи, с длиной практически прочитанных данных",
				"Таблица", name,
				"Длина поля", _length,
				"Прочитано", _str->GetSize() - startlen);
		}
	}
	else
	{
		error("Попытка чтения Blob-поля при отстутствующем файле Blob",
			"Таблица", name,
			"Длина поля", _length);
	}

	return _str;
}

//---------------------------------------------------------------------------
uint32_t Table::readBlob(void* buf, uint32_t _startblock, uint32_t _length)
{
	uint32_t _curblock;
	char* _curb;
	char* _buf;
	uint16_t _curlen;
	uint32_t _filelen, _numblock;
	uint32_t readed;

	if(!_startblock && _length)
	{
		error("Попытка чтения нулевого блока файла Blob",
			"Таблица", name);
		return 0;
	}

	_buf = (char*)buf;
	readed = 0;
	if(file_blob)
	{
		_filelen = file_blob->getlen();
		_numblock = _filelen >> 8;
		if(_numblock << 8 != _filelen)
			error("Длина файла Blob не кратна 0x100",
				"Таблица", name,
				"Длина файла", tohex(_filelen));

		_curb = new char[0x100];
		_curblock = _startblock;
		while(_curblock)
		{
			if(_curblock >= _numblock)
			{
				error("Попытка чтения блока файла Blob за пределами файла",
					"Таблица", name,
					"Всего блоков", _numblock,
					"Читаемый блок", _curblock);
				return readed;
			}
			file_blob->getdata(_curb, _curblock << 8, 0x100);
			_curblock = *(uint32_t*)_curb;
			_curlen = *(uint16_t*)(_curb + 4);
			if(_curlen > 0xfa)
			{
				error("Попытка чтения из блока файла Blob более 250 байт",
					"Таблица", name,
					"Индекс блока", _curblock,
					"Читаемых байт", _curlen);
				return readed;
			}
			memcpy(_buf, _curb + 6, _curlen);
			_buf += _curlen;
			readed += _curlen;

			if(readed > _length) break; // аварийный выход из возможного ошибочного зацикливания
		}
		delete[] _curb;

		if(readed != _length)
		{
			error("Несовпадение длины Blob-поля, указанного в записи, с длиной практически прочитанных данных",
				"Таблица", name,
				"Длина поля", _length,
				"Прочитано", readed);
		}
	}
	else
	{
		error("Попытка чтения Blob-поля при отстутствующем файле Blob",
			"Таблица", name,
			"Длина поля", _length);
	}

	return readed;
}

//---------------------------------------------------------------------------
bool Table::export_to_xml(String _filename, bool blob_to_file, bool unpack)
{
	String* us;
	String s;
	String recname;
	//String fieldname;
	int32_t i;
	uint32_t j, numr, nr;
	char* rec;
	bool dircreated = false;
	bool canwriteblob = false;
	String dir;
	class index* curindex;
	int32_t ic; // image count, количество полей с типом image
	int32_t rc; // repeat count, количество повторов имени записи подряд (для случая, если индекс не уникальный)

	char UnicodeHeader[3] = {'\xef', '\xbb', '\xbf'};
	String part1 = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\r\n<!--Файл сформирован программой Tool_1CD-->\r\n<Table Name=\"";
	String part2 = "\">\r\n\t<Fields>\r\n";
	String part3 = "\t</Fields>\r\n\t<Records>\r\n";
	String part4 = "\t</Records>\r\n</Table>\r\n";

	String fpart1 = "\t\t<Field Name=\"";
	String fpart2 = "\" Type=\"";
	String fpart3 = "\" Length=\"";
	String fpart4 = "\" Precision=\"";
	String fpart5 = "\"/>\r\n";

	String rpart1 = "\t\t<Record>\r\n";
	String rpart2 = "\t\t</Record>\r\n";
	String rpart3 = "\t\t\t<";
	String status = "Экспорт таблицы ";
	status += name;
	status += " ";

	TFileStream* f = new TFileStream(_filename, fmCreate);

	f->Write(UnicodeHeader, 3);
	f->Write(part1.c_str(), part1.GetLength());
	f->Write(name.c_str(), name.GetLength());
	f->Write(part2.c_str(), part2.GetLength());

	if(num_indexes)
	{
		curindex = indexes[0];
		for(i = 0; i < num_indexes; i++) if(indexes[i]->get_is_primary())
		{
			curindex = indexes[i];
			break;
		}
	}
	else curindex = NULL;

	ic = 0;
	for(i = 0; i < num_fields; i++)
	{
		f->Write(fpart1.c_str(), fpart1.GetLength());
		us = &(fields[i]->name);
		f->Write(us->c_str(), us->Length());
		f->Write(fpart2.c_str(), fpart2.GetLength());
		s = fields[i]->get_presentation_type();
		f->Write(s.c_str(), s.GetLength());
		f->Write(fpart3.c_str(), fpart3.GetLength());
		s = fields[i]->getlength();
		f->Write(s.c_str(), s.GetLength());
		f->Write(fpart4.c_str(), fpart4.GetLength());
		s = fields[i]->getprecision();
		f->Write(s.c_str(), s.GetLength());
		f->Write(fpart5.c_str(), fpart5.GetLength());
		if(fields[i]->type == tf_image) ic++;
	}

	f->Write(part3.c_str(), part3.GetLength());

	if(curindex) numr = curindex->get_numrecords();
	else numr = numrecords_found;

	rec = new char[get_recordlen()];

	if(msreg) msreg->Status(status);

	recname = "";
	rc = 0;
	for(j = 0; j < numr; j++)
	{
		if(j % 100 == 0 && j) if(msreg) msreg->Status(status + j);

		f->Write(rpart1.c_str(), rpart1.GetLength());
		if(curindex) nr = curindex->get_numrec(j);
		else nr = recordsindex[j];
		getrecord(nr, rec);
		if(ic){
			s = get_file_name_for_record(rec);
			if(s.CompareIC(recname) == 0) rc++;
			else
			{
				recname = s;
				rc = 0;
			}
		}
		for(i = 0; i < num_fields; i++)
		{
			f->Write(rpart3.c_str(), rpart3.GetLength());
			us = &(fields[i]->name);
			f->Write(us->c_str(), us->Length());
			f->Write(">", 1);

			if(blob_to_file && fields[i]->type == tf_image)
			{
				if(!dircreated)
				{
					dircreated = true;
					dir = _filename + ".blob";
					if(!DirectoryExists(dir))
					{
						try
						{
							CreateDir(dir);
							dir += "/";
							canwriteblob = true;
						}
						catch(...)
						{
							if(msreg) msreg->AddMessage_("Не удалось создать каталог blob", msWarning,
								"Таблица", name,
								"Путь", dir);
						}
					}
					else
					{
						dir += "/";
						canwriteblob = true;
					}
				}

				if(canwriteblob)
				{
					s = recname;
					if(ic > 1)
					{
						if(s.GetLength()) s += "_";
						s += fields[i]->name;
                    }
					if(rc)
					{
						s += "_";
						s += rc + 1;
					}
					if(!fields[i]->save_blob_to_file(rec, dir + s, unpack)) s = "{NULL}";
				}
				else s = "{ERROR}";
			}
			else s = fields[i]->get_XML_presentation(rec);

			f->Write(s.c_str(), s.GetLength());
			f->Write("</", 2);
			f->Write(us->c_str(), us->Length());
			f->Write(">\r\n", 3);
		}
		f->Write(rpart2.c_str(), rpart2.GetLength());

	}
	f->Write(part4.c_str(), part4.GetLength());

	delete[] rec;
	delete f;
	if(msreg) msreg->Status("");
	return true;
}

//---------------------------------------------------------------------------
int64_t Table::get_fileoffset(uint32_t phys_numrecord)
{
	uint32_t _offset = phys_numrecord * recordlen;
	return file_data->get_fileoffset(_offset);
}

//---------------------------------------------------------------------------
bool Table::get_edit()
{
#ifndef PublicRelease
	return edit;
#else
	return false;
#endif //#ifdef PublicRelease
}
//---------------------------------------------------------------------------

#ifndef PublicRelease
void Table::begin_edit()
{
	if(edit) return;
	if(base->readonly)
	{
		if(msreg) msreg->AddMessage_("Попытка входа в режим редактирования в режиме \"Только чтение\"", msWarning,
			"Таблица", name);
		return;
	}
	edit = true;
}

//---------------------------------------------------------------------------
changed_rec_type Table::get_rec_type(uint32_t phys_numrecord)
{
	changed_rec* cr;
	if(!edit) return crt_not_changed;
	cr = ch_rec;
	while(cr)
	{
		if(cr->numrec == phys_numrecord) return cr->changed_type;
		cr = cr->next;
	}
	return crt_not_changed;
}

//---------------------------------------------------------------------------
changed_rec_type Table::get_rec_type(uint32_t phys_numrecord, int32_t numfield)
{
	changed_rec* cr;
	if(!edit) return crt_not_changed;
	cr = ch_rec;
	while(cr)
	{
		if(cr->numrec == phys_numrecord)
		{
			if(cr->changed_type == crt_changed) return cr->fields[numfield] ? crt_changed : crt_not_changed;
			return cr->changed_type;
		}
		cr = cr->next;
	}
	return crt_not_changed;
}

//---------------------------------------------------------------------------
void Table::export_table(String path)
{
	TFileStream* f;
	String dir;
	export_import_table_root* root;

	dir = path + "\\" + name;
	CreateDir(dir);
	dir += "\\";

	root = new export_import_table_root;

	if(file_data)
	{
		root->has_data = true;
		root->data_version_1 = file_data->version.version_1;
		root->data_version_2 = file_data->version.version_2;
	}
	else root->has_data = false;

	if(file_blob)
	{
		root->has_blob = true;
		root->blob_version_1 = file_blob->version.version_1;
		root->blob_version_2 = file_blob->version.version_2;
	}
	else root->has_blob = false;

	if(file_index)
	{
		root->has_index = true;
		root->index_version_1 = file_index->version.version_1;
		root->index_version_2 = file_index->version.version_2;
	}
	else root->has_index = false;

	if(descr_table)
	{
		root->has_descr = true;
		root->descr_version_1 = descr_table->version.version_1;
		root->descr_version_2 = descr_table->version.version_2;
	}
	else root->has_descr = false;

	f = NULL;
	try
	{
		f = new TFileStream(dir + "root", fmCreate|fmShareDenyNone);
		f->Write(root, sizeof(export_import_table_root));
	}
	catch(...)
	{

	}
	delete f;
	delete root;

	if(file_data) file_data->savetofile(dir + "data");
	if(file_blob) file_blob->savetofile(dir + "blob");
	if(file_index) file_index->savetofile(dir + "index");
	if(descr_table) descr_table->savetofile(dir + "descr");

}

//---------------------------------------------------------------------------
void Table::import_table(String path)
{
	String dir;

	dir = path + "\\" + name;
	if(!DirectoryExists(dir))
	{
		return;
	}
	import_table2(dir);
}

//---------------------------------------------------------------------------
void Table::import_table2(String path)
{
	TFileStream* f;
	bool fopen;
	String dir;
	export_import_table_root* root;
	v8ob* ob;

	String str;
	char* buf;
	uint32_t i;

	bool descr_changed = false;

	if(!DirectoryExists(path))
	{
		return;
	}
	dir = path + "\\";

	try
	{
		f = new TFileStream(dir + "root", fmOpenRead);
	}
	catch(...)
	{
		if(msreg) msreg->AddMessage_("Ошибка открытия файла импорта таблицы root", msWarning,
			"Таблица", name,
			"Файл", dir + "root");
		return;
	}
	root = new export_import_table_root;
	f->Read(root, sizeof(export_import_table_root));
	delete f;

	if(root->has_data)
	{
		fopen = false;
		try
		{
			f = new TFileStream(dir + "data", fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			if(msreg) msreg->AddMessage_("Ошибка открытия файла импорта таблицы data", msWarning,
				"Таблица", name,
				"Файл", dir + "data");
		}
		if(fopen)
		{
			if(!file_data){
				file_data = new v8object(base);
				descr_changed = true;
			}
			file_data->setdata(f);
			ob = (v8ob*)base->getblock_for_write(file_data->block, true);
			ob->version.version_1 = root->data_version_1;
			ob->version.version_2 = root->data_version_2;
			delete f;
		}
	}

	if(root->has_blob)
	{
		fopen = false;
		try
		{
			f = new TFileStream(dir + "blob", fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			if(msreg) msreg->AddMessage_("Ошибка открытия файла импорта таблицы blob", msWarning,
				"Таблица", name,
				"Файл", dir + "blob");
		}
		if(fopen)
		{
			if(!file_blob){
				file_blob = new v8object(base);
				descr_changed = true;
			}
			file_blob->setdata(f);
			ob = (v8ob*)base->getblock_for_write(file_blob->block, true);
			ob->version.version_1 = root->blob_version_1;
			ob->version.version_2 = root->blob_version_2;
			delete f;
		}
	}

	if(root->has_index)
	{
		fopen = false;
		try
		{
			f = new TFileStream(dir + "index", fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			if(msreg) msreg->AddMessage_("Ошибка открытия файла импорта таблицы index", msWarning,
				"Таблица", name,
				"Файл", dir + "index");
		}
		if(fopen)
		{
			if(!file_index){
				file_index = new v8object(base);
				descr_changed = true;
			}
			file_index->setdata(f);
			ob = (v8ob*)base->getblock_for_write(file_index->block, true);
			ob->version.version_1 = root->index_version_1;
			ob->version.version_2 = root->index_version_2;
			delete f;
		}
	}

	if(descr_changed) if(root->has_descr)
	{
		fopen = false;
		try
		{
			f = new TFileStream(dir + "descr", fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			if(msreg) msreg->AddMessage_("Ошибка открытия файла импорта таблицы descr", msWarning,
				"Файл", dir + "descr");
		}
		if(fopen)
		{
			if(!descr_table) descr_table = new v8object(base); // вообще, если descr_table == NULL, то это огромная ошибка!
			ob = (v8ob*)base->getblock_for_write(descr_table->get_block_number(), true);
			ob->version.version_1 = root->descr_version_1;
			ob->version.version_2 = root->descr_version_2;

			i = f->GetSize();
			buf = new char[i + 2];
			f->Read(buf, i);
			buf[i] =0;
			buf[i + 1] =0;
			str = String((WCHART*)buf);
			delete[] buf;
			delete f;

			i = str.Pos("{\"Files\",");
			if(i == 0)
			{
				if(msreg) msreg->AddMessage_("Ошибка поиска раздела Files в файле импорта таблицы descr", msWarning,
					"Файл", dir + "descr");
				delete root;
				return;
			}
			str.SetLength(i - 1);
			str += "{\"Files\",";
			str += file_data ? String(file_data->get_block_number()) : String("0");
			str += ",";
			str += file_blob ? String(file_blob->get_block_number()) : String("0");
			str += ",";
			str += file_index ? String(file_index->get_block_number()) : String("0");
			str += "}\n}";
			descr_table->setdata(str.c_str(), str.GetLength() * 2);

		}


	}

	base->flush();

	delete root;
}

//---------------------------------------------------------------------------
void Table::set_edit_value(uint32_t phys_numrecord, int32_t numfield, bool null, String value, TStream* st)
{
	Field* fld;
	char* rec;
	char* k;
	char* editrec;
	char* fldvalue;
	type_fields tf;
	bool changed;
	changed_rec* cr;
	changed_rec* cr2;
	int32_t i, j;
	TStream** ost;

	if(!edit) return;

	fld = fields[numfield];
	tf = fld->gettype();
	if(tf == tf_version || tf == tf_version8) return;
	if(null && !fld->null_exists) return;

	rec = new char[recordlen];
	fldvalue = new char[fld->getlen()];

	if(tf == tf_string || tf == tf_text || tf == tf_image)
	{
		memset(fldvalue, 0, fld->getlen());
		k = fldvalue;
		if(fld->null_exists)
		{
			if(!null && st) *k = 1;
			k++;
		}
		*(TStream**)k = st;
	}
	else fld->get_bynary_value(fldvalue, null, value);

	changed = true;
	if(phys_numrecord < phys_numrecords)
	{
		getrecord(phys_numrecord, rec);
		changed = memcmp(rec + fld->offset, fldvalue, fld->len) != 0;
	}

	for(cr = ch_rec; cr; cr = cr->next) if(cr->numrec == phys_numrecord) break;
	if(!cr)
	{
		if(!changed)
		{
			delete[] rec;
			delete[] fldvalue;
			return; // значение не изменилось, ничего не делаем
		}
		cr = new changed_rec(this, phys_numrecord >= phys_numrecords ? crt_insert : crt_changed, phys_numrecord);
		if(phys_numrecord <= phys_numrecords) memcpy(cr->rec, rec, recordlen);
	}

	editrec = cr->rec;

	if(cr->fields[numfield]) if(tf == tf_string || tf == tf_text || tf == tf_image)
	{
		ost = (TStream**)(editrec + fld->offset + (fld->getnull_exists() ? 1 : 0));
		if(*ost != st)
		{
			delete *ost;
			*ost = NULL;
		}
	}

	if(changed)
	{
		memcpy(editrec + fld->offset, fldvalue, fld->len);
		cr->fields[numfield] = 1;
	}
	else
	{
		cr->fields[numfield] = 0;
		j = 0;
		for(i = 0; i < num_fields; i++) j += cr->fields[i];
		if(j == 0)
		{
			// измененных полей больше нет, надо удалить запись из измененных
			if(ch_rec == cr) ch_rec = cr->next;
			else for(cr2 = ch_rec; cr2; cr2 = cr2->next) if(cr2->next == cr)
			{
				cr2->next = cr->next;
				break;
			}
			delete cr;
		}
		else memcpy(editrec + fld->offset, fldvalue, fld->len);
	}

	delete[] rec;
	delete[] fldvalue;
}

//---------------------------------------------------------------------------
void Table::restore_edit_value(uint32_t phys_numrecord, int32_t numfield)
{
	Field* fld;
	char* rec;
	changed_rec* cr;
	changed_rec* cr2;
	int32_t i, j;
	type_fields tf;
	TStream** ost;

	if(phys_numrecord >= phys_numrecords) return;

	for(cr = ch_rec; cr; cr = cr->next) if(cr->numrec == phys_numrecord) break;
	if(!cr) return;
	if(cr->changed_type != crt_changed) return;

	fld = fields[numfield];
	tf = fld->gettype();
	if(cr->fields[numfield])
	{
		if(tf == tf_string || tf == tf_text || tf == tf_image)
		{
			ost = (TStream**)(cr->rec + fld->offset + (fld->getnull_exists() ? 1 : 0));
			delete *ost;
			*ost = NULL;
		}
	}

	cr->fields[numfield] = 0;
	j = 0;
	for(i = 0; i < num_fields; i++) j += cr->fields[i];
	if(j == 0)
	{
		// измененных полей больше нет, надо удалить запись из измененных
		if(ch_rec == cr) ch_rec = cr->next;
		else for(cr2 = ch_rec; cr2; cr2 = cr2->next) if(cr2->next == cr)
		{
			cr2->next = cr->next;
			break;
		}
		delete cr;
	}
	else{
		rec = new char[recordlen];
		getrecord(phys_numrecord, rec);
		memcpy(cr->rec + fld->offset, rec + fld->offset, fld->len);
		delete[] rec;
	}

}

//---------------------------------------------------------------------------
void Table::set_rec_type(uint32_t phys_numrecord, changed_rec_type crt)
{
	changed_rec* cr;
	changed_rec* cr2;

	for(cr = ch_rec; cr; cr = cr->next) if(cr->numrec == phys_numrecord) break;

	if(phys_numrecord < phys_numrecords)
	{
		switch(crt)
		{
			case crt_changed:
				error("Попытка прямой установки признака \"Изменена\" существующей записи таблицы",
					"Таблица", name,
					"Физический номер записи", phys_numrecord);
				break;
			case crt_insert:
				error("Попытка прямой установки признака \"Добавлена\" существующей записи таблицы",
					"Таблица", name,
					"Физический номер записи", phys_numrecord);
			case crt_delete:
				if(cr)
				{
					cr->clear();
					delete[] cr->rec;
					delete[] cr->fields;
					cr->changed_type = crt;
				}
				else new changed_rec(this, crt, phys_numrecord);
				break;
			case crt_not_changed:
				if(cr)
				{
					if(ch_rec == cr) ch_rec = cr->next;
					else for(cr2 = ch_rec; cr2; cr2 = cr2->next) if(cr2->next == cr)
					{
						cr2->next = cr->next;
						break;
					}
					delete cr;
				}
				break;
		}
	}
	else
	{
		switch(crt)
		{
			case crt_changed:
				error("Попытка прямой установки признака \"Изменена\" добавленной записи таблицы",
					"Таблица", name,
					"Физический номер записи", phys_numrecord);
				break;
			case crt_insert:
				if(cr) cr->changed_type = crt;
				else
				{
					if(phys_numrecord > phys_numrecords + added_numrecords)
					{
						error("Попытка добавленния записи таблицы, с номером больше максимального",
							"Таблица", name,
							"Максимальный номер записи", phys_numrecords + added_numrecords,
							"Физический номер записи", phys_numrecord);
					}
					else
					{
						new changed_rec(this, crt, phys_numrecord);
						added_numrecords++;
					}
				}
				break;
			case crt_delete:
				if(cr)
				{
					if(ch_rec == cr) ch_rec = cr->next;
					else for(cr2 = ch_rec; cr2; cr2 = cr2->next) if(cr2->next == cr)
					{
						cr2->next = cr->next;
						break;
					}
					delete cr;
					added_numrecords--;
				}
				for(cr = ch_rec; cr; cr = cr->next) if(cr->numrec > phys_numrecord) cr->numrec--;
				break;
			case crt_not_changed:
				error("Попытка прямой установки признака \"Не изменена\" добавленной записи таблицы",
					"Таблица", name,
					"Физический номер записи", phys_numrecord);
				break;
		}
	}
}
#endif //#ifdef PublicRelease

//---------------------------------------------------------------------------
char* Table::get_edit_record(uint32_t phys_numrecord, char* rec)
{
	changed_rec* cr;
	for(cr = ch_rec; cr; cr = cr->next) if(phys_numrecord == cr->numrec)
	{
		if(cr->changed_type != crt_delete)
		{
			memcpy(rec, cr->rec, recordlen);
			return rec;
		}
		break;
	}
	return getrecord(phys_numrecord, rec);
}

//---------------------------------------------------------------------------
uint32_t Table::get_phys_numrec(int32_t ARow, class index* cur_index)
{
	uint32_t numrec;

	if(ARow == 0)
	{
		error("Попытка получения номера физической записи по нулевому номеру строки.",
			"Таблица", name);
		return 0;
	}


#ifndef PublicRelease
	if(edit)
	{
		if((uint32_t)ARow > log_numrecords + added_numrecords)
		{
			error("Попытка получения номера физической записи по номеру строки, превышающему количество записей",
				"Таблица", name,
				"Количество логических записей", log_numrecords,
				"Количество добавленных записей", added_numrecords,
				"Номер строки", ARow);
			return 0;
		}
		if((uint32_t)ARow > log_numrecords) return ARow - 1 - log_numrecords + phys_numrecords;
	}
#endif //#ifdef PublicRelease

	if((uint32_t)ARow > log_numrecords)
	{
		error("Попытка получения номера физической записи по номеру строки, превышающему количество записей",
			"Таблица", name,
			"Количество логических записей", log_numrecords,
			"Номер строки", ARow);
		return 0;
	}
	if(cur_index) numrec = cur_index->get_numrec(ARow - 1);
	else
	{
		#ifndef console
		#ifndef getcfname
		tr_syn->BeginRead();
		#endif
		#endif
		numrec = recordsindex[ARow - 1];
		#ifndef console
		#ifndef getcfname
		tr_syn->EndRead();
		#endif
		#endif
	}

	return numrec;
}

//---------------------------------------------------------------------------
void Table::create_file_data()
{
	if(!file_data) return;
	file_data = new v8object(base);
	refresh_descr_table();
}

//---------------------------------------------------------------------------
void Table::create_file_blob()
{
	if(!file_blob) return;
	file_blob = new v8object(base);
	refresh_descr_table();
}

//---------------------------------------------------------------------------
void Table::create_file_index()
{
	if(!file_index) return;
	file_index = new v8object(base);
	refresh_descr_table();

}

//---------------------------------------------------------------------------
void Table::refresh_descr_table()
{
	error("Попытка обновления файла описания таблицы.",
		"Таблица", name);
	return;

}

#ifndef PublicRelease
//---------------------------------------------------------------------------
void Table::delete_data_record(uint32_t phys_numrecord)
{
	char* rec;
	int32_t first_empty_rec;

	if(!edit)
	{
		error("Попытка удаления записи из файла file_data не в режиме редактирования.",
			"Таблица", name,
			"Индекс удаляемой записи", phys_numrecord);
		return;
	}

	if(!file_data)
	{
		error("Попытка удаления записи из несуществующего файла file_data.",
			"Таблица", name,
			"Индекс удаляемой записи", phys_numrecord);
		return;
	}

	if(phys_numrecord >= phys_numrecords)
	{
		error("Попытка удаления записи в файле file_data за пределами файла.",
			"Таблица", name,
			"Всего записей", phys_numrecords,
			"Индекс удаляемой записи", phys_numrecord);
		return;
	}

	if(!phys_numrecord)
	{
		error("Попытка удаления нулевой записи в файле file_data.",
			"Таблица", name,
			"Всего записей", phys_numrecords);
		return;
	}

	if(phys_numrecord == phys_numrecords - 1)
	{
		file_data->set_len(phys_numrecord * recordlen);
		phys_numrecords--;
	}
	else
	{
		rec = new char[recordlen];
		memset(rec, 0, recordlen);
		file_data->getdata(&first_empty_rec, 0, 4);
		*((int32_t*)rec) = first_empty_rec;
		file_data->setdata(&first_empty_rec, 0, 4);
		write_data_record(phys_numrecord, rec);
		delete[] rec;
	}

}

//---------------------------------------------------------------------------
void Table::delete_blob_record(uint32_t blob_numrecord)
{
	int32_t prev_free_first;
	int32_t i, j;

	if(!edit)
	{
		error("Попытка удаления записи из файла file_blob не в режиме редактирования.",
			"Таблица", name,
			"Смещение удаляемой записи", blob_numrecord << 8);
		return;
	}

	if(!file_blob)
	{
		error("Попытка удаления записи из несуществующего файла file_blob.",
			"Таблица", name,
			"Смещение удаляемой записи", blob_numrecord << 8);
		return;
	}

	if(blob_numrecord << 8 >= file_blob->getlen())
	{
		error("Попытка удаления записи в файле file_blob за пределами файла.",
			"Таблица", name,
			"Смещение удаляемой записи", blob_numrecord << 8,
			"Длина файла", file_blob->getlen());
		return;
	}

	if(blob_numrecord == 0)
	{
		error("Попытка удаления нулевой записи в файле file_blob.",
			"Таблица", name,
			"Длина файла", file_blob->getlen());
		return;
	}

	file_blob->getdata(&prev_free_first, 0, 4); // читаем предыдущее начало свободных блоков
	for(i = blob_numrecord; i; file_blob->getdata(&i, i << 8, 4)) j = i; // ищем последний блок в цепочке удаляемых
	file_blob->setdata(&prev_free_first, j << 8, 4); // устанавливаем в конец цепочки удаляемых блоков старое начало цепочки свободных блоков
	file_blob->setdata(&blob_numrecord, 0, 4); // устанавливаем новое начало цепочки свободных блоков на начало удаляемых блоков
}

//---------------------------------------------------------------------------
void Table::delete_index_record(uint32_t phys_numrecord)
{
	char* rec;

	rec = new char[recordlen];
	getrecord(phys_numrecord, rec);
	delete_index_record(phys_numrecord, rec);
	delete[] rec;
}

//---------------------------------------------------------------------------
void Table::delete_index_record(uint32_t phys_numrecord, char* rec)
{
	class index* ind;
	int32_t i;

	if(*rec)
	{
		error("Попытка удаления индекса удаленной записи.",
			"Таблица", name,
			"Физический номер записи", phys_numrecord);
		return;
	}

	for(i = 0; i < num_indexes; i++) indexes[i]->delete_index(rec, phys_numrecord);
}

//---------------------------------------------------------------------------
void Table::write_data_record(uint32_t phys_numrecord, char* rec)
{
	char* b;

	if(!edit)
	{
		error("Попытка записи в файл file_data не в режиме редактирования.",
			"Таблица", name,
			"Индекс записываемой записи", phys_numrecord);
		return;
	}

	if(!file_data) create_file_data();

	if(phys_numrecord > phys_numrecords && !(phys_numrecord == 1 && phys_numrecords == 0))
	{
		error("Попытка записи в файл file_data за пределами файла.",
			"Таблица", name,
			"Всего записей", phys_numrecords,
			"Индекс записываемой записи", phys_numrecord);
		return;
	}

	if(!phys_numrecord)
	{
		error("Попытка записи нулевой записи в файл file_data.",
			"Таблица", name,
			"Всего записей", phys_numrecords);
		return;
	}

	if(phys_numrecords == 0)
	{
		b = new char[recordlen];
		memset(b, 0, recordlen);
		b[0] = 1;
		file_data->setdata(b, 0, recordlen);
		delete[] b;
	}
	file_data->setdata(rec, phys_numrecord * recordlen, recordlen);
}

//---------------------------------------------------------------------------
uint32_t Table::write_blob_record(char* blob_record, uint32_t blob_len)
{
	uint32_t cur_block, cur_offset, prev_offset, first_block, next_block;
	uint16_t cur_len;
	uint32_t zero = 0;

	if(!edit)
	{
		error("Попытка записи в файл file_blob не в режиме редактирования.",
			"Таблица", name);
		return 0;
	}

	if(!blob_len) return 0;

	if(!file_blob)
	{
		create_file_blob();
		char* b = new char[256];
		memset(b, 0, 256);
		file_blob->setdata(b, 0, 256);
		delete[] b;
	}

	file_blob->getdata(&first_block, 0, 4);
	if(!first_block) first_block = file_blob->getlen() >> 8;
	prev_offset = 0;

	for(cur_block = first_block; blob_len; blob_len -= cur_len, cur_block = next_block, blob_record += cur_len)
	{
		cur_len = MIN(blob_len, 250);
		if(cur_len < 250) memset(blob_record, 0, 250);

		if(prev_offset) file_blob->setdata(&cur_block, prev_offset, 4);

		cur_offset = cur_block << 8;
		next_block = 0;
		if(cur_offset < file_blob->getlen()) file_blob->getdata(&next_block, cur_offset, 4);

		file_blob->setdata(&zero, cur_offset, 4);
		file_blob->setdata(&cur_len, cur_offset + 4, 2);
		file_blob->setdata(blob_record, cur_offset + 6, 250);

		if(!next_block) next_block = file_blob->getlen() >> 8;
		prev_offset = cur_offset;
	}
	if(next_block << 8 < file_blob->getlen()) file_blob->setdata(&next_block, 0, 4);
	else file_blob->setdata(&zero, 0, 4);

	return first_block;
}

//---------------------------------------------------------------------------
uint32_t Table::write_blob_record(TStream* bstr)
{
	uint32_t cur_block, cur_offset, prev_offset, first_block, next_block;
	uint16_t cur_len;
	uint32_t blob_len;
	uint32_t zero;
	char blob_record[256];

	if(!edit)
	{
		error("Попытка записи в файл file_blob не в режиме редактирования.",
			"Таблица", name);
		return 0;
	}

	blob_len = bstr->GetSize();
	if(!blob_len) return 0;
	bstr->Seek(0, soFromBeginning);
	zero = 0;

	if(!file_blob)
	{
		create_file_blob();
		memset(blob_record, 0, 256);
		file_blob->setdata(blob_record, 0, 256);
	}

	file_blob->getdata(&first_block, 0, 4);
	if(!first_block) first_block = file_blob->getlen() >> 8;
	prev_offset = 0;

	for(cur_block = first_block; blob_len; blob_len -= cur_len, cur_block = next_block)
	{
		cur_len = MIN(blob_len, 250);
		if(cur_len < 250) memset(blob_record, 0, 250); //-V512
		bstr->Read(blob_record, cur_len);

		if(prev_offset) file_blob->setdata(&cur_block, prev_offset, 4);

		cur_offset = cur_block << 8;
		next_block = 0;
		if(cur_offset < file_blob->getlen()) file_blob->getdata(&next_block, cur_offset, 4);

		file_blob->setdata(&zero, cur_offset, 4);
		file_blob->setdata(&cur_len, cur_offset + 4, 2);
		file_blob->setdata(blob_record, cur_offset + 6, 250);

		if(!next_block) next_block = file_blob->getlen() >> 8;
		prev_offset = cur_offset;
	}
	if(next_block << 8 < file_blob->getlen()) file_blob->setdata(&next_block, 0, 4);
	else file_blob->setdata(&zero, 0, 4);

	return first_block;
}

//---------------------------------------------------------------------------

void Table::write_index_record(const uint32_t phys_numrecord, const char* rec)
{
	char* index_buf;
	class index* ind;
	int32_t i;

	if(*rec)
	{
		error("Попытка записи индексов помеченной на удаление записи.",
			"Таблица", name);
		return;
	}

	for(i = 0; i < num_indexes; i++) indexes[i]->write_index(phys_numrecord, rec);

}

//---------------------------------------------------------------------------
void Table::cancel_edit()
{
	changed_rec* cr;
	changed_rec* cr2;
	for(cr = ch_rec; cr;)
	{
		cr2 = cr->next;
		delete cr;
		cr = cr2;
	}

	edit = false;
	ch_rec = NULL;
	added_numrecords = 0;
}

//---------------------------------------------------------------------------
void Table::end_edit()
{
	changed_rec* cr;

	// удаляем удаленные записи
	for(cr = ch_rec; cr; cr = cr->next) if(cr->changed_type == crt_delete) delete_record(cr->numrec);

	// записываем измененные записи
	for(cr = ch_rec; cr; cr = cr->next) if(cr->changed_type == crt_changed) update_record(cr->numrec, cr->rec, cr->fields);

	// добавляем новые записи
	for(cr = ch_rec; cr; cr = cr->next) if(cr->changed_type == crt_insert) insert_record(cr->rec);

	cancel_edit();
	base->flush();
}

//---------------------------------------------------------------------------
void Table::delete_record(uint32_t phys_numrecord)
{
	int32_t i;
	uint32_t j;
	Field* f;
	type_fields tf;
	char* rec;

	rec = new char[recordlen];
	getrecord(phys_numrecord, rec);

	delete_index_record(phys_numrecord, rec);

	for(i = 0; i < num_fields; i++)
	{
		f = fields[i];
		tf = f->type;
		if(tf == tf_image || tf == tf_string || tf == tf_text)
		{
			j = *(uint32_t*)(rec + f->offset);
			if(j) delete_blob_record(j);
		}
	}

	delete_data_record(phys_numrecord);

	delete[] rec;
}

//---------------------------------------------------------------------------
void Table::insert_record(char* rec)
{
	int32_t i, offset;
	char* j;
	Field* f;
	type_fields tf;
	uint32_t phys_numrecord;
	uint32_t k, l;
	_version ver;
	TStream** st;

	if(!file_data) create_file_data();

	for(i = 0; i < num_fields; i++)
	{
		f = fields[i];
		tf = f->type;
		offset = f->offset + (f->getnull_exists() ? 1 : 0);
		switch(tf)
		{
			case tf_image:
			case tf_string:
			case tf_text:
				st = (TStream**)(rec + offset);
				if(*st)
				{
					l = (*st)->GetSize();
					k = write_blob_record(*st);
					delete *st;
					*st = NULL;
				}
				else{
					k = 0;
					l = 0;
				}
				*(uint32_t*)(rec + offset) = k;
				*(uint32_t*)(rec + offset + 4) = l;
				if(f->getnull_exists()) *(rec + f->offset) = l ? 1 : 0;
			case tf_version:
				file_data->get_version_rec_and_increase(&ver);
				memcpy(rec + offset, &ver, 8);
				memcpy(rec + offset + 8, &ver, 8);
				break;
			case tf_version8:
				file_data->get_version_rec_and_increase(&ver);
				memcpy(rec + offset, &ver, 8);
				break;
		}
	}

	if(file_data->len == 0)
	{
		j = new char[recordlen];
		memset(j, 0, recordlen);
		*j = 1;
		file_data->setdata(j, 0, recordlen);
		delete[] j;
		phys_numrecord = 1;
	}
	else
	{
		file_data->getdata(&phys_numrecord, 1, 4);
		if(phys_numrecord)
		{
			file_data->getdata(&k, phys_numrecord * recordlen + 1, 4);
			file_data->setdata(&k, 1, 4);
		}
		else phys_numrecord = file_data->len / recordlen;
	}

	write_data_record(phys_numrecord, rec);
	write_index_record(phys_numrecord, rec);

}

//---------------------------------------------------------------------------
void Table::update_record(uint32_t phys_numrecord, char* rec, char* changed_fields)
{
	char* orec;
	int32_t i, offset;
	Field* f;
	type_fields tf;
	uint32_t k, l;
	_version ver;
	TStream** st;

	orec = new char[recordlen];
	getrecord(phys_numrecord, orec);
	delete_index_record(phys_numrecord, orec);
	for(i = 0; i < num_fields; i++)
	{
		f = fields[i];
		tf = f->type;
		offset = f->offset + (f->getnull_exists() ? 1 : 0);
		if(changed_fields[i])
		{
			if(tf == tf_image || tf == tf_string || tf == tf_text)
			{
				if(f->getnull_exists())
				{
					if(*(orec + f->offset))
					{
						k = *(uint32_t*)(orec + offset);
						if(k) delete_blob_record(k);
					}
					if(*(rec + f->offset))
					{
						st = (TStream**)(rec + offset);
						if(*st)
						{
							l = (*st)->GetSize();
							k = write_blob_record(*st);
							delete *st;
							*st = NULL;
							*(orec + f->offset) = 1;
						}
						else{
							k = 0;
							l = 0;
							*(orec + f->offset) = 0;
						}
					}
					else{
						k = 0;
						l = 0;
						*(orec + f->offset) = 0;
					}
				}
				else
				{
					k = *(uint32_t*)(orec + offset);
					if(k) delete_blob_record(k);

					st = (TStream**)(rec + offset);
					if(*st)
					{
						l = (*st)->GetSize();
						k = write_blob_record(*st);
						delete *st;
						*st = NULL;
					}
					else{
						k = 0;
						l = 0;
					}
				}
				*(uint32_t*)(orec + offset) = k;
				*(uint32_t*)(orec + offset + 4) = l;
			}
			else memcpy(orec + f->offset, rec + f->offset, f->len);
		}
		else
		{
			if(tf == tf_version)
			{
				file_data->get_version_rec_and_increase(&ver);
				memcpy(orec + offset + 8, &ver, 8);
			}
			else if(tf == tf_version8)
			{
				file_data->get_version_rec_and_increase(&ver);
				memcpy(orec + offset, &ver, 8);
			}
		}

	}

	write_index_record(phys_numrecord, orec);
	write_data_record(phys_numrecord, orec);
	delete[] orec;

}

//---------------------------------------------------------------------------
// получить шаблон проверки записи (массив, содержащий для каждого байта массив из 256 байт, содержащий 0, если значение не допусимо и 1, если допустимо)
char* Table::get_record_template_test()
{
	int32_t len;
	char* res;
	char* curp;
	int32_t i, j, l;
	Field* f;
	bool required;

	len = recordlen << 8;
	res = new char[len];
	memset(res, 0, len);

	for(i = 0; i < num_fields; i++)
	{
		required = false;
		f = fields[i];
		curp = res + (f->getoffset() << 8);

		if(f->getnull_exists())
		{
			curp[0] = 1;
			curp[1] = 1;
			curp += 256;
		}

		l = f->getlength();
		switch(f->gettype())
		{
			case tf_binary: // B // длина = length
				memset(curp, 1, 256 * l);
				break;
			case tf_bool: // L // длина = 1
				curp[0] = 1;
				curp[1] = 1;
				break;
			case tf_numeric: // N // длина = (length + 2) / 2
				j = (l + 2) / 2;
				for(; j > 0; --j)
				{
					memcpy(curp, NUM_TEST_TEMPLATE, 256);
					curp += 256;
				}
				break;
			case tf_char: // NC // длина = length * 2
				memset(curp, 1, 256 * 2 * l);
				break;
			case tf_varchar: // NVC // длина = length * 2 + 2
				if(l > 255) j = 256;
				else j = l + 1;
				memset(curp, 1, j);
				//curp[0x20] = 1;
				curp += 256;
				j = (l >> 8) + 1;
				memset(curp, 1, j);
				curp += 256;
				memset(curp, 1, 256 * 2 * l);
				break;
			case tf_version: // RV // 16, 8 версия создания и 8 версия модификации ? каждая версия int32_t(изменения) + int32_t(реструктуризация)
				memset(curp, 1, 256 * 16);
				break;
			case tf_string: // NT // 8 (unicode text)
				memset(curp, 1, 256 * 8);
				break;
			case tf_text: // T // 8 (ascii text)
				memset(curp, 1, 256 * 8);
				break;
			case tf_image: // I // 8 (image = bynary data)
				memset(curp, 1, 256 * 8);
				break;
			case tf_datetime: // DT //7
				if(f->getname().CompareIC("_DATE_TIME") == 0) required = true;
				else if(f->getname().CompareIC("_NUMBERPREFIX") == 0) required = true;

				memcpy(curp, DATE1_TEST_TEMPLATE, 256);
				curp += 256;
				memcpy(curp, NUM_TEST_TEMPLATE, 256);
				curp += 256;
				memcpy(curp, DATE3_TEST_TEMPLATE, 256);
				if(required) curp[0] = 0;
				curp += 256;
				memcpy(curp, DATE4_TEST_TEMPLATE, 256);
				if(required) curp[0] = 0;
				curp += 256;
				memcpy(curp, DATE5_TEST_TEMPLATE, 256);
				curp += 256;
				memcpy(curp, DATE67_TEST_TEMPLATE, 256);
				curp += 256;
				memcpy(curp, DATE67_TEST_TEMPLATE, 256);
				break;
			case tf_version8: // 8, скрытое поле при recordlock == false и отсутствии поля типа tf_version
				memset(curp, 1, 256 * 8);
				break;
			case tf_varbinary: // VB // длина = length + 2
				if(l > 255) j = 256;
				else j = l + 1;
				memset(curp, 1, j);
				curp += 256;
				j = (l >> 8) + 1;
				memset(curp, 1, j);
				curp += 256;
				memset(curp, 1, 256 * l);
				break;
		}
	}

	res[0] = 1;

	return res;
}

#endif //#ifdef PublicRelease

//---------------------------------------------------------------------------
// заполнить recordsindex не динамически
void Table::fillrecordsindex()
{
	uint32_t i;
	int32_t j;
	char* rec;

    if(recordsindex_complete) return;
    recordsindex = new uint32_t [phys_numrecords];
	rec = new char[recordlen];

	j = 0;
	for(i = 0; i < phys_numrecords; i++)
	{
		getrecord(i, rec);
		if(*rec) continue;
		recordsindex[j++] = i;
	}
	recordsindex_complete = true;
	numrecords_review = phys_numrecords;
	numrecords_found = j;
	log_numrecords = j;

	delete[] rec;
}

String Table::get_file_name_for_field(int32_t num_field, char* rec, uint32_t numrec)
{
	String s("");
	int32_t i;
	class index* ind;

	if(num_indexes)
	{
		ind = indexes[0];
		for(i = 0; i < num_indexes; i++) if(indexes[i]->is_primary)
		{
			ind = indexes[i];
			break;
		}
		for(i = 0; i < ind->num_records; i++)
		{
			if(s.GetLength()) s += "_";
			s += ind->records[i].field->get_XML_presentation(rec);
		}
		if(!ind->is_primary && numrec){
			s += "_";
			s += numrec;
		}
	}
	if(!issystem || !(name.CompareIC("CONFIG") == 0 || name.CompareIC("CONFIGSAVE") == 0 || name.CompareIC("FILES") == 0 || name.CompareIC("PARAMS") == 0))
	{
		if(s.GetLength()) s += "_";
		s += fields[num_field]->getname();
	}

	return s;
}

String Table::get_file_name_for_record(char* rec)
{
	String s("");
	int32_t i;
	class index* ind;

	if(num_indexes)
	{
		ind = indexes[0];
		for(i = 0; i < num_indexes; i++) if(indexes[i]->is_primary)
		{
			ind = indexes[i];
			break;
		}
		for(i = 0; i < ind->num_records; i++)
		{
			if(s.GetLength()) s += "_";
			s += ind->records[i].field->get_XML_presentation(rec);
		}
	}

	return s;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс table_file

//---------------------------------------------------------------------------
table_file::table_file(Table* _t, const String& _name, uint32_t _maxpartno)
{
	uint32_t i;

	t = _t;
	name = _name;
	maxpartno = _maxpartno;
	addr = new table_blob_file[maxpartno + 1];
	for(i = 0; i <= maxpartno; ++i)
	{
		addr[i].blob_start = 0;
		addr[i].blob_length = 0;
	}
	memset(&ft_create, 0, sizeof(ft_create));
	memset(&ft_modify, 0, sizeof(ft_modify));
}

//---------------------------------------------------------------------------
table_file::~table_file()
{
	delete[] addr;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс TableFiles

//---------------------------------------------------------------------------
TableFiles::TableFiles(Table* t)
{
	Field* filename;
	Field* f;
	Field* partno;
	int32_t* start;
	int32_t* length;
	unsigned char* create;
	unsigned char* modify;
	uint32_t i;
	size_t j;
	String s;
	table_rec tr, *ptr;
	std::vector<table_rec> allrec;
	std::map<String,int32_t> maxpartnos;
	std::map<String,int32_t>::iterator pmaxpartno;
	table_file* tf;
	std::map<String,table_file*>::iterator pfilesmap;

	tab = t;
	ready = test_table();
	if(!ready) return;

	rec = new char[tab->get_recordlen()];

	filename = tab->getfield(0);

	f = tab->getfield(1);
	create = (unsigned char*)(rec + f->getoffset());
	f = tab->getfield(2);
	modify = (unsigned char*)(rec + f->getoffset());
	f = tab->getfield(5);
	start = (int32_t*)(rec + f->getoffset());
	length = start + 1;

	if(tab->get_numfields() > 6) partno = tab->getfield(6);
	else partno = NULL;

	for(i = 0; i < tab->get_phys_numrecords(); ++i)
	{
		tab->getrecord(i, rec);
		if(*rec) continue;
		if(*start == 0) continue;
		if(*length == 0) continue;

		tr.name = filename->get_presentation(rec);
		if(tr.name.IsEmpty()) continue;

		tr.addr.blob_start = *start;
		tr.addr.blob_length = *length;

		if(partno) tr.partno = partno->get_presentation(rec, true).ToIntDef(0);
		else tr.partno = 0;
		time1CD_to_FileTime(&tr.ft_create, create);
		time1CD_to_FileTime(&tr.ft_modify, modify);

		allrec.push_back(tr);

		s = tr.name.UpperCase();
		pmaxpartno = maxpartnos.find(s);
		if(pmaxpartno == maxpartnos.end()) maxpartnos[s] = tr.partno;
		else if(pmaxpartno->second < tr.partno) pmaxpartno->second = tr.partno;
	}

	for(pmaxpartno = maxpartnos.begin(); pmaxpartno != maxpartnos.end(); ++pmaxpartno)
	{
		tf = new table_file(tab, pmaxpartno->first, pmaxpartno->second);
		allfiles[pmaxpartno->first] = tf;
	}

	for(j = 0; j < allrec.size(); ++j)
	{
		ptr = &(allrec[j]);
		pfilesmap = allfiles.find(ptr->name.UpperCase());
		tf = pfilesmap->second;
		tf->addr[ptr->partno] = ptr->addr;
		if(!ptr->partno)
		{
			tf->ft_create = ptr->ft_create;
			tf->ft_modify = ptr->ft_modify;
			tf->name = ptr->name;
		}
	}
}

//---------------------------------------------------------------------------
TableFiles::~TableFiles()
{
	if (rec) {
		delete[] rec;
		rec = nullptr;
	}
	for (auto p : allfiles) {
		delete p.second;
	}
	allfiles.clear();
}

//---------------------------------------------------------------------------
bool TableFiles::test_table()
{
	if(!tab) return false;
	if(tab->get_numfields() < 6)
	{
		error("Ошибка проверки таблицы контейнера файлов. В таблице меньше 6 полей"
			,"Таблица", tab->getname()
			,"Кол-во полей", tab->get_numfields());
		return false;
	}

	if(tab->get_numfields() > 7)
	{
		error("Ошибка проверки таблицы контейнера файлов. В таблице больше 7 полей"
			,"Таблица", tab->getname()
			,"Кол-во полей", tab->get_numfields());
		return false;
	}

	if(tab->getfield(0)->getname().CompareIC("FILENAME"))
	{
		error("Ошибка проверки таблицы контейнера файлов. Первое поле таблицы не FILENAME"
			,"Таблица", tab->getname()
			,"Поле", tab->getfield(0)->getname());
		return false;
	}

	if(tab->getfield(1)->getname().CompareIC("CREATION"))
	{
		error("Ошибка проверки таблицы контейнера файлов. Второе поле таблицы не CREATION"
			,"Таблица", tab->getname()
			,"Поле", tab->getfield(1)->getname());
		return false;
	}

	if(tab->getfield(2)->getname().CompareIC("MODIFIED"))
	{
		error("Ошибка проверки таблицы контейнера файлов. Третье поле таблицы не MODIFIED"
			,"Таблица", tab->getname()
			,"Поле", tab->getfield(2)->getname());
		return false;
	}

	if(tab->getfield(3)->getname().CompareIC("ATTRIBUTES"))
	{
		error("Ошибка проверки таблицы контейнера файлов. Четвертое поле таблицы не ATTRIBUTES"
			,"Таблица", tab->getname()
			,"Поле", tab->getfield(3)->getname());
		return false;
	}

	if(tab->getfield(4)->getname().CompareIC("DATASIZE"))
	{
		error("Ошибка проверки таблицы контейнера файлов. Пятое поле таблицы не DATASIZE"
			,"Таблица", tab->getname()
			,"Поле", tab->getfield(4)->getname());
		return false;
	}

	if(tab->getfield(5)->getname().CompareIC("BINARYDATA"))
	{
		error("Ошибка проверки таблицы контейнера файлов. Шестое поле таблицы не BINARYDATA"
			,"Таблица", tab->getname()
			,"Поле", tab->getfield(5)->getname());
		return false;
	}

	if(tab->get_numfields() > 6)
	{
		if(tab->getfield(6)->getname().CompareIC("PARTNO"))
		{
			error("Ошибка проверки таблицы контейнера файлов. Седьмое поле таблицы не PARTNO"
				,"Таблица", tab->getname()
				,"Поле", tab->getfield(6)->getname());
			return false;
		}
	}
	return true;
}

//---------------------------------------------------------------------------
table_file* TableFiles::getfile(const String& name)
{
	std::map<String,table_file*>::iterator p;

	p = allfiles.find(name.UpperCase());
	if(p == allfiles.end()) return NULL;
	else return p->second;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс TableFileStream

//---------------------------------------------------------------------------
TableFileStream::TableFileStream(table_file* tf)
{
	curoffset = 0;
	tablefile = tf;
	streams = new TStream*[tablefile->maxpartno + 1];
	for(uint32_t i = 0; i <= tablefile->maxpartno; ++i) streams[i] = NULL;
}

//---------------------------------------------------------------------------
TableFileStream::~TableFileStream()
{
	for(uint32_t i = 0; i <= tablefile->maxpartno; ++i) delete streams[i];
	delete[] streams;
}

//---------------------------------------------------------------------------
int32_t TableFileStream::Read(void *Buffer, int32_t Count)
{
	uint32_t nbf; // индекс текущего table_blob_file в tablefile->addr
	uint32_t ibf; // индекс внутри table_blob_file
	uint32_t curoff; // curoffset
	table_blob_file* addr;
	TStream* str;
	int32_t curlen;
	char* buf;

	buf = (char*)Buffer;
	addr = tablefile->addr;
	for(nbf = 0, curoff = 0; nbf <= tablefile->maxpartno; ++nbf)
	{
		if(curoff + addr[nbf].blob_length > curoffset)
		{
			ibf = curoffset - curoff;
			break;
		}
		curoff += addr[nbf].blob_length;
	}
	if(nbf > tablefile->maxpartno) return 0;

	while(Count)
	{
		curlen = MIN(Count, (int32_t)(addr[nbf].blob_length - ibf));
		str = streams[nbf];
		if(!str)
		{
			if(ibf == 0 && addr[nbf].blob_length == curlen)
			{

			}
		}

		if(str)
		{
			str->Seek(ibf, soBeginning);
			str->Read(buf, curlen);
		}
		else
		{

		}
		buf += curlen;
		Count -= curlen;
	}

	// TODO
	return 0;
}

//---------------------------------------------------------------------------
int32_t TableFileStream::Read(System::DynamicArray<System::t::Byte> Buffer, int32_t Offset, int32_t Count)
{
	// TODO
	return 0;
}

//---------------------------------------------------------------------------
int32_t TableFileStream::Seek(int32_t Offset, System::Word Origin)
{
	// TODO
	return 0;
}

//---------------------------------------------------------------------------
int64_t TableFileStream::Seek(const int64_t Offset, TSeekOrigin Origin)
{
	// TODO
	return 0;
}

//---------------------------------------------------------------------------

//********************************************************
// Класс T_1CD

//---------------------------------------------------------------------------
bool T_1CD::getblock(void* buf, uint32_t block_number, int32_t blocklen)
{
	if(!fs) return false;
	if(blocklen < 0) blocklen = pagesize;
	if(block_number >= length)
	{
		error("Попытка чтения блока за пределами файла.",
			"Индекс блока", tohex(block_number),
			"Всего блоков", tohex(length));
		return false;
	}

	memcpy(buf, memblock::getblock(fs, block_number), blocklen);
	return true;
}

//---------------------------------------------------------------------------
char*  T_1CD::getblock(uint32_t block_number)
{
	if(!fs) return NULL;
	if(block_number >= length)
	{
		error("Попытка чтения блока за пределами файла.",
			"Индекс блока", tohex(block_number),
			"Всего блоков", tohex(length));
		return NULL;
	}

	return memblock::getblock(fs, block_number);
}

//---------------------------------------------------------------------------
char*  T_1CD::getblock_for_write(uint32_t block_number, bool read)
{
	v8con* bc;


	if(!fs) return NULL;
	if(block_number > length)
	{
		error("Попытка получения блока за пределами файла базы.",
			"Индекс блока", tohex(block_number),
			"Всего блоков", tohex(length));
		return NULL;
	}

	if(block_number == length)
	{
		length++;
		fs->SetSize(fs->GetSize() + pagesize);
		bc = (v8con*)getblock_for_write(0, true);
		bc->length = length;
	}

	return memblock::getblock_for_write(fs, block_number, read);
}

//---------------------------------------------------------------------------
int32_t T_1CD::get_numtables()
{
	return num_tables;
}

//---------------------------------------------------------------------------
Table* T_1CD::gettable(int32_t numtable)
{
	if(numtable >= num_tables)
	{
		error("Попытка получения таблицы по номеру, превышающему количество таблиц",
			"Количество таблиц", num_tables,
			"Номер таблицы", numtable + 1);
		return NULL;
	}
	return tables[numtable];
}

//---------------------------------------------------------------------------
void T_1CD::init()
{
	filename    = "";
	fs          = nullptr;
	free_blocks = nullptr;
	root_object = nullptr;
	tables      = nullptr;
	num_tables  = 0;
	table_config     = nullptr;
	table_configsave = nullptr;
	table_params     = nullptr;
	table_files      = nullptr;
	table_dbschema   = nullptr;
	table_configcas  = nullptr;
	table_configcassave   = nullptr;
	table__extensionsinfo = nullptr;

	_files_config     = nullptr;
	_files_configsave = nullptr;
	_files_params     = nullptr;
	_files_files      = nullptr;
	_files_configcas  = nullptr;
	_files_configcassave = nullptr;

	cs_config     = nullptr;
	cs_configsave = nullptr;

	table_depot    = nullptr;
	table_users    = nullptr;
	table_objects  = nullptr;
	table_versions = nullptr;
	table_labels   = nullptr;
	table_history  = nullptr;
	table_lastestversions = nullptr;
	table_externals = nullptr;
	table_selfrefs  = nullptr;
	table_outrefs   = nullptr;

	supplier_configs_defined = false;
	locale                   = nullptr;

	is_infobase = false;
	is_depot    = false;
	Field::showGUIDasMS = false;

	pagemap  = nullptr;
	version  = ver8_2_14_0;
	pagesize = 0x1000;
	err      = nullptr;
	length   = 0;
	readonly = true;
}

//---------------------------------------------------------------------------
T_1CD::T_1CD()
{
	init();
}

//---------------------------------------------------------------------------
T_1CD::~T_1CD()
{
	if(free_blocks)
	{
		delete free_blocks;
		free_blocks = NULL;
	}

	if(root_object)
	{
		delete root_object;
		root_object = NULL;
	}

	delete cs_config;
	delete cs_configsave;

	delete _files_config;
	delete _files_configsave;
	delete _files_params;
	delete _files_files;
	delete _files_configcas;
	delete _files_configcassave;

	if(tables)
	{
		for(int32_t i = 0; i < num_tables; i++) delete tables[i];
		delete[] tables;
		tables = NULL;
	}
	num_tables = 0;

	// сначала закрываем кэшированные блоки (измененные блоки записывают себя в файл) ...
	memblock::delete_memblocks();

	// ... и только затем закрываем файл базы.
	if(fs)
	{
		delete fs;
		fs = NULL;
	}

	delete[] locale;
	if(pagemap) delete[] pagemap;
}

//---------------------------------------------------------------------------
T_1CD::T_1CD(String _filename, MessageRegistrator* _err, bool _monopoly)
{
	char* b;
	uint32_t* table_blocks;
	int32_t i, j;
	TMemoryStream* tstr;
	root_80* root80;
	root_81* root81;

	init();

	filename = System::Ioutils::TPath::GetFullPath(_filename);

	try
	{
		if(_monopoly) fs = new TFileStream(filename, fmOpenReadWrite | fmShareDenyWrite);
		else fs = new TFileStream(filename, fmOpenRead | fmShareDenyNone);
	}
	catch(...)
	{
		error("Ошибка открытия файла базы (файл открыт другой программой?)");
		fs = NULL;
		return;
	}

	v8con* cont = new v8con;
	fs->Read(cont, sizeof(v8con));

	if(memcmp(&(cont->sig), SIG_CON, 8) != 0)
	{
		error("Файл не является базой 1С (сигнатура не равна \"1CDBMSV8\")");
		delete fs;
		fs = NULL;
		delete cont;
		return;
	}

	pagesize = 0x1000;
	ver = cont->getver();
#ifndef delic
	if(ver == "8.0.3.0"){
		version = ver8_0_3_0;
		readonly = true;
	}
	else if(ver == "8.0.5.0"){
		version = ver8_0_5_0;
		readonly = true;
	}
	else if(ver == "8.1.0.0")
	{
		version = ver8_1_0_0;
		#ifdef PublicRelease
		readonly = true;
		#else
		readonly = !_monopoly;
		#endif
	}
	else if(ver == "8.2.0.0")
	{
		version = ver8_2_0_0;
		#ifdef PublicRelease
		readonly = true;
		#else
		readonly = !_monopoly;
		#endif
	}
	else
#endif
		 if(ver == "8.2.14.0")
	{
		version = ver8_2_14_0;
		#ifdef PublicRelease
		readonly = true;
		#else
		readonly = !_monopoly;
		#endif
	}
	else if(ver == "8.3.8.0")
	{
		version = ver8_3_8_0;
		#ifdef PublicRelease
		readonly = true;
		#else
		readonly = !_monopoly;
		#endif
		pagesize = cont->pagesize;
	}
	else
	{
		error("Неподдерживаемая версия базы 1С",
			"Версия базы", ver);
		delete fs;
		fs = NULL;
		delete cont;
		return;
	}

	length = fs->GetSize() / pagesize;
	if((int64_t)length * pagesize != fs->GetSize())
	{
		error(String("Длина файла базы не кратна длине страницы (" + tohex(pagesize) + ")"),
			"Длина файла", tohex(fs->GetSize()));
		delete fs;
		fs = NULL;
		return;
	}

	memblock::pagesize = pagesize;
	memblock::maxcount = 0x40000000 / pagesize; // гигабайт
	memblock::create_memblocks(length);

	if(length != cont->length)
	{
		error("Длина файла в блоках и количество блоков в заголовке не равны",
			"Длина файла в блоках", length,
			"Блоков в заголовке", cont->length);
	}

	free_blocks = new v8object(this, 1);
	root_object = new v8object(this, 2);


	if(version == ver8_0_3_0 || version == ver8_0_5_0)
	{
		root80 = (root_80*)root_object->getdata();

		locale = new char[strlen(root80->lang) + 1];
		strcpy(locale, root80->lang);

		num_tables = root80->numblocks;
		table_blocks = &(root80->blocks[0]);
	}
	else
	{
		if(version >= ver8_3_8_0)
		{
			tstr = new TMemoryStream;
			root_object->readBlob(tstr, 1);
			b = new char[tstr->GetSize()];
			memcpy(b, tstr->GetMemory(), tstr->GetSize());
			root81 = (root_81*)b;
		}
		else
		{
			root81 = (root_81*)root_object->getdata();
		}

		locale = new char[strlen(root81->lang) + 1];
		strcpy(locale, root81->lang);

		num_tables = root81->numblocks;
		table_blocks = &(root81->blocks[0]);

	}

	tables = new Table*[num_tables];
	for(i = 0, j = 0; i < num_tables; i++)
	{
		if(version < ver8_3_8_0)
		{
			tables[j] = new Table(this, table_blocks[i]);
		}
		else
		{
			root_object->readBlob(tstr, table_blocks[i]);
			tables[j] = new Table(this, String((char*)(tstr->GetMemory()), tstr->GetSize()), table_blocks[i]);
		}
		if(tables[j]->bad)
		{
			delete tables[j];
			continue;
		}
		if(!tables[j]->getname().CompareIC("CONFIG")) table_config = tables[j];
		if(!tables[j]->getname().CompareIC("CONFIGSAVE")) table_configsave = tables[j];
		if(!tables[j]->getname().CompareIC("PARAMS")) table_params = tables[j];
		if(!tables[j]->getname().CompareIC("FILES")) table_files = tables[j];
		if(!tables[j]->getname().CompareIC("DBSCHEMA")) table_dbschema = tables[j];
		if(!tables[j]->getname().CompareIC("CONFIGCAS")) table_configcas = tables[j];
		if(!tables[j]->getname().CompareIC("CONFIGCASSAVE")) table_configcassave = tables[j];
		if(!tables[j]->getname().CompareIC("_EXTENSIONSINFO")) table__extensionsinfo = tables[j];

		if(!tables[j]->getname().CompareIC("DEPOT")) table_depot = tables[j];
		if(!tables[j]->getname().CompareIC("USERS")) table_users = tables[j];
		if(!tables[j]->getname().CompareIC("OBJECTS")) table_objects = tables[j];
		if(!tables[j]->getname().CompareIC("VERSIONS")) table_versions = tables[j];
		if(!tables[j]->getname().CompareIC("LABELS")) table_labels = tables[j];
		if(!tables[j]->getname().CompareIC("HISTORY")) table_history = tables[j];
		if(!tables[j]->getname().CompareIC("LASTESTVERSIONS")) table_lastestversions = tables[j];
		if(!tables[j]->getname().CompareIC("EXTERNALS")) table_externals = tables[j];
		if(!tables[j]->getname().CompareIC("SELFREFS")) table_selfrefs = tables[j];
		if(!tables[j]->getname().CompareIC("OUTREFS")) table_outrefs = tables[j];

		if(j % 10 == 0) if(msreg) msreg->Status(String("Чтение таблиц ") + j);
		j++;
	}
	if(msreg) msreg->Status(String("Чтение таблиц ") + j);
	num_tables = j;

	if(version >= ver8_3_8_0)
	{
		delete[] b;
		delete tstr;
	}

#ifdef getcfname
	if(!table_config) error("Отсутствует таблица CONFIG");
#else
#ifdef delic
	if(!table_params) error("Отсутствует таблица PARAMS");
#ifdef delicfiles
	if(!table_config) error("Отсутствует таблица FILES");
	if(!table_config) error("Отсутствует таблица CONFIG");
#endif
#else
	if(!table_config && !table_configsave && !table_params && !table_files && !table_dbschema)
	{
		if(!table_depot && !table_users && !table_objects && !table_versions && !table_labels && !table_history && !table_lastestversions && !table_externals && !table_selfrefs && !table_outrefs)
		{
			if(msreg) msreg->AddMessage("База не является информационной базой 1С", msInfo);
		}
		else
		{
			is_depot = true;
			if(!table_depot) error("Отсутствует таблица DEPOT");
			if(!table_users) error("Отсутствует таблица USERS");
			if(!table_objects) error("Отсутствует таблица OBJECTS");
			if(!table_versions) error("Отсутствует таблица VERSIONS");
			if(!table_labels) error("Отсутствует таблица LABELS");
			if(!table_history) error("Отсутствует таблица HISTORY");
			if(!table_lastestversions) error("Отсутствует таблица LASTESTVERSIONS");
			if(!table_externals) error("Отсутствует таблица EXTERNALS");
			if(!table_selfrefs) error("Отсутствует таблица SELFREFS");
			if(!table_outrefs) error("Отсутствует таблица OUTREFS");
			Field::showGUIDasMS = true;
		}
	}
	else
	{
		is_infobase = true;
		if(!table_config) error("Отсутствует таблица CONFIG");
		if(!table_configsave) error("Отсутствует таблица CONFIGSAVE");
		if(!table_params) error("Отсутствует таблица PARAMS");
		if(!table_files) error("Отсутствует таблица FILES");
		if(!table_dbschema) error("Отсутствует таблица DBSCHEMA");
	}
#endif //#ifdef delic
#endif //#ifdef getcfname

	delete cont;
}

//---------------------------------------------------------------------------
bool T_1CD::is_open()
{
	return fs != NULL;
}

//---------------------------------------------------------------------------
db_ver T_1CD::getversion()
{
	return version;
}

//---------------------------------------------------------------------------
bool T_1CD::save_config(String _filename)
{
	if(!cs_config) cs_config = new ConfigStorageTableConfig(get_files_config());
	if(!cs_config->getready()) return false;
	return cs_config->save_config(_filename);
}

//---------------------------------------------------------------------------
bool T_1CD::save_configsave(String _filename)
{
	if(!cs_configsave) cs_configsave = new ConfigStorageTableConfigSave(get_files_config(), get_files_configsave());
	if(!cs_configsave->getready()) return false;
	return cs_configsave->save_config(_filename);
}

//---------------------------------------------------------------------------
void T_1CD::find_supplier_configs()
{
	std::map<String,table_file*>::iterator p;

	for(p = get_files_configsave()->files().begin(); p != get_files_configsave()->files().end(); ++p)
	{
		if(p->first.GetLength() == 73) add_supplier_config(p->second);
	}
	for(p = get_files_config()->files().begin(); p != get_files_config()->files().end(); ++p)
	{
		if(p->first.GetLength() == 73) add_supplier_config(p->second);
	}
	supplier_configs_defined = true;
}

//---------------------------------------------------------------------------
void T_1CD::add_supplier_config(table_file* tf)
{
	container_file* f;
	TStream* s;
	int32_t i;
	v8catalog* cat = NULL;
	v8catalog* cat2;
	v8file* file;
	tree* tr = NULL;
	String filenamemeta;
	String nodetype;
	String _name; // имя конфигурация поставщика
	String _supplier; // синоним конфигурация поставщика
	String _version; // версия конфигурация поставщика
	SupplierConfig sc;

	f = new container_file(tf, tf->name);
	if(!f->open())
	{
		delete f;
		return;
	}

	s = new TTempStream;

	try
	{
		try
		{
			ZInflateStream(f->stream, s);
		}
		catch(...)
		{
			error("Ошибка распаковки конфигурации поставщика",
				"Таблица", tf->t->getname(),
				"Имя", tf->name);
			delete s;
			return;
		}
		f->close();
		delete f;
		f = NULL;

		cat = new v8catalog(s, true);
		s = NULL;
		file = cat->GetFile("version");
		if(!file)
		{
			error("Не найден файл version в конфигурации поставщика",
				"Таблица", tf->t->getname(),
				"Имя файла", tf->name);
			delete cat;
			return;
		}

		tr = get_treeFromV8file(file);
		i = (*tr)[0][0][0].get_value().ToInt();
		delete tr;
		tr = NULL;

		#ifdef _DEBUG
		if(msreg) msreg->AddDebugMessage("Найдена версия контейнера конфигурации поставщика", msInfo,
			"Таблица", tf->t->getname(),
			"Имя файла", tf->name,
			"Версия", i);
		#endif

		if(i < 100) // 8.0
		{
			file = cat->GetFile("metadata");
			if(!file)
			{
				error("Не найден каталог metadata в конфигурации поставщика",
					"Таблица", tf->t->getname(),
					"Имя файла", tf->name);
				delete cat;
				return;
			}
			cat2 = file->GetCatalog();
			if(!cat2)
			{
				error("Файл metadata неявляется каталогом в конфигурации поставщика",
					"Таблица", tf->t->getname(),
					"Имя файла", tf->name);
				delete cat;
				return;
			}

		}
		else cat2 = cat; // else 8.1 или 8.2

		file = cat2->GetFile("root");
		if(!file)
		{
			error("Не найден файл root в конфигурации поставщика",
				"Таблица", tf->t->getname(),
				"Имя файла", tf->name);
			delete cat;
			return;
		}

		tr = get_treeFromV8file(file);
		filenamemeta = (*tr)[0][1].get_value();
		delete tr;
		tr = NULL;

		file = cat2->GetFile(filenamemeta);
		if(!file)
		{
			error("Не найден файл метаданных в конфигурации поставщика",
				"Таблица", tf->t->getname(),
				"Имя файла", tf->name,
				"Имя мета", filenamemeta);
			delete cat;
			return;
		}

		#ifdef _DEBUG
		if(msreg) msreg->AddDebugMessage("Найден файл метаданных в конфигурации поставщика", msInfo,
			"Таблица", tf->t->getname(),
			"Имя файла", tf->name,
			"Имя мета", filenamemeta);
		#endif

		tr = get_treeFromV8file(file);
		int32_t numnode = (*tr)[0][2].get_value().ToInt();
		for(i = 0; i < numnode; i++)
		{
			tree& node = (*tr)[0][3 + i];
			nodetype = node[0].get_value();
			if(nodetype.CompareIC("9cd510cd-abfc-11d4-9434-004095e12fc7") == 0) // узел "Общие"
			{
				tree& confinfo = node[1][1];
				int32_t verconfinfo = confinfo[0].get_value().ToInt();
				switch(verconfinfo)
				{
					case 15:
						_name = confinfo[1][1][2].get_value();
						_supplier = confinfo[11].get_value();
						_version = confinfo[12].get_value();
						break;
					case 22:
					case 32:
					case 34:
					case 36:
					case 37:
						_name = confinfo[1][1][2].get_value();
						_supplier = confinfo[14].get_value();
						_version = confinfo[15].get_value();
						break;
					default:
						_name = confinfo[1][1][2].get_value();
						_supplier = confinfo[14].get_value();
						_version = confinfo[15].get_value();
						#ifdef _DEBUG
						if(msreg) msreg->AddDebugMessage("Неизвестная версия свойств конфигурации поставщика", msInfo,
							"Таблица", tf->t->getname(),
							"Имя файла", tf->name,
							"Имя мета", filenamemeta,
							"Версия свойств", verconfinfo);
						#endif
						break;
				}
				break;
			}
		}
		delete tr;
		tr = NULL;

		if(i >= numnode)
		{
			error("Не найден узел Общие в метаданных конфигурации поставщика",
				"Таблица", tf->t->getname(),
				"Имя файла", tf->name,
				"Имя мета", filenamemeta);
			_supplier = "";
			_version = "";
		}
		#ifdef _DEBUG
		else
		{
			if(msreg) msreg->AddDebugMessage("Найдена конфигурация поставщика", msInfo,
				"Таблица", tf->t->getname(),
				"Имя файла", tf->name,
				"Имя", _name,
				"Версия", _version,
				"Поставщик", _supplier);
		}
		#endif


		sc.file = tf;
		sc.name = _name;
		sc.supplier = _supplier;
		sc.version = _version;
		supplier_configs.push_back(sc);

		delete cat;
		cat = NULL;
	}
	catch(...)
	{
		error("Произошла ошибка при разборе конфигурации поставщика",
			"Таблица", tf->t->getname(),
			"Имя файла", tf->name);
		delete cat;
		delete s;
		delete tr;
		delete f;
	}
}

//---------------------------------------------------------------------------
bool T_1CD::save_supplier_configs(uint32_t numcon, const String& _filename)
{
	TFileStream* _fs;
	container_file* f;
	table_file* tf;

	if(numcon >= supplier_configs.size()) return false;
	tf = supplier_configs[numcon].file;
	f = new container_file(tf, tf->name);
	if(!f->open())
	{
		delete f;
		return false;
	}

	try
	{
		_fs = new TFileStream(_filename, fmCreate);
	}
	catch(...)
	{
		error("Ошибка открытия файла конфигурации поставщика",
			"Имя файла", _filename);
		delete f;
		return false;
	}

	try
	{
		ZInflateStream(f->stream, _fs);
	}
	catch(...)
	{
		error("Ошибка распаковки файла конфигурации поставщика",
			"Имя файла", _filename);
		delete f;
		delete _fs;
		return false;
	}

	delete _fs;
	delete f;
	return true;
}

//---------------------------------------------------------------------------
bool T_1CD::get_readonly()
{
	return readonly;
}

//---------------------------------------------------------------------------
void T_1CD::set_block_as_free(uint32_t block_number)
{
	free_blocks->set_block_as_free(block_number);
}

//---------------------------------------------------------------------------
uint32_t T_1CD::get_free_block()
{
	return free_blocks->get_free_block();
}

//---------------------------------------------------------------------------
void T_1CD::flush()
{
	memblock::flush();
}

//---------------------------------------------------------------------------

#ifndef PublicRelease
void T_1CD::find_lost_objects()
{
	uint32_t i;
	char buf[8];
	v8object* v8obj;
	bool block_is_find;

	for(i = 1; i < length; i++)
	{
		getblock(buf, i, 8);
		if(memcmp(buf, SIG_OBJ, 8) == 0)
		{
			block_is_find = false;
			for(v8obj = v8object::get_first(); v8obj; v8obj = v8obj->get_next())
			{
				if(v8obj->get_block_number() == i)
				{
					block_is_find = true;
					break;
				}
			}
			if(!block_is_find) msreg->AddMessage_("Найден потерянный объект", msInfo, "Номер блока", tohex(i));
		}
	}
	msreg->AddMessage("Поиск потерянных объектов завершен", msSuccesfull);
}
#endif //#ifdef PublicRelease

//---------------------------------------------------------------------------
bool T_1CD::test_stream_format()
{
	uint32_t i;
	bool result;
	bool res;

	// CONFIGSAVE
	if(!table_config)
	{
		error("Ошибка тестирования. В базе нет таблицы CONFIG");
		return false;
	}

	if(table_config->get_numfields() < 6)
	{
		error("Ошибка тестирования. В таблице CONFIG меньше 6 полей",
			"Кол-во полей", table_config->get_numfields());
		return false;
	}

	if(table_config->get_numfields() > 7)
	{
		error("Ошибка тестирования. В таблице CONFIG больше 7 полей",
			"Кол-во полей", table_config->get_numfields());
		return false;
	}

	if(table_config->getfield(0)->getname().CompareIC("FILENAME"))
	{
		error("Ошибка тестирования. Первое поле таблицы CONFIG не FILENAME",
			"Поле", table_config->getfield(0)->getname());
		return false;
	}

	if(table_config->getfield(1)->getname().CompareIC("CREATION"))
	{
		error("Ошибка тестирования. Второе поле таблицы CONFIG не CREATION",
			"Поле", table_config->getfield(1)->getname());
		return false;
	}

	if(table_config->getfield(2)->getname().CompareIC("MODIFIED"))
	{
		error("Ошибка тестирования. Третье поле таблицы CONFIG не MODIFIED",
			"Поле", table_config->getfield(2)->getname());
		return false;
	}

	if(table_config->getfield(3)->getname().CompareIC("ATTRIBUTES"))
	{
		error("Ошибка тестирования. Четвертое поле таблицы CONFIG не ATTRIBUTES",
			"Поле", table_config->getfield(3)->getname());
		return false;
	}

	if(table_config->getfield(4)->getname().CompareIC("DATASIZE"))
	{
		error("Ошибка тестирования. Пятое поле таблицы CONFIG не DATASIZE",
			"Поле", table_config->getfield(4)->getname());
		return false;
	}

	if(table_config->getfield(5)->getname().CompareIC("BINARYDATA"))
	{
		error("Ошибка тестирования. Шестое поле таблицы CONFIG не BINARYDATA",
			"Поле", table_config->getfield(5)->getname());
		return false;
	}

	if(table_config->get_numfields() > 6)
	{
		if(table_config->getfield(6)->getname().CompareIC("PARTNO"))
		{
			error("Ошибка тестирования. Седьмое поле таблицы CONFIG не PARTNO",
				"Поле", table_config->getfield(6)->getname());
			return false;
		}
	}

	// CONFIGSAVE
	if(!table_configsave)
	{
		error("Ошибка тестирования. В базе нет таблицы CONFIGSAVE");
		return false;
	}

	if(table_configsave->get_numfields() < 6)
	{
		error("Ошибка тестирования. В таблице CONFIGSAVE меньше 6 полей",
			"Кол-во полей", table_configsave->get_numfields());
		return false;
	}

	if(table_configsave->get_numfields() > 7)
	{
		error("Ошибка тестирования. В таблице CONFIGSAVE больше 7 полей",
			"Кол-во полей", table_configsave->get_numfields());
		return false;
	}

	if(table_configsave->getfield(0)->getname().CompareIC("FILENAME"))
	{
		error("Ошибка тестирования. Первое поле таблицы CONFIGSAVE не FILENAME",
			"Поле", table_configsave->getfield(0)->getname());
		return false;
	}

	if(table_configsave->getfield(1)->getname().CompareIC("CREATION"))
	{
		error("Ошибка тестирования. Второе поле таблицы CONFIGSAVE не CREATION",
			"Поле", table_configsave->getfield(1)->getname());
		return false;
	}

	if(table_configsave->getfield(2)->getname().CompareIC("MODIFIED"))
	{
		error("Ошибка тестирования. Третье поле таблицы CONFIGSAVE не MODIFIED",
			"Поле", table_configsave->getfield(2)->getname());
		return false;
	}

	if(table_configsave->getfield(3)->getname().CompareIC("ATTRIBUTES"))
	{
		error("Ошибка тестирования. Четвертое поле таблицы CONFIGSAVE не ATTRIBUTES",
			"Поле", table_configsave->getfield(3)->getname());
		return false;
	}

	if(table_configsave->getfield(4)->getname().CompareIC("DATASIZE"))
	{
		error("Ошибка тестирования. Пятое поле таблицы CONFIGSAVE не DATASIZE",
			"Поле", table_configsave->getfield(4)->getname());
		return false;
	}

	if(table_configsave->getfield(5)->getname().CompareIC("BINARYDATA"))
	{
		error("Ошибка тестирования. Шестое поле таблицы CONFIGSAVE не BINARYDATA",
			"Поле", table_configsave->getfield(5)->getname());
		return false;
	}

	if(table_configsave->get_numfields() > 6)
	{
		if(table_configsave->getfield(6)->getname().CompareIC("PARTNO"))
		{
			error("Ошибка тестирования. Седьмое поле таблицы CONFIGSAVE не PARTNO",
				"Поле", table_configsave->getfield(6)->getname());
			return false;
		}
	}

	// PARAMS
	if(!table_params)
	{
		error("Ошибка тестирования. В базе нет таблицы PARAMS");
		return false;
	}

	if(table_params->get_numfields() < 6)
	{
		error("Ошибка тестирования. В таблице PARAMS меньше 6 полей",
			"Кол-во полей", table_params->get_numfields());
		return false;
	}

	if(table_params->get_numfields() > 7)
	{
		error("Ошибка тестирования. В таблице PARAMS больше 7 полей",
			"Кол-во полей", table_params->get_numfields());
		return false;
	}

	if(table_params->getfield(0)->getname().CompareIC("FILENAME"))
	{
		error("Ошибка тестирования. Первое поле таблицы PARAMS не FILENAME",
			"Поле", table_params->getfield(0)->getname());
		return false;
	}

	if(table_params->getfield(1)->getname().CompareIC("CREATION"))
	{
		error("Ошибка тестирования. Второе поле таблицы PARAMS не CREATION",
			"Поле", table_params->getfield(1)->getname());
		return false;
	}

	if(table_params->getfield(2)->getname().CompareIC("MODIFIED"))
	{
		error("Ошибка тестирования. Третье поле таблицы PARAMS не MODIFIED",
			"Поле", table_params->getfield(2)->getname());
		return false;
	}

	if(table_params->getfield(3)->getname().CompareIC("ATTRIBUTES"))
	{
		error("Ошибка тестирования. Четвертое поле таблицы PARAMS не ATTRIBUTES",
			"Поле", table_params->getfield(3)->getname());
		return false;
	}

	if(table_params->getfield(4)->getname().CompareIC("DATASIZE"))
	{
		error("Ошибка тестирования. Пятое поле таблицы PARAMS не DATASIZE",
			"Поле", table_params->getfield(4)->getname());
		return false;
	}

	if(table_params->getfield(5)->getname().CompareIC("BINARYDATA"))
	{
		error("Ошибка тестирования. Шестое поле таблицы PARAMS не BINARYDATA",
			"Поле", table_params->getfield(5)->getname());
		return false;
	}

	if(table_params->get_numfields() > 6)
	{
		if(table_params->getfield(6)->getname().CompareIC("PARTNO"))
		{
			error("Ошибка тестирования. Седьмое поле таблицы PARAMS не PARTNO",
				"Поле", table_params->getfield(6)->getname());
			return false;
		}
	}

	// FILES
	if(!table_files)
	{
		error("Ошибка тестирования. В базе нет таблицы FILES");
		return false;
	}

	if(table_files->get_numfields() < 6)
	{
		error("Ошибка тестирования. В таблице FILES меньше 6 полей",
			"Кол-во полей", table_files->get_numfields());
		return false;
	}

	if(table_files->get_numfields() > 7)
	{
		error("Ошибка тестирования. В таблице FILES больше 7 полей",
			"Кол-во полей", table_files->get_numfields());
		return false;
	}

	if(table_files->getfield(0)->getname().CompareIC("FILENAME"))
	{
		error("Ошибка тестирования. Первое поле таблицы FILES не FILENAME",
			"Поле", table_files->getfield(0)->getname());
		return false;
	}

	if(table_files->getfield(1)->getname().CompareIC("CREATION"))
	{
		error("Ошибка тестирования. Второе поле таблицы FILES не CREATION",
			"Поле", table_files->getfield(1)->getname());
		return false;
	}

	if(table_files->getfield(2)->getname().CompareIC("MODIFIED"))
	{
		error("Ошибка тестирования. Третье поле таблицы FILES не MODIFIED",
			"Поле", table_files->getfield(2)->getname());
		return false;
	}

	if(table_files->getfield(3)->getname().CompareIC("ATTRIBUTES"))
	{
		error("Ошибка тестирования. Четвертое поле таблицы FILES не ATTRIBUTES",
			"Поле", table_files->getfield(3)->getname());
		return false;
	}

	if(table_files->getfield(4)->getname().CompareIC("DATASIZE"))
	{
		error("Ошибка тестирования. Пятое поле таблицы FILES не DATASIZE",
			"Поле", table_files->getfield(4)->getname());
		return false;
	}

	if(table_files->getfield(5)->getname().CompareIC("BINARYDATA"))
	{
		error("Ошибка тестирования. Шестое поле таблицы FILES не BINARYDATA",
			"Поле", table_files->getfield(5)->getname());
		return false;
	}

	if(table_files->get_numfields() > 6)
	{
		if(table_files->getfield(6)->getname().CompareIC("PARTNO"))
		{
			error("Ошибка тестирования. Седьмое поле таблицы FILES не PARTNO",
				"Поле", table_files->getfield(6)->getname());
			return false;
		}
	}

	// DBSCHEMA
	if(!table_dbschema)
	{
		error("Ошибка тестирования. В базе нет таблицы DBSCHEMA");
		return false;
	}

	if(table_dbschema->get_numfields() != 1)
	{
		error("Ошибка тестирования. В таблице DBSCHEMA не 1 поле",
			"Кол-во полей", table_dbschema->get_numfields());
		return false;
	}

	if(table_dbschema->getfield(0)->getname().CompareIC("SERIALIZEDDATA"))
	{
		error("Ошибка тестирования. Первое поле таблицы DBSCHEMA не SERIALIZEDDATA",
			"Поле", table_dbschema->getfield(0)->getname());
		return false;
	}

	//================
	result = true;

	for(i = 0; i < table_config->get_phys_numrecords(); i++)
	{
		res = recursive_test_stream_format(table_config, i);
		result = result && res;
	}

	for(i = 0; i < table_configsave->get_phys_numrecords(); i++)
	{
		res = recursive_test_stream_format(table_configsave, i);
		result = result && res;
	}

	for(i = 0; i < table_params->get_phys_numrecords(); i++)
	{
		res = recursive_test_stream_format(table_params, i);
		result = result && res;
	}

	if(table_dbschema->get_phys_numrecords() < 2)
	{
		error("Ошибка тестирования. В таблице DBSCHEMA нет записей");
		result = false;
	}
	for(i = 0; i < table_dbschema->get_phys_numrecords(); i++)
	{
		res = recursive_test_stream_format2(table_dbschema, i);
		result = result && res;
	}

	msreg->Status("");
	return result;
}

//---------------------------------------------------------------------------
bool T_1CD::recursive_test_stream_format(Table* t, uint32_t nrec)
{
	int32_t j;
	char* rec;
	char* orec;
	Field* f_name;
	Field* f_data_size;
	Field* f_binary_data;
	String path;
	String slen;
	TStream* str;
	bool result;
	bool res;
	wchar_t tempfile[MAX_PATH];

	rec = new char[t->get_recordlen()];
	t->getrecord(nrec, rec);
	if(*rec)
	{
		delete[] rec;
		return true;
	}

	f_name = t->getfield(0);
	f_data_size = t->getfield(4);
	f_binary_data = t->getfield(5);

	path = t->getname() + "/" + f_name->get_presentation(rec);

	orec = rec + f_binary_data->getoffset();
	if(*(uint32_t*)(orec + 4) > 10 * 1024 * 1024) str = new TTempStream;
	else str = new TMemoryStream();
	t->readBlob(str, *(uint32_t*)orec, *(uint32_t*)(orec + 4));

	result = true;
	slen = f_data_size->get_presentation(rec, true);
	try
	{
		j = slen.ToInt();
	}
	catch(...)
	{
		if(msreg) msreg->AddMessage_("Ошибка чтения длины файла", msWarning,
			"Путь", path,
			"Длина файла", slen);
		result = false;
	}
	if(result) if((int64_t)j != str->GetSize())
	{
		if(msreg) msreg->AddMessage_("Фактическая длина файла отличается от указанной в таблице", msWarning,
			"Путь", path,
			"Фактическая длина файла", str->GetSize(),
			"Указанная длина файла", slen);
		result = false;
	}

	res = recursive_test_stream_format(str, path, f_name->get_presentation(rec).GetLength() > 72); // вторично упакованы могут быть только конфигурации поставщика (файлы с длиной имени более 72 символов)
	result = result && res;

	delete[] rec;
	delete str;

	return result;

}

//---------------------------------------------------------------------------
bool T_1CD::recursive_test_stream_format2(Table* t, uint32_t nrec)
{
	int32_t j;
	char* rec;
	char* orec;
	Field* f_sd;
	String path;
	TMemoryStream* str;
	bool result;

	rec = new char[t->get_recordlen()];
	t->getrecord(nrec, rec);
	if(*rec)
	{
		delete[] rec;
		return true;
	}

	f_sd = t->getfield(0);

	path = t->getname();

	orec = rec + f_sd->getoffset();
	str = new TMemoryStream();
	t->readBlob(str, *(uint32_t*)orec, *(uint32_t*)(orec + 4));

	result = recursive_test_stream_format(str, path);

	delete[] rec;
	delete str;

	return result;

}

//---------------------------------------------------------------------------
bool T_1CD::recursive_test_stream_format(TStream* str, String path, bool maybezipped2)
{
	bool zipped1;
	bool zipped2;
	char _buf[12];
	TStream* _s;
	TStream* _s2;
	TBytesStream* _sb;
	bool result;
	TEncoding *enc;
	TBytes bytes1;
	TBytes bytes2;
	v8catalog* cat;
	int32_t offset;
	String sf;
	wchar_t first_symbol;
	int32_t i, j;
	bool usetempfile;

	msreg->Status(path);

	if(path == "PARAMS/users.usr") return true;

	if(str->GetSize() == 0) return true;
	usetempfile = str->GetSize() > 10 * 1024 * 1024;

	if(usetempfile) _s = new TTempStream;
	else _s = new TMemoryStream();
	zipped1 = false;
	zipped2 = false;

	if(usetempfile) _s2 = new TTempStream;
	else _s2 = new TMemoryStream();

	_s2->CopyFrom(str, 0);
	try
	{
		_s2->Seek(0, soFromBeginning);
		_s->SetSize(0);
		ZInflateStream(_s2, _s);
		zipped1 = true;
	}
	catch (...)
	{
		_s->SetSize(0);
		_s->CopyFrom(str, 0);
	}

	if(zipped1 && maybezipped2)
	{
		_s2->SetSize(0);
		_s2->CopyFrom(_s, 0);
		try
		{
			_s2->Seek(0, soFromBeginning);
			_s->SetSize(0);
			ZInflateStream(_s2, _s);
			zipped2 = true;
		}
		catch (...)
		{
			_s->SetSize(0);
			_s->CopyFrom(_s2, 0);
		}
	}


	try
	{
		cat = new v8catalog(_s, zipped2, true);
	}
	catch (...)
	{
		error("Ошибка тестирования. Ошибка чтения формата.",
			"Путь", path);
		cat = NULL;

	}
	if(!cat || !cat->GetFirst())
	{

		if(_s->GetSize() >= 16)
		{
			_s->Seek(0, soFromBeginning);
			_s->ReadBuffer(_buf, 8);
			if(memcmp(_buf, SIG_MOXCEL, 7) == 0)
			{
				_s->Seek(13, soFromBeginning);
				_s2->SetSize(0);
				_s2->CopyFrom(_s, _s->GetSize() - 13);
				_s->SetSize(0);
				_s->CopyFrom(_s2, 0);
			}
			else if(memcmp(_buf, SIG_SKD, 8) == 0)
			{
				_s->Seek(24, soFromBeginning);
				_s2->SetSize(0);
				_s2->CopyFrom(_s, _s->GetSize() - 24);
				_s->SetSize(0);
				_s->CopyFrom(_s2, 0);
			}
		}

		_sb = new TBytesStream(bytes1);
		_s->Seek(0, soFromBeginning);
		_sb->CopyFrom(_s, 0);

		enc = NULL;
		offset = TEncoding::GetBufferEncoding(_sb->GetBytes(), enc);
		if(offset == 0)
		{
			error("Ошибка тестирования. Ошибка определения кодировки файла",
				"Путь", path);
			result = false;
		}
		else
		{
			if(_sb->GetSize()-offset > 0)
			{
				bytes2 = TEncoding::Convert(enc, TEncoding::Unicode, _sb->GetBytes(), offset, _sb->GetSize()-offset);
				if(bytes2.GetLength() == 0)
				{
					error("Ошибка тестирования. Ошибка конвертации",
						"Путь", path);
					result = false;
				}
				else
				{
					sf = String((WCHART*)&bytes2[0], bytes2.GetLength() / 2);
					for(i = 1; i <= sf.GetLength(); i++)
					{
						first_symbol = sf[i];
						if(first_symbol != L'\r' && first_symbol != L'\n' && first_symbol != L'\t' && first_symbol != L' ') break;
					}
					if(first_symbol == L'{' && sf.SubString(i, 15).CompareIC("{ХАРАКТЕРИСТИКИ"))
					{
						tree* rt = parse_1Ctext(sf, path);
						if(rt)
						{
							result = true;
							delete rt;
						}
						else result = false;
					}
					else result = true;
				}
			}
			else result = true;
		}

		delete _sb;

	}
	else result = recursive_test_stream_format(cat, path);

	delete cat;

	delete _s;
	delete _s2;

	return result;

}

//---------------------------------------------------------------------------
bool T_1CD::recursive_test_stream_format(v8catalog* cat, String path)
{
	v8catalog* c;
	bool result;
	bool res;
	v8file* v8f;
	v8file* v8fp;
	String fname;

	result = true;
	v8f = cat->GetFirst();
	while(v8f)
	{
		result = false;
		try
		{
			c = v8f->GetCatalog();
			result = true;
		}
		catch(...)
		{
			error("Ошибка тестирования. Ошибка чтения формата.",
				"Путь", path);
			c = NULL;
		}
		if(result)
		{
			if(c) result = recursive_test_stream_format(c, path + "/" + v8f->GetFileName());
			else
			{
				fname = v8f->GetFileName();
				if(fname != "module" && fname != "text")
				{
					result = recursive_test_stream_format(v8f->get_stream(), path + "/" + v8f->GetFileName());
				}
			}
		}
		v8fp = v8f;
		v8f = v8f->GetNext();
		v8fp->Close();
	}
	return result;
}

//---------------------------------------------------------------------------
#ifndef PublicRelease
bool T_1CD::create_table(String path)
{
	TFileStream* f;
	bool fopen;
	String dir;
	String str;
	char* buf;
	uint32_t i;
	int32_t j;
	export_import_table_root* root;
	v8ob* ob;
	v8object* descr_table;
	v8object* file_data;
	v8object* file_blob;
	v8object* file_index;
	tree* t;

	if(!DirectoryExists(path))
	{
		if(msreg) msreg->AddMessage_("Директория импорта таблицы не найдена", msWarning,
			"Директория", path);
		return false;
	}
	dir = path + "\\";

	try
	{
		f = new TFileStream(dir + "root", fmOpenRead);
	}
	catch(...)
	{
		if(msreg) msreg->AddMessage_("Ошибка открытия файла импорта таблицы root", msWarning,
			"Файл", dir + "root");
		return false;
	}
	root = new export_import_table_root;
	f->Read(root, sizeof(export_import_table_root));
	delete f;


	try
	{
		f = new TFileStream(dir + "descr", fmOpenRead);
	}
	catch(...)
	{
		if(msreg) msreg->AddMessage_("Ошибка открытия файла импорта таблицы descr", msWarning,
			"Файл", dir + "descr");
		return false;
	}

	i = f->GetSize();
	buf = new char[i + 2];
	f->Read(buf, i);
	buf[i] = 0;
	buf[i + 1] = 0;
	str = String((WCHART*)buf);
	delete[] buf;
	delete f;

	t = parse_1Ctext(str, dir + "descr");
	str = (*t)[0][0].get_value();

	for(j = 0; j < num_tables; j++) if(tables[j]->getname().CompareIC(str) == 0)
	{
		delete_table(tables[j]);
	}

	file_data = NULL;
	file_blob = NULL;
	file_index = NULL;

	if(root->has_data)
	{
		fopen = false;
		try
		{
			f = new TFileStream(dir + "data", fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			if(msreg) msreg->AddMessage_("Ошибка открытия файла импорта таблицы data", msWarning,
				"Файл", dir + "data");
		}
		if(fopen)
		{
			file_data = new v8object(this);
			file_data->setdata(f);
			ob = (v8ob*)getblock_for_write(file_data->get_block_number(), true);
			ob->version.version_1 = root->data_version_1;
			ob->version.version_2 = root->data_version_2;
			delete f;
		}
	}

	if(root->has_blob)
	{
		fopen = false;
		try
		{
			f = new TFileStream(dir + "blob", fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			if(msreg) msreg->AddMessage_("Ошибка открытия файла импорта таблицы blob", msWarning,
				"Файл", dir + "blob");
		}
		if(fopen)
		{
			file_blob = new v8object(this);
			file_blob->setdata(f);
			ob = (v8ob*)getblock_for_write(file_blob->get_block_number(), true);
			ob->version.version_1 = root->blob_version_1;
			ob->version.version_2 = root->blob_version_2;
			delete f;
		}
	}

	if(root->has_index)
	{
		fopen = false;
		try
		{
			f = new TFileStream(dir + "index", fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			if(msreg) msreg->AddMessage_("Ошибка открытия файла импорта таблицы index", msWarning,
				"Файл", dir + "index");
		}
		if(fopen)
		{
			file_index = new v8object(this);
			file_index->setdata(f);
			ob = (v8ob*)getblock_for_write(file_index->get_block_number(), true);
			ob->version.version_1 = root->index_version_1;
			ob->version.version_2 = root->index_version_2;
			delete f;
		}
	}

	if(root->has_descr)
	{
		fopen = false;
		try
		{
			f = new TFileStream(dir + "descr", fmOpenRead);
			fopen = true;
		}
		catch(...)
		{
			if(msreg) msreg->AddMessage_("Ошибка открытия файла импорта таблицы descr", msWarning,
				"Файл", dir + "descr");
		}
		if(fopen)
		{
			descr_table = new v8object(this);
			ob = (v8ob*)getblock_for_write(descr_table->get_block_number(), true);
			ob->version.version_1 = root->descr_version_1;
			ob->version.version_2 = root->descr_version_2;

			i = f->GetSize();
			buf = new char[i + 2];
			f->Read(buf, i);
			buf[i] =0;
			buf[i + 1] =0;
			str = String((WCHART*)buf);
			delete[] buf;
			delete f;

			i = str.Pos("{\"Files\",");
			if(i == 0)
			{
				if(msreg) msreg->AddMessage_("Ошибка поиска раздела Files в файле импорта таблицы descr", msWarning,
					"Файл", dir + "descr");
				delete root;
				return false;
			}
			str.SetLength(i - 1);
			str += "{\"Files\",";
			str += file_data ? String(file_data->get_block_number()) : String("0");
			str += ",";
			str += file_blob ? String(file_blob->get_block_number()) : String("0");
			str += ",";
			str += file_index ? String(file_index->get_block_number()) : String("0");
			str += "}\n}";
			descr_table->setdata(str.c_str(), str.GetLength() * 2);

			i = root_object->getlen();
			buf = new char[i + 4];
			root_object->getdata(buf, 0, i);

			if(version == ver8_0_3_0 || version == ver8_0_5_0)
			{
				root_80* root80 = (root_80*)buf;
				root80->blocks[root80->numblocks] = descr_table->get_block_number();
				root80->numblocks++;
				root_object->setdata(buf, i + 4);
			}
			else
			{
				root_81* root81 = (root_81*)buf;
				root81->blocks[root81->numblocks] = descr_table->get_block_number();
				root81->numblocks++;
				root_object->setdata(buf, i + 4);
			}


		}


	}

	flush();

	if(msreg) msreg->AddMessage_("Таблица создана и импортирована", msSuccesfull,
		"Путь", dir);

	delete root;
	return true;
}
#endif //#ifdef PublicRelease

//---------------------------------------------------------------------------
void T_1CD::set_readonly(bool ro)
{
	readonly = ro;
}

//---------------------------------------------------------------------------
bool T_1CD::test_list_of_tables()
{
	char* rec;
	char* orec;
	Field* f_name;
	Field* f_data_size;
	Field* f_binary_data;
	bool hasDBNames;
	bool result;
	bool is_slave;
	String slen;
	String sf;
	TMemoryStream* str;
	TBytesStream* _sb;
	TEncoding *enc;
	TBytes bytes1;
	TBytes bytes2;
	wchar_t first_symbol;
	int32_t i, j, l, l2;
	uint32_t k;
	int32_t offset;
	tree* t;
	tree* firstt;

	String _guid;
	String _name;
	String _num;
	String _tabname;

	if(!table_params)
	{
		error("Ошибка тестирования. В базе нет таблицы PARAMS");
		return false;
	}

	if(table_params->get_numfields() < 6)
	{
		error("Ошибка тестирования. В таблице PARAMS меньше 6 полей",
			"Кол-во полей", table_params->get_numfields());
		return false;
	}

	if(table_params->get_numfields() > 7)
	{
		error("Ошибка тестирования. В таблице PARAMS больше 7 полей",
			"Кол-во полей", table_params->get_numfields());
		return false;
	}

	if(table_params->getfield(0)->getname().CompareIC("FILENAME"))
	{
		error("Ошибка тестирования. Первое поле таблицы PARAMS не FILENAME",
			"Поле", table_params->getfield(0)->getname());
		return false;
	}

	if(table_params->getfield(1)->getname().CompareIC("CREATION"))
	{
		error("Ошибка тестирования. Второе поле таблицы PARAMS не CREATION",
			"Поле", table_params->getfield(1)->getname());
		return false;
	}

	if(table_params->getfield(2)->getname().CompareIC("MODIFIED"))
	{
		error("Ошибка тестирования. Третье поле таблицы PARAMS не MODIFIED",
			"Поле", table_params->getfield(2)->getname());
		return false;
	}

	if(table_params->getfield(3)->getname().CompareIC("ATTRIBUTES"))
	{
		error("Ошибка тестирования. Четвертое поле таблицы PARAMS не ATTRIBUTES",
			"Поле", table_params->getfield(3)->getname());
		return false;
	}

	if(table_params->getfield(4)->getname().CompareIC("DATASIZE"))
	{
		error("Ошибка тестирования. Пятое поле таблицы PARAMS не DATASIZE",
			"Поле", table_params->getfield(4)->getname());
		return false;
	}

	if(table_params->getfield(5)->getname().CompareIC("BINARYDATA"))
	{
		error("Ошибка тестирования. Шестое поле таблицы PARAMS не BINARYDATA",
			"Поле", table_params->getfield(5)->getname());
		return false;
	}

	if(table_params->get_numfields() > 6)
	{
		if(table_params->getfield(6)->getname().CompareIC("PARTNO"))
		{
			error("Ошибка тестирования. Седьмое поле таблицы PARAMS не PARTNO",
				"Поле", table_params->getfield(6)->getname());
			return false;
		}
	}

	result = true;

	f_name = table_params->getfield(0);
	f_data_size = table_params->getfield(4);
	f_binary_data = table_params->getfield(5);
	rec = new char[table_params->get_recordlen()];

	hasDBNames = false;
	for(k = 0; k < table_params->get_phys_numrecords(); k++)
	{

		table_params->getrecord(k, rec);
		if(*rec) continue;

		if(f_name->get_presentation(rec).CompareIC("DBNames") != 0) continue;

		hasDBNames = true;

		orec = rec + f_binary_data->getoffset();
		str = new TMemoryStream();
		table_params->readBlob(str, *(uint32_t*)orec, *(uint32_t*)(orec + 4));

		slen = f_data_size->get_presentation(rec, true);
		try
		{
			j = slen.ToInt();
		}
		catch(...)
		{
			if(msreg) msreg->AddMessage_("Ошибка чтения длины файла", msWarning,
				"Путь", "PARAMS/DBNames",
				"Длина файла", slen);
			result = false;
			break;
		}
		if((int64_t)j != str->GetSize())
		{
			if(msreg) msreg->AddMessage_("Фактическая длина файла отличается от указанной в таблице", msWarning,
				"Путь", "PARAMS/DBNames",
				"Фактическая длина файла", str->GetSize(),
				"Указанная длина файла", slen);
			result = false;
			break;
		}

		str->Seek(0, soFromBeginning);
		_sb = new TBytesStream(bytes1);

		if(version == ver8_0_3_0 || version == ver8_0_5_0)
		{
			_sb->CopyFrom(str, 0);
		}
		else
		{
			try
			{
				ZInflateStream(str, _sb);
			}
			catch (...)
			{
				if(msreg) msreg->AddMessage("Ошибка распаковки данных файла PARAMS/DBNames", msError);
				result = false;
				break;
			}
		}
		delete str;

		enc = NULL;
		offset = TEncoding::GetBufferEncoding(_sb->GetBytes(), enc);
		if(offset == 0)
		{
			error("Ошибка тестирования. Ошибка определения кодировки файла PARAMS/DBNames");
			result = false;
		}
		else
		{
			if(_sb->GetSize()-offset > 0)
			{
				bytes2 = TEncoding::Convert(enc, TEncoding::Unicode, _sb->GetBytes(), offset, _sb->GetSize()-offset);
				if(bytes2.GetLength() == 0)
				{
					error("Ошибка тестирования. Ошибка конвертации файла PARAMS/DBNames");
					result = false;
				}
				else
				{
					sf = String((WCHART*)&bytes2[0], bytes2.GetLength() / 2);
					for(i = 1; i <= sf.GetLength(); i++)
					{
						first_symbol = sf[i];
						if(first_symbol != L'\r' && first_symbol != L'\n' && first_symbol != L'\t' && first_symbol != L' ') break;
					}
					if(first_symbol == L'{')
					{
						tree* rt = parse_1Ctext(sf, "PARAMS/DBNames");
						if(rt)
						{
							firstt = &((*rt)[0][1][1]);

							for(t = firstt; t; t = t->get_next())
							{
								is_slave = false;
								_name = t->get_subnode(1)->get_value();
								if(_name.CompareIC("Fld") == 0) continue;
								if(_name.CompareIC("LineNo") == 0) continue;
								if(_name.CompareIC("Turnover") == 0) continue;
								if(_name.CompareIC("TurnoverDt") == 0) continue;
								if(_name.CompareIC("TurnoverCt") == 0) continue;
								if(_name.CompareIC("ByField") == 0) continue;
								if(_name.CompareIC("ByOwnerField") == 0) continue;
								if(_name.CompareIC("ByParentField") == 0) continue;
								if(_name.CompareIC("ByProperty") == 0) continue;
								if(_name.CompareIC("ByPropRecorder") == 0) continue;
								if(_name.CompareIC("ByResource") == 0) continue;
								if(_name.CompareIC("ByDim") == 0) continue;
								if(_name.CompareIC("ByDims") == 0) continue;
								if(_name.CompareIC("ByDimension") == 0) continue;
								if(_name.CompareIC("ByDimensions") == 0) continue;
								if(_name.CompareIC("ByDimRecorder") == 0) continue;
								if(_name.CompareIC("VT") == 0) is_slave = true;
								if(_name.CompareIC("ExtDim") == 0) is_slave = true;

								_guid = t->get_subnode(0)->get_value();
								_num = t->get_subnode(2)->get_value();

								if(_guid == "00000000-0000-0000-0000-000000000000") continue;

								_tabname = "_";
								_tabname += _name;
								_tabname += _num;
								l = _tabname.GetLength();

								bool table_found = false;
								for(i = 0; i < get_numtables(); i++)
								{
									if(is_slave)
									{
										sf = gettable(i)->getname();
										l2 = sf.GetLength();
										if(l2 > l) if(sf.SubString(l2 - l + 1, l).CompareIC(_tabname) == 0)
										{
											table_found = true;
											break;
										}
									}
									else if(gettable(i)->getname().CompareIC(_tabname) == 0)
									{
										table_found = true;
										break;
									}
								}


								if(!table_found)
								{
									if(msreg) msreg->AddMessage_("Отсутствует таблица", msWarning,
										"Имя таблицы", _tabname);
									result = false;
								}
							}

							delete rt;
						}
						else result = false;
					}
					else
					{
						error("Ошибка тестирования. Ошибка разбора файла PARAMS/DBNames. Первый символ не \"{\".");
						result = false;
					}
				}
			}
			else
			{
				error("Ошибка тестирования. Ошибка определения кодировки файла PARAMS/DBNames");
				result = false;
			}
		}

		delete _sb;

		break;

	}


	if(!hasDBNames)
	{
		error("Ошибка тестирования. В таблице PARAMS не найдена запись DBNames.");
		result = false;
	}

	delete[] rec;

	return result;
}

#ifndef PublicRelease
//---------------------------------------------------------------------------
bool T_1CD::replaceTREF(String mapfile)
{
	DynamicArray<int32_t> map; // динамический массив соответствия номеров
	int32_t i,j,m;
	int32_t k, l;
	uint32_t ii, jj, kk;
	char* rec;
	String str;
	TStringList* list;
	Table* t;
	Field* f;
	bool editsave;

	list = new TStringList;
	list->LoadFromFile(mapfile);

	m = 0;
	for(k = 0; k < list->Count(); k++)
	{
		str = (*list)[k];
		l = str.Pos("\t");
		if(!l) continue;
		j = str.SubString(l + 1, str.GetLength() - l).ToInt();
		if(m < j) m = j;
	}

	map.set_length(m + 1);

	for(k = 0; k < list->Count(); k++)
	{
		str = (*list)[k];
		l = str.Pos("\t");
		if(!l) continue;
		i = str.SubString(1, l - 1).ToInt();
		j = str.SubString(l + 1, str.GetLength() - l).ToInt();
		map[j] = i;
	}

	delete list;

	for(i = 0; i < num_tables; i++)
	{
		t = gettable(i);
		for(j = 0; j < t->get_numfields(); j ++)
		{
			f = t->getfield(j);
			str = f->getname();
			if(str.GetLength() > 3)
			if(str.SubString(str.GetLength() - 3, 4).CompareIC("TREF") == 0)
			if(f->gettype() == tf_binary)
			if(f->getlength() == 4)
			{
				if(msreg) msreg->Status(t->getname() + " : " + f->getname());
				k = f->getoffset();
				if(f->getnull_exists()) k++;
				rec = new char[t->get_recordlen()];
				editsave = t->edit;
				t->edit = true;
				for(kk = 0; kk < t->get_phys_numrecords(); kk++)
				{
					t->getrecord(kk, rec);
					if(*rec) continue;
					ii = reverse_byte_order(*((uint32_t*)(rec + k)));
					if(ii == 0) continue;
					ii = map[ii];
					*((int32_t*)(rec + k)) = reverse_byte_order(ii);
					t->write_data_record(kk, rec);
				}
				t->edit = editsave;

				delete[] rec;
			}
		}
	}
	if(msreg) msreg->Status("");

	flush();

	return true;
}

//---------------------------------------------------------------------------
bool T_1CD::delete_table(Table* tab)
{
	int32_t i;
	uint32_t j;
	uint32_t bl;
	char* buf;
	bool res = num_tables > 0;

	bl = tab->descr_table->block;

	res = res && delete_object(tab->descr_table);
	if(res) res = res && delete_object(tab->file_data);
	if(res) res = res && delete_object(tab->file_blob);
	if(res) res = res && delete_object(tab->file_index);

	if(res)
	{
		tab->descr_table = NULL;
		tab->file_data = NULL;
		tab->file_blob = NULL;
		tab->file_index = NULL;


		for(i = 0; i < num_tables; i++) if(tables[i] == tab) break;
		num_tables--;
		for(; i < num_tables; i++) tables[i] = tables[i + 1];
		delete tab;

		j = root_object->getlen();
		buf = new char[j];
		root_object->getdata(buf, 0, j);

		if(version == ver8_0_3_0 || version == ver8_0_5_0)
		{
			root_80* root80 = (root_80*)buf;
			for(i = 0; i < root80->numblocks; i++) if(root80->blocks[i] == bl) break;
			root80->numblocks--;
			for(; i < root80->numblocks; i++) root80->blocks[i] = root80->blocks[i + 1];
		}
		else
		{
			root_81* root81 = (root_81*)buf;
			for(i = 0; i < root81->numblocks; i++) if(root81->blocks[i] == bl) break;
			root81->numblocks--;
			for(; i < root81->numblocks; i++) root81->blocks[i] = root81->blocks[i + 1];
		}
		root_object->setdata(buf, j - 4);
		delete[] buf;

	}

	return res;
}

//---------------------------------------------------------------------------
bool T_1CD::delete_object(v8object* ob)
{
	objtab* b;
	uint32_t i;
	int32_t j;

	if(!ob) return true;

	if(ob->block == 1)
	{
		msreg->AddMessage_("Попытка удаления объекта таблицы свободных блоков", msWarning,
			"Номер блока объекта", ob->block);
			return false;
	}

	if(ob->block == 2)
	{
		msreg->AddMessage_("Попытка удаления корневого объекта", msWarning,
			"Номер блока объекта", ob->block);
			return false;
	}

	for(i = 0; i < ob->numblocks; i++)
	{
		b = (objtab*)getblock(ob->blocks[i]);
		for(j = 0; j < b->numblocks; j++) set_block_as_free(b->blocks[j]);
	}
	for(i = 0; i < ob->numblocks; i++) set_block_as_free(ob->blocks[i]);
	for(i = 0; i < ob->numblocks; i++) set_block_as_free(ob->block);

	delete ob;

	return true;

}

//---------------------------------------------------------------------------
void T_1CD::find_and_create_lost_tables()
{
	uint32_t i, k;
	int32_t j, numlosttables;
	char buf[8];
	v8object* v8obj;
	bool block_is_find;
	DynamicArray<uint32_t> losttables;
	char* b;

	numlosttables = 0;
	for(i = 1; i < length; i++)
	{
		getblock(buf, i, 8);
		if(memcmp(buf, SIG_OBJ, 8) == 0)
		{
			block_is_find = false;
			for(v8obj = v8object::get_first(); v8obj; v8obj = v8obj->get_next())
			{
				if(v8obj->get_block_number() == i)
				{
					block_is_find = true;
					break;
				}
			}
			if(!block_is_find)
			{
				v8obj = new v8object(this, i);
				if(v8obj->len > 3)
				{
					try
					{
						v8obj->getdata(buf, 0, 4);
						if(memcmp(buf, SIG_TABDESCR, 4) == 0)
						{
							if(losttables.get_length() <= numlosttables) losttables.set_length(losttables.get_length() + 1024);
							losttables[numlosttables++] = i;
						}
					}
					catch(...)
					{ //-V565

					}
				}
				delete v8obj;
			}
		}
	}

	if(numlosttables)
	{
		i = root_object->getlen();
		b = new char[i + numlosttables * 4];
		root_object->getdata(b, 0, i);

		if(version == ver8_0_3_0 || version == ver8_0_5_0)
		{
			root_80* root80 = (root_80*)b;
			for(j = 0, k = root80->numblocks; j < numlosttables; j++, k++) root80->blocks[k] = losttables[j];
			root80->numblocks += numlosttables;
		}
		else
		{
			root_81* root81 = (root_81*)b;
			for(j = 0, k = root81->numblocks; j < numlosttables; j++, k++) root81->blocks[k] = losttables[j];
			root81->numblocks += numlosttables;
		}
		root_object->setdata(b, i + numlosttables * 4);
		delete[] b;

	}

	msreg->AddMessage_("Поиск и восстановление потерянных таблиц завершены", msSuccesfull,
	"Количество восстановленных таблиц", numlosttables);

}

//---------------------------------------------------------------------------
void T_1CD::find_and_save_lost_objects()
{
	uint32_t i;
	char buf[8];
	v8object* v8obj;
	bool block_is_find;
	bool dir_created;
	String path;

	dir_created = false;
	path = filename.SubString(1, filename.LastDelimiter("\\"));
	path += "LostObjects\\";

	for(i = 1; i < length; i++)
	{
		getblock(buf, i, 8);
		if(memcmp(buf, SIG_OBJ, 8) == 0)
		{
			block_is_find = false;
			for(v8obj = v8object::get_first(); v8obj; v8obj = v8obj->get_next())
			{
				if(v8obj->get_block_number() == i)
				{
					block_is_find = true;
					break;
				}
			}
			if(!block_is_find)
			{
				if(!dir_created)
				{
					CreateDir(path);
					dir_created = true;
				}
				v8obj = new v8object(this, i);
				v8obj->savetofile(path + "block" + i);
				delete v8obj;
			}
		}
	}
	msreg->AddMessage("Поиск и сохранение потерянных объектов завершен", msSuccesfull);

}

#endif //#ifdef PublicRelease

//---------------------------------------------------------------------------
// Если не удалось получить версию, возвращается 0, иначе возвращается положительное число
int32_t T_1CD::get_ver_depot_config(int32_t ver) // Получение номера версии конфигурации (0 - последняя, -1 - предпоследняя и т.д.)
{
	char* rec;
	class index* ind;
	Field* fld;
	uint32_t i;
	int32_t v;
	String s;

	if(!is_open()) return 0;

	if(!is_depot)
	{
		error("База не является хранилищем конфигурации.");
		return 0;
	}

	if(ver > 0) return ver;

	// Определяем номер последней версии конфигурации
	if(!table_versions)
	{
		error("В базе хранилища отсутствует таблица VERSIONS.");
		return 0;
	}

	fld = get_field(table_versions, "VERNUM");
	if(!fld) return 0;

	ind = get_index(table_versions, "PK");
	if(!ind) return 0;

	i = ind->get_numrecords();
	if(i <= (uint32_t)(-ver))
	{
		msreg->AddMessage_("Запрошенной версии конфигурации не существует", msError,
			"Всего версий в хранилище", i,
			"Запрошенный номер версии", ver);
		return 0;
	}
	i = ind->get_numrec(i + ver - 1);

	rec = new char[table_versions->get_recordlen()];
	table_versions->getrecord(i, rec);
	s = fld->get_presentation(rec, true);
	delete[] rec;
	v = s.ToIntDef(0);
	if(!v)
	{
		if(msreg) msreg->AddMessage_("Не удалось получить реальный номер версии запрошенной конфигурации.", msError,
			"Запрошенный номер версии", ver);
		return 0;
	}

	return v;
}

Field* T_1CD::get_field(Table* tab, String fieldname)
{
	int32_t j;
	Field* fld;
	String s;

	for(j = 0; j < tab->num_fields; j++)
	{
		fld = tab->fields[j];
		if(fld->getname().CompareIC(fieldname) == 0) return fld;
	}
	if(msreg)
	{
		s ="В таблице ";
		s += tab->name;
		s += " не найдено поле ";
		s += fieldname;
		s += ".";
		error(s);
	}
	return NULL;
}

class index* T_1CD::get_index(Table* tab, String indexname)
{
	int32_t j;
	class index* ind;
	String s;

	for(j = 0; j < tab->num_indexes; j++)
	{
		ind = tab->indexes[j];
		if(ind->getname().CompareIC(indexname) == 0) return ind;
	}
	if(msreg)
	{
		s ="В таблице ";
		s += tab->name;
		s += " не найден индекс ";
		s += indexname;
		s += ".";
		error(s);
	}
	return NULL;
}

//---------------------------------------------------------------------------
// Сохранение конфигурации в файл из хранилища конфигураций
// ver - номер версии сохраняемой конфигурации
// ver > 0 - используется переданный номер версии
// ver <= 0 - номер версии от последней конфигурации. 0 - последняя конфигурация, -1 - предпоследняя и т.д., т.е. Номер версии определяется как номер последней + ver
bool T_1CD::save_depot_config(const String& _filename, int32_t ver)
{
	char* rec;
	char* frec;
	Field* fldd_depotver;
	Field* fldd_rootobjid;

	Field* fldv_vernum;
	Field* fldv_cversion;
	Field* fldv_snapshotcrc;
	Field* fldv_snapshotmaker;

	Field* fldh_objid;
	Field* fldh_vernum;
	Field* fldh_objverid;
	Field* fldh_removed;
	Field* fldh_datapacked;
	Field* fldh_objdata;
	Field* fldh_datahash;
	class index* indh;
	char* rech1;
	char* rech2;

	char rootobj[16];
	char curobj[16];
	uint32_t ih, nh;

	Field* flde_objid;
	Field* flde_vernum;
	Field* flde_extname;
	Field* flde_extverid;
	Field* flde_datapacked;
	Field* flde_extdata;
	Field* flde_datahash;
	class index* inde;
	char* rece;
	DynamicArray<char*> reces;
	DynamicArray<String> extnames;
	int32_t nreces;
	uint32_t ie, ne;

	bool ok;
	bool lastremoved;
	bool removed;
	bool datapacked;
	bool deletesobj;
	char emptyimage[8];
	uint32_t i, k, _crc;
	int32_t v, j, res, lastver, n;
	String sn;
	depot_ver depotVer;
	uint32_t configVerMajor, configVerMinor;
	TStream* in;
	TStream* out;
	TStream* st;
	std::vector<_packdata> packdates;
	TSearchRec srec;
	_packdata pd;
	_packdata* pdr;
	int64_t packlen;
	v8catalog* cat;
	v8catalog* cath;
	bool oldformat;
	v8file* f;
	tree* t;
	tree* tc;
	tree* tv; // корень дерева файла versions
	tree* tvc; // тек. элемент дерева файла versions
	tree* tr; // корень дерева файла root
	tree* trc; // тек. элемент дерева файла root
	tree* tcountv; // узел, содержащий счетчик в файле versions
	tree* tcountr; // узел, содержащий счетчик в файле root

	union
	{
		GUID guid;
		unsigned char uid[16];
	};

	union
	{
		char cv_b[2];
		unsigned short cv_s;
	};


	if(!is_open()) return false;

	if(!is_depot)
	{
		error("База не является хранилищем конфигурации.");
		return false;
	}

	// Получаем версию хранилища
	if(!table_depot)
	{
		error("В базе хранилища отсутствует таблица DEPOT.");
		return false;
	}

	fldd_depotver = get_field(table_depot, "DEPOTVER");
	if(!fldd_depotver) return false;
	fldd_rootobjid = get_field(table_depot, "ROOTOBJID");
	if(!fldd_rootobjid) return false;

	rec = new char[table_depot->get_recordlen()];
	ok = false;
	for(i = 0; i < table_depot->get_phys_numrecords(); i++)
	{
		table_depot->getrecord(i, rec);
		if(!*rec)
		{
			ok = true;
			break;
		}
	}
	if(!ok)
	{
		error("Не удалось прочитать запись в таблице DEPOT.");
		delete[] rec;
		return false;
	}

	{
	String s = fldd_depotver->get_presentation(rec, true);

	if(s.CompareIC("0300000000000000") == 0) depotVer = depotVer3;
	else if(s.CompareIC("0500000000000000") == 0) depotVer = depotVer5;
	else if(s.CompareIC("0600000000000000") == 0) depotVer = depotVer6;
	else if(s.CompareIC("0700000000000000") == 0) depotVer = depotVer7;
	else
	{
		msreg->AddMessage_("Неизвестная версия хранилища", msError,
			"Версия хранилища", s);
		delete[] rec;
		return false;
	}
	}

	memcpy(rootobj, rec + fldd_rootobjid->offset, 16);
	delete[] rec;

	// "Нормализуем" версию конфигурации
	ver = get_ver_depot_config(ver);

	// Ищем строку с номером версии конфигурации
	if(!table_versions)
	{
		error("В базе хранилища отсутствует таблица VERSIONS.");
		return false;
	}

	fldv_vernum = get_field(table_versions, "VERNUM");
	if(!fldv_vernum) return false;
	if(depotVer >= depotVer5)
	{
		fldv_cversion = get_field(table_versions, "CVERSION");
		if(!fldv_cversion) return false;
	}
	fldv_snapshotcrc = get_field(table_versions, "SNAPSHOTCRC");
	if(!fldv_snapshotcrc) return false;
	fldv_snapshotmaker = get_field(table_versions, "SNAPSHOTMAKER");
	if(!fldv_snapshotmaker) return false;

	rec = new char[table_versions->get_recordlen()];
	ok = false;
	for(i = 0; i < table_versions->get_phys_numrecords(); i++)
	{
		table_versions->getrecord(i, rec);
		if(*rec) continue;
		String s = fldv_vernum->get_presentation(rec, true);
		v = s.ToIntDef(0);
		if(v == ver)
		{
			ok = true;
			break;
		}
	}

	if(!ok)
	{
		if(msreg) msreg->AddMessage_("В хранилище не найдена версия конфигурации", msError,
			"Требуемая версия", ver);
		delete[] rec;
		return false;
	}

	boost::filesystem::path filepath = boost::filesystem::path(static_cast<std::string>(_filename));
	/*
  // Проверяем, нет ли снэпшота нужной версии конфигурации
  if(*(rec + fldv_snapshotcrc->offset)) if(*(rec + fldv_snapshotmaker->offset)) if(memcmp(rootobj, rec + fldv_snapshotmaker->offset + 1, 16) == 0)
  {
    _crc = *(uint32_t*)(rec + fldv_snapshotcrc->offset + 1);

    //s = System.IOUtils.TPath::GetDirectoryName(filename);
    s = filename.SubString(1, filename.LastDelimiter("\\"));
    s += "cache\\ddb";
    ss = "00000";
    ss += ver;
    s += ss.SubString(ss.GetLength() - 4, 5);
    s += ".snp";
    if(FileExists(s))
    {
      try
      {
        in = new TFileStream(s, fmOpenRead | fmShareDenyNone);
      }
      catch(...)
      {
        if(msreg) msreg->AddMessage_("Не удалось открыть файл снэпшота", msWarning,
          "Имя файла", s,
          "Требуемая версия", ver);
        in = NULL;
      }
      try
      {
        //if(FileExists(__filename)) DeleteFile(__filename);
        out = new TFileStream(__filename, fmCreate | fmShareDenyWrite);
      }
      catch(...)
      {
        if(msreg) msreg->AddMessage_("Не удалось создать файл конфигурации", msWarning,
          "Имя файла", __filename);
        delete[] rec;
        return false;
      }
      if(in)
      {
        try
        {
          InflateStream(in, out);
        }
        catch(...)
        {
          if(msreg) msreg->AddMessage_("Не удалось распаковать файл снэпшота", msWarning,
            "Имя файла", s,
            "Требуемая версия", ver);
          delete out;
          out = NULL;
        }
        delete in;
        in = NULL;
        if(out)
        {
          k = _crc32(out);
          if(k == _crc)
          {
            delete out;
            delete[] rec;
            return true;
          }
          if(msreg) msreg->AddMessage_("Файл снэпшота испорчен (не совпала контрольная сумма)", msWarning,
            "Имя файла", s,
            "Требуемая версия", ver,
            "Должен быть CRC32", tohex(_crc),
            "Получился CRC32", tohex(k));
          delete out;
        }
      }
    }
    else
    {
      if(msreg) msreg->AddMessage_("Не найден файл снэпшота", msWarning,
        "Имя файла", s,
        "Требуемая версия", ver);
    }
  }
	*/
	// Определяем версию структуры конфигурации (для файла version)
	if(depotVer >= depotVer5)
	{
		frec = rec + fldv_cversion->offset;
		cv_b[0] = frec[1];
		cv_b[1] = frec[0];
		configVerMajor = cv_s;
		frec += 2;
		cv_b[0] = frec[1];
		cv_b[1] = frec[0];
		configVerMinor = cv_s;
	}
	else
	{
		configVerMinor = 0;
		if(version == ver8_0_3_0 || version == ver8_0_5_0) configVerMajor = 6;
		else if(version == ver8_1_0_0) configVerMajor = 106;
		else configVerMajor = 216;
	}

	delete[] rec;

	// Инициализируем таблицы HISTORY и EXTERNALS
	if(!table_history)
	{
		error("В базе хранилища отсутствует таблица HISTORY.");
		return false;
	}

	if(!table_externals)
	{
		error("В базе хранилища отсутствует таблица EXTERNALS.");
		return false;
	}

	fldh_objid = get_field(table_history, "OBJID");
	if(!fldh_objid) return false;
	fldh_vernum = get_field(table_history, "VERNUM");
	if(!fldh_vernum) return false;
	fldh_objverid = get_field(table_history, "OBJVERID");
	if(!fldh_objverid) return false;
	fldh_removed = get_field(table_history, "REMOVED");
	if(!fldh_removed) return false;
	fldh_datapacked = get_field(table_history, "DATAPACKED");
	if(!fldh_datapacked) return false;
	fldh_objdata = get_field(table_history, "OBJDATA");
	if(!fldh_objdata) return false;

	flde_objid = get_field(table_externals, "OBJID");
	if(!flde_objid) return false;
	flde_vernum = get_field(table_externals, "VERNUM");
	if(!flde_vernum) return false;
	flde_extname = get_field(table_externals, "EXTNAME");
	if(!flde_extname) return false;
	flde_extverid = get_field(table_externals, "EXTVERID");
	if(!flde_extverid) return false;
	flde_datapacked = get_field(table_externals, "DATAPACKED");
	if(!flde_datapacked) return false;
	flde_extdata = get_field(table_externals, "EXTDATA");
	if(!flde_extdata) return false;

	boost::filesystem::path root_path(static_cast<std::string>(filename)); // путь к 1cd
	boost::filesystem::path objects_path;

	if(depotVer >= depotVer6)
	{
		fldh_datahash = get_field(table_history, "DATAHASH");
		if(!fldh_datahash) return false;
		flde_datahash = get_field(table_externals, "DATAHASH");
		if(!flde_datahash) return false;

		boost::filesystem::path subpath = root_path.parent_path() / "data" / "pack";
		std::regex pack_mask("pack-.*\\.ind");
		boost::filesystem::directory_iterator dit(subpath), dend;
		for (; dit != dend; dit++)
		{
			boost::filesystem::path current_path = dit->path();
			if (!std::regex_match(current_path.filename().string(), pack_mask)) {
				continue;
			}
				try
				{
					in = new TFileStream(current_path.string(), fmOpenRead | fmShareDenyNone);
				}
				catch(...)
				{
					if(msreg) msreg->AddMessage_("Ошибка открытия файла", msError,
						"Файл", current_path.string());
					return false;
				}
				in->Seek(8, soFromBeginning);
				in->Read(&i, 4);
				pd.datahashes = new _datahash[i];
				in->Read(pd.datahashes, i * sizeof(_datahash));
				delete in;
				pd.count = i;

				boost::filesystem::path pack_item = current_path;
				pack_item.replace_extension("pck");
				try
				{
					pd.pack = new TFileStream(pack_item.string(), fmOpenRead | fmShareDenyNone);
				}
				catch(...)
				{
				    delete pd.pack;
					if(msreg) msreg->AddMessage_("Ошибка открытия файла", msError,
						"Файл", pack_item.string());
					return false;
				}
				packdates.push_back(pd);
		}

		objects_path = root_path / "data" / "objects";
	}
	else
	{
		fldh_datahash = NULL;
		flde_datahash = NULL;
	}

	indh = get_index(table_history, "PK");
	if(!indh) return 0;
	inde = get_index(table_externals, "PK");
	if(!inde) return 0;

	rech1 = new char[table_history->get_recordlen()];
	rech2 = new char[table_history->get_recordlen()];
	rece = new char[table_externals->get_recordlen()];
	memset(rece, 0, table_externals->get_recordlen());
	nreces = 0;
	reces.SetLength(0);

	nh = indh->get_numrecords();
	ne = inde->get_numrecords();
	memset(curobj, 0, 16);

	if (boost::filesystem::exists(filepath)) {
		boost::filesystem::remove(filepath);
	}
	cat = new v8catalog(filepath.string(), false);

	// root, versions

	std::map<String,String> vermap; // контейнер для сортировки versions
	std::map<String,String> rootmap; // контейнер для сортировки root
	std::map<String,TStream*> extmap; // контейнер для сортировки файлов в корне
	std::map<String,TStream*> metamap; // контейнер для сортировки файлов в metadata

	tv = new tree("", nd_list, NULL); // корень дерева файла versions
	tvc = new tree("", nd_list, tv); // тек. элемент дерева файла versions
	tr = new tree("", nd_list, NULL); // корень дерева файла root
	trc = new tree("", nd_list, tr); // тек. элемент дерева файла root

	tvc->add_child("1", nd_number);
	tcountv = tvc->add_child("0", nd_number); // узел, содержащий счетчик в файле versions

	CreateGUID(guid);
	vermap[""] = GUIDasMS(uid);

	String sversion;
	{// Создаем и записываем файл version
	String s;
	t = new tree("", nd_list, NULL);
	tc = new tree("", nd_list, t);
	tc = new tree("", nd_list, tc);
	s = configVerMajor;
	tc->add_child(s, nd_number);
	s = configVerMinor;
	tc->add_child(s, nd_number);
	sversion = outtext(t);
	delete t;
	}

	in = new TMemoryStream;
	in->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().GetLength());
	{
		TStreamWriter sw(in, TEncoding::UTF8, 1024);
		sw.Write(sversion);
	}
	out = new TMemoryStream;
	in->Seek(0, soFromBeginning);
	ZDeflateStream(in, out);
	delete in;
	extmap["version"] = out;
	CreateGUID(guid);
	vermap["version"] = GUIDasMS(uid);

	if(configVerMajor < 100)
	{
		trc->add_child("1", nd_number);
		trc->add_child(GUIDasMS((unsigned char*)rootobj), nd_guid);
		tcountr = trc->add_child("0", nd_number); // узел, содержащий счетчик в файле root
		oldformat = true;
	}
	else
	{
		trc->add_child("2", nd_number);
		trc->add_child(GUIDasMS((unsigned char*)rootobj), nd_guid);
		tcountr = NULL;
		oldformat = false;
	}

	lastver = -1;
	lastremoved = true;
	memset(emptyimage, 0, 8);

	in = new TMemoryStream;
	for(ih = 0, ie = 0; ih <= nh; ih++)
	{
		if(ih < nh)
		{
			i = indh->get_numrec(ih);
			table_history->getrecord(i, rech2);
		}

		if(memcmp(curobj, rech2 + fldh_objid->offset, 16) != 0 || ih == nh)
		{ // это новый объект или конец таблицы
			if(!lastremoved)
			{
				ok = false;
				deletesobj = false;
				packlen = 0;
				rec = rech1 + fldh_objdata->offset + 1;
				if(*(rech1 + fldh_objdata->offset) && memcmp(emptyimage, rec, 8))
				{
					out = new TTempStream;
					if(oldformat)
					{
						table_history->readBlob(in, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
						in->Seek(0, soFromBeginning);
						ZInflateStream(in, out);
					}
					else table_history->readBlob(out, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
					out->Close();
					ok = true;
				}
				else if(depotVer >= depotVer6)
				{
					rec = rech1 + fldh_datahash->offset + (fldh_datahash->null_exists ? 1 : 0);
					for(i = 0; i < packdates.size(); i++)
					{
						pdr = &packdates[i];
						for(k = 0; k < pdr->count; k++) if(memcmp(rec, pdr->datahashes[k].datahash, 20) == 0)
						{
							st = pdr->pack;
							st->Seek(pdr->datahashes[k].offset, soBeginning);
							st->Read(&packlen, 8);
							out = new TTempStream;
							out->CopyFrom(st, packlen);
							ok = true;
							out->Close();
							break;
						}
						if(ok) break;
					}

					if(!ok)
					{
						String ss = fldh_datahash->get_presentation(rech1, true);
						boost::filesystem::path current_object_path = objects_path / static_cast<std::string>(ss.SubString(3, ss.GetLength() - 2));
						if(boost::filesystem::exists(current_object_path))
						{
							try
							{
								out = new TFileStream(current_object_path.string(), fmOpenRead | fmShareDenyNone);
								ok = true;
							}
							catch(...)
							{
								if(msreg) msreg->AddMessage_("Ошибка открытия файла", msError,
									"Файл", current_object_path.string(),
									"Таблица", "HISTORY",
									"Объект", fldh_objid->get_presentation(rech1, false, L'.', true),
									"Версия", fldh_vernum->get_presentation(rech1, false));
							}
						}
						else
						{
							if(msreg) msreg->AddMessage_("Не найден файл", msError,
								"Файл", current_object_path.string(),
								"Таблица", "HISTORY",
								"Объект", fldh_objid->get_presentation(rech1, false, L'.', true),
								"Версия", fldh_vernum->get_presentation(rech1, false));
						}
					}
				}
				String s = fldh_objid->get_presentation(rech1, false, L'.', true);
				if(!ok)
				{
					if(msreg) msreg->AddMessage_("Ошибка чтения объекта конфигурации", msError,
						"Таблица", "HISTORY",
						"Объект", s,
						"Версия", fldh_vernum->get_presentation(rech1, false));
				}
				else
				{
					if(oldformat)
					{
						rootmap[s] = GUIDasMS((unsigned char*)rech1 + fldh_objverid->offset);
						metamap[s] = out;
					}
					else
					{
						vermap[s] = GUIDasMS((unsigned char*)rech1 + fldh_objverid->offset);
						extmap[s] = out;
					}


					// Вот тут идем по EXTERNALS
					while(true)
					{
						if(ie > ne) break;
						res = memcmp(rece + flde_objid->offset, curobj, 16);
						if(res > 0) break;
						if(!res)
						{
							s = flde_vernum->get_presentation(rece, false);
							v = s.ToIntDef(MaxInt);
							s = flde_extname->get_presentation(rece);
							if(v <= ver) if(*(rece + flde_datapacked->offset))
							{
								for(j = 0; j < nreces; j++) if(s.CompareIC(flde_extname->get_presentation(reces[j])) == 0) break;
								if(j == reces.GetLength()){
									reces.SetLength(reces.GetLength() + 1);
									reces[j] = new char[table_externals->get_recordlen()];
								}
								if(j == nreces) nreces++;
								memcpy(reces[j], rece, table_externals->get_recordlen());
							}
							if(v == lastver)
							{
								extnames.SetLength(extnames.GetLength() + 1);
								extnames[extnames.GetLength() - 1] = s;
							}
						}
						if(ie == ne)
						{
							ie++;
							break;
						}
						i = inde->get_numrec(ie++);
						table_externals->getrecord(i, rece);
					}
					for(j = 0; j < nreces; j++)
					{
						rec = reces[j];
						sn = flde_extname->get_presentation(rec);
						ok = false;
						for(n = 0; n < extnames.GetLength(); n++ ) if(sn.CompareIC(extnames[n]) == 0)
						{
							ok = true;
							break;
						}
						if(!ok) continue;

						ok = false;
						packlen = 0;
						deletesobj = false;
						frec = rec + flde_extdata->offset;
						if(memcmp(emptyimage, frec, 8))
						{
							out = new TTempStream;
							table_externals->readBlob(out, *(uint32_t*)frec, *(uint32_t*)(frec + 4));
							out->Close();
							ok = true;
						}
						else if(depotVer >= depotVer6)
						{
							frec = rec + flde_datahash->offset + (flde_datahash->null_exists ? 1 : 0);
							for(i = 0; i < packdates.size(); i++)
							{
								pdr = &packdates[i];
								for(k = 0; k < pdr->count; k++) if(memcmp(frec, pdr->datahashes[k].datahash, 20) == 0)
								{
									out = new TTempStream;
									st = pdr->pack;
									st->Seek(pdr->datahashes[k].offset, soBeginning);
									st->Read(&packlen, 8);
									out->CopyFrom(st, packlen);
									ok = true;
									out->Close();
									break;
								}
								if(ok) break;
							}

							if(!ok)
							{
								String ss = flde_datahash->get_presentation(rec, true);
								boost::filesystem::path current_object_path = objects_path / static_cast<std::string>(ss.SubString(3, ss.GetLength() - 2));
								if (boost::filesystem::exists(current_object_path))
								{
									try
									{
										out = new TFileStream(current_object_path.string(), fmOpenRead | fmShareDenyNone);
										ok = true;
									}
									catch(...)
									{
										if(msreg) msreg->AddMessage_("Ошибка открытия файла", msError,
											"Файл", current_object_path.string(),
											"Таблица", "EXTERNALS",
											"Объект", flde_extname->get_presentation(rec),
											"Версия", flde_vernum->get_presentation(rec));
									}
								}
								else
								{
									if(msreg) msreg->AddMessage_("Не найден файл", msError,
										"Файл", current_object_path.string(),
										"Таблица", "EXTERNALS",
										"Объект", flde_extname->get_presentation(rec),
										"Версия", flde_vernum->get_presentation(rec));
								}
							}
						}
						if(!ok)
						{
							if(msreg) msreg->AddMessage_("Ошибка чтения объекта конфигурации", msError,
								"Таблица", "EXTERNALS",
								"Объект", sn,
								"Версия", flde_vernum->get_presentation(rec));
						}
						else
						{
							vermap[sn] = GUIDasMS((unsigned char*)rec + flde_extverid->offset);
							extmap[sn] = out;
						}

					}
					nreces = 0;
					extnames.SetLength(0);
				}
			}

			memcpy(curobj, rech2 + fldh_objid->offset, 16);
			lastremoved = true;
		}

		if(ih < nh)
		{
			String s = fldh_vernum->get_presentation(rech2, false);
			v = s.ToIntDef(MaxInt);
			if(v <= ver)
			{
				removed = *(rech2 + fldh_removed->offset);
				if(removed)
				{
					lastremoved = true;
				}
				else
				{
					datapacked = false;
					if(*(rech2 + fldh_datapacked->offset)) if(*(rech2 + fldh_datapacked->offset + 1)) datapacked = true;
					if(datapacked)
					{
						memcpy(rech1, rech2, table_history->get_recordlen());
						lastremoved = false;
						lastver = v;
					}
				}
			}
		}
	}

	delete[] rech1;
	delete[] rech2;
	delete[] rece;
	for(j = 0; j < nreces; j++) delete[] reces[j];


	// Завершаем формирование списков версий
	std::map<String,String>::iterator pmap;

	CreateGUID(guid);
	vermap["versions"] = GUIDasMS(uid);
	if(oldformat)
	{
		tcountv->set_value(vermap.size(), nd_number);
		tcountr->set_value(rootmap.size(), nd_number);
	}
	else
	{
		CreateGUID(guid);
		vermap["root"] = GUIDasMS(uid);
		tcountv->set_value(vermap.size(), nd_number);
	}

	// Запись root
	for(pmap = rootmap.begin(); pmap != rootmap.end(); ++pmap)
	{
		trc->add_child(pmap->first, nd_string);
		trc->add_child(pmap->second, nd_guid);
	}
	String tree_text = outtext(tr);
	delete tr;
	in->SetSize(0);
	in->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().GetLength());
	{
		TStreamWriter sw(in, TEncoding::UTF8, 1024);
		sw.Write(tree_text);
	}
	in->Seek(0, soFromBeginning);
	out = new TTempStream;
	if(oldformat)
	{
		out->CopyFrom(in, 0);
		metamap["root"] = out;
	}
	else
	{
		ZDeflateStream(in, out);
		extmap["root"] = out;
	}

	// Запись versions

	for(pmap = vermap.begin(); pmap != vermap.end(); ++pmap)
	{
		tvc->add_child(pmap->first, nd_string);
		tvc->add_child(pmap->second, nd_guid);
	}

	String tv_text = outtext(tv);
	delete tv;
	in->SetSize(0);
	in->Write(TEncoding::UTF8->GetPreamble(), TEncoding::UTF8->GetPreamble().GetLength());
	{
		TStreamWriter sw(in, TEncoding::UTF8, 1024);
		sw.Write(tv_text);
	}
	out = new TTempStream;
	in->Seek(0, soFromBeginning);
	ZDeflateStream(in, out);
	extmap["versions"] = out;

	std::map<String,TStream*>::iterator psmap;
	if(oldformat)
	{
		cath = cat->CreateCatalog("metadata", true);
		for(psmap = metamap.begin(); psmap != metamap.end(); ++psmap)
		{
			f = cath->createFile(psmap->first);
			f->WriteAndClose(psmap->second);
			delete psmap->second;
		}
	}
	for(psmap = extmap.begin(); psmap != extmap.end(); ++psmap)
	{
		f = cat->createFile(psmap->first);
		f->WriteAndClose(psmap->second);
		delete psmap->second;
	}

	delete in;

	for(i = 0; i < packdates.size(); i++)
	{
		delete packdates[i].pack;
		delete[] packdates[i].datahashes;
	}

	delete cat;
	return true;
}

//---------------------------------------------------------------------------
// Сохранение файлов конфигурации в каталог из хранилища конфигураций
// ver_begin - начальный номер диапазона версий сохраняемых файлов конфигурации
// ver_end - конечный номер диапазона версий сохраняемых файлов конфигурации
// ver_begin > 0, ver_end > 0 - используется переданный номер версии
// ver_begin <= 0, ver_end <= 0 - номер версии от последней конфигурации. 0 - последняя конфигурация, -1 - предпоследняя и т.д., т.е. Номер версии определяется как номер последней + ver
bool T_1CD::save_part_depot_config(const String& _filename, int32_t ver_begin, int32_t ver_end)
{
	char* rec;
	char* frec;
	Field* fldd_depotver;

	Field* fldv_vernum;
	Field* fldv_cversion;
	Field* fldv_snapshotcrc;
	Field* fldv_snapshotmaker;

	Field* fldh_objid;
	Field* fldh_vernum;
	Field* fldh_objverid;
	Field* fldh_removed;
	Field* fldh_datapacked;
	Field* fldh_objdata;
	Field* fldh_datahash;
	class index* indh;
	char* rech; // текущая запись HISTORY
	char* rech1; // запись с версией < ver_begin
	bool hasrech1;
	char* rech2; // запись с версией <= ver_end
	bool hasrech2;

	char rootobj[16];
	char curobj[16];
	uint32_t ih, nh;

	Field* flde_objid;
	Field* flde_vernum;
	Field* flde_extname;
	Field* flde_extverid;
	Field* flde_datapacked;
	Field* flde_extdata;
	Field* flde_datahash;
	class index* inde;
	char* rece;
	uint32_t ie, ne, je;

	bool ok;
	bool removed;
	bool datapacked;
	bool deletesobj;
	bool iscatalog;
	bool changed;
	bool inreaded;
	bool hasext;
	char emptyimage[8];
	char verid[16];
	uint32_t i, k, _crc;
	int32_t v, j, res, lastver, n;
	String s, ss, sp, sn, se;
	depot_ver depotVer;
	uint32_t configVerMajor, configVerMinor;
	TMemoryStream* in;
	TMemoryStream* out;
	TStream* sobj;
	TStreamWriter* sw;
	TMemoryStream* sd;
	bool hasdeleted;
	std::vector<_packdata> packdates;
	TSearchRec srec;
	_packdata pd;
	_packdata* pdr;
	int64_t packlen;
	v8catalog* cat;
	String cath;
	TFileStream* f;
	String __filename;

	union
	{
		GUID guid;
		unsigned char uid[16];
	};

	union
	{
		char cv_b[2];
		unsigned short cv_s;
	};


	if(!is_open()) return false;

	if(!is_depot)
	{
		error("База не является хранилищем конфигурации.");
		return false;
	}

	// Получаем версию хранилища
	if(!table_depot)
	{
		error("В базе хранилища отсутствует таблица DEPOT.");
		return false;
	}

	fldd_depotver = get_field(table_depot, "DEPOTVER");
	if(!fldd_depotver) return false;

	rec = new char[table_depot->get_recordlen()];
	ok = false;
	for(i = 0; i < table_depot->get_phys_numrecords(); i++)
	{
		table_depot->getrecord(i, rec);
		if(!*rec)
		{
			ok = true;
			break;
		}
	}
	if(!ok)
	{
		error("Не удалось прочитать запись в таблице DEPOT.");
		delete[] rec;
		return false;
	}

	s = fldd_depotver->get_presentation(rec, true);

	if(s.CompareIC("0300000000000000") == 0) depotVer = depotVer3;
	else if(s.CompareIC("0500000000000000") == 0) depotVer = depotVer5;
	else if(s.CompareIC("0600000000000000") == 0) depotVer = depotVer6;
	else
	{
		msreg->AddMessage_("Неизвестная версия хранилища", msError,
			"Версия хранилища", s);
		delete[] rec;
		return false;
	}

	delete[] rec;

	// "Нормализуем" версию конфигурации
	ver_begin = get_ver_depot_config(ver_begin);
	ver_end = get_ver_depot_config(ver_end);
	if(ver_end < ver_begin)
	{
		v = ver_begin;
		ver_begin = ver_end;
		ver_end = v;
	}

	// Ищем строку с номером версии конфигурации
	if(!table_versions)
	{
		error("В базе хранилища отсутствует таблица VERSIONS.");
		return false;
	}

	fldv_vernum = get_field(table_versions, "VERNUM");
	if(!fldv_vernum) return false;
	if(depotVer >= depotVer5)
	{
		fldv_cversion = get_field(table_versions, "CVERSION");
		if(!fldv_cversion) return false;
	}
	fldv_snapshotcrc = get_field(table_versions, "SNAPSHOTCRC");
	if(!fldv_snapshotcrc) return false;
	fldv_snapshotmaker = get_field(table_versions, "SNAPSHOTMAKER");
	if(!fldv_snapshotmaker) return false;

	rec = new char[table_versions->get_recordlen()];
	n = 0;
	for(i = 0; i < table_versions->get_phys_numrecords(); i++)
	{
		table_versions->getrecord(i, rec);
		if(*rec) continue;
		s = fldv_vernum->get_presentation(rec, true);
		v = s.ToIntDef(0);
		if(v == ver_begin) n++;
		if(v == ver_end) n++;
		if(n >= 2) break;
	}

	if(n < 2)
	{
		if(msreg) msreg->AddMessage_("В хранилище не найдены запрошенные версии конфигурации", msError
			, "Версия с", ver_begin
			, "Версия по", ver_end);
		delete[] rec;
		return false;
	}

	__filename = System::Ioutils::TPath::GetFullPath(_filename);

	// Определяем версию структуры конфигурации (для файла version)
	if(depotVer >= depotVer5)
	{
		frec = rec + fldv_cversion->offset;
		cv_b[0] = frec[1];
		cv_b[1] = frec[0];
		configVerMajor = cv_s;
		frec += 2;
		cv_b[0] = frec[1];
		cv_b[1] = frec[0];
		configVerMinor = cv_s;
	}
	else
	{
		configVerMinor = 0;
		if(version == ver8_0_3_0 || version == ver8_0_5_0) configVerMajor = 6;
		else if(version == ver8_1_0_0) configVerMajor = 106;
		else configVerMajor = 216;
	}

	delete[] rec;

	// Инициализируем таблицы HISTORY и EXTERNALS
	if(!table_history)
	{
		error("В базе хранилища отсутствует таблица HISTORY.");
		return false;
	}

	if(!table_externals)
	{
		error("В базе хранилища отсутствует таблица EXTERNALS.");
		return false;
	}

	fldh_objid = get_field(table_history, "OBJID");
	if(!fldh_objid) return false;
	fldh_vernum = get_field(table_history, "VERNUM");
	if(!fldh_vernum) return false;
	fldh_objverid = get_field(table_history, "OBJVERID");
	if(!fldh_objverid) return false;
	fldh_removed = get_field(table_history, "REMOVED");
	if(!fldh_removed) return false;
	fldh_datapacked = get_field(table_history, "DATAPACKED");
	if(!fldh_datapacked) return false;
	fldh_objdata = get_field(table_history, "OBJDATA");
	if(!fldh_objdata) return false;

	flde_objid = get_field(table_externals, "OBJID");
	if(!flde_objid) return false;
	flde_vernum = get_field(table_externals, "VERNUM");
	if(!flde_vernum) return false;
	flde_extname = get_field(table_externals, "EXTNAME");
	if(!flde_extname) return false;
	flde_extverid = get_field(table_externals, "EXTVERID");
	if(!flde_extverid) return false;
	flde_datapacked = get_field(table_externals, "DATAPACKED");
	if(!flde_datapacked) return false;
	flde_extdata = get_field(table_externals, "EXTDATA");
	if(!flde_extdata) return false;

	if(depotVer >= depotVer6)
	{
		fldh_datahash = get_field(table_history, "DATAHASH");
		if(!fldh_datahash) return false;
		flde_datahash = get_field(table_externals, "DATAHASH");
		if(!flde_datahash) return false;

		sp = filename.SubString(1, filename.LastDelimiter("\\")) + "data\\pack\\";
		s = sp + "pack-*.ind";
		if(FindFirst(s, 0, srec) == 0)
		{
			do
			{
				try
				{
					f = new TFileStream(sp + srec.Name, fmOpenRead | fmShareDenyNone);
				}
				catch(...)
				{
					if(msreg) msreg->AddMessage_("Ошибка открытия файла", msError,
						"Файл", srec.Name);
					FindClose(srec);
					return false;
				}
				f->Seek(8, soFromBeginning);
				f->Read(&i, 4);
				pd.datahashes = new _datahash[i];
				f->Read(pd.datahashes, i * sizeof(_datahash));
				delete f;
				pd.count = i;
				try
				{
					s = sp + srec.Name.SubString(1, srec.Name.GetLength() - 3) + "pck";
					pd.pack = new TFileStream(s, fmOpenRead | fmShareDenyNone);
				}
				catch(...)
				{
				    delete pd.pack;
					if(msreg) msreg->AddMessage_("Ошибка открытия файла", msError,
						"Файл", s);
					FindClose(srec);
					return false;
				}
				packdates.push_back(pd);
			} while(FindNext(srec) == 0);
			FindClose(srec);
		}

		sp = filename.SubString(1, filename.LastDelimiter("\\")) + "data\\objects\\";
	}
	else
	{
		fldh_datahash = NULL;
		flde_datahash = NULL;
	}

	indh = get_index(table_history, "PK");
	if(!indh) return 0;
	inde = get_index(table_externals, "PK");
	if(!inde) return 0;

	rech = new char[table_history->get_recordlen()];
	rech1 = new char[table_history->get_recordlen()];
	rech2 = new char[table_history->get_recordlen()];
	rece = new char[table_externals->get_recordlen()];
	memset(rece, 0, table_externals->get_recordlen());

	nh = indh->get_numrecords();
	ne = inde->get_numrecords();
	memset(curobj, 0, 16);


	if(configVerMajor < 100)
	{
		cath = __filename + "\\metadata\\";
	}
	else
	{
		cath = __filename + "\\";
	}

	lastver = -1;
	memset(emptyimage, 0, 8);

	sd = new TMemoryStream;
	sw = new TStreamWriter(sd, TEncoding::UTF8, 4096);
	hasdeleted = false;

	in = new TMemoryStream;
	out = new TMemoryStream;
	cat = NULL;
	hasrech1 = false;
	hasrech2 = false;
	for(ih = 0, ie = 0; ih <= nh; ih++)
	{
		if(ih < nh)
		{
			i = indh->get_numrec(ih);
			table_history->getrecord(i, rech);
		}

		if(memcmp(curobj, rech + fldh_objid->offset, 16) != 0 || ih == nh)
		{ // это новый объект или конец таблицы
			if(ih) if(hasrech2)
			{
				s = fldh_vernum->get_presentation(rech2, false);
				lastver = s.ToIntDef(MaxInt);
				sn = fldh_objid->get_presentation(rech2, false, L'.', true);

				hasext = true;
				removed = *(rech2 + fldh_removed->offset);
				if(removed)
				{
					if(hasrech1)
					{
						sw->Write(sn + "\r\n");
						hasdeleted = true;
					}
					else hasext = false;
				}
				else
				{
					datapacked = false;
					if(*(rech2 + fldh_datapacked->offset)) if(*(rech2 + fldh_datapacked->offset + 1)) datapacked = true;

					if(datapacked)
					{
						changed = true;
						inreaded = false;
						if(hasrech1)
						{
							datapacked = false;
							if(*(rech1 + fldh_datapacked->offset)) if(*(rech1 + fldh_datapacked->offset + 1)) datapacked = true;
							if(datapacked)
							{
								rec = rech1 + fldh_objdata->offset + 1;
								if(*(rech1 + fldh_objdata->offset) && memcmp(emptyimage, rec, 8))
								{
									table_history->readBlob(out, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
									rec = rech2 + fldh_objdata->offset + 1;
									if(*(rech2 + fldh_objdata->offset) && memcmp(emptyimage, rec, 8))
									{
										table_history->readBlob(in, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
										inreaded = true;
										if(in->GetSize() == out->GetSize()) if(memcmp(in->GetMemory(), out->GetMemory(), in->GetSize()) == 0) changed = false;
									}
								}
								else if(depotVer >= depotVer6)
								{
									if(memcmp(rech2 + fldh_datahash->offset + (fldh_datahash->null_exists ? 1 : 0)
											, rech1 + fldh_datahash->offset + (fldh_datahash->null_exists ? 1 : 0)
											, fldh_datahash->length) == 0) changed = false;
								}
							}
						}

						if(changed)
						{
							ok = false;
							deletesobj = false;
							packlen = 0;
							rec = rech2 + fldh_objdata->offset + 1;
							if(inreaded)
							{
								sobj = in;
								ok = true;
							}
							else if(*(rech2 + fldh_objdata->offset) && memcmp(emptyimage, rec, 8))
							{
								table_history->readBlob(in, *(uint32_t*)rec, *(uint32_t*)(rec + 4));
								sobj = in;
								ok = true;
							}
							else if(depotVer >= depotVer6)
							{
								rec = rech2 + fldh_datahash->offset + (fldh_datahash->null_exists ? 1 : 0);
								for(i = 0; i < packdates.size(); i++)
								{
									pdr = &packdates[i];
									for(k = 0; k < pdr->count; k++) if(memcmp(rec, pdr->datahashes[k].datahash, 20) == 0)
									{
										sobj = pdr->pack;
										sobj->Seek(pdr->datahashes[k].offset, soBeginning);
										sobj->Read(&packlen, 8);
										in->SetSize(0);
										in->CopyFrom(sobj, packlen);
										sobj = in;
										ok = true;
										break;
									}
									if(ok) break;
								}

								if(!ok)
								{
									ss = fldh_datahash->get_presentation(rech2, true);
									s = sp + ss.SubString(1, 2) + L'\\' + ss.SubString(3, ss.GetLength() - 2);
									if(FileExists(s))
									{
										try
										{
											sobj = new TFileStream(s, fmOpenRead | fmShareDenyNone);
											deletesobj = true;
											ok = true;
										}
										catch(...)
										{
											if(msreg) msreg->AddMessage_("Ошибка открытия файла", msError,
												"Файл", s,
												"Таблица", "HISTORY",
												"Объект", sn,
												"Версия", lastver);
										}
									}
									else
									{
										if(msreg) msreg->AddMessage_("Не найден файл", msError,
											"Файл", s,
											"Таблица", "HISTORY",
											"Объект", sn,
											"Версия", lastver);
									}
								}
							}

							if(!ok)
							{
								if(msreg) msreg->AddMessage_("Ошибка чтения объекта конфигурации", msError,
									"Таблица", "HISTORY",
									"Объект", sn,
									"Версия", lastver);
							}
							else
							{
								f = new TFileStream(cath + sn, fmCreate);
								sobj->Seek(0, soFromBeginning);
								ZInflateStream(sobj, f);
								delete f;
								if(deletesobj) delete sobj;

							}
						}
					}
				}

				// Вот тут идем по EXTERNALS
				while(hasext)
				{
					if(ie > ne) break;
					res = memcmp(rece + flde_objid->offset, curobj, 16);
					if(res > 0) break;
					if(!res)
					{
						s = flde_vernum->get_presentation(rece, false);
						v = s.ToIntDef(MaxInt);
						if(v == lastver)
						{
							se = flde_extname->get_presentation(rece);
							if(removed)
							{
								sw->Write(se + "\r\n");
								hasdeleted = true;
							}
							else
							{
								datapacked = *(rece + flde_datapacked->offset);

								// ==> Поиск записи о файле
								// В случае отсутствия данных (datapacked = false) в этой записи пытаемся найти предыдущую запись с данными
								// (с тем же objid, extname и extverid), но в пределах ver_begin <= vernum < lastver
								memcpy(verid, rece + flde_extverid->offset, 16);
								je = ie;
								while(!datapacked && v > ver_begin && je)
								{
									i = inde->get_numrec(--je);
									table_externals->getrecord(i, rece);
									res = memcmp(rece + flde_objid->offset, curobj, 16);
									if(res) break;
									s = flde_extname->get_presentation(rece);
									if(s.CompareIC(se)) continue;
									if(memcmp(verid, rece + flde_extverid->offset, 16) == 0)
									{
										s = flde_vernum->get_presentation(rece, false);
										v = s.ToIntDef(MaxInt);
										if(v < ver_begin) break;
										datapacked = *(rece + flde_datapacked->offset);
									}
								}
								// <== Поиск записи о файле

								if(datapacked)
								{
									ok = false;
									packlen = 0;
									deletesobj = false;
									frec = rece + flde_extdata->offset;
									if(memcmp(emptyimage, frec, 8))
									{
										table_externals->readBlob(in, *(uint32_t*)frec, *(uint32_t*)(frec + 4));
										sobj = in;
										ok = true;
									}
									else if(depotVer >= depotVer6)
									{
										frec = rece + flde_datahash->offset + (flde_datahash->null_exists ? 1 : 0);
										for(i = 0; i < packdates.size(); i++)
										{
											pdr = &packdates[i];
											for(k = 0; k < pdr->count; k++) if(memcmp(frec, pdr->datahashes[k].datahash, 20) == 0)
											{
												sobj = pdr->pack;
												sobj->Seek(pdr->datahashes[k].offset, soBeginning);
												sobj->Read(&packlen, 8);
												in->SetSize(0);
												in->CopyFrom(sobj, packlen);
												sobj = in;
												ok = true;
												break;
											}
											if(ok) break;
										}

										if(!ok)
										{
											ss = flde_datahash->get_presentation(rece, true);
											s = sp + ss.SubString(1, 2) + L'\\' + ss.SubString(3, ss.GetLength() - 2);
											if(FileExists(s))
											{
												try
												{
													sobj = new TFileStream(s, fmOpenRead | fmShareDenyNone);
													deletesobj = true;
													ok = true;
												}
												catch(...)
												{
													if(msreg) msreg->AddMessage_("Ошибка открытия файла", msError,
														"Файл", s,
														"Таблица", "EXTERNALS",
														"Объект", sn,
														"Файл конфигурации", se,
														"Версия", v);
												}
											}
											else
											{
												if(msreg) msreg->AddMessage_("Не найден файл", msError,
													"Файл", s,
													"Таблица", "EXTERNALS",
													"Объект", sn,
													"Файл конфигурации", se,
													"Версия", v);
											}
										}
									}
									if(!ok)
									{
										if(msreg) msreg->AddMessage_("Ошибка чтения объекта конфигурации", msError,
											"Таблица", "EXTERNALS",
											"Объект", sn,
											"Файл конфигурации", se,
											"Версия", v);
									}
									else
									{
										out->SetSize(0);
										sobj->Seek(0, soFromBeginning);
										ZInflateStream(sobj, out);
										iscatalog = false;
										if(out->GetSize() > 0)
										{
											cat = new v8catalog(out, false, true);
											iscatalog = cat->IsCatalog();
										}
										if(iscatalog) cat->SaveToDir(cath + se);
										else
										{
											f = new TFileStream(cath + se, fmCreate);
											f->CopyFrom(out, 0);
											delete f;
										}
										delete cat;
										cat = NULL;
										if(deletesobj) delete sobj;

									}

								}

							}
						}

					}
					if(ie == ne)
					{
						ie++;
						break;
					}
					i = inde->get_numrec(ie++);
					table_externals->getrecord(i, rece);
				}
			}

			memcpy(curobj, rech + fldh_objid->offset, 16);
			hasrech1 = false;
			hasrech2 = false;
		}

		if(ih < nh)
		{
			s = fldh_vernum->get_presentation(rech, false);
			v = s.ToIntDef(MaxInt);
			if(v < ver_begin)
			{
				memcpy(rech1, rech, table_history->get_recordlen());
				hasrech1 = true;
			}
			else if(v <= ver_end)
			{
				memcpy(rech2, rech, table_history->get_recordlen());
				hasrech2 = true;
			}
		}
	}

	delete sw;
	if(hasdeleted)
	{
		f = new TFileStream(__filename + "\\deleted", fmCreate);
		f->CopyFrom(sd, 0);
		delete f;
	}
	delete sd;

	delete[] rech;
	delete[] rech1;
	delete[] rech2;
	delete[] rece;

	for(i = 0; i < packdates.size(); i++)
	{
		delete packdates[i].pack;
		delete[] packdates[i].datahashes;
	}



	return true;
}


#ifndef PublicRelease
//---------------------------------------------------------------------------
// Проверка и восстановление таблицы размещения файла DATA переденной таблицы
// Проверка записей происходит по тестовому шаблону, созданному из описания полей
// Проверяются страницы файла DATA, если проверка не проходит, производится поиск подходящей страницы в файле.
void T_1CD::restore_DATA_allocation_table(Table* tab)
{
	char* rectt;
	uint32_t block;
	v8ob* rootobj;
	objtab* ca;
	char* cd;
	uint32_t i, l, cl, a, d, r;
	int32_t j, k, m, n, rl;
	bool ok;
	std::vector<uint32_t> bk;
	String s;

	block = tab->get_file_data()->get_block_number();

	if(block < 5 || block >= length)
	{
		if(msreg) msreg->AddMessage_("Номер корневого блока файла DATA некорректный", msError
			,"Таблица", tab->getname()
			,"Номер блока", block
		);
		return;
	}

	rootobj = (v8ob*)getblock_for_write(block, true);

	if(memcmp(rootobj->sig, SIG_OBJ, 8))
	{
		if(msreg) msreg->AddMessage_("Сигнатура корневого блока файла DATA некорректная.", msError
			,"Таблица", tab->getname()
			,"Номер блока (dec)", block
			,"Номер блока (hex)", tohex(block)
		);
		return;
	}

	l = rootobj->len;
	rl = tab->get_recordlen();
	if(l / rl * rl != l)
	{
		if(msreg) msreg->AddMessage_("Длина файла DATA не кратна длине одной записи.", msError
			,"Таблица", tab->getname()
			,"Номер блока (dec)", block
			,"Номер блока (hex)", tohex(block)
			,"Длина файла", l
			,"Длина записи", tab->get_recordlen()
		);
		return;
	}

	rectt = tab->get_record_template_test();

	for(i = 0, j = 0, k = 0; l; ++i)
	{
		cl = l > 0x1000 ? 0x1000 : l; // TODO pagesize

		if(!j)
		{
			a = rootobj->blocks[k];
			if(a < 5 || a >= length)
			{
				if(msreg) msreg->AddMessage_("Некорректный номер блока таблицы размещения файла DATA. Создана новая страница размещения", msWarning
					,"Таблица", tab->getname()
					,"Индекс страницы", k
					,"Номер блока", a
				);
				a = length;
				ca = (objtab*)getblock_for_write(a, false);
			}
			else ca = (objtab*)getblock_for_write(a, true);

			n = ((uint64_t)l + 0xfff) >> 12;
			if(n > 1023) n = 1023;
			m = ca->numblocks;

			if(n != m)
			{
				if(msreg) msreg->AddMessage_("Некорректное число блоков на странице размещения файла DATA. Исправлено.", msWarning
					,"Таблица", tab->getname()
					,"Номер блока", a
					,"Индекс страницы", k
					,"Неверное количество блоков", m
					,"Верное количество блоков", n
				);
				ca->numblocks = n;
			}

			++k;
		}

		d = ca->blocks[j];
		ok = true;
		if(d < 5 || d >= length)
		{
			if(msreg) msreg->AddMessage_("Некорректный номер страницы данных файла DATA.", msWarning
				,"Таблица", tab->getname()
				,"Номер блока", a
				,"Индекс страницы размещения", k - 1
				,"Индекс блока на странице", j
				,"Неверный номер страницы", d
			);
			ok = false;
		}
		if(ok)
		{
			ok = test_block_by_template(d, rectt, i, rl, cl);
			if(!ok)
			{
			if(msreg) msreg->AddMessage_("Cтраница данных файла DATA не подходит по шаблону.", msWarning
				,"Таблица", tab->getname()
				,"Номер блока", d
				,"Индекс страницы размещения", k - 1
				,"Индекс блока на странице", j
				,"Индекс страницы в файле DATA", i
			);
			}
		}

		if(!ok)
		{
			bk.clear();
			for(r = 5; r < length; ++r)
			{
				if(test_block_by_template(r, rectt, i, rl, cl)) bk.push_back(r);
			}
			if(bk.size() == 0)
			{
				if(msreg) msreg->AddMessage_("Не удалось найти подходящую страницу данных файла DATA по шаблону.", msError
					,"Таблица", tab->getname()
					,"Индекс страницы размещения", k - 1
					,"Индекс блока на странице", j
					,"Индекс страницы в файле DATA", i
				);
			}
			else if(bk.size() == 1)
			{
				d = bk[0];
				ca->blocks[j] = d;
				if(msreg) msreg->AddMessage_("Найдена подходящая страница данных файла DATA. Страница восстановлена", msInfo
					,"Таблица", tab->getname()
					,"Номер блока", d
					,"Индекс страницы размещения", k - 1
					,"Индекс блока на странице", j
					,"Индекс страницы в файле DATA", i
				);

			}
			else
			{
				s = "";
				for(d = 0; d < bk.size(); ++d)
				{
					if(d > 0) s += ", ";
					s += tohex(bk[d]);
				}
				if(msreg) msreg->AddMessage_("Найдено несколько подходящих страниц данных файла DATA.", msHint
					,"Таблица", tab->getname()
					,"Список подходящих блоков", s
					,"Индекс страницы размещения", k - 1
					,"Индекс блока на странице", j
					,"Индекс страницы в файле DATA", i
				);
			}
		}

		if(++j >= 1023) // TODO pagesize
		{
			j = 0;
			++a;
		}
		if(l < 0x1000) l = 0; // TODO pagesize
		else l-=0x1000; // TODO pagesize
	}


	delete[] rectt;

	flush();
}

//---------------------------------------------------------------------------
// Проверка блока таблицы по шаблону
bool T_1CD::test_block_by_template(uint32_t testblock, char* tt, uint32_t num, int32_t rlen, int32_t len)
{
	unsigned char b[0x1000]; // TODO pagesize
	bool ok;
	int32_t i, j;

	ok = true;
	fs->Seek((int64_t)testblock << 12, (TSeekOrigin)soFromBeginning);
	fs->ReadBuffer(b, len);

	if(!num)
	{
		if(b[0] != 1) return false;
		for(i = 1; i < 5; ++i) if(b[i] != 0) return false;
		j = 0;
		i = rlen;
	}
	else
	{
		i = (num << 12) / rlen;
		j = (num << 12) - rlen * i;
		i = 0;
	}
	for(; i < len; ++i)
	{
		if(tt[(j << 8) + b[i]] == 0) return false;
		if(++j >= rlen) j = 0;
	}
	for(i = len; i < 0x1000; ++i) if(b[i]) return false; // TODO pagesize
	return true;
}

#endif //#ifdef PublicRelease

//---------------------------------------------------------------------------
TableFiles* T_1CD::get_files_config()
{
	if(!_files_config) _files_config = new TableFiles(table_config);
	return _files_config;
}

//---------------------------------------------------------------------------
TableFiles* T_1CD::get_files_configsave()
{
	if(!_files_configsave) _files_configsave = new TableFiles(table_configsave);
	return _files_configsave;
}

//---------------------------------------------------------------------------
TableFiles* T_1CD::get_files_params()
{
	if(!_files_params) _files_params = new TableFiles(table_params);
	return _files_params;
}

//---------------------------------------------------------------------------
TableFiles* T_1CD::get_files_files()
{
	if(!_files_files) _files_files = new TableFiles(table_files);
	return _files_files;
}

//---------------------------------------------------------------------------
TableFiles* T_1CD::get_files_configcas()
{
	if(!_files_configcas) _files_configcas = new TableFiles(table_configcas);
	return _files_configcas;
}

//---------------------------------------------------------------------------
TableFiles* T_1CD::get_files_configcassave()
{
	if(!_files_configcassave) _files_configcassave = new TableFiles(table_configcassave);
	return _files_configcassave;
}

//---------------------------------------------------------------------------
bool T_1CD::save_config_ext(const String& _filename, const TGUID& uid, const String& hashname)
{
	ConfigStorageTableConfigCasSave* cs;
	bool res;

	cs = new ConfigStorageTableConfigCasSave(get_files_configcas(), get_files_configcassave(), uid, hashname);
	if(!cs->getready()) res = false;
	else res = cs->save_config(_filename);
	delete cs;
	return res;
}

//---------------------------------------------------------------------------
bool T_1CD::save_config_ext_db(const String& _filename, const String& hashname)
{
	ConfigStorageTableConfigCas* cs;
	bool res;

	cs = new ConfigStorageTableConfigCas(get_files_configcas(), hashname);
	if(!cs->getready()) res = false;
	else res = cs->save_config(_filename);
	delete cs;
	return res;

}

//---------------------------------------------------------------------------
void T_1CD::pagemapfill()
{
	if(pagemap) delete[] pagemap;
	pagemap = new pagemaprec[length];

	pagemap[0].type = pt_root;
	pagemap[1].type = pt_freeroot;
	pagemap[2].type = pt_rootfileroot;

}

//---------------------------------------------------------------------------
String T_1CD::pagemaprec_presentation(pagemaprec& pmr)
{
	switch(pmr.type)
	{
		case pt_lost: return String("потерянная страница");
		case pt_root: return String("корневая страница базы");
		case pt_freeroot: return String("корневая страница таблицы свободных блоков");
		case pt_freealloc: return String("страница размещения таблицы свободных блоков номер ") + pmr.number;
		case pt_free: return String("свободная страница номер ") + pmr.number;
		case pt_rootfileroot: return String("корневая страница корневого файла");
		case pt_rootfilealloc: return String("страница размещения корневого файла номер ") + pmr.number;
		case pt_rootfile: return String("страница данных корневого файла номер ") + pmr.number;
		case pt_descrroot: return String("корневая страница файла descr таблицы ") + tables[pmr.tab]->getname();
		case pt_descralloc: return String("страница размещения файла descr таблицы ") + tables[pmr.tab]->getname() + " номер " + pmr.number;
		case pt_descr: return String("страница данных файла descr таблицы ") + tables[pmr.tab]->getname() + " номер " + pmr.number;
		case pt_dataroot: return String("корневая страница файла data таблицы ") + tables[pmr.tab]->getname();
		case pt_dataalloc: return String("страница размещения файла data таблицы ") + tables[pmr.tab]->getname() + " номер " + pmr.number;
		case pt_data: return String("страница данных файла data таблицы ") + tables[pmr.tab]->getname() + " номер " + pmr.number;
		case pt_indexroot: return String("корневая страница файла index таблицы ") + tables[pmr.tab]->getname();
		case pt_indexalloc: return String("страница размещения файла index таблицы ") + tables[pmr.tab]->getname() + " номер " + pmr.number;
		case pt_index: return String("страница данных файла index таблицы ") + tables[pmr.tab]->getname() + " номер " + pmr.number;
		case pt_blobroot: return String("корневая страница файла blob таблицы ") + tables[pmr.tab]->getname();
		case pt_bloballoc: return String("страница размещения файла blob таблицы ") + tables[pmr.tab]->getname() + " номер " + pmr.number;
		case pt_blob: return String("страница данных файла blob таблицы ") + tables[pmr.tab]->getname() + " номер " + pmr.number;

		default: return String("??? неизвестный тип страницы ???");
	}
}

std::map<String,table_file*> &TableFiles::files()
{
	return allfiles;
}

//---------------------------------------------------------------------------


