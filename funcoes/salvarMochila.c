#include <stdio.h>
#include <stdlib.h>
#include "salvarMochila.h"

void salvarMochila(Figurinha *mochila, int total_mochila){

    FILE *arquivo = fopen("extras/mochila.bin", "wb");

    if(arquivo == NULL){
        return;
    }

    fwrite(&total_mochila, sizeof(int), 1, arquivo);
    fwrite(mochila, sizeof(Figurinha), total_mochila, arquivo);

    fclose(arquivo);
}