#ifndef ABRIRPACOTE_H
#define ABRIRPACOTE_H

#include <raylib.h>
#include "biblioteca.h"

//Função para transformar número inteiro em texto
void numeroParaTexto(int numero, char *destino);

//Função para transformar número em texto com dois dígitos
void numeroParaTextoComDoisDigitos(int numero, char *destino);

//Função principal para abrir pacotes
void abrirPacote(Figurinha *figurinhas,Figurinha *mochila,Figurinha *album,int total,int *total_mochila,int *total_album,Font fonteCopa,Color azulBrasil,Color amareloBrasil,EstadoMenu *estadoAtual);

#endif