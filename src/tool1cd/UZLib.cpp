/*
    Tool1CD library provides access to 1CD database files.
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of Tool1CD Library.

    Tool1CD Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Tool1CD Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with Tool1CD Library.  If not, see <http://www.gnu.org/licenses/>.
*/
#include <stdio.h>
#include <assert.h>
#include <memory>

#include "UZLib.h"

#if !defined(_WIN32)
#pragma package(smart_init)
#endif

const int CHUNKSIZE = 16384;

#ifndef DEF_MEM_LEVEL
#	if MAX_MEM_LEVEL >= 8
		const int DEF_MEM_LEVEL = 8;
#	else
		const int DEF_MEM_LEVEL = MAX_MEM_LEVEL
#	endif
#endif

//---------------------------------------------------------------------------
bool ZDeflateStream(TStream* src, TStream* dst)
{
	int ret, flush;
	unsigned have;
	z_stream strm;
	unsigned char in[CHUNKSIZE];
	unsigned char out[CHUNKSIZE];

	// allocate deflate state
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;

	ret = deflateInit2(&strm, Z_BEST_COMPRESSION, Z_DEFLATED, -MAX_WBITS, DEF_MEM_LEVEL, Z_DEFAULT_STRATEGY);

	if (ret != Z_OK) {
		return false;
	}

	// compress until end of file
	do {
		strm.avail_in = static_cast<unsigned int>(src->Read(in, CHUNKSIZE));
		if (strm.avail_in == 0) {
			return false;
		}
		/* TODO: ZDeflateStream проверка ошибок */

		flush = (strm.avail_in < CHUNKSIZE) ? Z_FINISH : Z_NO_FLUSH;
		strm.next_in = in;

		// run deflate() on input until output buffer not full, finish
		//   compression if all of source has been read in
		do {
			strm.avail_out = CHUNKSIZE;
			strm.next_out = out;
			ret = deflate(&strm, flush);    // no bad return value
			assert(ret != Z_STREAM_ERROR);  // state not clobbered
			have = CHUNKSIZE - strm.avail_out;

			if (have > 0) {
				uint32_t data_written = static_cast<uint32_t>(dst->Write(out, have));
				if (data_written < have) {
					(void) deflateEnd(&strm);
					return false; //  Z_ERRNO
				}
			}
		} while (strm.avail_out == 0);
		assert(strm.avail_in == 0);     // all input will be used

		// done when last data in file processed
	} while (flush != Z_FINISH);
	assert(ret == Z_STREAM_END);        // stream will be complete

	// clean up and return
	(void)deflateEnd(&strm);
	return true;
}

//---------------------------------------------------------------------------
void ZInflateStream(TStream* src, TStream* dst)
{
	z_stream strm;
	int ret;
	
	unsigned have;

	std::unique_ptr<unsigned char[]> srcBuf(new unsigned char[CHUNKSIZE]);
	std::unique_ptr<unsigned char[]> dstBuf(new unsigned char[CHUNKSIZE]);

	/* allocate inflate state */
	strm.zalloc   = Z_NULL;
	strm.zfree    = Z_NULL;
	strm.opaque   = Z_NULL;
	strm.avail_in = 0;
	strm.next_in  = Z_NULL;

	ret = inflateInit2(&strm, -MAX_WBITS);

	/* decompress until deflate stream ends or end of file */
	do {
		
		strm.avail_in = static_cast<int>(src->Read(srcBuf.get(), CHUNKSIZE));

		if (strm.avail_in == 0) break;

		strm.next_in = srcBuf.get();

		/* run inflate() on input until output buffer not full */
		do {
			strm.avail_out = CHUNKSIZE;
			strm.next_out = dstBuf.get();
			ret = inflate(&strm, Z_NO_FLUSH);

			switch (ret) {
			case Z_STREAM_ERROR:
				(void)inflateEnd(&strm);
				throw ZError( "The stream structure was inconsistent" );
				break;
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;
			case Z_DATA_ERROR:
				(void)inflateEnd(&strm);
				throw ZError( "Input data was corrupted" );
				break;
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				throw ZError( "Not enough memory" );
				break;
			}

			have = CHUNKSIZE - strm.avail_out;
			dst->Write(dstBuf.get(), have);

		} while (strm.avail_out == 0);

		/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	(void)inflateEnd(&strm);

}


void ZInflateOrCopy(TStream *src, TStream *dst)
{
	try {

		ZInflateStream(src, dst);

	} catch (ZError &) {
		dst->CopyFrom(src, 0);
	}
}
