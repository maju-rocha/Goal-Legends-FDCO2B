#include "biblioteca.h"
#include <string.h>

//Váriaveis globais
int pacotes_fechados = 0;
int figurinha_repetida = 0;
Menu_FimdeJogo estadoAtual = MENU_FIM;

int estaNoAlbum(Figurinha *album, int total_album, char codigo[]) {
    for(int i = 0; i < total_album; i++) {
        if(strcmp(album[i].codigo, codigo) == 0) {
            return 1;
        }
    }
    return 0;
}
