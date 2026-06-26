#include <stdio.h>
#include <stdlib.h>
#include "carregarMochila.h"

void carregarMochila(Figurinha *mochila, int *total_mochila){

    FILE *arquivo = fopen("extras/mochila.bin", "rb");

    if(arquivo == NULL){
        *total_mochila = 0;
        return;
    }

    fread(total_mochila, sizeof(int), 1, arquivo);
    fread(mochila, sizeof(Figurinha), *total_mochila, arquivo);

    fclose(arquivo);
}