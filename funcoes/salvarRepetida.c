#include <stdio.h>
#include <stdlib.h>
#include "salvarRepetida.h"

void salvarRepetida(){
    FILE *f = fopen("extras/repetidas.bin", "wb");//Salva a quantidade de repetidas no arquivo repetidas.bin

    if(f == NULL){
        return;   
    }//if teste

    fprintf(f, "%d", pacotes_fechados);//Escreve a quantidade de pacotes no arquivo
    fclose(f);//Fecha o arquivo
}