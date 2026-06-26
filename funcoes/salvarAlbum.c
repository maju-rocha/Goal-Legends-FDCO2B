#include <stdio.h>
#include <stdlib.h>
#include "salvarAlbum.h"

void salvarAlbum(Figurinha *album, int total_album){

    FILE *arquivo = fopen("extras/album.bin", "wb");//Escreve em modo binário no arquivo album.bin

    if(arquivo == NULL){
        return;
    }//if teste

    fwrite(&total_album, sizeof(int), 1, arquivo);//Salva a quantidade de figurinhas no arquivo.
    fwrite(album, sizeof(Figurinha), total_album, arquivo);//Salva o vetor de figurinhas no arquivo.

    fclose(arquivo);//Fecha orarquivo
}