#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include "biblioteca.h"

//Funcao para limpar espacos antes e depois do texto
void limparEspacos(char *texto){

    int inicio = 0;
    int fim = strlen(texto) - 1;

    while(texto[inicio] == ' '){
        inicio++;
    }//while

    while(fim >= inicio && texto[fim] == ' '){
        texto[fim] = '\0';
        fim--;
    }//while

    if(inicio > 0){

        int j = 0;

        for(int i = inicio; texto[i] != '\0'; i++){
            texto[j] = texto[i];
            j++;
        }//for

        texto[j] = '\0';

    }//if

}//void

//Funcao para limpar todos os campos da figurinha
void limparFigurinha(Figurinha *f){

    limparEspacos((*f).codigo);
    limparEspacos((*f).titulo);
    limparEspacos((*f).secao);
    limparEspacos((*f).grupo);
    limparEspacos((*f).tipo);

}//void

//Funcao para trocar espaco por underline no nome da pasta
void trocarEspacoPorUnderline(char *texto){

    for(int i = 0; texto[i] != '\0'; i++){

        if(texto[i] == ' '){
            texto[i] = '_';
        }//if

    }//for

}//void

void albumGrafico(Figurinha *figurinhas, int total_figurinhas, Figurinha *album, int total_album){

    //Váriaveis das imagens e posições
    Texture2D imagens[1000];
    int posicoesX[1000];
    int posicoesY[1000];
    int temImagem[1000];
    int paginaDaFigurinha[1000];

    //Váriaveis das páginas/seções
    char secoes[100][50];
    int totalSecoes = 0;

    int total_mostradas = 0;
    char caminho[256];

    //Limpa os espacos das figurinhas do album
    for(int j = 0; j < total_album; j++){
        limparFigurinha(&album[j]);
    }//for

    //Limpa os espacos da lista completa de figurinhas
    for(int i = 0; i < total_figurinhas; i++){
        limparFigurinha(&figurinhas[i]);
    }//for

    //=======================================================//
    //============== Montagem das páginas/seções ============//
    //=======================================================//

    for(int i = 0; i < total_figurinhas; i++){

        int existe = 0;

        for(int j = 0; j < totalSecoes; j++){

            if(strcmp(secoes[j], figurinhas[i].secao) == 0){
                existe = 1;
            }//if

        }//for

        if(!existe){
            strcpy(secoes[totalSecoes], figurinhas[i].secao);
            totalSecoes++;
        }//if

    }//for

    //=======================================================//
    //================ Inicialização da tela ================//
    //=======================================================//

    InitWindow(1000, 800, "Album Grafico");
    SetTargetFPS(60);

    //Tamanho e posições das figurinhas
    int larguraFigurinha = 110;
    int alturaFigurinha = 150;

    int espacoX = 140;
    int espacoY = 170;

    int inicioX = 165;
    int inicioY = 85;

    //=======================================================//
    //============ Carregamento das texturas antes ==========//
    //=======================================================//

    for(int pagina = 0; pagina < totalSecoes; pagina++){

        int coluna = 0;
        int linha = 0;

        for(int i = 0; i < total_figurinhas; i++){

            if(strcmp(figurinhas[i].secao, secoes[pagina]) == 0){

                int x = inicioX + coluna * espacoX;
                int y = inicioY + linha * espacoY;

                posicoesX[total_mostradas] = x;
                posicoesY[total_mostradas] = y;
                paginaDaFigurinha[total_mostradas] = pagina;
                temImagem[total_mostradas] = 0;

                //Verifica se a figurinha está no album
                if(estaNoAlbum(album, total_album, figurinhas[i].codigo)){

                    char pasta[100];

                    strcpy(pasta, figurinhas[i].secao);
                    trocarEspacoPorUnderline(pasta);

                    strcpy(caminho, "imagens/imagens_figurinhas/");
                    strcat(caminho, pasta);
                    strcat(caminho, "/");
                    strcat(caminho, figurinhas[i].codigo);
                    strcat(caminho, ".png");

                    imagens[total_mostradas] = LoadTexture(caminho);

                    if(imagens[total_mostradas].id != 0){
                        temImagem[total_mostradas] = 1;
                    }else{
                        printf("Erro ao carregar: %s\n", caminho);
                    }//if else

                }//if

                total_mostradas++;

                coluna++;

                if(coluna == 5){
                    coluna = 0;
                    linha++;
                }//if

            }//if

        }//for

    }//for

    int paginaAtual = 0;

    //=======================================================//
    //=============== Parte gráfica do álbum ================//
    //=======================================================//

    while(!WindowShouldClose()){

        //Tecla para próxima página
        if(IsKeyPressed(KEY_RIGHT)){

            paginaAtual++;

            if(paginaAtual >= totalSecoes){
                paginaAtual = 0;
            }//if

        }//if

        //Tecla para página anterior
        if(IsKeyPressed(KEY_LEFT)){

            paginaAtual--;

            if(paginaAtual < 0){
                paginaAtual = totalSecoes - 1;
            }//if

        }//if

        Color corAlbum = {181, 238, 255, 255};

        BeginDrawing();
        ClearBackground(corAlbum);

        //Texto da seleção
        int tamanhoFonte = 30;
        int larguraTexto = MeasureText(secoes[paginaAtual], tamanhoFonte);

        DrawText(
            secoes[paginaAtual],
            (1000 - larguraTexto) / 2,
            25,
            tamanhoFonte,
            BLACK
        );

        //Texto de instruções
        int larguraInstrucao = MeasureText("Use as setas <- e -> para mudar de pagina", 18);

        DrawText(
            "Use as setas <- e -> para mudar de pagina",
            (1000 - larguraInstrucao) / 2,
            775,
            18,
            DARKGRAY
        );

        //Desenha as figurinhas da página atual
        for(int i = 0; i < total_mostradas; i++){

            if(paginaDaFigurinha[i] == paginaAtual){

                int x = posicoesX[i];
                int y = posicoesY[i];

                if(temImagem[i]){

                    Rectangle origem = {
                        0,
                        0,
                        (float)imagens[i].width,
                        (float)imagens[i].height
                    };

                    Rectangle destino = {
                        (float)x,
                        (float)y,
                        larguraFigurinha,
                        alturaFigurinha
                    };

                    Vector2 centro = {0, 0};

                    DrawTexturePro(imagens[i], origem, destino, centro, 0, WHITE);

                }else{

                    DrawRectangle(
                        x,
                        y,
                        larguraFigurinha,
                        alturaFigurinha,
                        LIGHTGRAY
                    );

                }//if else

                //Contorno da figurinha
                DrawRectangleLines(
                    x,
                    y,
                    larguraFigurinha,
                    alturaFigurinha,
                    BLACK
                );

            }//if

        }//for

        EndDrawing();

    }//while

    //Liberações de memória do álbum
    for(int i = 0; i < total_mostradas; i++){

        if(temImagem[i]){
            UnloadTexture(imagens[i]);
        }//if

    }//for

    //Fecha a janela do álbum
    CloseWindow();

}//void