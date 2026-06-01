#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h> 
#include "figurinhas.h" 

int main(){

    char linha[300];//variavel com todos os bytes de uma linha
    
    int total = 0;//variavel para contabilizar a quantidade
    
    Figurinha *figurinhas = malloc (981 * sizeof(Figurinha));//Cria vetor dinamico para as figurinhas 

    Figurinha *inventario = malloc (981 * sizeof(Figurinha));//Cria vetor dinamico para o inventário

    Figurinha *mochila = malloc (981 * sizeof(Figurinha));//Cria vetor dinamico para a mochila
    
    if (figurinhas == NULL){//testa se o malloc funcionou 
        printf("Erro de alocacao.\n"); 
        return 1;
    }

    FILE *arquivo = fopen("figurinhas2026.csv","rb");//cria e abre o arquivo para leitura de figurinhas
        
    if (arquivo == NULL){//testa se o arquivo abriu 
        printf("Erro ao abrir o arquivo.\n"); 
        return 1; 
    }//if

    fgets(linha, sizeof(linha), arquivo);//ignora a primeira linha do arquivo figurinhas2026.csv
    
    while (fscanf(arquivo," %9[^,],%49[^,],%49[^,],%49[^,],%49[^\n]",figurinhas[total].codigo,figurinhas[total].titulo,figurinhas[total].secao,figurinhas[total].grupo,figurinhas[total].tipo) == 5){
        total++; 
    }//le uma linha, se conseguiu ler os 5 espaços (codigo,titulo,secao,grupo e tipo) incrementa no total e assim vai pelo arquivo inteiro
    
    fclose(arquivo);//fecha arquivo
    
    srand(time(NULL));//libera aleatoriedade para funcao abrirPacote 
    
    printf("Pacote com 7 figurinhas:\n"); 
    
    abrirPacote(figurinhas, total);//chama funcao abrirPacote
    
    free(figurinhas);//Libera a memória do vetor figurinhas

    free(inventario);//Libera a memória do vetor inventário

    free(mochila);//Libera a memória do vetor mochila
    return 0; }