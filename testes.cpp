#include "testes.hpp"

//testes do namespace pre_parser
void testes_pre_parser(vector<char*> vetor){
    char * instrucao = "add";
    char* instrucao_copy = "copy";
    char* label = "label1:";
    char* diretiva = "section";
    char* diretiva2 = "space";

    assert(pre_parser::isInstruction(instrucao));
    assert(!pre_parser::isInstruction("lol"));
    assert(pre_parser::isLabel(label));
    assert(!pre_parser::isLabel(diretiva));
    assert(pre_parser::numOperandos(instrucao_copy) == 2);
    assert(pre_parser::numOperandos(instrucao) == 1);
    assert(pre_parser::numOperandos(diretiva) == -1);

    assert(pre_parser::isDiretiva(vetor[0]));

}