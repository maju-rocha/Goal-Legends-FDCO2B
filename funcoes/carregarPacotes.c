#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"
#include "global.h"
#include "carregarPacotes.h"
#include "carregarAlbum.h"
#include "carregarMochila.h"

void carregarPacotes(Figurinha *album, int *total_album, Figurinha *mochila, int *total_mochila){

    FILE *arquivo_carrega_pacotes = fopen("extras/pacotes.bin", "rb");

    if(arquivo_carrega_pacotes == NULL){
        pacotes_fechados = 0;
    }else{
        fread(&pacotes_fechados, sizeof(int), 1, arquivo_carrega_pacotes);
        fclose(arquivo_carrega_pacotes);
    }

    carregarAlbum(album, total_album);
    carregarMochila(mochila, total_mochila);
}