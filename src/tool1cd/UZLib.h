//---------------------------------------------------------------------------

#ifndef UZLibH
#define UZLibH


#include <System.Classes.hpp>
#include <string>

#include "DetailedException.h"
#include "zlib.h"

class ZError: public DetailedException
{
public:
		explicit ZError(const String &message) throw()
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

int inf(FILE *source, FILE *dest);
int def(FILE *source, FILE *dest, int level);

#endif
