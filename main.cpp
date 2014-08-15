#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>
#include <cstdlib>
#include <vector>
#include <cstring>

using namespace std;


void verificarVetor(vector<char*> vetor){
    int i;
    for(i=0;i<vetor.size();i++){
        cout << vetor[i] << endl;

    }

}

vector<char*> substituir(vector<char*> vetorTokens,int i){
    char* procurar = vetorTokens[i-1];
    char* novo = vetorTokens[i+1];
    int j;
    int arrumado = 0;

    j = 0;
    while(!arrumado){
        if(procurar[j] == ':'){
            procurar[j] = '\0';
            arrumado = 1;
        }

        j++;
    }



    for(j=0;j<vetorTokens.size();j++){
        if(!strcmp(vetorTokens[j],procurar)){
            vetorTokens[j] = novo;
        }

    }

    return vetorTokens;

}

char* readFile(char* filename)
{
	char* content;
	long int size;
    FILE* file = fopen(filename,"r");
    if(file == NULL)
    {
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);

    content = (char*)malloc(size);

    fread(content,1,size,file);

    return content;
}

vector<char*> tokens(char* arquivo){
    char* stringTotal;
    vector<char*> vetorTokens;
    char* pch;

    stringTotal = readFile(arquivo);

    pch = strtok (stringTotal," ,'\n'");
    while (pch != NULL)
    {
        //printf ("%s\n",pch);
        vetorTokens.push_back(pch);
        pch = strtok (NULL, " ,'\n'");
    }

    return vetorTokens;

}

int main(int argc,char * argv[]){
    string nomeInput;
    string preProcess;
    string object;

    ifstream fpInput;

    vector<char*> vetorTokens;

    char buffer;

    int i;




    if(argc != 4){
        cout << "Erro na passagem de argumentos" << endl;
        exit(1);
    }
    else{
        nomeInput = argv[1];
        preProcess = argv[2];
        object = argv[3];
    }



    fpInput.open(argv[1]);
    if(!fpInput.is_open()){
        cout << "Erro ao ler o arquivo assembly de input" << endl;
        exit(1);
    }

    ofstream fpPre;
    fpPre.open(argv[2]);

    while(!fpInput.eof()){
        buffer = fpInput.get();


        if(buffer == ';'){
            while(buffer != '\n' && !fpInput.eof()){
                buffer = fpInput.get();

            }

        }

        if(buffer != EOF){
            fpPre << buffer ;
        }




    }
    fpPre.close();

    vetorTokens = tokens(argv[2]);

    for(i=0;i<vetorTokens.size();i++){
        if(!strcmp(vetorTokens[i],"EQU")){
            vetorTokens = substituir(vetorTokens,i);
            vetorTokens.erase(vetorTokens.begin()+(i-1),vetorTokens.begin()+(i+2));
            i-=2;
        }

    }


    verificarVetor(vetorTokens);


    fpInput.close();


    return 0;
}

