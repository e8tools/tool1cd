#ifndef V8CATALOGH
#define V8CATALOGH

#include <map>

#include "V8File.h"

class v8file;

//---------------------------------------------------------------------------
struct fat_item{
	int32_t header_start;
	int32_t data_start;
	int32_t ff;            // всегда 7fffffff
};

class v8catalog {
public:
		explicit v8catalog(v8file* f);   // создать каталог из файла
		explicit v8catalog(String name); // создать каталог из физического файла (cf, epf, erf, hbk, cfu)
		v8catalog(String name, bool _zipped); // создать каталог из физического файла (cf, epf, erf, hbk, cfu)
		v8catalog(TStream* stream, bool _zipped, bool leave_stream = false); // создать каталог из потока

		bool IsCatalog();

		~v8catalog();

		v8file* GetFile(const String& FileName);
		v8file* GetFirst();
		v8file* createFile(const String& FileName, bool _selfzipped = false); // CreateFile в win64 определяется как CreateFileW, пришлось заменить на маленькую букву
		v8catalog* CreateCatalog(const String& FileName, bool _selfzipped = false);
		void DeleteFile(const String& FileName);
		v8catalog* GetParentCatalog();
		v8file* GetSelfFile();
		void SaveToDir(String DirName);
		bool isOpen();
		void Flush();
		void HalfClose();
		void HalfOpen(const String& name);

		v8file* get_first_file();
		void first_file(v8file *value);

		v8file* get_last_file();
		void last_file(v8file* value);
private:
		friend v8file;
		TCriticalSection *Lock;
		v8file* file;  // файл, которым является каталог. Для корневого каталога NULL
		TStream* data; // поток каталога. Если file не NULL (каталог не корневой), совпадает с file->data
		TStream* cfu;  // поток файла cfu. Существует только при is_cfu == true
		void initialize();
		v8file* first; // первый файл в каталоге
		v8file* last;  // последний файл в каталоге
		std::map<String,v8file*> files; // Соответствие имен и файлов
		int64_t start_empty; // начало первого пустого блока
		int page_size;   // размер страницы по умолчанию
		int version;     // версия
		bool zipped;     // признак зазипованности файлов каталога
		bool is_cfu;     // признак файла cfu (файл запакован deflate'ом)
		bool iscatalog;
		bool iscatalogdefined;

		bool is_fatmodified;
		bool is_emptymodified;
		bool is_modified;

		void free_block(int start);

		int write_block(TStream* block, int start, bool use_page_size, int len = -1);       // возвращает адрес начала блока
		int write_datablock(TStream* block, int start, bool _zipped = false, int len = -1); // возвращает адрес начала блока

		TStream* read_datablock(int start);
		int64_t get_nextblock(int64_t start);

		bool is_destructed; // признак, что работает деструктор
		bool flushed;       // признак, что происходит сброс
		bool leave_data;    // признак, что не нужно удалять основной поток (data) при уничтожении объекта
};

#endif
