/*
 *
 *
 *
 *
 */

#include "Index.h"

extern Registrator msreg_g;

//---------------------------------------------------------------------------
index::index(Table* _base)
{
	tbase       = _base;
    is_primary  = false;
	num_records = 0;
	records     = nullptr;
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
	msreg_g.Status(readindex);

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
				if(curindex % 10000 == 0) msreg_g.Status(readindex + curindex);
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
	msreg_g.Status("");
}

//---------------------------------------------------------------------------
#ifndef PublicRelease
void index::dump_recursive(v8object* file_index, TFileStream* f, int32_t level, uint64_t curblock)
{
	unsigned char bf[3];
	char* buf;
	char* rbuf;
	uint32_t curlen;

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
		msreg_g.AddError("Попытка распаковки страницы индекса не являющейся листом.",
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
				msreg_g.AddError("Ошибка упаковки индексов на странице-листе. Индекс не уникальный либо неверно отсортирован.",
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
					msreg_g.AddError("Ошибка записи индекса. Индекс уже существует.",
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
					msreg_g.AddError("Ошибка записи индекса. Индекс уже существует.",
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


