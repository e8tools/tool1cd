
#ifndef APIcfBaseH
#define APIcfBaseH

#include <System.Classes.hpp>
#include <map>
#include <set>
#include <limits>
#include <vector>
#include <string>

#include "UZLib.h"

#pragma comment (lib, "zlibstatic.lib")


	const char str_cfu[] = ".cfu";
	const char str_cfe[] = ".cfe";
	const char str_cf[]  = ".cf";
	const char str_epf[] = ".epf";
	const char str_erf[] = ".erf";
	const char str_backslash[] = "\\";

// шаблон заголовка блока
const char _BLOCK_HEADER_TEMPLATE[] = "\r\n00000000 00000000 00000000 \r\n";
const char _EMPTY_CATALOG_TEMPLATE[16] = {'\xff','\xff','\xff','\x7f',0,2,0,0,0,0,0,0,0,0,0,0};

const int32_t LAST_BLOCK = std::numeric_limits<int>::max();
const uint32_t BLOCK_HEADER_LEN = 32U;
const uint32_t CATALOG_HEADER_LEN = 16U;

const int64_t EPOCH_START_WIN = 504911232000000;

//---------------------------------------------------------------------------
struct v8header_struct{
	int64_t time_create;
	int64_t time_modify;
	int32_t zero;
};

//---------------------------------------------------------------------------
struct fat_item{
	int32_t header_start;
	int32_t data_start;
	int32_t ff;            // всегда 7fffffff
};

//---------------------------------------------------------------------------
struct catalog_header{
	int32_t start_empty; // начало первого пустого блока
	int32_t page_size;   // размер страницы по умолчанию
	int32_t version;     // версия
	int32_t zero;        // всегда ноль?
};

//---------------------------------------------------------------------------
class v8catalog;
class TV8FileStream;

//---------------------------------------------------------------------------
enum class FileIsCatalog {
	unknown,
	yes,
	no
};

//---------------------------------------------------------------------------
class v8file{
  private:
	friend v8catalog;
	friend TV8FileStream;
	String name;
	int64_t time_create;
	int64_t time_modify;
	TCriticalSection *Lock;
	TStream* data;
	v8catalog* parent;
	FileIsCatalog iscatalog;

	v8catalog* self;        // указатель на каталог, если файл является каталогом

	v8file* next;           // следующий файл в каталоге

	v8file* previous;       // предыдущий файл в каталоге

	bool is_opened;         // признак открытого файла (инициализирован поток data)

	int start_data;         // начало блока данных файла в каталоге (0 означает, что файл в каталоге не записан)
	int start_header;       // начало блока заголовка файла в каталоге
	bool is_datamodified;   // признак модифицированности данных файла (требуется запись в каталог при закрытии)
	bool is_headermodified; // признак модифицированности заголовка файла (требуется запись в каталог при закрытии)

	bool is_destructed;     // признак, что работает деструктор
	bool flushed;           // признак, что происходит сброс
	bool selfzipped;        // Признак, что файл является запакованным независимо от признака zipped каталога
	std::set<TV8FileStream*> streams;

  public:
	v8file(v8catalog* _parent, const String& _name, v8file* _previous, int _start_data, int _start_header, int64_t* _time_create, int64_t* _time_modify);

	~v8file();
	bool IsCatalog();
	v8catalog* GetCatalog();
	int64_t GetFileLength();
	int64_t GetFileLength64();

	int64_t Read(void* Buffer, int Start, int Length);
	int64_t Read(std::vector<System::t::Byte> Buffer, int Start, int Length);

	int64_t Write(const void* Buffer, int Start, int Length);                           // дозапись/перезапись частично
	int64_t Write(std::vector<System::t::Byte> Buffer, int Start, int Length); // дозапись/перезапись частично
	int64_t Write(const void* Buffer, int Length);                                      // перезапись целиком
	int64_t Write(TStream* Stream, int Start, int Length);                              // дозапись/перезапись частично
	int64_t Write(TStream* Stream);                                                     // перезапись целиком

	String GetFileName();
	String GetFullName();

	void SetFileName(const String& _name);
	v8catalog* GetParentCatalog();
	void DeleteFile();
	v8file* GetNext();
	bool Open();
	void Close();

	int64_t WriteAndClose(TStream* Stream, int Length = -1); // перезапись целиком и закрытие файла (для экономии памяти не используется data файла)

	void GetTimeCreate(System::FILETIME* ft);
	void GetTimeModify(System::FILETIME* ft);
	void SetTimeCreate(System::FILETIME* ft);
	void SetTimeModify(System::FILETIME* ft);
	void SaveToFile(const String& FileName);
	void SaveToStream(TStream* stream);
	TV8FileStream* get_stream(bool own = false);
	void Flush();
};

//---------------------------------------------------------------------------
class v8catalog{
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

  public:

	v8catalog(v8file* f);   // создать каталог из файла
	v8catalog(String name); // создать каталог из физического файла (cf, epf, erf, hbk, cfu)
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
};

//---------------------------------------------------------------------------
class TV8FileStream : public TStream
{
protected:
	v8file* file;
	bool own;
	int64_t pos;
public:
	TV8FileStream(v8file* f, bool ownfile = false);
	virtual ~TV8FileStream();
	virtual int64_t Read(void *Buffer, int64_t Count) override;
	virtual int Read(std::vector<System::t::Byte> Buffer, int Offset, int Count);
	virtual int64_t Write(const void *Buffer, int64_t Count) override;
	virtual int Write(const std::vector<System::t::Byte> Buffer, int Offset, int Count);
	virtual int Seek(int Offset, System::Word Origin);
	virtual int64_t Seek(const int64_t Offset, TSeekOrigin Origin) override;
};

#endif

