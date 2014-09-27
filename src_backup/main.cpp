#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <list>
#include "testes.hpp"
#include "pre_parser.hpp"

using namespace std;
using namespace pre_parser;

typedef struct rotulo
{
    int defined;
    int value;
    list<int> use;
}rotulo;

map<string, int> tabelaDeRotulos1;
map<string, rotulo> tabelaDeRotulos;
vector< vector <string> > vetorTokensInput;
vector< vector <string> > vetorTokensTratado;
int totErros = 0;
map<string,string> tabelaDefines;


//Apagar dois pontos dos rotulos
string apagaDoisPontos(string str){
    string a = str;
    return a.erase(a.find(':'));

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

void gerarTabelaDefines(char* input){
    ifstream fpInput;
    string buffer;
    vector<string> lineToTokens;
    vector<string> line;
    int i;
    string chave;
    string valor;

    fpInput.open(input);

    tabelaDefines.erase(tabelaDefines.begin(),tabelaDefines.end());


    if(!fpInput.is_open()){
        cout << "Arquivo de input não existe" << endl;
        exit(1);

    }

    while(!fpInput.eof()){
        getline(fpInput, buffer);
        if(!buffer.empty()) {
            lineToTokens = tokens(buffer);
            line = ignoraComentarios(lineToTokens);

            for(i=0;i<line.size();i++){
                if(stringCompareI(line[i],"EQU")){
                    chave = line[i-1].erase(line[i-1].find(':'));
                    valor = line[i+1];
                    tabelaDefines.insert(pair<string,string>(chave,valor));
                }

            }

        }


    }

    fpInput.close();



}

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

int preprocessamento(char* input, char* output){
    ifstream fpInput;
    ofstream fpOutput;
    int erro=0;

    gerarTabelaDefines(input);

    return erro;
}

int macro(char* input, char* output){
    ifstream fpInput;
    ofstream fpOutput;
    int erro = 0;

    return erro;
}

int passagemUnica(char* input, char* output){
    ifstream fpInput;
    ofstream fpOutput;
    string buffer;
    string labelTratado;
    string operando;
    int i;
    int endereco;
    int linha;
    vector<string> lineToTokens;
    vector<string> line;
    int indice;
    int erro = 0;
    int find;
    vector<string> endline;
    vector<string> linePassada;
    int tamanhoVetor;
    int numOp;
    int opCode;
    rotulo symbol;
    list<int> listaux;
    endline.push_back("\n");
    string strParaArquivo;

    linha = 1;
    endereco = 0;

    fpInput.open(input);
    fpOutput.open(output);


    if(!fpInput.is_open()){
        cout << "Arquivo de input não existe" << endl;
        exit(1);

    }

    while(!fpInput.eof()){
        strParaArquivo = "";
        getline(fpInput, buffer);
        find = 0;
        if(!buffer.empty()){
            lineToTokens = tokens(buffer);
            line = ignoraComentarios(lineToTokens);

            // Verifica se eh definicao de rotulo
            if(isLabel(line[0])){
                labelTratado = apagaDoisPontos(line[0]);
                if(tabelaDeRotulos.empty()){
                    symbol.value = endereco;
                    symbol.defined = 1;
                    tabelaDeRotulos.insert(pair<string,rotulo>(labelTratado,symbol));
                }
                else{
                    for(map<string,rotulo>::iterator it=tabelaDeRotulos.begin(); it!=tabelaDeRotulos.end(); it++){
                        if(it->first == labelTratado){
                            symbol = it->second;
                            if(symbol.defined == 1) {
                                //erro, rotulo redefinido
                                cout << "Erro semantico! Linha: " << linha << endl;
                                totErros++;
                                erro = 1;
                            }
                            else{
                                it->second.defined = 1;
                                it->second.value = endereco;
                            }
                            find = 1;
                            break;
                        }
                    }
                    if(!find){
                        // Insere novo rotulo na tabela de simbolos
                        symbol.value = endereco;
                        symbol.defined = 1;
                        symbol.use = listaux;
                        tabelaDeRotulos.insert(pair<string,rotulo>(labelTratado,symbol));
                    }
                }
                indice = 1;
            }
            else{
                indice = 0;
            }

            // Verifica se eh uma operacao
            opCode = pre_parser::isInstruction(line[indice]); //Verifica se eh instrucao e qual
            if(opCode != 0){
                numOp = pre_parser::numOperandosByOpCode(opCode);
                strParaArquivo += to_string(opCode);

                if(line.size() != numOp + indice + 1){
                    // Erro! Numero de operandos invalido!
                    cout << "Erro sintatico! Linha: " << linha << endl;
                }
                else if(numOp > 0){
                    //TODO: tratar os simbolos nao definidos
                    find = 0;
                    for(i=0; i<numOp; i++){
                        operando = line[indice+i+1];
                        if(tabelaDeRotulos.empty()){
                            symbol.defined = 0;
                            symbol.value = 0;
                            symbol.use = listaux;
                            symbol.use.push_back(endereco+i+1);
                            tabelaDeRotulos.insert(pair<string,rotulo>(operando,symbol));
                            strParaArquivo += " ";
                            strParaArquivo += to_string(0);
                        }
                        else{
                            for(map<string,rotulo>::iterator it=tabelaDeRotulos.begin(); it!=tabelaDeRotulos.end(); it++){
                                if(it->first == operando){
                                    if(it->second.defined){
                                        //mete no codigo
                                        strParaArquivo += " ";
                                        strParaArquivo += to_string(it->second.value);

                                    }
                                    else{
                                        it->second.use.push_back(endereco+i+1);
                                        strParaArquivo += " ";
                                        strParaArquivo += to_string(0);
                                    }
                                    find = 1;
                                    break;
                                }
                            }
                            if(!find){
                                symbol.defined = 0;
                                symbol.value = 0;
                                symbol.use = listaux;
                                symbol.use.push_back(endereco+i+1);
                                tabelaDeRotulos.insert(pair<string,rotulo>(operando,symbol));
                                strParaArquivo += " ";
                                strParaArquivo += to_string(0);
                            }
                        }
                    }
                }
                endereco = endereco + 1 + numOp;
            }
            else{
                cout << "Erro lexico! Linha: " << linha << endl;
            }
        }
        linha++;
        strParaArquivo += " ";
        fpOutput << strParaArquivo;
    }

    fpInput.close();
    fpOutput.close();
    return erro;
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
    vector<string> linePassada;
    int tamanhoVetor;

    endline.push_back("\n");



    linha = 1;
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

            if(isLabel(line[0]) && !stringCompareI(line[1],"EQU")) {
                if(tabelaDeRotulos1.find(line[0]) != tabelaDeRotulos1.end()){
                    //erro!
                    cout << "Erro semantico! Dupla definicao de rotulos. Linha: " << linha << endl;
                    totErros++;
                    erro = 1;
                }
                else {
                    tabelaDeRotulos1.insert(std::pair<string,int>(line[0],endereco));
                }
                indice = 1;
            }
            else{
                indice = 0;
            }


            if(pre_parser::isInstruction(line[indice])){
                if(!(stringCompareI(linePassada[0],"IF") && stringCompareI(linePassada[1],"0"))
                   &&  !(stringCompareI(linePassada[0],"IF") && stringCompareI(tabelaDefines[linePassada[1]],"0"))  ){
                    endereco = endereco + 1 + pre_parser::numOperandos(line[indice]);
                }

            }
            else if(isDiretiva(line[indice])){
                if(stringCompareI(line[indice],"CONST")){
                    endereco = endereco + 1;

                }
                else if(stringCompareI(line[indice],"SPACE")){
                    if(line.size() == 3){
                        tamanhoVetor = atoi(line[indice+1].c_str());
                        endereco = endereco + tamanhoVetor;
                    }else{
                        endereco = endereco + 1;
                    }


                }
            }
            else {
                if(!isLabel(line[indice])){
                    //erro
                    cout << "Erro lexico! Operacao ou diretiva invalida. Linha: " << linha <<endl;
                    totErros++;
                    erro = 1;
                }
            }
            vetorTokensInput.push_back(line);
        }
        else if(fpInput.eof()){
            vetorTokensInput.push_back(endline);
        }

        linha++;
        linePassada = line;
    }

    fpInput.close();
    return erro;
}


