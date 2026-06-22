#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include "biblioteca.h"

#define MAX_RASTRO 12

// Declarações prévias das funções visuais do Pênalti
void DrawLineBlurMode(Vector2 startPos, Vector2 endPos, float thick, Color color);
void DesenharGoleiroPro(Vector2 pos, Color corUniforme);
void DesenharBolaPro(Vector2 pos, float raio, float rotacao);

// Estados internos do Minigame
typedef enum { P_MENU, P_JOGANDO, P_GOL, P_DEFENDEU, P_FIM_DE_JOGO } EstadoPenalti;

void jogarPenalti() {
    const int larguraTela = 1000;
    const int alturaTela = 800;
    
    if (!IsWindowReady()) {
        SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT); 
        InitWindow(larguraTela, alturaTela, "Minijogo: Penalty Ultra Striker");
    }

    // 🎵 ÁUDIO: Inicializa o dispositivo de som do jogo
    InitAudioDevice();

    // 🎵 ÁUDIO: Carrega os efeitos sonoros de chute e gol
    Sound somChute = LoadSound("extras/chute.wav");
    Sound somGol = LoadSound("extras/gol.wav");

    // 📻 MÚSICA: Carrega a música de fundo (coloquei o nome como 'musica.mp3')
    // Você pode usar .mp3 ou .ogg para a música de fundo!
    Music musicaFundo = LoadMusicStream("extras/musica.mp3");
    
    // Começa a tocar a música de fundo e ativa o modo de repetição (loop)
    PlayMusicStream(musicaFundo);

    HideCursor(); 
    Image imagemBola = LoadImage("extras/bola_cursor.png"); 
    ImageResize(&imagemBola, 40, 40); 
    Texture2D cursorBola = LoadTextureFromImage(imagemBola); 
    UnloadImage(imagemBola); 
    Vector2 posicaoGoleiro = { 500, 290 };
    Vector2 alvoGoleiro = { 500, 290 };

    Vector2 posicaoBola = { 500, 660 };
    Vector2 velocidadeBola = { 0, 0 };
    bool bolaChutada = false;
    float raioBola = 16.0f; 
    float rotacaoBola = 0.0f;
    
    Vector2 rastroBola[MAX_RASTRO];
    int contadorRastro = 0;

    float anguloMira = 0.0f;
    float velocidadeMira = 0.04f;
    float intensidadeEfeito = 0.0f;

    // Placar e Regras
    int gols = 0;
    int tentativas = 0;
    const int maxTentativas = 5;
    int pontuacaoCombo = 0;

    // Variáveis para a recompensa final
    int pacotesGanhos = 0;
    int totalFigurinhas = 0;
    float porcentagemAproveitamento = 0.0f;

    EstadoPenalti estadoAtual = P_MENU;
    int timerFrames = 0;
    float escalaTextoModo = 1.0f;

    Color verdeCanarinho = (Color){ 34, 139, 34, 255 };
    Color amareloBrasil = (Color){ 255, 215, 0, 255 };
    Color azulBrasil = (Color){ 0, 39, 118, 255 };
    Color darkGlass = (Color){ 15, 20, 35, 230 }; 

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        // 📻 MÚSICA: Alimenta o streaming da música para ela continuar tocando fluentemente
        UpdateMusicStream(musicaFundo);

        float tempoGlobal = (float)GetTime();
        Vector2 mousePoint = GetMousePosition();

        switch (estadoAtual) {
            case P_MENU:
                escalaTextoModo = 1.0f + sinf(tempoGlobal * 5.0f) * 0.05f;
                if (IsKeyPressed(KEY_SPACE)) {
                    gols = 0;
                    tentativas = 0;
                    pontuacaoCombo = 0;
                    estadoAtual = P_JOGANDO;
                }
                break;

            case P_JOGANDO:
                if (!bolaChutada) {
                    anguloMira += velocidadeMira;
                    if (anguloMira > 0.75f || anguloMira < -0.75f) velocidadMira *= -1;

                    alvoGoleiro.x = 500 + sinf(tempoGlobal * 4.0f) * 200.0f; 
                    posicaoGoleiro.x += (alvoGoleiro.x - posicaoGoleiro.x) * 0.1f;

                    if (IsKeyPressed(KEY_SPACE)) {
                        velocidadeBola.x = sinf(anguloMira) * -16.0f;
                        velocidadeBola.y = -19.0f; 
                        bolaChutada = true;
                        contadorRastro = 0;
                        intensidadeEfeito = 0.0f;

                        // 🎵 ÁUDIO: Toca o som do chute
                        PlaySound(somChute);
                    }
                } else {
                    if (IsKeyDown(KEY_LEFT)){
                        velocidadeBola.x -= 0.45f;
                        intensidadeEfeito = -1.0f;
                        rotacaoBola -= 0.3f;
                    } else if (IsKeyDown(KEY_RIGHT)) {
                        velocidadeBola.x += 0.45f;
                        intensidadeEfeito = 1.0f;
                        rotacaoBola += 0.3f;
                    } else {
                        intensidadeEfeito *= 0.9f; 
                        rotacaoBola += velocidadeBola.x * 0.02f;
                    }

                    for (int i = MAX_RASTRO - 1; i > 0; i--) rastroBola[i] = rastroBola[i - 1];
                    rastroBola[0] = posicaoBola;
                    if (contadorRastro < MAX_RASTRO) contadorRastro++;

                    posicaoBola.x += velocidadeBola.x;
                    posicaoBola.y += velocidadeBola.y;

                    alvoGoleiro.x = posicaoBola.x + (velocidadeBola.x * 1.1f);
                    if (alvoGoleiro.x < 260) alvoGoleiro.x = 260; 
                    if (alvoGoleiro.x > 740) alvoGoleiro.x = 740;
                    
                    posicaoGoleiro.x += (alvoGoleiro.x - posicaoGoleiro.x) * 0.09f;

                    if (CheckCollisionCircles(posicaoBola, raioBola, posicaoGoleiro, 32.0f)) {
                        estadoAtual = P_DEFENDEU;
                        tentativas++;
                        
                        pontuacaoCombo -= 80;
                        if (pontuacaoCombo < 0) pontuacaoCombo = 0;
                        
                        timerFrames = 0;
                    }
                    else if (posicaoBola.y <= 220) {
                        if (posicaoBola.x >= 252 && posicaoBola.x <= 748) {
                            estadoAtual = P_GOL;
                            gols++;
                            pontuacaoCombo += 100 + (int)(fabsf(velocidadeBola.x) * 10); 

                            // 🎵 ÁUDIO: Toca o som do Gol
                            PlaySound(somGol);
                        } else {
                            estadoAtual = P_DEFENDEU; 
                            
                            pontuacaoCombo -= 80;
                            if (pontuacaoCombo < 0) pontuacaoCombo = 0;
                        }
                        tentativas++;
                        timerFrames = 0;
                    }
                }
                break;

            case P_GOL:
            case P_DEFENDEU:
                timerFrames++;
                if (timerFrames > 110) { 
                    posicaoBola = (Vector2){ 500, 660 };
                    velocidadeBola = (Vector2){ 0, 0 };
                    posicaoGoleiro = (Vector2){ 500, 290 };
                    bolaChutada = false;
                    anguloMira = 0.0f;
                    intensidadeEfeito = 0.0f;

                    if (tentativas >= maxTentativas) {
                        porcentagemAproveitamento = ((float)pontuacaoCombo / 904.0f) * 100.0f;
                        if (porcentagemAproveitamento > 100.0f) porcentagemAproveitamento = 100.0f; 
                        
                        if (pontuacaoCombo <= 250) {
                            pacotesGanhos = 1;
                        } else if (pontuacaoCombo > 250 && pontuacaoCombo <= 500) {
                            pacotesGanhos = 2;
                        } else if (pontuacaoCombo > 500 && pontuacaoCombo <= 750) {
                            pacotesGanhos = 3;
                        } else { 
                            pacotesGanhos = 4;
                        }
                        
                        totalFigurinhas = pacotesGanhos * 7; 
                        
                        estadoAtual = P_FIM_DE_JOGO;
                    } else {
                        estadoAtual = P_JOGANDO;
                    }
                }
                break;

            case P_FIM_DE_JOGO:
                if (IsKeyPressed(KEY_SPACE)) estadoAtual = P_MENU;
                break;
        }

        // --- PIPELINE DE DESENHO GRÁFICO ---
        BeginDrawing();
        ClearBackground(verdeCanarinho); 

        for (int y = 200; y < alturaTela; y += 50) {
            Color corGramado = ( (y / 50) % 2 == 0 ) ? verdeCanarinho : (Color){ 28, 120, 28, 255 };
            DrawRectangle(0, y, larguraTela, 50, corGramado);
        }

        DrawRectangle(0, 0, larguraTela, 190, azulBrasil); 
        DrawRectangle(0, 180, larguraTela, 20, darkGlass); 

        for (int i = 0; i < larguraTela; i += 16) {
            DrawRectangle(i, 50, 12, 130, Fade(amareloBrasil, 0.15f)); 
            if (rand() % 45 == 0) { 
                DrawCircle(i + (rand() % 10), 50 + (rand() % 100), rand() % 4 + 2, WHITE);
            }
        }

        int espacamentoRede = 15;
        for (int x = 250; x <= 750; x += espacamentoRede) {
            DrawLineEx((Vector2){ (float)x, 200 }, (Vector2){ (float)(x + (x - 500) / 4), 150 }, 1.5f, Fade(WHITE, 0.25f)); 
            DrawLineEx((Vector2){ (float)x, 200 }, (Vector2){ (float)x, 330 }, 1.0f, Fade(WHITE, 0.15f));
        }
        for (int y = 150; y <= 200; y += 10) {
            DrawLineEx((Vector2){ 250.0f + (250-500)/4.0f, (float)y }, (Vector2){ 750.0f + (750-500)/4.0f, (float)y }, 1.0f, Fade(WHITE, 0.2f)); 
        }
        for (int y = 200; y <= 330; y += espacamentoRede) {
            DrawLineEx((Vector2){ 250, (float)y }, (Vector2){ 750, (float)y }, 1.0f, Fade(WHITE, 0.25f)); 
        }

        DrawRectangleLinesEx((Rectangle){ 150, 200, 700, 420 }, 4, Fade(WHITE, 0.8f)); 
        DrawCircleSector((Vector2){ 500, 200 }, 80, 0, 180, 0, Fade(WHITE, 0.15f)); 
        DrawLineEx((Vector2){ 0, 330 }, (Vector2){ larguraTela, 330 }, 4, Fade(WHITE, 0.8f)); 
        DrawCircle(500, 560, 8, WHITE); 

        DrawLineEx((Vector2){ 246, 330 }, (Vector2){ 246, 196 }, 10, WHITE); 
        DrawLineEx((Vector2){ 250, 330 }, (Vector2){ 250, 196 }, 8, WHITE);  
        DrawLineEx((Vector2){ 754, 330 }, (Vector2){ 754, 196 }, 10, WHITE); 
        DrawLineEx((Vector2){ 750, 330 }, (Vector2){ 750, 196 }, 8, WHITE);  
        DrawLineEx((Vector2){ 245, 200 }, (Vector2){ 755, 200 }, 8, WHITE);  

        switch (estadoAtual) {
            case P_MENU:
                DrawRectangle(0, 0, larguraTela, alturaTela, Fade(BLACK, 0.7f));
                DrawRectangleRounded((Rectangle){ 200, 180, 600, 440 }, 0.05f, 4, darkGlass);
                DrawRectangleRoundedLinesEx((Rectangle){ 200, 180, 600, 440 }, 0.05f, 4, 2.0f, amareloBrasil);
                DrawText("SUPER STRIKER", larguraTela/2 - MeasureText("SUPER STRIKER", 50)/2, 230, 50, amareloBrasil);
                DrawText("PRO PENALTY WORLD CUP", larguraTela/2 - MeasureText("PRO PENALTY WORLD CUP", 20)/2, 290, 20, WHITE);
                DrawText("Pressione ESPACO para Iniciar", (int)(larguraTela/2 - MeasureText("Pressione ESPACO para Iniciar", 24)/2), 400, 24, Fade(WHITE, escalaTextoModo));
                DrawRectangleRounded((Rectangle){ 240, 460, 520, 120 }, 0.1f, 4, azulBrasil);
                DrawText("CONTROLES AVANCADOS:", 260, 480, 16, amareloBrasil);
                DrawText("- [ESPACO]: Dispara o chute (Mira Oscilante)", 260, 510, 16, WHITE);
                DrawText("- [SETAS ESQ / DIR]: Coloca EFEITO na bola", 260, 540, 16, WHITE);
                break;

            case P_JOGANDO:
                {
                    Rectangle rectPlacar = { 40, 30, 250, 75 };
                    DrawRectangleRounded(rectPlacar, 0.2f, 4, darkGlass);
                    DrawRectangleRoundedLinesEx(rectPlacar, 0.2f, 4, 1.5f, amareloBrasil);
                    DrawText(TextFormat("TENTATIVAS: %d/%d", tentativas, maxTentativas), 55, 45, 16, WHITE);
                    DrawText(TextFormat("GOLS:      %d", gols), 55, 70, 18, amareloBrasil);

                    Rectangle rectCombo = { 710, 30, 250, 75 };
                    DrawRectangleRounded(rectCombo, 0.2f, 4, darkGlass);
                    DrawRectangleRoundedLinesEx(rectCombo, 0.2f, 4, 1.5f, WHITE);
                    DrawText("COMBO MULTIPLIER", 725, 45, 14, LIGHTGRAY);
                    DrawText(TextFormat("%05d PTS", pontuacaoCombo), 725, 65, 26, amareloBrasil);
                }

                if (bolaChutada && fabsf(intensidadeEfeito) > 0.1f) {
                    DrawText(intensidadeEfeito > 0 ? "EFEITO MAGNETICO >>>" : "<<< EFEITO MAGNETICO", 
                             (int)(posicaoBola.x - 110), (int)(posicaoBola.y + 25), 14, amareloBrasil);
                }

                if (!bolaChutada){
                    float miraX = posicaoBola.x + sinf(anguloMira) * -220.0f;
                    float miraY = posicaoBola.y - cosf(anguloMira) * 220.0f;
                    DrawLineBlurMode(posicaoBola, (Vector2){ miraX, miraY }, 4.0f, amareloBrasil);
                    DrawCircleGradient((int)miraX, (int)miraY, 10, amareloBrasil, Fade(amareloBrasil, 0.0f));
                } else {
                    for (int i = 0; i < contadorRastro; i++) {
                        float fatorFading = (float)(MAX_RASTRO - i) / MAX_RASTRO;
                        DrawCircleV(rastroBola[i], raioBola * fatorFading, Fade(amareloBrasil, fatorFading * 0.35f));
                    }
                }

                DesenharGoleiroPro(posicaoGoleiro, amareloBrasil);
                DesenharBolaPro(posicaoBola, raioBola, rotacaoBola);
                break;

            case P_GOL:
                DrawRectangle(0, 0, larguraTela, alturaTela, Fade(verdeCanarinho, 0.4f));
                DrawText("GOOOOOOL!!!", larguraTela/2 - MeasureText("GOOOOOOL!!!", 70)/2 + 3, 303, 70, BLACK);
                DrawText("GOOOOOOL!!!", larguraTela/2 - MeasureText("GOOOOOOL!!!", 70)/2, 300, 70, amareloBrasil);
                DrawText("CHUTE PERFEITO NO ANGULO!", larguraTela/2 - MeasureText("CHUTE PERFEITO NO ANGULO!", 24)/2, 390, 24, WHITE);
                break;

            case P_DEFENDEU:
                DrawRectangle(0, 0, larguraTela, alturaTela, Fade(BLACK, 0.5f));
                if (posicaoBola.x < 252 || posicaoBola.x > 748 || posicaoBola.y < 220) {
                    DrawText("PARA FORA!", larguraTela/2 - MeasureText("PARA FORA!", 60)/2, 320, 60, WHITE);
                } else {
                    DrawText("QUE DEFESA ESPETACULAR!", larguraTela/2 - MeasureText("QUE DEFESA ESPETACULAR!", 50)/2, 320, 50, amareloBrasil);
                }
                break;

            case P_FIM_DE_JOGO:
                DrawRectangle(0, 0, larguraTela, alturaTela, Fade(BLACK, 0.85f));
                
                DrawRectangleRounded((Rectangle){ 180, 120, 640, 520 }, 0.05f, 4, darkGlass);
                DrawRectangleRoundedLinesEx((Rectangle){ 180, 120, 640, 520 }, 0.05f, 4, 2.5f, amareloBrasil);

                DrawText("FIM DO TORNEIO", larguraTela/2 - MeasureText("FIM DO TORNEIO", 40)/2, 150, 40, WHITE);
                
                if (gols >= 4) {
                    DrawText("SELECAO CAMPEA DO MUNDO!", larguraTela/2 - MeasureText("SELECAO CAMPEA DO MUNDO!", 26)/2, 220, 26, amareloBrasil);
                } else {
                    DrawText("ELIMINADO NAS QUARTAS DE FINAL...", larguraTela/2 - MeasureText("ELIMINADO NAS QUARTAS DE FINAL...", 22)/2, 220, 22, LIGHTGRAY);
                }

                DrawText(TextFormat("SCORE ACUMULADO: %d PONTOS", pontuacaoCombo), larguraTela/2 - MeasureText(TextFormat("SCORE ACUMULADO: %d PONTOS", pontuacaoCombo), 20)/2, 280, 20, WHITE);
                DrawText(TextFormat("APROVEITAMENTO: %.1f%% / 100%%", porcentagemAproveitamento), larguraTela/2 - MeasureText(TextFormat("APROVEITAMENTO: %.1f%% / 100%%", porcentagemAproveitamento), 18)/2, 315, 18, azulBrasil);

                DrawRectangleRounded((Rectangle){ 230, 360, 540, 180 }, 0.08f, 4, azulBrasil);
                DrawRectangleRoundedLinesEx((Rectangle){ 230, 360, 540, 180 }, 0.08f, 4, 2.0f, amareloBrasil);
                
                DrawText("RECOMPENSA COPA DO MUNDO:", larguraTela/2 - MeasureText("RECOMPENSA COPA DO MUNDO:", 18)/2, 380, 18, amareloBrasil);
                
                const char* textoPacotes = TextFormat("VOCE GANHOU: %d PACOTES!", pacotesGanhos);
                DrawText(textoPacotes, larguraTela/2 - MeasureText(textoPacotes, 24)/2, 420, 24, WHITE);
                
                const char* textoFigurinhas = TextFormat("(+ %d Figurinhas para o seu Album)", totalFigurinhas);
                DrawText(textoFigurinhas, larguraTela/2 - MeasureText(textoFigurinhas, 16)/2, 460, 16, amareloBrasil);
                
                for(int f = 0; f < pacotesGanhos; f++) {
                    int posXCard = 420 + (f * 45) - ((pacotesGanhos - 1) * 22);
                    DrawRectangleRounded((Rectangle){ (float)posXCard, 495, 30, 38 }, 0.1f, 4, amareloBrasil);
                    DrawRectangleRoundedLinesEx((Rectangle){ (float)posXCard, 495, 30, 38 }, 0.1f, 4, 1.0f, WHITE);
                }

                DrawText("Pressione ESPACO para recomecar o desafio", larguraTela/2 - MeasureText("Pressione ESPACO para recomecar o desafio", 18)/2, 590, 18, LIGHTGRAY);
                break;
        }

        DrawTexture(cursorBola, (int)mousePoint.x - cursorBola.width/2, (int)mousePoint.y - cursorBola.height/2, WHITE);

        EndDrawing();
    }

    // 🎵 ÁUDIO: Descarrega os arquivos de som e música ao fechar
    UnloadSound(somChute);
    UnloadSound(somGol);
    UnloadMusicStream(musicaFundo); // Descarrega a música de fundo
    
    // 🎵 ÁUDIO: Fecha o sistema de som com segurança
    CloseAudioDevice();

    UnloadTexture(cursorBola);
    CloseWindow();
}

