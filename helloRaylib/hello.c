#include "include/raylib.h"
#include <math.h>

// Estados do Jogo
typedef enum { MENU, JOGANDO, GOL, DEFENDEU, FIM_DE_JOGO } EstadoJogo;

int main(void) {
    // Configuração da tela
    const int larguraTela = 800;
    const int alturaTela = 600;
    InitWindow(larguraTela, alturaTela, "Copa do Mundo - Decisao por Penaltis");

    // Elementos do Jogo
    Vector2 posicaoGoleiro = { 400, 220 };
    float velocidadeGoleiro = 4.0f;
    int direcaoGoleiro = 1; // 1 = Direita, -1 = Esquerda

    Vector2 posicaoBola = { 400, 500 };
    Vector2 velocidadeBola = { 0, 0 };
    bool bolaChutada = false;

    // Mira do Chute
    float anguloMira = 0.0f;
    float velocidadeMira = 0.05f;

    // Placar
    int gols = 0;
    int tentativas = 0;
    const int maxTentativas = 5;

    EstadoJogo estadoAtual = MENU;
    int timerFrames = 0; // Para controlar o tempo das telas de Gol/Defesa

    SetTargetFPS(60);

    // Loop Principal
    while (!WindowShouldClose()) {
        // --- ATUALIZAÇÃO DA LÓGICA ---
        switch (estadoAtual) {
            case MENU:
                if (IsKeyPressed(KEY_SPACE)) {
                    gols = 0;
                    tentativas = 0;
                    estadoAtual = JOGANDO;
                }
                break;

            case JOGANDO:
                // 1. Movimento do Goleiro (Inteligência Artificial simples de vaivém)
                posicaoGoleiro.x += velocidadeGoleiro * direcaoGoleiro;
                if (posicaoGoleiro.x > 530 || posicaoGoleiro.x < 270) {
                    direcaoGoleiro *= -1; // Inverte a direção nas traves
                }

                // 2. Movimento da Mira (Se a bola ainda não foi chutada)
                if (!bolaChutada) {
                    anguloMira += velocidadeMira;
                    if (anguloMira > 0.8f || anguloMira < -0.8f) {
                        velocidadeMira *= -1; // Oscila a mira da esquerda para a direita
                    }

                    // Chutar a bola com ESPAÇO
                    if (IsKeyPressed(KEY_SPACE)) {
                        velocidadeBola.x = sinf(anguloMira) * -12.0f;
                        velocidadeBola.y = -15.0f;
                        bolaChutada = true;
                    }
                } else {
                    // Mover a bola após o chute
                    posicaoBola.x += velocidadeBola.x;
                    posicaoBola.y += velocidadeBola.y;

                    // 3. Verificação de Colisão com o Goleiro
                    if (CheckCollisionCircles(posicaoBola, 15, posicaoGoleiro, 25)) {
                        estadoAtual = DEFENDEU;
                        tentativas++;
                        timerFrames = 0;
                    }
                    // 4. Verificação se a bola entrou no Gol (Linha de fundo / Travessão)
                    else if (posicaoBola.y <= 210) {
                        // Limites da trave: X entre 250 e 550
                        if (posicaoBola.x >= 250 && posicaoBola.x <= 550) {
                            estadoAtual = GOL;
                            gols++;
                        } else {
                            estadoAtual = DEFENDEU; // Chutou para fora
                        }
                        tentativas++;
                        timerFrames = 0;
                    }
                }
                break;

            case GOL:
            case DEFENDEU:
                timerFrames++;
                if (timerFrames > 90) { // Espera ~1.5 segundos
                    // Reiniciar posicionamento da bola
                    posicaoBola = (Vector2){ 400, 500 };
                    velocidadeBola = (Vector2){ 0, 0 };
                    bolaChutada = false;
                    anguloMira = 0.0f;

                    if (tentativas >= maxTentativas) {
                        estadoAtual = FIM_DE_JOGO;
                    } else {
                        estadoAtual = JOGANDO;
                    }
                }
                break;

            case FIM_DE_JOGO:
                if (IsKeyPressed(KEY_SPACE)) {
                    estadoAtual = MENU;
                }
                break;
        }

        // --- DESENHO NA TELA ---
        BeginDrawing();
        ClearBackground(GREEN); // Gramado do estádio

        // Desenhar as linhas da grande área e a trave de futebol
        DrawRectangle(200, 150, 400, 50, DARKGREEN); // Sombra interna do gol
        DrawRectangleLines(250, 150, 300, 100, WHITE); // Linha da Trave
        DrawLine(100, 250, 700, 250, WHITE); // Linha de fundo

        switch (estadoAtual) {
            case MENU:
                DrawText("COPA DO MUNDO: DISPUTA DE PENALTIS", 120, 200, 28, GOLD);
                DrawText("Pressione ESPACO para iniciar o jogo", 200, 350, 20, WHITE);
                break;

            case JOGANDO:
                // Desenhar Mira (Se a bola não foi chutada)
                if (!bolaChutada) {
                    float miraX = posicaoBola.x + sinf(anguloMira) * -100.0f;
                    float miraY = posicaoBola.y - cosf(anguloMira) * 100.0f;
                    DrawLineEx(posicaoBola, (Vector2){ miraX, miraY }, 3.0f, RED);
                }

                // Desenhar Goleiro (Círculo azul representando a camisa de goleiro)
                DrawCircleV(posicaoGoleiro, 25, DARKBLUE);
                DrawText("GOLEIRO", posicaoGoleiro.x - 28, posicaoGoleiro.y - 40, 12, YELLOW);

                // Desenhar Bola
                DrawCircleV(posicaoBola, 15, WHITE);
                DrawCircleLines(posicaoBola.x, posicaoBola.y, 15, BLACK); // Detalhe da bola

                // HUD do Placar
                DrawText(TextFormat("Chutes: %d/%d", tentativas, maxTentativas), 20, 20, 20, WHITE);
                DrawText(TextFormat("Gols: %d", gols), 20, 50, 20, YELLOW);
                DrawText("Comando: ESPACO para chutar", 500, 20, 18, RAYWHITE);
                break;

            case GOL:
                DrawText("GOL DO BRASIL!!", 250, 300, 40, YELLOW);
                break;

            case DEFENDEU:
                if (posicaoBola.x < 250 || posicaoBola.x > 550) {
                    DrawText("PRA FORA!!", 320, 300, 40, RED);
                } else {
                    DrawText("DEFENDEU O GOLEIRO!", 200, 300, 40, ORANGE);
                }
                break;

            case FIM_DE_JOGO:
                DrawText("FIM DA DISPUTA!", 260, 180, 35, WHITE);
                if (gols >= 4) {
                    DrawText("PARABENS! VOCE E CAMPEAO DA COPA!", 150, 280, 24, GOLD);
                } else {
                    DrawText("Nao foi dessa vez... Eliminado nas quartas!", 140, 280, 24, RED);
                }
                DrawText(TextFormat("Total de Gols: %d", gols), 330, 350, 22, YELLOW);
                DrawText("Pressione ESPACO para voltar ao Menu", 210, 450, 18, RAYWHITE);
                break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}