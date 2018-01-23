#ifndef V8FILEH
#define V8FILEH

#include <System.Classes.hpp>
#include <vector>
#include <set>

#include "V8Catalog.h"
#include "TV8FileStream.h"
#include "../Parse_tree.h"

class v8catalog;
class TV8FileStream;

enum class FileIsCatalog {
	unknown,
	yes,
	no
};

class v8file {
public:
		v8file(v8catalog* _parent, const String& _name, v8file* _previous, int _start_data, int _start_header, int64_t* _time_create, int64_t* _time_modify);

		~v8file();
		bool IsCatalog();
		v8catalog* GetCatalog();
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

		tree* get_tree();
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

		inline bool try_open() { return (!is_opened) ? Open() : true; }
};

#endif
