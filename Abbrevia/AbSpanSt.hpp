// CodeGear C++Builder
// Copyright (c) 1995, 2012 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'AbSpanSt.pas' rev: 24.00 (Windows)

#ifndef AbspanstHPP
#define AbspanstHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.Classes.hpp>	// Pascal unit
#include <AbArcTyp.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Abspanst
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TAbSpanBaseStream;
class PASCALIMPLEMENTATION TAbSpanBaseStream : public System::Classes::TStream
{
	typedef System::Classes::TStream inherited;
	
protected:
	System::UnicodeString FArchiveName;
	Abarctyp::TAbRequestImageEvent FOnRequestImage;
	System::UnicodeString __fastcall GetImageName(int ImageNumber);
	
public:
	__fastcall TAbSpanBaseStream(const System::UnicodeString ArchiveName);
	__property Abarctyp::TAbRequestImageEvent OnRequestImage = {read=FOnRequestImage, write=FOnRequestImage};
public:
	/* TObject.Destroy */ inline __fastcall virtual ~TAbSpanBaseStream(void) { }
	
};


class DELPHICLASS TAbSpanReadStream;
class PASCALIMPLEMENTATION TAbSpanReadStream : public TAbSpanBaseStream
{
	typedef TAbSpanBaseStream inherited;
	
protected:
	unsigned FCurrentImage;
	bool FIsSplit;
	unsigned FLastImage;
	System::Classes::TStream* FStream;
	Abarctyp::TAbRequestNthDiskEvent FOnRequestNthDisk;
	void __fastcall GotoImage(int ImageNumber);
	void __fastcall SetOnRequestImage(Abarctyp::TAbRequestImageEvent Value);
	
public:
	__fastcall TAbSpanReadStream(const System::UnicodeString ArchiveName, unsigned CurrentImage, System::Classes::TStream* Stream);
	__fastcall virtual ~TAbSpanReadStream(void);
	virtual int __fastcall Read(void *Buffer, int Count)/* overload */;
	virtual int __fastcall Write(const void *Buffer, int Count)/* overload */;
	virtual __int64 __fastcall Seek(const __int64 Offset, System::Classes::TSeekOrigin Origin)/* overload */;
	void __fastcall SeekImage(unsigned Image, const __int64 Offset);
	__property OnRequestImage = {write=SetOnRequestImage};
	__property Abarctyp::TAbRequestNthDiskEvent OnRequestNthDisk = {read=FOnRequestNthDisk, write=FOnRequestNthDisk};
/* Hoisted overloads: */
	
public:
	inline int __fastcall  Read(System::DynamicArray<System::Byte> Buffer, int Offset, int Count){ return System::Classes::TStream::Read(Buffer, Offset, Count); }
	inline int __fastcall  Read(System::DynamicArray<System::Byte> &Buffer, int Count){ return System::Classes::TStream::Read(Buffer, Count); }
	inline int __fastcall  Write(const System::DynamicArray<System::Byte> Buffer, int Offset, int Count){ return System::Classes::TStream::Write(Buffer, Offset, Count); }
	inline int __fastcall  Write(const System::DynamicArray<System::Byte> Buffer, int Count){ return System::Classes::TStream::Write(Buffer, Count); }
	inline int __fastcall  Seek(int Offset, System::Word Origin){ return System::Classes::TStream::Seek(Offset, Origin); }
	
};


class DELPHICLASS TAbSpanWriteStream;
class PASCALIMPLEMENTATION TAbSpanWriteStream : public TAbSpanBaseStream
{
	typedef TAbSpanBaseStream inherited;
	
protected:
	unsigned FCurrentImage;
	__int64 FImageSize;
	System::Classes::TStream* FStream;
	__int64 FThreshold;
	Abarctyp::TAbRequestDiskEvent FOnRequestBlankDisk;
	void __fastcall NewImage(void);
	
public:
	__fastcall TAbSpanWriteStream(const System::UnicodeString ArchiveName, System::Classes::TStream* Stream, __int64 Threshold);
	__fastcall virtual ~TAbSpanWriteStream(void);
	virtual int __fastcall Read(void *Buffer, int Count)/* overload */;
	virtual int __fastcall Write(const void *Buffer, int Count)/* overload */;
	bool __fastcall WriteUnspanned(const void *Buffer, int Count, bool FailOnSpan = false);
	virtual __int64 __fastcall Seek(const __int64 Offset, System::Classes::TSeekOrigin Origin)/* overload */;
	System::Classes::TStream* __fastcall ReleaseStream(void);
	__property unsigned CurrentImage = {read=FCurrentImage, nodefault};
	__property Abarctyp::TAbRequestDiskEvent OnRequestBlankDisk = {read=FOnRequestBlankDisk, write=FOnRequestBlankDisk};
/* Hoisted overloads: */
	
public:
	inline int __fastcall  Read(System::DynamicArray<System::Byte> Buffer, int Offset, int Count){ return System::Classes::TStream::Read(Buffer, Offset, Count); }
	inline int __fastcall  Read(System::DynamicArray<System::Byte> &Buffer, int Count){ return System::Classes::TStream::Read(Buffer, Count); }
	inline int __fastcall  Write(const System::DynamicArray<System::Byte> Buffer, int Offset, int Count){ return System::Classes::TStream::Write(Buffer, Offset, Count); }
	inline int __fastcall  Write(const System::DynamicArray<System::Byte> Buffer, int Count){ return System::Classes::TStream::Write(Buffer, Count); }
	inline int __fastcall  Seek(int Offset, System::Word Origin){ return System::Classes::TStream::Seek(Offset, Origin); }
	
};


//-- var, const, procedure ---------------------------------------------------
}	/* namespace Abspanst */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_ABSPANST)
using namespace Abspanst;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// AbspanstHPP
