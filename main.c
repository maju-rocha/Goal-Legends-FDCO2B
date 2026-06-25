#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
<<<<<<< HEAD
#include "figurinhas.h"

int main(){

    char linha[300];//variavel com todos os bytes de uma linha
    int total = 0;//variavel para contabilizar a quantidade


    Figurinha *figurinhas = malloc (981 * sizeof(Figurinha));//Cria vetor dinamico para as figurinhas

    if (figurinhas == NULL){//testa se o malloc funcionou
        printf("Erro de alocacao.\n");
        return 1;
    }

    FILE *arquivo = fopen("figurinhas2026.csv","r");//cria e abre o arquivo para leitura de figurinhas
    
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
=======
#include <raylib.h>
#include "biblioteca.h"
#include "carregarPacotes.h"
#include "menu.h"

int main(void){

    srand((int)time(NULL));

    //Váriaveis
    char linha[300];
    int total = 0;
    int total_album = 0;
    int total_mochila = 0;
        
    //Ponteiros para vetores
    Figurinha *figurinhas = malloc(981 * sizeof(Figurinha));
    Figurinha *album = malloc(981 * sizeof(Figurinha));
    Figurinha *mochila = malloc(981 * sizeof(Figurinha));
    
    //Verificação de alocação
    if(figurinhas == NULL || album == NULL || mochila == NULL){
        printf("Erro de alocacao.\n"); 
        return 1;
    }//

    //Abertura de arquivo para escrita em modo binário
    FILE *arquivo = fopen("extras/figurinhas2026.csv","rb");
    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo figurinhas2026.csv.\n"); 
        return 1; 
    }//if teste

    //Abertura de arquivo_album para uso ou exportação/escrita em modo padrão
    FILE *arquivo_album = fopen("extras/album.csv", "r");
    if(arquivo_album != NULL){
        while(fscanf(arquivo_album," %9[^,],%49[^,],%49[^,],%49[^,],%49[^\n]", album[total_album].codigo, album[total_album].titulo, album[total_album].secao, album[total_album].grupo,album[total_album].tipo) == 5){
            total_album++;
        }
        fclose(arquivo_album);
    }//if teste

    //Abertura de arquivo_mochila para uso ou exportação/escrita em modo padrão
    FILE *arquivo_mochila = fopen("extras/mochila.csv", "r");
    if(arquivo_mochila != NULL){
        while(fscanf(arquivo_mochila, " %9[^,],%49[^,],%49[^,],%49[^,],%49[^\n]", mochila[total_mochila].codigo, mochila[total_mochila].titulo, mochila[total_mochila].secao, mochila[total_mochila].grupo, mochila[total_mochila].tipo) == 5){
            total_mochila++;
        }
        fclose(arquivo_mochila);
    }//if teste

    //Leitura por lina do arquivo
    fgets(linha, sizeof(linha), arquivo); 
    while (total < 981 && fscanf(arquivo, " %9[^,],%49[^,],%49[^,],%49[^,],%49[^\n]", figurinhas[total].codigo, figurinhas[total].titulo, figurinhas[total].secao, figurinhas[total].grupo, figurinhas[total].tipo) == 5){
        total++;
    }
    //Fecha arquivo
    fclose(arquivo);

    //Carrega a quantidade de pacotes (quando abre e fecha o jogo fica salvo)
    carregarPacotes(album, &total_album, mochila, &total_mochila);

    //Chama a função do menu principal
    menuPrincipal(figurinhas, album,mochila, total, &total_album, &total_mochila);
    
>>>>>>> 16c582dd713c6b6c580db3e690781347b7bb83d1
    return 0;
}