all: programa


programa: pre_parser.o main.o
	g++ pre_parser.o main.o -o montador

pre_parser.o: pre_parser.cpp pre_parser.hpp
	g++ pre_parser.cpp -c

main.o: main.cpp
	g++ main.cpp -c
