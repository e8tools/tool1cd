//---------------------------------------------------------------------------

#ifndef FileStorageH
#define FileStorageH

#include <System.Classes.hpp>
#include <vector>

#include "APIcfBase.h"
#include "Class_1CD.h"

//---------------------------------------------------------------------------
class FileStorage;

//---------------------------------------------------------------------------
class FileStorageStream : public TStream
{
protected:
	TStream* _data;
	FileStorage* _storage;
	String _name;
	virtual __int64 __fastcall GetSize(void){return _data->Size;};
	virtual void __fastcall SetSize(int NewSize){_data->Size = NewSize;};
	virtual void __fastcall SetSize(const __int64 NewSize){_data->Size = NewSize;};
public:
	__fastcall FileStorageStream(FileStorage* _s, TStream* _d, const String& _n);
	virtual __fastcall ~FileStorageStream();

	virtual int __fastcall Read(void *Buffer, int Count){return _data->Read(Buffer, Count);};
	virtual int __fastcall Read(System::DynamicArray<System::Byte> Buffer, int Offset, int Count){return _data->Read(Buffer, Offset, Count);};
	virtual int __fastcall Write(const void *Buffer, int Count){return _data->Write(Buffer, Count);};
	virtual int __fastcall Write(const System::DynamicArray<System::Byte> Buffer, int Offset, int Count){return _data->Write(Buffer, Offset, Count);};
	virtual int __fastcall Seek(int Offset, System::Word Origin){return _data->Seek(Offset, Origin);};
	virtual __int64 __fastcall Seek(const __int64 Offset, TSeekOrigin Origin){return _data->Seek(Offset, Origin);};

	__property TStream* data = {read = _data};
	__property String name = {read = _name};

};

////---------------------------------------------------------------------------
//class TReadOnlyStream : public TStream
//{
//protected:
//	TStream* _data;
//	virtual __int64 __fastcall GetSize(void){return _data->Size;};
//	virtual void __fastcall SetSize(int NewSize){throw(Exception(L"Set size read-only stream"));};
//	virtual void __fastcall SetSize(const __int64 NewSize){throw(Exception(L"Set size read-only stream"));};
//public:
//	__fastcall TReadOnlyStream(TStream* _d){_data = _d;};
//	virtual __fastcall ~TReadOnlyStream(){delete _data;};
//
//	virtual int __fastcall Read(void *Buffer, int Count){return _data->Read(Buffer, Count);};
//	virtual int __fastcall Read(System::DynamicArray<System::Byte> Buffer, int Offset, int Count){return _data->Read(Buffer, Offset, Count);};
//	virtual int __fastcall Write(const void *Buffer, int Count){throw(Exception(L"Write read-only stream"));};
//	virtual int __fastcall Write(const System::DynamicArray<System::Byte> Buffer, int Offset, int Count){throw(Exception(L"Write read-only stream"));};
//	virtual int __fastcall Seek(int Offset, System::Word Origin){return _data->Seek(Offset, Origin);};
//	virtual __int64 __fastcall Seek(const __int64 Offset, TSeekOrigin Origin){return _data->Seek(Offset, Origin);};
//
//	__property TStream* data = {read = _data};
//
//};

//---------------------------------------------------------------------------
class FileStorage
{
friend FileStorageStream;
private:
	std::map<String, FileStorageStream*> openfiles; // набор открытых файлов
protected:
	bool _readonly;
	String __fastcall normalizepath(const String& path);
	FileStorageStream* __fastcall getstream(const String& path);
public:
	__fastcall FileStorage(bool readonly_ = false) : _readonly(readonly_){}
	virtual __fastcall ~FileStorage();
	virtual String __fastcall presentation() = 0; // Представление хранилища файлов
	virtual FileStorageStream* __fastcall open(const String& path) = 0; // Открыть файл. Если файл не существует, возвращается NULL
	virtual bool __fastcall write(const String& path, TStream* str) = 0; // создание файла и запись или перезапись целиком
	virtual bool __fastcall fileexists(const String& path) = 0; // проверка существования файла
	virtual int __fastcall count() = 0; // количество файлов в хранилище (файлов и каталогов верхнего уровня)
	virtual String __fastcall getname(unsigned int index) = 0; // Получить имя файла (или каталога) по индексу
	__property bool readonly = {read = _readonly};
};

//---------------------------------------------------------------------------
class FileStorageDirectory : public FileStorage
{
protected:
	String dir;
	std::vector<String> names;
public:
	__fastcall FileStorageDirectory(const String& _dir);
	virtual __fastcall ~FileStorageDirectory();
	virtual String __fastcall presentation();
	virtual FileStorageStream* __fastcall open(const String& path); // Если файл не существует, возвращается NULL
	virtual bool __fastcall write(const String& path, TStream* str); // создание файла или перезапись целиком
	virtual bool __fastcall fileexists(const String& path);
	virtual int __fastcall count(){return names.size();}
	virtual String __fastcall getname(unsigned int index);
};
//---------------------------------------------------------------------------

class FileStorageCFFile : public FileStorage
{
protected:
	String filename;
	v8catalog* cat;
	std::vector<String> names;
public:
	__fastcall FileStorageCFFile(const String& fname);
	virtual __fastcall ~FileStorageCFFile();
	virtual String __fastcall presentation();
	virtual FileStorageStream* __fastcall open(const String& path); // Если файл не существует, возвращается NULL
	virtual bool __fastcall write(const String& path, TStream* str); // создание файла или перезапись целиком
	virtual bool __fastcall fileexists(const String& path);
	virtual int __fastcall count(){return names.size();}
	virtual String __fastcall getname(unsigned int index);
};

//---------------------------------------------------------------------------

class FileStorageTable : public FileStorage
{
protected:
	table* t;
public:
	__fastcall FileStorageTable(table* _t);
	virtual __fastcall ~FileStorageTable();
	virtual String __fastcall presentation();
	virtual FileStorageStream* __fastcall open(const String& path); // Если файл не существует, возвращается NULL
	virtual bool __fastcall write(const String& path, TStream* str){return false;} // создание файла или перезапись целиком
	virtual bool __fastcall fileexists(const String& path);
	virtual int __fastcall count();
	virtual String __fastcall getname(unsigned int index);

};
//---------------------------------------------------------------------------
#endif

