(* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is TurboPower Abbrevia
 *
 * The Initial Developer of the Original Code is
 * TurboPower Software
 *
 * Portions created by the Initial Developer are Copyright (C) 1997-2002
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 * Craig Peterson <capeterson@users.sourceforge.net>
 *
 * ***** END LICENSE BLOCK ***** *)

{*********************************************************}
{* ABBREVIA: AbCrtl.pas                                  *}
{*********************************************************}
{* ABBREVIA: C++Builder C runtime functions              *}
{*********************************************************}

unit AbCrtl;

{$I AbDefine.inc}

interface

uses
  Windows;

type
  UInt32 = LongWord;
  size_t = {$IF defined(CPUX64)}Int64{$ELSE}Integer{$IFEND}; // NativeInt is 8 bytes in Delphi 2007

const
  __turboFloat: LongInt = 0;
  _fltused: LongInt = 0;

procedure abs; cdecl;
  external 'msvcrt.dll';
procedure _llshl; cdecl;
  external 'msvcrt.dll';
procedure _llushr; cdecl;
  external 'msvcrt.dll';
procedure _ftol; cdecl;
  external 'msvcrt.dll' {$IFDEF BCB}name '__ftol'{$ENDIF};

{ ctype.h declarations ===================================================== }
function isdigit(ch: Integer): Integer; cdecl;

{ string.h declarations ==================================================== }
function memcpy(Dest, Src: Pointer; Count: size_t): Pointer; cdecl;
function memmove(Dest, Src: Pointer; Count: size_t): Pointer; cdecl;
function memset(Dest: Pointer; Value: Byte; Count: size_t): Pointer; cdecl;
function strlen(P: PAnsiChar): Integer; cdecl;
function strcpy(Des, Src: PAnsiChar): PAnsiChar; cdecl;
function strncpy(Des, Src: PAnsiChar; MaxLen: Integer): PAnsiChar; cdecl;

function memcmp(s1,s2: Pointer; numBytes: LongWord): integer; cdecl;
  external 'msvcrt.dll';
function wcscpy(strDestination, strSource: PWideChar): PWideChar; cdecl;
  external 'msvcrt.dll';

{ stdlib.h declarations ==================================================== }
function malloc(Size: Integer): Pointer; cdecl;
procedure free(Ptr: Pointer); cdecl;
function realloc(Ptr: Pointer; Size: Integer): Pointer; cdecl;

{ intrin.h declarations ==================================================== }
procedure ___cpuid(CPUInfo: PInteger; InfoType: Integer); cdecl;
  external 'msvcrt.dll';

{ stdio.h declarations ===================================================== }
function sprintf(S: PChar; const Format: PChar): Integer;
  cdecl; varargs; external 'msvcrt.dll' {$IFDEF BCB}name '_sprintf'{$ENDIF};

{ process.h declarations =================================================== }
function _beginthreadex(security: Pointer; stack_size: Cardinal;
  start_address: Pointer; arglist: Pointer; initflag: Cardinal;
  var thrdaddr: Cardinal): THandle; cdecl;

{ MSVC/Win64 declarations ================================================== }
procedure __C_specific_handler; cdecl; external 'msvcrt.dll';

implementation

{ ctype.h declarations ===================================================== }
function isdigit(ch: Integer): Integer; cdecl;
begin
  if AnsiChar(ch) in ['0'..'9'] then
    Result := 1
  else
    Result := 0;
end;

{ string.h declarations ==================================================== }
function memcpy(Dest, Src: Pointer; Count: size_t): Pointer; cdecl;
begin
  System.Move(Src^, Dest^, Count);
  Result := Dest;
end;
{ -------------------------------------------------------------------------- }
function memmove(Dest, Src: Pointer; Count: size_t): Pointer; cdecl;
begin
  System.Move(Src^, Dest^, Count);
  Result := Dest;
end;
{ -------------------------------------------------------------------------- }
function memset(Dest: Pointer; Value: Byte; Count: size_t): Pointer; cdecl;
begin
  FillChar(Dest^, Count, Value);
  Result := Dest;
end;
{ -------------------------------------------------------------------------- }
function strlen(P: PAnsiChar): Integer; cdecl;
{$IF RTLVersion >= 20}
asm
  jmp System.@PCharLen
end;
{$ELSE}
begin
  Result := 0;
  while P^ <> #0 do
    Inc(P);
end;
{$IFEND}
{ -------------------------------------------------------------------------- }
function strcpy(Des, Src: PAnsiChar): PAnsiChar; cdecl;
begin
  Result := Des;
  Move(Src^, Des^, strlen(Src) + 1);
end;
{ -------------------------------------------------------------------------- }
function strncpy(Des, Src: PAnsiChar; MaxLen: Integer): PAnsiChar; cdecl;
var
  Len: Integer;
begin
  Len := strlen(Src);
  if Len > MaxLen then
    Len := MaxLen;
  Move(Src^, Des^, Len);
  if Len < MaxLen then
    FillChar(Des[Len], MaxLen - Len, 0);
  Result := Des;
end;

{ stdlib.h declarations ==================================================== }
function malloc(Size: Integer): Pointer; cdecl;
begin
  GetMem(Result, Size);
end;
{ -------------------------------------------------------------------------- }
procedure free(Ptr: Pointer); cdecl;
begin
  FreeMem(Ptr)
end;
{ -------------------------------------------------------------------------- }
function realloc(Ptr: Pointer; Size: Integer): Pointer; cdecl;
begin
  Result := ReallocMemory(Ptr, Size);
end;

{ process.h declarations =================================================== }
function _beginthreadex(security: Pointer; stack_size: Cardinal;
  start_address: Pointer; arglist: Pointer; initflag: Cardinal;
  var thrdaddr: Cardinal): THandle; cdecl;
begin
  Result := CreateThread(security, stack_size, start_address, arglist,
    initflag, thrdaddr);
end;
{ -------------------------------------------------------------------------- }

end.
