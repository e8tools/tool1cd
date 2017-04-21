main: main.cpp libdmpassystem.a
	g++ main.cpp -o $@ -I. -std=c++11 -ldmpassystem -lboost_system -lboost_filesystem -L. -fpermissive

implementation.o: implementation.cpp $(wildcard *.hpp)
	g++ -c implementation.cpp -o implementation.o -I. -std=c++11 -fpermissive -g -pg

libdmpassystem.a: implementation.o
	ar rvs $@ $?
