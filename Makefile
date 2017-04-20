CFLAGS=-std=c++11 -Wfatal-errors -fpermissive -I../../SystemClasses
LDFLAGS=-L../../SystemClasses -ldmpassystem -lboost_regex -lboost_filesystem -lboost_system -lz

.cpp.o:
	g++ $(CFLAGS) -o $@ -c $<

tool: cTool_1CD_Main.o ParseCommandLine.o MessageRegistration.o Class_1CD.o Common.o ConfigStorage.o Parse_tree.o \
	APIcfBase.o TempStream.o Base64.o UZLib.o
	g++ -o $@ $? $(LDFLAGS)

clean:
	rm *.o
	rm tool

