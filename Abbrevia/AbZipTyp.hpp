// CodeGear C++Builder
// Copyright (c) 1995, 2012 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'AbZipTyp.pas' rev: 24.00 (Windows)

#ifndef AbziptypHPP
#define AbziptypHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.Classes.hpp>	// Pascal unit
#include <AbArcTyp.hpp>	// Pascal unit
#include <AbUtils.hpp>	// Pascal unit
#include <AbSpanSt.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Abziptyp
{
//-- type declarations -------------------------------------------------------
typedef System::StaticArray<System::Byte, 4096> TAbByteArray4K;

typedef TAbByteArray4K *PAbByteArray4K;

typedef System::StaticArray<System::Byte, 8193> TAbByteArray8K;

typedef TAbByteArray8K *PAbByteArray8K;

typedef System::StaticArray<short, 8193> TAbIntArray8K;

typedef TAbIntArray8K *PAbIntArray8K;

typedef System::StaticArray<System::Word, 32767> TAbWordArray;

typedef TAbWordArray *PAbWordArray;

typedef System::StaticArray<System::Byte, 65535> TAbByteArray;

typedef TAbByteArray *PAbByteArray;

typedef System::StaticArray<short, 32767> TAbSmallIntArray;

typedef TAbSmallIntArray *PAbSmallIntArray;

typedef System::StaticArray<int, 16383> TAbIntegerArray;

typedef TAbIntegerArray *PAbIntegerArray;

#pragma pack(push,1)
struct DECLSPEC_DRECORD TAbZip64EndOfCentralDirectoryRecord
{
public:
	int Signature;
	__int64 RecordSize;
	System::Word VersionMadeBy;
	System::Word VersionNeededToExtract;
	unsigned DiskNumber;
	unsigned StartDiskNumber;
	__int64 EntriesOnDisk;
	__int64 TotalEntries;
	__int64 DirectorySize;
	__int64 DirectoryOffset;
};
#pragma pack(pop)


#pragma pack(push,1)
struct DECLSPEC_DRECORD TAbZip64EndOfCentralDirectoryLocator
{
public:
	int Signature;
	int StartDiskNumber;
	__int64 RelativeOffset;
	int TotalDisks;
};
#pragma pack(pop)


#pragma pack(push,1)
struct DECLSPEC_DRECORD TAbZipEndOfCentralDirectoryRecord
{
public:
	int Signature;
	System::Word DiskNumber;
	System::Word StartDiskNumber;
	System::Word EntriesOnDisk;
	System::Word TotalEntries;
	unsigned DirectorySize;
	unsigned DirectoryOffset;
	System::Word CommentLength;
};
#pragma pack(pop)


#pragma pack(push,1)
struct DECLSPEC_DRECORD TAbFollower
{
public:
	System::Byte Size;
	System::StaticArray<System::Byte, 32> FSet;
};
#pragma pack(pop)


typedef System::StaticArray<TAbFollower, 256> TAbFollowerSets;

typedef TAbFollowerSets *PAbFollowerSets;

struct TAbSfEntry;
typedef TAbSfEntry *PAbSfEntry;

#pragma pack(push,1)
struct DECLSPEC_DRECORD TAbSfEntry
{
	union
	{
		struct 
		{
			int L;
		};
		struct 
		{
			System::Word Code;
			System::Byte Value;
			System::Byte BitLength;
		};
		
	};
};
#pragma pack(pop)


struct TAbSfTree;
typedef TAbSfTree *PAbSfTree;

#pragma pack(push,1)
struct DECLSPEC_DRECORD TAbSfTree
{
public:
	short Entries;
	short MaxLength;
	System::StaticArray<TAbSfEntry, 257> Entry;
};
#pragma pack(pop)


struct TInfoZipUnicodePathRec;
typedef TInfoZipUnicodePathRec *PInfoZipUnicodePathRec;

#pragma pack(push,1)
struct DECLSPEC_DRECORD TInfoZipUnicodePathRec
{
public:
	System::Byte Version;
	int NameCRC32;
	System::StaticArray<char, 1> UnicodeName;
};
#pragma pack(pop)


struct TXceedUnicodePathRec;
typedef TXceedUnicodePathRec *PXceedUnicodePathRec;

#pragma pack(push,1)
struct DECLSPEC_DRECORD TXceedUnicodePathRec
{
public:
	unsigned Signature;
	int Length;
	System::StaticArray<System::WideChar, 1> UnicodeName;
};
#pragma pack(pop)


struct TZip64LocalHeaderRec;
typedef TZip64LocalHeaderRec *PZip64LocalHeaderRec;

#pragma pack(push,1)
struct DECLSPEC_DRECORD TZip64LocalHeaderRec
{
public:
	__int64 UncompressedSize;
	__int64 CompressedSize;
};
#pragma pack(pop)


enum DECLSPEC_DENUM TAbZipCompressionMethod : unsigned char { cmStored, cmShrunk, cmReduced1, cmReduced2, cmReduced3, cmReduced4, cmImploded, cmTokenized, cmDeflated, cmEnhancedDeflated, cmDCLImploded, cmBzip2 = 12, cmLZMA = 14, cmIBMTerse = 18, cmLZ77, cmJPEG = 96, cmWavPack, cmPPMd };

enum DECLSPEC_DENUM TAbZipSupportedMethod : unsigned char { smStored, smDeflated, smBestMethod };

enum DECLSPEC_DENUM TAbZipHostOS : unsigned char { hosDOS, hosAmiga, hosVAX, hosUnix, hosVMCMS, hosAtari, hosOS2, hosMacintosh, hosZSystem, hosCPM, hosNTFS, hosTOPS20 = 10, hosMVS, hosWinNT = 11, hosVSE, hosQDOS = 12, hosRISC, hosVFAT, hosAltMVS, hosBeOS, hosTandem, hosOS400, hosTHEOS = 18, hosDarwin, hosAtheOS = 30 };

enum DECLSPEC_DENUM TAbZipDictionarySize : unsigned char { dsInvalid, ds4K, ds8K };

enum DECLSPEC_DENUM TAbZipDeflationOption : unsigned char { doInvalid, doNormal, doMaximum, doFast, doSuperFast };

typedef void __fastcall (__closure *TAbNeedPasswordEvent)(System::TObject* Sender, System::AnsiString &NewPassword);

class DELPHICLASS TAbZipDataDescriptor;
class PASCALIMPLEMENTATION TAbZipDataDescriptor : public System::TObject
{
	typedef System::TObject inherited;
	
protected:
	int FCRC32;
	__int64 FCompressedSize;
	__int64 FUncompressedSize;
	
public:
	void __fastcall SaveToStream(System::Classes::TStream* Stream);
	__property int CRC32 = {read=FCRC32, write=FCRC32, nodefault};
	__property __int64 CompressedSize = {read=FCompressedSize, write=FCompressedSize};
	__property __int64 UncompressedSize = {read=FUncompressedSize, write=FUncompressedSize};
public:
	/* TObject.Create */ inline __fastcall TAbZipDataDescriptor(void) : System::TObject() { }
	/* TObject.Destroy */ inline __fastcall virtual ~TAbZipDataDescriptor(void) { }
	
};


class DELPHICLASS TAbZipFileHeader;
#pragma pack(push,4)
class PASCALIMPLEMENTATION TAbZipFileHeader : public System::TObject
{
	typedef System::TObject inherited;
	
protected:
	int FValidSignature;
	int FSignature;
	System::Word FVersionNeededToExtract;
	System::Word FGeneralPurposeBitFlag;
	System::Word FCompressionMethod;
	System::Word FLastModFileTime;
	System::Word FLastModFileDate;
	int FCRC32;
	unsigned FCompressedSize;
	unsigned FUncompressedSize;
	System::AnsiString FFileName;
	Abarctyp::TAbExtraField* FExtraField;
	TAbZipCompressionMethod __fastcall GetCompressionMethod(void);
	double __fastcall GetCompressionRatio(void);
	bool __fastcall GetDataDescriptor(void);
	TAbZipDeflationOption __fastcall GetDeflationOption(void);
	TAbZipDictionarySize __fastcall GetDictionarySize(void);
	bool __fastcall GetEncrypted(void);
	bool __fastcall GetIsUTF8(void);
	System::Byte __fastcall GetShannonFanoTreeCount(void);
	bool __fastcall GetValid(void);
	void __fastcall SetCompressionMethod(TAbZipCompressionMethod Value);
	void __fastcall SetIsUTF8(bool Value);
	
public:
	__fastcall TAbZipFileHeader(void);
	__fastcall virtual ~TAbZipFileHeader(void);
	__property int Signature = {read=FSignature, write=FSignature, nodefault};
	__property System::Word VersionNeededToExtract = {read=FVersionNeededToExtract, write=FVersionNeededToExtract, nodefault};
	__property System::Word GeneralPurposeBitFlag = {read=FGeneralPurposeBitFlag, write=FGeneralPurposeBitFlag, nodefault};
	__property TAbZipCompressionMethod CompressionMethod = {read=GetCompressionMethod, write=SetCompressionMethod, nodefault};
	__property System::Word LastModFileTime = {read=FLastModFileTime, write=FLastModFileTime, nodefault};
	__property System::Word LastModFileDate = {read=FLastModFileDate, write=FLastModFileDate, nodefault};
	__property int CRC32 = {read=FCRC32, write=FCRC32, nodefault};
	__property unsigned CompressedSize = {read=FCompressedSize, write=FCompressedSize, nodefault};
	__property unsigned UncompressedSize = {read=FUncompressedSize, write=FUncompressedSize, nodefault};
	__property System::AnsiString FileName = {read=FFileName, write=FFileName};
	__property Abarctyp::TAbExtraField* ExtraField = {read=FExtraField};
	__property double CompressionRatio = {read=GetCompressionRatio};
	__property TAbZipDeflationOption DeflationOption = {read=GetDeflationOption, nodefault};
	__property TAbZipDictionarySize DictionarySize = {read=GetDictionarySize, nodefault};
	__property bool HasDataDescriptor = {read=GetDataDescriptor, nodefault};
	__property bool IsValid = {read=GetValid, nodefault};
	__property bool IsEncrypted = {read=GetEncrypted, nodefault};
	__property bool IsUTF8 = {read=GetIsUTF8, write=SetIsUTF8, nodefault};
	__property System::Byte ShannonFanoTreeCount = {read=GetShannonFanoTreeCount, nodefault};
};

#pragma pack(pop)

class DELPHICLASS TAbZipLocalFileHeader;
#pragma pack(push,4)
class PASCALIMPLEMENTATION TAbZipLocalFileHeader : public TAbZipFileHeader
{
	typedef TAbZipFileHeader inherited;
	
public:
	__fastcall TAbZipLocalFileHeader(void);
	__fastcall virtual ~TAbZipLocalFileHeader(void);
	void __fastcall LoadFromStream(System::Classes::TStream* Stream);
	void __fastcall SaveToStream(System::Classes::TStream* Stream);
};

#pragma pack(pop)

class DELPHICLASS TAbZipDirectoryFileHeader;
#pragma pack(push,4)
class PASCALIMPLEMENTATION TAbZipDirectoryFileHeader : public TAbZipFileHeader
{
	typedef TAbZipFileHeader inherited;
	
protected:
	System::Word FVersionMadeBy;
	System::Word FDiskNumberStart;
	System::Word FInternalFileAttributes;
	unsigned FExternalFileAttributes;
	unsigned FRelativeOffset;
	System::AnsiString FFileComment;
	
public:
	__fastcall TAbZipDirectoryFileHeader(void);
	__fastcall virtual ~TAbZipDirectoryFileHeader(void);
	void __fastcall LoadFromStream(System::Classes::TStream* Stream);
	void __fastcall SaveToStream(System::Classes::TStream* Stream);
	__property System::Word VersionMadeBy = {read=FVersionMadeBy, write=FVersionMadeBy, nodefault};
	__property System::Word DiskNumberStart = {read=FDiskNumberStart, write=FDiskNumberStart, nodefault};
	__property System::Word InternalFileAttributes = {read=FInternalFileAttributes, write=FInternalFileAttributes, nodefault};
	__property unsigned ExternalFileAttributes = {read=FExternalFileAttributes, write=FExternalFileAttributes, nodefault};
	__property unsigned RelativeOffset = {read=FRelativeOffset, write=FRelativeOffset, nodefault};
	__property System::AnsiString FileComment = {read=FFileComment, write=FFileComment};
};

#pragma pack(pop)

class DELPHICLASS TAbZipDirectoryFileFooter;
class PASCALIMPLEMENTATION TAbZipDirectoryFileFooter : public System::TObject
{
	typedef System::TObject inherited;
	
protected:
	unsigned FDiskNumber;
	unsigned FStartDiskNumber;
	__int64 FEntriesOnDisk;
	__int64 FTotalEntries;
	__int64 FDirectorySize;
	__int64 FDirectoryOffset;
	System::AnsiString FZipfileComment;
	bool __fastcall GetIsZip64(void);
	
public:
	void __fastcall LoadFromStream(System::Classes::TStream* Stream);
	void __fastcall LoadZip64FromStream(System::Classes::TStream* Stream);
	void __fastcall SaveToStream(System::Classes::TStream* Stream, __int64 aZip64TailOffset = -1LL);
	__property unsigned DiskNumber = {read=FDiskNumber, write=FDiskNumber, nodefault};
	__property __int64 EntriesOnDisk = {read=FEntriesOnDisk, write=FEntriesOnDisk};
	__property __int64 TotalEntries = {read=FTotalEntries, write=FTotalEntries};
	__property __int64 DirectorySize = {read=FDirectorySize, write=FDirectorySize};
	__property __int64 DirectoryOffset = {read=FDirectoryOffset, write=FDirectoryOffset};
	__property unsigned StartDiskNumber = {read=FStartDiskNumber, write=FStartDiskNumber, nodefault};
	__property System::AnsiString ZipfileComment = {read=FZipfileComment, write=FZipfileComment};
	__property bool IsZip64 = {read=GetIsZip64, nodefault};
public:
	/* TObject.Create */ inline __fastcall TAbZipDirectoryFileFooter(void) : System::TObject() { }
	/* TObject.Destroy */ inline __fastcall virtual ~TAbZipDirectoryFileFooter(void) { }
	
};


class DELPHICLASS TAbZipItem;
class PASCALIMPLEMENTATION TAbZipItem : public Abarctyp::TAbArchiveItem
{
	typedef Abarctyp::TAbArchiveItem inherited;
	
protected:
	TAbZipDirectoryFileHeader* FItemInfo;
	unsigned FDiskNumberStart;
	Abarctyp::TAbExtraField* FLFHExtraField;
	__int64 FRelativeOffset;
	TAbZipCompressionMethod __fastcall GetCompressionMethod(void);
	double __fastcall GetCompressionRatio(void);
	TAbZipDeflationOption __fastcall GetDeflationOption(void);
	TAbZipDictionarySize __fastcall GetDictionarySize(void);
	Abarctyp::TAbExtraField* __fastcall GetExtraField(void);
	System::AnsiString __fastcall GetFileComment(void);
	System::Word __fastcall GetGeneralPurposeBitFlag(void);
	TAbZipHostOS __fastcall GetHostOS(void);
	System::Word __fastcall GetInternalFileAttributes(void);
	System::AnsiString __fastcall GetRawFileName(void);
	System::Byte __fastcall GetShannonFanoTreeCount(void);
	System::Word __fastcall GetVersionMadeBy(void);
	System::Word __fastcall GetVersionNeededToExtract(void);
	void __fastcall SaveCDHToStream(System::Classes::TStream* Stream);
	void __fastcall SaveDDToStream(System::Classes::TStream* Stream);
	void __fastcall SaveLFHToStream(System::Classes::TStream* Stream);
	void __fastcall SetCompressionMethod(TAbZipCompressionMethod Value);
	void __fastcall SetDiskNumberStart(unsigned Value);
	void __fastcall SetFileComment(const System::AnsiString Value);
	void __fastcall SetGeneralPurposeBitFlag(System::Word Value);
	void __fastcall SetHostOS(TAbZipHostOS Value);
	void __fastcall SetInternalFileAttributes(System::Word Value);
	void __fastcall SetRelativeOffset(__int64 Value);
	void __fastcall SetVersionMadeBy(System::Word Value);
	void __fastcall SetVersionNeededToExtract(System::Word Value);
	void __fastcall UpdateVersionNeededToExtract(void);
	void __fastcall UpdateZip64ExtraHeader(void);
	virtual int __fastcall GetCRC32(void);
	virtual unsigned __fastcall GetExternalFileAttributes(void);
	virtual bool __fastcall GetIsDirectory(void);
	virtual bool __fastcall GetIsEncrypted(void);
	virtual System::Word __fastcall GetLastModFileDate(void);
	virtual System::Word __fastcall GetLastModFileTime(void);
	virtual int __fastcall GetNativeFileAttributes(void);
	virtual void __fastcall SetCompressedSize(const __int64 Value);
	virtual void __fastcall SetCRC32(const int Value);
	virtual void __fastcall SetExternalFileAttributes(unsigned Value);
	virtual void __fastcall SetFileName(const System::UnicodeString Value);
	virtual void __fastcall SetLastModFileDate(const System::Word Value);
	virtual void __fastcall SetLastModFileTime(const System::Word Value);
	virtual void __fastcall SetUncompressedSize(const __int64 Value);
	
public:
	__fastcall TAbZipItem(void);
	__fastcall virtual ~TAbZipItem(void);
	void __fastcall LoadFromStream(System::Classes::TStream* Stream);
	__property TAbZipCompressionMethod CompressionMethod = {read=GetCompressionMethod, write=SetCompressionMethod, nodefault};
	__property double CompressionRatio = {read=GetCompressionRatio};
	__property TAbZipDeflationOption DeflationOption = {read=GetDeflationOption, nodefault};
	__property TAbZipDictionarySize DictionarySize = {read=GetDictionarySize, nodefault};
	__property unsigned DiskNumberStart = {read=FDiskNumberStart, write=SetDiskNumberStart, nodefault};
	__property Abarctyp::TAbExtraField* ExtraField = {read=GetExtraField};
	__property System::AnsiString FileComment = {read=GetFileComment, write=SetFileComment};
	__property TAbZipHostOS HostOS = {read=GetHostOS, write=SetHostOS, nodefault};
	__property System::Word InternalFileAttributes = {read=GetInternalFileAttributes, write=SetInternalFileAttributes, nodefault};
	__property System::Word GeneralPurposeBitFlag = {read=GetGeneralPurposeBitFlag, write=SetGeneralPurposeBitFlag, nodefault};
	__property Abarctyp::TAbExtraField* LFHExtraField = {read=FLFHExtraField};
	__property System::AnsiString RawFileName = {read=GetRawFileName};
	__property __int64 RelativeOffset = {read=FRelativeOffset, write=SetRelativeOffset};
	__property System::Byte ShannonFanoTreeCount = {read=GetShannonFanoTreeCount, nodefault};
	__property System::Word VersionMadeBy = {read=GetVersionMadeBy, write=SetVersionMadeBy, nodefault};
	__property System::Word VersionNeededToExtract = {read=GetVersionNeededToExtract, write=SetVersionNeededToExtract, nodefault};
};


class DELPHICLASS TAbZipArchive;
class PASCALIMPLEMENTATION TAbZipArchive : public Abarctyp::TAbArchive
{
	typedef Abarctyp::TAbArchive inherited;
	
public:
	TAbZipItem* operator[](int Index) { return Items[Index]; }
	
protected:
	TAbZipSupportedMethod FCompressionMethodToUse;
	TAbZipDeflationOption FDeflationOption;
	TAbZipDirectoryFileFooter* FInfo;
	bool FIsExecutable;
	System::AnsiString FPassword;
	System::Byte FPasswordRetries;
	unsigned FStubSize;
	Abarctyp::TAbArchiveItemExtractEvent FExtractHelper;
	Abarctyp::TAbArchiveItemExtractToStreamEvent FExtractToStreamHelper;
	Abarctyp::TAbArchiveItemTestEvent FTestHelper;
	Abarctyp::TAbArchiveItemInsertEvent FInsertHelper;
	Abarctyp::TAbArchiveItemInsertFromStreamEvent FInsertFromStreamHelper;
	TAbNeedPasswordEvent FOnNeedPassword;
	Abarctyp::TAbRequestDiskEvent FOnRequestLastDisk;
	Abarctyp::TAbRequestNthDiskEvent FOnRequestNthDisk;
	Abarctyp::TAbRequestDiskEvent FOnRequestBlankDisk;
	void __fastcall DoExtractHelper(int Index, const System::UnicodeString NewName);
	void __fastcall DoExtractToStreamHelper(int Index, System::Classes::TStream* aStream);
	void __fastcall DoTestHelper(int Index);
	void __fastcall DoInsertHelper(int Index, System::Classes::TStream* OutStream);
	void __fastcall DoInsertFromStreamHelper(int Index, System::Classes::TStream* OutStream);
	TAbZipItem* __fastcall GetItem(int Index);
	System::AnsiString __fastcall GetZipfileComment(void);
	void __fastcall PutItem(int Index, TAbZipItem* Value);
	void __fastcall DoRequestDisk(const System::UnicodeString AMessage, bool &Abort);
	virtual void __fastcall DoRequestLastDisk(bool &Abort);
	virtual void __fastcall DoRequestNthDisk(System::TObject* Sender, System::Byte DiskNumber, bool &Abort);
	virtual void __fastcall DoRequestBlankDisk(System::TObject* Sender, bool &Abort);
	virtual void __fastcall ExtractItemAt(int Index, const System::UnicodeString UseName);
	virtual void __fastcall ExtractItemToStreamAt(int Index, System::Classes::TStream* aStream);
	virtual void __fastcall TestItemAt(int Index);
	virtual System::UnicodeString __fastcall FixName(const System::UnicodeString Value);
	virtual bool __fastcall GetSupportsEmptyFolders(void);
	virtual void __fastcall LoadArchive(void);
	virtual void __fastcall SaveArchive(void);
	void __fastcall SetZipfileComment(const System::AnsiString Value);
	__property bool IsExecutable = {read=FIsExecutable, write=FIsExecutable, nodefault};
	
public:
	void __fastcall DoRequestImage(System::TObject* Sender, int ImageNumber, System::UnicodeString &ImageName, bool &Abort);
	__fastcall virtual TAbZipArchive(System::Classes::TStream* aStream, const System::UnicodeString ArchiveName);
	__fastcall virtual ~TAbZipArchive(void);
	virtual Abarctyp::TAbArchiveItem* __fastcall CreateItem(const System::UnicodeString FileName);
	__property TAbZipSupportedMethod CompressionMethodToUse = {read=FCompressionMethodToUse, write=FCompressionMethodToUse, nodefault};
	__property TAbZipDeflationOption DeflationOption = {read=FDeflationOption, write=FDeflationOption, nodefault};
	__property Abarctyp::TAbArchiveItemExtractEvent ExtractHelper = {read=FExtractHelper, write=FExtractHelper};
	__property Abarctyp::TAbArchiveItemExtractToStreamEvent ExtractToStreamHelper = {read=FExtractToStreamHelper, write=FExtractToStreamHelper};
	__property Abarctyp::TAbArchiveItemTestEvent TestHelper = {read=FTestHelper, write=FTestHelper};
	__property Abarctyp::TAbArchiveItemInsertEvent InsertHelper = {read=FInsertHelper, write=FInsertHelper};
	__property Abarctyp::TAbArchiveItemInsertFromStreamEvent InsertFromStreamHelper = {read=FInsertFromStreamHelper, write=FInsertFromStreamHelper};
	__property System::AnsiString Password = {read=FPassword, write=FPassword};
	__property System::Byte PasswordRetries = {read=FPasswordRetries, write=FPasswordRetries, default=3};
	__property unsigned StubSize = {read=FStubSize, nodefault};
	__property System::AnsiString ZipfileComment = {read=GetZipfileComment, write=SetZipfileComment};
	__property TAbZipItem* Items[int Index] = {read=GetItem, write=PutItem/*, default*/};
	__property TAbNeedPasswordEvent OnNeedPassword = {read=FOnNeedPassword, write=FOnNeedPassword};
	__property Abarctyp::TAbRequestDiskEvent OnRequestLastDisk = {read=FOnRequestLastDisk, write=FOnRequestLastDisk};
	__property Abarctyp::TAbRequestNthDiskEvent OnRequestNthDisk = {read=FOnRequestNthDisk, write=FOnRequestNthDisk};
	__property Abarctyp::TAbRequestDiskEvent OnRequestBlankDisk = {read=FOnRequestBlankDisk, write=FOnRequestBlankDisk};
protected:
	/* TAbArchive.CreateInit */ inline __fastcall TAbZipArchive(void) : Abarctyp::TAbArchive() { }
	
public:
	/* TAbArchive.Create */ inline __fastcall virtual TAbZipArchive(const System::UnicodeString FileName, System::Word Mode) : Abarctyp::TAbArchive(FileName, Mode) { }
	
};


//-- var, const, procedure ---------------------------------------------------
static const System::Int8 Ab_ZipVersion = System::Int8(0x3f);
extern DELPHI_PACKAGE int Ab_ZipLocalFileHeaderSignature;
extern DELPHI_PACKAGE int Ab_ZipDataDescriptorSignature;
extern DELPHI_PACKAGE int Ab_ZipCentralDirectoryFileHeaderSignature;
extern DELPHI_PACKAGE int Ab_Zip64EndCentralDirectorySignature;
extern DELPHI_PACKAGE int Ab_Zip64EndCentralDirectoryLocatorSignature;
extern DELPHI_PACKAGE int Ab_ZipEndCentralDirectorySignature;
extern DELPHI_PACKAGE int Ab_ZipSpannedSetSignature;
extern DELPHI_PACKAGE int Ab_ZipPossiblySpannedSignature;
extern DELPHI_PACKAGE System::Word Ab_GeneralZipSignature;
extern DELPHI_PACKAGE int Ab_ArchiveExtraDataRecord;
extern DELPHI_PACKAGE int Ab_DigitalSignature;
extern DELPHI_PACKAGE System::Word Ab_WindowsExeSignature;
extern DELPHI_PACKAGE int Ab_LinuxExeSignature;
static const System::Int8 AbDefZipSpanningThreshold = System::Int8(0x0);
static const System::Int8 AbDefPasswordRetries = System::Int8(0x3);
static const System::Int8 AbFileIsEncryptedFlag = System::Int8(0x1);
static const System::Int8 AbHasDataDescriptorFlag = System::Int8(0x8);
static const System::Word AbLanguageEncodingFlag = System::Word(0x800);
extern DELPHI_PACKAGE System::Word Ab_Zip64SubfieldID;
extern DELPHI_PACKAGE System::Word Ab_InfoZipUnicodePathSubfieldID;
extern DELPHI_PACKAGE System::Word Ab_XceedUnicodePathSubfieldID;
extern DELPHI_PACKAGE unsigned Ab_XceedUnicodePathSignature;
static const TAbZipSupportedMethod AbDefCompressionMethodToUse = (TAbZipSupportedMethod)(2);
static const TAbZipDeflationOption AbDefDeflationOption = (TAbZipDeflationOption)(1);
extern DELPHI_PACKAGE Abutils::TAbArchiveType __fastcall VerifyZip(System::Classes::TStream* Strm);
extern DELPHI_PACKAGE Abutils::TAbArchiveType __fastcall VerifySelfExtracting(System::Classes::TStream* Strm);
extern DELPHI_PACKAGE System::UnicodeString __fastcall ZipCompressionMethodToString(TAbZipCompressionMethod aMethod);
extern DELPHI_PACKAGE __int64 __fastcall FindCentralDirectoryTail(System::Classes::TStream* aStream);
extern DELPHI_PACKAGE void __fastcall MakeSelfExtracting(System::Classes::TStream* StubStream, System::Classes::TStream* ZipStream, System::Classes::TStream* SelfExtractingStream);
}	/* namespace Abziptyp */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_ABZIPTYP)
using namespace Abziptyp;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// AbziptypHPP
