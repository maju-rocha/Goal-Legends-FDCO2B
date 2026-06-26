#include <stdio.h>
#include <stdlib.h>
#include "carregarAlbum.h"

void carregarAlbum(Figurinha *album, int *total_album){

    FILE *arquivo = fopen("extras/album.bin", "rb");//Le em modo binário o arquivo album.bin

    //Caso não tenha o arquivo ele começa do zero
    if(arquivo == NULL){
        *total_album = 0;
        return;
    }//if

    fread(total_album, sizeof(int), 1, arquivo);//Le a quantidade total de figurinhas do album
    fread(album, sizeof(Figurinha), *total_album, arquivo);//Le as figurinhas do arquivo

    fclose(arquivo);
}