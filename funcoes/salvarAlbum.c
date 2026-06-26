#include <stdio.h>
#include <stdlib.h>
#include "salvarAlbum.h"

void salvarAlbum(Figurinha *album, int total_album){

    FILE *arquivo = fopen("extras/album.bin", "wb");

    if(arquivo == NULL){
        return;
    }

    fwrite(&total_album, sizeof(int), 1, arquivo);
    fwrite(album, sizeof(Figurinha), total_album, arquivo);

    fclose(arquivo);
}