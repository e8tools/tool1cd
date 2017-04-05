// CodeGear C++Builder
// Copyright (c) 1995, 2012 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'AbBzip2.pas' rev: 24.00 (Windows)

#ifndef Abbzip2HPP
#define Abbzip2HPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.SysUtils.hpp>	// Pascal unit
#include <System.Classes.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Abbzip2
{
//-- type declarations -------------------------------------------------------
typedef void * __cdecl (*TAlloc)(void * opaque, int Items, int Size);

typedef void __cdecl (*TFree)(void * opaque, void * Block);

struct DECLSPEC_DRECORD TBZStreamRec
{
public:
	System::Byte *next_in;
	int avail_in;
	int total_in_lo32;
	int total_in_hi32;
	System::Byte *next_out;
	int avail_out;
	int total_out_lo32;
	int total_out_hi32;
	void *state;
	TAlloc bzalloc;
	TFree bzfree;
	void *opaque;
};


class DELPHICLASS TCustomBZip2Stream;
class PASCALIMPLEMENTATION TCustomBZip2Stream : public System::Classes::TStream
{
	typedef System::Classes::TStream inherited;
	
private:
	System::Classes::TStream* FStrm;
	__int64 FStrmPos;
	System::Classes::TNotifyEvent FOnProgress;
	TBZStreamRec FBZRec;
	System::StaticArray<System::Byte, 65536> FBuffer;
	
protected:
	DYNAMIC void __fastcall Progress(System::TObject* Sender);
	__property System::Classes::TNotifyEvent OnProgress = {read=FOnProgress, write=FOnProgress};
	__fastcall TCustomBZip2Stream(System::Classes::TStream* Strm);
public:
	/* TObject.Destroy */ inline __fastcall virtual ~TCustomBZip2Stream(void) { }
	
};


enum DECLSPEC_DENUM TBlockSize100k : unsigned char { bs1, bs2, bs3, bs4, bs5, bs6, bs7, bs8, bs9 };

class DELPHICLASS TBZCompressionStream;
class PASCALIMPLEMENTATION TBZCompressionStream : public TCustomBZip2Stream
{
	typedef TCustomBZip2Stream inherited;
	
private:
	float __fastcall GetCompressionRate(void);
	
public:
	__fastcall TBZCompressionStream(TBlockSize100k BlockSize100k, System::Classes::TStream* Dest);
	__fastcall virtual ~TBZCompressionStream(void);
	virtual int __fastcall Read(void *Buffer, int Count)/* overload */;
	virtual int __fastcall Write(const void *Buffer, int Count)/* overload */;
	virtual __int64 __fastcall Seek(const __int64 Offset, System::Classes::TSeekOrigin Origin)/* overload */;
	__property float CompressionRate = {read=GetCompressionRate};
	__property OnProgress;
/* Hoisted overloads: */
	
public:
	inline int __fastcall  Read(System::DynamicArray<System::Byte> Buffer, int Offset, int Count){ return System::Classes::TStream::Read(Buffer, Offset, Count); }
	inline int __fastcall  Read(System::DynamicArray<System::Byte> &Buffer, int Count){ return System::Classes::TStream::Read(Buffer, Count); }
	inline int __fastcall  Write(const System::DynamicArray<System::Byte> Buffer, int Offset, int Count){ return System::Classes::TStream::Write(Buffer, Offset, Count); }
	inline int __fastcall  Write(const System::DynamicArray<System::Byte> Buffer, int Count){ return System::Classes::TStream::Write(Buffer, Count); }
	inline int __fastcall  Seek(int Offset, System::Word Origin){ return System::Classes::TStream::Seek(Offset, Origin); }
	
};


class DELPHICLASS TBZDecompressionStream;
class PASCALIMPLEMENTATION TBZDecompressionStream : public TCustomBZip2Stream
{
	typedef TCustomBZip2Stream inherited;
	
public:
	__fastcall TBZDecompressionStream(System::Classes::TStream* Source);
	__fastcall virtual ~TBZDecompressionStream(void);
	virtual int __fastcall Read(void *Buffer, int Count)/* overload */;
	virtual int __fastcall Write(const void *Buffer, int Count)/* overload */;
	virtual __int64 __fastcall Seek(const __int64 Offset, System::Classes::TSeekOrigin Origin)/* overload */;
	__property OnProgress;
/* Hoisted overloads: */
	
public:
	inline int __fastcall  Read(System::DynamicArray<System::Byte> Buffer, int Offset, int Count){ return System::Classes::TStream::Read(Buffer, Offset, Count); }
	inline int __fastcall  Read(System::DynamicArray<System::Byte> &Buffer, int Count){ return System::Classes::TStream::Read(Buffer, Count); }
	inline int __fastcall  Write(const System::DynamicArray<System::Byte> Buffer, int Offset, int Count){ return System::Classes::TStream::Write(Buffer, Offset, Count); }
	inline int __fastcall  Write(const System::DynamicArray<System::Byte> Buffer, int Count){ return System::Classes::TStream::Write(Buffer, Count); }
	inline int __fastcall  Seek(int Offset, System::Word Origin){ return System::Classes::TStream::Seek(Offset, Origin); }
	
};


class DELPHICLASS EBZip2Error;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EBZip2Error : public System::Sysutils::Exception
{
	typedef System::Sysutils::Exception inherited;
	
public:
	/* Exception.Create */ inline __fastcall EBZip2Error(const System::UnicodeString Msg) : System::Sysutils::Exception(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall EBZip2Error(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : System::Sysutils::Exception(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EBZip2Error(NativeUInt Ident)/* overload */ : System::Sysutils::Exception(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EBZip2Error(System::PResStringRec ResStringRec)/* overload */ : System::Sysutils::Exception(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EBZip2Error(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : System::Sysutils::Exception(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EBZip2Error(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : System::Sysutils::Exception(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EBZip2Error(const System::UnicodeString Msg, int AHelpContext) : System::Sysutils::Exception(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EBZip2Error(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : System::Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EBZip2Error(NativeUInt Ident, int AHelpContext)/* overload */ : System::Sysutils::Exception(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EBZip2Error(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : System::Sysutils::Exception(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EBZip2Error(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : System::Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EBZip2Error(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : System::Sysutils::Exception(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EBZip2Error(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EBZCompressionError;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EBZCompressionError : public EBZip2Error
{
	typedef EBZip2Error inherited;
	
public:
	/* Exception.Create */ inline __fastcall EBZCompressionError(const System::UnicodeString Msg) : EBZip2Error(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall EBZCompressionError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EBZip2Error(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EBZCompressionError(NativeUInt Ident)/* overload */ : EBZip2Error(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EBZCompressionError(System::PResStringRec ResStringRec)/* overload */ : EBZip2Error(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EBZCompressionError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EBZip2Error(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EBZCompressionError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EBZip2Error(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EBZCompressionError(const System::UnicodeString Msg, int AHelpContext) : EBZip2Error(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EBZCompressionError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EBZip2Error(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EBZCompressionError(NativeUInt Ident, int AHelpContext)/* overload */ : EBZip2Error(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EBZCompressionError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EBZip2Error(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EBZCompressionError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EBZip2Error(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EBZCompressionError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EBZip2Error(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EBZCompressionError(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EBZDecompressionError;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EBZDecompressionError : public EBZip2Error
{
	typedef EBZip2Error inherited;
	
public:
	/* Exception.Create */ inline __fastcall EBZDecompressionError(const System::UnicodeString Msg) : EBZip2Error(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall EBZDecompressionError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EBZip2Error(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EBZDecompressionError(NativeUInt Ident)/* overload */ : EBZip2Error(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EBZDecompressionError(System::PResStringRec ResStringRec)/* overload */ : EBZip2Error(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EBZDecompressionError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EBZip2Error(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EBZDecompressionError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EBZip2Error(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EBZDecompressionError(const System::UnicodeString Msg, int AHelpContext) : EBZip2Error(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EBZDecompressionError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EBZip2Error(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EBZDecompressionError(NativeUInt Ident, int AHelpContext)/* overload */ : EBZip2Error(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EBZDecompressionError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EBZip2Error(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EBZDecompressionError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EBZip2Error(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EBZDecompressionError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EBZip2Error(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EBZDecompressionError(void) { }
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
extern DELPHI_PACKAGE void __fastcall BZCompressBuf(const void * InBuf, int InBytes, /* out */ void * &OutBuf, /* out */ int &OutBytes);
extern DELPHI_PACKAGE void __fastcall BZDecompressBuf(const void * InBuf, int InBytes, int OutEstimate, /* out */ void * &OutBuf, /* out */ int &OutBytes);
}	/* namespace Abbzip2 */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_ABBZIP2)
using namespace Abbzip2;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Abbzip2HPP
