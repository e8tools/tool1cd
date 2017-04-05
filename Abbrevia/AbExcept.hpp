// CodeGear C++Builder
// Copyright (c) 1995, 2012 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'AbExcept.pas' rev: 24.00 (Windows)

#ifndef AbexceptHPP
#define AbexceptHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.SysUtils.hpp>	// Pascal unit
#include <AbUtils.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Abexcept
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS EAbException;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbException : public System::Sysutils::Exception
{
	typedef System::Sysutils::Exception inherited;
	
public:
	int ErrorCode;
public:
	/* Exception.Create */ inline __fastcall EAbException(const System::UnicodeString Msg) : System::Sysutils::Exception(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall EAbException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : System::Sysutils::Exception(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbException(NativeUInt Ident)/* overload */ : System::Sysutils::Exception(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbException(System::PResStringRec ResStringRec)/* overload */ : System::Sysutils::Exception(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbException(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : System::Sysutils::Exception(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbException(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : System::Sysutils::Exception(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbException(const System::UnicodeString Msg, int AHelpContext) : System::Sysutils::Exception(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : System::Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbException(NativeUInt Ident, int AHelpContext)/* overload */ : System::Sysutils::Exception(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbException(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : System::Sysutils::Exception(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbException(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : System::Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbException(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : System::Sysutils::Exception(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbException(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbArchiveBusy;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbArchiveBusy : public EAbException
{
	typedef EAbException inherited;
	
public:
	__fastcall EAbArchiveBusy(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbArchiveBusy(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbArchiveBusy(NativeUInt Ident)/* overload */ : EAbException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbArchiveBusy(System::PResStringRec ResStringRec)/* overload */ : EAbException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbArchiveBusy(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbArchiveBusy(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbArchiveBusy(const System::UnicodeString Msg, int AHelpContext) : EAbException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbArchiveBusy(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbArchiveBusy(NativeUInt Ident, int AHelpContext)/* overload */ : EAbException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbArchiveBusy(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbArchiveBusy(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbArchiveBusy(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbArchiveBusy(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbBadStream;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbBadStream : public EAbException
{
	typedef EAbException inherited;
	
protected:
	System::Sysutils::Exception* FInnerException;
	
public:
	__fastcall EAbBadStream(void);
	__fastcall EAbBadStream(System::Sysutils::Exception* aInnerException);
	__property System::Sysutils::Exception* InnerException = {read=FInnerException};
public:
	/* Exception.CreateFmt */ inline __fastcall EAbBadStream(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbBadStream(NativeUInt Ident)/* overload */ : EAbException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbBadStream(System::PResStringRec ResStringRec)/* overload */ : EAbException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbBadStream(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbBadStream(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbBadStream(const System::UnicodeString Msg, int AHelpContext) : EAbException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbBadStream(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbBadStream(NativeUInt Ident, int AHelpContext)/* overload */ : EAbException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbBadStream(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbBadStream(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbBadStream(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbBadStream(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbDuplicateName;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbDuplicateName : public EAbException
{
	typedef EAbException inherited;
	
public:
	__fastcall EAbDuplicateName(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbDuplicateName(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbDuplicateName(NativeUInt Ident)/* overload */ : EAbException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbDuplicateName(System::PResStringRec ResStringRec)/* overload */ : EAbException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbDuplicateName(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbDuplicateName(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbDuplicateName(const System::UnicodeString Msg, int AHelpContext) : EAbException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbDuplicateName(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbDuplicateName(NativeUInt Ident, int AHelpContext)/* overload */ : EAbException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbDuplicateName(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbDuplicateName(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbDuplicateName(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbDuplicateName(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbFileNotFound;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbFileNotFound : public EAbException
{
	typedef EAbException inherited;
	
public:
	__fastcall EAbFileNotFound(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbFileNotFound(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbFileNotFound(NativeUInt Ident)/* overload */ : EAbException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbFileNotFound(System::PResStringRec ResStringRec)/* overload */ : EAbException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFileNotFound(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFileNotFound(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbFileNotFound(const System::UnicodeString Msg, int AHelpContext) : EAbException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbFileNotFound(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFileNotFound(NativeUInt Ident, int AHelpContext)/* overload */ : EAbException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFileNotFound(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFileNotFound(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFileNotFound(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbFileNotFound(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbNoArchive;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbNoArchive : public EAbException
{
	typedef EAbException inherited;
	
public:
	__fastcall EAbNoArchive(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbNoArchive(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbNoArchive(NativeUInt Ident)/* overload */ : EAbException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbNoArchive(System::PResStringRec ResStringRec)/* overload */ : EAbException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbNoArchive(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbNoArchive(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbNoArchive(const System::UnicodeString Msg, int AHelpContext) : EAbException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbNoArchive(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbNoArchive(NativeUInt Ident, int AHelpContext)/* overload */ : EAbException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbNoArchive(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbNoArchive(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbNoArchive(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbNoArchive(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbUserAbort;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbUserAbort : public EAbException
{
	typedef EAbException inherited;
	
public:
	__fastcall EAbUserAbort(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbUserAbort(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbUserAbort(NativeUInt Ident)/* overload */ : EAbException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbUserAbort(System::PResStringRec ResStringRec)/* overload */ : EAbException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbUserAbort(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbUserAbort(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbUserAbort(const System::UnicodeString Msg, int AHelpContext) : EAbException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbUserAbort(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbUserAbort(NativeUInt Ident, int AHelpContext)/* overload */ : EAbException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbUserAbort(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbUserAbort(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbUserAbort(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbUserAbort(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbNoSuchDirectory;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbNoSuchDirectory : public EAbException
{
	typedef EAbException inherited;
	
public:
	__fastcall EAbNoSuchDirectory(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbNoSuchDirectory(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbNoSuchDirectory(NativeUInt Ident)/* overload */ : EAbException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbNoSuchDirectory(System::PResStringRec ResStringRec)/* overload */ : EAbException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbNoSuchDirectory(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbNoSuchDirectory(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbNoSuchDirectory(const System::UnicodeString Msg, int AHelpContext) : EAbException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbNoSuchDirectory(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbNoSuchDirectory(NativeUInt Ident, int AHelpContext)/* overload */ : EAbException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbNoSuchDirectory(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbNoSuchDirectory(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbNoSuchDirectory(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbNoSuchDirectory(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbUnhandledType;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbUnhandledType : public EAbException
{
	typedef EAbException inherited;
	
public:
	__fastcall EAbUnhandledType(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbUnhandledType(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbUnhandledType(NativeUInt Ident)/* overload */ : EAbException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbUnhandledType(System::PResStringRec ResStringRec)/* overload */ : EAbException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbUnhandledType(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbUnhandledType(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbUnhandledType(const System::UnicodeString Msg, int AHelpContext) : EAbException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbUnhandledType(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbUnhandledType(NativeUInt Ident, int AHelpContext)/* overload */ : EAbException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbUnhandledType(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbUnhandledType(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbUnhandledType(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbUnhandledType(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbSpanningNotSupported;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbSpanningNotSupported : public EAbException
{
	typedef EAbException inherited;
	
public:
	__fastcall EAbSpanningNotSupported(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbSpanningNotSupported(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbSpanningNotSupported(NativeUInt Ident)/* overload */ : EAbException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbSpanningNotSupported(System::PResStringRec ResStringRec)/* overload */ : EAbException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbSpanningNotSupported(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbSpanningNotSupported(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbSpanningNotSupported(const System::UnicodeString Msg, int AHelpContext) : EAbException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbSpanningNotSupported(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbSpanningNotSupported(NativeUInt Ident, int AHelpContext)/* overload */ : EAbException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbSpanningNotSupported(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbSpanningNotSupported(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbSpanningNotSupported(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbSpanningNotSupported(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbInvalidSpanningThreshold;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbInvalidSpanningThreshold : public EAbException
{
	typedef EAbException inherited;
	
public:
	__fastcall EAbInvalidSpanningThreshold(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbInvalidSpanningThreshold(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbInvalidSpanningThreshold(NativeUInt Ident)/* overload */ : EAbException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbInvalidSpanningThreshold(System::PResStringRec ResStringRec)/* overload */ : EAbException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbInvalidSpanningThreshold(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbInvalidSpanningThreshold(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbInvalidSpanningThreshold(const System::UnicodeString Msg, int AHelpContext) : EAbException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbInvalidSpanningThreshold(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbInvalidSpanningThreshold(NativeUInt Ident, int AHelpContext)/* overload */ : EAbException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbInvalidSpanningThreshold(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbInvalidSpanningThreshold(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbInvalidSpanningThreshold(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbInvalidSpanningThreshold(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbZipException;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbZipException : public EAbException
{
	typedef EAbException inherited;
	
public:
	/* Exception.Create */ inline __fastcall EAbZipException(const System::UnicodeString Msg) : EAbException(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall EAbZipException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipException(NativeUInt Ident)/* overload */ : EAbException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipException(System::PResStringRec ResStringRec)/* overload */ : EAbException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipException(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipException(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbZipException(const System::UnicodeString Msg, int AHelpContext) : EAbException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbZipException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipException(NativeUInt Ident, int AHelpContext)/* overload */ : EAbException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipException(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipException(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipException(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbZipException(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbCabException;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbCabException : public EAbException
{
	typedef EAbException inherited;
	
public:
	/* Exception.Create */ inline __fastcall EAbCabException(const System::UnicodeString Msg) : EAbException(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall EAbCabException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbCabException(NativeUInt Ident)/* overload */ : EAbException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbCabException(System::PResStringRec ResStringRec)/* overload */ : EAbException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbCabException(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbCabException(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbCabException(const System::UnicodeString Msg, int AHelpContext) : EAbException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbCabException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbCabException(NativeUInt Ident, int AHelpContext)/* overload */ : EAbException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbCabException(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbCabException(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbCabException(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbCabException(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbTarException;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbTarException : public EAbException
{
	typedef EAbException inherited;
	
public:
	/* Exception.Create */ inline __fastcall EAbTarException(const System::UnicodeString Msg) : EAbException(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall EAbTarException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbTarException(NativeUInt Ident)/* overload */ : EAbException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbTarException(System::PResStringRec ResStringRec)/* overload */ : EAbException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbTarException(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbTarException(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbTarException(const System::UnicodeString Msg, int AHelpContext) : EAbException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbTarException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbTarException(NativeUInt Ident, int AHelpContext)/* overload */ : EAbException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbTarException(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbTarException(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbTarException(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbTarException(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbGzipException;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbGzipException : public EAbException
{
	typedef EAbException inherited;
	
public:
	/* Exception.Create */ inline __fastcall EAbGzipException(const System::UnicodeString Msg) : EAbException(Msg) { }
	/* Exception.CreateFmt */ inline __fastcall EAbGzipException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbGzipException(NativeUInt Ident)/* overload */ : EAbException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbGzipException(System::PResStringRec ResStringRec)/* overload */ : EAbException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbGzipException(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbGzipException(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbGzipException(const System::UnicodeString Msg, int AHelpContext) : EAbException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbGzipException(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbGzipException(NativeUInt Ident, int AHelpContext)/* overload */ : EAbException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbGzipException(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbGzipException(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbGzipException(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbGzipException(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbZipBadSpanStream;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbZipBadSpanStream : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbZipBadSpanStream(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbZipBadSpanStream(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipBadSpanStream(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipBadSpanStream(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipBadSpanStream(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipBadSpanStream(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbZipBadSpanStream(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbZipBadSpanStream(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipBadSpanStream(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipBadSpanStream(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipBadSpanStream(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipBadSpanStream(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbZipBadSpanStream(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbZipBadCRC;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbZipBadCRC : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbZipBadCRC(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbZipBadCRC(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipBadCRC(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipBadCRC(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipBadCRC(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipBadCRC(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbZipBadCRC(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbZipBadCRC(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipBadCRC(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipBadCRC(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipBadCRC(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipBadCRC(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbZipBadCRC(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbZipInflateBlock;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbZipInflateBlock : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbZipInflateBlock(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbZipInflateBlock(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipInflateBlock(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipInflateBlock(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipInflateBlock(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipInflateBlock(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbZipInflateBlock(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbZipInflateBlock(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipInflateBlock(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipInflateBlock(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipInflateBlock(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipInflateBlock(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbZipInflateBlock(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbZipInvalid;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbZipInvalid : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbZipInvalid(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbZipInvalid(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipInvalid(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipInvalid(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipInvalid(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipInvalid(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbZipInvalid(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbZipInvalid(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipInvalid(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipInvalid(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipInvalid(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipInvalid(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbZipInvalid(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbInvalidIndex;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbInvalidIndex : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbInvalidIndex(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbInvalidIndex(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbInvalidIndex(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbInvalidIndex(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbInvalidIndex(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbInvalidIndex(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbInvalidIndex(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbInvalidIndex(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbInvalidIndex(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbInvalidIndex(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbInvalidIndex(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbInvalidIndex(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbInvalidIndex(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbZipInvalidFactor;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbZipInvalidFactor : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbZipInvalidFactor(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbZipInvalidFactor(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipInvalidFactor(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipInvalidFactor(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipInvalidFactor(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipInvalidFactor(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbZipInvalidFactor(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbZipInvalidFactor(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipInvalidFactor(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipInvalidFactor(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipInvalidFactor(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipInvalidFactor(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbZipInvalidFactor(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbZipInvalidLFH;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbZipInvalidLFH : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbZipInvalidLFH(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbZipInvalidLFH(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipInvalidLFH(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipInvalidLFH(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipInvalidLFH(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipInvalidLFH(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbZipInvalidLFH(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbZipInvalidLFH(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipInvalidLFH(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipInvalidLFH(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipInvalidLFH(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipInvalidLFH(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbZipInvalidLFH(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbZipInvalidMethod;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbZipInvalidMethod : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbZipInvalidMethod(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbZipInvalidMethod(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipInvalidMethod(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipInvalidMethod(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipInvalidMethod(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipInvalidMethod(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbZipInvalidMethod(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbZipInvalidMethod(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipInvalidMethod(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipInvalidMethod(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipInvalidMethod(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipInvalidMethod(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbZipInvalidMethod(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbZipInvalidPassword;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbZipInvalidPassword : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbZipInvalidPassword(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbZipInvalidPassword(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipInvalidPassword(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipInvalidPassword(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipInvalidPassword(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipInvalidPassword(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbZipInvalidPassword(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbZipInvalidPassword(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipInvalidPassword(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipInvalidPassword(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipInvalidPassword(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipInvalidPassword(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbZipInvalidPassword(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbZipInvalidStub;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbZipInvalidStub : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbZipInvalidStub(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbZipInvalidStub(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipInvalidStub(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipInvalidStub(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipInvalidStub(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipInvalidStub(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbZipInvalidStub(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbZipInvalidStub(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipInvalidStub(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipInvalidStub(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipInvalidStub(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipInvalidStub(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbZipInvalidStub(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbZipNoExtraction;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbZipNoExtraction : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbZipNoExtraction(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbZipNoExtraction(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipNoExtraction(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipNoExtraction(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipNoExtraction(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipNoExtraction(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbZipNoExtraction(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbZipNoExtraction(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipNoExtraction(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipNoExtraction(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipNoExtraction(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipNoExtraction(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbZipNoExtraction(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbZipNoInsertion;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbZipNoInsertion : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbZipNoInsertion(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbZipNoInsertion(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipNoInsertion(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipNoInsertion(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipNoInsertion(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipNoInsertion(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbZipNoInsertion(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbZipNoInsertion(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipNoInsertion(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipNoInsertion(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipNoInsertion(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipNoInsertion(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbZipNoInsertion(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbZipSpanOverwrite;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbZipSpanOverwrite : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbZipSpanOverwrite(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbZipSpanOverwrite(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipSpanOverwrite(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipSpanOverwrite(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipSpanOverwrite(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipSpanOverwrite(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbZipSpanOverwrite(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbZipSpanOverwrite(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipSpanOverwrite(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipSpanOverwrite(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipSpanOverwrite(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipSpanOverwrite(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbZipSpanOverwrite(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbZipStreamFull;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbZipStreamFull : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbZipStreamFull(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbZipStreamFull(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipStreamFull(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipStreamFull(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipStreamFull(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipStreamFull(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbZipStreamFull(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbZipStreamFull(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipStreamFull(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipStreamFull(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipStreamFull(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipStreamFull(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbZipStreamFull(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbZipTruncate;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbZipTruncate : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbZipTruncate(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbZipTruncate(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipTruncate(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipTruncate(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipTruncate(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipTruncate(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbZipTruncate(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbZipTruncate(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipTruncate(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipTruncate(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipTruncate(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipTruncate(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbZipTruncate(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbZipUnsupported;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbZipUnsupported : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbZipUnsupported(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbZipUnsupported(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipUnsupported(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipUnsupported(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipUnsupported(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipUnsupported(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbZipUnsupported(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbZipUnsupported(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipUnsupported(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipUnsupported(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipUnsupported(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipUnsupported(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbZipUnsupported(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbZipVersion;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbZipVersion : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbZipVersion(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbZipVersion(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipVersion(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbZipVersion(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipVersion(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbZipVersion(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbZipVersion(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbZipVersion(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipVersion(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbZipVersion(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipVersion(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbZipVersion(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbZipVersion(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbReadError;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbReadError : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbReadError(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbReadError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbReadError(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbReadError(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbReadError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbReadError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbReadError(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbReadError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbReadError(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbReadError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbReadError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbReadError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbReadError(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbGzipBadCRC;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbGzipBadCRC : public EAbGzipException
{
	typedef EAbGzipException inherited;
	
public:
	__fastcall EAbGzipBadCRC(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbGzipBadCRC(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbGzipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbGzipBadCRC(NativeUInt Ident)/* overload */ : EAbGzipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbGzipBadCRC(System::PResStringRec ResStringRec)/* overload */ : EAbGzipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbGzipBadCRC(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbGzipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbGzipBadCRC(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbGzipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbGzipBadCRC(const System::UnicodeString Msg, int AHelpContext) : EAbGzipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbGzipBadCRC(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbGzipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbGzipBadCRC(NativeUInt Ident, int AHelpContext)/* overload */ : EAbGzipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbGzipBadCRC(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbGzipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbGzipBadCRC(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbGzipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbGzipBadCRC(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbGzipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbGzipBadCRC(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbGzipBadFileSize;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbGzipBadFileSize : public EAbGzipException
{
	typedef EAbGzipException inherited;
	
public:
	__fastcall EAbGzipBadFileSize(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbGzipBadFileSize(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbGzipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbGzipBadFileSize(NativeUInt Ident)/* overload */ : EAbGzipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbGzipBadFileSize(System::PResStringRec ResStringRec)/* overload */ : EAbGzipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbGzipBadFileSize(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbGzipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbGzipBadFileSize(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbGzipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbGzipBadFileSize(const System::UnicodeString Msg, int AHelpContext) : EAbGzipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbGzipBadFileSize(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbGzipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbGzipBadFileSize(NativeUInt Ident, int AHelpContext)/* overload */ : EAbGzipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbGzipBadFileSize(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbGzipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbGzipBadFileSize(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbGzipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbGzipBadFileSize(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbGzipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbGzipBadFileSize(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbGzipInvalid;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbGzipInvalid : public EAbGzipException
{
	typedef EAbGzipException inherited;
	
public:
	__fastcall EAbGzipInvalid(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbGzipInvalid(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbGzipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbGzipInvalid(NativeUInt Ident)/* overload */ : EAbGzipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbGzipInvalid(System::PResStringRec ResStringRec)/* overload */ : EAbGzipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbGzipInvalid(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbGzipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbGzipInvalid(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbGzipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbGzipInvalid(const System::UnicodeString Msg, int AHelpContext) : EAbGzipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbGzipInvalid(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbGzipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbGzipInvalid(NativeUInt Ident, int AHelpContext)/* overload */ : EAbGzipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbGzipInvalid(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbGzipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbGzipInvalid(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbGzipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbGzipInvalid(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbGzipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbGzipInvalid(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbTarInvalid;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbTarInvalid : public EAbTarException
{
	typedef EAbTarException inherited;
	
public:
	__fastcall EAbTarInvalid(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbTarInvalid(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbTarException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbTarInvalid(NativeUInt Ident)/* overload */ : EAbTarException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbTarInvalid(System::PResStringRec ResStringRec)/* overload */ : EAbTarException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbTarInvalid(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbTarException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbTarInvalid(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbTarException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbTarInvalid(const System::UnicodeString Msg, int AHelpContext) : EAbTarException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbTarInvalid(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbTarException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbTarInvalid(NativeUInt Ident, int AHelpContext)/* overload */ : EAbTarException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbTarInvalid(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbTarException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbTarInvalid(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbTarException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbTarInvalid(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbTarException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbTarInvalid(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbTarBadFileName;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbTarBadFileName : public EAbTarException
{
	typedef EAbTarException inherited;
	
public:
	__fastcall EAbTarBadFileName(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbTarBadFileName(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbTarException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbTarBadFileName(NativeUInt Ident)/* overload */ : EAbTarException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbTarBadFileName(System::PResStringRec ResStringRec)/* overload */ : EAbTarException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbTarBadFileName(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbTarException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbTarBadFileName(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbTarException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbTarBadFileName(const System::UnicodeString Msg, int AHelpContext) : EAbTarException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbTarBadFileName(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbTarException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbTarBadFileName(NativeUInt Ident, int AHelpContext)/* overload */ : EAbTarException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbTarBadFileName(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbTarException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbTarBadFileName(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbTarException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbTarBadFileName(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbTarException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbTarBadFileName(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbTarBadLinkName;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbTarBadLinkName : public EAbTarException
{
	typedef EAbTarException inherited;
	
public:
	__fastcall EAbTarBadLinkName(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbTarBadLinkName(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbTarException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbTarBadLinkName(NativeUInt Ident)/* overload */ : EAbTarException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbTarBadLinkName(System::PResStringRec ResStringRec)/* overload */ : EAbTarException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbTarBadLinkName(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbTarException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbTarBadLinkName(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbTarException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbTarBadLinkName(const System::UnicodeString Msg, int AHelpContext) : EAbTarException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbTarBadLinkName(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbTarException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbTarBadLinkName(NativeUInt Ident, int AHelpContext)/* overload */ : EAbTarException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbTarBadLinkName(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbTarException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbTarBadLinkName(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbTarException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbTarBadLinkName(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbTarException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbTarBadLinkName(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbTarBadOp;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbTarBadOp : public EAbTarException
{
	typedef EAbTarException inherited;
	
public:
	__fastcall EAbTarBadOp(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbTarBadOp(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbTarException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbTarBadOp(NativeUInt Ident)/* overload */ : EAbTarException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbTarBadOp(System::PResStringRec ResStringRec)/* overload */ : EAbTarException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbTarBadOp(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbTarException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbTarBadOp(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbTarException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbTarBadOp(const System::UnicodeString Msg, int AHelpContext) : EAbTarException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbTarBadOp(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbTarException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbTarBadOp(NativeUInt Ident, int AHelpContext)/* overload */ : EAbTarException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbTarBadOp(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbTarException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbTarBadOp(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbTarException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbTarBadOp(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbTarException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbTarBadOp(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbVMSInvalidOrigin;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbVMSInvalidOrigin : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbVMSInvalidOrigin(int Value);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbVMSInvalidOrigin(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbVMSInvalidOrigin(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbVMSInvalidOrigin(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbVMSInvalidOrigin(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbVMSInvalidOrigin(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbVMSInvalidOrigin(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbVMSInvalidOrigin(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbVMSInvalidOrigin(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbVMSInvalidOrigin(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbVMSInvalidOrigin(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbVMSInvalidOrigin(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbVMSInvalidOrigin(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbVMSErrorOpenSwap;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbVMSErrorOpenSwap : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbVMSErrorOpenSwap(const System::UnicodeString Value);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbVMSErrorOpenSwap(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbVMSErrorOpenSwap(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbVMSErrorOpenSwap(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbVMSErrorOpenSwap(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbVMSErrorOpenSwap(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbVMSErrorOpenSwap(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbVMSErrorOpenSwap(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbVMSErrorOpenSwap(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbVMSErrorOpenSwap(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbVMSErrorOpenSwap(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbVMSErrorOpenSwap(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbVMSErrorOpenSwap(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbVMSSeekFail;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbVMSSeekFail : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbVMSSeekFail(const System::UnicodeString Value);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbVMSSeekFail(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbVMSSeekFail(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbVMSSeekFail(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbVMSSeekFail(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbVMSSeekFail(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbVMSSeekFail(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbVMSSeekFail(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbVMSSeekFail(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbVMSSeekFail(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbVMSSeekFail(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbVMSSeekFail(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbVMSSeekFail(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbVMSReadFail;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbVMSReadFail : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbVMSReadFail(int Count, const System::UnicodeString Value);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbVMSReadFail(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbVMSReadFail(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbVMSReadFail(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbVMSReadFail(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbVMSReadFail(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbVMSReadFail(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbVMSReadFail(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbVMSReadFail(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbVMSReadFail(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbVMSReadFail(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbVMSReadFail(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbVMSReadFail(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbVMSWriteFail;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbVMSWriteFail : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbVMSWriteFail(int Count, const System::UnicodeString Value);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbVMSWriteFail(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbVMSWriteFail(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbVMSWriteFail(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbVMSWriteFail(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbVMSWriteFail(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbVMSWriteFail(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbVMSWriteFail(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbVMSWriteFail(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbVMSWriteFail(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbVMSWriteFail(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbVMSWriteFail(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbVMSWriteFail(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbVMSWriteTooManyBytes;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbVMSWriteTooManyBytes : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbVMSWriteTooManyBytes(int Count);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbVMSWriteTooManyBytes(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbVMSWriteTooManyBytes(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbVMSWriteTooManyBytes(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbVMSWriteTooManyBytes(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbVMSWriteTooManyBytes(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbVMSWriteTooManyBytes(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbVMSWriteTooManyBytes(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbVMSWriteTooManyBytes(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbVMSWriteTooManyBytes(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbVMSWriteTooManyBytes(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbVMSWriteTooManyBytes(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbVMSWriteTooManyBytes(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbBBSReadTooManyBytes;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbBBSReadTooManyBytes : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbBBSReadTooManyBytes(int Count);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbBBSReadTooManyBytes(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbBBSReadTooManyBytes(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbBBSReadTooManyBytes(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbBBSReadTooManyBytes(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbBBSReadTooManyBytes(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbBBSReadTooManyBytes(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbBBSReadTooManyBytes(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbBBSReadTooManyBytes(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbBBSReadTooManyBytes(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbBBSReadTooManyBytes(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbBBSReadTooManyBytes(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbBBSReadTooManyBytes(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbBBSSeekOutsideBuffer;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbBBSSeekOutsideBuffer : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbBBSSeekOutsideBuffer(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbBBSSeekOutsideBuffer(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbBBSSeekOutsideBuffer(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbBBSSeekOutsideBuffer(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbBBSSeekOutsideBuffer(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbBBSSeekOutsideBuffer(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbBBSSeekOutsideBuffer(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbBBSSeekOutsideBuffer(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbBBSSeekOutsideBuffer(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbBBSSeekOutsideBuffer(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbBBSSeekOutsideBuffer(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbBBSSeekOutsideBuffer(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbBBSSeekOutsideBuffer(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbBBSInvalidOrigin;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbBBSInvalidOrigin : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbBBSInvalidOrigin(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbBBSInvalidOrigin(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbBBSInvalidOrigin(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbBBSInvalidOrigin(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbBBSInvalidOrigin(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbBBSInvalidOrigin(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbBBSInvalidOrigin(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbBBSInvalidOrigin(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbBBSInvalidOrigin(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbBBSInvalidOrigin(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbBBSInvalidOrigin(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbBBSInvalidOrigin(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbBBSInvalidOrigin(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbBBSWriteTooManyBytes;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbBBSWriteTooManyBytes : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbBBSWriteTooManyBytes(int Count);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbBBSWriteTooManyBytes(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbBBSWriteTooManyBytes(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbBBSWriteTooManyBytes(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbBBSWriteTooManyBytes(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbBBSWriteTooManyBytes(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbBBSWriteTooManyBytes(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbBBSWriteTooManyBytes(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbBBSWriteTooManyBytes(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbBBSWriteTooManyBytes(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbBBSWriteTooManyBytes(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbBBSWriteTooManyBytes(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbBBSWriteTooManyBytes(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbSWSNotEndofStream;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbSWSNotEndofStream : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbSWSNotEndofStream(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbSWSNotEndofStream(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbSWSNotEndofStream(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbSWSNotEndofStream(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbSWSNotEndofStream(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbSWSNotEndofStream(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbSWSNotEndofStream(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbSWSNotEndofStream(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbSWSNotEndofStream(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbSWSNotEndofStream(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbSWSNotEndofStream(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbSWSNotEndofStream(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbSWSNotEndofStream(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbSWSSeekFailed;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbSWSSeekFailed : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbSWSSeekFailed(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbSWSSeekFailed(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbSWSSeekFailed(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbSWSSeekFailed(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbSWSSeekFailed(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbSWSSeekFailed(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbSWSSeekFailed(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbSWSSeekFailed(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbSWSSeekFailed(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbSWSSeekFailed(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbSWSSeekFailed(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbSWSSeekFailed(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbSWSSeekFailed(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbSWSWriteFailed;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbSWSWriteFailed : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbSWSWriteFailed(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbSWSWriteFailed(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbSWSWriteFailed(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbSWSWriteFailed(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbSWSWriteFailed(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbSWSWriteFailed(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbSWSWriteFailed(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbSWSWriteFailed(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbSWSWriteFailed(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbSWSWriteFailed(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbSWSWriteFailed(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbSWSWriteFailed(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbSWSWriteFailed(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbSWSInvalidOrigin;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbSWSInvalidOrigin : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbSWSInvalidOrigin(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbSWSInvalidOrigin(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbSWSInvalidOrigin(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbSWSInvalidOrigin(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbSWSInvalidOrigin(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbSWSInvalidOrigin(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbSWSInvalidOrigin(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbSWSInvalidOrigin(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbSWSInvalidOrigin(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbSWSInvalidOrigin(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbSWSInvalidOrigin(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbSWSInvalidOrigin(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbSWSInvalidOrigin(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbSWSInvalidNewOrigin;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbSWSInvalidNewOrigin : public EAbZipException
{
	typedef EAbZipException inherited;
	
public:
	__fastcall EAbSWSInvalidNewOrigin(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbSWSInvalidNewOrigin(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbZipException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbSWSInvalidNewOrigin(NativeUInt Ident)/* overload */ : EAbZipException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbSWSInvalidNewOrigin(System::PResStringRec ResStringRec)/* overload */ : EAbZipException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbSWSInvalidNewOrigin(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbSWSInvalidNewOrigin(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbSWSInvalidNewOrigin(const System::UnicodeString Msg, int AHelpContext) : EAbZipException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbSWSInvalidNewOrigin(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbZipException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbSWSInvalidNewOrigin(NativeUInt Ident, int AHelpContext)/* overload */ : EAbZipException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbSWSInvalidNewOrigin(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbSWSInvalidNewOrigin(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbSWSInvalidNewOrigin(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbZipException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbSWSInvalidNewOrigin(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbNoCabinetDll;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbNoCabinetDll : public EAbCabException
{
	typedef EAbCabException inherited;
	
public:
	__fastcall EAbNoCabinetDll(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbNoCabinetDll(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbCabException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbNoCabinetDll(NativeUInt Ident)/* overload */ : EAbCabException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbNoCabinetDll(System::PResStringRec ResStringRec)/* overload */ : EAbCabException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbNoCabinetDll(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbNoCabinetDll(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbNoCabinetDll(const System::UnicodeString Msg, int AHelpContext) : EAbCabException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbNoCabinetDll(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbCabException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbNoCabinetDll(NativeUInt Ident, int AHelpContext)/* overload */ : EAbCabException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbNoCabinetDll(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbNoCabinetDll(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbNoCabinetDll(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbNoCabinetDll(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbFCIFileOpenError;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbFCIFileOpenError : public EAbCabException
{
	typedef EAbCabException inherited;
	
public:
	__fastcall EAbFCIFileOpenError(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbFCIFileOpenError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbCabException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIFileOpenError(NativeUInt Ident)/* overload */ : EAbCabException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIFileOpenError(System::PResStringRec ResStringRec)/* overload */ : EAbCabException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIFileOpenError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIFileOpenError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbFCIFileOpenError(const System::UnicodeString Msg, int AHelpContext) : EAbCabException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbFCIFileOpenError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbCabException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIFileOpenError(NativeUInt Ident, int AHelpContext)/* overload */ : EAbCabException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIFileOpenError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIFileOpenError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIFileOpenError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbFCIFileOpenError(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbFCIFileReadError;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbFCIFileReadError : public EAbCabException
{
	typedef EAbCabException inherited;
	
public:
	__fastcall EAbFCIFileReadError(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbFCIFileReadError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbCabException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIFileReadError(NativeUInt Ident)/* overload */ : EAbCabException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIFileReadError(System::PResStringRec ResStringRec)/* overload */ : EAbCabException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIFileReadError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIFileReadError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbFCIFileReadError(const System::UnicodeString Msg, int AHelpContext) : EAbCabException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbFCIFileReadError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbCabException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIFileReadError(NativeUInt Ident, int AHelpContext)/* overload */ : EAbCabException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIFileReadError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIFileReadError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIFileReadError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbFCIFileReadError(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbFCIFileWriteError;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbFCIFileWriteError : public EAbCabException
{
	typedef EAbCabException inherited;
	
public:
	__fastcall EAbFCIFileWriteError(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbFCIFileWriteError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbCabException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIFileWriteError(NativeUInt Ident)/* overload */ : EAbCabException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIFileWriteError(System::PResStringRec ResStringRec)/* overload */ : EAbCabException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIFileWriteError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIFileWriteError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbFCIFileWriteError(const System::UnicodeString Msg, int AHelpContext) : EAbCabException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbFCIFileWriteError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbCabException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIFileWriteError(NativeUInt Ident, int AHelpContext)/* overload */ : EAbCabException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIFileWriteError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIFileWriteError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIFileWriteError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbFCIFileWriteError(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbFCIFileCloseError;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbFCIFileCloseError : public EAbCabException
{
	typedef EAbCabException inherited;
	
public:
	__fastcall EAbFCIFileCloseError(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbFCIFileCloseError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbCabException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIFileCloseError(NativeUInt Ident)/* overload */ : EAbCabException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIFileCloseError(System::PResStringRec ResStringRec)/* overload */ : EAbCabException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIFileCloseError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIFileCloseError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbFCIFileCloseError(const System::UnicodeString Msg, int AHelpContext) : EAbCabException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbFCIFileCloseError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbCabException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIFileCloseError(NativeUInt Ident, int AHelpContext)/* overload */ : EAbCabException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIFileCloseError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIFileCloseError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIFileCloseError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbFCIFileCloseError(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbFCIFileSeekError;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbFCIFileSeekError : public EAbCabException
{
	typedef EAbCabException inherited;
	
public:
	__fastcall EAbFCIFileSeekError(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbFCIFileSeekError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbCabException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIFileSeekError(NativeUInt Ident)/* overload */ : EAbCabException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIFileSeekError(System::PResStringRec ResStringRec)/* overload */ : EAbCabException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIFileSeekError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIFileSeekError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbFCIFileSeekError(const System::UnicodeString Msg, int AHelpContext) : EAbCabException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbFCIFileSeekError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbCabException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIFileSeekError(NativeUInt Ident, int AHelpContext)/* overload */ : EAbCabException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIFileSeekError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIFileSeekError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIFileSeekError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbFCIFileSeekError(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbFCIFileDeleteError;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbFCIFileDeleteError : public EAbCabException
{
	typedef EAbCabException inherited;
	
public:
	__fastcall EAbFCIFileDeleteError(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbFCIFileDeleteError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbCabException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIFileDeleteError(NativeUInt Ident)/* overload */ : EAbCabException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIFileDeleteError(System::PResStringRec ResStringRec)/* overload */ : EAbCabException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIFileDeleteError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIFileDeleteError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbFCIFileDeleteError(const System::UnicodeString Msg, int AHelpContext) : EAbCabException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbFCIFileDeleteError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbCabException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIFileDeleteError(NativeUInt Ident, int AHelpContext)/* overload */ : EAbCabException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIFileDeleteError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIFileDeleteError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIFileDeleteError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbFCIFileDeleteError(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbFCIAddFileError;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbFCIAddFileError : public EAbCabException
{
	typedef EAbCabException inherited;
	
public:
	__fastcall EAbFCIAddFileError(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbFCIAddFileError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbCabException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIAddFileError(NativeUInt Ident)/* overload */ : EAbCabException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIAddFileError(System::PResStringRec ResStringRec)/* overload */ : EAbCabException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIAddFileError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIAddFileError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbFCIAddFileError(const System::UnicodeString Msg, int AHelpContext) : EAbCabException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbFCIAddFileError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbCabException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIAddFileError(NativeUInt Ident, int AHelpContext)/* overload */ : EAbCabException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIAddFileError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIAddFileError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIAddFileError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbFCIAddFileError(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbFCICreateError;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbFCICreateError : public EAbCabException
{
	typedef EAbCabException inherited;
	
public:
	__fastcall EAbFCICreateError(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbFCICreateError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbCabException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCICreateError(NativeUInt Ident)/* overload */ : EAbCabException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCICreateError(System::PResStringRec ResStringRec)/* overload */ : EAbCabException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCICreateError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCICreateError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbFCICreateError(const System::UnicodeString Msg, int AHelpContext) : EAbCabException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbFCICreateError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbCabException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCICreateError(NativeUInt Ident, int AHelpContext)/* overload */ : EAbCabException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCICreateError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCICreateError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCICreateError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbFCICreateError(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbFCIFlushCabinetError;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbFCIFlushCabinetError : public EAbCabException
{
	typedef EAbCabException inherited;
	
public:
	__fastcall EAbFCIFlushCabinetError(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbFCIFlushCabinetError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbCabException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIFlushCabinetError(NativeUInt Ident)/* overload */ : EAbCabException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIFlushCabinetError(System::PResStringRec ResStringRec)/* overload */ : EAbCabException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIFlushCabinetError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIFlushCabinetError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbFCIFlushCabinetError(const System::UnicodeString Msg, int AHelpContext) : EAbCabException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbFCIFlushCabinetError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbCabException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIFlushCabinetError(NativeUInt Ident, int AHelpContext)/* overload */ : EAbCabException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIFlushCabinetError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIFlushCabinetError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIFlushCabinetError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbFCIFlushCabinetError(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbFCIFlushFolderError;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbFCIFlushFolderError : public EAbCabException
{
	typedef EAbCabException inherited;
	
public:
	__fastcall EAbFCIFlushFolderError(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbFCIFlushFolderError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbCabException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIFlushFolderError(NativeUInt Ident)/* overload */ : EAbCabException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbFCIFlushFolderError(System::PResStringRec ResStringRec)/* overload */ : EAbCabException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIFlushFolderError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFCIFlushFolderError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbFCIFlushFolderError(const System::UnicodeString Msg, int AHelpContext) : EAbCabException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbFCIFlushFolderError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbCabException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIFlushFolderError(NativeUInt Ident, int AHelpContext)/* overload */ : EAbCabException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFCIFlushFolderError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIFlushFolderError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFCIFlushFolderError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbFCIFlushFolderError(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbFDICopyError;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbFDICopyError : public EAbCabException
{
	typedef EAbCabException inherited;
	
public:
	__fastcall EAbFDICopyError(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbFDICopyError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbCabException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbFDICopyError(NativeUInt Ident)/* overload */ : EAbCabException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbFDICopyError(System::PResStringRec ResStringRec)/* overload */ : EAbCabException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFDICopyError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFDICopyError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbFDICopyError(const System::UnicodeString Msg, int AHelpContext) : EAbCabException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbFDICopyError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbCabException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFDICopyError(NativeUInt Ident, int AHelpContext)/* overload */ : EAbCabException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFDICopyError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFDICopyError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFDICopyError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbFDICopyError(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbFDICreateError;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbFDICreateError : public EAbCabException
{
	typedef EAbCabException inherited;
	
public:
	__fastcall EAbFDICreateError(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbFDICreateError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbCabException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbFDICreateError(NativeUInt Ident)/* overload */ : EAbCabException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbFDICreateError(System::PResStringRec ResStringRec)/* overload */ : EAbCabException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFDICreateError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFDICreateError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbFDICreateError(const System::UnicodeString Msg, int AHelpContext) : EAbCabException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbFDICreateError(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbCabException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFDICreateError(NativeUInt Ident, int AHelpContext)/* overload */ : EAbCabException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFDICreateError(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFDICreateError(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFDICreateError(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbFDICreateError(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbInvalidCabTemplate;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbInvalidCabTemplate : public EAbCabException
{
	typedef EAbCabException inherited;
	
public:
	__fastcall EAbInvalidCabTemplate(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbInvalidCabTemplate(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbCabException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbInvalidCabTemplate(NativeUInt Ident)/* overload */ : EAbCabException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbInvalidCabTemplate(System::PResStringRec ResStringRec)/* overload */ : EAbCabException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbInvalidCabTemplate(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbInvalidCabTemplate(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbInvalidCabTemplate(const System::UnicodeString Msg, int AHelpContext) : EAbCabException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbInvalidCabTemplate(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbCabException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbInvalidCabTemplate(NativeUInt Ident, int AHelpContext)/* overload */ : EAbCabException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbInvalidCabTemplate(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbInvalidCabTemplate(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbInvalidCabTemplate(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbInvalidCabTemplate(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbInvalidCabFile;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbInvalidCabFile : public EAbCabException
{
	typedef EAbCabException inherited;
	
public:
	__fastcall EAbInvalidCabFile(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbInvalidCabFile(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbCabException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbInvalidCabFile(NativeUInt Ident)/* overload */ : EAbCabException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbInvalidCabFile(System::PResStringRec ResStringRec)/* overload */ : EAbCabException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbInvalidCabFile(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbInvalidCabFile(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbInvalidCabFile(const System::UnicodeString Msg, int AHelpContext) : EAbCabException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbInvalidCabFile(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbCabException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbInvalidCabFile(NativeUInt Ident, int AHelpContext)/* overload */ : EAbCabException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbInvalidCabFile(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbInvalidCabFile(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbInvalidCabFile(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbCabException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbInvalidCabFile(void) { }
	
};

#pragma pack(pop)

class DELPHICLASS EAbFileTooLarge;
#pragma pack(push,4)
class PASCALIMPLEMENTATION EAbFileTooLarge : public EAbException
{
	typedef EAbException inherited;
	
public:
	__fastcall EAbFileTooLarge(void);
public:
	/* Exception.CreateFmt */ inline __fastcall EAbFileTooLarge(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size) : EAbException(Msg, Args, Args_Size) { }
	/* Exception.CreateRes */ inline __fastcall EAbFileTooLarge(NativeUInt Ident)/* overload */ : EAbException(Ident) { }
	/* Exception.CreateRes */ inline __fastcall EAbFileTooLarge(System::PResStringRec ResStringRec)/* overload */ : EAbException(ResStringRec) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFileTooLarge(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(Ident, Args, Args_Size) { }
	/* Exception.CreateResFmt */ inline __fastcall EAbFileTooLarge(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size)/* overload */ : EAbException(ResStringRec, Args, Args_Size) { }
	/* Exception.CreateHelp */ inline __fastcall EAbFileTooLarge(const System::UnicodeString Msg, int AHelpContext) : EAbException(Msg, AHelpContext) { }
	/* Exception.CreateFmtHelp */ inline __fastcall EAbFileTooLarge(const System::UnicodeString Msg, System::TVarRec const *Args, const int Args_Size, int AHelpContext) : EAbException(Msg, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFileTooLarge(NativeUInt Ident, int AHelpContext)/* overload */ : EAbException(Ident, AHelpContext) { }
	/* Exception.CreateResHelp */ inline __fastcall EAbFileTooLarge(System::PResStringRec ResStringRec, int AHelpContext)/* overload */ : EAbException(ResStringRec, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFileTooLarge(System::PResStringRec ResStringRec, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(ResStringRec, Args, Args_Size, AHelpContext) { }
	/* Exception.CreateResFmtHelp */ inline __fastcall EAbFileTooLarge(NativeUInt Ident, System::TVarRec const *Args, const int Args_Size, int AHelpContext)/* overload */ : EAbException(Ident, Args, Args_Size, AHelpContext) { }
	/* Exception.Destroy */ inline __fastcall virtual ~EAbFileTooLarge(void) { }
	
};

#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
extern DELPHI_PACKAGE void __fastcall AbConvertException(System::Sysutils::Exception* const E, Abutils::TAbErrorClass &eClass, int &eErrorCode);
}	/* namespace Abexcept */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_ABEXCEPT)
using namespace Abexcept;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// AbexceptHPP
