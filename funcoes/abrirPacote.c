#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include "biblioteca.h"


void abrirPacote(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, int *pacotes_fechados) {
    if (*pacotes_fechados <= 0) return;

    InitWindow(1000, 800, "Goal Legends - Abrindo Pacote");
    SetTargetFPS(60);
    Font fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf");
    
    // --- CARREGAMENTO DO GIF COM REDIMENSIONAMENTO ---
    int animFrames = 0;
    Image imAnim = LoadImageAnim("imagens/animacao.gif", &animFrames);
    // Redimensionamos a imagem para um tamanho que cabe na tela (ex: 500x500)
    ImageResize(&imAnim, 500, 500); 
    Texture2D texAnim = LoadTextureFromImage(imAnim);
    
    int currentAnimFrame = 0;
    int frameDelay = 8; // Ajusta este valor (maior = mais lento)
    int frameCounter = 0;

    EstadoAbertura estado = ESCOLHER_QTD;
    int qtd_abrir = 1, total_figurinhas = 0, figurinha_atual = 0;
    Figurinha *sorteadas_array = NULL;
    int *status_sorteadas = NULL;
    Texture2D texFig = {0};

    while (!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        // Lógica de Estados
        if (estado == ESCOLHER_QTD) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, (Rectangle){350, 400, 50, 50}) && qtd_abrir > 1) qtd_abrir--;
                if (CheckCollisionPointRec(mouse, (Rectangle){600, 400, 50, 50}) && qtd_abrir < *pacotes_fechados) qtd_abrir++;
                if (CheckCollisionPointRec(mouse, (Rectangle){400, 500, 200, 60})) {
                    *pacotes_fechados -= qtd_abrir;
                    total_figurinhas = qtd_abrir * 7;
                    sorteadas_array = malloc(total_figurinhas * sizeof(Figurinha));
                    status_sorteadas = malloc(total_figurinhas * sizeof(int));
                    for(int i=0; i<total_figurinhas; i++) {
                        int idx = rand() % total;
                        sorteadas_array[i] = figurinhas[idx];
                        if(!estaNoAlbum(album, *total_album, sorteadas_array[i].codigo)) {
                            album[*total_album++] = sorteadas_array[i];
                            status_sorteadas[i] = 1;
                        } else {
                            mochila[*total_mochila++] = sorteadas_array[i];
                            status_sorteadas[i] = 2;
                        }
                    }
                    estado = ANIMACAO;
                }
            }
        } 
        else if (estado == ANIMACAO) {
            frameCounter++;
            if (frameCounter >= frameDelay) {
                currentAnimFrame++;
                if (currentAnimFrame >= animFrames) {
                    estado = REVELAR_CARTA;
                    char path[200];
                    sprintf(path, "imagens/imagens_figurinhas/%s/%s.png", sorteadas_array[0].secao, sorteadas_array[0].codigo);
                    if(FileExists(path)) texFig = LoadTexture(path);
                } else {
                    // Atualiza a textura com o frame específico do GIF redimensionado
                    UpdateTexture(texAnim, ((unsigned char *)imAnim.data) + (imAnim.width * imAnim.height * 4 * currentAnimFrame));
                }
                frameCounter = 0;
            }
        }
        else if (estado == REVELAR_CARTA && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (++figurinha_atual >= total_figurinhas) estado = FIM_ABERTURA;
            else {
                if(texFig.id > 0) UnloadTexture(texFig);
                char path[200];
                sprintf(path, "imagens/imagens_figurinhas/%s/%s.png", sorteadas_array[figurinha_atual].secao, sorteadas_array[figurinha_atual].codigo);
                if(FileExists(path)) texFig = LoadTexture(path);
            }
        }
        else if (estado == FIM_ABERTURA && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) break;

        // Desenho
        BeginDrawing();
        ClearBackground((Color){20, 40, 20, 255});

        if (estado == ESCOLHER_QTD) {
            DrawTextEx(fonteCopa, "QTD PACOTES:", (Vector2){300, 300}, 20, 2, WHITE);
            DrawTextEx(fonteCopa, TextFormat("%d", qtd_abrir), (Vector2){500, 300}, 20, 2, YELLOW);
            DrawRectangle(400, 500, 200, 60, GOLD);
            DrawText("ABRIR", 460, 520, 20, BLACK);
        } else if (estado == ANIMACAO) {
            // Desenha centralizado (500 é o meio da tela, subtraímos metade da largura do GIF)
            DrawTexture(texAnim, 500 - 250, 400 - 250, WHITE);
        } else if (estado == REVELAR_CARTA) {
            if (texFig.id > 0) DrawTexturePro(texFig, (Rectangle){0,0,texFig.width,texFig.height}, (Rectangle){320, 120, 360, 400}, (Vector2){0,0}, 0, WHITE);
            DrawTextEx(fonteCopa, "CLIQUE PARA PROXIMA", (Vector2){350, 700}, 20, 1, WHITE);
        } else if (estado == FIM_ABERTURA) {
            DrawTextEx(fonteCopa, "CONCLUIDO!", (Vector2){400, 400}, 30, 2, WHITE);
        }
        EndDrawing();
    }

    // Limpeza
    if(sorteadas_array) free(sorteadas_array);
    if(status_sorteadas) free(status_sorteadas);
    if(texFig.id > 0) UnloadTexture(texFig);
    UnloadTexture(texAnim); UnloadImage(imAnim); UnloadFont(fonteCopa); CloseWindow();
    salvarPacotes(*pacotes_fechados);
}