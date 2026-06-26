#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "listarAlbum.h"

void listarFigurinhasAlbum(Figurinha *album, int total_album){
    for (int i = 0; i < total_album; i++){
        
        //Texto de cada figurinha do album com seus campos formatados
        printf("Código: %s, Título: %s\n", album[i].codigo, album[i].titulo);
    }

}//void