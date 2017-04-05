//---------------------------------------------------------------------------

#ifndef FileFormatH
#define FileFormatH
//---------------------------------------------------------------------------

enum file_format
{
	ff_unknown, // неизвестный
	ff_gif, // GIF
	ff_utf8, // UTF-8
	ff_pcx, // PCX
	ff_bmp,
	ff_jpg,
	ff_png,
	ff_tiff,
	ff_ico,
	ff_wmf,
	ff_emf,
	ff_zip
};

file_format __fastcall get_file_format(TStream* s);

#endif
