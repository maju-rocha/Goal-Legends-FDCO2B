#include <stdio.h>
#include <stdlib.h>
#include "figurinhas.h"

void abrirPacote(Figurinha *figurinhas, int total) {

    printf("Pacote com 7 figurinhas:\n\n");

    for (int i = 0; i < 7; i++) {

        int sorteada = rand() % total;

        printf("Codigo:  %s\n", figurinhas[sorteada].codigo);
        printf("Titulo: %s\n", figurinhas[sorteada].titulo);
        printf("Secao : %s\n", figurinhas[sorteada].secao);
        printf("Grupo : %s\n", figurinhas[sorteada].grupo);
        printf("Tipo  : %s\n\n", figurinhas[sorteada].tipo);
    }//for

}//função abrirPacote
