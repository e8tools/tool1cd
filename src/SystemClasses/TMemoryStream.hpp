#ifndef SYSTEM_CLASSES_TMEMORYSTREAM
#define SYSTEM_CLASSES_TMEMORYSTREAM

#include <cstring>
#include <vector>

#include "System.hpp"
#include "TStream.hpp"

namespace System {

namespace Classes {

class TMemoryStream : public TStream
{
public:

	TMemoryStream();
	virtual ~TMemoryStream();

	virtual int64_t GetSize() const override;

	virtual void    SetSize(int64_t NewSize) override;

	virtual int64_t Read(void *Buffer, int64_t Count) override;

	virtual int64_t Write(const void *Buffer, int64_t Count) override;

	const std::vector<t::Byte> &GetBytes() const;

	void *GetMemory();

private:

	void EnsureSize(size_t size);

	std::vector<t::Byte> _data;
};

class TBytesStream : public TMemoryStream
{
public:
	TBytesStream(const std::vector<t::Byte> &initial);

	virtual ~TBytesStream();
};

} // Classes

} // System

#endif
