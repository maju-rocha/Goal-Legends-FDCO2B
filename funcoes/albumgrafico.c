#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include "biblioteca.h"

//Paleta de cores usada na tela do album grafico
#define COPA_AZUL_ESCURO (Color){ 8, 12, 24, 255 }
#define COPA_AZUL_MEDIO  (Color){ 20, 28, 48, 255 }
#define COPA_VERDE_NEON  (Color){ 0, 230, 150, 255 }
#define COPA_VERMELHO    (Color){ 210, 20, 52, 255 }
#define COPA_OURO_PURO   (Color){ 255, 190, 0, 255 }
#define COPA_OURO_BRILHO (Color){ 255, 230, 120, 255 }
#define COPA_CINZA_CARD  (Color){ 28, 36, 56, 255 }

#define quantidadeMaximaParticulas 50

//Estrutura usada para criar as particulas do fundo
typedef struct{

    Vector2 posicao;
    Vector2 velocidade;
    float tamanhoBase;
    float randomizadorPosicao;
    float transparenciaParticula;
    float velocidadeTransparenciaParticula;

}ParticulaCopa;

//Funcao para limpar espacos antes e depois do texto
void limparEspacos(char *texto){

    int inicio = 0;
    int fim = strlen(texto) - 1;

    while(texto[inicio] == ' ' || texto[inicio] == '\t'){
        inicio++;
    }//while

    while(fim >= inicio && (texto[fim] == ' ' || texto[fim] == '\t' || texto[fim] == '\n' || texto[fim] == '\r')){
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

//Funcao para trocar espacos por underline no nome da pasta
void trocarEspacoPorUnderline(char *texto){

    for(int i = 0; texto[i] != '\0'; i++){

        if(texto[i] == ' '){
            texto[i] = '_';
        }//if

    }//for

}//void

// --- FUNÇÃO ATUALIZADA: Redimensiona e centraliza proporcionalmente qualquer imagem ---
void DesenharFundoParallax(Texture2D textura, Vector2 posicaoMouse, float transparenciaParticula){

    if(textura.id == 0){
        return;
    }//if

    // Fator do Parallax baseado no mouse
    float deslocamentoX = (posicaoMouse.x - 500.0f) * -0.02f;
    float deslocamentoY = (posicaoMouse.y - 400.0f) * -0.02f;

    // Tamanho da janela (1000x800) + margem de 40px para o parallax ter folga (1040x840)
    float larguraAlvo = 1040.0f;
    float alturaAlvo = 840.0f;

    // Calcula a proporção exata sem distorcer a imagem (Efeito "Cover")
    float escalaX = larguraAlvo / (float)textura.width;
    float escalaY = alturaAlvo / (float)textura.height;
    
    // Pega a maior escala para garantir que a tela inteira seja preenchida
    float escalaFinal = (escalaX > escalaY) ? escalaX : escalaY;

    float larguraEscalada = textura.width * escalaFinal;
    float alturaEscalada = textura.height * escalaFinal;

    // Centraliza matematicamente a imagem na janela (compensando a margem do Parallax)
    float posX = (1000.0f - larguraEscalada) / 2.0f + deslocamentoX;
    float posY = (800.0f - alturaEscalada) / 2.0f + deslocamentoY;

    Rectangle origem = { 0.0f, 0.0f, (float)textura.width, (float)textura.height };
    Rectangle destino = { posX, posY, larguraEscalada, alturaEscalada };
    Vector2 centro = { 0.0f, 0.0f };

    DrawTexturePro(textura, origem, destino, centro, 0.0f, Fade(WHITE, transparenciaParticula));

}//void

//Funcao principal do album grafico
void albumGrafico(Figurinha *figurinhas, int total_figurinhas, Figurinha *album, int total_album){

    Texture2D imagens[total_figurinhas];
    int posicoesX[total_figurinhas];
    int posicoesY[total_figurinhas];
    int temImagem[total_figurinhas];
    int paginaDaFigurinha[total_figurinhas];
    char codigosFigurinha[total_figurinhas][20];
    char tiposFigurinha[total_figurinhas][30];

    char secoes[100][50];
    int totalSecoes = 0;
    int totalMostradas = 0;
    char caminho[256];

    //Limpa os dados do album
    for(int j = 0; j < total_album; j++){
        limparFigurinha(&album[j]);
    }//for

    //Limpa os dados de todas as figurinhas
    for(int i = 0; i < total_figurinhas; i++){
        limparFigurinha(&figurinhas[i]);
    }//for

    //Cria a lista de secoes que serao usadas como paginas
    for(int i = 0; i < total_figurinhas; i++){

        int existe = 0;

        for(int j = 0; j < totalSecoes; j++){

            if(strcmp(secoes[j], figurinhas[i].secao) == 0){
                existe = 1;
                break;
            }//if

        }//for

        if(!existe && totalSecoes < 100){
            strcpy(secoes[totalSecoes], figurinhas[i].secao);
            totalSecoes++;
        }//if

    }//for

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1000, 800, "FIFA WORLD CUP 2026 - SUPREME COLLECTOR ALBUM");
    SetTargetFPS(60);

    //Carrega as imagens de fundo
    Texture2D fundosCopa[3];

    fundosCopa[0] = LoadTexture("imagens/FIFA_2026_Verde.png");
    SetTextureFilter(fundosCopa[0], TEXTURE_FILTER_BILINEAR);

    fundosCopa[1] = LoadTexture("imagens/FIFA_2026_Vermelho.png");
    SetTextureFilter(fundosCopa[1], TEXTURE_FILTER_BILINEAR);

    fundosCopa[2] = LoadTexture("imagens/FIFA_2026_Azul.png");
    SetTextureFilter(fundosCopa[2], TEXTURE_FILTER_BILINEAR);

    int larguraFigurinha = 95;
    int alturaFigurinha = 130;
    int espacoX = 145;
    int espacoY = 150;
    int inicioX = 135;
    int inicioY = 110;

    //Organiza as figurinhas por secao e carrega as imagens que ja estao no album
    for(int pagina = 0; pagina < totalSecoes; pagina++){

        int coluna = 0;
        int linha = 0;

        for(int i = 0; i < total_figurinhas; i++){

            if(strcmp(figurinhas[i].secao, secoes[pagina]) == 0){

                posicoesX[totalMostradas] = inicioX + coluna * espacoX;
                posicoesY[totalMostradas] = inicioY + linha * espacoY;
                paginaDaFigurinha[totalMostradas] = pagina;
                temImagem[totalMostradas] = 0;

                strcpy(codigosFigurinha[totalMostradas], figurinhas[i].codigo);
                strcpy(tiposFigurinha[totalMostradas], figurinhas[i].tipo);

                if(estaNoAlbum(album, total_album, figurinhas[i].codigo)){

                    char pasta[100];

                    strcpy(pasta, figurinhas[i].secao);
                    trocarEspacoPorUnderline(pasta);

                    //Monta o caminho da imagem da figurinha
                    strcpy(caminho, "imagens/imagens_figurinhas/");
                    strcat(caminho, pasta);
                    strcat(caminho, "/");
                    strcat(caminho, figurinhas[i].codigo);
                    strcat(caminho, ".png");

                    imagens[totalMostradas] = LoadTexture(caminho);

                    if(imagens[totalMostradas].id != 0){
                        SetTextureFilter(imagens[totalMostradas], TEXTURE_FILTER_BILINEAR);
                        temImagem[totalMostradas] = 1;
                    }//if

                }//if

                totalMostradas++;
                coluna++;

                if(coluna == 5){
                    coluna = 0;
                    linha++;
                }//if

            }//if

        }//for

    }//for

    //Cria as particulas decorativas do fundo
    ParticulaCopa particulas[quantidadeMaximaParticulas];

    for(int i = 0; i < quantidadeMaximaParticulas; i++){

        particulas[i].posicao = (Vector2){ (float)GetRandomValue(-20, 1020), (float)GetRandomValue(0, 800) };
        particulas[i].velocidade = (Vector2){ (float)GetRandomValue(-8, 8) / 15.0f, (float)GetRandomValue(-15, -4) / 10.0f };
        particulas[i].tamanhoBase = (float)GetRandomValue(2, 4);
        particulas[i].randomizadorPosicao = (float)GetRandomValue(0, 360);
        particulas[i].transparenciaParticula = (float)GetRandomValue(20, 80) / 100.0f;
        particulas[i].velocidadeTransparenciaParticula = (float)GetRandomValue(5, 15) / 1000.0f;

    }//for

    int paginaAtual = 0;
    int proximaPagina = 0;
    float tempoGlobal = 0.0f;
    float viradaProgresso = 1.0f;
    float velocidadeVirada = 0.07f;
    int estadoTransicao = 0;
    int direcaoTransicao = 1;

    //Laco principal da janela
    while(!WindowShouldClose()){

        tempoGlobal += GetFrameTime();

        Vector2 posicaoMouse = GetMousePosition();

        //Atualiza as particulas do fundo
        for(int i = 0; i < quantidadeMaximaParticulas; i++){

            particulas[i].posicao.x += particulas[i].velocidade.x + sinf(tempoGlobal + particulas[i].randomizadorPosicao) * 0.2f;
            particulas[i].posicao.y += particulas[i].velocidade.y;
            particulas[i].transparenciaParticula -= particulas[i].velocidadeTransparenciaParticula;

            float distanciaMouse = Vector2Distance(posicaoMouse, particulas[i].posicao);

            if(distanciaMouse < 80.0f){
                particulas[i].posicao.x += (particulas[i].posicao.x - posicaoMouse.x) * 0.03f;
            }//if

            if(particulas[i].transparenciaParticula <= 0.0f || particulas[i].posicao.y < 70.0f){

                particulas[i].posicao = (Vector2){ (float)GetRandomValue(0, 1000), 760.0f };
                particulas[i].transparenciaParticula = (float)GetRandomValue(60, 100) / 100.0f;

            }//if

        }//for

        //Controla o efeito de virar pagina
        if(estadoTransicao == 1){

            viradaProgresso -= velocidadeVirada;

            if(viradaProgresso <= 0.0f){
                viradaProgresso = 0.0f;
                paginaAtual = proximaPagina;
                estadoTransicao = 2;
            }//if

        }else if(estadoTransicao == 2){

            viradaProgresso += velocidadeVirada;

            if(viradaProgresso >= 1.0f){
                viradaProgresso = 1.0f;
                estadoTransicao = 0;
            }//if

        }//if

        //Verifica os comandos para trocar de pagina
        if(estadoTransicao == 0){

            if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)){
                proximaPagina = (paginaAtual + 1) % totalSecoes;
                direcaoTransicao = 1;
                estadoTransicao = 1;
            }//if

            if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)){
                proximaPagina = (paginaAtual - 1 + totalSecoes) % totalSecoes;
                direcaoTransicao = -1;
                estadoTransicao = 1;
            }//if

            Rectangle botaoVoltar = { 5, 370, 50, 60 };
            Rectangle botaoAvancar = { 945, 370, 50, 60 };

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){

                if(CheckCollisionPointRec(posicaoMouse, botaoVoltar)){
                    proximaPagina = (paginaAtual - 1 + totalSecoes) % totalSecoes;
                    direcaoTransicao = -1;
                    estadoTransicao = 1;
                }//if

                if(CheckCollisionPointRec(posicaoMouse, botaoAvancar)){
                    proximaPagina = (paginaAtual + 1) % totalSecoes;
                    direcaoTransicao = 1;
                    estadoTransicao = 1;
                }//if

            }//if

        }//if

        BeginDrawing();
        ClearBackground(COPA_AZUL_ESCURO);

        int indiceFundo = paginaAtual % 3;

        //Desenha o fundo conforme a pagina atual
        DesenharFundoParallax(fundosCopa[indiceFundo], posicaoMouse, 1.0f);

        //Desenha as particulas
        for(int i = 0; i < quantidadeMaximaParticulas; i++){

            float tamanhoPulso = particulas[i].tamanhoBase * (1.0f + sinf(tempoGlobal * 3.0f + particulas[i].randomizadorPosicao) * 0.2f);

            DrawCircleV(particulas[i].posicao, tamanhoPulso, Fade(COPA_OURO_PURO, particulas[i].transparenciaParticula * 0.5f));

        }//for

        float s = viradaProgresso;
        float escalaHorizontal = (s * s * (3.0f - 2.0f * s));

        //Desenha as figurinhas da pagina atual
        for(int i = 0; i < totalMostradas; i++){

            if(paginaDaFigurinha[i] == paginaAtual){

                float originalX = (float)posicoesX[i];
                float x = originalX;
                float y = (float)posicoesY[i];

                if(estadoTransicao != 0){

                    if(direcaoTransicao == 1){
                        x = 500.0f - (500.0f - originalX) * escalaHorizontal;
                    }else{
                        x = 500.0f + (originalX - 500.0f) * escalaHorizontal;
                    }//if

                }//if

                Rectangle retanguloFigurinha = { x, y, (float)larguraFigurinha * escalaHorizontal, (float)alturaFigurinha };
                bool mouseEmCima = (estadoTransicao == 0) && CheckCollisionPointRec(posicaoMouse, retanguloFigurinha);

                if(mouseEmCima){

                    retanguloFigurinha.y -= 6.0f;
                    retanguloFigurinha.x -= 3.0f;
                    retanguloFigurinha.width += 6.0f;
                    retanguloFigurinha.height += 6.0f;

                }//if

                bool ehEspecial = (strstr(tiposFigurinha[i], "Especial") != NULL);

                if(temImagem[i]){

                    Rectangle origemImagem = { 0.0f, 0.0f, (float)imagens[i].width, (float)imagens[i].height };

                    DrawTexturePro(imagens[i], origemImagem, retanguloFigurinha, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);

                    if(ehEspecial){

                        float pulso = (sinf(tempoGlobal * 5.0f) + 1.0f) / 2.0f;
                        Color corBorda = Fade(COPA_OURO_PURO, 0.5f + (pulso * 0.5f));

                        DrawRectangleLinesEx((Rectangle){retanguloFigurinha.x - 1.0f, retanguloFigurinha.y - 1.0f, retanguloFigurinha.width + 2.0f, retanguloFigurinha.height + 2.0f}, 2.0f, corBorda);
                        DrawRectangleLinesEx((Rectangle){retanguloFigurinha.x - 3.0f, retanguloFigurinha.y - 3.0f, retanguloFigurinha.width + 6.0f, retanguloFigurinha.height + 6.0f}, 1.0f, Fade(COPA_OURO_BRILHO, pulso * 0.3f));

                    }else{

                        Color corLinha = Fade(BLACK, 0.5f);

                        if(mouseEmCima){
                            corLinha = COPA_VERDE_NEON;
                        }//if

                        DrawRectangleLinesEx(retanguloFigurinha, 1.5f, corLinha);

                    }//if

                    if(mouseEmCima){

                        float reflexoX = retanguloFigurinha.x + ((float)sinf(tempoGlobal * 2.5f) + 1.0f) * 0.5f * retanguloFigurinha.width;

                        DrawLineEx((Vector2){reflexoX, retanguloFigurinha.y}, (Vector2){reflexoX + 15.0f, retanguloFigurinha.y + retanguloFigurinha.height}, 3.0f, Fade(WHITE, 0.25f));

                    }//if

                }else{

                    DrawRectangleRec(retanguloFigurinha, COPA_CINZA_CARD);

                    Color corLinha = Fade(WHITE, 0.08f);

                    if(mouseEmCima){
                        corLinha = COPA_VERDE_NEON;
                    }//if

                    DrawRectangleLinesEx(retanguloFigurinha, 1.5f, corLinha);

                    if(mouseEmCima){
                        DrawRectangleRec(retanguloFigurinha, Fade(COPA_AZUL_MEDIO, 0.4f));
                    }//if

                    if(escalaHorizontal > 0.25f){

                        DrawLineEx((Vector2){retanguloFigurinha.x + 4.0f, retanguloFigurinha.y + 4.0f}, (Vector2){retanguloFigurinha.x + retanguloFigurinha.width - 4.0f, retanguloFigurinha.y + retanguloFigurinha.height - 4.0f}, 1.0f, Fade(WHITE, 0.02f));

                        int larguraCodigo = MeasureText(codigosFigurinha[i], 16);
                        float posicaoCodigoX = retanguloFigurinha.x + (retanguloFigurinha.width - (larguraCodigo * escalaHorizontal)) / 2.0f;
                        Color corCodigo = Fade(WHITE, 0.35f);

                        if(mouseEmCima){
                            corCodigo = COPA_VERDE_NEON;
                        }//if

                        DrawText(codigosFigurinha[i], (int)posicaoCodigoX + 1, (int)(retanguloFigurinha.y + (retanguloFigurinha.height / 2.0f) - 7.0f), 16, Fade(BLACK, 0.6f));
                        DrawText(codigosFigurinha[i], (int)posicaoCodigoX, (int)(retanguloFigurinha.y + (retanguloFigurinha.height / 2.0f) - 8.0f), 16, corCodigo);

                    }//if

                }//if

            }//if

        }//for

        //Desenha a parte de cima da tela
        DrawRectangle(0, 0, 1000, 85, Fade(COPA_AZUL_MEDIO, 0.85f));
        DrawRectangle(0, 83, 1000, 2, COPA_OURO_PURO);

        DrawText(secoes[paginaAtual], 32, 27, 32, Fade(BLACK, 0.4f));
        DrawText(secoes[paginaAtual], 30, 25, 32, WHITE);

        // =========================================================
        // BARRA DE PROGRESSO CORRIGIDA
        // =========================================================
        float porcentagemConclusao = 0.0f;

        if(total_figurinhas > 0){
            porcentagemConclusao = ((float)total_album / total_figurinhas) * 100.0f;
        }//if

        DrawRectangle(650, 47, 320, 10, Fade(BLACK, 0.5f));
        DrawRectangle(651, 48, (int)(318 * (porcentagemConclusao / 100.0f)), 8, COPA_VERDE_NEON);
        DrawText(TextFormat("ALBUM PROGRESSO: %.1f%% (%d/%d)", porcentagemConclusao, total_album, total_figurinhas), 650, 24, 13, Fade(WHITE, 0.7f));
        // =========================================================

        DrawText(TextFormat("PAGINA %02d / %02d", paginaAtual + 1, totalSecoes), 455, 58, 15, COPA_OURO_PURO);

        Rectangle botaoVoltarReal = { 5, 370, 50, 60 };
        Rectangle botaoAvancarReal = { 945, 370, 50, 60 };

        bool emCimaVoltar = CheckCollisionPointRec(posicaoMouse, botaoVoltarReal) && (estadoTransicao == 0);
        bool emCimaAvancar = CheckCollisionPointRec(posicaoMouse, botaoAvancarReal) && (estadoTransicao == 0);

        float tamanhoBotaoVoltar = 22.0f;
        Color corBotaoVoltar = Fade(WHITE, 0.1f);
        Color corTextoVoltar = WHITE;

        if(emCimaVoltar){
            tamanhoBotaoVoltar = 26.0f;
            corBotaoVoltar = COPA_VERDE_NEON;
            corTextoVoltar = COPA_VERDE_NEON;
        }//if

        DrawCircleV((Vector2){30, 400}, tamanhoBotaoVoltar, corBotaoVoltar);
        DrawCircleV((Vector2){30, 400}, 20.0f, COPA_AZUL_MEDIO);
        DrawText("<", 24, 387, 24, corTextoVoltar);

        float tamanhoBotaoAvancar = 22.0f;
        Color corBotaoAvancar = Fade(WHITE, 0.1f);
        Color corTextoAvancar = WHITE;

        if(emCimaAvancar){
            tamanhoBotaoAvancar = 26.0f;
            corBotaoAvancar = COPA_VERDE_NEON;
            corTextoAvancar = COPA_VERDE_NEON;
        }//if

        DrawCircleV((Vector2){970, 400}, tamanhoBotaoAvancar, corBotaoAvancar);
        DrawCircleV((Vector2){970, 400}, 20.0f, COPA_AZUL_MEDIO);
        DrawText(">", 964, 387, 24, corTextoAvancar);

        //Desenha a parte de baixo com as instrucoes
        DrawRectangle(0, 765, 1000, 35, COPA_AZUL_MEDIO);
        DrawLine(0, 765, 1000, 765, Fade(WHITE, 0.08f));

        int larguraInstrucao = MeasureText("Seta Direita / Esquerda ou Cliques para mudar de pagina", 14);

        DrawText("Seta Direita / Esquerda ou Cliques para mudar de pagina", (1000 - larguraInstrucao) / 2, 775, 14, Fade(WHITE, 0.5f));

        EndDrawing();

    }//while

    //Descarrega as imagens das figurinhas
    for(int i = 0; i < totalMostradas; i++){

        if(temImagem[i]){
            UnloadTexture(imagens[i]);
        }//if

    }//for

    //Descarrega as imagens de fundo
    for(int i = 0; i < 3; i++){
        UnloadTexture(fundosCopa[i]);
    }//for

    CloseWindow();

}//void