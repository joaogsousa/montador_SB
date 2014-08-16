#ifndef PRE_PARSER_HPP
#define PER_PARSER_HPP



#include <iostream>
#include <vector>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstdlib>
#include <cstring>

using namespace std;

namespace pre_parser{
    static char* isInstruction(char* string);
    static int isPseudo(char* string);
    static char* isDiretiva(char* string);
    static int isLabel(char* string);
    static int numOperandos(char* instrucao);
    static int isOperando(char* string);
    static vector<char*> getBasicInstFromPseudo(vector<char*> pseudo);
    static int stringCompareI(char * str1, char * str2);
    static int letraCompare(char c, char d);
    static int isNum(char c);
    static int isChar(char c);


};


#endif
