#include "System.Classes.hpp"

using namespace System;
using namespace System::Classes;

String doit(char d1, char d2)
{
	String s = (int)d1;
	s += ".";
	s += (int)d2;

	return s;
}

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

	if (doit('\x00', '\x02') == "0.2") {
		std::cout << "Ok!" << std::endl;
	} else {
		std::cout << "Bug :(" << std::endl;
	}
	return 0;
}