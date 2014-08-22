all: programa


programa: main.o pre_parser.o testes.o
	g++ main.o pre_parser.o testes.o -o montador

main.o: main.cpp
	g++ main.cpp -Wno-write-strings -c

pre_parser.o: pre_parser.cpp pre_parser.hpp
	g++ pre_parser.cpp -Wno-write-strings -c

testes.o: testes.cpp testes.hpp
	g++ testes.cpp -Wno-write-strings -c

execute:
	@./montador input.asm output.pre object.obj

clear: 
	rm *.o
	rm montador
