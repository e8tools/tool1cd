main: main.cpp libdmpassystem.a
	g++ main.cpp -o $@ -I. -std=c++11 -ldmpassystem -lboost_system -lboost_filesystem -L. -fpermissive

.cpp.o: $(wildcard *.hpp)
	g++ -c $< -o $@ -I. -std=c++11 -fpermissive -g -pg

libdmpassystem.a: String.o System.Classes.o System.o System.IOUtils.o \
	System.SysUtils.o TMemoryStream.o TStreamReader.o TStreamWriter.o TStream.o \
	TFileStream.o
	ar rvs $@ $?
