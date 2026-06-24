#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include <raylib.h>
#include <raymath.h>
#include "biblioteca.h"

// Paleta de Cores de Alta Costura - Copa do Mundo Premium
#define COPA_AZUL_ESCURO (Color){ 8, 12, 24, 255 }     // Fundo Espacial Profundo
#define COPA_AZUL_MEDIO  (Color){ 20, 28, 48, 255 }    // UI Containers e Rodapé
#define COPA_VERDE_NEON  (Color){ 0, 230, 150, 255 }   // Feedback de Hover e Luz Ativa
#define COPA_VERMELHO    (Color){ 210, 20, 52, 255 }   // Vermelho Real Fifa
#define COPA_OURO_PURO   (Color){ 255, 190, 0, 255 }   // Destaque Lendário / Campeão
#define COPA_OURO_BRILHO (Color){ 255, 230, 120, 255 } // Brilho Incandescente
#define COPA_CINZA_CARD  (Color){ 28, 36, 56, 255 }    // Slot Vazio Fosco Premium

// Estrutura Avançada de Partículas Atmosféricas
typedef struct {
    Vector2 posicao;
    Vector2 velocidade;
    float tamanhoBase;
    float seedSeno;
    float alpha;
    float velocidadeAlpha;
} ParticulaCopa;

#define MAX_PARTICULAS 50

// Funções de utilidade otimizadas
void limparEspacos(char *texto){
    int inicio = 0;
    int fim = strlen(texto) - 1;
    while(texto[inicio] == ' ' || texto[inicio] == '\t'){ inicio++; }
    while(fim >= inicio && (texto[fim] == ' ' || texto[fim] == '\t' || texto[fim] == '\n' || texto[fim] == '\r')){ texto[fim] = '\0'; fim--; }
    if(inicio > 0){
        int j = 0;
        for(int i = inicio; texto[i] != '\0'; i++){ texto[j] = texto[i]; j++; }
        texto[j] = '\0';
    }
}

void limparFigurinha(Figurinha *f){
    limparEspacos((*f).codigo);
    limparEspacos((*f).titulo);
    limparEspacos((*f).secao);
    limparEspacos((*f).grupo);
    limparEspacos((*f).tipo);
}

void trocarEspacoPorUnderline(char *texto){
    for(int i = 0; texto[i] != '\0'; i++){
        if(texto[i] == ' '){ texto[i] = '_'; }
    }
}

// Renderizador com efeito Parallax
void DesenharFundoParallax(Texture2D textura, Vector2 mousePos, float alpha) {
    if (textura.id == 0) return; 
    
    float deslocamentoX = (mousePos.x - 500.0f) * -0.02f;
    float deslocamentoY = (mousePos.y - 400.0f) * -0.02f;

    Rectangle origem = { 0.0f, 0.0f, (float)textura.width, (float)textura.height };
    Rectangle destino = { deslocamentoX - 20, deslocamentoY - 20, 1040.0f, 840.0f };
    Vector2 centro = { 0.0f, 0.0f };

    DrawTexturePro(textura, origem, destino, centro, 0.0f, Fade(WHITE, alpha));
}

