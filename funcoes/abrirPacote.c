#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include "../headers/biblioteca.h"

void limparEspacos(char *texto);
void limparFigurinha(Figurinha *f);
void trocarEspacoPorUnderline(char *texto);

// Paleta de Cores Premium
#define COPA_AZUL_ESCURO (Color){ 5, 7, 15, 255 }      
#define COPA_AZUL_MEDIO  (Color){ 14, 20, 38, 255 }    
#define COPA_VERDE_NEON  (Color){ 0, 255, 140, 255 }   
#define COPA_VERMELHO    (Color){ 240, 30, 70, 255 }   
#define COPA_OURO_PURO   (Color){ 255, 185, 0, 255 }   
#define COPA_OURO_BRILHO (Color){ 255, 240, 150, 255 }  
#define COPA_CINZA_CARD  (Color){ 22, 28, 48, 255 }    

typedef struct {
    Vector2 posicao;
    Vector2 velocidade;
    float tamanhoBase;
    float seedSeno;
    float alpha;
    float velocidadeAlpha;
    Color cor;
} ParticulaLuxuosa;

#define MAX_PARTICULAS_MASTER 60

typedef struct {
    int quantidadeDesejada;
    int fase;                  
    float tempoAnimacao;
    int cartaAtualIdx;
    Figurinha pacoteSorteado[7];
    bool cartasProcessadas[7]; 
    char statusMensagem[50];
    Texture2D texturaCartaAtual; 
    char codigoFotoCarregada[10]; 
    ParticulaLuxuosa particulas[MAX_PARTICULAS_MASTER];
    bool particulasInicializadas;
    float shakeIntensity;
} ControleMasterPacote;

static ControleMasterPacote fluxo = {0, 0, 0.0f, 0, {0}, {0}, "", {0}, "", {0}, false, 0.0f};

void InicializarParticulasLuxo() {
    for (int i = 0; i < MAX_PARTICULAS_MASTER; i++) {
        fluxo.particulas[i].posicao = (Vector2){ (float)GetRandomValue(0, 1000), (float)GetRandomValue(0, 800) };
        fluxo.particulas[i].velocidade = (Vector2){ (float)GetRandomValue(-8, 8) / 10.0f, (float)GetRandomValue(-15, -5) / 10.0f };
        fluxo.particulas[i].tamanhoBase = (float)GetRandomValue(2, 5);
        fluxo.particulas[i].seedSeno = (float)GetRandomValue(0, 360);
        fluxo.particulas[i].alpha = (float)GetRandomValue(30, 90) / 100.0f;
        fluxo.particulas[i].velocidadeAlpha = (float)GetRandomValue(5, 15) / 1000.0f;
        fluxo.particulas[i].cor = (GetRandomValue(0, 1) == 0) ? COPA_OURO_PURO : COPA_VERDE_NEON;
    }
    fluxo.particulasInicializadas = true;
}

void GerenciarEfeitoParticulas(Vector2 mousePos, float tempoGlobal) {
    if (!fluxo.particulasInicializadas) InicializarParticulasLuxo();

    for (int i = 0; i < MAX_PARTICULAS_MASTER; i++) {
        fluxo.particulas[i].posicao.x += fluxo.particulas[i].velocidade.x + sinf(tempoGlobal * 2.0f + fluxo.particulas[i].seedSeno) * 0.25f;
        fluxo.particulas[i].posicao.y += fluxo.particulas[i].velocidade.y;
        fluxo.particulas[i].alpha -= fluxo.particulas[i].velocidadeAlpha;

        float distAoMouse = Vector2Distance(mousePos, fluxo.particulas[i].posicao);
        if (distAoMouse < 100.0f) {
            fluxo.particulas[i].posicao = Vector2Lerp(fluxo.particulas[i].posicao, mousePos, 0.01f);
        }

        if (fluxo.particulas[i].alpha <= 0.0f || fluxo.particulas[i].posicao.y < -10 || fluxo.particulas[i].posicao.x < -10 || fluxo.particulas[i].posicao.x > 1010) {
            fluxo.particulas[i].posicao = (Vector2){ (float)GetRandomValue(0, 1000), 810.0f };
            fluxo.particulas[i].alpha = (float)GetRandomValue(50, 95) / 100.0f;
        }

        float tamPulso = fluxo.particulas[i].tamanhoBase * (1.0f + sinf(tempoGlobal * 4.0f + fluxo.particulas[i].seedSeno) * 0.2f);
        DrawCircleV(fluxo.particulas[i].posicao, tamPulso, ColorAlpha(fluxo.particulas[i].cor, fluxo.particulas[i].alpha * 0.5f));
    }
}

