#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"

void resetarLista(Figurinha *vetor, int total){
    
    FILE *arquivo = fopen("extras/figurinhas2026.csv", "r");//abre o arquivo original para leitura
    FILE *arquivo_copia = fopen("extras/figurinhas2026copy.csv", "w");//abre um novo arquivo para escrita da lista resetada

    if (arquivo == NULL){
        printf("\nErro ao abrir o arquivo para resetar a lista.\n");
        return;
    }//if
    for(int i = 0; i < total; i++){
        fscanf(arquivo, " %9[^,],%49[^,],%49[^,],%49[^,],%49[^\n]", vetor[i].codigo, vetor[i].titulo, vetor[i].secao, vetor[i].grupo, vetor[i].tipo);
        fprintf(arquivo_copia, " %9s,%49s,%49s,%49s,%49s\n", vetor[i].codigo, vetor[i].titulo, vetor[i].secao, vetor[i].grupo, vetor[i].tipo);
    }//for

    fclose(arquivo);//fecha o arquivo original
    fclose(arquivo_copia);//fecha o arquivo de cópia

    printf("\nLista resetada com sucesso!\n");
}//função para resetar a lista de figurinhas