void albumGrafico(Figurinha *figurinhas, int total_figurinhas, Figurinha *album, int total_album){
    Texture2D *imagens = (Texture2D *)malloc(total_figurinhas * sizeof(Texture2D));
    int *posicoesX = (int *)malloc(total_figurinhas * sizeof(int));
    int *posicoesY = (int *)malloc(total_figurinhas * sizeof(int));
    int *temImagem = (int *)malloc(total_figurinhas * sizeof(int));
    int *paginaDaFigurinha = (int *)malloc(total_figurinhas * sizeof(int));
    char (*codigosFigurinha)[20] = (char (*)[20])malloc(total_figurinhas * sizeof(*codigosFigurinha));
    char (*tiposFigurinha)[30] = (char (*)[30])malloc(total_figurinhas * sizeof(*tiposFigurinha));

    char secoes[100][50];
    int totalSecoes = 0;
    int total_mostradas = 0;
    int figurinhasColadasContador = 0;
    char caminho[256];

    for(int j = 0; j < total_album; j++){ limparFigurinha(&album[j]); }
    for(int i = 0; i < total_figurinhas; i++){ limparFigurinha(&figurinhas[i]); }

    // Criação do índice dinâmico de seções (Páginas)
    for(int i = 0; i < total_figurinhas; i++){
        int existe = 0;
        for(int j = 0; j < totalSecoes; j++){
            if(strcmp(secoes[j], figurinhas[i].secao) == 0){ existe = 1; break; }
        }
        if(!existe && totalSecoes < 100){
            strcpy(secoes[totalSecoes], figurinhas[i].secao);
            totalSecoes++;
        }
    }

    SetConfigFlags(FLAG_MSAA_4X_HINT); 
    InitWindow(1000, 800, "FIFA WORLD CUP 2026 - SUPREME COLLECTOR ALBUM");
    SetTargetFPS(60);

    // Carregamento dos fundos com Filtro Bilinear aplicado
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

    // Pré-carregamento otimizado de ativos
    for(int pagina = 0; pagina < totalSecoes; pagina++){
        int coluna = 0;
        int linha = 0;

        for(int i = 0; i < total_figurinhas; i++){
            if(strcmp(figurinhas[i].secao, secoes[pagina]) == 0){
                posicoesX[total_mostradas] = inicioX + coluna * espacoX;
                posicoesY[total_mostradas] = inicioY + linha * espacoY;
                paginaDaFigurinha[total_mostradas] = pagina;
                temImagem[total_mostradas] = 0;
                strcpy(codigosFigurinha[total_mostradas], figurinhas[i].codigo);
                strcpy(tiposFigurinha[total_mostradas], figurinhas[i].tipo);

                if(estaNoAlbum(album, total_album, figurinhas[i].codigo)){
                    char pasta[100];
                    strcpy(pasta, figurinhas[i].secao);
                    trocarEspacoPorUnderline(pasta);
                    
                    // Concatenação de strings para formar o caminho do arquivo
                    strcpy(caminho, "imagens/imagens_figurinhas/");
                    strcat(caminho, pasta);
                    strcat(caminho, "/");
                    strcat(caminho, figurinhas[i].codigo);
                    strcat(caminho, ".png");

                    imagens[total_mostradas] = LoadTexture(caminho);
                    if(imagens[total_mostradas].id != 0){
                        // Filtro Bilinear aplicado na figurinha para manter qualidade no zoom
                        SetTextureFilter(imagens[total_mostradas], TEXTURE_FILTER_BILINEAR);
                        
                        temImagem[total_mostradas] = 1;
                        figurinhasColadasContador++;
                    }
                }
                total_mostradas++;
                coluna++;
                if(coluna == 5){ coluna = 0; linha++; }
            }
        }
    }

    // Inicialização da física de partículas
    ParticulaCopa part[MAX_PARTICULAS];
    for (int i = 0; i < MAX_PARTICULAS; i++) {
        part[i].posicao = (Vector2){ (float)GetRandomValue(-20, 1020), (float)GetRandomValue(0, 800) };
        part[i].velocidade = (Vector2){ (float)GetRandomValue(-8, 8) / 15.0f, (float)GetRandomValue(-15, -4) / 10.0f };
        part[i].tamanhoBase = (float)GetRandomValue(2, 4);
        part[i].seedSeno = (float)GetRandomValue(0, 360);
        part[i].alpha = (float)GetRandomValue(20, 80) / 100.0f;
        part[i].velocidadeAlpha = (float)GetRandomValue(5, 15) / 1000.0f;
    }

    int paginaAtual = 0;
    int proximaPagina = 0;
    float tempoGlobal = 0.0f;
    float viradaProgresso = 1.0f; 
    float velocidadeVirada = 0.07f; 
    int estadoTransicao = 0; 
    int direcaoTransicao = 1; 

    while(!WindowShouldClose()){
        tempoGlobal += GetFrameTime();
        Vector2 mousePos = GetMousePosition();

        for (int i = 0; i < MAX_PARTICULAS; i++) {
            part[i].posicao.x += part[i].velocidade.x + sinf(tempoGlobal + part[i].seedSeno) * 0.2f;
            part[i].posicao.y += part[i].velocidade.y;
            part[i].alpha -= part[i].velocidadeAlpha;
            
            float distAoMouse = Vector2Distance(mousePos, part[i].posicao);
            if(distAoMouse < 80.0f) { part[i].posicao.x += (part[i].posicao.x - mousePos.x) * 0.03f; }

            if (part[i].alpha <= 0.0f || part[i].posicao.y < 70) {
                part[i].posicao = (Vector2){ (float)GetRandomValue(0, 1000), 760.0f };
                part[i].alpha = (float)GetRandomValue(60, 100) / 100.0f;
            }
        }

        if (estadoTransicao == 1) { 
            viradaProgresso -= velocidadeVirada;
            if (viradaProgresso <= 0.0f) { viradaProgresso = 0.0f; paginaAtual = proximaPagina; estadoTransicao = 2; }
        } else if (estadoTransicao == 2) { 
            viradaProgresso += velocidadeVirada;
            if (viradaProgresso >= 1.0f) { viradaProgresso = 1.0f; estadoTransicao = 0; }
        }

        if (estadoTransicao == 0) {
            if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)){
                proximaPagina = (paginaAtual + 1) % totalSecoes; direcaoTransicao = 1; estadoTransicao = 1;
            }
            if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)){
                proximaPagina = (paginaAtual - 1 + totalSecoes) % totalSecoes; direcaoTransicao = -1; estadoTransicao = 1;
            }

            Rectangle btnVoltar = { 5, 370, 50, 60 };
            Rectangle btnAvancar = { 945, 370, 50, 60 };

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){
                if(CheckCollisionPointRec(mousePos, btnVoltar)) { proximaPagina = (paginaAtual - 1 + totalSecoes) % totalSecoes; direcaoTransicao = -1; estadoTransicao = 1; }
                if(CheckCollisionPointRec(mousePos, btnAvancar)) { proximaPagina = (paginaAtual + 1) % totalSecoes; direcaoTransicao = 1; estadoTransicao = 1; }
            }
        }

        BeginDrawing();
        ClearBackground(COPA_AZUL_ESCURO);

        int indexFundo = paginaAtual % 3;
        // Ajustado para 1.0f para o fundo ficar totalmente nítido e visível
        DesenharFundoParallax(fundosCopa[indexFundo], mousePos, 1.0f);

        for (int i = 0; i < MAX_PARTICULAS; i++) {
            float tamPulso = part[i].tamanhoBase * (1.0f + sinf(tempoGlobal * 3.0f + part[i].seedSeno) * 0.2f);
            DrawCircleV(part[i].posicao, tamPulso, Fade(COPA_OURO_PURO, part[i].alpha * 0.5f));
        }

        float s = viradaProgresso;
        float escalaHorizontal = (s * s * (3.0f - 2.0f * s));

        for(int i = 0; i < total_mostradas; i++){
            if(paginaDaFigurinha[i] == paginaAtual){
                float originalX = (float)posicoesX[i];
                float y = (float)posicoesY[i];

                float x = originalX;
                if (estadoTransicao != 0) {
                    if (direcaoTransicao == 1) { x = 500.0f - (500.0f - originalX) * escalaHorizontal; } 
                    else { x = 500.0f + (originalX - 500.0f) * escalaHorizontal; }
                }

                Rectangle rectCard = { x, y, (float)larguraFigurinha * escalaHorizontal, (float)alturaFigurinha };
                bool mouseOver = (estadoTransicao == 0) && CheckCollisionPointRec(mousePos, rectCard);

                if (mouseOver) { rectCard.y -= 6; rectCard.x -= 3; rectCard.width += 6; rectCard.height += 6; }

                bool ehRara = (strstr(tiposFigurinha[i], "Rara") != NULL || strstr(tiposFigurinha[i], "Lendaria") != NULL || strstr(tiposFigurinha[i], "Ouro") != NULL);

                if(temImagem[i]){
                    Rectangle origem = { 0.0f, 0.0f, (float)imagens[i].width, (float)imagens[i].height };
                    DrawTexturePro(imagens[i], origem, rectCard, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);

                    if (ehRara) {
                        float pulso = (sinf(tempoGlobal * 5.0f) + 1.0f) / 2.0f;
                        Color corBorda = Fade(COPA_OURO_PURO, 0.5f + (pulso * 0.5f));
                        DrawRectangleLinesEx((Rectangle){rectCard.x-1, rectCard.y-1, rectCard.width+2, rectCard.height+2}, 2.0f, corBorda);
                        DrawRectangleLinesEx((Rectangle){rectCard.x-3, rectCard.y-3, rectCard.width+6, rectCard.height+6}, 1.0f, Fade(COPA_OURO_BRILHO, pulso * 0.3f));
                    } else {
                        DrawRectangleLinesEx(rectCard, 1.5f, mouseOver ? COPA_VERDE_NEON : Fade(BLACK, 0.5f));
                    }

                    if (mouseOver) {
                        float reflexoX = rectCard.x + ((float)sinf(tempoGlobal * 2.5f) + 1.0f) * 0.5f * rectCard.width;
                        DrawLineEx((Vector2){reflexoX, rectCard.y}, (Vector2){reflexoX + 15, rectCard.y + rectCard.height}, 3.0f, Fade(WHITE, 0.25f));
                    }
                } else {
                    DrawRectangleRec(rectCard, COPA_CINZA_CARD);
                    DrawRectangleLinesEx(rectCard, 1.5f, mouseOver ? COPA_VERDE_NEON : Fade(WHITE, 0.08f));
                    
                    if (mouseOver) { DrawRectangleRec(rectCard, Fade(COPA_AZUL_MEDIO, 0.4f)); }

                    if (escalaHorizontal > 0.25f) {
                        DrawLineEx((Vector2){rectCard.x + 4, rectCard.y + 4}, (Vector2){rectCard.x + rectCard.width - 4, rectCard.y + rectCard.height - 4}, 1.0f, Fade(WHITE, 0.02f));
                        
                        int largCod = MeasureText(codigosFigurinha[i], 16);
                        float posCodX = rectCard.x + (rectCard.width - (largCod * escalaHorizontal)) / 2.0f;
                        
                        DrawText(codigosFigurinha[i], (int)posCodX + 1, (int)(rectCard.y + (rectCard.height / 2) - 7), 16, Fade(BLACK, 0.6f));
                        DrawText(codigosFigurinha[i], (int)posCodX, (int)(rectCard.y + (rectCard.height / 2) - 8), 16, mouseOver ? COPA_VERDE_NEON : Fade(WHITE, 0.35f));
                    }
                }
            }
        }

        // --- INTERFACE DO USUÁRIO ---
        DrawRectangle(0, 0, 1000, 85, Fade(COPA_AZUL_MEDIO, 0.85f));
        DrawRectangle(0, 83, 1000, 2, COPA_OURO_PURO); 

        DrawText(secoes[paginaAtual], 32, 27, 32, Fade(BLACK, 0.4f));
        DrawText(secoes[paginaAtual], 30, 25, 32, WHITE);

        float pctConclusao = (total_figurinhas > 0) ? ((float)figurinhasColadasContador / total_figurinhas) * 100.0f : 0.0f;
        
        DrawRectangle(650, 47, 320, 10, Fade(BLACK, 0.5f));
        DrawRectangle(651, 48, (int)(318 * (pctConclusao / 100.0f)), 8, COPA_VERDE_NEON);
        DrawText(TextFormat("ALBUM PROGRESS: %.1f%% (%d/%d)", pctConclusao, figurinhasColadasContador, total_figurinhas), 650, 24, 13, Fade(WHITE, 0.7f));

        DrawText(TextFormat("PAGE %02d / %02d", paginaAtual + 1, totalSecoes), 455, 58, 15, COPA_OURO_PURO);

        Rectangle btnVoltarReal = { 5, 370, 50, 60 };
        Rectangle btnAvancarReal = { 945, 370, 50, 60 };
        
        bool overVoltar = CheckCollisionPointRec(mousePos, btnVoltarReal) && (estadoTransicao == 0);
        bool overAvancar = CheckCollisionPointRec(mousePos, btnAvancarReal) && (estadoTransicao == 0);

        DrawCircleV((Vector2){30, 400}, overVoltar ? 26 : 22, overVoltar ? COPA_VERDE_NEON : Fade(WHITE, 0.1f));
        DrawCircleV((Vector2){30, 400}, 20, COPA_AZUL_MEDIO);
        DrawText("<", 24, 387, 24, overVoltar ? COPA_VERDE_NEON : WHITE);

        DrawCircleV((Vector2){970, 400}, overAvancar ? 26 : 22, overAvancar ? COPA_VERDE_NEON : Fade(WHITE, 0.1f));
        DrawCircleV((Vector2){970, 400}, 20, COPA_AZUL_MEDIO);
        DrawText(">", 964, 387, 24, overAvancar ? COPA_VERDE_NEON : WHITE);

        DrawRectangle(0, 765, 1000, 35, COPA_AZUL_MEDIO);
        DrawLine(0, 765, 1000, 765, Fade(WHITE, 0.08f));
        int largInst = MeasureText("Seta Direita / Esquerda ou Cliques Laterais para Folhear", 14);
        DrawText("Seta Direita / Esquerda ou Cliques Laterais para Folhear", (1000 - largInst) / 2, 775, 14, Fade(WHITE, 0.5f));

        EndDrawing();
    }

    for(int i = 0; i < total_mostradas; i++){
        if(temImagem[i]){ UnloadTexture(imagens[i]); }
    }
    for (int i = 0; i < 3; i++) {
        UnloadTexture(fundosCopa[i]);
    }

    free(imagens);
    free(posicoesX);
    free(posicoesY);
    free(temImagem);
    free(paginaDaFigurinha);
    free(codigosFigurinha);
    free(tiposFigurinha);

    CloseWindow();
}