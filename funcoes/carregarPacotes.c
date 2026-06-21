#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"

void carregarPacotes(){
    FILE *f = fopen("extras/pacotes.txt", "r");

    if(f == NULL){
        pacotes_fechados = 0; // valor padrão
        return;
    }//if

    fscanf(f, "%d", &pacotes_fechados);
    fclose(f);
}