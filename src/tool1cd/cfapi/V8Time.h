#ifndef V8TIME_H
#define V8TIME_H

#include <System.Classes.hpp>

using namespace System;

class V8Time
{
public:

	static const int64_t EPOCH_START_WIN;
	static V8Time current_time();

	explicit V8Time();
	explicit V8Time(const int64_t value);
	explicit V8Time(const FILETIME &value);

	 ~V8Time() = default;

	FILETIME to_file_time() const;
	void from_file_time(const FILETIME &value);
	size_t write_to_stream(TMemoryStream *out_stream) const;

private:
	int64_t _data {0};

	int64_t inner_from_file_time(const FILETIME &value);
};

#endif // V8TIME_H
