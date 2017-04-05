//   Important note about DLL memory management when your DLL uses the
//   static version of the RunTime Library:
//
//   If your DLL exports any functions that pass String objects (or structs/
//   classes containing nested Strings) as parameter or function results,
//   you will need to add the library MEMMGR.LIB to both the DLL project and
//   any other projects that use the DLL.  You will also need to use MEMMGR.LIB
//   if any other projects which use the DLL will be performing new or delete
//   operations on any non-TObject-derived classes which are exported from the
//   DLL. Adding MEMMGR.LIB to your project will change the DLL and its calling
//   EXE's to use the BORLNDMM.DLL as their memory manager.  In these cases,
//   the file BORLNDMM.DLL should be deployed along with your DLL.
//
//   To avoid using BORLNDMM.DLL, pass string information using "char *" or
//   ShortString parameters.
//
//   If your DLL uses the dynamic version of the RTL, you do not need to
//   explicitly add MEMMGR.LIB as this will be done implicitly for you

//#include <vcl.h>
#include <System.Classes.hpp>
#include <windows.h>
#pragma hdrstop

#define BUILD_DLL
#include "v8cfMain.h"
#include "cunicode.h"


#define ArchiveCacheCount 5
#define CacheCount 40

v8archive v8archives[ArchiveCacheCount];
UINT Timer;
//bool isinit = false;

void CALLBACK TimeProc(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime){
	if(idEvent == Timer)
	{
		//MessageBox(NULL, "yes", "TimerProc", MB_OK);
		for(int i = 0; i < ArchiveCacheCount; i++)
		{
			v8archive* a = &v8archives[i];
			if(a->open) continue;
			if(a->timequota == 0) continue;
			if(--a->timequota == 0)
			{
				delete a->catalog;
				a->catalog = NULL;
				a->lastfile = NULL;
				a->name[0] = 0;
				a->open = false;
				a->timequota = 0;
				a->filelength.QuadPart = 0;
			}
		}
	}
//	else MessageBox(NULL, "no", "TimerProc", MB_OK);
}

//Actual DLL entry point:
void EntryPoint(const unsigned long reason)
{
	int i;
	switch(reason){
		case DLL_PROCESS_ATTACH:
			//MessageBox(NULL, "DllEntryPoint ATTACH", "v8cf", MB_OK);
			// Инициализируем массив v8archives
			for(i = 0; i < ArchiveCacheCount; i++){
				v8archive* a = &v8archives[i];
				a->catalog = NULL;
				a->lastfile = NULL;
				a->name[0] = 0;
				a->open = false;
				a->timequota = 0;
				a->filelength.QuadPart = 0;
				//a->lastwrite
			}

			Timer = SetTimer(NULL, 0, 15000, (TIMERPROC)TimeProc); // Срабатывает каждые 15 секунд
			//isinit = true;
			break;
		case DLL_PROCESS_DETACH:
			//MessageBox(NULL, "DllEntryPoint DETACH", "v8cf", MB_OK);
			KillTimer(NULL, Timer);
			for(i = 0; i < ArchiveCacheCount; i++){
				v8archive* a = &v8archives[i];
				if(a->timequota) delete a->catalog;
			}
			break;
	}
}

#pragma argsused

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
	EntryPoint(reason);
	return 1;
}

//---------------------------------------------------------------------------

v8archive* getv8archive(const char* filename)
{
	FILETIME ft;
	FILETIME fc;
	ULARGE_INTEGER fs;
	HANDLE hf;
	int i, j, minquota;
	v8archive* a;
	AnsiString fn = filename;

	//if(!isinit) EntryPoint(DLL_PROCESS_ATTACH);

	hf = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if(hf == INVALID_HANDLE_VALUE)
	{
		//MessageBox(NULL, "Error CreateFile", "v8cf", MB_OK);
		return NULL;
	}

	fs.LowPart = GetFileSize(hf, &fs.HighPart);
	GetFileTime(hf, &fc, NULL, &ft);
	CloseHandle(hf);

	minquota = CacheCount + 1;
	j = 0;

	for(i = 0; i < ArchiveCacheCount; i++)
	{
		a = &v8archives[i];

		if(a->timequota < minquota)
		{
			minquota = a->timequota;
			j = i;
		}

		if(!a->timequota) continue;
		if(fn.AnsiCompareIC(a->name)) continue;
		if(fs.QuadPart != a->filelength.QuadPart) continue;
		if(ft.dwLowDateTime != a->lastwrite.dwLowDateTime) continue;
		if(ft.dwHighDateTime != a->lastwrite.dwHighDateTime) continue;
		if(fc.dwLowDateTime != a->filecreate.dwLowDateTime) continue;
		if(fc.dwHighDateTime != a->filecreate.dwHighDateTime) continue;
		a->open = true;
		a->timequota = CacheCount; // 10 минут (40 * 15 сек)
		a->catalog->HalfOpen(fn);
		//MessageBox(NULL, "yes", "v8cf", MB_OK);
		return a;
	}

	a = &v8archives[j];
	if(a->timequota) delete a->catalog;
	a->catalog = new v8catalog(fn);
	a->lastfile = NULL;
	strcpy(a->name, filename);
	a->open = true;
	a->timequota = CacheCount;
	a->filelength.QuadPart = fs.QuadPart;
	a->lastwrite = ft;
	a->filecreate = fc;

	//MessageBox(NULL, "no", "v8cf", MB_OK);
	return a;
}

