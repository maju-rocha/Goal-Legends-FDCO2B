#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include "biblioteca.h"

// Estrutura atualizada para o Raylib
typedef struct {
    char pergunta[200];
    char opcoes[4][100];
    int correta; // índice de 0 a 3
} PerguntaQuiz;

// Estados do jogo
typedef enum { TELA_PERGUNTA, TELA_FEEDBACK, TELA_FIM } EstadoQuiz;

void jogarQuiz(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album) {
    PerguntaQuiz banco[5] = {
        {"Quem ganhou a Copa do Mundo de 2002?", {"Brasil", "Alemanha", "Italia", "Argentina"}, 0},
        {"Qual pais sediou a Copa de 2014?", {"Africa do Sul", "Brasil", "Russia", "Alemanha"}, 1},
        {"Quem e o maior artilheiro das Copas do Mundo?", {"Pele", "Ronaldo", "Miroslav Klose", "Messi"}, 2},
        {"Qual selecao ganhou a primeira Copa em 1930?", {"Brasil", "Argentina", "Uruguai", "Italia"}, 2},
        {"Qual jogador fez o gol 'A Mao de Deus'?", {"Pele", "Diego Maradona", "Zinedine Zidane", "Johan Cruyff"}, 1}
    };

    int num_perguntas = 3;
    int sorteadas[3];
    int score = 0;
    int pergunta_atual = 0;
    int opcao_selecionada = -1;
    
    // Sorteia as perguntas antes de abrir a janela
    for(int i = 0; i < num_perguntas; i++) {
        int p_idx, repetida;
        do {
            repetida = 0;
            p_idx = rand() % 5;
            for(int j = 0; j < i; j++) {
                if(sorteadas[j] == p_idx) repetida = 1;
            }
        } while(repetida);
        sorteadas[i] = p_idx;
    }

    // Inicializa o Raylib com 1000x800
    InitWindow(1000, 800, "Minijogo: Quiz da Copa");
    Font fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf"); // Carrega a fonte
    SetTargetFPS(60);

    EstadoQuiz estado = TELA_PERGUNTA;

    // Cores tematicas
    Color verdeCampo = (Color){ 34, 139, 34, 255 };
    Color amareloBrasil = (Color){ 255, 215, 0, 255 };
    Color azulBrasil = (Color){ 0, 39, 118, 255 };

    // Retângulos centralizados na tela de 1000px de largura
    Rectangle btnOpcoes[4] = {
        { 200, 250, 600, 60 },
        { 200, 330, 600, 60 },
        { 200, 410, 600, 60 },
        { 200, 490, 600, 60 }
    };

    Rectangle btnContinuar = { 300, 600, 400, 60 };

    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();

        // Lógica de Atualização
        if (estado == TELA_PERGUNTA) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                for (int i = 0; i < 4; i++) {
                    if (CheckCollisionPointRec(mousePoint, btnOpcoes[i])) {
                        opcao_selecionada = i;
                        if (opcao_selecionada == banco[sorteadas[pergunta_atual]].correta) {
                            score++;
                        }
                        estado = TELA_FEEDBACK;
                    }
                }
            }
        } else if (estado == TELA_FEEDBACK || estado == TELA_FIM) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePoint, btnContinuar)) {
                if (estado == TELA_FEEDBACK) {
                    pergunta_atual++;
                    if (pergunta_atual >= num_perguntas) {
                        estado = TELA_FIM;
                    } else {
                        estado = TELA_PERGUNTA;
                        opcao_selecionada = -1;
                    }
                } else if (estado == TELA_FIM) {
                    break;
                }
            }
        }

        // Lógica de Desenho
        BeginDrawing();
        ClearBackground(verdeCampo);

        if (estado == TELA_PERGUNTA) {
            const char* numPergText = TextFormat("Pergunta %d de %d", pergunta_atual + 1, num_perguntas);
            DrawTextEx(fonteCopa, numPergText, (Vector2){ 40, 40 }, 14, 2, LIGHTGRAY);
            
            // Centraliza o texto da pergunta
            const char* textoPergunta = banco[sorteadas[pergunta_atual]].pergunta;
            int widthPergunta = MeasureTextEx(fonteCopa, textoPergunta, 16, 2).x;
            DrawTextEx(fonteCopa, textoPergunta, (Vector2){ 500 - (widthPergunta / 2), 120 }, 16, 2, WHITE);

            for (int i = 0; i < 4; i++) {
                bool mouseEmCima = CheckCollisionPointRec(mousePoint, btnOpcoes[i]);
                DrawRectangleRec(btnOpcoes[i], mouseEmCima ? LIGHTGRAY : amareloBrasil);
                DrawRectangleLinesEx(btnOpcoes[i], 2, mouseEmCima ? azulBrasil : DARKGRAY);
                
                const char* textoOpcao = banco[sorteadas[pergunta_atual]].opcoes[i];
                int widthOpcao = MeasureTextEx(fonteCopa, textoOpcao, 14, 2).x;
                DrawTextEx(fonteCopa, textoOpcao, (Vector2){ btnOpcoes[i].x + (btnOpcoes[i].width / 2) - (widthOpcao / 2), btnOpcoes[i].y + 22 }, 14, 2, azulBrasil);
            }
        } 
        else if (estado == TELA_FEEDBACK) {
            bool acertou = (opcao_selecionada == banco[sorteadas[pergunta_atual]].correta);
            const char* msg = acertou ? ">>> CORRETO! <<<" : ">>> INCORRETO! <<<";
            Color corMsg = acertou ? amareloBrasil : RED;

            int widthMsg = MeasureTextEx(fonteCopa, msg, 28, 2).x;
            DrawTextEx(fonteCopa, msg, (Vector2){ 500 - (widthMsg / 2), 250 }, 28, 2, corMsg);
            
            if (!acertou) {
                int certa = banco[sorteadas[pergunta_atual]].correta;
                const char* textoCerta = TextFormat("A resposta certa era: %s", banco[sorteadas[pergunta_atual]].opcoes[certa]);
                int widthCerta = MeasureTextEx(fonteCopa, textoCerta, 14, 2).x;
                DrawTextEx(fonteCopa, textoCerta, (Vector2){ 500 - (widthCerta / 2), 350 }, 14, 2, WHITE);
            }

            // Botão continuar
            bool mouseEmCima = CheckCollisionPointRec(mousePoint, btnContinuar);
            DrawRectangleRec(btnContinuar, mouseEmCima ? LIGHTGRAY : amareloBrasil);
            int widthCont = MeasureTextEx(fonteCopa, "Continuar", 16, 2).x;
            DrawTextEx(fonteCopa, "Continuar", (Vector2){ btnContinuar.x + (btnContinuar.width / 2) - (widthCont / 2), btnContinuar.y + 22 }, 16, 2, azulBrasil);
        }
        else if (estado == TELA_FIM) {
            const char* msgFim = "--- FIM DO QUIZ ---";
            int widthFim = MeasureTextEx(fonteCopa, msgFim, 28, 2).x;
            DrawTextEx(fonteCopa, msgFim, (Vector2){ 500 - (widthFim / 2), 150 }, 28, 2, WHITE);
            
            const char* msgScore = TextFormat("Voce acertou %d de %d!", score, num_perguntas);
            int widthScore = MeasureTextEx(fonteCopa, msgScore, 20, 2).x;
            DrawTextEx(fonteCopa, msgScore, (Vector2){ 500 - (widthScore / 2), 250 }, 20, 2, amareloBrasil);

            const char* msgRecompensa;
            Color corRecompensa;
            if (score > 0) {
                msgRecompensa = "Voce ganhou pacotes de recompensa!";
                corRecompensa = amareloBrasil;
            } else {
                msgRecompensa = "Sem recompensas desta vez. Tente novamente!";
                corRecompensa = RED;
            }
            int widthRec = MeasureTextEx(fonteCopa, msgRecompensa, 14, 2).x;
            DrawTextEx(fonteCopa, msgRecompensa, (Vector2){ 500 - (widthRec / 2), 350 }, 14, 2, corRecompensa);

            // Botão sair
            bool mouseEmCima = CheckCollisionPointRec(mousePoint, btnContinuar);
            DrawRectangleRec(btnContinuar, mouseEmCima ? LIGHTGRAY : amareloBrasil);
            int widthSair = MeasureTextEx(fonteCopa, "Sair", 16, 2).x;
            DrawTextEx(fonteCopa, "Sair", (Vector2){ btnContinuar.x + (btnContinuar.width / 2) - (widthSair / 2), btnContinuar.y + 22 }, 16, 2, azulBrasil);
        }

        EndDrawing();
    }

    UnloadFont(fonteCopa); // Libera a memória da fonte
    CloseWindow();

    if (score > 0) {
        printf("\n[Quiz Finalizado] Parabens! Voce acertou %d perguntas!\n", score);
        printf("Pressione ENTER para abrir sua recompensa...\n");
        getchar();
        abrirPacote(figurinhas, mochila, album, total, total_mochila, total_album);
    } else {
        printf("\n[Quiz Finalizado] Voce nao acertou nenhuma, sem recompensas.\n");
    }
}