#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"

void excluirAlbum(Figurinha *figurinhas, Figurinha *album, int *total_album){

    char codigo[10];//variavel para armazenar o código da figurinha a ser excluida

    printf("Digite o código da figurinha que deseja excluir: \n");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo,"\n")] = '\0';

    for (int i = 0; i < *total_album; i++){

        char codigo_album[10];//variavel auxiliar para armazenar o código da figurinha do album sem espaços extras

        strcpy(codigo_album, album[i].codigo);//copia o código da figurinha do album para a variável auxiliar

        for(int k = strlen(codigo_album) - 1; k >= 0 && codigo_album[k] == ' '; k--){

            codigo_album[k] = '\0';//remove os espaços em branco do final do código, caso haja

        }//for

        printf("Comparando '%s' com '%s'\n", codigo, codigo_album);

        if(strcmp(codigo, codigo_album) == 0){

            printf("FIGURINHA ENCONTRADA!\n");

            for (int j = i; j < *total_album - 1; j++){

            album[j] = album[j + 1];//move as figurinhas seguintes para preencher o espaço da figurinha excluida

            }//for

            (*total_album)--;//decrementa o total de figurinhas do album

            printf("Novo total: %d\n", *total_album);

            FILE *arquivo = fopen("album.csv", "w");

            if(arquivo != NULL){

                for(int k = 0; k < *total_album; k++){

                    fprintf(arquivo, "%s,%s,%s,%s,%s\n", album[k].codigo, album[k].titulo, album[k].secao, album[k].grupo, album[k].tipo);
                    //reescreve o arquivo album.csv com as figurinhas restantes após a exclusão
                }

                fclose(arquivo);//fecha o arquivo album.csv

            }

            return;

        }//if

    }//for
    printf("Figurinha com o código %s não encontrada.\n", codigo);

}//função para excluir uma figurinha do vetor album, recebe o vetor de album e o total de figurinhas lidas do arquivo, solicita ao usuário o código da figurinha a ser excluida, procura a figurinha no vetor e a remove se encontrada, caso contrário informa que a figurinha não foi encontrada.