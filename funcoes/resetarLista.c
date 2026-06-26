#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resetarLista.h"

//Funcao para limpar espacos antes e depois do texto
static void limparCampoResetar(char *texto){

    int inicio = 0;//Váriavel de começo
    int fim = strlen(texto) - 1;//Variável para o fim do texto

    //Confere se texto é um espaço vazio, caso seja adiciona uma casa
    while(texto[inicio] == ' ' || texto[inicio] == '\t'){
        inicio++;
    }//while

    //Confere se inicio chegou no fim e é um espaço vazio
    while(fim >= inicio && (texto[fim] == ' ' || texto[fim] == '\t' || texto[fim] == '\n' || texto[fim] == '\r')){
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

//Funcao para copiar cada campo separado por virgula
static int copiarCampoResetar(char *linha, int posicao, char *destino){

    int j = 0;//Variável para posição do destino

    //Copia os caracteres até encontrar vírgula, fim da linha ou fim da string
    while(linha[posicao] != ',' && linha[posicao] != '\n' && linha[posicao] != '\0'){
        destino[j] = linha[posicao];
        j++;
        posicao++;
    }//while

    destino[j] = '\0';//Fim da string

    limparCampoResetar(destino);//Limpa espaços antes e depois do campo

    if(linha[posicao] == ','){
        posicao++;
    }//if

    return posicao;

}

void resetarLista(Figurinha *vetor, int total){

    FILE *arquivo = fopen("extras/figurinhas2026.csv", "r");//Le arquivo figurinhas2026.csv
    FILE *arquivo_copia = fopen("extras/figurinhas2026copy.csv", "w");//Escreve no figurinhas2026copy.csv

    char linha[1024];//Variável com todos os caracteres de uma linha
    int i = 0;//Variável para controlar a posição do vetor

    if(arquivo == NULL){
        printf("\nErro ao abrir o arquivo original para resetar a lista.\n");
        return;
    }//if teste

    if(arquivo_copia == NULL){
        printf("\nErro ao abrir o arquivo de copia para resetar a lista.\n");
        fclose(arquivo);//Fecha o arquivo
        return;
    }//if teste

    //Copia o conteúdo de um arquivo para outro
    while(fgets(linha, sizeof(linha), arquivo) && i < total){

        fputs(linha, arquivo_copia);//Copia a linha do arquivo original para o arquivo de copia

        int posicao = 0;//Variável para posição dentro da linha

        //Atualiza o vetor na
        posicao = copiarCampoResetar(linha, posicao, vetor[i].codigo);
        posicao = copiarCampoResetar(linha, posicao, vetor[i].titulo);
        posicao = copiarCampoResetar(linha, posicao, vetor[i].secao);
        posicao = copiarCampoResetar(linha, posicao, vetor[i].grupo);
        posicao = copiarCampoResetar(linha, posicao, vetor[i].tipo);

        i++;
    }//while

    //Fecha os arquivos
    fclose(arquivo);
    fclose(arquivo_copia);

    printf("\nLista resetada com sucesso!\n");//Texto terminal

}//funcao para resetar a lista de figurinhas