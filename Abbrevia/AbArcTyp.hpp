// CodeGear C++Builder
// Copyright (c) 1995, 2012 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'AbArcTyp.pas' rev: 24.00 (Windows)

#ifndef AbarctypHPP
#define AbarctypHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <Winapi.Windows.hpp>	// Pascal unit
#include <System.Classes.hpp>	// Pascal unit
#include <System.Types.hpp>	// Pascal unit
#include <AbUtils.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Abarctyp
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TAbArchiveItem;
class PASCALIMPLEMENTATION TAbArchiveItem : public System::TObject
{
	typedef System::TObject inherited;
	
protected:
	TAbArchiveItem* NextItem;
	Abutils::TAbArchiveAction FAction;
	__int64 FCompressedSize;
	int FCRC32;
	System::UnicodeString FDiskFileName;
	unsigned FExternalFileAttributes;
	System::UnicodeString FFileName;
	bool FIsEncrypted;
	System::Word FLastModFileTime;
	System::Word FLastModFileDate;
	bool FTagged;
	__int64 FUncompressedSize;
	virtual __int64 __fastcall GetCompressedSize(void);
	virtual int __fastcall GetCRC32(void);
	System::UnicodeString __fastcall GetDiskPath(void);
	virtual unsigned __fastcall GetExternalFileAttributes(void);
	virtual System::UnicodeString __fastcall GetFileName(void);
	virtual bool __fastcall GetIsDirectory(void);
	virtual bool __fastcall GetIsEncrypted(void);
	virtual System::Word __fastcall GetLastModFileDate(void);
	virtual System::Word __fastcall GetLastModFileTime(void);
	virtual int __fastcall GetNativeFileAttributes(void);
	System::UnicodeString __fastcall GetStoredPath(void);
	virtual __int64 __fastcall GetUncompressedSize(void);
	virtual void __fastcall SetCompressedSize(const __int64 Value);
	virtual void __fastcall SetCRC32(const int Value);
	virtual void __fastcall SetExternalFileAttributes(unsigned Value);
	virtual void __fastcall SetFileName(const System::UnicodeString Value);
	virtual void __fastcall SetIsEncrypted(bool Value);
	virtual void __fastcall SetLastModFileDate(const System::Word Value);
	virtual void __fastcall SetLastModFileTime(const System::Word Value);
	virtual void __fastcall SetUncompressedSize(const __int64 Value);
	virtual System::TDateTime __fastcall GetLastModTimeAsDateTime(void);
	virtual void __fastcall SetLastModTimeAsDateTime(const System::TDateTime Value);
	
public:
	__fastcall TAbArchiveItem(void);
	__fastcall virtual ~TAbArchiveItem(void);
	bool __fastcall MatchesDiskName(const System::UnicodeString FileMask);
	bool __fastcall MatchesStoredName(const System::UnicodeString FileMask);
	bool __fastcall MatchesStoredNameEx(const System::UnicodeString FileMask);
	__property Abutils::TAbArchiveAction Action = {read=FAction, write=FAction, nodefault};
	__property __int64 CompressedSize = {read=GetCompressedSize, write=SetCompressedSize};
	__property int CRC32 = {read=GetCRC32, write=SetCRC32, nodefault};
	__property System::UnicodeString DiskFileName = {read=FDiskFileName, write=FDiskFileName};
	__property System::UnicodeString DiskPath = {read=GetDiskPath};
	__property unsigned ExternalFileAttributes = {read=GetExternalFileAttributes, write=SetExternalFileAttributes, nodefault};
	__property System::UnicodeString FileName = {read=GetFileName, write=SetFileName};
	__property bool IsDirectory = {read=GetIsDirectory, nodefault};
	__property bool IsEncrypted = {read=GetIsEncrypted, write=SetIsEncrypted, nodefault};
	__property System::Word LastModFileDate = {read=GetLastModFileDate, write=SetLastModFileDate, nodefault};
	__property System::Word LastModFileTime = {read=GetLastModFileTime, write=SetLastModFileTime, nodefault};
	__property int NativeFileAttributes = {read=GetNativeFileAttributes, nodefault};
	__property System::UnicodeString StoredPath = {read=GetStoredPath};
	__property bool Tagged = {read=FTagged, write=FTagged, nodefault};
	__property __int64 UncompressedSize = {read=GetUncompressedSize, write=SetUncompressedSize};
	__property System::TDateTime LastModTimeAsDateTime = {read=GetLastModTimeAsDateTime, write=SetLastModTimeAsDateTime};
};


