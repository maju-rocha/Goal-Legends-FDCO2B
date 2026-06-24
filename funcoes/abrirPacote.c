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

// Estrutura atualizada
typedef struct {
    int quantidadeDesejada;
    int fase;                  
    float tempoAnimacao;
    int cartaAtualIdx;
    Figurinha pacoteSorteado[7];
    int statusCartas[7]; // 1 = NOVA, 2 = REPETIDA
    char statusMensagem[50];
    Texture2D texturaCartaAtual; 
    char codigoFotoCarregada[10]; 
    ParticulaLuxuosa particulas[MAX_PARTICULAS_MASTER];
    bool particulasInicializadas;
    float shakeIntensity;
    
    // -- Variáveis do GIF --
    Image gifImage;
    Texture2D gifTextura;
    int animFrames;
    int currentAnimFrame;
    int frameCounter;
    bool gifCarregado;
} ControleMasterPacote;

// Inicializa tudo a zero
static ControleMasterPacote fluxo = {0};

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

// Cabeçalho corrigido: removemos o int *pacotes_fechados do final
void abrirPacoteGrafico(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, Font fonteCopa, Color azulBrasil, Color amareloBrasil, EstadoMenu *estadoAtual) {
    Vector2 mousePoint = GetMousePosition();
    float tempoGlobal = (float)GetTime();

    if (!fluxo.gifCarregado) {
        fluxo.animFrames = 0;
        fluxo.gifImage = LoadImageAnim("imagens/animacao.gif", &fluxo.animFrames);
        fluxo.gifTextura = LoadTextureFromImage(fluxo.gifImage);
        fluxo.currentAnimFrame = 0;
        fluxo.frameCounter = 0;
        fluxo.gifCarregado = true;
    }

    DrawRectangleGradientV(0, 0, 1000, 800, COPA_AZUL_ESCURO, azulBrasil);
    GerenciarEfeitoParticulas(mousePoint, tempoGlobal);
    DesenharVinhetaCinema();

    // ---- FASE 0: SELEÇÃO COM BOTÕES + E - ----
    if (fluxo.fase == 0) {
        if (fluxo.quantidadeDesejada < 1) fluxo.quantidadeDesejada = 1;
        if (fluxo.quantidadeDesejada > pacotes_fechados && pacotes_fechados > 0) fluxo.quantidadeDesejada = pacotes_fechados;
        if (pacotes_fechados == 0) fluxo.quantidadeDesejada = 0;

        DrawRectangle(180, 160, 640, 440, ColorAlpha(COPA_AZUL_MEDIO, 0.85f));
        DrawRectangleLinesEx((Rectangle){180, 160, 640, 440}, 2.0f, ColorAlpha(COPA_OURO_PURO, 0.5f));
        
        DrawLineEx((Vector2){175, 155}, (Vector2){210, 155}, 3.0f, COPA_OURO_PURO);
        DrawLineEx((Vector2){175, 155}, (Vector2){175, 190}, 3.0f, COPA_OURO_PURO);
        DrawLineEx((Vector2){825, 605}, (Vector2){790, 605}, 3.0f, COPA_VERDE_NEON);
        DrawLineEx((Vector2){825, 605}, (Vector2){825, 570}, 3.0f, COPA_VERDE_NEON);

        char txtDisp[50];
        sprintf(txtDisp, "PACOTES DISPONÍVEIS: %02d", pacotes_fechados);
        DrawTextEx(fonteCopa, txtDisp, (Vector2){ 500 - MeasureTextEx(fonteCopa, txtDisp, 15, 1).x/2, 210 }, 15, 1, ColorAlpha(WHITE, 0.5f));
        DrawTextEx(fonteCopa, "SELECIONE A QUANTIDADE", (Vector2){ 500 - MeasureTextEx(fonteCopa, "SELECIONE A QUANTIDADE", 22, 2).x/2, 255 }, 22, 2, WHITE);

        Rectangle btnMenos = { 330, 320, 60, 60 };
        Rectangle boxQtd = { 420, 320, 160, 60 };
        Rectangle btnMais = { 610, 320, 60, 60 };

        bool hoverMenos = CheckCollisionPointRec(mousePoint, btnMenos);
        bool hoverMais = CheckCollisionPointRec(mousePoint, btnMais);

        DrawRectangleRec(btnMenos, hoverMenos ? ColorAlpha(COPA_VERMELHO, 0.2f) : ColorAlpha(COPA_AZUL_ESCURO, 0.6f));
        DrawRectangleLinesEx(btnMenos, hoverMenos ? 2.0f : 1.0f, hoverMenos ? COPA_VERMELHO : COPA_OURO_PURO);
        DrawTextEx(fonteCopa, "-", (Vector2){ btnMenos.x + 30 - MeasureTextEx(fonteCopa, "-", 24, 1).x/2, btnMenos.y + 18 }, 24, 1, WHITE);

        DrawRectangleRec(boxQtd, ColorAlpha(BLACK, 0.4f));
        DrawRectangleLinesEx(boxQtd, 1.0f, ColorAlpha(WHITE, 0.2f));
        char txtQtd[10];
        sprintf(txtQtd, "%02d", fluxo.quantidadeDesejada);
        DrawTextEx(fonteCopa, txtQtd, (Vector2){ boxQtd.x + 80 - MeasureTextEx(fonteCopa, txtQtd, 32, 2).x/2, boxQtd.y + 14 }, 32, 2, COPA_VERDE_NEON);

        DrawRectangleRec(btnMais, hoverMais ? ColorAlpha(COPA_VERDE_NEON, 0.2f) : ColorAlpha(COPA_AZUL_ESCURO, 0.6f));
        DrawRectangleLinesEx(btnMais, hoverMais ? 2.0f : 1.0f, hoverMais ? COPA_VERDE_NEON : COPA_OURO_PURO);
        DrawTextEx(fonteCopa, "+", (Vector2){ btnMais.x + 30 - MeasureTextEx(fonteCopa, "+", 24, 1).x/2, btnMais.y + 18 }, 24, 1, WHITE);

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (hoverMenos && fluxo.quantidadeDesejada > 1) fluxo.quantidadeDesejada--;
            if (hoverMais && fluxo.quantidadeDesejada < pacotes_fechados) fluxo.quantidadeDesejada++;
        }

        Rectangle btnAbrir = { 400, 420, 200, 50 };
        bool hoverAbrir = CheckCollisionPointRec(mousePoint, btnAbrir);
        DrawRectangleRec(btnAbrir, hoverAbrir ? ColorAlpha(COPA_OURO_PURO, 0.2f) : ColorAlpha(COPA_OURO_PURO, 0.05f));
        DrawRectangleLinesEx(btnAbrir, hoverAbrir ? 2.0f : 1.0f, COPA_OURO_PURO);
        DrawTextEx(fonteCopa, "ABRIR PACOTES", (Vector2){ 500 - MeasureTextEx(fonteCopa, "ABRIR PACOTES", 16, 1).x/2, btnAbrir.y + 17 }, 16, 1, hoverAbrir ? COPA_OURO_PURO : WHITE);

        if (hoverAbrir && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            if (pacotes_fechados > 0 && fluxo.quantidadeDesejada > 0) {
                pacotes_fechados -= 1;
                salvarPacotes(); // CORRIGIDO: Chama salvarPacotes sem parâmetros
                
                for (int f = 0; f < 7; f++) {
                    Figurinha sorteada = figurinhas[rand() % total];
                    limparFigurinha(&sorteada); 
                    fluxo.pacoteSorteado[f] = sorteada;
                    
                    if (!estaNoAlbum(album, *total_album, sorteada.codigo)) {
                        album[*total_album] = sorteada;
                        (*total_album)++;
                        FILE *fio = fopen("extras/album.csv", "a");
                        if (fio) { fprintf(fio, "%s,%s,%s,%s,%s\n", sorteada.codigo, sorteada.titulo, sorteada.secao, sorteada.grupo, sorteada.tipo); fclose(fio); }
                        fluxo.statusCartas[f] = 1; 
                    } else {
                        mochila[*total_mochila] = sorteada;
                        (*total_mochila)++;
                        FILE *fio = fopen("extras/mochila.csv", "a");
                        if (fio) { fprintf(fio, "%s,%s,%s,%s,%s\n", sorteada.codigo, sorteada.titulo, sorteada.secao, sorteada.grupo, sorteada.tipo); fclose(fio); }
                        fluxo.statusCartas[f] = 2; 
                        salvarRepetida();
                    }
                }
                fluxo.cartaAtualIdx = 0;
                fluxo.fase = 1;
                strcpy(fluxo.statusMensagem, ""); 
            } else {
                strcpy(fluxo.statusMensagem, "PACOTES INSUFICIENTES!");
            }
        }

        Rectangle btnVoltar = { 400, 510, 200, 45 };
        bool hoverVoltar = CheckCollisionPointRec(mousePoint, btnVoltar);
        DrawRectangleRec(btnVoltar, hoverVoltar ? ColorAlpha(COPA_VERMELHO, 0.15f) : ColorAlpha(WHITE, 0.02f));
        DrawRectangleLinesEx(btnVoltar, 1.0f, hoverVoltar ? COPA_VERMELHO : ColorAlpha(WHITE, 0.2f));
        DrawTextEx(fonteCopa, "VOLTAR AO MENU", (Vector2){ 500 - MeasureTextEx(fonteCopa, "VOLTAR AO MENU", 12, 1).x/2, 526 }, 12, 1, hoverVoltar ? COPA_VERMELHO : ColorAlpha(WHITE, 0.6f));

        if (hoverVoltar && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            if (fluxo.gifCarregado) {
                UnloadTexture(fluxo.gifTextura);
                UnloadImage(fluxo.gifImage);
                fluxo.gifCarregado = false;
            }
            fluxo.quantidadeDesejada = 1; 
            *estadoAtual = MENU_PRINCIPAL;
        }
        
        if (strlen(fluxo.statusMensagem) > 0) DrawTextEx(fonteCopa, fluxo.statusMensagem, (Vector2){ 500 - MeasureTextEx(fonteCopa, fluxo.statusMensagem, 14, 1).x/2, 120 }, 14, 1, COPA_VERMELHO);
    }

    // ---- FASE 1: PACOTE ESPERANDO ----
    else if (fluxo.fase == 1) {
        float glow = (sinf(tempoGlobal * 5.0f) + 1.0f) / 2.0f; 
        Rectangle pack = { 380, 130, 240, 450 }; 
        
        DrawRectangleLinesEx((Rectangle){ pack.x - 4, pack.y - 4, pack.width + 8, pack.height + 8 }, 2.0f, ColorAlpha(COPA_OURO_PURO, 0.2f + (glow * 0.4f)));
        DrawRectangleRec(pack, COPA_AZUL_MEDIO);
        DrawRectangleLinesEx(pack, 3.0f, COPA_OURO_PURO);
        DrawRectangle(395, 145, 210, 420, COPA_AZUL_ESCURO);

        float laserY = pack.y + 15 + ((float)sinf(tempoGlobal * 2.5f) + 1.0f) * 0.5f * 400.0f;
        DrawRectangleGradientH(pack.x + 15, laserY, 210, 3, ColorAlpha(COPA_VERDE_NEON, 0.0f), COPA_VERDE_NEON);

        DrawTextEx(fonteCopa, "FIFA OFICIAL", (Vector2){ 500 - MeasureTextEx(fonteCopa, "FIFA OFICIAL", 12, 1).x/2, 200 }, 12, 1, COPA_OURO_PURO);
        DrawTextEx(fonteCopa, "PACOTE\nULTIMATE", (Vector2){ 500 - MeasureTextEx(fonteCopa, "PACOTE\nULTIMATE", 22, 1).x/2, 320 }, 22, 1, WHITE);
        
        char press[50];
        sprintf(press, "APERTE [ ESPACO ] PARA ABRIR");
        DrawTextEx(fonteCopa, press, (Vector2){ 500 - MeasureTextEx(fonteCopa, press, 14, 1).x/2, 620 }, 14, 1, ColorAlpha(COPA_VERDE_NEON, 0.6f + (glow * 0.4f)));
    
        if (IsKeyPressed(KEY_SPACE)) {
            fluxo.fase = 2;
            fluxo.currentAnimFrame = 0; 
        }
    }

    // ---- FASE 2: ANIMAÇÃO DO GIF ----
    else if (fluxo.fase == 2) {
        fluxo.frameCounter++;
        
        if (fluxo.frameCounter >= 6) { 
            fluxo.currentAnimFrame++;
            
            if (fluxo.currentAnimFrame >= fluxo.animFrames) {
                fluxo.fase = 3;
                fluxo.currentAnimFrame = 0;
                fluxo.codigoFotoCarregada[0] = '\0';
            } else {
                UpdateTexture(fluxo.gifTextura, ((unsigned char *)fluxo.gifImage.data) + (fluxo.gifImage.width * fluxo.gifImage.height * 4 * fluxo.currentAnimFrame));
            }
            fluxo.frameCounter = 0;
        }

        float glow = (sinf(tempoGlobal * 8.0f) + 1.0f) / 2.0f;
        DrawCircle(500, 400, 240.0f + (glow * 15.0f), ColorAlpha(COPA_OURO_PURO, 0.15f));
        
        Rectangle origem = { 0.0f, 0.0f, (float)fluxo.gifTextura.width, (float)fluxo.gifTextura.height };
        Rectangle destino = { 375.0f, 190.0f, 250.0f, 420.0f }; 
        Vector2 centro = { 0.0f, 0.0f };
        
        DrawTexturePro(fluxo.gifTextura, origem, destino, centro, 0.0f, WHITE);
    }

    // ---- FASE 3: CARROSSEL DE CARTAS ----
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
        
        if (fluxo.statusCartas[fluxo.cartaAtualIdx] == 1) { // NOVA
            if (ehRara) {
                float r = (sinf(tempoGlobal * 3.0f) + 1.0f) * 0.5f;
                corBorda = ColorLerp(COPA_OURO_PURO, COPA_OURO_BRILHO, r);
                DrawRectangleLinesEx((Rectangle){card.x - 2, card.y - 2, card.width + 4, card.height + 4}, 2.0f, corBorda);
            } else {
                corBorda = ColorAlpha(COPA_VERDE_NEON, 0.7f);
                DrawRectangleLinesEx(card, 2.0f, corBorda);
            }
        } else { // REPETIDA
            corBorda = ColorAlpha(WHITE, 0.2f);
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

        if (fluxo.statusCartas[fluxo.cartaAtualIdx] == 1) { 
            DrawRectangle(card.x + 15, card.y + 445, 240, 60, ColorAlpha(COPA_VERDE_NEON, 0.15f));
            DrawRectangleLinesEx((Rectangle){card.x + 15, card.y + 445, 240, 60}, 1.0f, COPA_VERDE_NEON);
            DrawTextEx(fonteCopa, "NOVA FIGURINHA!", (Vector2){ (card.x + 135) - MeasureTextEx(fonteCopa, "NOVA FIGURINHA!", 12, 1).x/2, card.y + 458 }, 12, 1, COPA_VERDE_NEON);
            DrawTextEx(fonteCopa, "ADICIONADA AO ALBUM", (Vector2){ (card.x + 135) - MeasureTextEx(fonteCopa, "ADICIONADA AO ALBUM", 10, 1).x/2, card.y + 480 }, 10, 1, WHITE);
        } else { 
            DrawRectangle(card.x + 15, card.y + 445, 240, 60, ColorAlpha(COPA_AZUL_MEDIO, 0.6f));
            DrawRectangleLinesEx((Rectangle){card.x + 15, card.y + 445, 240, 60}, 1.0f, ColorAlpha(WHITE, 0.3f));
            DrawTextEx(fonteCopa, "REPETIDA", (Vector2){ (card.x + 135) - MeasureTextEx(fonteCopa, "REPETIDA", 12, 1).x/2, card.y + 458 }, 12, 1, ColorAlpha(WHITE, 0.7f));
            DrawTextEx(fonteCopa, "ENVIADA PARA A MOCHILA", (Vector2){ (card.x + 135) - MeasureTextEx(fonteCopa, "ENVIADA PARA A MOCHILA", 10, 1).x/2, card.y + 480 }, 10, 1, ColorAlpha(WHITE, 0.5f));
        }

        if (fluxo.cartaAtualIdx > 0) DrawTextEx(fonteCopa, "<", (Vector2){ 290, 350 }, 26, 1, ColorAlpha(WHITE, 0.3f));
        if (fluxo.cartaAtualIdx < 6) DrawTextEx(fonteCopa, ">", (Vector2){ 690, 350 }, 26, 1, ColorAlpha(WHITE, 0.3f));

        char indicador[32];
        sprintf(indicador, "FIGURINHA: %d / 7", fluxo.cartaAtualIdx + 1);
        DrawTextEx(fonteCopa, indicador, (Vector2){ 500 - MeasureTextEx(fonteCopa, indicador, 13, 1).x/2, 640 }, 13, 1, ColorAlpha(WHITE, 0.5f));

        if (IsKeyPressed(KEY_RIGHT) && fluxo.cartaAtualIdx < 6) {
            fluxo.cartaAtualIdx++;
        }
        if (IsKeyPressed(KEY_LEFT) && fluxo.cartaAtualIdx > 0) {
            fluxo.cartaAtualIdx--;
        }

        float pulseEnter = (sinf(tempoGlobal * 6.0f) + 1.0f) / 2.0f;
        DrawTextEx(fonteCopa, "APERTE [ ENTER ] PARA CONTINUAR", (Vector2){ 500 - MeasureTextEx(fonteCopa, "APERTE [ ENTER ] PARA CONTINUAR", 13, 1).x/2, 675 }, 13, 1, ColorAlpha(COPA_OURO_PURO, 0.5f + pulseEnter * 0.5f));
        
        if (IsKeyPressed(KEY_ENTER)) {
            fluxo.quantidadeDesejada--;
            if (fluxo.quantidadeDesejada > 0 && pacotes_fechados > 0) {
                pacotes_fechados--;
                salvarPacotes(); // CORRIGIDO: Chama salvarPacotes sem parâmetros
                
                for (int f = 0; f < 7; f++) {
                    Figurinha sorteada = figurinhas[rand() % total];
                    limparFigurinha(&sorteada);
                    fluxo.pacoteSorteado[f] = sorteada;
                    
                    if (!estaNoAlbum(album, *total_album, sorteada.codigo)) {
                        album[*total_album] = sorteada;
                        (*total_album)++;
                        FILE *fio = fopen("extras/album.csv", "a");
                        if (fio) { fprintf(fio, "%s,%s,%s,%s,%s\n", sorteada.codigo, sorteada.titulo, sorteada.secao, sorteada.grupo, sorteada.tipo); fclose(fio); }
                        fluxo.statusCartas[f] = 1;
                    } else {
                        mochila[*total_mochila] = sorteada;
                        (*total_mochila)++;
                        FILE *fio = fopen("extras/mochila.csv", "a");
                        if (fio) { fprintf(fio, "%s,%s,%s,%s,%s\n", sorteada.codigo, sorteada.titulo, sorteada.secao, sorteada.grupo, sorteada.tipo); fclose(fio); }
                        fluxo.statusCartas[f] = 2;
                        salvarRepetida();
                    }
                }
                fluxo.cartaAtualIdx = 0;
                fluxo.fase = 1; 
            } else {
                if (fluxo.texturaCartaAtual.id > 0) UnloadTexture(fluxo.texturaCartaAtual);
                fluxo.texturaCartaAtual.id = 0;
                fluxo.codigoFotoCarregada[0] = '\0';
                if (fluxo.gifCarregado) {
                    UnloadTexture(fluxo.gifTextura);
                    UnloadImage(fluxo.gifImage);
                    fluxo.gifCarregado = false;
                }
                fluxo.fase = 0;
                fluxo.quantidadeDesejada = 1; 
                *estadoAtual = MENU_PRINCIPAL;
            }
        }
    }
}