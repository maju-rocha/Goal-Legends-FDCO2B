#include <string.h>
#include "global.h"

//Variáveis globais
int pacotes_fechados = 0;
int figurinha_repetida = 0;
Menu_FimdeJogo estadoAtual = MENU_FIM;

//Função para limpar espacos antes e depois do texto
void limparEspacos(char *texto){

    int inicio = 0;
    int fim = strlen(texto) - 1;

    while(texto[inicio] == ' ' || texto[inicio] == '\t'){
        inicio++;
    }

    while(fim >= inicio && 
         (texto[fim] == ' ' || texto[fim] == '\t' || texto[fim] == '\n' || texto[fim] == '\r')){
        texto[fim] = '\0';
        fim--;
    }

    if(inicio > 0){

        int j = 0;

        for(int i = inicio; texto[i] != '\0'; i++){
            texto[j] = texto[i];
            j++;
        }

        texto[j] = '\0';
    }
}

//Função para limpar todos os campos da figurinha
void limparFigurinha(Figurinha *f){

    limparEspacos((*f).codigo);
    limparEspacos((*f).titulo);
    limparEspacos((*f).secao);
    limparEspacos((*f).grupo);
    limparEspacos((*f).tipo);
}

//Função para trocar espaços por underline
void trocarEspacoPorUnderline(char *texto){

    for(int i = 0; texto[i] != '\0'; i++){

        if(texto[i] == ' '){
            texto[i] = '_';
        }
    }
}

//Função para comparar se uma figurinha específica já está no álbum
int estaNoAlbum(Figurinha *album, int total_album, char codigo[]){

    for(int i = 0; i < total_album; i++){

        if(strcmp(album[i].codigo, codigo) == 0){
            return 1;
        }
    }

    return 0;
}