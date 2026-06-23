#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"

void trocarFigurinha(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int *total_mochila, int *total_album){

    if(figurinha_repetida >= 14){
        //Aumenta um pacote geral e salva na quantidade geral 
        pacotes_fechados++;
        salvarPacotes();
    
        //Váriaveis de arquivo
        FILE *arquivo_mochila = fopen("extras/mochila.csv", "r");
        FILE *temp = fopen("temp.csv", "w");
    
        //Váriaveis
        char linha[1024];
        int linhaAtual = 0;
    
        //Lê até 14 linhas do arquivo mochila
        while(fgets(linha, sizeof(linha), arquivo_mochila)){
            linhaAtual++;
    
            if(linhaAtual <= 14){
                continue;
            }//if
    
            fputs(linha, temp);
        }//while
    
        //Fecha os arquivos
        fclose(arquivo_mochila);
        fclose(temp);
    
        //Reescreve por cima a pasta "nova"
        remove("extras/mochila.csv");
        rename("temp.csv", "extras/mochila.csv");
    
        //Remove a quantidade de figurinhas de troca e salva a quantidade de figurinhas repetidas
        figurinha_repetida -= 14;
        salvarRepetida();
    }else{
        printf("Você não tem figurinhas suficientes para trocar\n");
    }//if
    
}
