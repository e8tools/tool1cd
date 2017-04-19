tool: cTool_1CD_Main.cpp ParseCommandLine.cpp MessageRegistration.cpp Class_1CD.cpp Common.cpp
	g++ -o $@ $? -std=c++11 -Wfatal-errors \
	-I../../SystemClasses -L../../SystemClasses \
	-fpermissive -ldmpassystem -lboost_regex -lboost_filesystem -lboost_system
