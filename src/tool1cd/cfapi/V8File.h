#ifndef V8FILEH
#define V8FILEH

#include <System.Classes.hpp>
#include <boost/filesystem.hpp>
#include <vector>
#include <set>

#include "V8Catalog.h"
#include "TV8FileStream.h"
#include "../Parse_tree.h"
#include "V8Time.h"

class V8Catalog;
class TV8FileStream;

enum class FileIsCatalog {
	unknown,
	yes,
	no
};

class V8File {
public:
	typedef std::set<TV8FileStream*> TV8FileStreams;

	V8File(V8Catalog *_parent, const std::string &_name, V8File *_previous, int _start_data, int _start_header,
		   int64_t set_time_create, int64_t set_time_modify);

	~V8File();
	bool is_catalog();
	V8Catalog* get_catalog();
	int64_t get_file_length();

	int64_t read(void* Buffer, int Start, int Length);
	int64_t read(std::vector<uint8_t> Buffer, int Start, int Length);

	int64_t write(const void* Buffer, int Start, int Length);                           // дозапись/перезапись частично
	int64_t write(std::vector<uint8_t> Buffer, int Start, int Length); // дозапись/перезапись частично
	int64_t write(const void* Buffer, int Length);                                      // перезапись целиком
	int64_t write(TStream* Stream, int Start, int Length);                              // дозапись/перезапись частично
	int64_t write(TStream* Stream);                                                     // перезапись целиком

	std::string get_file_name();
	void set_file_name(const std::string &_name);
	std::string get_full_name();

	V8Catalog* get_parent_catalog();
	void delete_file();
	V8File* get_next();
	bool open();
	void close();

	int64_t write_and_close(TStream* Stream, int Length = -1); // перезапись целиком и закрытие файла (для экономии памяти не используется data файла)

	System::FILETIME get_time_create() const;
	void set_time_create(const System::FILETIME &file_time);
	void set_time_create(const V8Time &time);

	System::FILETIME get_time_modify() const;
	void set_time_modify(const System::FILETIME &file_time);
	void set_time_modify(const V8Time &time);

	void save_to_file(const boost::filesystem::path &FileName);
	void save_to_stream(TStream* stream);
	TV8FileStream* get_stream(bool own = false);
	void flush();

	tree* get_tree();

	TCriticalSection* get_lock();
	TStream* get_data();
	int get_start_data() const;
	int get_start_header() const;

	bool get_data_modified() const;
	void set_data_modified(const bool value);

	bool get_destructed() const;
	void set_destructed(const bool value);

	bool get_self_zipped() const;
	void set_self_zipped(const bool value);

	TV8FileStreams& streams();

private:
	std::string name;

	V8Time _time_create;
	V8Time _time_modify;
	TCriticalSection *Lock;
	TStream* data;
	V8Catalog* parent;
	FileIsCatalog iscatalog;

	V8Catalog* self;        // указатель на каталог, если файл является каталогом
	V8File* next;           // следующий файл в каталоге
	V8File* previous;       // предыдущий файл в каталоге
	bool is_opened;         // признак открытого файла (инициализирован поток data)
	int start_data;         // начало блока данных файла в каталоге (0 означает, что файл в каталоге не записан)
	int start_header;       // начало блока заголовка файла в каталоге
	bool _datamodified;   // признак модифицированности данных файла (требуется запись в каталог при закрытии)
	bool is_headermodified; // признак модифицированности заголовка файла (требуется запись в каталог при закрытии)
	bool _destructed;     // признак, что работает деструктор
	bool flushed;           // признак, что происходит сброс
	bool _selfzipped;        // Признак, что файл является запакованным независимо от признака zipped каталога
	TV8FileStreams _streams;

	inline bool try_open() { return (!is_opened) ? open() : true; }
};

#endif
