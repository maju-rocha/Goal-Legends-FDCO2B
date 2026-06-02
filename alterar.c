#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"

void alterarFigurinha(Figurinha *vetor, int total) {
    char codigoBusca[10];
    int encontrada = 0;

    printf("\n--- Alterar Figurinha ---\n");
    printf("Digite o codigo da figurinha que deseja alterar: ");
    scanf(" %9[^\n]", codigoBusca);

    for (int i = 0; i < total; i++) {
        char codigo_limpo[15]; 
        
        strcpy(codigo_limpo, vetor[i].codigo);

        for(int k = strlen(codigo_limpo) - 1; k >= 0 && codigo_limpo[k] == ' '; k--) {
            codigo_limpo[k] = '\0'; 
        }//for

        if (strcmp(codigo_limpo, codigoBusca) == 0) {
            printf("\nFigurinha Encontrada: %s - %s\n", vetor[i].codigo, vetor[i].titulo);
            
            printf("Digite o novo Titulo: ");
            scanf(" %49[^\n]", vetor[i].titulo);
            
            printf("Digite a nova Secao: ");
            scanf(" %49[^\n]", vetor[i].secao);

            printf("Digite o novo Tipo: ");
            scanf(" %49[^\n]", vetor[i].tipo);
            
            printf("\n Figurinha alterada com sucesso!\n");
            encontrada = 1;
            break;
        }//if
    }//for

    if (!encontrada) {
        printf("\n Figurinha nao encontrada para alteracao.\n");
    }//if
}//void