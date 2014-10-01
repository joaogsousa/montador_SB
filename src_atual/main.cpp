#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <string>
#include <string.h>
#include <cstring>
#include <map>
#include <list>
#include "pre_parser.hpp"
#include <cassert>


#define tipo_map_macro std::map<const std::basic_string<char>, std::vector<std::vector<std::basic_string<char> > > >::iterator
#define tipo_end_macro std::map<const std::basic_string<char>, std::vector<std::vector<std::basic_string<char> > > >::end



using namespace std;
using namespace pre_parser;

typedef struct rotulo
{
    int defined;
    int value;
    list<int> use;
}rotulo;

map<string, rotulo> tabelaDeRotulos;
vector< vector <string> > vetorTokensInput;
vector< vector <string> > vetorTokensTratado;
int totErros = 0;
map<string,string> tabelaDefines;
map<string,vector <vector <string> > > tabelaDeMacros;


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

int gerarTokens(char* input){
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


    vetorTokensInput.clear();

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



int preprocessamento(char* input, char* output){
    ifstream fpInput;
    ofstream fpOutput;
    int erro=0;
    const char * pontoASM = ".asm";
    const char * pontoPre = ".pre";
    char * inExtension;
    char * outExtension;

    inExtension = addFormato(input, pontoASM);
    outExtension = addFormato(output,pontoPre);

    gerarTabelaDefines(inExtension);
    gerarTokens(inExtension);
    vetorTokensTratado = vetorTokensInput;
    avaliarEqu();
    avaliarIf();
    pre_parser::gerarPreProcessado(vetorTokensTratado,outExtension);

    return erro;
}

int macro(char* input, char* output){
    ifstream fpInput;
    ofstream fpOutput;
    int erro = 0;
    int i;
    int j;
    int isOnText;
    const char * pontoPre = ".pre";
    const char * pontoM = ".mcr";
    char * inExtension;
    char * outExtension;

    inExtension = addFormato(input,pontoPre);
    outExtension = addFormato(output,pontoM);

    string sec("section");
    string tex("text");
    string dat("data");
    string strEnd("end");
    string macroDef("macro");



    vetorTokensTratado.clear();

    string strDeDef;

    vector<vector <string> > vetorNulo;
    vetorNulo.clear();

    gerarTokens(inExtension);

    isOnText = 0;

    for(i=0;i<vetorTokensInput.size();i++){
        if(stringCompareI(vetorTokensInput[i][0],sec) && stringCompareI(vetorTokensInput[i][1],tex)){
            isOnText = 1;
        }
        if(stringCompareI(vetorTokensInput[i][0],sec) && stringCompareI(vetorTokensInput[i][1],dat)){
            isOnText = 0;
        }

        if(isLabel(vetorTokensInput[i][0]) && stringCompareI(vetorTokensInput[i][1],macroDef)){
            //inicio de definicao de macro
            if(isOnText == 0){
                //erro, def de macro fora do text
                cout << "Erro semantico. Linha: " << i+1 << "Definicao de macro fora da secao texto." << endl;
                erro++;
            }
            if(vetorTokensInput[i].size() != 2){
                cout << "Erro sintatico. Linha: " << i+1 << "Erro na definicao de macro." << endl;
                erro++;

            }

            strDeDef.clear();
            strDeDef = apagaDoisPontos(vetorTokensInput[i][0]);
            vetorNulo.clear();

            i++;
            while(!stringCompareI(vetorTokensInput[i][0],strEnd)){
                vetorNulo.push_back(vetorTokensInput[i]);
                i++;
            }

            tabelaDeMacros[strDeDef] = vetorNulo;


        }else{
            //caso nao seja definicao de macro
            //se houver esta macro
            //typename set< map<string,vector<vector <string> > > >::iterator iterador;
            tipo_map_macro iterador;
            if((iterador = tabelaDeMacros.find(vetorTokensInput[i][0])) != tabelaDeMacros.end()){
                for(j = 0;j<(iterador->second).size();j++){
                    vetorTokensTratado.push_back((iterador->second).at(j));

                }

            }else{
                if(vetorTokensInput[i].size()>=1){
                    vetorTokensTratado.push_back(vetorTokensInput[i]);
                }

            }
        }
    }

    pre_parser::gerarPreProcessado(vetorTokensTratado, outExtension);
    output = (char *) malloc(strlen(outExtension) + 1);
    strcpy(output,outExtension);



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
    string strParaArquivoTotal;
    int endMod;
    const char * pontoM = ".mcr";
    const char * pontoO = ".o";
    char * inExtension;
    char * outExtension;

    linha = 1;
    endereco = 0;
    
    inExtension = addFormato(input, pontoM);
    outExtension = addFormato(output, pontoO);

    fpInput.open(inExtension);
    fpOutput.open(outExtension);


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
                strParaArquivo += " ";

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
                            strParaArquivo += to_string(0);
                            strParaArquivo += " ";
                        }
                        else{
                            for(map<string,rotulo>::iterator it=tabelaDeRotulos.begin(); it!=tabelaDeRotulos.end(); it++){
                                if(it->first == operando){
                                    if(it->second.defined){
                                        //mete no codigo
                                        strParaArquivo += to_string(it->second.value);
                                        strParaArquivo += " ";

                                    }
                                    else{
                                        it->second.use.push_back(endereco+i+1);
                                        strParaArquivo += to_string(0);
                                        strParaArquivo += " ";
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
                                strParaArquivo += to_string(0);
                                strParaArquivo += " ";
                            }
                        }
                    }
                }
                endereco = endereco + 1 + numOp;
            }
            else{
                // caso nao seja instrucao, ainda pode ser const e space
                string strSpace("space");
                string strConst("const");
                string strTxt("text");
                string strDat("data");
                string strSec("section");

                if(stringCompareI(line[indice],strSpace)){
                    if(line.size() == 2){
                        strParaArquivo += to_string(0);
                        strParaArquivo += " ";
                        endereco++;

                    }else{
                        int k;
                        for(k=0;k<stoi(line[indice+1]);k++){
                            strParaArquivo += to_string(0);
                            strParaArquivo += " ";
                            endereco++;

                        }

                    }

                }else if(stringCompareI(line[indice],strConst)){
                    strParaArquivo += line[indice+1];
                    strParaArquivo += " ";
                    endereco++;


                }else if(stringCompareI(line[indice],strSec) && stringCompareI(line[indice + 1],strTxt)){
                    //esta na secao texto

                }else if(stringCompareI(line[indice],strSec) && stringCompareI(line[indice + 1],strDat)){
                    //esta na secao data


                }
                else{
                    cout << "Erro lexico! Linha: " << linha << endl;
                }
            }
        }
        linha++;
        //fpOutput << strParaArquivo;
        strParaArquivoTotal += strParaArquivo;
    }

    //varredura das listas para atualizar enderecos
    for(map<string,rotulo>::iterator it=tabelaDeRotulos.begin(); it!=tabelaDeRotulos.end(); it++){
        if(it->second.defined == 0){
            cout << "Erro semantico. Simbolo nao definido." << endl;
            erro++;

        }else{
            while(!it->second.use.empty()){
                //caution!!! código denso!
                endMod = it->second.use.front();
                endMod = indiceComEndereco(endMod,strParaArquivoTotal);
                strParaArquivoTotal.erase(endMod,1);
                string insercao("");
                insercao = to_string(it->second.value);
                strParaArquivoTotal.insert(endMod,insercao);
                cout << "valor inserido: " << insercao << endl;
                it->second.use.pop_front();

            }



        }



    }

    fpOutput << strParaArquivoTotal;

    //cout << "string do arquivo total: " << endl;
    //cout << strParaArquivoTotal << endl;

    fpInput.close();
    fpOutput.close();
    return erro;
}


