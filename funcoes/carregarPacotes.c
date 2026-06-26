#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"
#include "global.h"
#include "carregarPacotes.h"

void carregarPacotes(){

    FILE *arquivo_carrega_pacotes = fopen("extras/pacotes.bin", "rb");//Lê em binário

    if(arquivo_carrega_pacotes == NULL){
        pacotes_fechados = 0;//Se o arquivo não existir, começa com 0 pacotes
        return;
    }//if

    if(fread(&pacotes_fechados, sizeof(int), 1, arquivo_carrega_pacotes) != 1){
        pacotes_fechados = 0;//Se não conseguir ler, começa com 0 pacotes
    }//if

    fclose(arquivo_carrega_pacotes);//Fecha o arquivo

}//void