class DELPHICLASS TAbArchiveListEnumerator;
class DELPHICLASS TAbArchiveList;
#pragma pack(push,4)
class PASCALIMPLEMENTATION TAbArchiveListEnumerator : public System::TObject
{
	typedef System::TObject inherited;
	
private:
	int FIndex;
	TAbArchiveList* FList;
	
public:
	__fastcall TAbArchiveListEnumerator(TAbArchiveList* aList);
	TAbArchiveItem* __fastcall GetCurrent(void);
	bool __fastcall MoveNext(void);
	__property TAbArchiveItem* Current = {read=GetCurrent};
public:
	/* TObject.Destroy */ inline __fastcall virtual ~TAbArchiveListEnumerator(void) { }
	
};

#pragma pack(pop)

#pragma pack(push,4)
class PASCALIMPLEMENTATION TAbArchiveList : public System::TObject
{
	typedef System::TObject inherited;
	
public:
	TAbArchiveItem* operator[](int Index) { return Items[Index]; }
	
protected:
	System::Classes::TList* FList;
	bool FOwnsItems;
	System::StaticArray<TAbArchiveItem*, 1021> HashTable;
	int __fastcall GenerateHash(const System::UnicodeString S);
	int __fastcall GetCount(void);
	TAbArchiveItem* __fastcall Get(int Index);
	void __fastcall Put(int Index, TAbArchiveItem* Item);
	void __fastcall UpdateHash(TAbArchiveItem* aItem, const System::UnicodeString aOldFileName);
	
public:
	__fastcall TAbArchiveList(bool AOwnsItems);
	__fastcall virtual ~TAbArchiveList(void);
	int __fastcall Add(TAbArchiveItem* aItem);
	void __fastcall Clear(void);
	void __fastcall Delete(int Index);
	int __fastcall Find(const System::UnicodeString FN);
	TAbArchiveListEnumerator* __fastcall GetEnumerator(void);
	bool __fastcall IsActiveDupe(const System::UnicodeString FN);
	__property int Count = {read=GetCount, nodefault};
	__property TAbArchiveItem* Items[int Index] = {read=Get, write=Put/*, default*/};
};

#pragma pack(pop)

enum DECLSPEC_DENUM TAbStoreOption : unsigned char { soStripDrive, soStripPath, soRemoveDots, soRecurse, soFreshen, soReplace };

typedef System::Set<TAbStoreOption, TAbStoreOption::soStripDrive, TAbStoreOption::soReplace>  TAbStoreOptions;

enum DECLSPEC_DENUM TAbExtractOption : unsigned char { eoCreateDirs, eoRestorePath };

typedef System::Set<TAbExtractOption, TAbExtractOption::eoCreateDirs, TAbExtractOption::eoRestorePath>  TAbExtractOptions;

enum DECLSPEC_DENUM TAbArchiveStatus : unsigned char { asInvalid, asIdle, asBusy };

typedef void __fastcall (__closure *TAbArchiveEvent)(System::TObject* Sender);

typedef void __fastcall (__closure *TAbArchiveConfirmEvent)(System::TObject* Sender, bool &Confirm);

typedef void __fastcall (__closure *TAbArchiveProgressEvent)(System::TObject* Sender, System::Byte Progress, bool &Abort);

typedef void __fastcall (__closure *TAbArchiveItemEvent)(System::TObject* Sender, TAbArchiveItem* Item);

typedef void __fastcall (__closure *TAbArchiveItemConfirmEvent)(System::TObject* Sender, TAbArchiveItem* Item, Abutils::TAbProcessType ProcessType, bool &Confirm);

typedef void __fastcall (__closure *TAbConfirmOverwriteEvent)(System::UnicodeString &Name, bool &Confirm);

