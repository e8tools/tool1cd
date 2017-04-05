// CodeGear C++Builder
// Copyright (c) 1995, 2012 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'AbDfEnc.pas' rev: 24.00 (Windows)

#ifndef AbdfencHPP
#define AbdfencHPP

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

namespace Abdfenc
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
extern DELPHI_PACKAGE int __fastcall Deflate(System::Classes::TStream* aSource, System::Classes::TStream* aDest, Abdfbase::TAbDeflateHelper* aHelper);
}	/* namespace Abdfenc */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_ABDFENC)
using namespace Abdfenc;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// AbdfencHPP
