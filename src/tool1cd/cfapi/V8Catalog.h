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
#ifndef V8CATALOGH
#define V8CATALOGH

#include <boost/filesystem.hpp>
#include <map>

#include "V8File.h"
#include "APIcfBase.h"
#include "../SystemClasses/TStream.hpp"
#include "../SystemClasses/System.Classes.hpp"

class V8File;

class V8Catalog {
public:
	typedef std::map<std::string,V8File*> V8Files;

	explicit V8Catalog(V8File* f);   // создать каталог из файла
	explicit V8Catalog(const std::string &name); // создать каталог из физического файла (cf, epf, erf, hbk, cfu)
	V8Catalog(const std::string &name, bool _zipped); // создать каталог из физического файла (cf, epf, erf, hbk, cfu)
	V8Catalog(TStream* stream, bool _zipped, bool leave_stream = false); // создать каталог из потока

	bool is_catalog() const;

	~V8Catalog();

	V8File* get_file(const std::string &FileName);
	V8File* get_first();
	V8File* createFile(const std::string &FileName, bool _selfzipped = false); // CreateFile в win64 определяется как CreateFileW, пришлось заменить на маленькую букву
	V8Catalog* CreateCatalog(const std::string &FileName, bool _selfzipped = false);
	void delete_file(const std::string &FileName);
	V8Catalog* get_parent_catalog();
	V8File* get_self_file();
	void SaveToDir(const boost::filesystem::path &dir) const;
	bool isOpen() const;
	void Flush();
	void HalfClose();
	void HalfOpen(const std::string &name);

	V8File* get_first_file();
	void first_file(V8File *value);

	V8File* get_last_file();
	void last_file(V8File* value);

	V8Files& v8files();

	inline void Acquire() { Lock->Acquire(); }
	inline void Release() { Lock->Release(); }

	std::string get_full_name();

	bool data_empty() const;
	void erase_data();

	bool is_fatmodified() const;
	void fatmodified(const bool value);

	void free_block(int start);

	int write_block(TStream* block, int start, bool use_page_size, int len = -1);       // возвращает адрес начала блока
	int write_datablock(TStream* block, int start, bool _zipped = false, int len = -1); // возвращает адрес начала блока

	TStream* read_datablock(int start);
	int64_t get_nextblock(int64_t start);

	bool is_destructed() const;

private:
	System::Classes::TCriticalSection *Lock;
	V8File* file;  // файл, которым является каталог. Для корневого каталога NULL
	TStream* data; // поток каталога. Если file не NULL (каталог не корневой), совпадает с file->data
	TStream* cfu;  // поток файла cfu. Существует только при is_cfu == true
	void initialize();
	V8File* first; // первый файл в каталоге
	V8File* last;  // последний файл в каталоге
	std::map<std::string,V8File*> files; // Соответствие имен и файлов

	stFileHeader file_header;

	bool zipped;     // признак зазипованности файлов каталога
	bool is_cfu;     // признак файла cfu (файл запакован deflate'ом)
	mutable bool iscatalog;
	mutable bool iscatalogdefined;

	bool _fatmodified;
	bool is_emptymodified;
	bool is_modified;

	bool _destructed; // признак, что работает деструктор
	bool flushed;       // признак, что происходит сброс
	bool leave_data;    // признак, что не нужно удалять основной поток (data) при уничтожении объекта
};

#endif
