#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"
#include "global.h"
#include "carregarPacotes.h"
#include "carregarAlbum.h"
#include "carregarMochila.h"

/*
 * Inicializa o progresso salvo do jogador assim que o jogo começa.
 * Lê o arquivo binário onde a quantidade de pacotes foi guardada 
 * na última sessão e chama as funções que carregam as figurinhas 
 * coladas no álbum e as repetidas na mochila.
 */
void carregarPacotes(Figurinha *album, int *total_album, Figurinha *mochila, int *total_mochila){

    // Abre o arquivo de salvamento dos pacotes em modo de leitura binária 
    FILE *arquivo_carrega_pacotes = fopen("extras/pacotes.bin", "rb");

    // Se o arquivo for NULL, significa que o jogador abriu o jogo pela primeira vez
    // ou que o arquivo foi apagado. 
    if(arquivo_carrega_pacotes == NULL){
        pacotes_fechados = 0; //garante que ele começa com 0 pacotes.
    }
    else {
        // Se o arquivo existir, lê 1 bloco do tamanho de um 'int' (4 bytes)
        // e guarda esse valor lido diretamente na variável global 'pacotes_fechados'.
        fread(&pacotes_fechados, sizeof(int), 1, arquivo_carrega_pacotes);
        
        // Sempre que abrir um arquivo, fecha com fclose 
        // para libertar a memória e não corromper os dados.
        fclose(arquivo_carrega_pacotes);
    }

    // Após carregar a quantidade de pacotes, carrega as cartas
    // para as funções para manter o código organizado.
    carregarAlbum(album, total_album);
    carregarMochila(mochila, total_mochila);
}