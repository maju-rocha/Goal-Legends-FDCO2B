#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pesquisar.h"

void pesquisarFigurinha(Figurinha *vetor, int total){
    
    char codigo[10];//variavel para armazenar o código da figurinha a ser pesquisada
    int encontrada = 0;//variavel para indicar se a figurinha foi encontrada ou não

    printf("Digite o código da figurinha que deseja pesquisar (ex: MEX2): \n");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo,"\n")] = '\0'; 

    for (int i = 0; i < total; i++){

        char codigo_busca[15];//variavel auxiliar para armazenar o código da figurinha sem espaços extras

        strcpy(codigo_busca, vetor[i].codigo);

        for(int k = strlen(codigo_busca) - 1; k >= 0 && codigo_busca[k] == ' '; k--){//remove os espaços em branco do final do código
            codigo_busca[k] = '\0'; 
        }//for

        if (strcmp(codigo_busca, codigo) == 0){
            printf("\n FIGURINHA ENCONTRADA!\n");
            
            printf("Codigo: %s\n", vetor[i].codigo);
            printf("Titulo: %s\n", vetor[i].titulo);
            printf("Secao: %s\n", vetor[i].secao);
            printf("Grupo: %s\n", vetor[i].grupo);
            printf("Tipo: %s\n", vetor[i].tipo);
            
            encontrada = 1;
            break; 
        }//if
    }//for

    if (!encontrada){
        printf("\n Figurinha com o codigo '%s' nao foi encontrada.\n", codigo);
    }//if
}//função para pesquisar figurinha