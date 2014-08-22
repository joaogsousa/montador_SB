#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <string>
#include "testes.hpp"
#include "pre_parser.hpp"
#include <cstring>
#include <map>

using namespace std;
using namespace pre_parser;

map<string, int> tabelaDeRotulos;
vector< vector <string> > vetorTokensInput;

// Cria vetor de tokens separados por espacos e \n
vector<string> tokens(string str){
    vector<string> vetorTokens;
    char* pch;
    char * aux = new char[str.size() + 1];
    string pch_string;

    copy(str.begin(), str.end(), aux);
    aux[str.size()] = '\0';

    pch = strtok (aux," ,");
    pch_string = pch;


    while(pch != NULL) {
        vetorTokens.push_back(pch_string);
        pch = strtok (NULL, " ,");
        if(pch != NULL) {
            pch_string = pch;
        }
    }

    return vetorTokens;

}

vector<string> ignoraComentarios(vector<string> line) {
    unsigned int i = 0;
    int achou = 0;
    string aux;

    while(!achou && i < line.size()){
        aux = line[i];
        if(aux[0] == ';') {
            achou = 1;
            line.erase(line.begin() + i, line.begin() + i + (line.size() - i));
        }
        i++;
    }
    return line;
}

int primeiraPassagem(char* input){
    ifstream fpInput;
    string buffer;
    int endereco;
    int linha;
    vector<string> lineToTokens;
    vector<string> line;
    int indice;
    int erro = 0;
    vector<string> endline;

    endline.push_back("\n");



    linha = 0;
    endereco = 0;

    fpInput.open(input);


    if(!fpInput.is_open()){
        cout << "Arquivo de input não existe" << endl;
        exit(1);

    }

    while(!fpInput.eof()){
        getline(fpInput, buffer);
        if(!buffer.empty()) {
            lineToTokens = tokens(buffer);
            line = ignoraComentarios(lineToTokens);

            if(isLabel(line[0])) {
                if(tabelaDeRotulos.find(line[0]) != tabelaDeRotulos.end()){
                    //erro!
                    cout << "Erro!\n" << "Erro semantico: Dupla definicao de rótulos\n Linha: " << linha << endl;
                    erro = 1;
                }
                else {
                    tabelaDeRotulos.insert(std::pair<string,int>(line[0],endereco));
                }
                indice = 1;
            }
            else{
                indice = 0;
            }


            if(pre_parser::isInstruction(line[indice])){
                endereco = endereco + 1 + pre_parser::numOperandos(line[indice]);

            }
            else if(isDiretiva(line[indice])){
                //eh uma diretiva
                //chamar subrotina
            }
            else {
                //erro
                cout << "Operação não identificada, erro léxico. Linha: " << linha <<endl;
                erro = 1;
            }
            vetorTokensInput.push_back(line);
        }
        else{
            vetorTokensInput.push_back(endline);

        }

        linha++;
    }

    fpInput.close();
    return erro;
}



int main(int argc, char* argv[]){

    ofstream fpPre;
    ofstream fpObj;

    if(argc != 4){
        cout << "Erro! Numero de argumentos diferente do esperado." << endl;
        exit(1);
    }

    // Arquivos que serao manipulados
    fpPre.open(argv[2]);    //.pre
    fpObj.open(argv[3]);    //.obj

    primeiraPassagem(argv[1]);

//    string dani = "dani linda lol ;jlkaf jlfkja jlk";
//    vector<string> vec = tokens(dani);
//    vec = ignoraComentarios(vec);
//
//    for(unsigned int i = 0; i<vec.size(); i++) {
//        cout << vec[i] << endl;
//    }


    pre_parser::verificarMap(tabelaDeRotulos);

    //pre_parser::verificarVector(vetorTokensInput);

    fpObj.close();

    return 0;
}

