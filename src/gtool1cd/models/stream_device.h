/*
    GTool1CD provides GUI front end to Tool1CD library
    Copyright © 2009-2017 awa
    Copyright © 2017-2018 E8 Tools contributors

    This file is part of GTool1CD.

    GTool1CD is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    GTool1CD is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with GTool1CD.  If not, see <http://www.gnu.org/licenses/>.
*/
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
