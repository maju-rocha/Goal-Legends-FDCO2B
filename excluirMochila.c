#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"

void excluirMochila(Figurinha *figurinhas, Figurinha *mochila, int *total_mochila){
    
    char codigo[10];//variavel para armazenar o código da figurinha a ser excluida

    printf("Digite o código da figurinha que deseja excluir da mochila: \n");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo,"\n")]='\0';
    setbuf(stdin, NULL);

    for (int i = 0; i < *total_mochila; i++) {
        if(strcmp(codigo, mochila[i].codigo) == 0){
            //encontra a figurinha com o código correspondente e a exclui do vetor mochila
            for (int j = i; j < *total_mochila - 1; j++) {
                mochila[j] = mochila[j + 1];
            }
            (*total_mochila)--; //decrementa o total de figurinhas da mochila

            FILE *arquivo = fopen("mochila.csv", "w");

            if(arquivo != NULL){

                for(int k = 0; k < *total_mochila; k++){

                    fprintf(arquivo, "%s,%s,%s,%s,%s\n", mochila[k].codigo, mochila[k].titulo, mochila[k].secao, mochila[k].grupo, mochila[k].tipo);
                    //reescreve o arquivo mochila.csv com as figurinhas restantes após a exclusão
                }

                fclose(arquivo);//fecha o arquivo mochila.csv

            }

            printf("Figurinha excluida com sucesso.\n");
            return;
        }//if
    }//for

    printf("Figurinha com o código %s não encontrada.\n", codigo);

}//função para excluir uma figurinha do vetor mochila, recebe o vetor de mochila e o total de figurinhas lidas do arquivo, solicita ao usuário o código da figurinha a ser excluida, procura a figurinha no vetor e a remove se encontrada, caso contrário informa que a figurinha não foi encontrada.