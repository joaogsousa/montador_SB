#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <cstring>
#include "pre_parser.cpp"
#include <cassert>


using namespace std;
using namespace pre_parser;

//remover os ifs e as linhas que dever ser tiradas
vector<char*> removerLinhas(vector<char*> vetor){
    int i;
    int numToRemove;
    int indice;

    for(i=0;i<vetor.size();i++){
        numToRemove = 0;
        if(pre_parser::stringCompareI(vetor[i],"IF")){
            if(pre_parser::stringCompareI(vetor[i+1],"0")){
                //remover a proxima linha
                if(pre_parser::isLabel(vetor[i+2])){
                    numToRemove++;
                    indice = i+3;
                }
                else{
                    indice = i+2;
                }
                numToRemove++; //representa o mnemonico da instrucao
                numToRemove = numToRemove + pre_parser::numOperandos(vetor[indice]);


                vetor.erase(vetor.begin()+i+2,vetor.begin()+i+2+numToRemove);
            }
            vetor.erase(vetor.begin()+i,vetor.begin()+i+2);
            i --;
        }

    }


    return vetor;

}


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


// Percorre e imprime o vetor (Testes)
void verificarVetor(vector<char*> vetor){
    unsigned i;
    for(i=0; i<vetor.size(); i++){
        cout << vetor[i] << endl;
    }

}

// Percorre o vetor e substitui as ocorrencias de vetor[i-1] pelo valor de vetor[i+1]
vector<char*> substituir(vector<char*> vetorTokens, int i){
    char* procurar = vetorTokens[i-1];
    char* novo = vetorTokens[i+1];
    unsigned j;
    int arrumado = 0;

    // Retirando ':' da string a ser substituida
    j = 0;
    while(!arrumado){
        if(procurar[j] == ':'){
            procurar[j] = '\0';
            arrumado = 1;
        }
        j++;
    }

    //Percorrendo o vetor e substituindo as ocorrencias
    for(j=0; j<vetorTokens.size(); j++){
        if(pre_parser::stringCompareI(vetorTokens[j], procurar)){
            vetorTokens[j] = novo;
        }
    }

    return vetorTokens;

}

char* readFile(char* filename) {
	char* content;
	long int size;
    FILE* file = fopen(filename, "r");
    if(file == NULL)
        return NULL;

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    content = (char*)malloc(size);

    fread(content, 1, size, file);

    return content;
}

// Cria vetor de tokens separados por espacos e \n
vector<char*> tokens(char* arquivo){
    char* stringTotal;
    vector<char*> vetorTokens;
    char* pch;

    stringTotal = readFile(arquivo);

    pch = strtok (stringTotal," ,'\n'");
    while (pch != NULL)
    {
        //printf ("%s\n",pch);
        vetorTokens.push_back(pch);
        pch = strtok (NULL, " ,'\n'");
    }

    return vetorTokens;

}

int main(int argc, char * argv[]){

    ifstream fpInput;
    ofstream fpPre;
    ofstream fpObj;

    vector<char*> vetorTokens;

    char buffer;
    unsigned i;

    if(argc != 4){
        cout << "Erro! Numero de argumentos diferente do esperado." << endl;
        exit(1);
    }

    // Arquivos que serao manipulados
    fpInput.open(argv[1]);  //.asm
    fpPre.open(argv[2]);    //.pre
    fpObj.open(argv[3]);    //.obj

    if(!fpInput.is_open()){
        cout << "Erro! Nao foi possivel ler o arquivo .asm de entrada." << endl;
        exit(1);
    }

    // Removendo comentarios
    while(!fpInput.eof()) {
        buffer = fpInput.get();
        if(buffer == ';') {
            while(buffer != '\n' && !fpInput.eof()){
                buffer = fpInput.get();
            }
        }
        if(buffer != EOF)
            fpPre << buffer ;
    }

    fpPre.close();

    /** Lidando com diretivas **/

    vetorTokens = tokens(argv[2]);
    // Encontra as variaveis inicializadas e as substitui nas instrucoes
    for(i=0; i<vetorTokens.size(); i++){
        if(pre_parser::stringCompareI(vetorTokens[i],"EQU")){
            vetorTokens = substituir(vetorTokens,i);
            vetorTokens.erase(vetorTokens.begin()+(i-1), vetorTokens.begin()+(i+2));
            i -= 2;
        }
    }


    vetorTokens = removerLinhas(vetorTokens);

    // Teste do preprocessamento
    verificarVetor(vetorTokens);

    testes_pre_parser(vetorTokens);

    fpInput.close();
    fpObj.close();

    return 0;
}

