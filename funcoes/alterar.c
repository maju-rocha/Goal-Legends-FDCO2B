#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"

//Funcao para limpar espacos antes e depois do texto
static void limparCampoAlterar(char *texto){

    int inicio = 0;
    int fim = strlen(texto) - 1;

    while(texto[inicio] == ' '){
        inicio++;
    }//while

    while(fim >= inicio && texto[fim] == ' '){
        texto[fim] = '\0';
        fim--;
    }//while

    if(inicio > 0){

        int j = 0;

        for(int i = inicio; texto[i] != '\0'; i++){
            texto[j] = texto[i];
            j++;
        }//for

        texto[j] = '\0';

    }//if

}//void

//Funcao para limpar o ENTER que sobra no teclado
static void limparBufferAlterar(){

    int c;

    while((c = getchar()) != '\n' && c != EOF);

}//void

void alterarFigurinha(Figurinha *vetor, int total){

    char codigoBusca[10];
    int encontrada = 0;

    printf("\n--- Alterar Figurinha ---\n");
    printf("Digite o codigo da figurinha que deseja alterar: ");
    scanf(" %6[^\n]", codigoBusca);
    limparBufferAlterar();

    limparCampoAlterar(codigoBusca);

    for(int i = 0; i < total; i++){

        char codigo_limpo[15];

        strcpy(codigo_limpo, vetor[i].codigo);
        limparCampoAlterar(codigo_limpo);

        if(strcmp(codigo_limpo, codigoBusca) == 0){

            printf("\nFigurinha Encontrada: %s - %s\n", vetor[i].codigo, vetor[i].titulo);

            printf("Digite o novo Titulo: ");
            scanf(" %28[^\n]", vetor[i].titulo);
            limparBufferAlterar();
            limparCampoAlterar(vetor[i].titulo);

            printf("Digite a nova Secao: ");
            scanf(" %19[^\n]", vetor[i].secao);
            limparBufferAlterar();
            limparCampoAlterar(vetor[i].secao);

            printf("Digite o novo Tipo: ");
            scanf(" %8[^\n]", vetor[i].tipo);
            limparBufferAlterar();
            limparCampoAlterar(vetor[i].tipo);

            encontrada = 1;

            FILE *arquivo = fopen("extras/figurinhas2026copy.csv", "w");

            if(arquivo == NULL){
                printf("\nErro ao abrir o arquivo para salvar alteracao.\n");
                return;
            }//if

            for(int k = 0; k < total; k++){

                limparCampoAlterar(vetor[k].codigo);
                limparCampoAlterar(vetor[k].titulo);
                limparCampoAlterar(vetor[k].secao);
                limparCampoAlterar(vetor[k].grupo);
                limparCampoAlterar(vetor[k].tipo);

                fprintf(arquivo, "%-6s, %-28s, %-19s, %-7s, %s\n",
                        vetor[k].codigo,
                        vetor[k].titulo,
                        vetor[k].secao,
                        vetor[k].grupo,
                        vetor[k].tipo);

            }//for

            fclose(arquivo);

            printf("\nFigurinha alterada com sucesso!\n");

            break;

        }//if

    }//for

    if(!encontrada){
        printf("\nFigurinha nao encontrada para alteracao.\n");
    }//if

}//funcao para alterar uma linha especifica no figurinhas2026copy.csv