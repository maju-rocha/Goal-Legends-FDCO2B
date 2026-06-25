#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resetarLista.h"

void resetarLista(Figurinha *vetor, int total){

    FILE *arquivo = fopen("extras/figurinhas2026.csv", "r");
    FILE *arquivo_copia = fopen("extras/figurinhas2026copy.csv", "w");

    char linha[1024];

    if(arquivo == NULL){
        printf("\nErro ao abrir o arquivo original para resetar a lista.\n");
        return;
    }//if

    if(arquivo_copia == NULL){
        printf("\nErro ao abrir o arquivo de copia para resetar a lista.\n");
        fclose(arquivo);
        return;
    }//if

    while(fgets(linha, sizeof(linha), arquivo)){
        fputs(linha, arquivo_copia);
    }//while

    fclose(arquivo);
    fclose(arquivo_copia);

    printf("\nLista resetada com sucesso!\n");

}//funcao para resetar a lista de figurinhas