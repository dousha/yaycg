default:
	make all

all: main.cpp
	g++ -std=c++11 -O2 main.cpp -o yaycg

yaycg: main.cpp
	make all

run: yaycg
	./yaycg

debug:
	g++ -std=c++11 -g -O2 main.cpp -o yaycg