void gerenciarTexturaCarta(Figurinha fig) {
    if (strcmp(fluxo.codigoFotoCarregada, fig.codigo) == 0) return;

    if (fluxo.texturaCartaAtual.id > 0) {
        UnloadTexture(fluxo.texturaCartaAtual);
        fluxo.texturaCartaAtual.id = 0;
    }

    char nomePastaPais[100];
    strcpy(nomePastaPais, fig.secao);
    trocarEspacoPorUnderline(nomePastaPais);

    char caminhoFoto[256];
    sprintf(caminhoFoto, "imagens/imagens_figurinhas/%s/%s.png", nomePastaPais, fig.codigo);

    if (FileExists(caminhoFoto)) {
        Image img = LoadImage(caminhoFoto);
        ImageResize(&img, 210, 290);
        fluxo.texturaCartaAtual = LoadTextureFromImage(img);
        UnloadImage(img);
        strcpy(fluxo.codigoFotoCarregada, fig.codigo);
    } else {
        fluxo.texturaCartaAtual.id = 0;
        fluxo.codigoFotoCarregada[0] = '\0';
    }
}

void DesenharVinhetaCinema() {
    DrawRectangleGradientV(0, 0, 1000, 120, ColorAlpha(BLACK, 0.7f), ColorAlpha(BLACK, 0.0f));
    DrawRectangleGradientV(0, 680, 1000, 120, ColorAlpha(BLACK, 0.0f), ColorAlpha(BLACK, 0.8f));
}