typedef void __fastcall (__closure *TAbArchiveItemFailureEvent)(System::TObject* Sender, TAbArchiveItem* Item, Abutils::TAbProcessType ProcessType, Abutils::TAbErrorClass ErrorClass, int ErrorCode);

typedef void __fastcall (__closure *TAbArchiveItemExtractEvent)(System::TObject* Sender, TAbArchiveItem* Item, const System::UnicodeString NewName);

typedef void __fastcall (__closure *TAbArchiveItemExtractToStreamEvent)(System::TObject* Sender, TAbArchiveItem* Item, System::Classes::TStream* OutStream);

typedef void __fastcall (__closure *TAbArchiveItemTestEvent)(System::TObject* Sender, TAbArchiveItem* Item);

typedef void __fastcall (__closure *TAbArchiveItemInsertEvent)(System::TObject* Sender, TAbArchiveItem* Item, System::Classes::TStream* OutStream);

typedef void __fastcall (__closure *TAbArchiveItemInsertFromStreamEvent)(System::TObject* Sender, TAbArchiveItem* Item, System::Classes::TStream* OutStream, System::Classes::TStream* InStream);

typedef void __fastcall (__closure *TAbArchiveItemProgressEvent)(System::TObject* Sender, TAbArchiveItem* Item, System::Byte Progress, bool &Abort);

typedef void __fastcall (__closure *TAbProgressEvent)(System::Byte Progress, bool &Abort);

typedef void __fastcall (__closure *TAbRequestDiskEvent)(System::TObject* Sender, bool &Abort);

typedef void __fastcall (__closure *TAbRequestImageEvent)(System::TObject* Sender, int ImageNumber, System::UnicodeString &ImageName, bool &Abort);

typedef void __fastcall (__closure *TAbRequestNthDiskEvent)(System::TObject* Sender, System::Byte DiskNumber, bool &Abort);

