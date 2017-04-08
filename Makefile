
main: main.cpp
	g++ $? -o $@ -I . -std=c++11 -lboost_system -lboost_filesystem
