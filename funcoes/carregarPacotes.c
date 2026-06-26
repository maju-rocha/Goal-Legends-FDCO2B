#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"
#include "global.h"
#include "carregarPacotes.h"

/*
 * Inicializa o progresso salvo do jogador assim que o jogo começa.
 * Lê o arquivo binário onde a quantidade de pacotes foi guardada 
 * na última sessão e chama as funções que carregam as figurinhas 
 * coladas no álbum e as repetidas na mochila.
 */
void carregarPacotes(){

    FILE *arquivo_carrega_pacotes = fopen("extras/pacotes.bin", "rb");//Lê em binário

    // Se o arquivo for NULL, significa que o jogador abriu o jogo pela primeira vez
    // ou que o arquivo foi apagado. 
    if(arquivo_carrega_pacotes == NULL){
        pacotes_fechados = 0;//Se o arquivo não existir, começa com 0 pacotes
        return;
    }//if

    if(fread(&pacotes_fechados, sizeof(int), 1, arquivo_carrega_pacotes) != 1){
        pacotes_fechados = 0;//Se não conseguir ler, começa com 0 pacotes
    }//if

    fclose(arquivo_carrega_pacotes);//Fecha o arquivo

}//void
