#include <stdio.h>
#include <stdlib.h>
#include "salvarMochila.h"

void salvarMochila(Figurinha *mochila, int total_mochila){

    FILE *arquivo = fopen("extras/mochila.bin", "wb");//Escreve em modo binário no arquivo mochila.bin

    if(arquivo == NULL){
        return;
    }//if teste

    fwrite(&total_mochila, sizeof(int), 1, arquivo);//Salva a quantidade de figurinhas no arquivo.
    fwrite(mochila, sizeof(Figurinha), total_mochila, arquivo);//Salva o vetor de figurinhas no arquivo.

    fclose(arquivo);//Fecha o arquivo
}