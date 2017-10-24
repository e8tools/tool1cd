#ifndef V8OBJECT_H
#define V8OBJECT_H

#include <System.Classes.hpp>
#include <climits>

#include "MemBlock.h"
#include "Class_1CD.h"
#include "db_ver.h"

#pragma pack(push)
#pragma pack(1)

struct _version_rec
{
	uint32_t version_1; // версия реструктуризации
	uint32_t version_2; // версия изменения
};

struct _version
{
	uint32_t version_1; // версия реструктуризации
	uint32_t version_2; // версия изменения
	uint32_t version_3; // версия изменения 2
};

// Структура страницы размещения уровня 1 версий от 8.3.8
struct objtab838
{
	uint32_t blocks[1]; // реальное количество блоков зависит от размера страницы (pagesize)
};

// структура заголовочной страницы файла данных или файла свободных страниц
struct v8ob
{
	char sig[8]; // сигнатура SIG_OBJ
	uint32_t len; // длина файла
	_version version;
	uint32_t blocks[1018];
};

// структура заголовочной страницы файла данных начиная с версии 8.3.8
struct v838ob_data
{
	unsigned char sig[2]; // сигнатура 0x1C 0xFD (1C File Data?)
	int16_t fatlevel; // уровень таблицы размещения (0x0000 - в таблице blocks номера страниц с данными, 0x0001 - в таблице blocks номера страниц с таблицами размещения второго уровня, в которых уже, в свою очередь, находятся номера страниц с данными)
	_version version;
	uint64_t len; // длина файла
	uint32_t blocks[1]; // Реальная длина массива зависит от размера страницы и равна pagesize/4-6 (от это 1018 для 4К до 16378 для 64К)
};

// структура заголовочной страницы файла свободных страниц начиная с версии 8.3.8
struct v838ob_free
{
	unsigned char sig[2]; // сигнатура 0x1C 0xFF (1C File Free?)
	int16_t fatlevel; // 0x0000 пока! но может ... уровень таблицы размещения (0x0000 - в таблице blocks номера страниц с данными, 0x0001 - в таблице blocks номера страниц с таблицами размещения второго уровня, в которых уже, в свою очередь, находятся номера страниц с данными)
	uint32_t version; // ??? предположительно...
	uint32_t blocks[1]; // Реальная длина массива зависит от размера страницы и равна pagesize/4-6 (от это 1018 для 4К до 16378 для 64К)
};

#pragma pack(pop)

class T_1CD;
class Table;

// типы внутренних файлов
enum class v8objtype
{
	unknown = 0, // тип неизвестен
	data80  = 1, // файл данных формата 8.0 (до 8.2.14 включительно)
	free80  = 2, // файл свободных страниц формата 8.0 (до 8.2.14 включительно)
	data838 = 3, // файл данных формата 8.3.8
	free838 = 4  // файл свободных страниц формата 8.3.8
};

class v8object
{
friend Table;
friend T_1CD;
public:
	v8object(T_1CD* _base, int32_t blockNum); // конструктор существующего объекта
	v8object(T_1CD* _base); // конструктор нового (еще не существующего) объекта
	~v8object();
	char* getdata(); // чтение всего объекта целиком, поддерживает кеширование объектов. Буфер принадлежит объекту
	char* getdata(void* buf, uint64_t _start, uint64_t _length); // чтение кусочка объекта, поддерживает кеширование блоков. Буфер не принадлежит объекту
	bool setdata(const void* buf, uint64_t _start, uint64_t _length); // запись кусочка объекта, поддерживает кеширование блоков.
	bool setdata(const void* buf, uint64_t _length); // запись объекта целиком, поддерживает кеширование блоков.
	bool setdata(TStream* stream); // записывает поток целиком в объект, поддерживает кеширование блоков.
	bool setdata(TStream* stream, uint64_t _start, uint64_t _length); // запись части потока в объект, поддерживает кеширование блоков.
	uint64_t getlen();
	void savetofile(String filename);
	void set_lockinmemory(bool _lock);
	static void garbage();
	uint64_t get_fileoffset(uint64_t offset); // получить физическое смещение в файле по смещению в объекте
	void set_block_as_free(uint32_t block_number); // пометить блок как свободный
	uint32_t get_free_block(); // получить номер свободного блока (и пометить как занятый)
	void get_version_rec_and_increase(_version* ver); // получает версию очередной записи и увеличивает сохраненную версию объекта
	void get_version(_version* ver); // получает сохраненную версию объекта
	void write_new_version(); // записывает новую версию объекта
	static v8object* get_first();
	static v8object* get_last();
	v8object* get_next();
	uint32_t get_block_number();
	TStream* readBlob(TStream* _str, uint32_t _startblock, uint32_t _length = UINT_MAX, bool rewrite = true);
private:
	T_1CD* base;

	uint64_t len;               // длина объекта. Для типа таблицы свободных страниц - количество свободных блоков
	_version version;           // текущая версия объекта
	_version_rec version_rec;   // текущая версия записи
	bool new_version_recorded;  // признак, что новая версия объекта записана
	v8objtype type;             // тип и формат файла
	int32_t fatlevel;           // Количество промежуточных уровней в таблице размещения
	uint64_t numblocks;         // кол-во страниц в корневой таблице размещения объекта
	uint32_t real_numblocks;    // реальное кол-во страниц в корневой таблице (только для файлов свободных страниц, может быть больше numblocks)
	uint32_t* blocks;           // таблица страниц корневой таблицы размещения объекта (т.е. уровня 0)
	uint32_t block;             // номер блока объекта
	char* data;                 // данные, представляемые объектом, NULL если не прочитаны или len = 0

	static v8object* first;
	static v8object* last;
	v8object* next;
	v8object* prev;
	uint32_t lastdataget; // время (Windows time, в миллисекундах) последнего обращения к данным объекта (data)
	bool lockinmemory;

	void set_len(uint64_t _len); // установка новой длины объекта

	void init();
	void init(T_1CD* _base, int32_t blockNum);
};

#endif
