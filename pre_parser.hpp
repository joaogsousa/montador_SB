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
    char* isInstruction(char* string);
    int isPseudo(char* string);
    char* isDiretiva(char* string);
    int isLabel(char* string);
    int numOperandos(char* instrucao);
    int isOperando(char* string);
    vector<char*> getBasicInstFromPseudo(vector<char*> pseudo);
    int stringCompareI(char * str1, char * str2);
    int letraCompare(char c, char d);
    int isNum(char c);
    int isChar(char c);
    void gerarPreProcessado(vector<char*> vetorTokens, char* nomeArquivo);
};


#endif
