#ifndef FIGURINHAS_H
#define FIGURINHAS_H

typedef struct {

    char codigo[10];
    char titulo[50];
    char secao[50];
    char grupo[50];
    char tipo[50];

} Figurinha;

void abrirPacote(Figurinha *figurinhas, int total);

#endif