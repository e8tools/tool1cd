// CodeGear C++Builder
// Copyright (c) 1995, 2012 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'AbDfCryS.pas' rev: 24.00 (Windows)

#ifndef AbdfcrysHPP
#define AbdfcrysHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.Classes.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Abdfcrys
{
//-- type declarations -------------------------------------------------------
typedef System::StaticArray<System::Byte, 12> TAbZipEncryptHeader;

class DELPHICLASS TAbZipDecryptEngine;
#pragma pack(push,4)
class PASCALIMPLEMENTATION TAbZipDecryptEngine : public System::TObject
{
	typedef System::TObject inherited;
	
private:
	bool FReady;
	System::StaticArray<int, 3> FState;
	
protected:
	void __fastcall zdeInitState(const System::AnsiString aPassphrase);
	
public:
	__fastcall TAbZipDecryptEngine(void);
	System::Byte __fastcall Decode(System::Byte aCh);
	void __fastcall DecodeBuffer(void *aBuffer, int aCount);
	bool __fastcall VerifyHeader(System::Byte const *aHeader, const System::AnsiString aPassphrase, int aCheckValue);
public:
	/* TObject.Destroy */ inline __fastcall virtual ~TAbZipDecryptEngine(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS TAbDfDecryptStream;
#pragma pack(push,4)
class PASCALIMPLEMENTATION TAbDfDecryptStream : public System::Classes::TStream
{
	typedef System::Classes::TStream inherited;
	
private:
	int FCheckValue;
	TAbZipDecryptEngine* FEngine;
	bool FOwnsStream;
	System::AnsiString FPassphrase;
	bool FReady;
	System::Classes::TStream* FStream;
	
public:
	__fastcall TAbDfDecryptStream(System::Classes::TStream* aStream, int aCheckValue, const System::AnsiString aPassphrase);
	__fastcall virtual ~TAbDfDecryptStream(void);
	bool __fastcall IsValid(void);
	virtual int __fastcall Read(void *aBuffer, int aCount)/* overload */;
	virtual int __fastcall Seek(int aOffset, System::Word aOrigin)/* overload */;
	virtual int __fastcall Write(const void *aBuffer, int aCount)/* overload */;
	__property bool OwnsStream = {read=FOwnsStream, write=FOwnsStream, nodefault};
/* Hoisted overloads: */
	
public:
	inline int __fastcall  Read(System::DynamicArray<System::Byte> Buffer, int Offset, int Count){ return System::Classes::TStream::Read(Buffer, Offset, Count); }
	inline int __fastcall  Read(System::DynamicArray<System::Byte> &Buffer, int Count){ return System::Classes::TStream::Read(Buffer, Count); }
	inline __int64 __fastcall  Seek(const __int64 Offset, System::Classes::TSeekOrigin Origin){ return System::Classes::TStream::Seek(Offset, Origin); }
	inline int __fastcall  Write(const System::DynamicArray<System::Byte> Buffer, int Offset, int Count){ return System::Classes::TStream::Write(Buffer, Offset, Count); }
	inline int __fastcall  Write(const System::DynamicArray<System::Byte> Buffer, int Count){ return System::Classes::TStream::Write(Buffer, Count); }
	
};

#pragma pack(pop)

class DELPHICLASS TAbZipEncryptEngine;
#pragma pack(push,4)
class PASCALIMPLEMENTATION TAbZipEncryptEngine : public System::TObject
{
	typedef System::TObject inherited;
	
private:
	bool FReady;
	System::StaticArray<int, 3> FState;
	
protected:
	void __fastcall zeeInitState(const System::AnsiString aPassphrase);
	
public:
	__fastcall TAbZipEncryptEngine(void);
	System::Byte __fastcall Encode(System::Byte aCh);
	void __fastcall EncodeBuffer(void *aBuffer, int aCount);
	void __fastcall CreateHeader(System::Byte *aHeader, const System::AnsiString aPassphrase, int aCheckValue);
public:
	/* TObject.Destroy */ inline __fastcall virtual ~TAbZipEncryptEngine(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS TAbDfEncryptStream;
#pragma pack(push,4)
class PASCALIMPLEMENTATION TAbDfEncryptStream : public System::Classes::TStream
{
	typedef System::Classes::TStream inherited;
	
private:
	char *FBuffer;
	int FBufSize;
	TAbZipEncryptEngine* FEngine;
	System::Classes::TStream* FStream;
	
public:
	__fastcall TAbDfEncryptStream(System::Classes::TStream* aStream, int aCheckValue, const System::AnsiString aPassphrase);
	__fastcall virtual ~TAbDfEncryptStream(void);
	virtual int __fastcall Read(void *aBuffer, int aCount)/* overload */;
	virtual int __fastcall Seek(int aOffset, System::Word aOrigin)/* overload */;
	virtual int __fastcall Write(const void *aBuffer, int aCount)/* overload */;
/* Hoisted overloads: */
	
public:
	inline int __fastcall  Read(System::DynamicArray<System::Byte> Buffer, int Offset, int Count){ return System::Classes::TStream::Read(Buffer, Offset, Count); }
	inline int __fastcall  Read(System::DynamicArray<System::Byte> &Buffer, int Count){ return System::Classes::TStream::Read(Buffer, Count); }
	inline __int64 __fastcall  Seek(const __int64 Offset, System::Classes::TSeekOrigin Origin){ return System::Classes::TStream::Seek(Offset, Origin); }
	inline int __fastcall  Write(const System::DynamicArray<System::Byte> Buffer, int Offset, int Count){ return System::Classes::TStream::Write(Buffer, Offset, Count); }
	inline int __fastcall  Write(const System::DynamicArray<System::Byte> Buffer, int Count){ return System::Classes::TStream::Write(Buffer, Count); }
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
}	/* namespace Abdfcrys */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_ABDFCRYS)
using namespace Abdfcrys;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// AbdfcrysHPP
