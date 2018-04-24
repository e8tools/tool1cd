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

#ifndef UZLibH
#define UZLibH


#include <System.Classes.hpp>
#include <string>

#include "DetailedException.h"
#include "zlib.h"

class ZError: public DetailedException
{
public:
		explicit ZError(const std::string &message) throw()
		: DetailedException(message)
	{}
		virtual ~ZError() noexcept {}
};

//---------------------------------------------------------------------------
/*!
 * @brief Base class for Zlib streams
 */
class ZStreamBase
{
public:
	/*!
	 * @brief Buffert type
	 */
	typedef unsigned char char_t;

	/*!
	 * @brief Constructor
	 */
	ZStreamBase();

	/*!
	 * @brief Destructor
	 */
	virtual ~ZStreamBase();

	/*!
	 * @brief Byte available in input
	 *
	 * @return true if not data are available in input
	 */
	bool isInEmpty() const { return z_stream_.avail_in == 0; }

	/*!
	 * @briet Byte available in output
	 *
	 * @return true if not data are available in output
	 */
	bool isOutEmpty() const { return z_stream_.avail_out == 0; }

	/*!
	 * @brief Stream input complete on inflate
	 *
	 * @return true if the stream input is compltete on inflate
	 */
	bool isStreamEnd() const { return stream_end_; }

	/*!
	 * @brief Write a buffer to the ZLib stream
	 *
	 * @param buf pointer to the buffer
	 * @param buf_size buffer size
	 */
	void write(char_t* buf, size_t buf_size);

	/*!
	 * @brief Read a buffer from the ZLib stream
	 *
	 * @param buf pointer to the buffer
	 * @param buf_size buffer size
	 */
	virtual void read(char_t* buf, size_t buf_size) = 0;

	/*!
	 * @brief Read all ZLib output stream
	 *
	 * Use the read function for read each data chunk until
	 * the output buffer is empty, isOutEmpty return true.
	 *
	 * @param buf return a new allocated buffer
	 * @param buf_size return the size of the allocated buffer
	 */
	void readAll(char_t** buf, size_t& buf_size);

	/*!
	 * @brief Mark as complete the write operation
	 */
	void flush() { flush_ = true; }

protected:
	z_stream z_stream_;
	bool flush_;
	bool stream_end_;
};


class ZDeflateStream : ZStreamBase
{
	/*!
	 * @brief Constructor
	 */
	ZDeflateStream();

	/*!
	 * @brief Destructor
	 */
	virtual ~ZDeflateStream();
};

class ZInflateStream : ZStreamBase
{
	/*!
	 * @brief Constructor
	 */
	ZInflateStream();

	/*!
	 * @brief Destructor
	 */
	virtual ~ZInflateStream();
};


void ZInflateStream(TStream* src, TStream* dst);
bool ZDeflateStream(TStream* src, TStream* dst);

void ZInflateOrCopy(TStream *src, TStream *dst);

int inf(FILE *source, FILE *dest);
int def(FILE *source, FILE *dest, int level);

#endif
