// CodeGear C++Builder
// Copyright (c) 1995, 2012 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'AbDfInW.pas' rev: 24.00 (Windows)

#ifndef AbdfinwHPP
#define AbdfinwHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.Classes.hpp>	// Pascal unit
#include <AbDfBase.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Abdfinw
{
//-- type declarations -------------------------------------------------------
struct DECLSPEC_DRECORD TAbDfMatch
{
public:
	int maLen;
	int maDist;
	char maLit;
};


typedef System::StaticArray<void *, 536870911> TAbPointerList;

typedef TAbPointerList *PAbPointerList;

class DELPHICLASS TAbDfInputWindow;
class PASCALIMPLEMENTATION TAbDfInputWindow : public System::TObject
{
	typedef System::TObject inherited;
	
private:
	bool FAdvanceStart;
	char *FBuffer;
	char *FBufferEnd;
	int FBytesUsed;
	int FChainLen;
	TAbPointerList *FHashChains;
	TAbPointerList *FHashHeads;
	int FHashIndex;
	int FChecksum;
	char *FCurrent;
	char *FLookAheadEnd;
	int FMaxMatchLen;
	bool FMustSlide;
	Abdfbase::TAbProgressStep FOnProgress;
	char *FSlidePoint;
	char *FStart;
	int FStartOffset;
	System::Classes::TStream* FStream;
	__int64 FStreamSize;
	bool FUseCRC32;
	bool FUseDeflate64;
	int FWinMask;
	int FWinSize;
	
protected:
	int __fastcall iwGetChecksum(void);
	void __fastcall iwReadFromStream(void);
	void __fastcall iwSetCapacity(int aValue);
	void __fastcall iwSlide(void);
	
public:
	__fastcall TAbDfInputWindow(System::Classes::TStream* aStream, __int64 aStreamSize, int aWinSize, int aChainLength, bool aUseDeflate64, bool aUseCRC32);
	__fastcall virtual ~TAbDfInputWindow(void);
	void __fastcall Advance(int aCount, int aHashCount);
	void __fastcall AdvanceByOne(void);
	bool __fastcall FindLongestMatch(int aAmpleLength, TAbDfMatch &aMatch, const TAbDfMatch &aPrevMatch);
	char __fastcall GetNextChar(void);
	int __fastcall GetNextKeyLength(void);
	int __fastcall Position(void);
	void __fastcall ReadBuffer(void *aBuffer, int aCount, __int64 aOffset);
	__property int ChainLen = {read=FChainLen, write=FChainLen, nodefault};
	__property int Checksum = {read=iwGetChecksum, nodefault};
	__property Abdfbase::TAbProgressStep OnProgress = {read=FOnProgress, write=FOnProgress};
};


//-- var, const, procedure ---------------------------------------------------
}	/* namespace Abdfinw */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_ABDFINW)
using namespace Abdfinw;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// AbdfinwHPP