int main(int argc, char* argv[]){
    char * outPre = new char[14];
    char * outMacro = new char[16];
    char * input;
    char * output;
    strcpy(outPre, "outputs/outPre\0");
    strcpy(outMacro, "outputs/outMacro\0");
    string strIn(argv[2]);
    string strOut(argv[3]);

    strIn += ".asm";


    if(argc != 4){
        cout << "Erro! Numero de argumentos diferente do esperado." << endl;
        exit(1);
    }

    //passagemUnica(argv[2], argv[3]);

    input = argv[2];
    output = argv[3];
    
     if(!strcmp(argv[1], "-p")){
         preprocessamento(input,output);
         strOut += ".pre";
     }
     else if(!strcmp(argv[1], "-m")){
         preprocessamento(input,outPre);
         macro(outPre,output); 
         strOut += ".mcr";
     }
     else if(!strcmp(argv[1], "-o")){
         preprocessamento(input, outPre);
         macro(outPre, outMacro);
         passagemUnica(outMacro, output);
         strOut += ".o";
     }
     else {
         // Usuario nao informou corretamente o tipo de operacao >> encerrar programa!
         cout << "Erro! Tipo de operacao nao reconhecido." << endl;
         return 1;
     }

    //*************************************************
    //debug
    //************************************************
    //verificarVector(vetorTokensTratado);
    //verificarVector(vetorTokensInput);
    //getchar();

    //verificarMapStringToVector(tabelaDeMacros);
    //cout << "tabela macros vazia? "  <<tabelaDeMacros.empty() << endl;


    string strTeste("88 99 9 9 1 4 333 4 5 234 434234 8");

    cout << "indices" << endl;
    assert(indiceComEndereco(4,strTeste) == 10);
    cout << indiceComEndereco(4,strTeste) << endl;


    return 0;
}

