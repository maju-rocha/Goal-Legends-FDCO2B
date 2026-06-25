#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"

void alterarFigurinha(Figurinha *vetor, int total){
    char codigoBusca[10];//variavel para armazenar o código da figurinha a ser alterada
    int encontrada = 0;//variavel para indicar se a figurinha foi encontrada ou não

    printf("\n--- Alterar Figurinha ---\n");
    printf("Digite o codigo da figurinha que deseja alterar: ");
    scanf(" %9[^\n]", codigoBusca);

    for (int i = 0; i < total; i++){
        char codigo_limpo[15];//variavel auxiliar para armazenar o código da figurinha sem espaços extras 
        
        strcpy(codigo_limpo, vetor[i].codigo);

        for(int k = strlen(codigo_limpo) - 1; k >= 0 && codigo_limpo[k] == ' '; k--){
            codigo_limpo[k] = '\0'; 
        }//for

        if (strcmp(codigo_limpo, codigoBusca) == 0){
            printf("\nFigurinha Encontrada: %s - %s\n", vetor[i].codigo, vetor[i].titulo);
            
            printf("Digite o novo Titulo: ");
            scanf(" %49[^\n]", vetor[i].titulo);
            
            printf("Digite a nova Secao: ");
            scanf(" %49[^\n]", vetor[i].secao);

            printf("Digite o novo Tipo: ");
            scanf(" %49[^\n]", vetor[i].tipo);
            
            printf("\n Figurinha alterada com sucesso!\n");
            encontrada = 1;

            FILE *arquivo = fopen("extras/figurinhas2026copy.csv", "w");//abre o arquivo para escrita da lista alterada

            if(arquivo != NULL){

                for(int k = 0; k < total; k++){

                    fprintf(arquivo, "%s,%s,%s,%s,%s\n", vetor[k].codigo, vetor[k].titulo, vetor[k].secao, vetor[k].grupo, vetor[k].tipo);
                    //reescreve o arquivo alterar.csv com as figurinhas restantes após a exclusão
                }//for

                fclose(arquivo);//fecha o arquivo alterar.csv

            }//if
            break;
        }//if
    }//for


    if (!encontrada){
        printf("\n Figurinha nao encontrada para alteracao.\n");
    }//if
}//funcao para alterar uma linha especifica no figurinhas2026copy.csv