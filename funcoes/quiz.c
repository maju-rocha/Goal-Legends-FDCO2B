#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include <raylib.h>
#include "../headers/biblioteca.h"

typedef struct {
    char pergunta[200];
    char opcoes[4][100];
    int correta; 
} PerguntaQuiz;

typedef enum { TELA_PERGUNTA, TELA_FEEDBACK, TELA_FIM } EstadoQuiz;

void jogarQuiz(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album) {
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

    int num_perguntas = 7; 
    int total_no_banco = 20; 
    int sorteadas[7];      
    int score = 0;
    int pergunta_atual = 0;
    int opcao_selecionada = -1;
    
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

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(1000, 800, "Minijogo: Quiz da Copa - Edicao Canarinho");
    
    Font fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf"); 
    
    HideCursor(); 
    Image imagemBola = LoadImage("extras/bola_cursor.png"); 
    ImageResize(&imagemBola, 40, 40); 
    Texture2D cursorBola = LoadTextureFromImage(imagemBola); 
    UnloadImage(imagemBola); 
    
    SetTargetFPS(60);

    EstadoQuiz estado = TELA_PERGUNTA;

    Color verdeCampo = (Color){ 34, 139, 34, 255 };
    Color amareloBrasil = (Color){ 255, 215, 0, 255 };
    Color azulBrasil = (Color){ 0, 39, 118, 255 };
    Color sombraUI = Fade(BLACK, 0.4f); 

    Rectangle btnOpcoes[4] = {
        { 180, 270, 640, 58 },
        { 180, 350, 640, 58 },
        { 180, 430, 640, 58 },
        { 180, 510, 640, 58 }
    };

    Rectangle btnContinuar = { 320, 620, 360, 58 };

    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();
        float tempo = (float)GetTime();

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

        // Background Dinâmico Animado
        float offsetBg = tempo * 40.0f; 
        for (int i = -1000; i < 2000; i += 80) {
            DrawLineEx((Vector2){ i + offsetBg, 0 }, (Vector2){ i - 1000 + offsetBg, 1500 }, 20.0f, Fade(WHITE, 0.05f));
        }

        if (estado == TELA_PERGUNTA) {
            Rectangle hudPlacar = { 35, 30, 260, 50 };
            DrawRectangleRounded((Rectangle){hudPlacar.x + 4, hudPlacar.y + 4, hudPlacar.width, hudPlacar.height}, 0.2f, 4, sombraUI); 
            DrawRectangleRounded(hudPlacar, 0.2f, 4, azulBrasil);
            DrawRectangleRoundedLinesEx(hudPlacar, 0.2f, 4, 2.0f, amareloBrasil);

            const char* numPergText = TextFormat("RODADA: %d / %d", pergunta_atual + 1, num_perguntas);
            DrawTextEx(fonteCopa, numPergText, (Vector2){ 55, 48 }, 12, 1, amareloBrasil); 

            Rectangle cardPergunta = { 100, 110, 800, 130 };
            DrawRectangleRounded((Rectangle){cardPergunta.x + 8, cardPergunta.y + 8, cardPergunta.width, cardPergunta.height}, 0.08f, 6, sombraUI); 
            DrawRectangleRounded(cardPergunta, 0.08f, 6, azulBrasil);
            DrawRectangleRoundedLinesEx(cardPergunta, 0.08f, 6, 2.5f, amareloBrasil);
            
            const char* textoPergunta = banco[sorteadas[pergunta_atual]].pergunta;
            int widthPergunta = MeasureTextEx(fonteCopa, textoPergunta, 13, 1).x;
            Vector2 posPergunta = { 500 - (widthPergunta / 2), 165 };
            if(widthPergunta > 760) posPergunta.x = 120; 
            
            DrawTextEx(fonteCopa, textoPergunta, posPergunta, 13, 1, WHITE); 

            for (int i = 0; i < 4; i++) {
                bool mouseEmCima = CheckCollisionPointRec(mousePoint, btnOpcoes[i]);
                int offsetAnimacao = 0;
                Color corFundoBtn = amareloBrasil;
                Color corBordaBtn = azulBrasil;
                Color corTextoBtn = azulBrasil;

                if (mouseEmCima) {
                    if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                        offsetAnimacao = 4; 
                    } else {
                        offsetAnimacao = -4; 
                        corFundoBtn = azulBrasil;
                        corBordaBtn = amareloBrasil;
                        corTextoBtn = WHITE;
                        
                        Rectangle shadow = { btnOpcoes[i].x + 6, btnOpcoes[i].y + 6, btnOpcoes[i].width, btnOpcoes[i].height };
                        DrawRectangleRounded(shadow, 0.15f, 4, sombraUI);
                    }
                } else {
                    Rectangle shadow = { btnOpcoes[i].x + 3, btnOpcoes[i].y + 3, btnOpcoes[i].width, btnOpcoes[i].height };
                    DrawRectangleRounded(shadow, 0.15f, 4, sombraUI);
                }

                Rectangle btnAnimado = { btnOpcoes[i].x, btnOpcoes[i].y + offsetAnimacao, btnOpcoes[i].width, btnOpcoes[i].height };

                DrawRectangleRounded(btnAnimado, 0.15f, 4, corFundoBtn);
                DrawRectangleRoundedLinesEx(btnAnimado, 0.15f, 4, 2.0f, corBordaBtn);
                
                const char* textoOpcao = banco[sorteadas[pergunta_atual]].opcoes[i];
                int widthOpcao = MeasureTextEx(fonteCopa, textoOpcao, 12, 1).x;
                DrawTextEx(fonteCopa, textoOpcao, (Vector2){ btnAnimado.x + (btnAnimado.width / 2) - (widthOpcao / 2), btnAnimado.y + 22 }, 12, 1, corTextoBtn);
            }
        } 
        else if (estado == TELA_FEEDBACK) {
            bool acertou = (opcao_selecionada == banco[sorteadas[pergunta_atual]].correta);
            
            // Alteração de cor dinâmica: Verde para acerto, Vermelho para erro
            Color corDestaque = acertou ? GREEN : RED; 
            
            Rectangle cardFeedback = { 150, 180, 700, 360 };
            DrawRectangleRounded((Rectangle){cardFeedback.x + 10, cardFeedback.y + 10, cardFeedback.width, cardFeedback.height}, 0.05f, 6, sombraUI); 
            DrawRectangleRounded(cardFeedback, 0.05f, 6, azulBrasil);
            DrawRectangleRoundedLinesEx(cardFeedback, 0.05f, 6, 3.0f, corDestaque); 

            const char* msg = acertou ? "CORRETO! VITORIA!" : "RESPOSTA INCORRETA";
            float textSize = 22 + sin(tempo * 6.0f) * 2; 
            int widthMsg = MeasureTextEx(fonteCopa, msg, textSize, 2).x;
            
            DrawTextEx(fonteCopa, msg, (Vector2){ 500 - (widthMsg / 2) + 3, 250 + 3 }, textSize, 2, Fade(BLACK, 0.6f)); 
            DrawTextEx(fonteCopa, msg, (Vector2){ 500 - (widthMsg / 2), 250 }, textSize, 2, corDestaque); 
            
            if (!acertou) {
                int certa = banco[sorteadas[pergunta_atual]].correta;
                const char* textoCerta = TextFormat("RESPOSTA CERTA: %s", banco[sorteadas[pergunta_atual]].opcoes[certa]);
                int widthCerta = MeasureTextEx(fonteCopa, textoCerta, 12, 1).x;
                DrawTextEx(fonteCopa, textoCerta, (Vector2){ 500 - (widthCerta / 2), 360 }, 12, 1, WHITE);
            } else {
                const char* textoCerta = "+1 PONTO COMPUTADO";
                int widthCerta = MeasureTextEx(fonteCopa, textoCerta, 12, 1).x;
                DrawTextEx(fonteCopa, textoCerta, (Vector2){ 500 - (widthCerta / 2), 360 }, 12, 1, Fade(WHITE, 0.7f));
            }

            bool mouseEmCima = CheckCollisionPointRec(mousePoint, btnContinuar);
            int offsetAnimacao = 0;
            Color corFundoBtn = amareloBrasil;

            if (mouseEmCima) {
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) offsetAnimacao = 4;
                else {
                    offsetAnimacao = -4;
                    corFundoBtn = WHITE;
                    DrawRectangleRounded((Rectangle){ btnContinuar.x + 6, btnContinuar.y + 6, btnContinuar.width, btnContinuar.height }, 0.15f, 4, sombraUI);
                }
            } else {
                DrawRectangleRounded((Rectangle){ btnContinuar.x + 3, btnContinuar.y + 3, btnContinuar.width, btnContinuar.height }, 0.15f, 4, sombraUI);
            }

            Rectangle btnAnimado = { btnContinuar.x, btnContinuar.y + offsetAnimacao, btnContinuar.width, btnContinuar.height };
            DrawRectangleRounded(btnAnimado, 0.15f, 4, corFundoBtn);
            DrawRectangleRoundedLinesEx(btnAnimado, 0.15f, 4, 2.0f, azulBrasil);
            
            int widthCont = MeasureTextEx(fonteCopa, "CONTINUAR >>", 12, 1).x;
            DrawTextEx(fonteCopa, "CONTINUAR >>", (Vector2){ btnAnimado.x + (btnAnimado.width / 2) - (widthCont / 2), btnAnimado.y + 22 }, 12, 1, azulBrasil);
        }
        else if (estado == TELA_FIM) {
            Rectangle cardFim = { 150, 120, 700, 440 };
            DrawRectangleRounded((Rectangle){cardFim.x + 10, cardFim.y + 10, cardFim.width, cardFim.height}, 0.05f, 6, sombraUI);
            DrawRectangleRounded(cardFim, 0.05f, 6, azulBrasil);
            DrawRectangleRoundedLinesEx(cardFim, 0.05f, 6, 3.0f, amareloBrasil);

            const char* msgFim = "--- FIM DO QUIZ ---";
            int widthFim = MeasureTextEx(fonteCopa, msgFim, 24, 2).x;
            DrawTextEx(fonteCopa, msgFim, (Vector2){ 500 - (widthFim / 2) + 3, 190 + 3 }, 24, 2, BLACK);
            DrawTextEx(fonteCopa, msgFim, (Vector2){ 500 - (widthFim / 2), 190 }, 24, 2, WHITE);
            
            const char* msgScore = TextFormat("VOCE ACERTOU %d DE %d!", score, num_perguntas);
            int widthScore = MeasureTextEx(fonteCopa, msgScore, 16, 1).x;
            DrawTextEx(fonteCopa, msgScore, (Vector2){ 500 - (widthScore / 2), 290 }, 16, 1, amareloBrasil);

            float alphaRec = 0.6f + sin(tempo * 5.0f) * 0.4f;
            const char* msgRecompensa = (score > 0) ? "RECOMPENSAS LIBERADAS NO TERMINAL!" : "SEM RECOMPENSAS DESTA VEZ.";
            int widthRec = MeasureTextEx(fonteCopa, msgRecompensa, 12, 1).x;
            DrawTextEx(fonteCopa, msgRecompensa, (Vector2){ 500 - (widthRec / 2), 370 }, 12, 1, Fade(WHITE, alphaRec));

            bool mouseEmCima = CheckCollisionPointRec(mousePoint, btnContinuar);
            int offsetAnimacao = 0;
            Color corFundoBtn = amareloBrasil;

            if (mouseEmCima) {
                if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) offsetAnimacao = 4;
                else {
                    offsetAnimacao = -4;
                    corFundoBtn = WHITE;
                    DrawRectangleRounded((Rectangle){ btnContinuar.x + 6, btnContinuar.y + 6, btnContinuar.width, btnContinuar.height }, 0.15f, 4, sombraUI);
                }
            } else {
                DrawRectangleRounded((Rectangle){ btnContinuar.x + 3, btnContinuar.y + 3, btnContinuar.width, btnContinuar.height }, 0.15f, 4, sombraUI);
            }
            int widthRec = MeasureText(msgRecompensa, 30);
            DrawText(msgRecompensa, 500 - (widthRec / 2), 350, 30, corRecompensa);

            Rectangle btnAnimado = { btnContinuar.x, btnContinuar.y + offsetAnimacao, btnContinuar.width, btnContinuar.height };
            DrawRectangleRounded(btnAnimado, 0.15f, 4, corFundoBtn);
            DrawRectangleRoundedLinesEx(btnAnimado, 0.15f, 4, 2.0f, azulBrasil);
            
            int widthSair = MeasureTextEx(fonteCopa, "SAIR DO JOGO", 12, 1).x;
            DrawTextEx(fonteCopa, "SAIR DO JOGO", (Vector2){ btnAnimado.x + (btnAnimado.width / 2) - (widthSair / 2), btnAnimado.y + 22 }, 12, 1, azulBrasil);
        }

        // ==========================================
        // RENDER DO CURSOR
        // ==========================================
        DrawTexture(cursorBola, (int)mousePoint.x - cursorBola.width/2, (int)mousePoint.y - cursorBola.height/2, WHITE);

        EndDrawing();
    }

    UnloadTexture(cursorBola);
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