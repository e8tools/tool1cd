// CodeGear C++Builder
// Copyright (c) 1995, 2012 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'AbUtils.pas' rev: 24.00 (Windows)

#ifndef AbutilsHPP
#define AbutilsHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <Winapi.Windows.hpp>	// Pascal unit
#include <System.SysUtils.hpp>	// Pascal unit
#include <System.Classes.hpp>	// Pascal unit
#include <AbCharset.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Abutils
{
//-- type declarations -------------------------------------------------------
enum DECLSPEC_DENUM TAbArchiveAction : unsigned char { aaFailed, aaNone, aaAdd, aaDelete, aaFreshen, aaMove, aaReplace, aaStreamAdd };

enum DECLSPEC_DENUM TAbProcessType : unsigned char { ptAdd, ptDelete, ptExtract, ptFreshen, ptMove, ptReplace, ptFoundUnhandled };

enum DECLSPEC_DENUM TAbLogType : unsigned char { ltAdd, ltDelete, ltExtract, ltFreshen, ltMove, ltReplace, ltStart, ltFoundUnhandled };

enum DECLSPEC_DENUM TAbErrorClass : unsigned char { ecAbbrevia, ecInOutError, ecFilerError, ecFileCreateError, ecFileOpenError, ecCabError, ecOther };

enum DECLSPEC_DENUM TAbArchiveType : unsigned char { atUnknown, atZip, atSpannedZip, atSelfExtZip, atTar, atGzip, atGzippedTar, atCab, atBzip2, atBzippedTar };

typedef int PtrInt;

typedef unsigned PtrUInt;

typedef System::AnsiString AbSysString;

enum DECLSPEC_DENUM TAbPathType : unsigned char { ptNone, ptRelative, ptAbsolute };

struct DECLSPEC_DRECORD TAbAttrExRec
{
public:
	System::TDateTime Time;
	__int64 Size;
	int Attr;
	unsigned Mode;
};


//-- var, const, procedure ---------------------------------------------------
static const System::WideChar AbPathDelim = (System::WideChar)(0x5c);
static const System::WideChar AbPathSep = (System::WideChar)(0x3b);
static const System::WideChar AbDosPathDelim = (System::WideChar)(0x5c);
static const System::WideChar AbUnixPathDelim = (System::WideChar)(0x2f);
static const System::WideChar AbDosPathSep = (System::WideChar)(0x3b);
static const System::WideChar AbUnixPathSep = (System::WideChar)(0x3a);
#define AbDosAnyFile L"*.*"
static const System::WideChar AbUnixAnyFile = (System::WideChar)(0x2a);
#define AbAnyFile L"*.*"
static const System::WideChar AbThisDir = (System::WideChar)(0x2e);
#define AbParentDir L".."
extern DELPHI_PACKAGE System::StaticArray<unsigned, 256> AbCrc32Table;
#define AB_SPAN_VOL_LABEL L"PKBACK# %3.3d"
static const int Date1900 = int(0x1ac05);
static const int Date1970 = int(0x20fe4);
extern DELPHI_PACKAGE System::TDateTime Unix0Date;
static const int SecondsInDay = int(0x15180);
static const System::Word SecondsInHour = System::Word(0xe10);
static const System::Int8 SecondsInMinute = System::Int8(0x3c);
static const System::Int8 HoursInDay = System::Int8(0x18);
static const System::Int8 MinutesInHour = System::Int8(0x3c);
static const System::Word MinutesInDay = System::Word(0x5a0);
static const System::Int8 AB_FMODE_FILE = System::Int8(0x0);
static const System::Word AB_FMODE_FIFO = System::Word(0x1000);
static const System::Word AB_FMODE_CHARSPECFILE = System::Word(0x2000);
static const System::Word AB_FMODE_DIR = System::Word(0x4000);
static const System::Word AB_FMODE_BLOCKSPECFILE = System::Word(0x6000);
static const System::Word AB_FMODE_FILE2 = System::Word(0x8000);
static const System::Word AB_FMODE_FILELINK = System::Word(0xa000);
static const System::Word AB_FMODE_SOCKET = System::Word(0xc000);
static const System::Word AB_FPERMISSION_OWNERREAD = System::Word(0x100);
static const System::Byte AB_FPERMISSION_OWNERWRITE = System::Byte(0x80);
static const System::Int8 AB_FPERMISSION_OWNEREXECUTE = System::Int8(0x40);
static const System::Int8 AB_FPERMISSION_GROUPREAD = System::Int8(0x20);
static const System::Int8 AB_FPERMISSION_GROUPWRITE = System::Int8(0x10);
static const System::Int8 AB_FPERMISSION_GROUPEXECUTE = System::Int8(0x8);
static const System::Int8 AB_FPERMISSION_OTHERREAD = System::Int8(0x4);
static const System::Int8 AB_FPERMISSION_OTHERWRITE = System::Int8(0x2);
static const System::Int8 AB_FPERMISSION_OTHEREXECUTE = System::Int8(0x1);
static const System::Word AB_FPERMISSION_GENERIC = System::Word(0x1a4);
extern DELPHI_PACKAGE bool __fastcall AbCopyFile(const System::UnicodeString Source, const System::UnicodeString Destination, bool FailIfExists);
extern DELPHI_PACKAGE void __fastcall AbCreateDirectory(const System::UnicodeString Path);
extern DELPHI_PACKAGE System::UnicodeString __fastcall AbCreateTempFile(const System::UnicodeString Dir);
extern DELPHI_PACKAGE System::UnicodeString __fastcall AbGetTempDirectory(void);
extern DELPHI_PACKAGE System::UnicodeString __fastcall AbGetTempFile(const System::UnicodeString Dir, bool CreateIt);
extern DELPHI_PACKAGE System::WideChar __fastcall AbDrive(const System::UnicodeString ArchiveName);
extern DELPHI_PACKAGE bool __fastcall AbDriveIsRemovable(const System::UnicodeString ArchiveName);
extern DELPHI_PACKAGE __int64 __fastcall AbGetDriveFreeSpace(const System::UnicodeString ArchiveName);
extern DELPHI_PACKAGE bool __fastcall AbFileMatch(System::UnicodeString FileName, System::UnicodeString FileMask);
extern DELPHI_PACKAGE void __fastcall AbFindFiles(const System::UnicodeString FileMask, int SearchAttr, System::Classes::TStrings* FileList, bool Recurse);
extern DELPHI_PACKAGE void __fastcall AbFindFilesEx(const System::UnicodeString FileMask, int SearchAttr, System::Classes::TStrings* FileList, bool Recurse);
extern DELPHI_PACKAGE System::UnicodeString __fastcall AbAddBackSlash(const System::UnicodeString DirName);
extern DELPHI_PACKAGE int __fastcall AbFindNthSlash(const System::UnicodeString Path, int n);
extern DELPHI_PACKAGE TAbPathType __fastcall AbGetPathType(const System::UnicodeString Value);
extern DELPHI_PACKAGE System::UnicodeString __fastcall AbGetShortFileSpec(const System::UnicodeString LongFileSpec);
extern DELPHI_PACKAGE void __fastcall AbIncFilename(System::UnicodeString &Filename, System::Word Value);
extern DELPHI_PACKAGE void __fastcall AbParseFileName(System::UnicodeString FileSpec, /* out */ System::UnicodeString &Drive, /* out */ System::UnicodeString &Path, /* out */ System::UnicodeString &FileName);
extern DELPHI_PACKAGE void __fastcall AbParsePath(System::UnicodeString Path, System::Classes::TStrings* SubPaths);
extern DELPHI_PACKAGE bool __fastcall AbPatternMatch(const System::UnicodeString Source, int iSrc, const System::UnicodeString Pattern, int iPat);
extern DELPHI_PACKAGE System::Byte __fastcall AbPercentage(__int64 V1, __int64 V2);
extern DELPHI_PACKAGE void __fastcall AbStripDots(System::UnicodeString &FName);
extern DELPHI_PACKAGE void __fastcall AbStripDrive(System::UnicodeString &FName);
extern DELPHI_PACKAGE void __fastcall AbFixName(System::UnicodeString &FName);
extern DELPHI_PACKAGE void __fastcall AbUnfixName(System::UnicodeString &FName);
extern DELPHI_PACKAGE void __fastcall AbUpdateCRC(int &CRC, const void *Buffer, int Len);
extern DELPHI_PACKAGE int __fastcall AbUpdateCRC32(System::Byte CurByte, int CurCrc);
extern DELPHI_PACKAGE int __fastcall AbCRC32Of(const System::RawByteString aValue);
extern DELPHI_PACKAGE unsigned __fastcall AbWriteVolumeLabel(const System::UnicodeString VolName, System::WideChar Drive);
extern DELPHI_PACKAGE System::TDateTime __fastcall AbUnixTimeToLocalDateTime(int UnixTime);
extern DELPHI_PACKAGE int __fastcall AbLocalDateTimeToUnixTime(System::TDateTime DateTime);
extern DELPHI_PACKAGE System::TDateTime __fastcall AbDosFileDateToDateTime(System::Word FileDate, System::Word FileTime);
extern DELPHI_PACKAGE int __fastcall AbDateTimeToDosFileDate(System::TDateTime Value);
extern DELPHI_PACKAGE System::TDateTime __fastcall AbGetFileTime(const System::UnicodeString aFileName);
extern DELPHI_PACKAGE bool __fastcall AbSetFileTime(const System::UnicodeString aFileName, System::TDateTime aValue);
extern DELPHI_PACKAGE int __fastcall AbSwapLongEndianness(int Value);
extern DELPHI_PACKAGE int __fastcall AbDOS2UnixFileAttributes(int Attr);
extern DELPHI_PACKAGE int __fastcall AbUnix2DosFileAttributes(int Attr);
extern DELPHI_PACKAGE void __fastcall AbSetFileAttr(const System::UnicodeString aFileName, int aAttr);
extern DELPHI_PACKAGE __int64 __fastcall AbFileGetSize(const System::UnicodeString aFileName);
extern DELPHI_PACKAGE bool __fastcall AbFileGetAttrEx(const System::UnicodeString aFileName, /* out */ TAbAttrExRec &aAttr);
extern DELPHI_PACKAGE System::UnicodeString __fastcall AbGetVolumeLabel(System::WideChar Drive);
extern DELPHI_PACKAGE void __fastcall AbSetSpanVolumeLabel(System::WideChar Drive, int VolNo);
extern DELPHI_PACKAGE bool __fastcall AbTestSpanVolumeLabel(System::WideChar Drive, int VolNo);
extern DELPHI_PACKAGE System::AnsiString __fastcall AbLeftStr(const System::AnsiString AText, const int ACount);
extern DELPHI_PACKAGE unsigned __fastcall AbStrLen(const char * Str);
extern DELPHI_PACKAGE char * __fastcall AbStrPCopy(char * Dest, const System::AnsiString Source);
extern DELPHI_PACKAGE char * __fastcall AbStrPLCopy(char * Dest, const System::AnsiString Source, unsigned MaxLen);
}	/* namespace Abutils */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_ABUTILS)
using namespace Abutils;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// AbutilsHPP
