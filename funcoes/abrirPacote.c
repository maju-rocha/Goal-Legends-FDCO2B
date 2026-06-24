#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include "../headers/biblioteca.h"

int estaNoAlbum(Figurinha *album, int total_album, char codigo[]){
    for(int i = 0; i < total_album; i++) {
        if(strcmp(album[i].codigo, codigo) == 0) return 1;
    }
    return 0;
}

void limparString(char *str) {
    if (!str) return;
    int len = strlen(str);
    while(len > 0 && (str[len-1] == ' ' || str[len-1] == '\r' || str[len-1] == '\n')) str[--len] = '\0';
    int start = 0;
    while(str[start] == ' ') start++;
    if (start > 0) memmove(str, str + start, len - start + 1);
}

// Função para desenhar botões com estilo Flat Design
void DesenharBotao(Rectangle rec, const char* texto, Color corBase, Vector2 mouse, Font fonte) {
    Color corAtual = CheckCollisionPointRec(mouse, rec) ? ColorBrightness(corBase, 0.2f) : corBase;
    DrawRectangleRounded(rec, 0.3f, 10, BLACK); // Sombra
    DrawRectangleRounded((Rectangle){rec.x, rec.y, rec.width, rec.height-4}, 0.3f, 10, corAtual);
    int textWidth = MeasureTextEx(fonte, texto, 20, 2).x;
    DrawTextEx(fonte, texto, (Vector2){ rec.x + (rec.width - textWidth)/2, rec.y + 18 }, 20, 2, WHITE);
}

typedef enum { ESCOLHER_QTD, ANIMACAO, REVELAR_CARTA, FIM_ABERTURA } EstadoAbertura;

void abrirPacote(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, int *pacotes_fechados){
    if(*pacotes_fechados <= 0) return;

    InitWindow(1000, 800, "Abrindo Pacotes - Goal Legends");
    SetTargetFPS(60);
    Font fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf");
    
    int animFrames = 0;
    Image imAnim = LoadImageAnim("imagens/animacao.gif", &animFrames);
    Texture2D texAnim = LoadTextureFromImage(imAnim);
    int currentAnimFrame = 0, frameDelay = 4, frameCounter = 0;

    EstadoAbertura estado = ESCOLHER_QTD;
    int qtd_sel = 1, total_figurinhas = 0, fig_atual = 0;
    Figurinha *sort = NULL;
    int *status = NULL;
    Texture2D textura_fig = { 0 };

    while(!WindowShouldClose()) {
        Vector2 mouse = GetMousePosition();

        if (estado == ESCOLHER_QTD) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mouse, (Rectangle){350, 400, 50, 50}) && qtd_sel > 1) qtd_sel--;
                if (CheckCollisionPointRec(mouse, (Rectangle){600, 400, 50, 50}) && qtd_sel < *pacotes_fechados) qtd_sel++;
                if (CheckCollisionPointRec(mouse, (Rectangle){400, 500, 200, 60})) {
                    *pacotes_fechados -= qtd_sel;
                    total_figurinhas = qtd_sel * 7;
                    sort = malloc(total_figurinhas * sizeof(Figurinha));
                    status = malloc(total_figurinhas * sizeof(int));
                    for(int i = 0; i < total_figurinhas; i++) {
                        int idx = rand() % total;
                        sort[i] = figurinhas[idx];
                        status[i] = estaNoAlbum(album, *total_album, sort[i].codigo) ? 2 : 1;
                        if(status[i] == 1) { album[*total_album++] = sort[i]; } 
                        else { mochila[*total_mochila++] = sort[i]; }
                    }
                    estado = ANIMACAO;
                }
            }
        } else if (estado == ANIMACAO) {
            if (++frameCounter >= frameDelay) {
                if (++currentAnimFrame >= animFrames) {
                    estado = REVELAR_CARTA;
                    char pL[100], cL[100];
                    strcpy(pL, sort[fig_atual].secao); limparString(pL);
                    for(int j=0; pL[j]; j++) if(pL[j] == ' ') pL[j] = '_';
                    strcpy(cL, sort[fig_atual].codigo); limparString(cL);
                    textura_fig = LoadTexture(TextFormat("imagens/imagens_figurinhas/%s/%s.png", pL, cL));
                } else UpdateTexture(texAnim, ((unsigned char *)imAnim.data) + (imAnim.width * imAnim.height * 4 * currentAnimFrame));
                frameCounter = 0;
            }
        } else if (estado == REVELAR_CARTA) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, (Rectangle){400, 650, 200, 50})) {
                if (++fig_atual >= total_figurinhas) estado = FIM_ABERTURA;
                else {
                    UnloadTexture(textura_fig);
                    char pL[100], cL[100];
                    strcpy(pL, sort[fig_atual].secao); limparString(pL);
                    for(int j=0; pL[j]; j++) if(pL[j] == ' ') pL[j] = '_';
                    strcpy(cL, sort[fig_atual].codigo); limparString(cL);
                    textura_fig = LoadTexture(TextFormat("imagens/imagens_figurinhas/%s/%s.png", pL, cL));
                }
            }
        } else if (estado == FIM_ABERTURA && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) break;

        BeginDrawing();
        ClearBackground((Color){20, 40, 20, 255});

        if (estado == ESCOLHER_QTD) {
            DrawTextEx(fonteCopa, "QUANTOS PACOTES?", (Vector2){ 300, 100 }, 25, 2, WHITE);
            DesenharBotao((Rectangle){350, 400, 50, 50}, "-", MAROON, mouse, fonteCopa);
            DrawTextEx(fonteCopa, TextFormat("%d", qtd_sel), (Vector2){ 480, 410 }, 30, 2, WHITE);
            DesenharBotao((Rectangle){600, 400, 50, 50}, "+", BLUE, mouse, fonteCopa);
            DesenharBotao((Rectangle){400, 500, 200, 60}, "ABRIR", (Color){0, 150, 0, 255}, mouse, fonteCopa);
        } else if (estado == ANIMACAO) {
            DrawTexture(texAnim, 500 - texAnim.width/2, 400 - texAnim.height/2, WHITE);
        } else if (estado == REVELAR_CARTA) {
            DrawRectangleRounded((Rectangle){300, 100, 400, 530}, 0.08f, 20, WHITE);
            if (textura_fig.id != 0) DrawTexturePro(textura_fig, (Rectangle){0,0,textura_fig.width,textura_fig.height}, (Rectangle){320, 120, 360, 400}, (Vector2){0,0}, 0, WHITE);
            else DrawTextEx(fonteCopa, "SEM IMAGEM", (Vector2){ 350, 300 }, 20, 1, RED);
            DrawTextEx(fonteCopa, sort[fig_atual].titulo, (Vector2){ 350, 540 }, 14, 1, BLACK);
            DrawRectangleRounded((Rectangle){300, 600, 400, 40}, 0.2f, 10, status[fig_atual] == 1 ? GREEN : ORANGE);
            DesenharBotao((Rectangle){400, 650, 200, 50}, "PROXIMA", BLUE, mouse, fonteCopa);
        }
        EndDrawing();
    }
    if (sort) free(sort); if (status) free(status);
    UnloadTexture(texAnim); UnloadImage(imAnim); UnloadFont(fonteCopa); CloseWindow();
}