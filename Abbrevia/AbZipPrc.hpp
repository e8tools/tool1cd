// CodeGear C++Builder
// Copyright (c) 1995, 2012 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'AbZipPrc.pas' rev: 24.00 (Windows)

#ifndef AbzipprcHPP
#define AbzipprcHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.Classes.hpp>	// Pascal unit
#include <AbZipTyp.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Abzipprc
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
extern DELPHI_PACKAGE void __fastcall AbZipFromStream(Abziptyp::TAbZipArchive* Sender, Abziptyp::TAbZipItem* Item, System::Classes::TStream* OutStream, System::Classes::TStream* InStream);
extern DELPHI_PACKAGE void __fastcall AbZip(Abziptyp::TAbZipArchive* Sender, Abziptyp::TAbZipItem* Item, System::Classes::TStream* OutStream);
extern DELPHI_PACKAGE void __fastcall DeflateStream(System::Classes::TStream* UncompressedStream, System::Classes::TStream* CompressedStream);
}	/* namespace Abzipprc */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_ABZIPPRC)
using namespace Abzipprc;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// AbzipprcHPP
