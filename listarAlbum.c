#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"

void listarFigurinhasAlbum(Figurinha *album, int total_album){
    for (int i = 0; i < total_album; i++){
        
        printf("Código: %s, Título: %s\n", album[i].codigo, album[i].titulo);
        //imprime cada figurinha do album com seus campos formatados

    }//imprime cada figurinha do album com seus campos formatados

}//função para listar as figurinhas, recebe o vetor de album e o total de figurinhas, percorre o vetor e imprime cada figurinha com seus campos formatados.