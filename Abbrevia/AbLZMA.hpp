// CodeGear C++Builder
// Copyright (c) 1995, 2012 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'AbLZMA.pas' rev: 24.00 (Windows)

#ifndef AblzmaHPP
#define AblzmaHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.Classes.hpp>	// Pascal unit
#include <Winapi.Windows.hpp>	// Pascal unit
#include <System.SysUtils.hpp>	// Pascal unit
#include <AbCrtl.hpp>	// Pascal unit
#include <AbUtils.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Ablzma
{
//-- type declarations -------------------------------------------------------
typedef int SRes;

#pragma pack(push,1)
struct DECLSPEC_DRECORD ISeqInStream
{
public:
	int __cdecl (*Read)(void * p, void *buf, int &size);
};
#pragma pack(pop)


typedef ISeqInStream *PISeqInStream;

#pragma pack(push,1)
struct DECLSPEC_DRECORD ISeqOutStream
{
public:
	int __cdecl (*Write)(void * p, const void *buf, int size);
};
#pragma pack(pop)


typedef ISeqOutStream *PISeqOutStream;

#pragma pack(push,1)
struct DECLSPEC_DRECORD ICompressProgress
{
public:
	int __cdecl (*Progress)(void * p, __int64 inSize, __int64 outSize);
};
#pragma pack(pop)


typedef ICompressProgress *PICompressProgress;

#pragma pack(push,1)
struct DECLSPEC_DRECORD ISzAlloc
{
public:
	void * __cdecl (*Alloc)(void * p, int size);
	void __cdecl (*Free)(void * p, void * address);
};
#pragma pack(pop)


typedef ISzAlloc *PISzAlloc;

typedef System::Word CLzmaProb;

#pragma pack(push,1)
struct DECLSPEC_DRECORD CLzmaProps
{
public:
	unsigned lc;
	unsigned lp;
	unsigned pb;
	unsigned dicSize;
};
#pragma pack(pop)


#pragma pack(push,1)
struct DECLSPEC_DRECORD CLzmaDec
{
public:
	CLzmaProps prop;
	System::Word *probs;
	System::Byte *dic;
	System::Byte *buf;
	unsigned range;
	unsigned code;
	int dicPos;
	int dicBufSize;
	unsigned processedPos;
	unsigned checkDicSize;
	unsigned state;
	System::StaticArray<unsigned, 4> reps;
	unsigned remainLen;
	int needFlush;
	int needInitState;
	unsigned numProbs;
	unsigned tempBufSize;
	System::StaticArray<System::Byte, 20> tempBuf;
};
#pragma pack(pop)


typedef int ELzmaFinishMode;

typedef int ELzmaStatus;

typedef void * CLzmaEncHandle;

#pragma pack(push,1)
struct DECLSPEC_DRECORD CLzmaEncProps
{
public:
	int level;
	unsigned dictSize;
	int lc;
	int lp;
	int pb;
	int algo;
	int fb;
	int btMode;
	int numHashBytes;
	unsigned mc;
	unsigned writeEndMark;
	int numThreads;
};
#pragma pack(pop)


typedef System::StaticArray<System::Byte, 5> TLZMAPropertyData;

#pragma pack(push,1)
struct DECLSPEC_DRECORD TLZMAHeader
{
public:
	TLZMAPropertyData PropertyData;
	__int64 UncompressedSize;
};
#pragma pack(pop)


typedef TLZMAHeader *PLZMAHeader;

class DELPHICLASS EAbLZMAException;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbLZMAException : public System::Sysutils::Exception
{
	typedef System::Sysutils::Exception inherited;
	
public:
	/* Exception.Create */ inline __fastcall EAbLZMAException(const System::UnicodeString Msg) : System::Sysutils::Exception(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall EAbLZMAException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : System::Sysutils::Exception(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbLZMAException(NativeUInt Ident)/* overload */ : System::Sysutils::Exception(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbLZMAException(System::PResStringRec ResStringRec)/* overload */ : System::Sysutils::Exception(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbLZMAException(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : System::Sysutils::Exception(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbLZMAException(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : System::Sysutils::Exception(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbLZMAException(const System::UnicodeString Msg, int AHelpContext) : System::Sysutils::Exception(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbLZMAException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : System::Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbLZMAException(NativeUInt Ident, int AHelpContext)/* overload */ : System::Sysutils::Exception(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbLZMAException(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : System::Sysutils::Exception(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbLZMAException(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : System::Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbLZMAException(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : System::Sysutils::Exception(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbLZMAException(void) { }
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
static const System::Int8 SZ_OK = System::Int8(0x0);
static const System::Int8 SZ_ERROR_DATA = System::Int8(0x1);
static const System::Int8 SZ_ERROR_MEM = System::Int8(0x2);
static const System::Int8 SZ_ERROR_CRC = System::Int8(0x3);
static const System::Int8 SZ_ERROR_UNSUPPORTED = System::Int8(0x4);
static const System::Int8 SZ_ERROR_PARAM = System::Int8(0x5);
static const System::Int8 SZ_ERROR_INPUT_EOF = System::Int8(0x6);
static const System::Int8 SZ_ERROR_OUTPUT_EOF = System::Int8(0x7);
static const System::Int8 SZ_ERROR_READ = System::Int8(0x8);
static const System::Int8 SZ_ERROR_WRITE = System::Int8(0x9);
static const System::Int8 SZ_ERROR_PROGRESS = System::Int8(0xa);
static const System::Int8 SZ_ERROR_FAIL = System::Int8(0xb);
static const System::Int8 SZ_ERROR_THREAD = System::Int8(0xc);
static const System::Int8 SZ_ERROR_ARCHIVE = System::Int8(0x10);
static const System::Int8 SZ_ERROR_NO_ARCHIVE = System::Int8(0x11);
static const System::Int8 LZMA_PROPS_SIZE = System::Int8(0x5);
static const System::Int8 LZMA_REQUIRED_INPUT_MAX = System::Int8(0x14);
static const System::Int8 LZMA_FINISH_ANY = System::Int8(0x0);
static const System::Int8 LZMA_FINISH_END = System::Int8(0x1);
static const System::Int8 LZMA_STATUS_NOT_SPECIFIED = System::Int8(0x0);
static const System::Int8 LZMA_STATUS_FINISHED_WITH_MARK = System::Int8(0x1);
static const System::Int8 LZMA_STATUS_NOT_FINISHED = System::Int8(0x3);
static const System::Int8 LZMA_STATUS_NEEDS_MORE_INPUT = System::Int8(0x4);
static const System::Int8 LZMA_STATUS_MAYBE_FINISHED_WITHOUT_MARK = System::Int8(0x5);
extern DELPHI_PACKAGE void __fastcall LzmaCheck(int AResultCode);
extern DELPHI_PACKAGE void __fastcall RaiseLzmaException(int AResultCode);
extern DELPHI_PACKAGE void __cdecl LzmaDec_Construct(CLzmaDec &p);
extern DELPHI_PACKAGE void __fastcall LzmaDecodeStream(System::PByte AProperties, int APropSize, System::Classes::TStream* ASrc, System::Classes::TStream* ADes, __int64 AUncompressedSize = -1LL)/* overload */;
extern DELPHI_PACKAGE void __fastcall LzmaDecodeStream(System::Classes::TStream* ASourceStream, System::Classes::TStream* ATargetStream)/* overload */;
extern DELPHI_PACKAGE void __fastcall LzmaEncodeStream(System::Classes::TStream* ASourceStream, System::Classes::TStream* ATargetStream, __int64 ASourceSize);
extern DELPHI_PACKAGE int __fastcall LzmaGetUncompressedSize(void * APCompressedData, int ACompressedSize);
extern DELPHI_PACKAGE void __fastcall LzmaDecodeBuffer(void * APCompressedData, int ACompressedSize, void * APUncompressedData);
extern DELPHI_PACKAGE int __fastcall LzmaEncodeBuffer(void * APUncompressedData, int AUncompressedSize, void * APCompressedData, int ACompressedDataBufferCapacity, int ACompressionLevel = 0xffffffff, int ADictionarySize = 0xffffffff);
extern DELPHI_PACKAGE void __fastcall MatchFinder_GetPointerToCurrentPos(void);
extern DELPHI_PACKAGE void __fastcall MatchFinder_ReduceOffsets(void);
extern DELPHI_PACKAGE void __fastcall MatchFinder_MoveBlock(void);
extern DELPHI_PACKAGE void __fastcall MatchFinder_NeedMove(void);
extern DELPHI_PACKAGE void __fastcall MatchFinder_ReadIfRequired(void);
extern DELPHI_PACKAGE void __fastcall MatchFinder_Construct(void);
extern DELPHI_PACKAGE void __fastcall MatchFinder_Free(void);
extern DELPHI_PACKAGE void __fastcall MatchFinder_Create(void);
extern DELPHI_PACKAGE void __fastcall MatchFinder_Init(void);
extern DELPHI_PACKAGE void __fastcall MatchFinder_Normalize3(void);
extern DELPHI_PACKAGE void __fastcall GetMatchesSpec1(void);
extern DELPHI_PACKAGE void __fastcall MatchFinder_CreateVTable(void);
extern DELPHI_PACKAGE void __fastcall MatchFinderMt_Construct(void);
extern DELPHI_PACKAGE void __fastcall MatchFinderMt_Destruct(void);
extern DELPHI_PACKAGE void __fastcall MatchFinderMt_Create(void);
extern DELPHI_PACKAGE void __fastcall MatchFinderMt_ReleaseStream(void);
extern DELPHI_PACKAGE void __fastcall MatchFinderMt_CreateVTable(void);
extern DELPHI_PACKAGE void __cdecl LzmaDec_Init(CLzmaDec &p);
extern DELPHI_PACKAGE int __cdecl LzmaDec_DecodeToBuf(CLzmaDec &p, System::PByte dest, int &destLen, System::PByte src, int &srcLen, int finishMode, int &status);
extern DELPHI_PACKAGE void __cdecl LzmaDec_Free(CLzmaDec &state, PISzAlloc alloc);
extern DELPHI_PACKAGE int __cdecl LzmaDec_Allocate(CLzmaDec &state, System::PByte prop, int propsSize, PISzAlloc alloc);
extern DELPHI_PACKAGE int __cdecl LzmaDecode(System::PByte dest, int &destLen, System::PByte src, int &srcLen, System::PByte propData, int propSize, int finishMode, int &status, PISzAlloc alloc);
extern DELPHI_PACKAGE void __cdecl LzmaEncProps_Init(CLzmaEncProps &p);
extern DELPHI_PACKAGE int __cdecl LzmaEnc_SetProps(void * p, CLzmaEncProps &props);
extern DELPHI_PACKAGE void * __cdecl LzmaEnc_Create(PISzAlloc Alloc);
extern DELPHI_PACKAGE void __cdecl LzmaEnc_Destroy(void * p, PISzAlloc Alloc, PISzAlloc allocBig);
extern DELPHI_PACKAGE int __cdecl LzmaEnc_Encode(void * p, PISeqOutStream outStream, PISeqInStream inStream, PICompressProgress Progress, PISzAlloc Alloc, PISzAlloc allocBig);
extern DELPHI_PACKAGE int __cdecl LzmaEnc_WriteProperties(void * p, System::PByte properties, int &size);
extern DELPHI_PACKAGE int __cdecl LzmaEnc_MemEncode(void * p, System::PByte dest, int &destLen, System::PByte src, int srcLen, int writeEndMark, PICompressProgress Progress, PISzAlloc Alloc, PISzAlloc allocBig);
extern DELPHI_PACKAGE int __cdecl LzmaEncode(System::PByte dest, int &destLen, System::PByte src, int srcLen, CLzmaEncProps &props, System::PByte propsEncoded, int &propsSize, int writeEndMark, PICompressProgress progress, PISzAlloc alloc, PISzAlloc allocBig);
}	/* namespace Ablzma */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_ABLZMA)
using namespace Ablzma;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// AblzmaHPP
