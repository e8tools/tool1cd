/*
    Tool1CD library provides access to 1CD database files.
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of Tool1CD Library.

    Tool1CD Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tool1CD Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Tool1CD Library.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "MemBlock.h"
#include "Constants.h"
#include "SystemClasses/GetTickCount.hpp"

std::shared_ptr<MemBlock> MemBlock::create(std::shared_ptr<TFileStream> &fs, uint32_t page_size, uint32_t block_num,
bool for_write, bool read)
{
	auto buf = new char[page_size];
	if(for_write)
	{
		uint32_t fnumblocks = fs->GetSize() / page_size;
		if(fnumblocks <= block_num)
		{
			memset(buf, 0, page_size);
			fs->Seek((int64_t)block_num * page_size, (TSeekOrigin)soFromBeginning);
			fs->WriteBuffer(buf, page_size);
			fs->Seek(12, (TSeekOrigin)soFromBeginning);
			fs->WriteBuffer(&block_num, sizeof(uint32_t));
		}
		else
		{
			if(read)
			{
				fs->Seek((int64_t)block_num * page_size, (TSeekOrigin)soFromBeginning);
				fs->ReadBuffer(buf, page_size);
			}
			else memset(buf, 0, page_size);
		}
	}
	else
	{
		fs->Seek((int64_t)block_num * page_size, (TSeekOrigin)soFromBeginning);
		fs->ReadBuffer(buf, page_size);
	}

	std::shared_ptr<MemBlock> block(new MemBlock(page_size, block_num));
	std::copy(buf, buf + page_size, std::begin(block->data));
	return block;

}

//---------------------------------------------------------------------------
MemBlockManager::MemBlockManager(std::shared_ptr<TFileStream> &fs)
	: fs(fs)
{
}

//---------------------------------------------------------------------------
void MemBlockManager::garbage(bool aggressive)
{
	// TODO: блокировка на memblocks
	uint32_t curt = GetTickCount();
	for (auto &block : memblocks) {

		if (!block) {
			continue;
		}
		if (block->is_changed()) {
			continue;
		}

		auto when_to_kill = block->get_last_data_get() + LIVE_CASH * 60 * 1000;
		if (aggressive || curt >= when_to_kill) {
			block.reset();
		}
	}
}

//---------------------------------------------------------------------------
char* MemBlockManager::get_block(uint32_t _numblock)
{
	if(_numblock >= memblocks.size()) return nullptr;
	if (!memblocks[_numblock]) {
		memblocks[_numblock] = MemBlock::create(fs, pagesize, _numblock, false, true);
	}
	return memblocks[_numblock]->get_block(false);
}

//---------------------------------------------------------------------------
char* MemBlockManager::get_block_for_write(uint32_t _numblock, bool read)
{
	if (_numblock > memblocks.size()) {
		return nullptr;
	}

	if (_numblock == memblocks.size()) {
		add_block();
	}

	if(!memblocks[_numblock]) {
		memblocks[_numblock] = MemBlock::create(fs, pagesize, _numblock, true, read);
	}
	return memblocks[_numblock]->get_block(true);
}

//---------------------------------------------------------------------------
void MemBlockManager::create_memblocks(uint64_t _numblocks)
{
	memblocks.clear();
	memblocks.resize(_numblocks);
	memset(memblocks.data(), 0, memblocks.size() * sizeof(MemBlock *));
}

//---------------------------------------------------------------------------
void MemBlockManager::delete_memblocks()
{
	memblocks.clear();
}

//---------------------------------------------------------------------------
void MemBlockManager::add_block()
{
	memblocks.push_back(nullptr);
}

//---------------------------------------------------------------------------
uint64_t MemBlockManager::get_numblocks() const
{
	return memblocks.size();
}

//---------------------------------------------------------------------------
void MemBlockManager::flush()
{
	for (auto &block : memblocks) {
		block->write(fs);
	}
}

//---------------------------------------------------------------------------
void MemBlock::write(std::shared_ptr<TFileStream> &fs)
{
	if (!is_changed()) {
		return;
	}
	fs->Seek((int64_t)block_num * data.size(), (TSeekOrigin)soFromBeginning);
	fs->WriteBuffer(data.data(), data.size());
	changed = false;
}

uint32_t MemBlockManager::get_page_size() const
{
	return pagesize;
}

void MemBlockManager::set_page_size(const uint32_t value)
{
	pagesize = value;
}

uint32_t MemBlockManager::get_maxcount() const
{
	return maxcount;
}

void MemBlockManager::set_maxcount(const uint32_t value)
{
	maxcount = value;
}

MemBlockManager::MemBlockManager()
{

}

char *MemBlock::get_block(bool for_write)
{
	if (for_write) {
		changed = true;
	}
	last_data_get = GetTickCount();
	return data.data();
}

const char *MemBlock::get_block() const
{
	last_data_get = GetTickCount();
	return data.data();
}

uint32_t MemBlock::get_last_data_get() const
{
	return last_data_get;
}

bool MemBlock::is_changed() const
{
	return changed;
}
