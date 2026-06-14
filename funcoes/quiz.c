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

    // Inicializa o Raylib
    InitWindow(1000, 800, "Minijogo: Quiz da Copa");
    SetTargetFPS(60);

    EstadoQuiz estado = TELA_PERGUNTA;

    // Retângulos para os botões das opções
    Rectangle btnOpcoes[4] = {
        { 100, 200, 600, 60 },
        { 100, 280, 600, 60 },
        { 100, 360, 600, 60 },
        { 100, 440, 600, 60 }
    };

    Rectangle btnContinuar = { 300, 500, 200, 50 };

    while (!WindowShouldClose()) {
        // Lógica de Atualização
        Vector2 mousePoint = GetMousePosition();

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
                    break; // Sai do loop do Raylib para voltar ao terminal
                }
            }
        }

        // Lógica de Desenho
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (estado == TELA_PERGUNTA) {
            DrawText(TextFormat("Pergunta %d de %d", pergunta_atual + 1, num_perguntas), 20, 20, 20, DARKGRAY);
            DrawText(banco[sorteadas[pergunta_atual]].pergunta, 100, 100, 25, BLACK);

            for (int i = 0; i < 4; i++) {
                Color btnColor = CheckCollisionPointRec(mousePoint, btnOpcoes[i]) ? LIGHTGRAY : GRAY;
                DrawRectangleRec(btnOpcoes[i], btnColor);
                DrawText(banco[sorteadas[pergunta_atual]].opcoes[i], btnOpcoes[i].x + 20, btnOpcoes[i].y + 20, 20, WHITE);
            }
        } 
        else if (estado == TELA_FEEDBACK) {
            bool acertou = (opcao_selecionada == banco[sorteadas[pergunta_atual]].correta);
            const char* msg = acertou ? ">>> CORRETO! <<<" : ">>> INCORRETO! <<<";
            Color corMsg = acertou ? GREEN : RED;

            DrawText(msg, 250, 200, 30, corMsg);
            
            if (!acertou) {
                int certa = banco[sorteadas[pergunta_atual]].correta;
                DrawText(TextFormat("A resposta certa era: %s", banco[sorteadas[pergunta_atual]].opcoes[certa]), 150, 300, 20, DARKGRAY);
            }

            // Botão continuar
            DrawRectangleRec(btnContinuar, CheckCollisionPointRec(mousePoint, btnContinuar) ? DARKBLUE : BLUE);
            DrawText("Continuar", btnContinuar.x + 50, btnContinuar.y + 15, 20, WHITE);
        }
        else if (estado == TELA_FIM) {
            DrawText("--- FIM DO QUIZ ---", 250, 150, 30, BLACK);
            DrawText(TextFormat("Voce acertou %d de %d!", score, num_perguntas), 250, 250, 30, DARKGRAY);

            if (score > 0) {
                DrawText("Voce ganhou pacotes de recompensa!", 150, 350, 25, GREEN);
            } else {
                DrawText("Sem recompensas desta vez. Tente novamente!", 120, 350, 25, RED);
            }

            // Botão sair
            DrawRectangleRec(btnContinuar, CheckCollisionPointRec(mousePoint, btnContinuar) ? DARKBLUE : BLUE);
            DrawText("Sair", btnContinuar.x + 80, btnContinuar.y + 15, 20, WHITE);
        }

        EndDrawing();
    }

    // Fecha a janela gráfica
    CloseWindow();

    // Volta ao terminal para dar a recompensa
    if (score > 0) {
        printf("\n[Quiz Finalizado] Parabens! Voce acertou %d perguntas!\n", score);
        printf("Pressione ENTER para abrir sua recompensa...\n");
        getchar(); // Pausa para o usuário ler
        abrirPacote(figurinhas, mochila, album, total, total_mochila, total_album);
    } else {
        printf("\n[Quiz Finalizado] Voce nao acertou nenhuma, sem recompensas.\n");
    }
}