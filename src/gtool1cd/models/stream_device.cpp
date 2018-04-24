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
#include "stream_device.h"
#include "QDebug"

StreamDevice::StreamDevice(TStream *stream)
    : stream(stream)
{
	stream->SetPosition(0);
}

StreamDevice::~StreamDevice()
{
	delete stream;
	stream = nullptr;
}

qint64 StreamDevice::readData(char *data, qint64 maxlen)
{
	return stream->Read(data, maxlen);
}

qint64 StreamDevice::writeData(const char *data, qint64 len)
{
	return stream->Write(data, len);
}

qint64 StreamDevice::size() const
{
	return stream->GetSize();
}

qint64 StreamDevice::pos() const
{
	return stream->GetPosition();
}

bool StreamDevice::seek(qint64 pos)
{
	stream->SetPosition(pos);
	return true;
}

TStream *StreamDevice::get_stream()
{
	return stream;
}

void StreamDevice::close()
{
	emit aboutToClose();
	setErrorString(QString(""));
	stream->Close();
	setOpenMode(NotOpen);
}

bool StreamDevice::isSequential() const
{
	return false;
}

