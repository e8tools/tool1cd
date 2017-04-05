//---------------------------------------------------------------------------

#ifndef Base64H
#define Base64H

#include <System.Classes.hpp>
//---------------------------------------------------------------------------

void __fastcall base64_encode(TStream* infile, TStream* outfile, int linesize);
void __fastcall base64_decode(TStream* infile, TStream* outfile);
void __fastcall base64_decode(const String& instr, TStream* outfile, int start = 0);

#endif

