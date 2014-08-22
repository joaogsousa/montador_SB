#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <string>
#include "testes.hpp"
#include "pre_parser.hpp"


using namespace std;
using namespace pre_parser;



// Percorre e imprime o vetor (Testes)
void verificarVetor(vector<string> vetor){
    unsigned i;
    for(i=0; i<vetor.size(); i++){
        cout << vetor[i] << endl;
    }

}



string readFile(string filename) {
	string content;
	long int size;
    FILE* file = fopen(filename, "r");
    if(file == NULL)
        return NULL;

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    content = (string)malloc(size);

    fread(content, 1, size, file);

    return content;
}


int main(int argc, string argv[]){

    ifstream fpInput;
    ofstream fpPre;
    ofstream fpObj;


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





    fpInput.close();
    fpObj.close();

    return 0;
}

