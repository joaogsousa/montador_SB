all: programa


programa: main.o pre_parser.o testes.o pos_parser.o
	g++ main.o pre_parser.o testes.o pos_parser.o -std=c++11 -o montador

main.o: main.cpp
	g++ main.cpp  -std=c++11 -c

pre_parser.o: pre_parser.cpp pre_parser.hpp
	g++ pre_parser.cpp -c

testes.o: testes.cpp testes.hpp
	g++ testes.cpp -c

pos_parser.o: pos_parser.cpp pos_parser.hpp
	g++ pos_parser.cpp -c


execute:
	@./montador input.asm output.pre object.obj

clear:
	rm *.o
	rm montador
