#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"
#include "salvarAlbum.h"
#include "excluirAlbum.h"

void excluirAlbum(Figurinha *figurinhas, Figurinha *album, int *total_album){

    (void)figurinhas;//Variável não usada nessa função

    char codigo[10];//Variável para armazenar o código do usuário

    //Fgets para o código do usuário
    printf("Digite o código da figurinha que deseja excluir: \n");
    fgets(codigo, sizeof(codigo), stdin);
    codigo[strcspn(codigo, "\n")] = '\0';

    for(int i = 0; i < *total_album; i++){

        char codigo_album[10];//Variável para armazenar o código da figurinha sem espaços

        strcpy(codigo_album, album[i].codigo);//Copia o código da figurinha do álbum para a variável de busca
       
        //Remove os espaços em branco do final do código
        for(int k = strlen(codigo_album) - 1; k >= 0 && codigo_album[k] == ' '; k--){
            codigo_album[k] = '\0';
        }//for

        printf("Comparando '%s' com '%s'\n", codigo, codigo_album);//Texto terminal

        if(strcmp(codigo, codigo_album) == 0){

            printf("\nFIGURINHA ENCONTRADA!\n");//Texto terminal

            //Move as figurinhas seguintes para preencher o espaço da figurinha excluída
            for(int j = i; j < *total_album - 1; j++){
                album[j] = album[j + 1];
            }//for

            (*total_album)--;//Diminui o total do álbum

            salvarAlbum(album, *total_album);//Chama função para salvar no album.bin

            printf("Novo total: %d\n", *total_album);//Texto terminal
            printf("\nFigurinha excluida com sucesso.\n");//Texto terminal

            return;

        }//if

    }//for

    printf("\nFigurinha com o código %s não encontrada.\n", codigo);//Texto terminal

}//função para excluir uma figurinha do vetor album