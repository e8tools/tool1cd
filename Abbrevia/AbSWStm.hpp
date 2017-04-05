// CodeGear C++Builder
// Copyright (c) 1995, 2012 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'AbSWStm.pas' rev: 24.00 (Windows)

#ifndef AbswstmHPP
#define AbswstmHPP

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

namespace Abswstm
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TabSlidingWindowStream;
#pragma pack(push,4)
class PASCALIMPLEMENTATION TabSlidingWindowStream : public System::Classes::TStream
{
	typedef System::Classes::TStream inherited;
	
protected:
	System::StaticArray<System::Sysutils::PByteArray, 5> bsChunks;
	int bsBufferStart;
	int bsLastPos;
	int bsCurChunk;
	int bsPosInChunk;
	int bsPosInBuffer;
	int bsSize;
	bool bsDirty;
	System::Classes::TStream* bsStream;
	void __fastcall bsWriteChunk(int aIndex);
	void __fastcall bsSlide(void);
	
public:
	__fastcall TabSlidingWindowStream(System::Classes::TStream* aStream);
	__fastcall virtual ~TabSlidingWindowStream(void);
	void __fastcall Flush(void);
	virtual int __fastcall Read(void *Buffer, int Count)/* overload */;
	virtual int __fastcall Seek(int Offset, System::Word Origin)/* overload */;
	virtual int __fastcall Write(const void *Buffer, int Count)/* overload */;
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
static const System::Int8 abSWChunkCount = System::Int8(0x5);
}	/* namespace Abswstm */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_ABSWSTM)
using namespace Abswstm;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// AbswstmHPP
