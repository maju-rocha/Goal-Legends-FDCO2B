#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"
#include "global.h"
#include "salvarPacotes.h"

void salvarPacotes(){

    FILE *arquivo_salvar_pacotes = fopen("extras/pacotes.bin", "wb");

    if(arquivo_salvar_pacotes == NULL){
        return;
    }

    fwrite(&pacotes_fechados, sizeof(int), 1, arquivo_salvar_pacotes);

    fclose(arquivo_salvar_pacotes);
}