#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"

void carregarPacotes(){
    FILE *arquivo_carrega_pacotes = fopen("extras/pacotes.txt", "r");

    if(arquivo_carrega_pacotes == NULL){
        pacotes_fechados = 0; // valor padrão
        return;
    }//if

    fscanf(arquivo_carrega_pacotes, "%d", &pacotes_fechados);
    fclose(arquivo_carrega_pacotes);
}