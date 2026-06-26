#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inserir.h"

//Função para limpar espacos antes e depois do texto
static void limparCampoInserir(char *texto){

    int inicio = 0;//Variável de começo
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
static void limparBufferInserir(){

    int c;

    while((c = getchar()) != '\n' && c != EOF);//Exclui o texto digitado anteriormente e le todos os caracteres até o final da string

}//void

//Funcao principal
void inserirFigurinha(Figurinha *vetor, int *total){

    char codigoNovo[10];//Variável para armazenar o novo codigo
    int codigoExiste = 0;//Variável de condição

    if(*total >= 1100){
        printf("\nNao e possivel inserir mais figurinhas. Lista cheia.\n");
        return;
    }//if

    printf("\n--- Inserir Figurinha ---\n");//Texto terminal

    printf("Digite o codigo da nova figurinha: ");
    scanf(" %6[^\n]", codigoNovo);
    limparBufferInserir();

    limparCampoInserir(codigoNovo);

    //Confere se o codigo ja existe
    for(int i = 0; i < *total; i++){

        char codigo_limpo[15];//Variável para comparar o codigo

        strcpy(codigo_limpo, vetor[i].codigo);//Copia o codigo do vetor
        limparCampoInserir(codigo_limpo);//Limpa os espaços do codigo copiado

        if(strcmp(codigo_limpo, codigoNovo) == 0){
            codigoExiste = 1;
            break;
        }//if

    }//for

    if(codigoExiste){
        printf("\nJa existe uma figurinha com esse codigo.\n");
        return;
    }//if

    int posicao = *total;//Posição onde a nova figurinha sera inserida

    strcpy(vetor[posicao].codigo, codigoNovo);

    //Scanfs para o usuário
    printf("Digite o Titulo: ");
    scanf(" %28[^\n]", vetor[posicao].titulo);
    limparBufferInserir();
    limparCampoInserir(vetor[posicao].titulo);

    printf("Digite a Secao: ");
    scanf(" %19[^\n]", vetor[posicao].secao);
    limparBufferInserir();
    limparCampoInserir(vetor[posicao].secao);

    printf("Digite o Grupo: ");
    scanf(" %7[^\n]", vetor[posicao].grupo);
    limparBufferInserir();
    limparCampoInserir(vetor[posicao].grupo);

    printf("Digite o Tipo: ");
    scanf(" %8[^\n]", vetor[posicao].tipo);
    limparBufferInserir();
    limparCampoInserir(vetor[posicao].tipo);

    FILE *arquivo = fopen("extras/figurinhas2026copy.csv", "w");//Escreve no arquivo figurinhas2026copy.csv

    if(arquivo == NULL){
        printf("\nErro ao abrir o arquivo para inserir figurinha.\n");
        return;
    }//if teste

    //Escreve todas as figurinhas novamente, incluindo a nova
    for(int k = 0; k < *total + 1; k++){

        limparCampoInserir(vetor[k].codigo);
        limparCampoInserir(vetor[k].titulo);
        limparCampoInserir(vetor[k].secao);
        limparCampoInserir(vetor[k].grupo);
        limparCampoInserir(vetor[k].tipo);

        fprintf(arquivo,"%-6s, %-28s, %-19s, %-7s, %s\n",vetor[k].codigo,vetor[k].titulo,vetor[k].secao,vetor[k].grupo,vetor[k].tipo);//Escreve no no arquivo

    }//for

    fclose(arquivo);//Fecha o arquivo

    (*total)++;//Aumenta o total de figurinhas

    printf("\nFigurinha inserida com sucesso!\n");//Texto terminal

}//void