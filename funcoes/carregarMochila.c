#include <stdio.h>
#include <stdlib.h>
#include "carregarMochila.h"

void carregarMochila(Figurinha *mochila, int *total_mochila){

    FILE *arquivo = fopen("extras/mochila.bin", "rb");//Le em modo binário o arquivo mochila.bin

    //Caso não tenha o arquivo ele começa do zero
    if(arquivo == NULL){
        *total_mochila = 0;
        return;
    }//if

    fread(total_mochila, sizeof(int), 1, arquivo);//Le a quantidade total de figurinhas da mochila
    fread(mochila, sizeof(Figurinha), *total_mochila, arquivo);//Le as figurinhas do arquivo

    fclose(arquivo);//Fecha o arquivo
}
