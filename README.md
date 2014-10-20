Macro montador da linguagem hipotética de SB
2014-2
Dupla:
João Henrique Gonçalves de Sousa    -   11/0014171
Daniella Albuquerque dos Angelos    -   11/0010434
=========================================================================================================

Sobre o trabalho:
-Montador feito na disciplina Software Basico para traduzir um codigo assembly simples em código de máquina.
-Feito totalmente em linux, ubuntu.
-A pasta contém códigos e makefile
-Para compilar o projeto apenas faça: make. Para executar use o ./montador -option input output
-OBS!!! Utilizamos o C++ 11!!!!!!!
-Portanto se houver algum erro na hora do make deve ser pela falta do cpp 11 no seu computador, que é facil de ser adquerido.
-O nosso montador gera arquivos auxiliares das outras passagens, exemplo:
-Se for feito ./montador -o input output o montador gerara o arquivo final output.o que representa o objeto, mas tambem gerará os
arquivos: outPre.pre do pre processamento do input e o outMacro.mcr do processamento de macros do input.

