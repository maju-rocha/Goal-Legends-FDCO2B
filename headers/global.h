#ifndef GLOBAL_H
#define GLOBAL_H

#include "biblioteca.h"

//=======================================================//
//=================== Variaveis Globais =================//
//=======================================================//

extern int pacotes_fechados;
extern int figurinha_repetida;
extern Menu_FimdeJogo estadoAtual;

//=======================================================//
//================== Funcoes Globais ====================//
//=======================================================//

void limparEspacos(char *texto);
void limparFigurinha(Figurinha *f);
void trocarEspacoPorUnderline(char *texto);
int estaNoAlbum(Figurinha *album, int total_album, char codigo[]);

#endif