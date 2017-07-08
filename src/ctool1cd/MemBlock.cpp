/*
 * MemBlock.cpp
 *
 *  Created on: 24 июн. 2017 г.
 *
 */

#include "MemBlock.h"

MemBlock* MemBlock::first = nullptr;
MemBlock* MemBlock::last = nullptr;
uint32_t MemBlock::count = 0;
uint32_t MemBlock::maxcount;
MemBlock** MemBlock::memblocks = nullptr;

uint64_t MemBlock::numblocks = 0;

uint64_t MemBlock::array_numblocks = 0;

uint32_t MemBlock::delta = 128;
uint32_t MemBlock::pagesize = DEFAULT_PAGE_SIZE;

//---------------------------------------------------------------------------
MemBlock::MemBlock(TFileStream* fs, uint32_t _numblock, bool for_write, bool read)
{
	numblock = _numblock;
	lastdataget = 0;
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
MemBlock::~MemBlock()
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
char* MemBlock::getblock(bool for_write)
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
void MemBlock::garbage()
{
	uint32_t curt = GetTickCount();
	while(MemBlock::first)
	{
		if(curt - first->lastdataget > LIVE_CASH * 60 * 1000) delete MemBlock::first;
		else break;
	}
}

//---------------------------------------------------------------------------
char* MemBlock::getblock(TFileStream* fs, uint32_t _numblock)
{
	if(_numblock >= numblocks) return NULL;
	if(!memblocks[_numblock]) new MemBlock(fs, _numblock, false, true);
	return memblocks[_numblock]->getblock(false);
}

//---------------------------------------------------------------------------
char* MemBlock::getblock_for_write(TFileStream* fs, uint32_t _numblock, bool read)
{
	if(_numblock > numblocks) return NULL;
	if(_numblock == numblocks) add_block();
	if(!memblocks[_numblock]) new MemBlock(fs, _numblock, true, read);
	else memblocks[_numblock]->is_changed = true;
	return memblocks[_numblock]->getblock(true);
}

//---------------------------------------------------------------------------
void MemBlock::create_memblocks(uint64_t _numblocks)
{
	numblocks = _numblocks;
	array_numblocks = (numblocks / delta + 1) * delta;
	memblocks = new MemBlock*[array_numblocks];
	memset(memblocks, 0, array_numblocks * sizeof(MemBlock *));
}

//---------------------------------------------------------------------------
void MemBlock::delete_memblocks()
{
	//while(first) delete first;
	while (MemBlock::first) delete MemBlock::first;
	delete[] memblocks;
	numblocks = 0;
	array_numblocks = 0;
}

//---------------------------------------------------------------------------
void MemBlock::add_block()
{
	MemBlock** mb;

	if(numblocks < array_numblocks) memblocks[numblocks++] = NULL;
	else
	{
		mb = new MemBlock*[array_numblocks + delta];
		for(unsigned i = 0; i < array_numblocks; i++) mb[i] = memblocks[i];
		for(unsigned i = array_numblocks; i < array_numblocks + delta; i++) mb[i] = NULL;
		array_numblocks += delta;
		delete[] memblocks;
		memblocks = mb;
	}
}

//---------------------------------------------------------------------------
uint64_t MemBlock::get_numblocks()
{
	return numblocks;
}

//---------------------------------------------------------------------------
void MemBlock::flush()
{
	MemBlock* cur;
	for(cur = first; cur; cur = cur->next) if(cur->is_changed) cur->write();
}

//---------------------------------------------------------------------------
void MemBlock::write()
{
	if(!is_changed) return;
	file->Seek((int64_t)numblock * pagesize, (TSeekOrigin)soFromBeginning);
	file->WriteBuffer(buf, pagesize);
	is_changed = false;
}




