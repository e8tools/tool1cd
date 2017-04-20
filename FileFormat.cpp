//---------------------------------------------------------------------------
//#include <vcl.h>
#include <System.Classes.hpp>
//#pragma hdrstop

#include "FileFormat.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------
const unsigned char SIG_GIF87[6] = {'G', 'I', 'F', '8', '7', 'a'}; // версия 87 года
const unsigned char SIG_GIF89[6] = {'G', 'I', 'F', '8', '9', 'a'}; // версия 89 года
const unsigned char SIG_UTF8[3] = {0xEF, 0xBB, 0xBF};
const unsigned char SIG_PCX25[3] = {0x0a, 0x00, 0x01}; // версия 2.5
const unsigned char SIG_PCX28P[3] = {0x0a, 0x02, 0x01}; // версия 2.8  с информацией о палитре
const unsigned char SIG_PCX28[3] = {0x0a, 0x03, 0x01}; // Версия 2.8 без информации о палитре
const unsigned char SIG_PCX30[3] = {0x0a, 0x05, 0x01}; // Версия 3.0
const unsigned char SIG_BMP[2] = {0x42, 0x4d};
const unsigned char SIG_JPG[3] = {0xff, 0xd8, 0xff};
const unsigned char SIG_PNG[16] = {0x89, 0x50, 0x4E, 0x47, 0x0D, 0x0A, 0x1A, 0x0A, 0x00, 0x00, 0x00, 0x0D, 0x49, 0x48, 0x44, 0x52};
const unsigned char SIG_BIGTIFF[4] = {0x4D, 0x4D, 0x00, 0x2B};
const unsigned char SIG_TIFFBE[4] = {0x4D, 0x4D, 0x00, 0x2A};
const unsigned char SIG_TIFFLE[4] = {0x49, 0x49, 0x2A, 0x00};
const unsigned char SIG_ICO[4] = {0x00, 0x00, 0x01, 0x00};
const unsigned char SIG_WMFOLD[6] = {0x01, 0x00, 0x09, 0x00, 0x00, 0x03};
const unsigned char SIG_WMF[6] = {0xD7, 0xCD, 0xC6, 0x9A, 0x00, 0x00};
const unsigned char SIG_EMF[4] = {0x01, 0x00, 0x00, 0x00};
const unsigned char SIG_ZIP[2] = {'P', 'K'};

//---------------------------------------------------------------------------

file_format __fastcall get_file_format(TStream* s)
{
	unsigned char buf[32];
	int len;

	s->Seek(0, soFromBeginning);
	len = s->Read(buf, 32);

	if(len < 2) return ff_unknown;
	if(!memcmp(buf, SIG_BMP, 2)) return ff_bmp;
	if(!memcmp(buf, SIG_ZIP, 2)) return ff_zip;
	if(len < 3) return ff_unknown;
	if(!memcmp(buf, SIG_JPG, 3)) return ff_jpg;
	if(!memcmp(buf, SIG_UTF8, 3)) return ff_utf8;
	if(!memcmp(buf, SIG_PCX25, 3)) return ff_pcx;
	if(!memcmp(buf, SIG_PCX28P, 3)) return ff_pcx;
	if(!memcmp(buf, SIG_PCX28, 3)) return ff_pcx;
	if(!memcmp(buf, SIG_PCX30, 3)) return ff_pcx;
	if(len < 4) return ff_unknown;
	if(!memcmp(buf, SIG_BIGTIFF, 4)) return ff_tiff;
	if(!memcmp(buf, SIG_TIFFBE, 4)) return ff_tiff;
	if(!memcmp(buf, SIG_TIFFLE, 4)) return ff_tiff;
	if(!memcmp(buf, SIG_ICO, 4)) return ff_ico;
	if(!memcmp(buf, SIG_EMF, 4)) return ff_emf;
	if(len < 6) return ff_unknown;
	if(!memcmp(buf, SIG_GIF87, 6)) return ff_gif;
	if(!memcmp(buf, SIG_GIF89, 6)) return ff_gif;
	if(!memcmp(buf, SIG_WMFOLD, 6)) return ff_wmf;
	if(!memcmp(buf, SIG_WMF, 6)) return ff_wmf;
	if(len < 16) return ff_unknown;
	if(!memcmp(buf, SIG_PNG, 16)) return ff_png;

	return ff_unknown;
}

//---------------------------------------------------------------------------
