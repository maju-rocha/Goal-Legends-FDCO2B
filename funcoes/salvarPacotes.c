#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"
#include "global.h"
#include "salvarPacotes.h"

void salvarPacotes(){

    FILE *arquivo_salvar_pacotes = fopen("extras/pacotes.bin", "wb");//Salva em binário

    if(arquivo_salvar_pacotes == NULL){
        return;
    }//if teste

    fwrite(&pacotes_fechados, sizeof(int), 1, arquivo_salvar_pacotes);//Salva a quantidade de pacotes em binário

    fclose(arquivo_salvar_pacotes);//Fecha arquivo

}//void