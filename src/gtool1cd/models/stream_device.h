#ifndef STREAM_DEVICE_H
#define STREAM_DEVICE_H

#include <QIODevice>
#include <TStream.hpp>

class StreamDevice : public QIODevice
{
public:
	explicit StreamDevice(TStream *stream);
	virtual ~StreamDevice();

	bool isSequential() const override;
	void close() override;

	TStream *get_stream();

protected:

	qint64 readData(char *data, qint64 maxlen) override;
	qint64 writeData(const char *data, qint64 len) override;
	qint64 size() const override;
	qint64 pos() const override;
	bool seek(qint64 pos) override;

private:
	TStream *stream;
};

#endif // STREAM_DEVICE_H
