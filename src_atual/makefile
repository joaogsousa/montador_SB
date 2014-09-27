all: programa


programa: main.o pre_parser.o
	g++ main.o pre_parser.o -std=c++11 -o montador

main.o: main.cpp
	g++ main.cpp  -std=c++11 -c

pre_parser.o: pre_parser.cpp pre_parser.hpp
	g++ pre_parser.cpp -std=c++11 -c


pre:
	./montador -p inputs/input.asm outputs/output.pre

macro:
	./montador -m inputs/input.asm outputs/output.macro

obj:
	./montador -o inputs/input.asm outputs/output.obj

clear:
	rm *.o
	rm montador
