// CodeGear C++Builder
// Copyright (c) 1995, 2012 by Embarcadero Technologies, Inc.
// All rights reserved

// (DO NOT EDIT: machine generated header) 'AbConst.pas' rev: 24.00 (Windows)

#ifndef AbconstHPP
#define AbconstHPP

#pragma delphiheader begin
#pragma option push
#pragma option -w-      // All warnings off
#pragma option -Vx      // Zero-length empty class member 
#pragma pack(push,8)
#include <System.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Abconst
{
//-- type declarations -------------------------------------------------------
//-- var, const, procedure ---------------------------------------------------
#define AbVersion  (5.000000E+00)
#define AbVersionS L"5.0"
static const System::Byte Ab_MessageLen = System::Byte(0xff);
static const System::Int8 Ab_CaptionLen = System::Int8(0x50);
static const System::WideChar AB_ZIPPATHDELIM = (System::WideChar)(0x2f);
static const System::Int8 AbZipVersionNeeded = System::Int8(0x1);
static const System::Int8 AbUnknownCompressionMethod = System::Int8(0x2);
static const System::Int8 AbNoExtractionMethod = System::Int8(0x3);
static const System::Int8 AbInvalidPassword = System::Int8(0x4);
static const System::Int8 AbNoInsertionMethod = System::Int8(0x5);
static const System::Int8 AbInvalidFactor = System::Int8(0x6);
static const System::Int8 AbDuplicateName = System::Int8(0x7);
static const System::Int8 AbUnsupportedCompressionMethod = System::Int8(0x8);
static const System::Int8 AbUserAbort = System::Int8(0x9);
static const System::Int8 AbArchiveBusy = System::Int8(0xa);
static const System::Int8 AbBadSpanStream = System::Int8(0xb);
static const System::Int8 AbNoOverwriteSpanStream = System::Int8(0xc);
static const System::Int8 AbNoSpannedSelfExtract = System::Int8(0xd);
static const System::Int8 AbStreamFull = System::Int8(0xe);
static const System::Int8 AbNoSuchDirectory = System::Int8(0xf);
static const System::Int8 AbInflateBlockError = System::Int8(0x10);
static const System::Int8 AbBadStreamType = System::Int8(0x11);
static const System::Int8 AbTruncateError = System::Int8(0x12);
static const System::Int8 AbZipBadCRC = System::Int8(0x13);
static const System::Int8 AbZipBadStub = System::Int8(0x14);
static const System::Int8 AbFileNotFound = System::Int8(0x15);
static const System::Int8 AbInvalidLFH = System::Int8(0x16);
static const System::Int8 AbNoArchive = System::Int8(0x17);
static const System::Int8 AbErrZipInvalid = System::Int8(0x18);
static const System::Int8 AbReadError = System::Int8(0x19);
static const System::Int8 AbInvalidIndex = System::Int8(0x1a);
static const System::Int8 AbInvalidThreshold = System::Int8(0x1b);
static const System::Int8 AbUnhandledFileType = System::Int8(0x1c);
static const System::Int8 AbSpanningNotSupported = System::Int8(0x1d);
static const System::Int8 AbBBSReadTooManyBytes = System::Int8(0x28);
static const System::Int8 AbBBSSeekOutsideBuffer = System::Int8(0x29);
static const System::Int8 AbBBSInvalidOrigin = System::Int8(0x2a);
static const System::Int8 AbBBSWriteTooManyBytes = System::Int8(0x2b);
static const System::Int8 AbNoCabinetDllError = System::Int8(0x32);
static const System::Int8 AbFCIFileOpenError = System::Int8(0x33);
static const System::Int8 AbFCIFileReadError = System::Int8(0x34);
static const System::Int8 AbFCIFileWriteError = System::Int8(0x35);
static const System::Int8 AbFCIFileCloseError = System::Int8(0x36);
static const System::Int8 AbFCIFileSeekError = System::Int8(0x37);
static const System::Int8 AbFCIFileDeleteError = System::Int8(0x38);
static const System::Int8 AbFCIAddFileError = System::Int8(0x39);
static const System::Int8 AbFCICreateError = System::Int8(0x3a);
static const System::Int8 AbFCIFlushCabinetError = System::Int8(0x3b);
static const System::Int8 AbFCIFlushFolderError = System::Int8(0x3c);
static const System::Int8 AbFDICopyError = System::Int8(0x3d);
static const System::Int8 AbFDICreateError = System::Int8(0x3e);
static const System::Int8 AbInvalidCabTemplate = System::Int8(0x3f);
static const System::Int8 AbInvalidCabFile = System::Int8(0x40);
static const System::Int8 AbSWSNotEndofStream = System::Int8(0x50);
static const System::Int8 AbSWSSeekFailed = System::Int8(0x51);
static const System::Int8 AbSWSWriteFailed = System::Int8(0x52);
static const System::Int8 AbSWSInvalidOrigin = System::Int8(0x53);
static const System::Int8 AbSWSInvalidNewOrigin = System::Int8(0x54);
static const System::Int8 AbVMSReadTooManyBytes = System::Int8(0x64);
static const System::Int8 AbVMSInvalidOrigin = System::Int8(0x65);
static const System::Int8 AbVMSErrorOpenSwap = System::Int8(0x66);
static const System::Int8 AbVMSSeekFail = System::Int8(0x67);
static const System::Int8 AbVMSReadFail = System::Int8(0x68);
static const System::Int8 AbVMSWriteFail = System::Int8(0x69);
static const System::Int8 AbVMSWriteTooManyBytes = System::Int8(0x6a);
static const System::Byte AbGZipInvalid = System::Byte(0xc8);
static const System::Byte AbGzipBadCRC = System::Byte(0xc9);
static const System::Byte AbGzipBadFileSize = System::Byte(0xca);
static const System::Byte AbTarInvalid = System::Byte(0xdc);
static const System::Byte AbTarBadFileName = System::Byte(0xdd);
static const System::Byte AbTarBadLinkName = System::Byte(0xde);
static const System::Byte AbTarBadOp = System::Byte(0xdf);
extern DELPHI_PACKAGE System::UnicodeString __fastcall AbStrRes(int Index);
}	/* namespace Abconst */
#if !defined(DELPHIHEADER_NO_IMPLICIT_NAMESPACE_USE) && !defined(NO_USING_NAMESPACE_ABCONST)
using namespace Abconst;
#endif
#pragma pack(pop)
#pragma option pop

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// AbconstHPP