void HalfCloseArchive(v8archive* a)
{
	HANDLE hf;

	a->catalog->HalfClose();

	hf = CreateFile(a->name, GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	if(hf != INVALID_HANDLE_VALUE)
	{
		//MessageBox(NULL, "HalfCloseArchive yes", "v8cf", MB_OK);
		a->filelength.LowPart = GetFileSize(hf, &a->filelength.HighPart);
		GetFileTime(hf, &a->filecreate, NULL, &a->lastwrite);
		CloseHandle(hf);
	}
	//else MessageBox(NULL, "HalfCloseArchive no", "v8cf", MB_OK);

	a->lastfile = NULL;
	a->open = false;
}

//---------------------------------------------------------------------------

DLL_EXP HANDLE __stdcall OpenArchive(tOpenArchiveData *ArchiveData)
{
	//MessageBox(NULL, "OpenArchive", "v8cf", MB_OK);
	v8archive* a = getv8archive(ArchiveData->ArcName);
	ArchiveData->OpenResult = 0;
	ArchiveData->CmtBuf = NULL;
	ArchiveData->CmtBufSize = 0;
	ArchiveData->CmtSize = 0;
	ArchiveData->CmtState = 0;
	return a;
}

DLL_EXP HANDLE __stdcall OpenArchiveW(tOpenArchiveDataW *ArchiveData)
{
	//MessageBox(NULL, "OpenArchiveW", "v8cf", MB_OK);
	char aname[260];
	v8archive* a = getv8archive(walcopy(aname, ArchiveData->ArcName, 259));
	ArchiveData->OpenResult = 0;
	ArchiveData->CmtBuf = NULL;
	ArchiveData->CmtBufSize = 0;
	ArchiveData->CmtSize = 0;
	ArchiveData->CmtState = 0;
	return a;
}


DLL_EXP int __stdcall ReadHeader(HANDLE hArcData, tHeaderData *HeaderData)
{
	FILETIME ft;
	FILETIME lft;
	SYSTEMTIME st;

	//MessageBox(NULL, "ReadHeader", "v8cf", MB_OK);
	v8archive* a = (v8archive*)hArcData;
	v8file* f = a->lastfile;
	v8file* f2;

	if(!f) f = a->catalog->GetFirst();
	else
	{
		if(f->IsCatalog())
		{
			f2 = f->GetCatalog()->GetFirst();
			if(!f2) f2 = f->GetNext();
		}
		else f2 = f->GetNext();
		while(!f2)
		{
			if(f->GetParentCatalog()->GetSelfFile())
			{
				f = f->GetParentCatalog()->GetSelfFile();
				f2 = f->GetNext();
			}
			else break;
		}
		f = f2;
	}



	a->lastfile = f;

	if(f)
	{
		strcpy(HeaderData->ArcName, a->name);
		//strcpy(HeaderData->FileName, f->GetFullName().c_str());
		wcstombs(HeaderData->FileName, f->GetFullName().c_str(), 260);
		//HeaderData->Flags = 0;
		HeaderData->PackSize = f->GetFileLength();
		HeaderData->UnpSize = f->GetFileLength();
		HeaderData->HostOS = 0;
		HeaderData->FileCRC = 0;
		HeaderData->CmtBuf = NULL;
		HeaderData->CmtBufSize = 0;
		HeaderData->CmtSize = 0;
		HeaderData->CmtState = 0;
		f->GetTimeModify(&ft);
		FileTimeToLocalFileTime(&ft, &lft);
		FileTimeToSystemTime(&lft, &st);
		int year = st.wYear;
		if(year < 1980) year = 1980;
		if(year > 2100) year = 2100;
		HeaderData->FileTime = (year - 1980) << 25 | st.wMonth << 21 | st.wDay << 16 | st.wHour << 11 | st.wMinute << 5 | st.wSecond / 2;
		HeaderData->UnpVer = 0;
		HeaderData->Method = 0;
		HeaderData->FileAttr = f->IsCatalog() ? 0x10 : 0;
		return 0;
	}
	else
	{
		return E_END_ARCHIVE;
	}
}

DLL_EXP int __stdcall ReadHeaderEx(HANDLE hArcData, tHeaderDataEx *HeaderDataEx)
{
	FILETIME ft;
	FILETIME lft;
	SYSTEMTIME st;

	//MessageBox(NULL, "ReadHeaderEx", "v8cf", MB_OK);
	v8archive* a = (v8archive*)hArcData;
	v8file* f = a->lastfile;
	v8file* f2;

	if(!f) f = a->catalog->GetFirst();
	else
	{
		if(f->IsCatalog())
		{
			f2 = f->GetCatalog()->GetFirst();
			if(!f2) f2 = f->GetNext();
		}
		else f2 = f->GetNext();
		while(!f2)
		{
			if(f->GetParentCatalog()->GetSelfFile())
			{
				f = f->GetParentCatalog()->GetSelfFile();
				f2 = f->GetNext();
			}
			else break;
		}
		f = f2;
	}



	a->lastfile = f;

	if(f)
	{
		strcpy(HeaderDataEx->ArcName, a->name);
		//strcpy(HeaderData->FileName, f->GetFullName().c_str());
		wcstombs(HeaderDataEx->FileName, f->GetFullName().c_str(), 260);
		//HeaderData->Flags = 0;
		HeaderDataEx->PackSize = (unsigned int)f->GetFileLength();
		HeaderDataEx->PackSizeHigh = 0;
		HeaderDataEx->UnpSize = (unsigned int)f->GetFileLength();
		HeaderDataEx->UnpSizeHigh = 0;
		HeaderDataEx->HostOS = 0;
		HeaderDataEx->FileCRC = 0;
		HeaderDataEx->CmtBuf = NULL;
		HeaderDataEx->CmtBufSize = 0;
		HeaderDataEx->CmtSize = 0;
		HeaderDataEx->CmtState = 0;
		f->GetTimeModify(&ft);
		FileTimeToLocalFileTime(&ft, &lft);
		FileTimeToSystemTime(&lft, &st);
		int year = st.wYear;
		if(year < 1980) year = 1980;
		if(year > 2100) year = 2100;
		HeaderDataEx->FileTime = (year - 1980) << 25 | st.wMonth << 21 | st.wDay << 16 | st.wHour << 11 | st.wMinute << 5 | st.wSecond / 2;
		HeaderDataEx->UnpVer = 0;
		HeaderDataEx->Method = 0;
		HeaderDataEx->FileAttr = f->IsCatalog() ? 0x10 : 0;
		//HeaderData->FileAttr = 0;
		return 0;
	}
	else
	{
		return E_END_ARCHIVE;
	}
}

DLL_EXP int __stdcall ReadHeaderExW(HANDLE hArcData, tHeaderDataExW *HeaderDataEx)
{
	FILETIME ft;
	FILETIME lft;
	SYSTEMTIME st;

	//MessageBox(NULL, "ReadHeaderExW", "v8cf", MB_OK);
	v8archive* a = (v8archive*)hArcData;
	v8file* f = a->lastfile;
	v8file* f2;

	if(!f) f = a->catalog->GetFirst();
	else
	{
		if(f->IsCatalog())
		{
			f2 = f->GetCatalog()->GetFirst();
			if(!f2) f2 = f->GetNext();
		}
		else f2 = f->GetNext();
		while(!f2)
		{
			if(f->GetParentCatalog()->GetSelfFile())
			{
				f = f->GetParentCatalog()->GetSelfFile();
				f2 = f->GetNext();
			}
			else break;
		}
		f = f2;
	}



	a->lastfile = f;

	if(f)
	{
		awlcopy(HeaderDataEx->ArcName, a->name, 259);
		wcscpy(HeaderDataEx->FileName, f->GetFullName().c_str());
		//HeaderData->Flags = 0;
		HeaderDataEx->PackSize = (unsigned int)f->GetFileLength();
		HeaderDataEx->PackSizeHigh = 0;
		HeaderDataEx->UnpSize = (unsigned int)f->GetFileLength();
		HeaderDataEx->UnpSizeHigh = 0;
		HeaderDataEx->HostOS = 0;
		HeaderDataEx->FileCRC = 0;
		HeaderDataEx->CmtBuf = NULL;
		HeaderDataEx->CmtBufSize = 0;
		HeaderDataEx->CmtSize = 0;
		HeaderDataEx->CmtState = 0;
		f->GetTimeModify(&ft);
		FileTimeToLocalFileTime(&ft, &lft);
		FileTimeToSystemTime(&lft, &st);
		int year = st.wYear;
		if(year < 1980) year = 1980;
		if(year > 2100) year = 2100;
		HeaderDataEx->FileTime = (year - 1980) << 25 | st.wMonth << 21 | st.wDay << 16 | st.wHour << 11 | st.wMinute << 5 | st.wSecond / 2;
		HeaderDataEx->UnpVer = 0;
		HeaderDataEx->Method = 0;
		HeaderDataEx->FileAttr = f->IsCatalog() ? 0x10 : 0;
		//HeaderData->FileAttr = 0;
		return 0;
	}
	else
	{
		return E_END_ARCHIVE;
	}
}

DLL_EXP int __stdcall ProcessFile(HANDLE hArcData, int Operation, char *DestPath, char *DestName)
{
	AnsiString fname;

//	MessageBox(NULL, "ProcessFile", "v8cf", MB_OK);
	v8archive* a = (v8archive*)hArcData;
	v8file* f;
	f = a->lastfile;

	if(f)
	{
		if(Operation == PK_EXTRACT)
		{
			fname = DestPath;
			if(DestName) fname += DestName;
			f->SaveToFile(fname);
		}
		return 0;
	}else return E_END_ARCHIVE;
}

DLL_EXP int __stdcall ProcessFileW(HANDLE hArcData, int Operation, wchar_t *DestPath, wchar_t *DestName)
{
	UnicodeString fname;

//	MessageBox(NULL, "ProcessFileW", "v8cf", MB_OK);
	v8archive* a = (v8archive*)hArcData;
	v8file* f;
	f = a->lastfile;

	if(f)
	{
		if(Operation == PK_EXTRACT)
		{
			fname = DestPath;
			if(DestName) fname += DestName;
			f->SaveToFile(fname);
		}
		return 0;
	}else return E_END_ARCHIVE;
}

DLL_EXP int __stdcall CloseArchive(HANDLE hArcData)
{
	//MessageBox(NULL, "CloseArchive", "v8cf", MB_OK);
	v8archive* a = (v8archive*)hArcData;
	HalfCloseArchive(a);
	return 0;
}

DLL_EXP void __stdcall SetChangeVolProc(HANDLE hArcData, tChangeVolProc pChangeVolProc1)
{
//	MessageBox(NULL, "SetChangeVolProc", "v8cf", MB_OK);
}

DLL_EXP void __stdcall SetChangeVolProcW(HANDLE hArcData, tChangeVolProcW pChangeVolProc1)
{
//	MessageBox(NULL, "SetChangeVolProcW", "v8cf", MB_OK);
}

DLL_EXP void __stdcall SetProcessDataProc(HANDLE hArcData, tProcessDataProc pProcessDataProc)
{
//	MessageBox(NULL, "SetProcessDataProc", "v8cf", MB_OK);
}

DLL_EXP void __stdcall SetProcessDataProcW(HANDLE hArcData, tProcessDataProcW pProcessDataProc)
{
//	MessageBox(NULL, "SetProcessDataProcW", "v8cf", MB_OK);
}

DLL_EXP int __stdcall PackFiles(char *PackedFile, char *SubPath, char *SrcPath, char *AddList, int Flags)
{
	v8archive* a = getv8archive(PackedFile);

//	MessageBox(NULL, "PackFiles", "v8cf", MB_OK);
	v8catalog* catalog = a->catalog;
	v8catalog* subcat;
	v8catalog* cat;
	AnsiString name;
	AnsiString cat_name;
	AnsiString path_name;
	v8file* file;
	TFileStream* sfile;
	int i;
	char* name_ptr;

	subcat = catalog;
	if(SubPath) if(SubPath[0])
	{
		name = SubPath;
		while((i = name.Pos("\\")) != 0)
		{
			cat_name = name.SubString(0, i - 1);
			subcat = subcat->CreateCatalog(cat_name);
			name = name.SubString(i + 1, 260);
		}
		subcat = subcat->CreateCatalog(name);
	}

	path_name = SrcPath;
	if(path_name[path_name.Length()] != '\\') path_name += "\\";

	name_ptr = AddList;
	while(name_ptr[0])
	{
		cat = subcat;
		name = name_ptr;
		while((i = name.Pos("\\")) != 0)
		{
			if(Flags & PK_PACK_SAVE_PATHS)
			{
				cat_name = name.SubString(0, i - 1);
				cat = cat->CreateCatalog(cat_name);
				if(!cat) break;
			}
			name = name.SubString(i + 1, 260);
		}

		if(cat && name.Length())
		{
			file = cat->createFile(name);
			sfile = new TFileStream(path_name + name_ptr, fmOpenRead);
			file->Write(sfile);
			FILETIME _CreationTime;
			FILETIME _LastWriteTime;
			if(GetFileTime((HANDLE)sfile->Handle, &_CreationTime, NULL, &_LastWriteTime))
			{
				file->SetTimeCreate(&_CreationTime);
				file->SetTimeModify(&_LastWriteTime);
			}
			delete sfile;

			if(Flags & PK_PACK_MOVE_FILES) DeleteFile((path_name + name_ptr).c_str());
		}

		name_ptr += strlen(name_ptr) + 1;
	}

	HalfCloseArchive(a);
	return 0;

}

DLL_EXP int __stdcall PackFilesW(wchar_t *PackedFile, wchar_t *SubPath, wchar_t *SrcPath, wchar_t *AddList, int Flags)
{
	char aname[260];
	v8archive* a = getv8archive(walcopy(aname, PackedFile, 259));

//	MessageBox(NULL, "PackFilesW", "v8cf", MB_OK);
	v8catalog* catalog = a->catalog;
	v8catalog* subcat;
	v8catalog* cat;
	UnicodeString name;
	UnicodeString cat_name;
	UnicodeString path_name;
	v8file* file;
	TFileStream* sfile;
	int i;
	wchar_t* name_ptr;

	subcat = catalog;
	if(SubPath) if(SubPath[0])
	{
		name = SubPath;
		while((i = name.Pos(L"\\")) != 0)
		{
			cat_name = name.SubString(0, i - 1);
			subcat = subcat->CreateCatalog(cat_name);
			name = name.SubString(i + 1, 260);
		}
		subcat = subcat->CreateCatalog(name);
	}

	path_name = SrcPath;
	if(path_name[path_name.Length()] != L'\\') path_name += L"\\";

	name_ptr = AddList;
	while(name_ptr[0])
	{
		cat = subcat;
		name = name_ptr;
		while((i = name.Pos(L"\\")) != 0)
		{
			if(Flags & PK_PACK_SAVE_PATHS)
			{
				cat_name = name.SubString(0, i - 1);
				cat = cat->CreateCatalog(cat_name);
				if(!cat) break;
			}
			name = name.SubString(i + 1, 260);
		}

		if(cat && name.Length())
		{
			file = cat->createFile(name);
			sfile = new TFileStream(path_name + name_ptr, fmOpenRead);
			file->Write(sfile);
			FILETIME _CreationTime;
			FILETIME _LastWriteTime;
			if(GetFileTime((HANDLE)sfile->Handle, &_CreationTime, NULL, &_LastWriteTime))
			{
				file->SetTimeCreate(&_CreationTime);
				file->SetTimeModify(&_LastWriteTime);
			}
			delete sfile;

			if(Flags & PK_PACK_MOVE_FILES) DeleteFile((path_name + name_ptr).c_str());
		}

		name_ptr += wcslen(name_ptr) + 1;
	}

	HalfCloseArchive(a);
	return 0;

}

DLL_EXP int __stdcall DeleteFiles(char *PackedFile, char *DeleteList)
{
//	MessageBox(NULL, "DeleteFiles", "v8cf", MB_OK);
	v8archive* a = getv8archive(PackedFile);
	v8catalog* catalog = a->catalog;
	AnsiString name;
	AnsiString cat_name;
	v8file* file;
	v8catalog* cat;
	int i;
	char* name_ptr;

	name_ptr = DeleteList;
	while(name_ptr[0])
	{
		cat = catalog;
		name = name_ptr;
		while((i = name.Pos("\\")) != 0)
		{
			cat_name = name.SubString(0, i - 1);
			file = cat->GetFile(cat_name);
			if(!file)
			{
				cat = NULL;
				break;
			}
			cat = file->GetCatalog();
			if(!cat) break;
			name = name.SubString(i + 1, 260);
		}
		if(cat)
		{
			if(name == "*.*")
			{
				if(cat->GetSelfFile()) cat->GetSelfFile()->DeleteFile();
			}
			else cat->DeleteFile(name);
		}
		name_ptr += strlen(name_ptr) + 1;
	}

	HalfCloseArchive(a);
	return 0;
}

DLL_EXP int __stdcall DeleteFilesW(wchar_t *PackedFile, wchar_t *DeleteList)
{
//	MessageBox(NULL, "DeleteFilesW", "v8cf", MB_OK);
	char aname[260];
	v8archive* a = getv8archive(walcopy(aname, PackedFile, 259));
	v8catalog* catalog = a->catalog;
	UnicodeString name;
	UnicodeString cat_name;
	v8file* file;
	v8catalog* cat;
	int i;
	wchar_t* name_ptr;

	name_ptr = DeleteList;
	while(name_ptr[0])
	{
		cat = catalog;
		name = name_ptr;
		while((i = name.Pos(L"\\")) != 0)
		{
			cat_name = name.SubString(0, i - 1);
			file = cat->GetFile(cat_name);
			if(!file)
			{
				cat = NULL;
				break;
			}
			cat = file->GetCatalog();
			if(!cat) break;
			name = name.SubString(i + 1, 260);
		}
		if(cat)
		{
			if(name == L"*.*")
			{
				if(cat->GetSelfFile()) cat->GetSelfFile()->DeleteFile();
			}
			else cat->DeleteFile(name);
		}
		name_ptr += wcslen(name_ptr) + 1;
	}

	HalfCloseArchive(a);
	return 0;
}


DLL_EXP bool __stdcall CanYouHandleThisFile(char *FileName)
{
//	MessageBox(NULL, "CanYouHandleThisFile", "v8cf", MB_OK);
	v8archive* a = getv8archive(FileName);
	v8catalog* c = a->catalog;
	bool ret = c->IsCatalog();
	if(!ret)
	{
		delete a->catalog;
		a->catalog = NULL;
		a->lastfile = NULL;
		a->name[0] = 0;
		a->open = false;
		a->timequota = 0;
		a->filelength.QuadPart = 0;
	}
	else HalfCloseArchive(a);

	return ret;
}

DLL_EXP bool __stdcall CanYouHandleThisFileW(wchar_t *FileName)
{
//	MessageBox(NULL, "CanYouHandleThisFileW", "v8cf", MB_OK);
	char aname[260];
	v8archive* a = getv8archive(walcopy(aname, FileName, 259));
	v8catalog* c = a->catalog;
	bool ret = c->IsCatalog();
	if(!ret)
	{
		delete a->catalog;
		a->catalog = NULL;
		a->lastfile = NULL;
		a->name[0] = 0;
		a->open = false;
		a->timequota = 0;
		a->filelength.QuadPart = 0;
	}
	else HalfCloseArchive(a);

	return ret;
}


DLL_EXP int __stdcall GetPackerCaps()
{
//	MessageBox(NULL, "GetPackerCaps", "v8cf", MB_OK);
	return
		PK_CAPS_NEW //1	Can create new archives
		| PK_CAPS_MODIFY //2	Can modify existing archives
		| PK_CAPS_MULTIPLE //4	Archive can contain multiple files
		| PK_CAPS_DELETE //8	Can delete files
		//| PK_CAPS_OPTIONS //16	Has options dialog
		//| PK_CAPS_MEMPACK //32	Supports packing in memory
		| PK_CAPS_BY_CONTENT //64	Detect archive type by content
		| PK_CAPS_SEARCHTEXT //128	Allow searching for text in archives created with this plugin
		| PK_CAPS_HIDE //256	Don't show packer icon, don't open with Enter but with Ctrl+PgDn
		//| PK_CAPS_ENCRYPT //512	Plugin supports encryption.
		;
}






