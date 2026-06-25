#ifndef GLOBAL_H
#define GLOBAL_H

#include "biblioteca.h"

//=======================================================//
//=================== Variaveis Globais =================//
//=======================================================//

//Variavel global da quantidade de pacotes fechados
extern int pacotes_fechados;

//Variavel global da quantidade de figurinhas repetidas
extern int figurinha_repetida;

//Variavel global do estado atual do fim de jogo
extern Menu_FimdeJogo estadoAtual;

//=======================================================//
//================== Funcoes Globais ====================//
//=======================================================//

//Funcao para limpar espacos antes e depois do texto
void limparEspacos(char *texto);

//Funcao para limpar todos os campos da figurinha
void limparFigurinha(Figurinha *f);

//Funcao para trocar espacos por underline
void trocarEspacoPorUnderline(char *texto);

//Funcao para verificar se uma figurinha ja esta no album
int estaNoAlbum(Figurinha *album, int total_album, char codigo[]);

#endif