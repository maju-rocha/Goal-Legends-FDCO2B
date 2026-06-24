#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"

//Função para comparar se está no album
int estaNoAlbum(Figurinha *album, int total_album, char codigo[]){
    for(int i = 0; i < total_album; i++) {
        if(strcmp(album[i].codigo, codigo) == 0) {
            return 1;
        }
    }
    return 0;
}

void abrirPacote(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, int *pacotes_fechados){

    //Verifica se tem pacotes para abrir
    if(*pacotes_fechados <= 0){
        printf("OPSSS! Voce nao tem pacotes fechados no inventario!\n");
        printf("Jogue os Minigames da Copa para conquistar recompensas.\n");
        return;
    }

    //Váriaveis
    int qtd_abrir = 0;
    char buffer[100];
    char codigo_figurinha[20];

    //Pergunta a quantidade que deseja abrir
    printf("Voce tem %d pacote(s) disponivel(is).\n", *pacotes_fechados);
    printf("Quantos pacotes deseja abrir de uma vez? ");
    
    fgets(buffer, sizeof(buffer), stdin);
    sscanf(buffer, "%d", &qtd_abrir); // Converte o texto digitado para número

    //Verificações de teste
    if(qtd_abrir <= 0){
        printf("\nQuantidade invalida. Operacao cancelada.\n");
        return;
    }

    if(qtd_abrir > *pacotes_fechados){
        printf("\nVoce nao tem pacotes suficientes! Tentou abrir %d, mas so tem %d.\n", qtd_abrir, *pacotes_fechados);
        return;
    }

    //Diminui quantidade de pacotes geral e abre uma figurinha
    *pacotes_fechados -= qtd_abrir;
    salvarPacotes();
    int total_figurinhas = qtd_abrir * 7;

    //Abertura de arquivo .csv do album e mochila
    FILE *arquivoalbum = fopen("extras/album.csv", "a");
    if(arquivoalbum == NULL){
        printf("\nErro ao abrir album.csv\n");
        return;
    }

    FILE *arquivomochila = fopen("extras/mochila.csv", "a");
    if(arquivomochila == NULL){
        printf("\nErro ao abrir mochila.csv\n");
        fclose(arquivoalbum);
        return;
    }

    printf("\n>>> Abrindo %d pacote(s)... Voce tirou %d figurinhas!\n\n", qtd_abrir, total_figurinhas);

    //Loop para sortear todas as figurinhas escolhidas
    for(int i = 0; i < total_figurinhas; i++){
        int sorteada = rand() % total;

        printf("--- Figurinha %d de %d ---\n", i + 1, total_figurinhas);
        printf("Codigo: %s\n", figurinhas[sorteada].codigo);
        printf("Titulo: %s\n", figurinhas[sorteada].titulo);
        printf("Secao : %s\n", figurinhas[sorteada].secao);
        printf("Grupo : %s\n", figurinhas[sorteada].grupo);
        printf("Tipo  : %s\n\n", figurinhas[sorteada].tipo);

        //Condição para ver está no album, caso não coloca na mochila
        if(!estaNoAlbum(album, *total_album, figurinhas[sorteada].codigo)){

            
            album[*total_album] = figurinhas[sorteada];
            (*total_album)++;
            fprintf(arquivoalbum,"%s,%s,%s,%s,%s\n", figurinhas[sorteada].codigo, figurinhas[sorteada].titulo, figurinhas[sorteada].secao, figurinhas[sorteada].grupo, figurinhas[sorteada].tipo);

            printf("=> NOVA! Figurinha adicionada ao album.\n\n");

        }else{

            mochila[*total_mochila] = figurinhas[sorteada];
            (*total_mochila)++;

            figurinha_repetida++;
            salvarRepetida();

            fprintf(arquivomochila,"%s,%s,%s,%s,%s\n", figurinhas[sorteada].codigo, figurinhas[sorteada].titulo, figurinhas[sorteada].secao, figurinhas[sorteada].grupo, figurinhas[sorteada].tipo);

            printf("=> REPETIDA! Figurinha enviada para a mochila.\n\n");
        }
    }//for
    fclose(arquivoalbum);
    fclose(arquivomochila);
}//função