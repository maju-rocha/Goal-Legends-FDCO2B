#include <stdio.h>
#include <stdlib.h>
#include "biblioteca.h"

void trocarFigurinha(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int *total_mochila, int *total_album){

    FILE *arquivo_mochila = fopen("extras/mochila.csv", "r");

    if(arquivo_mochila == NULL){
        printf("Erro ao abrir o arquivo extras/mochila.csv\n");
        return;
    }//if

    char linha[1024];
    int totalLinhasArquivo = 0;

    //Conta quantas figurinhas existem realmente na mochila
    while(fgets(linha, sizeof(linha), arquivo_mochila)){
        totalLinhasArquivo++;
    }//while

    fclose(arquivo_mochila);

    //Atualiza o total da mochila na memoria
    *total_mochila = totalLinhasArquivo;
    figurinha_repetida = totalLinhasArquivo;

    if(*total_mochila >= 14){

        int pacotesPossiveis = *total_mochila / 14;
        int pacotesDesejados = 0;
        int c;

        printf("\nVoce tem %d figurinhas repetidas na mochila.\n", *total_mochila);
        printf("Cada pacote custa 14 figurinhas repetidas.\n");
        printf("Voce pode trocar por ate %d pacote(s).\n", pacotesPossiveis);
        printf("Quantos pacotes deseja trocar? ");

        if(scanf("%d", &pacotesDesejados) != 1){
            pacotesDesejados = 0;
        }//if

        while((c = getchar()) != '\n' && c != EOF);

        if(pacotesDesejados <= 0){
            printf("Troca cancelada.\n");
            return;
        }//if

        if(pacotesDesejados > pacotesPossiveis){
            printf("Voce nao tem figurinhas suficientes para trocar essa quantidade.\n");
            printf("Quantidade maxima possivel: %d pacote(s).\n", pacotesPossiveis);
            return;
        }//if

        int figurinhasParaRemover = pacotesDesejados * 14;

        //Aumenta a quantidade de pacotes fechados
        pacotes_fechados += pacotesDesejados;
        salvarPacotes();

        //Abre novamente a mochila para remover as figurinhas usadas
        arquivo_mochila = fopen("extras/mochila.csv", "r");

        if(arquivo_mochila == NULL){
            printf("Erro ao abrir o arquivo extras/mochila.csv\n");
            return;
        }//if

        FILE *temp = fopen("temp.csv", "w");

        if(temp == NULL){
            printf("Erro ao criar o arquivo temporario\n");
            fclose(arquivo_mochila);
            return;
        }//if

        int linhaAtual = 0;

        //Remove as primeiras figurinhas usadas na troca
        while(fgets(linha, sizeof(linha), arquivo_mochila)){
            linhaAtual++;

            if(linhaAtual <= figurinhasParaRemover){
                continue;
            }//if

            fputs(linha, temp);
        }//while

        fclose(arquivo_mochila);
        fclose(temp);

        remove("extras/mochila.csv");
        rename("temp.csv", "extras/mochila.csv");

        //Atualiza o total da mochila na memoria
        *total_mochila -= figurinhasParaRemover;

        //Atualiza a quantidade de repetidas
        figurinha_repetida = *total_mochila;
        salvarRepetida();

        printf("\nTroca realizada com sucesso!\n");
        printf("Voce recebeu %d pacote(s) novo(s).\n", pacotesDesejados);
        printf("Foram removidas %d figurinhas repetidas da mochila.\n", figurinhasParaRemover);
        printf("Figurinhas restantes na mochila: %d\n", *total_mochila);

    }else{

        printf("Voce nao tem figurinhas suficientes para trocar.\n");
        printf("Voce tem apenas %d figurinhas na mochila.\n", *total_mochila);
        printf("Sao necessarias 14 figurinhas repetidas para trocar por 1 pacote.\n");

    }//if

}//void