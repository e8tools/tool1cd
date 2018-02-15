#ifndef V8CATALOGH
#define V8CATALOGH

#include <map>

#include "V8File.h"
#include <boost/filesystem.hpp>

class V8File;

//---------------------------------------------------------------------------
struct fat_item{
	int32_t header_start;
	int32_t data_start;
	int32_t ff;            // всегда 7fffffff
};

class V8Catalog {
public:
	typedef std::map<String,V8File*> V8Files;

	explicit V8Catalog(V8File* f);   // создать каталог из файла
	explicit V8Catalog(String name); // создать каталог из физического файла (cf, epf, erf, hbk, cfu)
	V8Catalog(String name, bool _zipped); // создать каталог из физического файла (cf, epf, erf, hbk, cfu)
	V8Catalog(TStream* stream, bool _zipped, bool leave_stream = false); // создать каталог из потока

	bool IsCatalog() const;

	~V8Catalog();

	V8File* GetFile(const String& FileName);
	V8File* GetFirst();
	V8File* createFile(const String& FileName, bool _selfzipped = false); // CreateFile в win64 определяется как CreateFileW, пришлось заменить на маленькую букву
	V8Catalog* CreateCatalog(const String& FileName, bool _selfzipped = false);
	void DeleteFile(const String& FileName);
	V8Catalog* GetParentCatalog();
	V8File* GetSelfFile();
	void SaveToDir(const boost::filesystem::path &dir) const;
	bool isOpen() const;
	void Flush();
	void HalfClose();
	void HalfOpen(const String& name);

	V8File* get_first_file();
	void first_file(V8File *value);

	V8File* get_last_file();
	void last_file(V8File* value);

	V8Files& v8files();

	inline void Acquire() { Lock->Acquire(); }
	inline void Release() { Lock->Release(); }

	String get_full_name();

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
	TCriticalSection *Lock;
	V8File* file;  // файл, которым является каталог. Для корневого каталога NULL
	TStream* data; // поток каталога. Если file не NULL (каталог не корневой), совпадает с file->data
	TStream* cfu;  // поток файла cfu. Существует только при is_cfu == true
	void initialize();
	V8File* first; // первый файл в каталоге
	V8File* last;  // последний файл в каталоге
	std::map<String,V8File*> files; // Соответствие имен и файлов
	int64_t start_empty; // начало первого пустого блока
	int page_size;   // размер страницы по умолчанию
	int version;     // версия
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
