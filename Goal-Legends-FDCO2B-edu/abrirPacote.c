#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"

void abrirPacote(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album){

    char opcao[10];//variavel para armazenar a resposta do usuário sobre onde guardar a figurinha sorteada

    FILE *arquivoalbum = fopen("album.csv", "a");//abre o arquivo album.csv para escrita, caso o arquivo não exista ele será criado

    if(arquivoalbum == NULL){
        printf("Erro ao abrir album.csv\n");
        return;
    }//teste de abertura do arquivo album.csv

    FILE *arquivomochila = fopen("mochila.csv", "a");

    if(arquivomochila == NULL){
        printf("Erro ao abrir mochila.csv\n");
        fclose(arquivoalbum);
        return;
    }//teste de abertura ddo arquivo mochila.csv

    printf("\nPacote com 7 figurinhas:\n\n");

    for(int i = 0; i < 7; i++){
        int sorteada = rand() % total;

        printf("Codigo: %s\n", figurinhas[sorteada].codigo);
        printf("Titulo: %s\n", figurinhas[sorteada].titulo);
        printf("Secao : %s\n", figurinhas[sorteada].secao);
        printf("Grupo : %s\n", figurinhas[sorteada].grupo);
        printf("Tipo  : %s\n\n", figurinhas[sorteada].tipo);

        printf("Guardar no album? (s/n): ");
        fgets(opcao, sizeof(opcao), stdin);
        opcao[strcspn(opcao, "\n")] = '\0';

        printf("Resposta lida = [%s]\n", opcao);

        if(strcmp(opcao, "s") == 0){

            album[*total_album] = figurinhas[sorteada];//salva a figurinha no vetor album

            (*total_album)++;//incrementa a quantidade de figurinhas do album

            fprintf(arquivoalbum,"%s,%s,%s,%s,%s\n",figurinhas[sorteada].codigo, figurinhas[sorteada].titulo, figurinhas[sorteada].secao, figurinhas[sorteada].grupo, figurinhas[sorteada].tipo);
            //salva a figurinha no arquivo album.csv
            
            printf("Figurinha salva no album.\n\n");

        }else{

            printf("Guardar na mochila? (s/n): ");
            fgets(opcao, sizeof(opcao), stdin);
            opcao[strcspn(opcao, "\n")] = '\0';

            if(strcmp(opcao, "s") == 0){

                mochila[*total_mochila] = figurinhas[sorteada];//salva a figurinha no vetor mochila
                (*total_mochila)++;//incrementa a quantidade de figurinhas da mochila

                fprintf(arquivomochila,"%s,%s,%s,%s,%s\n", figurinhas[sorteada].codigo, figurinhas[sorteada].titulo, figurinhas[sorteada].secao, figurinhas[sorteada].grupo, figurinhas[sorteada].tipo);
                //salva a figurinha no arquivo mochila.csv

                printf("Figurinha salva na mochila.\n\n");

            }else{

                printf("Figurinha descartada.\n\n");

            }
        }//else
    }//for

    fclose(arquivoalbum);//fecha arquivo album
    fclose(arquivomochila);//fecha arquivo mochila

}//função para abrir um pacote de figurinhas, recebe o vetor de figurinhas e o total de figurinhas lidas do arquivo, sorteia 7 figurinhas aleatoriamente, exibe as informações de cada figurinha sorteada e pergunta ao usuário se deseja guardar a figurinha no album ou na mochila, salva a figurinha no vetor e no arquivo correspondente ou descarta a figurinha com base na resposta do usuário.