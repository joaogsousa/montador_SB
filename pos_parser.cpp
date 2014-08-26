#include "pos_parser.hpp"

namespace pos_parser{

    void segundaPassada(char* output,char* inputPre){
        ifstream arqPre(inputPre);
        ofstream arqObjeto(output);

        if(!arqPre.is_open()){
            cout << "Erro ao abrir o arquivo pre processado" << endl;
        }




    }



};

