#include <stdio.h>
#include <stdlib.h>
#include "salvarRepetida.h"

void salvarRepetida(){
    FILE *f = fopen("extras/repetidas.txt", "w");

    if(f == NULL){
        return;   
    }//if

    fprintf(f, "%d", pacotes_fechados);
    fclose(f);
}