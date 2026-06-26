#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"
#include "global.h"
#include "carregarRepetida.h"

void carregarRepetida(){

    FILE *arquivo = fopen("extras/repetidas.bin", "rb");//Le em modo binário o arquivo repetidas.bin

    if(arquivo == NULL){
        figurinha_repetida = 0;
        return;
    }//if teste

    fread(&figurinha_repetida, sizeof(int), 1, arquivo);//Le a quantidade de figurinhas repetidas no arquivo

    fclose(arquivo);//Fecha o arquivo
}