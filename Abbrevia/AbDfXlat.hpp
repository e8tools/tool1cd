// CodeGear C++Builder
// Copyright (c) 1995, 2012 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'AbDfXlat.pas' rev: 24.00 (Windows)

#ifndef AbdfxlatHPP
#define AbdfxlatHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.SysUtils.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Abdfxlat
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TAbDfTranslator;
#pragma pack(push,4)
class PASCALIMPLEMENTATION TAbDfTranslator : public System::TObject
{
	typedef System::TObject inherited;
	
private:
	char *FBuffer;
	System::Sysutils::TByteArray *FLenSymbols;
	System::Sysutils::TByteArray *FLongDistSymbols;
	System::Sysutils::TByteArray *FMediumDistSymbols;
	System::Sysutils::TByteArray *FShortDistSymbols;
	
protected:
	void __fastcall trBuild(void);
	
public:
	__fastcall TAbDfTranslator(void);
	__fastcall virtual ~TAbDfTranslator(void);
	int __fastcall TranslateLength(int aLen);
	int __fastcall TranslateDistance(int aDist);
	__property System::Sysutils::PByteArray LenSymbols = {read=FLenSymbols};
	__property System::Sysutils::PByteArray LongDistSymbols = {read=FLongDistSymbols};
	__property System::Sysutils::PByteArray MediumDistSymbols = {read=FMediumDistSymbols};
	__property System::Sysutils::PByteArray ShortDistSymbols = {read=FShortDistSymbols};
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
extern DELPHI_PACKAGE TAbDfTranslator* AbSymbolTranslator;
}	/* namespace Abdfxlat */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_ABDFXLAT)
using namespace Abdfxlat;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// AbdfxlatHPP
