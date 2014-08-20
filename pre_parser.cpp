#include "pre_parser.hpp"

namespace pre_parser{
    int isPseudo(char* string){

        return 0;
    }

    int isLabel(char* string){
        int i;
        i = 0;

        while(string[i] != '\0'){
            if(string[i] == ':' && string[i+1] == '\0'){
                return 1;
            }

            i++;
        }

        return 0;
    }

    int numOperandos(char* instrucao){
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

    int isOperando(char* string){
        if((string[0] == 'N' || string[0] == 'n') && !pre_parser::isLabel(string)){
            return 1;

        }else{
            return 0;
        }
    }


    vector<char*> getBasicInstFromPseudo(vector<char*> pseudo){

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

    int stringCompareI(char * str1, char * str2){
        int i = 0;

        if(strlen(str1) == strlen(str2)){
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


    char* isDiretiva(char* string){
        char* section = "section";
        char* equ = "equ";
        char* if_string = "if";
        char* space = "space";
        char* const_string = "const";

        if(stringCompareI(string,section)){
            return section;
        }
        if(stringCompareI(string,equ)){
            return equ;
        }
        if(stringCompareI(string,if_string)){
            return if_string;
        }
        if(stringCompareI(string,space)){
            return space;
        }
        if(stringCompareI(string,const_string)){
            return const_string;
        }
        return NULL;
    }

    char* isInstruction(char* string){

        if(stringCompareI(string,"ADD")){
            return "ADD";
        }
        if(stringCompareI(string,"SUB")){
            return "SUB";
        }
        if(stringCompareI(string,"MULT")){
            return "MULT";
        }
        if(stringCompareI(string,"DIV")){
            return "DIV";
        }
        if(stringCompareI(string,"JMP")){
            return "JMP";
        }
        //mais
        if(stringCompareI(string,"JMPN")){
            return "JMPN";
        }
        if(stringCompareI(string,"JMPP")){
            return "JMPP";
        }
        if(stringCompareI(string,"JMPZ")){
            return "JMPZ";
        }
        if(stringCompareI(string,"COPY")){
            return "COPY";
        }
        if(stringCompareI(string,"LOAD")){
            return "LOAD";
        }
        //mais
        if(stringCompareI(string,"STORE")){
            return "STORE";
        }
        if(stringCompareI(string,"INPUT")){
            return "INPUT";
        }
        if(stringCompareI(string,"OUTPUT")){
            return "OUTPUT";
        }
        if(stringCompareI(string,"STOP")){
            return "STOP";
        }

        return NULL;
    }

    void gerarPreProcessado(vector<char*> vetorTokens, char* nomeArquivo){
        ofstream arquivoPre;
        int i;
        int espaco;

        arquivoPre.open(nomeArquivo);

        for(i=0;i<vetorTokens.size();i++){
            espaco = 0;
            arquivoPre << vetorTokens[i];
            if(stringCompareI(vetorTokens[i],"STOP")){
                arquivoPre << '\n';
                espaco = 1;

            }
            if(stringCompareI(vetorTokens[i],"DATA") || stringCompareI(vetorTokens[i],"TEXT")){
                arquivoPre << '\n';
                espaco = 1;

            }
            if(i > 1 && (pre_parser::numOperandos(vetorTokens[i-1]) == 1 || pre_parser::numOperandos(vetorTokens[i-2]) == 2)){
                arquivoPre << '\n';
                espaco = 1;

            }

            if(!espaco){
                arquivoPre << ' ';

            }



        }

        arquivoPre.close();

    }

};
