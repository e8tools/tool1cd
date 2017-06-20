//---------------------------------------------------------------------------

#pragma hdrstop

#include <stdio.h>
#include <assert.h>
#include <malloc.h>
#include "UZLib.h"

//---------------------------------------------------------------------------
// warning C4068: unknown pragma in VC++

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

/*
void ZInflateStream_Old(TStream* src, TStream* dst)
{
	z_stream strm;
	int ret;
	uintmax_t srcSize;

	unsigned char srcBuf[CHUNKSIZE] = {0};
	unsigned char dstBuf[CHUNKSIZE] = {0};

	strm.zalloc   = Z_NULL;
	strm.zfree    = Z_NULL;
	strm.opaque   = Z_NULL;
	strm.avail_in = 0;
	strm.next_in  = Z_NULL;

	ret = inflateInit2(&strm, -MAX_WBITS);

	srcSize = src->GetSize();

	src->Read(srcBuf, srcSize);
	strm.avail_in  = srcSize;
	strm.avail_out = CHUNKSIZE;
	strm.next_in   = srcBuf;
	strm.next_out  = dstBuf;

	ret = inflate(&strm, Z_NO_FLUSH);

	(void)inflateEnd(&strm);

	dst->Write(dstBuf, strm.total_out);

}
*/

//---------------------------------------------------------------------------
bool ZDeflateStream(TStream* src, TStream* dst)
{
	int ret, flush;
	//unsigned have;
	size_t have;
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
		strm.avail_in = src->Read(in, CHUNKSIZE);
		if (strm.avail_in == 0) {
			return false;
		}
		/* TODO: Check error */

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

			size_t data_written = dst->Write(out, have);

			if (data_written < have) {
				(void)deflateEnd(&strm);
				return false; //  Z_ERRNO
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
void _ZInflateStream_(TStream* src, TStream* dst)
{
	z_stream strm;
	int ret;
	size_t srcSize;

	unsigned have;

	unsigned char srcBuf[CHUNKSIZE] = {0};
	unsigned char dstBuf[CHUNKSIZE] = {0};


	/* allocate inflate state */
	strm.zalloc   = Z_NULL;
	strm.zfree    = Z_NULL;
	strm.opaque   = Z_NULL;
	strm.avail_in = 0;
	strm.next_in  = Z_NULL;

	ret = inflateInit2(&strm, -MAX_WBITS);

	/* decompress until deflate stream ends or end of file */
	do {
		//strm.avail_in = fread(in, 1, CHUNKSIZE, source);

		srcSize = src->GetSize();         // определяем размер данных
		src->Read(srcBuf, srcSize);  // читаем из потока в буфер данные
		strm.avail_in = srcSize;

		if (strm.avail_in == 0)
			break;

		strm.next_in = srcBuf;

		/* run inflate() on input until output buffer not full */
		do {
			strm.avail_out = CHUNKSIZE;
			strm.next_out = dstBuf;
			ret = inflate(&strm, Z_NO_FLUSH);

			switch (ret) {
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;     /* and fall through */
			case Z_DATA_ERROR:
				(void)inflateEnd(&strm);
				return;
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				return;
			case Z_STREAM_ERROR:
				(void)inflateEnd(&strm);
				return;
			}

			have = CHUNKSIZE - strm.avail_out;
			dst->Write(dstBuf, have);

		} while (strm.avail_out == 0);

		/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	(void)inflateEnd(&strm);

}

//---------------------------------------------------------------------------
bool ZInflateStream(TStream* src, TStream* dst)
{
	z_stream strm;
	int ret;
	//uintmax_t srcSize;

	unsigned have;

	unsigned char *srcBuf;
	unsigned char *dstBuf;

	srcBuf = (unsigned char *)malloc(CHUNKSIZE);
	dstBuf = (unsigned char *)malloc(CHUNKSIZE);

	/* allocate inflate state */
	strm.zalloc   = Z_NULL;
	strm.zfree    = Z_NULL;
	strm.opaque   = Z_NULL;
	strm.avail_in = 0;
	strm.next_in  = Z_NULL;

	ret = inflateInit2(&strm, -MAX_WBITS);

	/* decompress until deflate stream ends or end of file */
	do {
		strm.avail_in = src->Read(static_cast<unsigned char *>(srcBuf), CHUNKSIZE);

		if (strm.avail_in == 0) break;

		strm.next_in = srcBuf;

		/* run inflate() on input until output buffer not full */
		do {
			strm.avail_out = CHUNKSIZE;
			strm.next_out = dstBuf;
			ret = inflate(&strm, Z_NO_FLUSH);

			switch (ret) {
			case Z_NEED_DICT:
			{
				ret = Z_DATA_ERROR;     /* and fall through */
				return false;
			}
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
			case Z_STREAM_ERROR:
			{
				(void)inflateEnd(&strm);
				free(srcBuf);
				free(dstBuf);
				return false;
			}
			}

			have = CHUNKSIZE - strm.avail_out;
			dst->Write(static_cast<unsigned char *>(dstBuf), have);

		} while (strm.avail_out == 0);

		/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	(void)inflateEnd(&strm);

	free(srcBuf);
	free(dstBuf);
	return true;
}


/* Decompress from file source to file dest until stream ends or EOF.
   inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_DATA_ERROR if the deflate data is
   invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
   the version of the library linked do not match, or Z_ERRNO if there
   is an error reading or writing the files. */
int inf(FILE *source, FILE *dest)
{
	int ret;
	unsigned have;
	z_stream strm;
	unsigned char in[CHUNKSIZE];
	unsigned char out[CHUNKSIZE];

	/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	//ret = inflateInit(&strm);
	ret = inflateInit2(&strm, -MAX_WBITS);
	if (ret != Z_OK)
		return ret;

	/* decompress until deflate stream ends or end of file */
	do {
		strm.avail_in = fread(in, 1, CHUNKSIZE, source);
		if (ferror(source)) {
			(void)inflateEnd(&strm);
			return Z_ERRNO;
		}
		if (strm.avail_in == 0)
			break;
		strm.next_in = in;

		/* run inflate() on input until output buffer not full */
		do {
			strm.avail_out = CHUNKSIZE;
			strm.next_out = out;
			ret = inflate(&strm, Z_NO_FLUSH);
			assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
			switch (ret) {
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;     /* and fall through */
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				return ret;
			}
			have = CHUNKSIZE - strm.avail_out;
			if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
				(void)inflateEnd(&strm);
				return Z_ERRNO;
			}
		} while (strm.avail_out == 0);

		/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	(void)inflateEnd(&strm);
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}

/* Compress from file source to file dest until EOF on source.
   def() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_STREAM_ERROR if an invalid compression
   level is supplied, Z_VERSION_ERROR if the version of zlib.h and the
   version of the library linked do not match, or Z_ERRNO if there is
   an error reading or writing the files. */
int def(FILE *source, FILE *dest, int level)
{
	int ret, flush;
	unsigned have;
	z_stream strm;
	unsigned char in[CHUNKSIZE];
	unsigned char out[CHUNKSIZE];

	/* allocate deflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	ret = deflateInit(&strm, level);
	if (ret != Z_OK)
		return ret;

	/* compress until end of file */
	do {
		strm.avail_in = fread(in, 1, CHUNKSIZE, source);
		if (ferror(source)) {
			(void)deflateEnd(&strm);
			return Z_ERRNO;
		}
		flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
		strm.next_in = in;

		/* run deflate() on input until output buffer not full, finish
		   compression if all of source has been read in */
		do {
            strm.avail_out = CHUNKSIZE;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNKSIZE - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

		/* done when last data in file processed */
	} while (flush != Z_FINISH);
	assert(ret == Z_STREAM_END);        /* stream will be complete */

	/* clean up and return */
	(void)deflateEnd(&strm);
	return Z_OK;
}

/*

void ZStreamBase::readAll(char_t** buf, size_t& buf_size)
{
	std::stringstream out_stream;

	char_t out_buf[ChunkSize];

    do
    {
        read (out_buf, ChunkSize);

		size_t out_size = ChunkSize - z_stream_.avail_out;
		out_stream.write (reinterpret_cast< const char* >(out_buf), out_size);

		if (out_stream.bad())
			throw ZError( "Error writing temporary stream" );

	} while (isOutEmpty());

	buf_size = out_stream.str().size();
	*buf = new char_t[buf_size];
	out_stream.read (reinterpret_cast< std::stringstream::char_type* >(*buf), buf_size);;
}

void ZStreamBase::write(char_t* buf, size_t buf_size)
{
	z_stream_.avail_in = buf_size;
	z_stream_.next_in = reinterpret_cast< Bytef* >(buf);
}

//============================================================================
ZDeflateStream::ZDeflateStream(int compression_level) : compression_level_( compression_level )
{
	int ret = deflateInit (&z_stream_, compression_level_);

	if (ret != Z_OK)
		throw ZError ("Can not initialize deflate stream");
}

ZDeflateStream::~ZDeflateStream()
{
	deflateEnd (&z_stream_);
}

void ZDeflateStream::read(char_t* buf, size_t buf_size)
{
    z_stream_.avail_out = buf_size;
    z_stream_.next_out = reinterpret_cast< Bytef* >(buf);

    int ret = deflate (&z_stream_, flush_ ? Z_FINISH : Z_NO_FLUSH);

    if (ret == Z_STREAM_ERROR)
        throw ZError( "Error on deflate" );
}

//============================================================================
ZInflateStream::ZInflateStream()
{
	z_stream_.avail_in = 0;
	z_stream_.next_in = Z_NULL;

	int ret = inflateInit2 (&z_stream_, -MAX_WBITS);

	if (ret != Z_OK)
		throw ZError ("Can not initialize inflate stream");
}

ZInflateStream::~ZInflateStream()
{
	inflateEnd (&z_stream_);
}

void ZInflateStream::read(char_t* buf, size_t buf_size)
{
	z_stream_.avail_out = buf_size;
	z_stream_.next_out = reinterpret_cast< Bytef* >(buf);

	int ret = inflate (&z_stream_, Z_NO_FLUSH);

	switch (ret)
	{
		case Z_STREAM_ERROR:
			throw ZError( "The stream structure was inconsistent" );
			break;
		case Z_NEED_DICT:
			throw ZError( "A preset dictionary is needed" );
			break;
		case Z_DATA_ERROR:
			throw ZError( "Input data was corrupted" );
			break;
		case Z_MEM_ERROR:
			throw ZError( "Not enough memory" );
			break;
		case Z_STREAM_END:
			stream_end_ = true;
			break;
	}
}


*/
