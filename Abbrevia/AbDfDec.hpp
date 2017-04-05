// CodeGear C++Builder
// Copyright (c) 1995, 2012 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'AbDfDec.pas' rev: 24.00 (Windows)

#ifndef AbdfdecHPP
#define AbdfdecHPP

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

namespace Abdfdec
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
extern DELPHI_PACKAGE int __fastcall Inflate(System::Classes::TStream* aSource, System::Classes::TStream* aDest, Abdfbase::TAbDeflateHelper* aHelper);
}	/* namespace Abdfdec */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_ABDFDEC)
using namespace Abdfdec;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// AbdfdecHPP
