all: programa


programa: main.o pre_parser.o
	g++ main.o pre_parser.o -std=c++11 -o montador

main.o: src/main.cpp
	g++ src/main.cpp  -std=c++11 -c

pre_parser.o: src/pre_parser.cpp header/pre_parser.hpp
	g++ src/pre_parser.cpp -std=c++11 -c


pre:
	@./montador -p inputs/input outputs/output

macro:
	@./montador -m inputs/input outputs/output

obj:
	@./montador -o inputs/input outputs/output

clear:
	rm *.o
