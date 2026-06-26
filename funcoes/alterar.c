#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "alterar.h"

//Funcao para limpar espacos antes e depois do texto
static void limparCampoAlterar(char *texto){

    int inicio = 0;//Váriavel de começo
    int fim = strlen(texto) - 1;//Variável para o fim do texto

    //Confere se texto é um espaço vazio, caso seja adiciona uma casa
    while(texto[inicio] == ' '){
        inicio++;
    }//while

    //Confere se inicio chegou no fim e é um espaço vazio
    while(fim >= inicio && texto[fim] == ' '){
        texto[fim] = '\0';
        fim--;
    }//while

    if(inicio > 0){
        int j = 0;//Variável para marcar a posição do texto corrigido

        //Começa no primeiro caracter válido
        for(int i = inicio; texto[i] != '\0'; i++){
            texto[j] = texto[i];//Copia para próxima posição
            j++;
        }//for

        texto[j] = '\0';//Fim da string
    }//if
}//void

//Funcao para limpar o ENTER que sobra no teclado
static void limparBufferAlterar(){

    int c;

    while((c = getchar()) != '\n' && c != EOF);//Exclui o texto digitado anteriormente e le todos os caracteres até o final da string

}//void

//Função principal
void alterarFigurinha(Figurinha *vetor, int total){

    char codigoBusca[10];//Variável de busca
    int encontrada = 0;//Variável de condição

    //Scanf do terminal
    printf("\n--- Alterar Figurinha ---\n");
    printf("Digite o codigo da figurinha que deseja alterar: ");
    scanf(" %6[^\n]", codigoBusca);
    limparBufferAlterar();

    limparCampoAlterar(codigoBusca);//Chama função criada anteriormente em relação a variável de busca

    for(int i = 0; i < total; i++){

        char codigo_limpo[15];//Variável para trocar

        strcpy(codigo_limpo, vetor[i].codigo);//Copia a variável de troca e a de busca
        limparCampoAlterar(codigo_limpo);//Chama função criada anteriormente relação a variável de troca

        //Compara e escreve no terminal e no arquivo figurinhas2026copy.csv
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

            FILE *arquivo = fopen("extras/figurinhas2026copy.csv", "w");//Escreve no arquivo figurinhas2026.csv

            if(arquivo == NULL){
                printf("\nErro ao abrir o arquivo para salvar alteracao.\n");
                return;
            }//if teste

            //Limpa os espaços de cada parte
            for(int k = 0; k < total; k++){

                limparCampoAlterar(vetor[k].codigo);
                limparCampoAlterar(vetor[k].titulo);
                limparCampoAlterar(vetor[k].secao);
                limparCampoAlterar(vetor[k].grupo);
                limparCampoAlterar(vetor[k].tipo);

                fprintf(arquivo, "%-6s, %-28s, %-19s, %-7s, %s\n",vetor[k].codigo,vetor[k].titulo,vetor[k].secao,vetor[k].grupo,vetor[k].tipo);//Escreve no arquivo de maneira formatada
            }//for

            fclose(arquivo);//Fecha o arquivo

            printf("\nFigurinha alterada com sucesso!\n");//Texto terminal

            break;
        }//if
    }//for

    //Segunda opção da condição
    if(!encontrada){
        printf("\nFigurinha nao encontrada para alteracao.\n");
    }//if

}