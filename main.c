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

    srand((int)time(NULL));

    char linha[300];
    int total = 0;
    int total_album = 0;
    int total_mochila = 0;
        
    Figurinha *figurinhas = malloc(981 * sizeof(Figurinha));
    Figurinha *album = malloc(981 * sizeof(Figurinha));
    Figurinha *mochila = malloc(981 * sizeof(Figurinha));
    
    if(figurinhas == NULL || album == NULL || mochila == NULL){
        printf("Erro de alocacao.\n"); 

        free(figurinhas);
        free(album);
        free(mochila);

        return 1;
    }

    FILE *arquivo = fopen("extras/figurinhas2026.csv", "r");

    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo figurinhas2026.csv.\n");

        free(figurinhas);
        free(album);
        free(mochila);

        return 1; 
    }

    fgets(linha, sizeof(linha), arquivo); 

    while(total < 981 && fscanf(arquivo, " %9[^,],%49[^,],%49[^,],%49[^,],%49[^\n]",
        figurinhas[total].codigo,
        figurinhas[total].titulo,
        figurinhas[total].secao,
        figurinhas[total].grupo,
        figurinhas[total].tipo) == 5){

        total++;
    }

    fclose(arquivo);

    carregarPacotes();
    carregarRepetida();

    menuPrincipal(figurinhas, album, mochila, total, &total_album, &total_mochila);

    free(figurinhas);
    free(album);
    free(mochila);
    
    return 0;
}