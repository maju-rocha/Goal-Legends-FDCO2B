#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"

void salvarPacotes(){
    FILE *f = fopen("extras/pacotes.txt", "w");

    if(f == NULL){
        return;   
    }//if

    fprintf(f, "%d", pacotes_fechados);
    fclose(f);
}