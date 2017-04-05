// CodeGear C++Builder
// Copyright (c) 1995, 2012 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'AbDfOutW.pas' rev: 24.00 (Windows)

#ifndef AbdfoutwHPP
#define AbdfoutwHPP

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

namespace Abdfoutw
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TAbDfOutputWindow;
#pragma pack(push,4)
class PASCALIMPLEMENTATION TAbDfOutputWindow : public System::TObject
{
	typedef System::TObject inherited;
	
private:
	char *FBuffer;
	int FChecksum;
	char *FCurrent;
	Abdfbase::TAbLogger* FLog;
	int FPartSize;
	int FSlideCount;
	System::Classes::TStream* FStream;
	int FStreamPos;
	bool FTestOnly;
	bool FUseCRC32;
	char *FWritePoint;
	
protected:
	int __fastcall swGetChecksum(void);
	void __fastcall swWriteToStream(bool aFlush);
	
public:
	__fastcall TAbDfOutputWindow(System::Classes::TStream* aStream, bool aUseDeflate64, bool aUseCRC32, int aPartSize, bool aTestOnly, Abdfbase::TAbLogger* aLog);
	__fastcall virtual ~TAbDfOutputWindow(void);
	void __fastcall AddBuffer(void *aBuffer, int aCount);
	void __fastcall AddLiteral(char aCh);
	void __fastcall AddLenDist(int aLen, int aDist);
	int __fastcall Position(void);
	__property int Checksum = {read=swGetChecksum, nodefault};
	__property Abdfbase::TAbLogger* Log = {read=FLog};
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
}	/* namespace Abdfoutw */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_ABDFOUTW)
using namespace Abdfoutw;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// AbdfoutwHPP
