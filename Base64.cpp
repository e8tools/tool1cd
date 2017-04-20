//---------------------------------------------------------------------------


#pragma hdrstop

#include "Base64.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

// Куски исходного кода взяты с http://base64.sourceforge.net/

// Translation Table as described in RFC1113
static const char cb64[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

//// Translation Table to decode (created by author)
static const char cd64[]="|$$$}rstuvwxyz{$$$$$$$>?@ABCDEFGHIJKLMNOPQRSTUVW$$$$$$XYZ[\\]^_`abcdefghijklmnopq";

// encode
// base64 encode a stream adding padding and line breaks as per spec.
// linesize - длина строки. Если linesize = 0, выводится всё в одну строку, без переносов
void __fastcall base64_encode(TStream* infile, TStream* outfile, int linesize)
{
	unsigned char in[3];
	wchar_t out[4];
	int64_t i;
	int len, blocksout = 0;
	int64_t inlenblock = infile->GetSize() / 3;

	infile->Seek(0, soBeginning);
	for(i = 0; i < inlenblock; i++)
	{
		if(blocksout >= linesize/4 && linesize)
		{
			outfile->Write(L"\r\n", 4);
			blocksout = 0;
		}
		blocksout++;
		infile->Read(in, 3);
		out[0] = cb64[in[0] >> 2];
		out[1] = cb64[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)];
		out[2] = cb64[((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)];
		out[3] = cb64[in[2] & 0x3f];
		outfile->Write(out, 8);
	}

	len = infile->GetSize() - infile->GetPosition();
	if(len > 0)
	{
		if(blocksout >= linesize / 4 && linesize) outfile->Write(L"\r\n", 4);
		in[1] = 0;
		in[2] = 0;
		infile->Read(in, len);
		out[0] = cb64[in[0] >> 2];
		out[1] = cb64[((in[0] & 0x03) << 4) | ((in[1] & 0xf0) >> 4)];
		out[2] = len > 1 ? cb64[((in[1] & 0x0f) << 2) | ((in[2] & 0xc0) >> 6)] : L'=';
		out[3] = len > 2 ? cb64[in[2] & 0x3f] : L'=';
		outfile->Write(out, 8);
	}

}


// decode
// decode a base64 encoded stream discarding padding, line breaks and noise
void __fastcall base64_decode(TStream* infile, TStream* outfile)
{
	unsigned char in[4], out[3], v;
	wchar_t s;
	int i, len;

//	infile->Seek(0i64, soBeginning);
	while(infile->GetPosition() < infile->GetSize())
	{
		for(len = 0, i = 0; i < 4 && infile->GetPosition() < infile->GetSize(); i++ )
		{
			v = 0;
			while(infile->GetPosition() < infile->GetSize() && v == 0)
			{
				infile->Read(&s, 2);
				v = (unsigned char)((s < 43 || s > 122) ? 0 : cd64[s - 43]);
				if(v) v = (unsigned char)((v == '$') ? 0 : v - 61);
			}
			if(v)
			{
				in[i] = (unsigned char)(v - 1);
				len++;
			}
//			if(infile->Position < infile->GetSize())
//			{
//				len++;
//				if(v) in[i] = (unsigned char)(v - 1);
//			}
			else in[i] = 0;
		}
		if(len)
		{
			out[0] = (unsigned char) (in[0] << 2 | in[1] >> 4);
			out[1] = (unsigned char) (in[1] << 4 | in[2] >> 2);
			out[2] = (unsigned char) (((in[2] << 6) & 0xc0) | in[3]);
			outfile->Write(out, len - 1);
		}

	}

}

// decode
// decode a base64 encoded stream discarding padding, line breaks and noise
void __fastcall base64_decode(const String& instr, TStream* outfile, int start)
{
	unsigned char in[4], out[3], v;
	char s;
	const char* str;
	int i, len, is, slen;

	slen = instr.Length();
	str = instr.c_str();
	is = start;

	while(is < slen)
	{
		for(len = 0, i = 0; i < 4 && is < slen; i++ )
		{
			v = 0;
			while(is < slen && v == 0)
			{
				s = str[is++];
				v = (unsigned char)((s < 43 || s > 122) ? 0 : cd64[s - 43]);
				if(v) v = (unsigned char)((v == '$') ? 0 : v - 61);
			}
			if(v)
			{
				in[i] = (unsigned char)(v - 1);
				len++;
			}
			else in[i] = 0;
		}
		if(len)
		{
			out[0] = (unsigned char) (in[0] << 2 | in[1] >> 4);
			out[1] = (unsigned char) (in[1] << 4 | in[2] >> 2);
			out[2] = (unsigned char) (((in[2] << 6) & 0xc0) | in[3]);
			outfile->Write(out, len - 1);
		}

	}

}

