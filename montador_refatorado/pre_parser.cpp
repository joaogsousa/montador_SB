#include "pre_parser.hpp"

namespace pre_parser{
    int isPseudo(string string1){

        return 0;
    }

    int isLabel(string string){
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


    string isDiretiva(string string1){
        string section("section");
        string equ("equ");
        string if_string("if");
        string space("space");
        string const_string("const");

        if(string1.compare(section)){
            return section;
        }
        if(string1.compare(equ)){
            return equ;
        }
        if(string1.compare(if_string)){
            return if_string;
        }
        if(string1.compare(space)){
            return space;
        }
        if(string1.compare(const_string)){
            return const_string;
        }
        return NULL;
    }

    int isInstruction(string string1){

        if(string1.compare("ADD")){
            return 1;
        }
        if(string1.compare("SUB")){
            return 1;
        }
        if(string1.compare("MULT")){
            return 1;
        }
        if(string1.compare("DIV")){
            return 1;
        }
        if(string1.compare("JMP")){
            return 1;
        }
        //mais
        if(string1.compare("JMPN")){
            return 1;
        }
        if(string1.compare("JMPP")){
            return 1;
        }
        if(string1.compare("JMPZ")){
            return 1;
        }
        if(string1.compare("COPY")){
            return 1;
        }
        if(string1.compare("LOAD")){
            return 1;
        }
        //mais
        if(string1.compare("STORE")){
            return 1;
        }
        if(string1.compare("INPUT")){
            return 1;
        }
        if(string1.compare("OUTPUT")){
            return 1;
        }
        if(string1.compare("STOP")){
            return 1;
        }

        return 0;
    }


};
