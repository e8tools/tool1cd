#ifndef v8cfMainH
#define v8cfMainH

#include "APIcfBase.h"
#include "wcxhead.h"

#if defined(BUILD_DLL)
//#	define DLL_EXP __declspec(dllexport)
#	define DLL_EXP extern "C" __declspec(dllexport)
#else
#	if defined(BUILD_APP)
#		define DLL_EXP extern "C" __declspec(dllimport)
#	else
#		define DLL_EXP
#	endif
#endif

/*
struct cashefile{
	v8catalog* catalog;
};
*/

struct v8archive
{
	v8catalog* catalog;
	v8file* lastfile;
	char name[260];

	bool open; // true, если открыт Total Commander'ом
	int timequota; // если больше нуля, catalog содержит разобранный файл
	FILETIME lastwrite;
	FILETIME filecreate;
	ULARGE_INTEGER filelength;
};




DLL_EXP HANDLE __stdcall OpenArchive(tOpenArchiveData *ArchiveData);
DLL_EXP HANDLE __stdcall OpenArchiveW(tOpenArchiveDataW *ArchiveData);
DLL_EXP int __stdcall ReadHeader(HANDLE hArcData, tHeaderData *HeaderData);
DLL_EXP int __stdcall ReadHeaderEx(HANDLE hArcData, tHeaderDataEx *HeaderDataEx);
DLL_EXP int __stdcall ReadHeaderExW(HANDLE hArcData, tHeaderDataExW *HeaderDataEx);
DLL_EXP int __stdcall ProcessFile(HANDLE hArcData, int Operation, char *DestPath, char *DestName);
DLL_EXP int __stdcall ProcessFileW(HANDLE hArcData, int Operation, wchar_t *DestPath, wchar_t *DestName);
DLL_EXP int __stdcall CloseArchive(HANDLE hArcData);
DLL_EXP void __stdcall SetChangeVolProc(HANDLE hArcData, tChangeVolProc pChangeVolProc1);
DLL_EXP void __stdcall SetChangeVolProcW(HANDLE hArcData, tChangeVolProcW pChangeVolProc1);
DLL_EXP void __stdcall SetProcessDataProc(HANDLE hArcData, tProcessDataProc pProcessDataProc);
DLL_EXP void __stdcall SetProcessDataProcW(HANDLE hArcData, tProcessDataProcW pProcessDataProc);
DLL_EXP int __stdcall PackFiles(char *PackedFile, char *SubPath, char *SrcPath, char *AddList, int Flags);
DLL_EXP int __stdcall PackFilesW(wchar_t *PackedFile, wchar_t *SubPath, wchar_t *SrcPath, wchar_t *AddList, int Flags);
DLL_EXP int __stdcall DeleteFiles(char *PackedFile, char *DeleteList);
DLL_EXP int __stdcall DeleteFilesW(wchar_t *PackedFile, wchar_t *DeleteList);
DLL_EXP bool __stdcall CanYouHandleThisFile(char *FileName);
DLL_EXP bool __stdcall CanYouHandleThisFileW(wchar_t *FileName);
DLL_EXP int __stdcall GetPackerCaps();


#endif

