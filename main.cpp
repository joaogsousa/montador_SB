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
vector< vector <string> > vetorTokensTratado;
int totErros = 0;


void limparLinhasVazias(){
    int i;

    for(i=0;i<vetorTokensTratado.size();i++){
        if(vetorTokensTratado[i][0] == "\n"){
            vetorTokensTratado.erase(vetorTokensTratado.begin() + i);
            i--;
        }

    }
}

void avaliarIf(){
    int i;
    int j;
    string stringIf = "IF";
    string zero = "0";

    for(i=0;i<vetorTokensTratado.size();i++){
        for(j=0;j<vetorTokensTratado[i].size();j++){
            if(pre_parser::stringCompareI(vetorTokensTratado[i][j],stringIf)){
                if(pre_parser::stringCompareI(vetorTokensTratado[i][j+1],zero)){
                    vetorTokensTratado.erase(vetorTokensTratado.begin()+i+1);
                }
                vetorTokensTratado.erase(vetorTokensTratado.begin()+i);
                i--;
            }
        }
    }

}


//substitue ocorrencia de string no vetor
void substituir(string busca,string subs){
    int i;
    int j;


    for(i=0;i<vetorTokensTratado.size();i++){
        for(j=0;j<vetorTokensTratado[i].size();j++){
            if(pre_parser::stringCompareI(vetorTokensTratado[i][j],busca)){
                vetorTokensTratado[i][j] = subs;
            }
        }
    }

}

//pegar o vetor com tudo e avalia o equ em outro vetor
void avaliarEqu(){
    int i;
    int j;
    string strEqu = "EQU";
    string stringBusca;
    string stringSubs;


    for(i=0;i<vetorTokensTratado.size();i++){
        for(j=0;j<vetorTokensTratado[i].size();j++){
            if(pre_parser::stringCompareI(vetorTokensTratado[i][j],strEqu)){
                stringBusca = vetorTokensTratado[i][j-1];
                stringSubs = vetorTokensTratado[i][j+1];

                vetorTokensTratado.erase(vetorTokensTratado.begin()+i);
                i--;

                stringBusca.erase(stringBusca.find(':'));

                substituir(stringBusca,stringSubs);

            }
        }
    }
}

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
                    cout << "Erro semantico! Dupla definicao de rotulos. Linha: " << linha << endl;
                    totErros++;
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
                cout << "Erro lexico! Operacao ou diretiva invalida. Linha: " << linha <<endl;
                totErros++;
                erro = 1;
            }
            vetorTokensInput.push_back(line);
        }
        else if(fpInput.eof()){
            vetorTokensInput.push_back(endline);

        }

        linha++;
    }

    fpInput.close();
    return erro;
}



int main(int argc, char* argv[]){
    if(argc != 4){
        cout << "Erro! Numero de argumentos diferente do esperado." << endl;
        exit(1);
    }

    primeiraPassagem(argv[1]);

    if(totErros) {
        cout << "\nPre-processamento finalizado com " << totErros << " erros!\n";
    }

    cout << "\n**Tabela de memoria:**\n";
    pre_parser::verificarMap(tabelaDeRotulos);

    cout << "\n**Vetor antes de ser tratado**\n";
    pre_parser::verificarVector(vetorTokensInput);


    //comecar tratamento
    vetorTokensTratado = vetorTokensInput;

    avaliarEqu();
    avaliarIf();

    cout << "**Vetor depois de avaliado equ**\n";
    pre_parser::verificarVector(vetorTokensTratado);

    limparLinhasVazias();

    pre_parser::gerarPreProcessado(vetorTokensTratado,argv[2]);

    testes_pre_parser();


    return 0;
}

