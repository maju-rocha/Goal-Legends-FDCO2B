#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include "../headers/biblioteca.h"

#define MAX_RASTRO 12

// Declarações prévias
void DesenharGoleiroPro(Vector2 pos, Color corUniforme);

// Estados internos do Minigame
typedef enum { P_MENU, P_JOGANDO, P_GOL, P_DEFENDEU, P_FIM_DE_JOGO } EstadoPenalti;

void jogarPenalti(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, int *qtd_pacotes) {
    const int larguraTela = 1000;
    const int alturaTela = 800;
    
    if (!IsWindowReady()) {
        InitWindow(larguraTela, alturaTela, "Minijogo: Penalty Strike");
    }

    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    // --- INICIALIZAÇÃO DO ÁUDIO ---
    if (!IsAudioDeviceReady()) {
        InitAudioDevice();
    }

    // Corrigido para "musicas/" com 's' no final
    Music musicaFundo = LoadMusicStream("audio/musica_fundo.mp3"); 
    Sound somChute = LoadSound("audio/chute.mp3");
    Sound somGol = LoadSound("audio/gol.mp3");
    
    // Inicia a música de fundo
    PlayMusicStream(musicaFundo);
    SetMusicVolume(musicaFundo, 0.1f);

    Font fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf");

    // Cursor e Texturas
    Image imagemBola = LoadImage("extras/bola_cursor.png");
    ImageResize(&imagemBola, 40, 40);
    Texture2D cursorBola = LoadTextureFromImage(imagemBola);
    UnloadImage(imagemBola);

    Texture2D imagemBola2026 = LoadTexture("imagens/imagem_bola2026.png");

    // Variáveis de jogo
    Vector2 posicaoGoleiro = { 500, 290 };
    Vector2 alvoGoleiro = { 500, 290 };
    Vector2 posicaoBola = { 500, 660 };
    Vector2 velocidadeBola = { 0, 0 };
    bool bolaChutada = false;
    float raioBola = 16.0f;
    float rotacaoBola = 0.0f;
    float escalaBola = 1.0f; 
    
    Vector2 rastroBola[MAX_RASTRO];
    int contadorRastro = 0;

    float anguloMira = 0.0f;
    float velocidadeMira = 0.04f;

    int gols = 0;
    int tentativas = 0;
    const int maxTentativas = 5;
    int pontuacaoCombo = 0;
    int pacotesGanhosRodada = 0;
    int totalFigurinhas = 0;
    bool pacotesComputados = false;

    EstadoPenalti estadoAtual = P_MENU;
    int timerFrames = 0;
    bool sairDoJogo = false; // Volta para o menu sem fechar o projeto
    bool fecharProjeto = false; // Fecha o projeto inteiro ao clicar no X

    Color verdeCanarinho = (Color){ 98, 209, 75, 255 }; 
    Color amareloBrasil = (Color){ 255, 215, 0, 255 };
    Color azulBrasil = (Color){ 0, 39, 118, 255 };
    Color sombraUI = Fade(BLACK, 0.4f);

    Rectangle btnTentar = { 170, 640, 310, 58 };
    Rectangle btnMenu = { 520, 640, 310, 58 };

    while (!sairDoJogo) {

        // Se clicar no X da janela, fecha o projeto inteiro
        if(WindowShouldClose()){
            fecharProjeto = true;
            break;
        }

        // Se apertar ESC, volta para o menu principal
        if(IsKeyPressed(KEY_ESCAPE)){
            sairDoJogo = true;
            break;
        }

        float tempoGlobal = (float)GetTime();
        Vector2 mousePoint = GetMousePosition();

        // Atualiza o streaming da música de fundo obrigatoriamente a cada frame
        UpdateMusicStream(musicaFundo);

        switch (estadoAtual) {
            case P_MENU:
                if (IsKeyPressed(KEY_SPACE)) {
                    gols = 0;
                    tentativas = 0;
                    pontuacaoCombo = 0;
                    pacotesComputados = false;
                    estadoAtual = P_JOGANDO;
                }
                break;

            case P_JOGANDO:
                if (!bolaChutada) {
                    escalaBola = 1.0f;
                    anguloMira += velocidadeMira;
                    if (anguloMira > 0.75f || anguloMira < -0.75f) velocidadeMira *= -1;

                    alvoGoleiro.x = 500 + sinf(tempoGlobal * 4.0f) * 200.0f;
                    posicaoGoleiro.x += (alvoGoleiro.x - posicaoGoleiro.x) * 0.1f;

                    if (IsKeyPressed(KEY_SPACE)) {
                        velocidadeBola.x = sinf(anguloMira) * -16.0f;
                        velocidadeBola.y = -19.0f;
                        bolaChutada = true;
                        contadorRastro = 0;

                        // TOCA O SOM DO CHUTE
                        PlaySound(somChute);
                        SetSoundVolume(somChute, 0.8f);
                    }
                } else {
                    if (IsKeyDown(KEY_LEFT)){
                        velocidadeBola.x -= 0.45f;
                        rotacaoBola -= 15.0f; 
                    } else if (IsKeyDown(KEY_RIGHT)) {
                        velocidadeBola.x += 0.45f;
                        rotacaoBola += 15.0f;
                    } else {
                        rotacaoBola += velocidadeBola.x * 2.0f;
                    }

                    for (int i = MAX_RASTRO - 1; i > 0; i--) rastroBola[i] = rastroBola[i - 1];
                    rastroBola[0] = posicaoBola;
                    if (contadorRastro < MAX_RASTRO) contadorRastro++;

                    posicaoBola.x += velocidadeBola.x;
                    posicaoBola.y += velocidadeBola.y;

                    escalaBola = 1.0f - ((660.0f - posicaoBola.y) / 440.0f) * 0.42f;

                    alvoGoleiro.x = posicaoBola.x + (velocidadeBola.x * 1.1f);
                    if (alvoGoleiro.x < 260) alvoGoleiro.x = 260;
                    if (alvoGoleiro.x > 740) alvoGoleiro.x = 740;
                    
                    posicaoGoleiro.x += (alvoGoleiro.x - posicaoGoleiro.x) * 0.09f;

                    if (CheckCollisionCircles(posicaoBola, raioBola * escalaBola, posicaoGoleiro, 32.0f)) {
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
                            
                            // TOCA O SOM DO GOL
                            PlaySound(somGol);
                            SetSoundVolume(somGol, 0.06f);
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
                if (timerFrames > 90) {
                    posicaoBola = (Vector2){ 500, 660 };
                    velocidadeBola = (Vector2){ 0, 0 };
                    posicaoGoleiro = (Vector2){ 500, 290 };
                    bolaChutada = false;
                    anguloMira = 0.0f;
                    rotacaoBola = 0.0f;

                    if (tentativas >= maxTentativas) {
                        if (pontuacaoCombo <= 250) pacotesGanhosRodada = 1;
                        else if (pontuacaoCombo <= 500) pacotesGanhosRodada = 2;
                        else if (pontuacaoCombo <= 750) pacotesGanhosRodada = 3;
                        else pacotesGanhosRodada = 4;
                        
                        if (!pacotesComputados) {
                            if (qtd_pacotes != NULL) {
                                *qtd_pacotes += pacotesGanhosRodada;
                                pacotes_fechados++;
                                salvarPacotes();
                            }
                            pacotesComputados = true;
                        }

                        totalFigurinhas = pacotesGanhosRodada * 7;
                        estadoAtual = P_FIM_DE_JOGO;
                    } else {
                        estadoAtual = P_JOGANDO;
                    }
                }
                break;

            case P_FIM_DE_JOGO:
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                    // TENTAR NOVAMENTE
                    if (CheckCollisionPointRec(mousePoint, btnTentar)) {
                        gols = 0;
                        tentativas = 0;
                        pontuacaoCombo = 0;
                        pacotesGanhosRodada = 0;
                        totalFigurinhas = 0;
                        pacotesComputados = false;
                        posicaoBola = (Vector2){ 500, 660 };
                        velocidadeBola = (Vector2){ 0, 0 };
                        posicaoGoleiro = (Vector2){ 500, 290 };
                        bolaChutada = false;
                        estadoAtual = P_JOGANDO;
                    } 
                    // VOLTAR AO MENU INICIAL
                    else if (CheckCollisionPointRec(mousePoint, btnMenu)) {
                        sairDoJogo = true; 
                    }
                }
                break;
        }

        // Se o jogador clicou para voltar ao menu, interrompe a renderização para sair limpo
        if (sairDoJogo) break;

        // --- RENDERIZAÇÃO ---
        BeginDrawing();
        ClearBackground(verdeCanarinho);

        // Gramado
        for (int y = 200; y < alturaTela; y += 60) {
            Color corGramado = ( (y / 60) % 2 == 0 ) ? verdeCanarinho : (Color){ 85, 185, 65, 255 };
            DrawRectangle(0, y, larguraTela, 60, corGramado);
        }

        // Arquibancada
        DrawRectangle(0, 0, larguraTela, 180, (Color){ 45, 45, 50, 255 }); 
        for (int y = 20; y < 180; y += 30) {
            DrawRectangle(0, y, larguraTela, 4, (Color){ 75, 75, 80, 255 });      
            DrawRectangle(0, y + 4, larguraTela, 26, (Color){ 55, 55, 60, 255 });  
        }

        // Torcida
        srand(1337); 
        Color coresCamisa[] = { RED, azulBrasil, amareloBrasil, WHITE, ORANGE, GREEN };
        Color coresPele[] = { (Color){241,194,125,255}, (Color){141,85,36,255}, (Color){255,219,172,255} };

        for (int y = 20; y < 180; y += 30) {
            for (int x = 8; x < larguraTela - 12; x += 24) {
                Color camisa = coresCamisa[rand() % 6];
                Color pele = coresPele[rand() % 3];
                int puloAnima = (int)(sinf(tempoGlobal * 4.0f + (x * 0.08f)) * 4.0f);
                if (rand() % 4 != 0) puloAnima = 0; 

                DrawRectangle(x, y + 10 + puloAnima, 18, 16, camisa);
                DrawRectangleLines(x, y + 10 + puloAnima, 18, 16, BLACK);
                DrawRectangle(x + 2, y + 2 + puloAnima, 14, 10, pele);
                DrawRectangleLines(x + 2, y + 2 + puloAnima, 14, 10, BLACK);
                DrawRectangle(x + 2, y + 2 + puloAnima, 14, 3, (rand() % 2 == 0) ? BLACK : (Color){100, 50, 20, 255});
            }
        }
        srand((unsigned int)tempoGlobal); 
        DrawRectangle(0, 180, larguraTela, 20, BLACK); 

        // Rede
        int divisorRede = 20;
        for (int x = 250; x <= 750; x += divisorRede) DrawRectangle(x, 200, 2, 130, (Color){ 255, 255, 255, 120 });
        for (int y = 200; y <= 330; y += 15) DrawRectangle(250, y, 500, 2, (Color){ 255, 255, 255, 120 });

        // Linhas de campo
        DrawRectangleLines(150, 200, 700, 420, WHITE);
        DrawLine(0, 330, larguraTela, 330, WHITE);
        DrawCircle(500, 560, 6, WHITE);

        // Traves
        DrawRectangle(242, 196, 12, 134, WHITE); DrawRectangle(242, 196, 12, 134, BLACK); DrawRectangle(244, 196, 8, 134, WHITE);
        DrawRectangle(746, 196, 12, 134, WHITE); DrawRectangle(746, 196, 12, 134, BLACK); DrawRectangle(748, 196, 8, 134, WHITE);
        DrawRectangle(242, 196, 516, 12, WHITE); DrawRectangle(242, 196, 516, 12, BLACK); DrawRectangle(244, 198, 512, 8, WHITE);

        switch (estadoAtual) {
            case P_MENU:
                DrawRectangle(0, 0, larguraTela, alturaTela, (Color){ 0, 0, 0, 200 });
                DrawRectangle(194, 194, 612, 412, BLACK);
                DrawRectangle(200, 200, 600, 400, (Color){ 25, 25, 25, 255 });
                DrawTextEx(fonteCopa, "MODO PENALTI", (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, "MODO PENALTI", 24, 2).x/2, 250 }, 24, 2, amareloBrasil);
                DrawTextEx(fonteCopa, "CONTROLES:", (Vector2){ 240, 340 }, 14, 2, WHITE);
                DrawTextEx(fonteCopa, "- ESPACO: CHUTAR BOLA", (Vector2){ 240, 380 }, 12, 2, LIGHTGRAY);
                DrawTextEx(fonteCopa, "- SETAS: ADICIONAR EFEITO", (Vector2){ 240, 410 }, 12, 2, LIGHTGRAY);
                DrawTextEx(fonteCopa, "PRESSIONE ESPACO PARA JOGAR", (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, "PRESSIONE ESPACO PARA JOGAR", 12, 2).x/2, 510 }, 12, 2, amareloBrasil);
                break;

            case P_JOGANDO:
                DrawRectangle(34, 24, 262, 82, BLACK); DrawRectangle(40, 30, 250, 70, (Color){ 25, 25, 25, 255 });
                DrawTextEx(fonteCopa, TextFormat("CHUTES: %d/%d", tentativas, maxTentativas), (Vector2){ 55, 45 }, 12, 2, WHITE);
                DrawTextEx(fonteCopa, TextFormat("GOLS:   %d", gols), (Vector2){ 55, 70 }, 12, 2, amareloBrasil);

                DrawRectangle(704, 24, 262, 82, BLACK); DrawRectangle(710, 30, 250, 70, (Color){ 25, 25, 25, 255 });
                DrawTextEx(fonteCopa, "PONTOS", (Vector2){ 725, 45 }, 10, 2, LIGHTGRAY);
                DrawTextEx(fonteCopa, TextFormat("%04d", pontuacaoCombo), (Vector2){ 725, 68 }, 18, 2, WHITE);

                if (!bolaChutada){
                    float miraX = posicaoBola.x + sinf(anguloMira) * -220.0f;
                    float miraY = posicaoBola.y - cosf(anguloMira) * 220.0f;
                    DrawLineEx(posicaoBola, (Vector2){ miraX, miraY }, 2.0f, amareloBrasil);
                } else {
                    for (int i = 0; i < contadorRastro; i++) {
                        DrawCircleV(rastroBola[i], (raioBola * escalaBola) * 0.4f, (Color){ 255, 255, 255, 80 });
                    }
                }

                DesenharGoleiroPro(posicaoGoleiro, amareloBrasil);
                Rectangle fonteBola = { 0.0f, 0.0f, (float)imagemBola2026.width, (float)imagemBola2026.height };
                Rectangle destinoBola = { posicaoBola.x, posicaoBola.y, 32.0f * escalaBola, 32.0f * escalaBola };
                Vector2 centroOrigem = { (32.0f * escalaBola) / 2.0f, (32.0f * escalaBola) / 2.0f };
                DrawTexturePro(imagemBola2026, fonteBola, destinoBola, centroOrigem, 0, WHITE);
                break;

            case P_GOL:
                DrawTextEx(fonteCopa, "GOOOOOOOOOOOOOL!", (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, "GOOOOOOOOOOOOOL!", 40, 2).x/2, 330 }, 40, 2, amareloBrasil);
                break;

            case P_DEFENDEU:
                if (posicaoBola.x < 252 || posicaoBola.x > 748 || posicaoBola.y < 220) {
                    DrawTextEx(fonteCopa, "PARA FORA!", (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, "PARA FORA!", 30, 2).x/2, 330 }, 30, 2, WHITE);
                } else {
                    DrawTextEx(fonteCopa, "QUE DEFESA INCRIVEL!", (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, "QUE DEFESA INCRIVEL!", 30, 2).x/2, 330 }, 30, 2, RED);
                }
                break;

            case P_FIM_DE_JOGO:
                DrawRectangle(0, 0, larguraTela, alturaTela, (Color){ 0, 0, 0, 220 });
                DrawTextEx(fonteCopa, "FIM DE JOGO", (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, "FIM DE JOGO", 32, 2).x/2, 100 }, 32, 2, RED);
                DrawTextEx(fonteCopa, TextFormat("GOLS:  %d/%d", gols, maxTentativas), (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, TextFormat("GOLS:  %d/%d", gols, maxTentativas), 14, 2).x/2, 160 }, 14, 2, WHITE);
                DrawTextEx(fonteCopa, TextFormat("SCORE: %d", pontuacaoCombo), (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, TextFormat("SCORE: %d", pontuacaoCombo), 14, 2).x/2, 190 }, 14, 2, WHITE);
                
                DrawRectangle(244, 244, 512, 282, BLACK);
                DrawRectangle(250, 250, 500, 272, (Color){ 25, 25, 25, 255 });
                DrawTextEx(fonteCopa, "RECOMPENSA:", (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, "RECOMPENSA:", 14, 2).x/2, 280 }, 14, 2, amareloBrasil);
                DrawTextEx(fonteCopa, TextFormat("%d PACOTES RECEBIDOS", pacotesGanhosRodada), (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, TextFormat("%d PACOTES RECEBIDOS", pacotesGanhosRodada), 12, 2).x/2, 320 }, 12, 2, WHITE);
                DrawTextEx(fonteCopa, TextFormat("(+%d FIGURINHAS)", totalFigurinhas), (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, TextFormat("(+%d FIGURINHAS)", totalFigurinhas), 10, 2).x/2, 350 }, 10, 2, LIGHTGRAY);
                
                int saldoTotal = (qtd_pacotes != NULL) ? *qtd_pacotes : 0;
                DrawTextEx(fonteCopa, TextFormat("SALDO TOTAL: %d PACOTES", saldoTotal), (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, TextFormat("SALDO TOTAL: %d PACOTES", saldoTotal), 11, 2).x/2, 390 }, 11, 2, GREEN);

                for(int f = 0; f < pacotesGanhosRodada; f++) {
                    int posXCard = 435 + (f * 40) - ((pacotesGanhosRodada - 1) * 20);
                    DrawRectangle(posXCard, 440, 30, 40, amareloBrasil);
                    DrawRectangleLines(posXCard, 440, 30, 40, WHITE);
                }

                // Botão: JOGAR DE NOVO
                bool hoverTentar = CheckCollisionPointRec(mousePoint, btnTentar);
                int animTentar = hoverTentar ? ((IsMouseButtonDown(MOUSE_LEFT_BUTTON)) ? 4 : -4) : 0;
                if(hoverTentar && !IsMouseButtonDown(MOUSE_LEFT_BUTTON)) DrawRectangleRounded((Rectangle){ btnTentar.x + 6, btnTentar.y + 6, btnTentar.width, btnTentar.height }, 0.15f, 4, sombraUI);
                Rectangle rTentar = { btnTentar.x, btnTentar.y + animTentar, btnTentar.width, btnTentar.height };
                DrawRectangleRounded(rTentar, 0.15f, 4, hoverTentar ? WHITE : amareloBrasil);
                DrawRectangleRoundedLinesEx(rTentar, 0.15f, 4, 2.0f, azulBrasil);
                DrawTextEx(fonteCopa, "JOGAR DE NOVO", (Vector2){ rTentar.x + (rTentar.width/2) - (MeasureTextEx(fonteCopa, "JOGAR DE NOVO", 12, 1).x/2), rTentar.y + 22 }, 12, 1, azulBrasil);

                // Botão: MENU PRINCIPAL
                bool hoverMenu = CheckCollisionPointRec(mousePoint, btnMenu);
                int animMenu = hoverMenu ? ((IsMouseButtonDown(MOUSE_LEFT_BUTTON)) ? 4 : -4) : 0;
                if(hoverMenu && !IsMouseButtonDown(MOUSE_LEFT_BUTTON)) DrawRectangleRounded((Rectangle){ btnMenu.x + 6, btnMenu.y + 6, btnMenu.width, btnMenu.height }, 0.15f, 4, sombraUI);
                Rectangle rMenu = { btnMenu.x, btnMenu.y + animMenu, btnMenu.width, btnMenu.height };
                DrawRectangleRounded(rMenu, 0.15f, 4, hoverMenu ? WHITE : amareloBrasil);
                DrawRectangleRoundedLinesEx(rMenu, 0.15f, 4, 2.0f, azulBrasil);
                DrawTextEx(fonteCopa, "MENU PRINCIPAL", (Vector2){ rMenu.x + (rMenu.width/2) - (MeasureTextEx(fonteCopa, "MENU PRINCIPAL", 12, 1).x/2), rMenu.y + 22 }, 12, 1, azulBrasil);
                break;
        }

        DrawTexture(cursorBola, (int)mousePoint.x - cursorBola.width/2, (int)mousePoint.y - cursorBola.height/2, WHITE);
        EndDrawing();
    }

    // DESCARREGA OS ÁUDIOS E RECURSOS DA MEMÓRIA ANTES DE RETORNAR
    StopMusicStream(musicaFundo);
    UnloadMusicStream(musicaFundo);
    UnloadSound(somChute);
    UnloadSound(somGol);

    UnloadTexture(cursorBola);
    UnloadTexture(imagemBola2026); 
    UnloadFont(fonteCopa);
    
    SetMouseCursor(MOUSE_CURSOR_DEFAULT);
    
    // Fecha apenas a janela do minigame antes de voltar para o menu
    if(IsWindowReady()){
        CloseWindow();
    }

    // Se a saída foi pelo X, encerra o projeto por completo
    if(fecharProjeto){
        exit(0);
    }
}

void DesenharGoleiroPro(Vector2 pos, Color corUniforme) {
    DrawRectangle(pos.x - 25, pos.y, 50, 35, corUniforme); DrawRectangleLines(pos.x - 25, pos.y, 50, 35, BLACK);
    DrawCircle((int)pos.x, (int)pos.y - 12, 12, (Color){ 241, 194, 125, 255 }); DrawCircleLines((int)pos.x, (int)pos.y - 12, 12, BLACK);
}