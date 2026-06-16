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
    // BANCO EXPANDIDO: 20 perguntas
    PerguntaQuiz banco[20] = {
        {"Quem ganhou a Copa do Mundo de 2002?", {"Brasil", "Alemanha", "Italia", "Argentina"}, 0},
        {"Qual pais sediou a Copa de 2014?", {"Africa do Sul", "Brasil", "Russia", "Alemanha"}, 1},
        {"Quem e o maior artilheiro das Copas do Mundo?", {"Pele", "Ronaldo", "Miroslav Klose", "Messi"}, 2},
        {"Qual selecao ganhou a primeira Copa em 1930?", {"Brasil", "Argentina", "Uruguai", "Italia"}, 2},
        {"Qual jogador fez o gol 'A Mao de Deus'?", {"Pele", "Diego Maradona", "Zinedine Zidane", "Johan Cruyff"}, 1},
        {"Quem marcou o gol do titulo da Espanha em 2010?", {"Xavi", "Fernando Torres", "Andres Iniesta", "David Villa"}, 2},
        {"Qual pais sediou a Copa do Mundo de 1994?", {"EUA", "Italia", "Franca", "Japao"}, 0},
        {"Quem e o unico tricampeao mundial como jogador?", {"Maradona", "Pele", "Cafu", "Ronaldo"}, 1},
        {"Qual selecao sofreu o 'Maracanazo' em 1950?", {"Argentina", "Alemanha", "Italia", "Brasil"}, 3},
        {"Quem ganhou a Copa do Mundo de 2018?", {"Croacia", "Brasil", "Franca", "Alemanha"}, 2},
        {"Qual foi o mascote da Copa de 2014 no Brasil?", {"Fuleco", "Zakumi", "Zabivaka", "La'eeb"}, 0},
        {"Quem levou a cabecada de Zidane na final de 2006?", {"Grosso", "Cannavaro", "Materazzi", "Maldini"}, 2},
        {"Qual selecao venceu a Copa do Mundo de 2022?", {"Franca", "Argentina", "Croacia", "Marrocos"}, 1},
        {"Qual pais sediou a primeira Copa do Mundo em 1930?", {"Brasil", "Argentina", "Chile", "Uruguai"}, 3},
        {"Quem foi o capitao do pentacampeonato do Brasil?", {"Ronaldo", "Rivaldo", "Roberto Carlos", "Cafu"}, 3},
        {"Qual destas selecoes nunca venceu uma Copa?", {"Espanha", "Inglaterra", "Franca", "Holanda"}, 3},
        {"Quem marcou dois gols na final de 1998 pela Franca?", {"Henry", "Zidane", "Trezeguet", "Platini"}, 1},
        {"Qual selecao e conhecida como 'Laranja Mecanica'?", {"Costa do Marfim", "Holanda", "Espanha", "Belgica"}, 1},
        {"Qual a selecao com mais participacoes em Copas?", {"Alemanha", "Brasil", "Italia", "Argentina"}, 1},
        {"Em que ano o Brasil ganhou sua primeira Copa?", {"1958", "1962", "1970", "1994"}, 0}
    };

    int num_perguntas = 7; // ATUALIZADO: Agora o jogador responde 7 perguntas
    int total_no_banco = 20; 
    int sorteadas[7];      // ATUALIZADO: O vetor precisa ter o mesmo tamanho de num_perguntas
    int score = 0;
    int pergunta_atual = 0;
    int opcao_selecionada = -1;
    
    // Sorteia as perguntas sem repetir
    for(int i = 0; i < num_perguntas; i++) {
        int p_idx, repetida;
        do {
            repetida = 0;
            p_idx = rand() % total_no_banco;
            for(int j = 0; j < i; j++) {
                if(sorteadas[j] == p_idx) repetida = 1;
            }
        } while(repetida);
        sorteadas[i] = p_idx;
    }

    InitWindow(1000, 800, "Minijogo: Quiz da Copa");
    Font fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf"); 
    SetTargetFPS(60);

    EstadoQuiz estado = TELA_PERGUNTA;

    Color verdeCampo = (Color){ 34, 139, 34, 255 };
    Color amareloBrasil = (Color){ 255, 215, 0, 255 };
    Color azulBrasil = (Color){ 0, 39, 118, 255 };

    Rectangle btnOpcoes[4] = {
        { 200, 250, 600, 60 },
        { 200, 330, 600, 60 },
        { 200, 410, 600, 60 },
        { 200, 490, 600, 60 }
    };

    Rectangle btnContinuar = { 300, 600, 400, 60 };

    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();

        // ==========================================
        // Lógica de Atualização
        // ==========================================
        if (estado == TELA_PERGUNTA) {
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
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
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePoint, btnContinuar)) {
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

        // ==========================================
        // Lógica de Desenho
        // ==========================================
        BeginDrawing();
        ClearBackground(verdeCampo);

        if (estado == TELA_PERGUNTA) {
            const char* numPergText = TextFormat("Pergunta %d de %d", pergunta_atual + 1, num_perguntas);
            DrawTextEx(fonteCopa, numPergText, (Vector2){ 42, 42 }, 14, 2, BLACK); 
            DrawTextEx(fonteCopa, numPergText, (Vector2){ 40, 40 }, 14, 2, LIGHTGRAY); 
            
            const char* textoPergunta = banco[sorteadas[pergunta_atual]].pergunta;
            int widthPergunta = MeasureTextEx(fonteCopa, textoPergunta, 16, 2).x;
            DrawTextEx(fonteCopa, textoPergunta, (Vector2){ 500 - (widthPergunta / 2) + 2, 120 + 2 }, 16, 2, BLACK); 
            DrawTextEx(fonteCopa, textoPergunta, (Vector2){ 500 - (widthPergunta / 2), 120 }, 16, 2, WHITE); 

            for (int i = 0; i < 4; i++) {
                bool mouseEmCima = CheckCollisionPointRec(mousePoint, btnOpcoes[i]);
                int offsetAnimacao = 0;

                if (mouseEmCima) {
                    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                        offsetAnimacao = 2; 
                    } else {
                        offsetAnimacao = -6; 
                        DrawRectangle(btnOpcoes[i].x + 4, btnOpcoes[i].y + 4, btnOpcoes[i].width, btnOpcoes[i].height, Fade(BLACK, 0.4f));
                    }
                }

                Rectangle btnAnimado = { btnOpcoes[i].x, btnOpcoes[i].y + offsetAnimacao, btnOpcoes[i].width, btnOpcoes[i].height };

                DrawRectangleRec(btnAnimado, mouseEmCima ? LIGHTGRAY : amareloBrasil);
                DrawRectangleLinesEx(btnAnimado, 2, mouseEmCima ? azulBrasil : DARKGRAY);
                
                const char* textoOpcao = banco[sorteadas[pergunta_atual]].opcoes[i];
                int widthOpcao = MeasureTextEx(fonteCopa, textoOpcao, 14, 2).x;
                DrawTextEx(fonteCopa, textoOpcao, (Vector2){ btnAnimado.x + (btnAnimado.width / 2) - (widthOpcao / 2), btnAnimado.y + 22 }, 14, 2, azulBrasil);
            }
        } 
        else if (estado == TELA_FEEDBACK) {
            bool acertou = (opcao_selecionada == banco[sorteadas[pergunta_atual]].correta);
            const char* msg = acertou ? ">>> CORRETO! <<<" : ">>> INCORRETO! <<<";
            Color corMsg = acertou ? amareloBrasil : RED;

            int widthMsg = MeasureTextEx(fonteCopa, msg, 28, 2).x;
            
            if ((int)(GetTime() * 3) % 2 == 0) {
                DrawTextEx(fonteCopa, msg, (Vector2){ 500 - (widthMsg / 2) + 3, 250 + 3 }, 28, 2, BLACK); 
                DrawTextEx(fonteCopa, msg, (Vector2){ 500 - (widthMsg / 2), 250 }, 28, 2, corMsg); 
            }
            
            if (!acertou) {
                int certa = banco[sorteadas[pergunta_atual]].correta;
                const char* textoCerta = TextFormat("A resposta certa era: %s", banco[sorteadas[pergunta_atual]].opcoes[certa]);
                int widthCerta = MeasureTextEx(fonteCopa, textoCerta, 14, 2).x;
                DrawTextEx(fonteCopa, textoCerta, (Vector2){ 500 - (widthCerta / 2) + 2, 350 + 2 }, 14, 2, BLACK); 
                DrawTextEx(fonteCopa, textoCerta, (Vector2){ 500 - (widthCerta / 2), 350 }, 14, 2, WHITE);
            }

            bool mouseEmCima = CheckCollisionPointRec(mousePoint, btnContinuar);
            int offsetAnimacao = 0;

            if (mouseEmCima) {
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) offsetAnimacao = 2;
                else {
                    offsetAnimacao = -6;
                    DrawRectangle(btnContinuar.x + 4, btnContinuar.y + 4, btnContinuar.width, btnContinuar.height, Fade(BLACK, 0.4f));
                }
            }

            Rectangle btnAnimado = { btnContinuar.x, btnContinuar.y + offsetAnimacao, btnContinuar.width, btnContinuar.height };

            DrawRectangleRec(btnAnimado, mouseEmCima ? LIGHTGRAY : amareloBrasil);
            DrawRectangleLinesEx(btnAnimado, 2, mouseEmCima ? azulBrasil : DARKGRAY);
            
            int widthCont = MeasureTextEx(fonteCopa, "Continuar", 16, 2).x;
            DrawTextEx(fonteCopa, "Continuar", (Vector2){ btnAnimado.x + (btnAnimado.width / 2) - (widthCont / 2), btnAnimado.y + 22 }, 16, 2, azulBrasil);
        }
        else if (estado == TELA_FIM) {
            const char* msgFim = "--- FIM DO QUIZ ---";
            int widthFim = MeasureTextEx(fonteCopa, msgFim, 28, 2).x;
            DrawTextEx(fonteCopa, msgFim, (Vector2){ 500 - (widthFim / 2) + 3, 150 + 3 }, 28, 2, BLACK); 
            DrawTextEx(fonteCopa, msgFim, (Vector2){ 500 - (widthFim / 2), 150 }, 28, 2, WHITE);
            
            const char* msgScore = TextFormat("Voce acertou %d de %d!", score, num_perguntas);
            int widthScore = MeasureTextEx(fonteCopa, msgScore, 20, 2).x;
            DrawTextEx(fonteCopa, msgScore, (Vector2){ 500 - (widthScore / 2) + 2, 250 + 2 }, 20, 2, BLACK); 
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
            DrawTextEx(fonteCopa, msgRecompensa, (Vector2){ 500 - (widthRec / 2) + 2, 350 + 2 }, 14, 2, BLACK); 
            DrawTextEx(fonteCopa, msgRecompensa, (Vector2){ 500 - (widthRec / 2), 350 }, 14, 2, corRecompensa);

            bool mouseEmCima = CheckCollisionPointRec(mousePoint, btnContinuar);
            int offsetAnimacao = 0;

            if (mouseEmCima) {
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) offsetAnimacao = 2;
                else {
                    offsetAnimacao = -6;
                    DrawRectangle(btnContinuar.x + 4, btnContinuar.y + 4, btnContinuar.width, btnContinuar.height, Fade(BLACK, 0.4f));
                }
            }

            Rectangle btnAnimado = { btnContinuar.x, btnContinuar.y + offsetAnimacao, btnContinuar.width, btnContinuar.height };

            DrawRectangleRec(btnAnimado, mouseEmCima ? LIGHTGRAY : amareloBrasil);
            DrawRectangleLinesEx(btnAnimado, 2, mouseEmCima ? azulBrasil : DARKGRAY);
            
            int widthSair = MeasureTextEx(fonteCopa, "Sair", 16, 2).x;
            DrawTextEx(fonteCopa, "Sair", (Vector2){ btnAnimado.x + (btnAnimado.width / 2) - (widthSair / 2), btnAnimado.y + 22 }, 16, 2, azulBrasil);
        }

        EndDrawing();
    }

    UnloadFont(fonteCopa);
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