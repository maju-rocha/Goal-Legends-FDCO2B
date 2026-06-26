#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <raylib.h>
#include "biblioteca.h"
#include "carregarPacotes.h"
#include "carregarRepetida.h"
#include "menu.h"
#include "global.h"

int main(void){

    srand((int)time(NULL));//Inicializa a aleatoriedade

    //Variáveis
    char linha[300];
    int total = 0;
    int total_album = 0;
    int total_mochila = 0;

    //Inicia vetores dinamicos
    Figurinha *figurinhas = malloc(1100 * sizeof(Figurinha));
    Figurinha *album = malloc(1100 * sizeof(Figurinha));\
    Figurinha *mochila = malloc(1100 * sizeof(Figurinha));

    if(figurinhas == NULL || album == NULL || mochila == NULL){
        printf("Erro de alocacao.\n"); 

        free(figurinhas);
        free(album);
        free(mochila);

        return 1;
    }//if teste

    FILE *arquivo = fopen("extras/figurinhas2026copy.csv", "r");;//Abre em modo de leitura o arquivo figurinhas202copy.csv

    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo figurinhas2026.csv.\n");

        free(figurinhas);
        free(album);
        free(mochila);

        return 1; 
    }//if teste

    fgets(linha, sizeof(linha), arquivo); 

    while(total < 1100 && fscanf(arquivo, " %9[^,],%49[^,],%49[^,],%49[^,],%49[^\n]", figurinhas[total].codigo, figurinhas[total].titulo, figurinhas[total].secao, figurinhas[total].grupo,figurinhas[total].tipo) == 5){

        total++;
    }

    fclose(arquivo);//Fecha Pacotes

    //Carrega os pacotes e figurinhas repetidas
    carregarPacotes();
    carregarRepetida();

    printf("%d",pacotes_fechados);

    menuPrincipal(figurinhas, album, mochila, total, &total_album, &total_mochila);//Abre o menu

    //Libera memória dos vetores dinâmicos
    free(figurinhas);
    free(album);
    free(mochila);
    
    return 0;
}