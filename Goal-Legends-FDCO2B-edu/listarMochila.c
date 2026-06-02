#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"

void listarFigurinhasMochila(Figurinha *mochila, int total_mochila){
    for (int i = 0; i < total_mochila; i++){
        
        printf("Código: %s, Título: %s\n", mochila[i].codigo, mochila[i].titulo);
        //imprime cada figurinha da mochila com seus campos formatados

    }//imprime cada figurinha da mochila com seus campos formatados

}//função para listar as figurinhas, recebe o vetor de mochila e o total de figurinhas, percorre o vetor e imprime cada figurinha com seus campos formatados.