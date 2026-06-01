#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "figurinhas.h"

char opcao[10];//variavel para armazenar a resposta do usuário sobre guardar ou não a figurinha

void abrirPacote(Figurinha *figurinhas, int total){

    printf("Pacote com 7 figurinhas:\n\n");

    for (int i = 0; i < 7; i++) {

        int sorteada = rand() % total;

        printf("Codigo:  %s\n", figurinhas[sorteada].codigo);
        printf("Titulo: %s\n", figurinhas[sorteada].titulo);
        printf("Secao : %s\n", figurinhas[sorteada].secao);
        printf("Grupo : %s\n", figurinhas[sorteada].grupo);
        printf("Tipo  : %s\n\n", figurinhas[sorteada].tipo);

        FILE *arquivoalbum = fopen("album.csv", "ab");//abre o arquivo album para escrita, se não existir ele cria

        if (arquivoalbum == NULL) {
            printf("Erro ao abrir o arquivo do album.\n");
            return;
        }//teste de arquivo album

        FILE *arquivomochila = fopen("mochila.csv", "ab");//abre o arquivo mochila para escrita, se não existir ele cria

        if (arquivomochila == NULL) {
            printf("Erro ao abrir o arquivo da mochila.\n");
            return;
        }//teste de arquivo mochila

        for (int i = 0; i < total; i++) {

            if(strcmp(figurinhas[sorteada].codigo, figurinhas[i].codigo) == 0 || strcmp(figurinhas[sorteada].titulo, figurinhas[i].titulo) == 0 || strcmp(figurinhas[sorteada].secao, figurinhas[i].secao) == 0 || strcmp(figurinhas[sorteada].grupo, figurinhas[i].grupo) == 0 || strcmp(figurinhas[sorteada].tipo, figurinhas[i].tipo) == 0){
                printf("Figurinha repetida, deseja guardar-la na mochila? (s/n): \n");
                fgets(opcao, sizeof(opcao), stdin);
                opcao[strcspn(opcao,"\n")]='\0';
                setbuf(stdin, NULL);
                
                if(strcmp(opcao, "s") == 0){
                    fprintf(arquivomochila, "%s,%s,%s,%s,%s\n", figurinhas[sorteada].codigo, figurinhas[sorteada].titulo, figurinhas[sorteada].secao, figurinhas[sorteada].grupo, figurinhas[sorteada].tipo);
                    //escreve a figurinha repetida no arquivo mochila, cada campo separado por vírgula e cada figurinha em uma nova linha
                }else{
                    printf("Figurinha descartada.\n");
                }//teste de resposta do usuário para guardar ou descartar a figurinha repetida
                
                continue;

            }else{
                printf("Figurinha nova, deseja guardar-la no album? (s/n): \n");
                fgets(opcao, sizeof(opcao), stdin);
                opcao[strcspn(opcao,"\n")]='\0';
                setbuf(stdin, NULL);

                if(strcmp(opcao, "s") == 0){
                    fprintf(arquivoalbum, "%s,%s,%s,%s,%s\n", figurinhas[sorteada].codigo, figurinhas[sorteada].titulo, figurinhas[sorteada].secao, figurinhas[sorteada].grupo, figurinhas[sorteada].tipo);
                    //escreve a figurinha nova no arquivo album, cada campo separado por vírgula e cada figurinha em uma nova linha
                }else{
                    printf("Figurinha descartada.\n");
                }//teste de resposta do usuário para guardar ou descartar a figurinha nova

                break;
            }//teste de repetição da figurinha

        }//for para verificar se a figurinha é repetida ou nova

        fclose(arquivomochila);//fecha o arquivo mochila

        fclose(arquivoalbum);//fecha o arquivo album

    }//for

}//função abrirPacote
