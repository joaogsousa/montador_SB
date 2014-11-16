#include "../header/pre_parser.hpp"
#define LABELMAX 100

namespace pre_parser{
    int isPseudo(string string1){

        return 0;
    }

	int possuiVirgula(string operando){
	   if(operando[operando.size() - 1] == ','){
		   return 1;
	   }
	   return 0;
	}

	string retiraVirgula(string operando){
		return operando.erase(operando.find(','));
	}
		 
	int totOperandos(vector<string> line){
		int i;
		int total = 0;
		string aux;

		for (i = 0; i < line.size(); ++i)
		{
			if(line[i] == "+" || line[i] == "-"){
				total --;
			}
			else{
				total++;
			}	
		}

		return total;
	}

    char * addFormato(char * input, const char * formato){
	    char * extension;

	    extension = (char *) malloc(strlen(input) + strlen(formato) + 1);
	    strcpy(extension,input);
	    strcat(extension,formato);

	    return extension;
    }

    int isLabel(string string1){
        int i;
        i = 0;

        while(string1[i] != '\0'){
            if(string1[i] == ':' && string1[i+1] == '\0'){
                return 1;
            }

            i++;
        }

        return 0;
    }

    int numOperandosByOpCode(int opCode){
        if(opCode == 9){
            return 2;
        }
        else if(opCode == 14){
            return 0;
        }
        else if(opCode > 0 && opCode < 15){
            return 1;
        }
        else{
            return -1;
        }
    }

    int numOperandos(string instrucao){
        if(stringCompareI(instrucao,"COPY")){
            return 2;

        }else{
            if(stringCompareI(instrucao,"STOP")){
                return 0;
            }
            else{
                if(isInstruction(instrucao)){
                    return 1;

                }
                else{
                    return -1;

                }
            }
        }
    }

    int isOperando(string string1){
        if((string1[0] == 'N' || string1[0] == 'n') && !pre_parser::isLabel(string1)){
            return 1;

        }else{
            return 0;
        }
    }


    vector<string> getBasicInstFromPseudo(vector<string> pseudo){

        return pseudo;
    }

    //1 - upper || 2 - lower || 3 - nao caracter
    int isChar(char c){
        int caracter;
        caracter = (int)c;

        if(caracter >= 65 && caracter <= 90){
            return 1;
        }
        else{
            if(caracter >= 97 && caracter <= 122){
                return -1;

            }
            else{
                return 0;
            }
        }
    }

    int isNum(char c){
        int caracter;
        caracter = (int)c;
        if(caracter >= 48 && caracter <= 57){
            return 1;
        }
        else{
            return 0;
        }
    }

    int letraCompare(char c, char d){
        int atoic = (int)c;
        int atoid = (int)d;

        if(isChar(c) == 1){
            atoic = atoic + 32;
        }
        if(isChar(d) == 1){
            atoid = atoid + 32;
        }

        return (atoid == atoic);
    }

    int stringCompareI(string str1, string str2){
        int i = 0;

        if(str1.size() == str2.size()){
            while(str1[i] != '\0'){
                if(!letraCompare(str1[i],str2[i])){
                    return 0;
                }

                i++;
            }
        }
        else{
            return 0;
        }

        return 1;
    }


    int isDiretiva(string string1){
        string section("section");
        string equ("equ");
        string if_string("if");
        string space("space");
        string const_string("const");

        if(stringCompareI(string1, section)){
            return 1;
        }
        if(stringCompareI(string1, equ)){
            return 1;
        }
        if(stringCompareI(string1, if_string)){
            return 1;
        }
        if(stringCompareI(string1, space)){
            return 1;
        }
        if(stringCompareI(string1, const_string)){
            return 1;
        }
        return 0;
    }

