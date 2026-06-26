#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
<<<<<<< HEAD
#include <raylib.h>
#include "biblioteca.h"
#include "carregarPacotes.h"
#include "carregarRepetida.h"
#include "menu.h"
#include "global.h"
=======
#include "figurinhas.h"
>>>>>>> 72b6c5f8a5c94268b52d68a12dcf6e0f8ccf7ad4

int main(){

<<<<<<< HEAD
    srand((int)time(NULL));//Inicializa a aleatoriedade

    //Variáveis
    char linha[300];
    int total = 0;
    int total_album = 0;
    int total_mochila = 0;

    //Inicia vetores dinamicos
    Figurinha *figurinhas = malloc(1100 * sizeof(Figurinha));
    Figurinha *album = malloc(1100 * sizeof(Figurinha));\
    Figurinha *mochila = malloc(1100 * sizeof(Figurinha));

    if(figurinhas == NULL || album == NULL || mochila == NULL){
        printf("Erro de alocacao.\n"); 

        free(figurinhas);
        free(album);
        free(mochila);

        return 1;
    }//if teste

    FILE *arquivo = fopen("extras/figurinhas2026copy.csv", "r");;//Abre em modo de leitura o arquivo figurinhas202copy.csv

    if(arquivo == NULL){
        printf("Erro ao abrir o arquivo figurinhas2026.csv.\n");

        free(figurinhas);
        free(album);
        free(mochila);

        return 1; 
    }//if teste

    fgets(linha, sizeof(linha), arquivo); 

    while(total < 1100 && fscanf(arquivo, " %9[^,],%49[^,],%49[^,],%49[^,],%49[^\n]", figurinhas[total].codigo, figurinhas[total].titulo, figurinhas[total].secao, figurinhas[total].grupo,figurinhas[total].tipo) == 5){

        total++;
    }

    fclose(arquivo);//Fecha Pacotes

    //Carrega os pacotes e figurinhas repetidas
    carregarPacotes();
    carregarRepetida(); 

    menuPrincipal(figurinhas, album, mochila, total, &total_album, &total_mochila);//Abre o menu

    //Libera memória dos vetores dinâmicos
    free(figurinhas);
    free(album);
    free(mochila);
=======
    char linha[300];//variavel com todos os bytes de uma linha
    int total = 0;//variavel para contabilizar a quantidade


    Figurinha *figurinhas = malloc (981 * sizeof(Figurinha));//Cria vetor dinamico para as figurinhas

    if (figurinhas == NULL){//testa se o malloc funcionou
        printf("Erro de alocacao.\n");
        return 1;
    }

    FILE *arquivo = fopen("figurinhas2026.csv","r");//cria e abre o arquivo para leitura de figurinhas
>>>>>>> 72b6c5f8a5c94268b52d68a12dcf6e0f8ccf7ad4
    
    if (arquivo == NULL){//testa se o arquivo abriu
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fgets(linha, sizeof(linha), arquivo);//ignora a primeira linha do arquivo figurinhas2026.csv

    while (fscanf(arquivo," %9[^,],%49[^,],%49[^,],%49[^,],%49[^\n]",figurinhas[total].codigo,figurinhas[total].titulo,figurinhas[total].secao,figurinhas[total].grupo,figurinhas[total].tipo) == 5){
       
        total++;//
    }//le uma linha, se conseguiu ler os 5 espaços (codigo,titulo,secao,grupo e tipo) incrementa no total e assim vai pelo arquivo inteiro

    fclose(arquivo);//fecha arquivo

    srand(time(NULL));//libera aleatoriedade para funcao abrirPacote

    abrirPacote(figurinhas, total);//chama funcao abrirPacote

    free(figurinhas);//Libera a memória do vetor figurinhas
    return 0;
}