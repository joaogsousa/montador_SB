-- ====================================================================================================== --
		Macro montador da linguagem hipotética de SB 2014-2
	Grupo:	João Henrique Gonçalves de Sousa    -   11/0014171
		Daniella Albuquerque dos Angelos    -   11/0010434
-- ======================================================================================================= --

Sobre o trabalho:
-> montador feito na disciplina software basico para traduzir um codigo assembly simples em código de máquina.
-> feito totalmente em linux, ubuntu.
-> a pasta contém os códigos fonte e o makefile
-> para compilar o projeto apenas faça: make. para executar use o ./montador -option input output
-> obs: utilizamos o c++ 11!!!! portanto se houver algum erro na compilacao, provavelmente sera a falta do cpp11 no seu computador, que é facilmente adquirido.
-> o nosso montador gera arquivos auxiliares das outras passagens, exemplo:
	se for feito	./montador -o input output 		o montador gerara o arquivo final output.o que representa o objeto, mas tambem gerará os arquivos: outPre.pre do pre-processamento e o outMacro.mcr do processamento de macros do input.