class DELPHICLASS TAbArchiveStreamHelper;
#pragma pack(push,4)
class PASCALIMPLEMENTATION TAbArchiveStreamHelper : public System::TObject
{
	typedef System::TObject inherited;
	
protected:
	System::Classes::TStream* FStream;
	
public:
	__fastcall TAbArchiveStreamHelper(System::Classes::TStream* AStream);
	virtual void __fastcall ExtractItemData(System::Classes::TStream* AStream) = 0 ;
	virtual bool __fastcall FindFirstItem(void) = 0 ;
	virtual bool __fastcall FindNextItem(void) = 0 ;
	virtual void __fastcall ReadHeader(void) = 0 ;
	virtual void __fastcall ReadTail(void) = 0 ;
	virtual bool __fastcall SeekItem(int Index) = 0 ;
	virtual void __fastcall WriteArchiveHeader(void) = 0 ;
	virtual void __fastcall WriteArchiveItem(System::Classes::TStream* AStream) = 0 ;
	virtual void __fastcall WriteArchiveTail(void) = 0 ;
	virtual int __fastcall GetItemCount(void) = 0 ;
public:
	/* TObject.Destroy */ inline __fastcall virtual ~TAbArchiveStreamHelper(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS TAbArchive;
class PASCALIMPLEMENTATION TAbArchive : public System::TObject
{
	typedef System::TObject inherited;
	
public:
	System::Classes::TStream* FStream;
	TAbArchiveStatus FStatus;
	
protected:
	System::UnicodeString FArchiveName;
	bool FAutoSave;
	System::UnicodeString FBaseDirectory;
	TAbArchiveItem* FCurrentItem;
	bool FDOSMode;
	TAbExtractOptions FExtractOptions;
	System::Word FImageNumber;
	System::Classes::TStream* FInStream;
	bool FIsDirty;
	__int64 FSpanningThreshold;
	TAbArchiveList* FItemList;
	System::UnicodeString FLogFile;
	bool FLogging;
	System::Classes::TFileStream* FLogStream;
	System::Word FMode;
	bool FOwnsStream;
	bool FSpanned;
	TAbStoreOptions FStoreOptions;
	System::UnicodeString FTempDir;
	TAbArchiveItemFailureEvent FOnProcessItemFailure;
	TAbArchiveProgressEvent FOnArchiveProgress;
	TAbArchiveProgressEvent FOnArchiveSaveProgress;
	TAbArchiveItemProgressEvent FOnArchiveItemProgress;
	TAbArchiveItemConfirmEvent FOnConfirmProcessItem;
	TAbConfirmOverwriteEvent FOnConfirmOverwrite;
	TAbArchiveConfirmEvent FOnConfirmSave;
	TAbArchiveEvent FOnLoad;
	TAbProgressEvent FOnProgress;
	TAbRequestImageEvent FOnRequestImage;
	TAbArchiveEvent FOnSave;
	__fastcall TAbArchive(void);
	void __fastcall CheckValid(void);
	bool __fastcall ConfirmPath(TAbArchiveItem* Item, const System::UnicodeString NewName, /* out */ System::UnicodeString &UseName);
	void __fastcall FreshenAt(int Index);
	bool __fastcall FreshenRequired(TAbArchiveItem* Item);
	void __fastcall GetFreshenTarget(TAbArchiveItem* Item);
	int __fastcall GetItemCount(void);
	void __fastcall MakeLogEntry(const System::UnicodeString FN, Abutils::TAbLogType LT);
	void __fastcall ReplaceAt(int Index);
	void __fastcall SaveIfNeeded(TAbArchiveItem* aItem);
	void __fastcall SetBaseDirectory(System::UnicodeString Value);
	void __fastcall SetLogFile(const System::UnicodeString Value);
	void __fastcall SetLogging(bool Value);
	virtual TAbArchiveItem* __fastcall CreateItem(const System::UnicodeString FileSpec) = 0 ;
	virtual void __fastcall ExtractItemAt(int Index, const System::UnicodeString UseName) = 0 ;
	virtual void __fastcall ExtractItemToStreamAt(int Index, System::Classes::TStream* aStream) = 0 ;
	virtual void __fastcall LoadArchive(void) = 0 ;
	virtual void __fastcall SaveArchive(void) = 0 ;
	virtual void __fastcall TestItemAt(int Index) = 0 ;
	virtual void __fastcall DoProcessItemFailure(TAbArchiveItem* Item, Abutils::TAbProcessType ProcessType, Abutils::TAbErrorClass ErrorClass, int ErrorCode);
	virtual void __fastcall DoArchiveSaveProgress(System::Byte Progress, bool &Abort);
	virtual void __fastcall DoArchiveProgress(System::Byte Progress, bool &Abort);
	virtual void __fastcall DoArchiveItemProgress(TAbArchiveItem* Item, System::Byte Progress, bool &Abort);
	virtual void __fastcall DoConfirmOverwrite(System::UnicodeString &FileName, bool &Confirm);
	virtual void __fastcall DoConfirmProcessItem(TAbArchiveItem* Item, const Abutils::TAbProcessType ProcessType, bool &Confirm);
	virtual void __fastcall DoConfirmSave(bool &Confirm);
	virtual void __fastcall DoLoad(void);
	virtual void __fastcall DoProgress(System::Byte Progress, bool &Abort);
	virtual void __fastcall DoSave(void);
	virtual System::UnicodeString __fastcall FixName(const System::UnicodeString Value);
	virtual __int64 __fastcall GetSpanningThreshold(void);
	virtual bool __fastcall GetSupportsEmptyFolders(void);
	virtual void __fastcall SetSpanningThreshold(__int64 Value);
	__property System::Classes::TStream* InStream = {read=FInStream};
	
public:
	__fastcall virtual TAbArchive(const System::UnicodeString FileName, System::Word Mode);
	__fastcall virtual TAbArchive(System::Classes::TStream* aStream, const System::UnicodeString aArchiveName);
	__fastcall virtual ~TAbArchive(void);
	virtual void __fastcall Add(TAbArchiveItem* aItem);
	void __fastcall AddFiles(const System::UnicodeString FileMask, int SearchAttr);
	void __fastcall AddFilesEx(const System::UnicodeString FileMask, const System::UnicodeString ExclusionMask, int SearchAttr);
	void __fastcall AddFromStream(const System::UnicodeString NewName, System::Classes::TStream* aStream);
	void __fastcall ClearTags(void);
	void __fastcall Delete(TAbArchiveItem* aItem);
	void __fastcall DeleteAt(int Index);
	void __fastcall DeleteFiles(const System::UnicodeString FileMask);
	void __fastcall DeleteFilesEx(const System::UnicodeString FileMask, const System::UnicodeString ExclusionMask);
	void __fastcall DeleteTaggedItems(void);
	void __fastcall Extract(TAbArchiveItem* aItem, const System::UnicodeString NewName);
	void __fastcall ExtractAt(int Index, const System::UnicodeString NewName);
	void __fastcall ExtractFiles(const System::UnicodeString FileMask);
	void __fastcall ExtractFilesEx(const System::UnicodeString FileMask, const System::UnicodeString ExclusionMask);
	void __fastcall ExtractTaggedItems(void);
	void __fastcall ExtractToStream(const System::UnicodeString aFileName, System::Classes::TStream* aStream);
	int __fastcall FindFile(const System::UnicodeString aFileName);
	int __fastcall FindItem(TAbArchiveItem* aItem);
	void __fastcall Freshen(TAbArchiveItem* aItem);
	void __fastcall FreshenFiles(const System::UnicodeString FileMask);
	void __fastcall FreshenFilesEx(const System::UnicodeString FileMask, const System::UnicodeString ExclusionMask);
	void __fastcall FreshenTaggedItems(void);
	virtual void __fastcall Load(void);
	virtual void __fastcall Move(TAbArchiveItem* aItem, const System::UnicodeString NewStoredPath);
	void __fastcall Replace(TAbArchiveItem* aItem);
	virtual void __fastcall Save(void);
	void __fastcall TagItems(const System::UnicodeString FileMask);
	void __fastcall TestTaggedItems(void);
	void __fastcall UnTagItems(const System::UnicodeString FileMask);
	virtual void __fastcall DoDeflateProgress(int aPercentDone);
	virtual void __fastcall DoInflateProgress(int aPercentDone);
	virtual void __fastcall DoSpanningMediaRequest(System::TObject* Sender, int ImageNumber, System::UnicodeString &ImageName, bool &Abort);
	__property TAbProgressEvent OnProgress = {read=FOnProgress, write=FOnProgress};
	__property System::UnicodeString ArchiveName = {read=FArchiveName};
	__property bool AutoSave = {read=FAutoSave, write=FAutoSave, nodefault};
	__property System::UnicodeString BaseDirectory = {read=FBaseDirectory, write=SetBaseDirectory};
	__property int Count = {read=GetItemCount, nodefault};
	__property bool DOSMode = {read=FDOSMode, write=FDOSMode, nodefault};
	__property TAbExtractOptions ExtractOptions = {read=FExtractOptions, write=FExtractOptions, nodefault};
	__property bool IsDirty = {read=FIsDirty, write=FIsDirty, nodefault};
	__property TAbArchiveList* ItemList = {read=FItemList};
	__property System::UnicodeString LogFile = {read=FLogFile, write=SetLogFile};
	__property bool Logging = {read=FLogging, write=SetLogging, nodefault};
	__property System::Word Mode = {read=FMode, nodefault};
	__property bool Spanned = {read=FSpanned, nodefault};
	__property __int64 SpanningThreshold = {read=GetSpanningThreshold, write=SetSpanningThreshold};
	__property TAbArchiveStatus Status = {read=FStatus, nodefault};
	__property TAbStoreOptions StoreOptions = {read=FStoreOptions, write=FStoreOptions, nodefault};
	__property bool SupportsEmptyFolders = {read=GetSupportsEmptyFolders, nodefault};
	__property System::UnicodeString TempDirectory = {read=FTempDir, write=FTempDir};
	__property TAbArchiveItemFailureEvent OnProcessItemFailure = {read=FOnProcessItemFailure, write=FOnProcessItemFailure};
	__property TAbArchiveProgressEvent OnArchiveProgress = {read=FOnArchiveProgress, write=FOnArchiveProgress};
	__property TAbArchiveProgressEvent OnArchiveSaveProgress = {read=FOnArchiveSaveProgress, write=FOnArchiveSaveProgress};
	__property TAbArchiveItemProgressEvent OnArchiveItemProgress = {read=FOnArchiveItemProgress, write=FOnArchiveItemProgress};
	__property TAbArchiveItemConfirmEvent OnConfirmProcessItem = {read=FOnConfirmProcessItem, write=FOnConfirmProcessItem};
	__property TAbConfirmOverwriteEvent OnConfirmOverwrite = {read=FOnConfirmOverwrite, write=FOnConfirmOverwrite};
	__property TAbArchiveConfirmEvent OnConfirmSave = {read=FOnConfirmSave, write=FOnConfirmSave};
	__property TAbArchiveEvent OnLoad = {read=FOnLoad, write=FOnLoad};
	__property TAbRequestImageEvent OnRequestImage = {read=FOnRequestImage, write=FOnRequestImage};
	__property TAbArchiveEvent OnSave = {read=FOnSave, write=FOnSave};
};


struct TAbExtraSubField;
typedef TAbExtraSubField *PAbExtraSubField;

#pragma pack(push,1)
struct DECLSPEC_DRECORD TAbExtraSubField
{
private:
	struct DECLSPEC_DRECORD _TAbExtraSubField__1
	{
	};
	
	
	
public:
	System::Word ID;
	System::Word Len;
	_TAbExtraSubField__1 Data;
};
#pragma pack(pop)


class DELPHICLASS TAbExtraField;
#pragma pack(push,4)
class PASCALIMPLEMENTATION TAbExtraField : public System::TObject
{
	typedef System::TObject inherited;
	
private:
	System::TByteDynArray FBuffer;
	void __fastcall DeleteField(PAbExtraSubField aSubField);
	bool __fastcall FindField(System::Word aID, /* out */ PAbExtraSubField &aSubField);
	bool __fastcall FindNext(PAbExtraSubField &aCurField);
	int __fastcall GetCount(void);
	System::Word __fastcall GetID(int aIndex);
	void __fastcall SetBuffer(const System::TByteDynArray aValue);
	
protected:
	virtual void __fastcall Changed(void);
	
public:
	void __fastcall Assign(TAbExtraField* aSource);
	void __fastcall Clear(void);
	void __fastcall CloneFrom(TAbExtraField* aSource, System::Word aID);
	void __fastcall Delete(System::Word aID);
	bool __fastcall Get(System::Word aID, /* out */ void * &aData, /* out */ System::Word &aDataSize);
	bool __fastcall GetStream(System::Word aID, /* out */ System::Classes::TStream* &aStream);
	bool __fastcall Has(System::Word aID);
	void __fastcall LoadFromStream(System::Classes::TStream* aStream, System::Word aSize);
	void __fastcall Put(System::Word aID, const void *aData, System::Word aDataSize);
	__property int Count = {read=GetCount, nodefault};
	__property System::TByteDynArray Buffer = {read=FBuffer, write=SetBuffer};
	__property System::Word IDs[int aIndex] = {read=GetID};
public:
	/* TObject.Create */ inline __fastcall TAbExtraField(void) : System::TObject() { }
	/* TObject.Destroy */ inline __fastcall virtual ~TAbExtraField(void) { }
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
static const bool AbDefAutoSave = false;
#define AbDefExtractOptions (System::Set<TAbExtractOption, TAbExtractOption::eoCreateDirs, TAbExtractOption::eoRestorePath> () << TAbExtractOption::eoCreateDirs )
#define AbDefStoreOptions (System::Set<TAbStoreOption, TAbStoreOption::soStripDrive, TAbStoreOption::soReplace> () << TAbStoreOption::soStripDrive << TAbStoreOption::soRemoveDots )
static const System::Word AbBufferSize = System::Word(0x8000);
static const System::Int8 AbLastDisk = System::Int8(-1);
static const System::Int8 AbLastImage = System::Int8(-1);
}	/* namespace Abarctyp */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_ABARCTYP)
using namespace Abarctyp;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// AbarctypHPP
