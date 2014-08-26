#ifndef PRE_PARSER_HPP
#define PER_PARSER_HPP

#include <iostream>
#include <vector>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstdlib>
#include <map>


using namespace std;

namespace pre_parser{
    int isInstruction(string string1);
    int isPseudo(string string1);
    int isDiretiva(string string1);
    int isLabel(string string1);
    int numOperandos(string instrucao);
    int isOperando(string string1);
    vector<string> getBasicInstFromPseudo(vector<string> pseudo);
    int stringCompareI(string str1, string str2);
    int letraCompare(char c, char d);
    int isNum(char c);
    int isChar(char c);
    void verificarMap(map<string,int> mapa);
    void verificarMapString(map<string,string> mapa);
    void verificarVector(vector<vector<string> > vetorStrings);
    void gerarPreProcessado(vector<vector<string> > vetorStrings,char* arquivoPre);
};


#endif
