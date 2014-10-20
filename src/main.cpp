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
#include "../header/pre_parser.hpp"
#include <cassert>

#define tipo_map_macro std::map<const std::basic_string<char>, std::vector<std::vector<std::basic_string<char> > > >::iterator

using namespace std;
using namespace pre_parser;

typedef struct rotulo
{
	int defined;
	int value;
	list<int> use;
	bool isLabel;
	bool isVar;
	bool isConst;
	int spaceSIZE = 1;
}rotulo;

typedef struct argumento
{
	string arg;
	int linha;
}argumento;

map<string, rotulo> tabelaDeRotulos;
vector< vector <string> > vetorTokensInput;
vector< vector <string> > vetorTokensTratado;
int erro = 0;
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

	pch = strtok (aux," ");
	pch_string = pch;


	while(pch != NULL) {
		vetorTokens.push_back(pch_string);
		pch = strtok (NULL, " ");
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
		cout << "Arquivo de input nao existe" << endl;
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
	vector<string> endline;
	vector<string> linePassada;
	int tamanhoVetor;


	vetorTokensInput.clear();

	endline.push_back("\n");



	linha = 1;
	endereco = 0;

	fpInput.open(input);


	if(!fpInput.is_open()){
		cout << "Arquivo de input nao existe" << endl;
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
	int find;
	vector<string> endline;
	vector<string> linePassada;
	int tamanhoVetor;
	int totOpSemAdd;
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
	int numText = 0;
	int numData = 0;
	int isOnText = 0;
	int isOnData = 0;
	list<argumento> argDiv;
	list<argumento> argJmp;
	list<argumento> argMod;
	map<string,int> constParaValor;
	int maisFatorDeCorrecao;
	string insercao;
	int auxValue = 0;
	vector<int> vetParaArquivo;
	vector<int> usouMais;
	int dataInicial, dataFinal = 0;
	int teveMais;

	//pendencias: div por zero, modificar constante, pulo para secao data
	//instrucoes que modificam: copy:9  ;   store:11    ;   input:12
	//div: 4
	//instrucoes de pulo:   jmp:5         jmpn:6    jmpp:7    jmpz:8


	linha = 1;
	endereco = 0;

	inExtension = addFormato(input, pontoM);
	outExtension = addFormato(output, pontoO);

	fpInput.open(inExtension);


	if(!fpInput.is_open()){
		cout << "Arquivo de input nao existe" << endl;
		exit(1);

	}

	while(!fpInput.eof()){
		strParaArquivo = "";
		getline(fpInput, buffer);
		find = 0;
		teveMais = 0;
		maisFatorDeCorrecao = 0;
		if(!buffer.empty()){
			lineToTokens = tokens(buffer);
			line = ignoraComentarios(lineToTokens);

			int indiceAux;
			int numLabels = 0;
			for(indiceAux = 0;indiceAux < line.size();indiceAux++){
				if(pre_parser::isLabel(line[indiceAux])){
					numLabels++;
				}

			}
			// ver se tem mais que um e dar erro
			if(numLabels > 1){
				erro++;
				cout << "Erro semantico! Linha: " << linha << ". Mais de um rótulo em uma linha." << endl;

			}


			// Verifica se eh definicao de rotulo
			if(isLabel(line[0])){
				labelTratado = apagaDoisPontos(line[0]);
				if(tabelaDeRotulos.empty()){
					symbol.value = endereco;
					symbol.defined = 1;
					//setar as variaveis de verificaçao de onde esta o label
					if(isOnText){
						symbol.isLabel = 1;
						symbol.isVar = 0;
					}else{
						symbol.isLabel = 0;
						symbol.isVar = 1;
					}
					if(stringCompareI(line[1],"const")){
						symbol.isConst = 1;
					}else{
						symbol.isConst = 0;
						if(line.size() > 2){
							symbol.spaceSIZE = stoi(line[2]);
						}
						else{
							symbol.spaceSIZE = 1;
						}
					}
					tabelaDeRotulos.insert(pair<string,rotulo>(labelTratado,symbol));
				}
				else{
					for(map<string,rotulo>::iterator it=tabelaDeRotulos.begin(); it!=tabelaDeRotulos.end(); it++){
						if(it->first == labelTratado){
							symbol = it->second;
							if(symbol.defined == 1) {
								//erro, rotulo redefinido
								cout << "Erro semantico! Linha: " << linha << ". Dupla definicao de rotulos." << endl;
								erro++;
							}
							else{
								it->second.defined = 1;
								it->second.value = endereco;
								//setar as variaveis de verificaçao de onde esta o label
								if(isOnText){
									it->second.isLabel = 1;
									it->second.isVar = 0;
								}else{
									it->second.isLabel = 0;
									it->second.isVar = 1;
								}
								if(stringCompareI(line[1],"const")){
									it->second.isConst = 1;
								}else{
									it->second.isConst = 0;
									if(line.size() > 2 && stringCompareI(line[1], "space")){
										it->second.spaceSIZE = stoi(line[2]);
									}
									else{
										it->second.spaceSIZE = 1;
									}
								}


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

						//setar as variaveis de verificaçao de onde esta o label
						if(isOnText){
							symbol.isLabel = 1;
							symbol.isVar = 0;
						}else{
							symbol.isLabel = 0;
							symbol.isVar = 1;
						}
						if(stringCompareI(line[1],"const")){
							symbol.isConst = 1;
						}else{
							symbol.isConst = 0;
							if(line.size() > 2 && isOnData){
								symbol.spaceSIZE = stoi(line[2]);
							}
							else{
								symbol.spaceSIZE = 1;
							}
						}

						tabelaDeRotulos.insert(pair<string,rotulo>(labelTratado,symbol));
					}
				}
				indice = 1;
			}
			else{
				indice = 0;
			}



			//pendencias: div por zero, modificar constante, pulo para secao data
			//instrucoes que modificam: copy:9  ;   store:11    ;   input:12
			//div: 4
			//instrucoes de pulo:   jmp:5         jmpn:6    jmpp:7    jmpz:8

			// Verifica se eh uma operacao
			opCode = pre_parser::isInstruction(line[indice]); //Verifica se eh instrucao e qual
			if(opCode != 0){
				//colocar os argumentos criticos nas listas
				argumento aux;
				if(opCode == 4){
					//div
					aux.arg = line[indice+1];
					aux.linha = linha;
					argDiv.push_back(aux);

				}
				if(opCode == 9 || opCode == 11 || opCode == 12){
					//modificadores
					if(opCode != 9){
						aux.arg = line[indice+1];
						aux.linha = linha;
						argMod.push_back(aux);
					}
					//copy modifica o segundo operando
					else{
						aux.arg = line[indice + 2];
						aux.linha = linha;
						argMod.push_back(aux);
					}

				}
				if(opCode > 4 && opCode < 9){
					//instrucoes de jump
					aux.arg = line[indice+1];
					aux.linha = linha;
					argJmp.push_back(aux);


				}



				//significa que é instruçao
				//verificar se esta na secao correta
				if(!isOnText){
					erro++;
					cout << "Erro semantico! Linha: " << linha << ". Instruçao fora da seçao TEXT." << endl;

				}

				//Tratamento de operandos
				numOp = pre_parser::numOperandosByOpCode(opCode);
				strParaArquivo += to_string(opCode);
				strParaArquivo += " ";
				vetParaArquivo.push_back(opCode);

				totOpSemAdd = totOperandos(line);
				//cout << totOpSemAdd << " Teste + " << line.size() << endl;
				// Verifica se o numero de operandos da instrucao eh valido (ignora os "+" e inteiros)
				if(totOpSemAdd != numOp + indice + 1){
					// Erro! Numero de operandos invalido!
					erro++;
					cout << "Erro sintatico! Linha: " << linha << ". Número de operandos invalido." << endl;
				}
				else if(numOp > 0){
					// Procurar os simbolos nas tabelas
					find = 0;
					for(i=0; i<numOp; i++){
						operando = line[teveMais + indice + i + 1];
						// Verificacao do copy
						if(numOp > 1){
							//Verificar se o primeiro operando tem virgula
							if(i == 0 && ((!possuiVirgula(operando)) && (!possuiVirgula(line[indice + 3])))){
								cout << "Erro sintatico! Linha: " << linha << ". Operandos nao separados por virgula." << endl;
								erro++;
							}
							else if(i==0){
								//Retirar virgula do operando
								if(possuiVirgula(operando)){
									operando = retiraVirgula(operando);
								}
							}
						}
						//Verificar se necessita levar vetor em consideracao
						if(line.size() != totOpSemAdd){
							usouMais.push_back(endereco+i+1);
							//Verifica se esta somando um digito
							if(isNum(line[teveMais + indice+i+3][0])){
								maisFatorDeCorrecao = stoi(line[teveMais + indice + i + 3]);
								//Verifica se eh um operador de vetor valido
								if(line[teveMais + indice+i+2] == "-"){
									maisFatorDeCorrecao *= -1;
								}
								else if(line[teveMais + indice+i+2] != "+"){
									//Nao usando "+" ou "-" para fazer operacao
									cout << "Erro lexico! Linha: " << linha << ". Operador invalido." << endl;
									erro++;
								}
								if(numOp > 1 && i==0){
									teveMais = 2;
								}
							}
						}
						else{
							maisFatorDeCorrecao = 0;
						}
						if(tabelaDeRotulos.empty()){
							symbol.defined = 0;
							//editar para caso com vetor
							symbol.value = 0;
							symbol.use = listaux;
							symbol.use.push_back(endereco+i+1);
							tabelaDeRotulos.insert(pair<string,rotulo>(operando,symbol));
							strParaArquivo += to_string(maisFatorDeCorrecao);
							strParaArquivo += " ";
							vetParaArquivo.push_back(maisFatorDeCorrecao);
						}
						else{
							for(map<string,rotulo>::iterator it=tabelaDeRotulos.begin(); it!=tabelaDeRotulos.end(); it++){
								if(it->first == operando){
									if(it->second.defined){
										//mete no codigo
										if(maisFatorDeCorrecao > it->second.spaceSIZE - 1 && it->second.isConst == false){
											erro++;
											cout << "Erro semantico! Linha: " << linha << ". Tentando acessar memoria nao reservada." << endl;
										}
										else{
											strParaArquivo += to_string(it->second.value + maisFatorDeCorrecao);
											strParaArquivo += " ";
											vetParaArquivo.push_back(it->second.value + maisFatorDeCorrecao);
										}
									}
									else{
										it->second.use.push_back(endereco+i+1);
										strParaArquivo += to_string(maisFatorDeCorrecao);
										strParaArquivo += " ";
										vetParaArquivo.push_back(maisFatorDeCorrecao);
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
								strParaArquivo += to_string(maisFatorDeCorrecao);
								strParaArquivo += " ";
								vetParaArquivo.push_back(maisFatorDeCorrecao);
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
						vetParaArquivo.push_back(0);
						endereco++;

					}else{
						int k;
						for(k=0;k<stoi(line[indice+1]);k++){
							strParaArquivo += to_string(0);
							strParaArquivo += " ";
							vetParaArquivo.push_back(0);
							endereco++;

						}

					}

					//incrementar erros caso estiver na secao texto
					if(!isOnData){
						erro++;
						cout << "Erro semantico! Linha: " << linha  << ". Declaraçao de variavel fora da secao DATA" << endl;
					}


				}else if(stringCompareI(line[indice],strConst)){
					strParaArquivo += line[indice+1];
					strParaArquivo += " ";
					vetParaArquivo.push_back(stoi(line[indice+1]));
					endereco++;

					//botar o valor de const em algum lugar para ver se é zero
					constParaValor[apagaDoisPontos(line[indice - 1])] = stoi(line[indice + 1]);


					//incrementar erros caso estiver na secao texto
					if(!isOnData){
						erro++;
						cout << "Erro semantico! Linha: " << linha  << ". Declaraçao de variavel fora da secao DATA" <<  endl;
					}


				}else if(stringCompareI(line[indice],strSec) && stringCompareI(line[indice + 1],strTxt)){
					//esta na secao texto
					if(isOnData){
						dataFinal = endereco;
					}
					isOnText = 1;
					isOnData = 0;
					numText++;


				}else if(stringCompareI(line[indice],strSec) && stringCompareI(line[indice + 1],strDat)){
					//esta na secao data
					isOnText = 0;
					isOnData = 1;
					numData++;
					dataInicial = endereco;

				}else if(stringCompareI(line[indice],strSec) && !stringCompareI(line[indice + 1],strDat) && !stringCompareI(line[indice + 1],strTxt)){
					//secao nao identificada
					erro++;
					cout << "Erro semantico! Linha: " << linha << ". Secao não identificada." << endl;

				}
				else{
					cout << "Erro lexico! Linha: " << linha << ". Token não identificado." << endl;
					erro++;
				}
			}
		}
		linha++;
	}

	if(isOnData){
		dataFinal = endereco - 1;
	}

	//Atualizar o obj com as listas de uso 
	for(map<string,rotulo>::iterator it=tabelaDeRotulos.begin(); it!=tabelaDeRotulos.end(); it++){
		if(!pre_parser::verificaValidadeDeToken(it->first)){
			erro++;
			cout << "Erro lexico! linha: " << linha <<  ". Identificador de rótulo invalido." << endl;

		}

		else if(it->second.defined == 0){
			cout << "Erro semantico. Linha: " << linha << ". Variavel nao declarada." << endl;
			erro++;

		}else{
			while(!it->second.use.empty()){
				// Percorrer as listas e somar com os valores obtidos na passagem unica	
				endMod = it->second.use.front();
				auxValue = vetParaArquivo[endMod]; 
				vetParaArquivo[endMod] += it->second.value;
				it->second.use.pop_front();
			}
		}
	}

	//Verificar acesso a memoria
	for (i = 0; i < usouMais.size(); ++i)
	{
		if(vetParaArquivo[usouMais[i]] < dataInicial || vetParaArquivo[usouMais[i]] > dataFinal){
			cout << "Erro semantico. Tentando acessar memoria nao reservada." << endl;
		}
	}

	//verificar erros finais
	if(numText == 0){
		erro++;
		cout << "Erro semantico! Linha: " << linha << ". Seçao TEXT não declarada." << endl;
	}

	if(numData == 0){
		erro++;
		cout << "Erro semantico! Linha: " << linha << ". Seçao DATA não declarada." << endl;
	}
	if(numData > 1){
		erro++;
		cout << "Erro semantico! Linha: " << linha << ". Secao DATA declarada mais de uma vez." << endl;
	}
	if(numText > 1){
		erro++;
		cout << "Erro semantico! Linha: " << linha << ". Secao TEXT declarada mais de uma vez." << endl;
	}


	//Verificar se não houve erro nos argumentos
	//modificadores:
	argumento elemento;
	while(!argMod.empty()){
		elemento = argMod.front();
		map<string,rotulo>::iterator it2 = tabelaDeRotulos.find(elemento.arg);
		if(it2 != tabelaDeRotulos.end()){
			if(it2->second.isConst == 1){
				erro++;
				cout << "Erro semantico! Linha: " << elemento.linha <<  ". Modificacao de constante." << endl;
			}
		}

		argMod.pop_front();
	}


	//jumps
	while(!argJmp.empty()){
		elemento = argJmp.front();
		map<string,rotulo>::iterator it2 = tabelaDeRotulos.find(elemento.arg);
		if(it2 != tabelaDeRotulos.end()){
			if(it2->second.isVar == 1){
				erro++;
				cout << "Erro semantico! Linha: " << elemento.linha <<  ". Pulo para a secao de dados." << endl;
			}
		}

		argJmp.pop_front();
	}

	//divs
	while(!argDiv.empty()){
		elemento = argDiv.front();
		map<string,int>::iterator it2 = constParaValor.find(elemento.arg);
		if(it2 != constParaValor.end()){
			if(it2->second == 0){
				erro++;
				cout << "Erro semantico! Linha: " << elemento.linha <<  ". Divisao por zero." << endl;
			}

		}

		argDiv.pop_front();
	}


	//somente escrever se tiver tudo nos conformes
	if(!erro){
		strParaArquivoTotal.clear();
		for(i=0; i<vetParaArquivo.size(); i++){
			strParaArquivoTotal += to_string(vetParaArquivo[i]);
			strParaArquivoTotal += " ";
		}
		fpOutput.open(outExtension);
		fpOutput << strParaArquivoTotal;
	}else{
		cout << "\n--> Arquivo obj nao gerado, ha erros no source." << endl;
	}

	fpInput.close();
	fpOutput.close();
	return erro;
}


int main(int argc, char* argv[]){
	char * outPre = new char[14];
	char * outMacro = new char[16];
	char * input;
	char * output;
	strcpy(outPre, "outputs/outPre");
	strcpy(outMacro, "outputs/outMacro");
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



	return 0;
}

