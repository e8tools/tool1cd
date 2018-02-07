#ifndef TV8FILESTREAMH
#define TV8FILESTREAMH

#include <System.Classes.hpp>
#include "V8File.h"

class V8File;

class TV8FileStream : public TStream {
public:
	TV8FileStream(V8File* f, bool ownfile = false);
	virtual ~TV8FileStream();
	virtual int64_t Read(void *Buffer, int64_t Count) override;
	virtual int Read(std::vector<uint8_t> Buffer, int Offset, int Count);
	virtual int64_t Write(const void *Buffer, int64_t Count) override;
	virtual int Write(const std::vector<uint8_t> Buffer, int Offset, int Count);
	virtual int64_t Seek(const int64_t Offset, const TSeekOrigin Origin) override;
protected:
	V8File* file;
	bool own;
	int64_t pos;
};

#endif
