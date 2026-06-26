#ifndef ABRIRPACOTE_H
#define ABRIRPACOTE_H

#include <raylib.h>
#include "biblioteca.h"

void numeroParaTexto(int numero, char *destino);

void numeroParaTextoComDoisDigitos(int numero, char *destino);

void abrirPacote(Figurinha *figurinhas,Figurinha *mochila,Figurinha *album,int total,int *total_mochila,int *total_album,Font fonteCopa,Color azulBrasil,Color amareloBrasil,EstadoMenu *estadoAtual);

#endif