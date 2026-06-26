#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"
#include "global.h"
#include "carregarRepetida.h"

void carregarRepetida(){

    FILE *arquivo = fopen("extras/repetidas.bin", "rb");

    if(arquivo == NULL){
        figurinha_repetida = 0;
        return;
    }

    fread(&figurinha_repetida, sizeof(int), 1, arquivo);

    fclose(arquivo);
}