int main(int argc, char* argv[]){
    char * outPre = new char[7];
    char * outMacro = new char[9];
    strcpy(outPre, "outPre\0");
    strcpy(outMacro, "outMacro\0");

    if(argc != 4){
        cout << "Erro! Numero de argumentos diferente do esperado." << endl;
        exit(1);
    }

    passagemUnica(argv[2], argv[3]);

    // if(!strcmp(argv[1], "-p")){
    //     preprocessamento(argv[2], argv[3]);
    // }
    // else if(!strcmp(argv[1], "-m")){
    //     preprocessamento(argv[2],outPre);
    //     macro(outPre, argv[3]);
    // }
    // else if(!strcmp(argv[1], "-o")){
    //     preprocessamento(argv[2], outPre);
    //     macro(outPre, outMacro);
    //     passagemUnica(outMacro, argv[3]);
    // }
    // else {
    //     // Usuario nao informou corretamente o tipo de operacao >> encerrar programa!
    //     cout << "Erro! Tipo de operacao nao reconhecido." << endl;
    //     return 1;
    // }


    // if(totErros) {
    //     cout << "\nPre-processamento finalizado com " << totErros << " erros!\n";
    // }

    // cout << "\n**Tabela de memoria:**\n";
    // pre_parser::verificarMap(tabelaDeRotulos1);

    // cout << "\n**Tabela de defines:**\n";
    // pre_parser::verificarMapString(tabelaDefines);

    // cout << "\n**Vetor antes de ser tratado**\n";
    // pre_parser::verificarVector(vetorTokensInput);


    // //comecar tratamento
    // vetorTokensTratado = vetorTokensInput;

    // avaliarEqu();
    // avaliarIf();

    // cout << "**Vetor depois de avaliado equ**\n";
    // pre_parser::verificarVector(vetorTokensTratado);

    // limparLinhasVazias();

    // pre_parser::gerarPreProcessado(vetorTokensTratado,argv[2]);

    // testes_pre_parser();


    return 0;
}