    int isInstruction(string string1){


        if(stringCompareI(string1, "ADD")){
            return 1;
        }
        if(stringCompareI(string1, "SUB")){
            return 2;
        }
        if(stringCompareI(string1, "MULT")){
            return 3;
        }
        if(stringCompareI(string1, "DIV")){
            return 4;
        }
        if(stringCompareI(string1, "JMP")){
            return 5;
        }
        //mais
        if(stringCompareI(string1, "JMPN")){
            return 6;
        }
        if(stringCompareI(string1, "JMPP")){
            return 7;
        }
        if(stringCompareI(string1, "JMPZ")){
            return 8;
        }
        if(stringCompareI(string1, "COPY")){
            return 9;
        }
        if(stringCompareI(string1, "LOAD")){
            return 10;
        }
        //mais
        if(stringCompareI(string1, "STORE")){
            return 11;
        }
        if(stringCompareI(string1, "INPUT")){
            return 12;
        }
        if(stringCompareI(string1, "OUTPUT")){
            return 13;
        }
		if(stringCompareI(string1, "C_INPUT")){
			return 14;
		}
		if(stringCompareI(string1, "C_OUTPUT")){
			return 15;
		}
		if(stringCompareI(string1, "S_INPUT")){
			return 16;
		}
		if(stringCompareI(string1, "S_OUTPUT")){
			return 17;
		}
        if(stringCompareI(string1, "STOP")){
            return 18;
        }
        return 0;
    }

    void verificarMap(map<string,int> mymap){
        // show content:
        for (map<string,int>::iterator it=mymap.begin(); it!=mymap.end(); ++it){
            std::cout << it->first << " => " << it->second << '\n';
        }


    }


    void verificarMapString(map<string,string> mymap){
        // show content:
        for (map<string,string>::iterator it=mymap.begin(); it!=mymap.end(); ++it){
            std::cout << it->first << " => " << it->second << '\n';
        }


    }


    void verificarMapStringToVector(map<string, vector< vector< string> > > mymap){
        // show content:
        for (map<string, vector< vector< string> > >::iterator it=mymap.begin(); it!=mymap.end(); ++it){
            std::cout << it->first << " => " << '\n';
            verificarVector(it->second);
            cout << endl << endl;

        }


    }


    void verificarVector(vector<vector<string> > vetorStrings){
        int i;
        int j;

        for(i=0;i<vetorStrings.size();i++){
            for(j=0;j<vetorStrings[i].size();j++){

                cout << vetorStrings[i][j] << " ";
            }
            cout << endl;
        }


    }


    void gerarPreProcessado(vector<vector<string> > vetorStrings,char* arquivoPre){
        int i;
        int j;
        ofstream arquivoSaida(arquivoPre);
        string vazia("");
        string espaco(" ");
        string newLine;
        newLine += '\n';

        //debug
        //cout << "debug do gerar" << endl;
        //cout << vetorStrings[vetorStrings.size()-1][0] << endl << endl;

        for(i=0;i<vetorStrings.size() - 1;i++){
            for(j=0;j<vetorStrings[i].size();j++){
                if(!stringCompareI(vetorStrings[i][j],vazia) && !stringCompareI(vetorStrings[i][j],espaco) && !stringCompareI(vetorStrings[i][j],newLine)){
                    arquivoSaida << vetorStrings[i][j];
                }
                if(j+1<vetorStrings[i].size()){
                    arquivoSaida << " ";
                }
            }
            if(vetorStrings[i].size() >= 1){
                arquivoSaida << endl;
            }
        }

        arquivoSaida.close();




    }


    int indiceComEndereco(int end,string str){
        int i;
        int numEspacos;
        numEspacos = 0;
        i = 0;
        while(numEspacos < end){
            if(str[i] == ' '){
                numEspacos ++;
            }

            i++;
        }

        return i;
    }

    int retornaIndiceDoLabel(string str){
        int i;
        string aux;

        for(i=0;i<str.size();i++){
            if(str[i] == '['){
                while(str[i] != ']'){
                    i++;
                    if(str[i] != ']'){
                        aux += str[i];
                    }
                    else{
                        return stoi(aux);
                    }
                }
            }

        }

        return 0;


    }

    int charValido(char c){
        if(isNum(c) || isChar(c) || c == '_'){
            return 1;
        }else{
            return 0;

        }

    }


    int verificaValidadeDeToken(string str){
        int i;

        if(str.size() > LABELMAX){
            return 0;
        }

        if(pre_parser::isNum(str[0])){
            return 0;
        }

        for(i=0;i<str.size();i++){
            if(!charValido(str[i])){
                return 0;
            }

        }

        return 1;


    }


};
