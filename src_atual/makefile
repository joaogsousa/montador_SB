all: programa


programa: main.o pre_parser.o
	g++ main.o pre_parser.o -std=c++11 -o montador

main.o: main.cpp
	g++ main.cpp  -std=c++11 -c

pre_parser.o: pre_parser.cpp pre_parser.hpp
	g++ pre_parser.cpp -std=c++11 -c


pre:
	./montador -p inputs/input outputs/output

macro:
	./montador -m inputs/input outputs/output

obj:
	./montador -o inputs/input outputs/output

clear:
	rm *.o
