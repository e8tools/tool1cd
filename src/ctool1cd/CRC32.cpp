#pragma hdrstop

#include "CRC32.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

unsigned int _crc32(TStream* str)
{
	unsigned int i, c = 0, dwSeed = -1;

	str->Seek(0, soFromBeginning);
	while(str->Read(&c, 1))
	{
		for(i = 0; i < 8; i++ )
		{
			if((dwSeed ^ c) & 1) dwSeed = (dwSeed >> 1) ^ 0xEDB88320;
			else dwSeed = (dwSeed >> 1);
			c >>= 1;
		}
	}
	return ~dwSeed;
}
