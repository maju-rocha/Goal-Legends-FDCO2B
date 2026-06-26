#include <stdio.h>
#include <stdlib.h>
#include "carregarAlbum.h"

void carregarAlbum(Figurinha *album, int *total_album){

    FILE *arquivo = fopen("extras/album.bin", "rb");

    if(arquivo == NULL){
        *total_album = 0;
        return;
    }

    fread(total_album, sizeof(int), 1, arquivo);
    fread(album, sizeof(Figurinha), *total_album, arquivo);

    fclose(arquivo);
}