#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <raylib.h>
#include "biblioteca.h"

int main(void){

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
    carregarPacotes();

    //Chama a função do menu principal
    menuPrincipal(figurinhas, album,mochila, total, &total_album, &total_mochila);
    
    return 0;
}