void abrirPacoteGrafico(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, Font fonteCopa, Color azulBrasil, Color amareloBrasil, EstadoMenu *estadoAtual) {
    Vector2 mousePoint = GetMousePosition();
    float tempoGlobal = (float)GetTime();

    // Fundo Gradiente Luxuoso Dinâmico usando as cores passadas pelo menu.c
    DrawRectangleGradientV(0, 0, 1000, 800, COPA_AZUL_ESCURO, azulBrasil);

    GerenciarEfeitoParticulas(mousePoint, tempoGlobal);
    DesenharVinhetaCinema();

    // ---- FASE 0: SELEÇÃO ----
    if (fluxo.fase == 0) {
        DrawRectangle(180, 160, 640, 440, ColorAlpha(COPA_AZUL_MEDIO, 0.85f));
        DrawRectangleLinesEx((Rectangle){180, 160, 640, 440}, 2.0f, ColorAlpha(COPA_OURO_PURO, 0.5f));
        
        DrawLineEx((Vector2){175, 155}, (Vector2){210, 155}, 3.0f, COPA_OURO_PURO);
        DrawLineEx((Vector2){175, 155}, (Vector2){175, 190}, 3.0f, COPA_OURO_PURO);
        DrawLineEx((Vector2){825, 605}, (Vector2){790, 605}, 3.0f, COPA_VERDE_NEON);
        DrawLineEx((Vector2){825, 605}, (Vector2){825, 570}, 3.0f, COPA_VERDE_NEON);

        char txtDisp[50];
        sprintf(txtDisp, "BOOSTERS DISPONÍVEIS: %02d", pacotes_fechados);
        DrawTextEx(fonteCopa, txtDisp, (Vector2){ 500 - MeasureTextEx(fonteCopa, txtDisp, 15, 1).x/2, 210 }, 15, 1, ColorAlpha(WHITE, 0.5f));
        DrawTextEx(fonteCopa, "CHOOSE YOUR ENERGY QUANTITY", (Vector2){ 500 - MeasureTextEx(fonteCopa, "CHOOSE YOUR ENERGY QUANTITY", 24, 2).x/2, 245 }, 24, 2, WHITE);

        int opcoesQtd[] = {1, 3, 5};
        for (int i = 0; i < 3; i++) {
            Rectangle btn = { 220 + (i * 195), 350, 165, 70 };
            bool hover = CheckCollisionPointRec(mousePoint, btn);
            
            DrawRectangleRec(btn, hover ? ColorAlpha(COPA_VERDE_NEON, 0.12f) : ColorAlpha(COPA_AZUL_ESCURO, 0.6f));
            DrawRectangleLinesEx(btn, hover ? 2.0f : 1.0f, hover ? COPA_VERDE_NEON : COPA_OURO_PURO);
            
            char txtBtn[20];
            sprintf(txtBtn, "[ %d PACK%s ]", opcoesQtd[i], opcoesQtd[i] > 1 ? "S" : "");
            DrawTextEx(fonteCopa, txtBtn, (Vector2){ btn.x + (165/2) - MeasureTextEx(fonteCopa, txtBtn, 14, 1).x/2, btn.y + 26 }, 14, 1, hover ? COPA_VERDE_NEON : WHITE);

            if (hover && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                if (pacotes_fechados >= opcoesQtd[i]) {
                    fluxo.quantidadeDesejada = opcoesQtd[i];
                    pacotes_fechados -= 1;
                    for (int f = 0; f < 7; f++) {
                        fluxo.pacoteSorteado[f] = figurinhas[rand() % total];
                        limparFigurinha(&fluxo.pacoteSorteado[f]); 
                        fluxo.cartasProcessadas[f] = false;
                    }
                    fluxo.cartaAtualIdx = 0;
                    fluxo.fase = 1;
                } else {
                    strcpy(fluxo.statusMensagem, "INSUFFICIENT PACK CREDITS!");
                }
            }
        }

        Rectangle btnVoltar = { 400, 510, 200, 45 };
        bool hoverVoltar = CheckCollisionPointRec(mousePoint, btnVoltar);
        DrawRectangleRec(btnVoltar, hoverVoltar ? ColorAlpha(COPA_VERMELHO, 0.15f) : ColorAlpha(WHITE, 0.02f));
        DrawRectangleLinesEx(btnVoltar, 1.0f, hoverVoltar ? COPA_VERMELHO : ColorAlpha(WHITE, 0.2f));
        DrawTextEx(fonteCopa, "ABORT MISSION", (Vector2){ 500 - MeasureTextEx(fonteCopa, "ABORT MISSION", 12, 1).x/2, 526 }, 12, 1, hoverVoltar ? COPA_VERMELHO : ColorAlpha(WHITE, 0.6f));

        if (hoverVoltar && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) *estadoAtual = MENU_PRINCIPAL;
        if (strlen(fluxo.statusMensagem) > 0) DrawTextEx(fonteCopa, fluxo.statusMensagem, (Vector2){ 500 - MeasureTextEx(fonteCopa, fluxo.statusMensagem, 14, 1).x/2, 120 }, 14, 1, COPA_VERMELHO);
    }

    // ---- FASE 1: PACOTE ----
    else if (fluxo.fase == 1) {
        float glow = (sinf(tempoGlobal * 5.0f) + 1.0f) / 2.0f; 
        Rectangle pack = { 380, 130, 240, 450 }; 
        
        DrawRectangleLinesEx((Rectangle){ pack.x - 4, pack.y - 4, pack.width + 8, pack.height + 8 }, 2.0f, ColorAlpha(COPA_OURO_PURO, 0.2f + (glow * 0.4f)));
        DrawRectangleRec(pack, COPA_AZUL_MEDIO);
        DrawRectangleLinesEx(pack, 3.0f, COPA_OURO_PURO);
        DrawRectangle(395, 145, 210, 420, COPA_AZUL_ESCURO);

        float laserY = pack.y + 15 + ((float)sinf(tempoGlobal * 2.5f) + 1.0f) * 0.5f * 400.0f;
        DrawRectangleGradientH(pack.x + 15, laserY, 210, 3, ColorAlpha(COPA_VERDE_NEON, 0.0f), COPA_VERDE_NEON);

        DrawTextEx(fonteCopa, "FIFA OFFICIAL", (Vector2){ 500 - MeasureTextEx(fonteCopa, "FIFA OFFICIAL", 12, 1).x/2, 200 }, 12, 1, COPA_OURO_PURO);
        DrawTextEx(fonteCopa, "ULTIMATE\nBOOSTER", (Vector2){ 500 - MeasureTextEx(fonteCopa, "ULTIMATE\nBOOSTER", 22, 1).x/2, 320 }, 22, 1, WHITE);
        
        char press[40];
        sprintf(press, "PRESS [ SPACE ] TO UNLEASH");
        DrawTextEx(fonteCopa, press, (Vector2){ 500 - MeasureTextEx(fonteCopa, press, 14, 1).x/2, 620 }, 14, 1, ColorAlpha(COPA_VERDE_NEON, 0.6f + (glow * 0.4f)));
    
        if (IsKeyPressed(KEY_SPACE)) {
            fluxo.fase = 2;
            fluxo.tempoAnimacao = 0.0f;
        }
    }

    // ---- FASE 2: EXPLOSÃO ----
    else if (fluxo.fase == 2) {
        fluxo.tempoAnimacao += GetFrameTime() * 2.2f;
        float t = fluxo.tempoAnimacao;

        float shake = (t < 0.20f) ? (1.0f - (t/0.20f)) * 14.0f : 0.0f;
        float skX = (float)GetRandomValue(-1, 1) * shake;
        float skY = (float)GetRandomValue(-1, 1) * shake;

        float afastamento = (t >= 0.20f) ? powf(t - 0.20f, 2.5f) * 900.0f : 0.0f;
        float queda = (t >= 0.20f) ? (t - 0.20f) * 450.0f : 0.0f;
        float rot = (t >= 0.20f) ? (t - 0.20f) * 110.0f : 0.0f;

        DrawRectanglePro((Rectangle){ 380 + skX - afastamento, 130 + skY + queda, 120, 450 }, (Vector2){ 120, 225 }, -rot, COPA_AZUL_MEDIO);
        DrawRectanglePro((Rectangle){ 500 + skX + afastamento, 130 + skY + queda, 120, 450 }, (Vector2){ 0, 225 }, rot, COPA_AZUL_MEDIO);

        if (t >= 0.15f && t <= 0.40f) {
            float shockAlpha = (1.0f - ((t - 0.15f) / 0.25f));
            DrawCircle(500, 350, (t - 0.15f) * 600.0f, ColorAlpha(WHITE, shockAlpha * 0.4f));
            DrawLineEx((Vector2){500 + skX, 100}, (Vector2){500 + skX, 600}, shockAlpha * 25.0f, COPA_VERDE_NEON);
        }

        if (fluxo.tempoAnimacao > 1.0f) {
            fluxo.fase = 3;
            fluxo.tempoAnimacao = 0.0f;
            fluxo.codigoFotoCarregada[0] = '\0'; 
        }
    }

    // ---- FASE 3: CARROSSEL ----
    else if (fluxo.fase == 3) {
        Figurinha fig = fluxo.pacoteSorteado[fluxo.cartaAtualIdx];
        gerenciarTexturaCarta(fig);

        Rectangle card = { 365, 100, 270, 520 };
        
        float tiltX = ((mousePoint.x - 500) / 500.0f) * 15.0f;
        float tiltY = ((mousePoint.y - 360) / 360.0f) * 12.0f;
        card.x += tiltX;
        card.y += tiltY + (sinf(tempoGlobal * 2.0f) * 3.0f); 

        DrawRectangleRec((Rectangle){ card.x + 12, card.y + 12, card.width, card.height }, ColorAlpha(BLACK, 0.4f));
        DrawRectangleRec(card, COPA_CINZA_CARD);
        
        bool ehRara = (strstr(fig.tipo, "Rara") != NULL || strstr(fig.tipo, "Lendaria") != NULL || strstr(fig.tipo, "Ouro") != NULL);
        Color corBorda = COPA_VERDE_NEON;
        
        if (!fluxo.cartasProcessadas[fluxo.cartaAtualIdx]) {
            if (ehRara) {
                float r = (sinf(tempoGlobal * 3.0f) + 1.0f) * 0.5f;
                corBorda = ColorLerp(COPA_OURO_PURO, COPA_OURO_BRILHO, r);
                DrawRectangleLinesEx((Rectangle){card.x - 2, card.y - 2, card.width + 4, card.height + 4}, 2.0f, corBorda);
            } else {
                corBorda = ColorAlpha(WHITE, 0.25f);
                DrawRectangleLinesEx(card, 2.0f, corBorda);
            }
        } else {
            corBorda = ColorAlpha(WHITE, 0.08f);
            DrawRectangleLinesEx(card, 2.0f, corBorda);
        }

        if (fluxo.texturaCartaAtual.id > 0) {
            DrawTexture(fluxo.texturaCartaAtual, card.x + 30, card.y + 60, WHITE);
            DrawRectangleLinesEx((Rectangle){ card.x + 29, card.y + 59, 212, 292 }, 1.5f, ColorAlpha(WHITE, 0.1f));
            
            float reflexoX = card.x + 30 + ((mousePoint.x / 1000.0f) * 210.0f);
            DrawLineEx((Vector2){reflexoX, card.y + 60}, (Vector2){reflexoX - 30, card.y + 350}, 3.0f, ColorAlpha(WHITE, 0.15f));
        } else {
            DrawRectangle(card.x + 30, card.y + 60, 210, 290, COPA_AZUL_ESCURO);
            DrawRectangleLinesEx((Rectangle){ card.x + 30, card.y + 60, 210, 290 }, 1.0f, ColorAlpha(WHITE, 0.04f));
        }

        DrawRectangle(card.x + 20, card.y + 18, 80, 24, COPA_AZUL_MEDIO);
        DrawRectangleLinesEx((Rectangle){card.x + 20, card.y + 18, 80, 24}, 1.0f, corBorda);
        DrawTextEx(fonteCopa, fig.codigo, (Vector2){ (card.x + 60) - MeasureTextEx(fonteCopa, fig.codigo, 11, 1).x/2, card.y + 24 }, 11, 1, ehRara ? COPA_OURO_PURO : WHITE);

        DrawTextEx(fonteCopa, fig.titulo, (Vector2){ (card.x + 135) - MeasureTextEx(fonteCopa, fig.titulo, 15, 1).x/2, card.y + 368 }, 15, 1, WHITE);
        DrawTextEx(fonteCopa, fig.secao, (Vector2){ (card.x + 135) - MeasureTextEx(fonteCopa, fig.secao, 11, 1).x/2, card.y + 395 }, 11, 1, ColorAlpha(WHITE, 0.4f));
        DrawTextEx(fonteCopa, fig.tipo, (Vector2){ (card.x + 135) - MeasureTextEx(fonteCopa, fig.tipo, 11, 1).x/2, card.y + 418 }, 11, 1, ehRara ? COPA_OURO_PURO : COPA_VERDE_NEON);

        if (!fluxo.cartasProcessadas[fluxo.cartaAtualIdx]) {
            DrawRectangle(card.x + 15, card.y + 445, 240, 60, COPA_AZUL_MEDIO);
            DrawRectangleLinesEx((Rectangle){card.x + 15, card.y + 445, 240, 60}, 1.0f, ColorAlpha(WHITE, 0.05f));
            
            DrawTextEx(fonteCopa, "[A] SLOT TO ALBUM", (Vector2){ card.x + 25, card.y + 452 }, 10, 1, WHITE);
            DrawTextEx(fonteCopa, "[M] KEEP IN MOCHILA", (Vector2){ card.x + 25, card.y + 469 }, 10, 1, WHITE);
            DrawTextEx(fonteCopa, "[D] DISCARD CARD", (Vector2){ card.x + 25, card.y + 486 }, 10, 1, COPA_VERMELHO);

            if (IsKeyPressed(KEY_A)) {
                album[*total_album] = fig; (*total_album)++;
                fluxo.cartasProcessadas[fluxo.cartaAtualIdx] = true;
                strcpy(fluxo.statusMensagem, "COLADA NO ALBUM!");
                FILE *fio = fopen("extras/album.csv", "a");
                if (fio) { fprintf(fio, "%s,%s,%s,%s,%s\n", fig.codigo, fig.titulo, fig.secao, fig.grupo, fig.tipo); fclose(fio); }
            }
            if (IsKeyPressed(KEY_M)) {
                mochila[*total_mochila] = fig; (*total_mochila)++;
                fluxo.cartasProcessadas[fluxo.cartaAtualIdx] = true;
                strcpy(fluxo.statusMensagem, "GUARDADA!");
                FILE *fio = fopen("extras/mochila.csv", "a");
                if (fio) { fprintf(fio, "%s,%s,%s,%s,%s\n", fig.codigo, fig.titulo, fig.secao, fig.grupo, fig.tipo); fclose(fio); }
            }
            if (IsKeyPressed(KEY_D)) {
                fluxo.cartasProcessadas[fluxo.cartaAtualIdx] = true;
                strcpy(fluxo.statusMensagem, "CARD FOI DESCARTADO");
            }
        } else {
            DrawRectangle(card.x + 15, card.y + 445, 240, 60, ColorAlpha(COPA_AZUL_MEDIO, 0.4f));
            // Erro corrigido aqui de fluxes -> fluxo
            DrawTextEx(fonteCopa, fluxo.statusMensagem, (Vector2){ (card.x + 135) - MeasureTextEx(fonteCopa, fluxo.statusMensagem, 11, 1).x/2, card.y + 469 }, 11, 1, COPA_VERDE_NEON);
        }

        if (fluxo.cartaAtualIdx > 0) DrawTextEx(fonteCopa, "<", (Vector2){ 290, 350 }, 26, 1, ColorAlpha(WHITE, 0.3f));
        if (fluxo.cartaAtualIdx < 6) DrawTextEx(fonteCopa, ">", (Vector2){ 690, 350 }, 26, 1, ColorAlpha(WHITE, 0.3f));

        char indicador[32];
        sprintf(indicador, "TRACK: %d / 7", fluxo.cartaAtualIdx + 1);
        DrawTextEx(fonteCopa, indicador, (Vector2){ 500 - MeasureTextEx(fonteCopa, indicador, 13, 1).x/2, 640 }, 13, 1, ColorAlpha(WHITE, 0.5f));

        if (IsKeyPressed(KEY_RIGHT) && fluxo.cartaAtualIdx < 6) {
            fluxo.cartaAtualIdx++;
            strcpy(fluxo.statusMensagem, fluxo.cartasProcessadas[fluxo.cartaAtualIdx] ? "AÇÃO JÁ PROCESSADA" : "");
        }
        if (IsKeyPressed(KEY_LEFT) && fluxo.cartaAtualIdx > 0) {
            fluxo.cartaAtualIdx--;
            strcpy(fluxo.statusMensagem, fluxo.cartasProcessadas[fluxo.cartaAtualIdx] ? "AÇÃO JÁ PROCESSADA" : "");
        }

        bool todasProntas = true;
        for (int c = 0; c < 7; c++) { if (!fluxo.cartasProcessadas[c]) todasProntas = false; }

        if (todasProntas) {
            float pulseEnter = (sinf(tempoGlobal * 6.0f) + 1.0f) / 2.0f;
            DrawTextEx(fonteCopa, "PRESS [ ENTER ] FOR NEXT LEVEL", (Vector2){ 500 - MeasureTextEx(fonteCopa, "PRESS [ ENTER ] FOR NEXT LEVEL", 13, 1).x/2, 675 }, 13, 1, ColorAlpha(COPA_OURO_PURO, 0.5f + pulseEnter * 0.5f));
            
            if (IsKeyPressed(KEY_ENTER)) {
                fluxo.quantidadeDesejada--;
                if (fluxo.quantidadeDesejada > 0 && pacotes_fechados > 0) {
                    pacotes_fechados--;
                    for (int f = 0; f < 7; f++) {
                        fluxo.pacoteSorteado[f] = figurinhas[rand() % total];
                        limparFigurinha(&fluxo.pacoteSorteado[f]);
                        fluxo.cartasProcessadas[f] = false;
                    }
                    fluxo.cartaAtualIdx = 0;
                    fluxo.fase = 1;
                } else {
                    if (fluxo.texturaCartaAtual.id > 0) UnloadTexture(fluxo.texturaCartaAtual);
                    fluxo.texturaCartaAtual.id = 0;
                    fluxo.codigoFotoCarregada[0] = '\0';
                    fluxo.fase = 0;
                    *estadoAtual = MENU_PRINCIPAL;
                }
            }
        }
    }
}