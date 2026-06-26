#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"
#include "global.h"
#include "carregarPacotes.h"

void carregarPacotes(){

    FILE *arquivo_carrega_pacotes = fopen("extras/pacotes.txt", "r");//Lê o arquivo pacotes.txt

    if(arquivo_carrega_pacotes == NULL){
        pacotes_fechados = 0;//Se o arquivo não existir, começa com 0 pacotes
        return;
    }//if

    fscanf(arquivo_carrega_pacotes, "%d", &pacotes_fechados);//Lê a quantidade de pacotes fechados

    fclose(arquivo_carrega_pacotes);//Fecha o arquivo

}//void