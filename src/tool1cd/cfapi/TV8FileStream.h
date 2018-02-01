#ifndef TV8FILESTREAMH
#define TV8FILESTREAMH

#include <System.Classes.hpp>
#include "V8File.h"

class v8file;

class TV8FileStream : public TStream {
public:
	TV8FileStream(v8file* f, bool ownfile = false);
	virtual ~TV8FileStream();
	virtual int64_t Read(void *Buffer, int64_t Count) override;
	virtual int Read(std::vector<uint8_t> Buffer, int Offset, int Count);
	virtual int64_t Write(const void *Buffer, int64_t Count) override;
	virtual int Write(const std::vector<uint8_t> Buffer, int Offset, int Count);
	virtual int64_t Seek(const int64_t Offset, TSeekOrigin Origin) override;
protected:
	v8file* file;
	bool own;
	int64_t pos;
};

#endif
