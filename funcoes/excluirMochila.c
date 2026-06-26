#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"
#include "salvarMochila.h"

void excluirMochila(Figurinha *mochila, int *total_mochila){

    char codigo[10];//Variável para armazenar o código da figurinha digitado

    //Fgets para o código do usuário
    printf("Digite o código da figurinha que deseja excluir da mochila: \n");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = '\0';

    for(int i = 0; i < *total_mochila; i++){

        if(strcmp(codigo, mochila[i].codigo) == 0){

            //Move todas as figurinhas depois de excluir uma posição para trás
            for(int j = i; j < *total_mochila - 1; j++){
                mochila[j] = mochila[j + 1];
            }//for

            (*total_mochila)--;//Diminui o total da mochila

            salvarMochila(mochila, *total_mochila);//Chama a função para salvar no mochila.bin

            printf("\nFigurinha excluida com sucesso.\n");//Texto terminal
            return;

        }//if
    }//for

    printf("Figurinha com o código %s não encontrada.\n", codigo);//Texto terminal

}//void