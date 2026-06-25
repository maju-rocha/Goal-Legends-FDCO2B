#include <stdio.h>
#include <stdlib.h>
#include "salvarPacotes.h"

void salvarPacotes(){
    FILE *arquivo_salvar_pacotes = fopen("extras/pacotes.txt", "w");

    if(arquivo_salvar_pacotes == NULL){
        return;   
    }//if

    fprintf(arquivo_salvar_pacotes, "%d", pacotes_fechados);
    fclose(arquivo_salvar_pacotes);
}