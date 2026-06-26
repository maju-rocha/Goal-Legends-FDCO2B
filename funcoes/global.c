#include <string.h>
#include "global.h"

//Variáveis globais
int pacotes_fechados = 0;
int figurinha_repetida = 0;
Menu_FimdeJogo estadoAtual = MENU_FIM;

//Função para limpar espacos antes e depois do texto
void limparEspacos(char *texto){

    int inicio = 0;//Variável para começar no primeiro caracter
    int fim = strlen(texto) - 1;//Variável para contar quantos caracteres tem na string

    //Confere se o caracter atual é um espaço ou uma tabulação \t
    while(texto[inicio] == ' ' || texto[inicio] == '\t'){
        inicio++;
    }//while

    //Confere e Remove o espaço/tabulação/quebra de linha e troca pelo '\0'
    while(fim >= inicio && (texto[fim] == ' ' || texto[fim] == '\t' || texto[fim] == '\n')){
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

//Função para limpar todos os campos da figurinha
void limparFigurinha(Figurinha *f){
    
    limparEspacos((*f).codigo);
    limparEspacos((*f).titulo);
    limparEspacos((*f).secao);
    limparEspacos((*f).grupo);
    limparEspacos((*f).tipo);
}//void

//Função para trocar espaços por underline
void trocarEspacoPorUnderline(char *texto){

    //Loop até quando encontrar '\0'
    for(int i = 0; texto[i] != '\0'; i++){

        if(texto[i] == ' '){
            texto[i] = '_';
        }//if Trocando espaços por underline
    }//for
}//void

//Função para comparar se uma figurinha específica já está no álbum
int estaNoAlbum(Figurinha *album, int total_album, char codigo[]){
    
    for(int i = 0; i < total_album; i++){

        if(strcmp(album[i].codigo, codigo) == 0){
            return 1;
        }//if
    }//for

    return 0;
}