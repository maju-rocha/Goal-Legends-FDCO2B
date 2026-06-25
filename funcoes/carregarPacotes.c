#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "biblioteca.h"

void carregarPacotes(Figurinha *album, int *total_album, Figurinha *mochila, int *total_mochila){

    FILE *arquivo_carrega_pacotes = fopen("extras/pacotes.txt", "r");

    if(arquivo_carrega_pacotes == NULL){
        pacotes_fechados = 0;
    }else{
        fscanf(arquivo_carrega_pacotes, "%d", &pacotes_fechados);
        fclose(arquivo_carrega_pacotes);
    }

    int limiteFigurinhasSalvas = 1000;

    for(int arquivoEscolhido = 0; arquivoEscolhido < 2; arquivoEscolhido++){

        FILE *arquivo_csv;
        Figurinha *vetorAtual;
        int *totalAtual;

        if(arquivoEscolhido == 0){
            arquivo_csv = fopen("extras/album.csv", "r");
            vetorAtual = album;
            totalAtual = total_album;
        }else{
            arquivo_csv = fopen("extras/mochila.csv", "r");
            vetorAtual = mochila;
            totalAtual = total_mochila;
        }

        *totalAtual = 0;

        if(arquivo_csv != NULL){

            char linha[300];

            while(fgets(linha, sizeof(linha), arquivo_csv) != NULL && *totalAtual < limiteFigurinhasSalvas){

                Figurinha f;

                f.codigo[0] = '\0';
                f.titulo[0] = '\0';
                f.secao[0] = '\0';
                f.grupo[0] = '\0';
                f.tipo[0] = '\0';

                int campo = 0;
                int posicaoCampo = 0;

                for(int i = 0; linha[i] != '\0'; i++){
                    if(linha[i] == '\n' || linha[i] == '\r'){
                        linha[i] = '\0';
                    }
                }

                for(int i = 0; ; i++){
                    char letra = linha[i];
                    if(letra == ',' || letra == '\0'){

                        if(campo == 0){
                            f.codigo[posicaoCampo] = '\0';
                        }else if(campo == 1){
                            f.titulo[posicaoCampo] = '\0';
                        }else if(campo == 2){
                            f.secao[posicaoCampo] = '\0';
                        }else if(campo == 3){
                            f.grupo[posicaoCampo] = '\0';
                        }else if(campo == 4){
                            f.tipo[posicaoCampo] = '\0';
                        }

                        campo++;
                        posicaoCampo = 0;

                        if(letra == '\0'){
                            break;
                        }
                    }else{

                        if(campo == 0 && posicaoCampo < sizeof(f.codigo) - 1){
                            f.codigo[posicaoCampo] = letra;
                            posicaoCampo++;
                        }else if(campo == 1 && posicaoCampo < sizeof(f.titulo) - 1){
                            f.titulo[posicaoCampo] = letra;
                            posicaoCampo++;
                        }else if(campo == 2 && posicaoCampo < sizeof(f.secao) - 1){
                            f.secao[posicaoCampo] = letra;
                            posicaoCampo++;
                        }else if(campo == 3 && posicaoCampo < sizeof(f.grupo) - 1){
                            f.grupo[posicaoCampo] = letra;
                            posicaoCampo++;
                        }else if(campo == 4 && posicaoCampo < sizeof(f.tipo) - 1){
                            f.tipo[posicaoCampo] = letra;
                            posicaoCampo++;
                        }
                    }
                }

                limparFigurinha(&f);

                if(strlen(f.codigo) > 0){
                    vetorAtual[*totalAtual] = f;
                    (*totalAtual)++;
                }
            }
            fclose(arquivo_csv);
        }
    }
}