void DrawLineBlurMode(Vector2 startPos, Vector2 endPos, float thick, Color color) {
    DrawLineEx(startPos, endPos, thick + 4.0f, Fade(color, 0.2f));
    DrawLineEx(startPos, endPos, thick + 1.5f, Fade(color, 0.5f));
    DrawLineEx(startPos, endPos, thick, WHITE);
}

void DesenharGoleiroPro(Vector2 pos, Color corUniforme) {
    Rectangle corpo = { pos.x - 30, pos.y, 60, 38 }; 
    DrawRectangleRounded(corpo, 0.3f, 4, corUniforme);
    DrawRectangleRoundedLinesEx(corpo, 0.3f, 4, 2.0f, (Color){ 0, 39, 118, 255 });

    DrawCircle((int)pos.x, (int)pos.y - 12, 14, (Color){ 241, 194, 125, 255 }); 
    DrawCircleLines((int)pos.x, (int)pos.y - 12, 14, BLACK);

    DrawCircle((int)pos.x - 40, (int)pos.y + 15, 9, ORANGE);
    DrawCircleLines((int)pos.x - 40, (int)pos.y + 15, 9, BLACK);
    DrawCircle((int)pos.x + 40, (int)pos.y + 15, 9, ORANGE);
    DrawCircleLines((int)pos.x + 40, (int)pos.y + 15, 9, BLACK);
}

void DesenharBolaPro(Vector2 pos, float raio, float rotacao) {
    DrawCircleV(pos, raio, WHITE);
    DrawCircleLines((int)pos.x, (int)pos.y, raio, DARKGRAY);

    for (int i = 0; i < 4; i++) {
        float anguloGomo = rotacao + (i * (PI / 2.0f));
        Vector2 extremidade = {
            pos.x + sinf(anguloGomo) * raio,
            pos.y + cosf(anguloGomo) * raio
        };
        DrawLineEx(pos, extremidade, 1.5f, Fade(BLACK, 0.6f));
    }
    DrawCircle((int)pos.x, (int)pos.y, raio * 0.3f, (Color){ 30, 30, 30, 255 });
}