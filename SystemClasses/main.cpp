#include "System.Classes.hpp"

using namespace System;
using namespace System::Classes;

int main()
{

	DynamicArray<Byte> buf;
	buf.push_back((Byte)'a');
	buf.push_back((Byte)'b');
	buf.push_back((Byte)'c');
	buf.push_back((Byte)'\n');

	TStream *data = new TMemoryStream();
	data->Write(buf, 0);
	data->Seek(0, soBeginning);

	TStream *stream = new TFileStream("somefile", fmCreate);
	stream->CopyFrom(data, 0);
	return 0;
}