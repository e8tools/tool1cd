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

	V8File(V8Catalog* _parent, const String& _name, V8File* _previous, int _start_data, int _start_header, int64_t time_create, int64_t time_modify);

	~V8File();
	bool IsCatalog();
	V8Catalog* GetCatalog();
	int64_t GetFileLength();

	int64_t Read(void* Buffer, int Start, int Length);
	int64_t Read(std::vector<uint8_t> Buffer, int Start, int Length);

	int64_t Write(const void* Buffer, int Start, int Length);                           // дозапись/перезапись частично
	int64_t Write(std::vector<uint8_t> Buffer, int Start, int Length); // дозапись/перезапись частично
	int64_t Write(const void* Buffer, int Length);                                      // перезапись целиком
	int64_t Write(TStream* Stream, int Start, int Length);                              // дозапись/перезапись частично
	int64_t Write(TStream* Stream);                                                     // перезапись целиком

	String GetFileName();
	String GetFullName();

	void SetFileName(const String& _name);
	V8Catalog* GetParentCatalog();
	void DeleteFile();
	V8File* GetNext();
	bool Open();
	void Close();

	int64_t WriteAndClose(TStream* Stream, int Length = -1); // перезапись целиком и закрытие файла (для экономии памяти не используется data файла)

	System::FILETIME get_time_create() const;
	void time_create(const System::FILETIME &file_time);
	void time_create(const V8Time &time);

	System::FILETIME get_time_modify() const;
	void time_modify(const System::FILETIME &file_time);
	void time_modify(const V8Time &time);

	void SaveToFile(const boost::filesystem::path &FileName);
	void SaveToStream(TStream* stream);
	TV8FileStream* get_stream(bool own = false);
	void Flush();

	tree* get_tree();

	TCriticalSection* get_lock();
	TStream* get_data();
	int get_start_data() const;
	int get_start_header() const;

	bool is_datamodified() const;
	void datamodified(const bool value);

	bool is_destructed() const;
	void destructed(const bool value);

	bool is_self_zipped() const;
	void self_zipped(const bool value);

	TV8FileStreams& streams();

private:
	String name;

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

	inline bool try_open() { return (!is_opened) ? Open() : true; }
};

#endif
