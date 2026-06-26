#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h> 
#include <raylib.h>
#include <time.h>
#include "biblioteca.h"
#include "global.h"
#include "quiz.h"
#include "salvarPacotes.h"


typedef struct{ 
    char pergunta[200];
    char opcoes[4][100];
    int correta; 
}PerguntaQuiz;

typedef enum{ TELA_PERGUNTA, TELA_FEEDBACK, TELA_FIM }EstadoQuiz;

void jogarQuiz(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album){
    PerguntaQuiz banco[20] = {
        {"Quem ganhou a Copa do Mundo de 2002?", {"Brasil", "Alemanha", "Italia", "Argentina"}, 0},
        {"Qual pais sediou a Copa de 2014?", {"Africa do Sul", "Brasil", "Russia", "Alemanha"}, 1},
        {"Quem e o maior artilheiro das Copas do Mundo?", {"Pele", "Ronaldo", "Miroslav Klose", "Messi"}, 3},
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

    //Váriaveis
    int num_perguntas = 7; 
    int total_no_banco = 20; 
    int sorteadas[7];      
    int score = 0;
    int pergunta_atual = 0;
    int opcao_selecionada = -1;
    int pacotes_ganhos_rodada = 0;
    bool voltarMenu = false; // Volta para o menu sem fechar o projeto
    bool fecharProjeto = false; // Fecha o projeto inteiro ao clicar no X

    //Semente para randomização
    srand(time(NULL));
    
    //Sorteio de perguntas
    for(int i = 0; i < num_perguntas; i++){
        int p_idx, repetida;
        do{
            repetida = 0;
            p_idx = rand() % total_no_banco;
            for(int j = 0; j < i; j++){
                if(sorteadas[j] == p_idx)repetida = 1;
            }
        }while(repetida);
        sorteadas[i] = p_idx;
    }

    SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT);
    InitWindow(1000, 800, "Minijogo: Quiz da Copa - Edicao Canarinho");
    SetExitKey(KEY_NULL);
    
    //Váriavel para começo de audio
    bool audioIniciadoAqui = false;

    //Teste de audio
    if(!IsAudioDeviceReady()){
        InitAudioDevice();
        audioIniciadoAqui = true;
    }

    //Váriaveis de audio
    Sound somAcerto = LoadSound("audio/correct.mp3");
    Sound somErro = LoadSound("audio/false.mp3");
    Music musicaFundo = LoadMusicStream("audio/music.mp3");
    SetMusicVolume(musicaFundo, 0.08f); 
    PlayMusicStream(musicaFundo);      

    //Váriavel da fonte
    Font fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf"); 
    
    //Váriaveis de imagem para o cursor
    HideCursor(); 
    Image imagemBola = LoadImage("imagens/bola_cursor.png"); 
    ImageResize(&imagemBola, 40, 40); 
    Texture2D cursorBola = LoadTextureFromImage(imagemBola); 
    
    SetTargetFPS(60);

    //Váriavel de estado
    EstadoQuiz estado = TELA_PERGUNTA;

    //Váriaveis para cores
    Color verdeCampo = (Color){ 34, 139, 34, 255 };
    Color amareloBrasil = (Color){ 255, 215, 0, 255 };
    Color azulBrasil = (Color){ 0, 39, 118, 255 };
    Color sombraUI = Fade(BLACK, 0.4f); 

    //Retângulos para opções
    Rectangle btnOpcoes[4] = {
        { 180, 270, 640, 58 },
        { 180, 350, 640, 58 },
        { 180, 430, 640, 58 },
        { 180, 510, 640, 58 }
    };

    //Retângulo para o botão continuar
    Rectangle btnContinuar = { 320, 620, 360, 58 };
    
    //Retângulo botões do fim
    Rectangle btnTentar = { 170, 460, 310, 58 };
    Rectangle btnMenu = { 520, 460, 310, 58 };

    //Estrutra do jogo
    while(!voltarMenu){

        // Se clicar no X da janela, fecha o projeto inteiro
        if(WindowShouldClose()){
            fecharProjeto = true;
            break;
        }

        // Se apertar ESC, volta para o menu principal
        if(IsKeyPressed(KEY_ESCAPE)){
            voltarMenu = true;
            break;
        }

        //Música de fundo
        UpdateMusicStream(musicaFundo);

        //Localização do mouse
        Vector2 mousePoint = GetMousePosition();

        //Váriavel de tempo para loop de imagem
        float tempo = (float)GetTime();

        //Lógica para estado de pergunta
        if(estado == TELA_PERGUNTA){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                for(int i = 0; i < 4; i++){
                    if(CheckCollisionPointRec(mousePoint, btnOpcoes[i])){
                        opcao_selecionada = i;
                        
                        if(opcao_selecionada == banco[sorteadas[pergunta_atual]].correta){
                            score++;
                            PlaySound(somAcerto);
                        }else{
                            PlaySound(somErro);
                        }
                        estado = TELA_FEEDBACK;
                    }
                }
            }
        } 
        //Lógica para estado de Feedback(recompensa)
        else if(estado == TELA_FEEDBACK){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePoint, btnContinuar)){
                pergunta_atual++;
                if(pergunta_atual >= num_perguntas){
                    estado = TELA_FIM;
                    
                    // --- CÁLCULO E ACÚMULO DE PACOTES ---
                    pacotes_ganhos_rodada = 0;
                    if(score == 7) pacotes_ganhos_rodada = 3;
                    else if(score >= 6) pacotes_ganhos_rodada = 2;
                    else if(score >= 3) pacotes_ganhos_rodada = 1;
                    
                    //Salva na variável do salvarPacotes.c
                    pacotes_fechados += pacotes_ganhos_rodada;
                    salvarPacotes();

                }else{
                    estado = TELA_PERGUNTA;
                    opcao_selecionada = -1;
                }
            }
        }
        //Estado de tela do fim
        else if(estado == TELA_FIM){
            if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
                //TENTAR NOVAMENTE
                if(CheckCollisionPointRec(mousePoint, btnTentar)){
                    score = 0;
                    pergunta_atual = 0;
                    opcao_selecionada = -1;
                    
                    //Sorteia novas perguntas
                    for(int i = 0; i < num_perguntas; i++){
                        int p_idx, repetida;
                        do{
                            repetida = 0;
                            p_idx = rand() % total_no_banco;
                            for(int j = 0; j < i; j++) {
                                if(sorteadas[j] == p_idx) repetida = 1;
                            }
                        } while(repetida);
                        sorteadas[i] = p_idx;
                    }
                    estado = TELA_PERGUNTA;
                } 
                //Volta ao menu inicial
                else if(CheckCollisionPointRec(mousePoint, btnMenu)){
                    voltarMenu = true;
                }
            }
        }

        // ==========================================
        // Lógica de Desenho
        // ==========================================
        BeginDrawing();
        ClearBackground(verdeCampo);

        //Váriavel para desenho de fundo em loop
        float offsetBg = tempo * 40.0f; 
        for (int i = -1000; i < 2000; i += 80){
            DrawLineEx((Vector2){ i + offsetBg, 0 }, (Vector2){ i - 1000 + offsetBg, 1500 }, 20.0f, Fade(WHITE, 0.05f));
        }

        //Estado de tela de pergunta
        if (estado == TELA_PERGUNTA){
            
            //Retângulo do placar
            Rectangle hudPlacar = { 35, 30, 260, 50 };

            //Desenho do placar
            DrawRectangleRounded((Rectangle){hudPlacar.x + 4, hudPlacar.y + 4, hudPlacar.width, hudPlacar.height}, 0.2f, 4, sombraUI); 
            DrawRectangleRounded(hudPlacar, 0.2f, 4, azulBrasil);
            DrawRectangleRoundedLinesEx(hudPlacar, 0.2f, 4, 2.0f, amareloBrasil);

            //Desenho/texto da rodada
            const char* numPergText = TextFormat("RODADA: %d / %d", pergunta_atual + 1, num_perguntas);
            DrawTextEx(fonteCopa, numPergText, (Vector2){ 55, 48 }, 12, 1, amareloBrasil); 

            //Retângulo da pergunta
            Rectangle cardPergunta = { 100, 110, 800, 130 };

            //Desenho da pergunta
            DrawRectangleRounded((Rectangle){cardPergunta.x + 8, cardPergunta.y + 8, cardPergunta.width, cardPergunta.height}, 0.08f, 6, sombraUI); 
            DrawRectangleRounded(cardPergunta, 0.08f, 6, azulBrasil);
            DrawRectangleRoundedLinesEx(cardPergunta, 0.08f, 6, 2.5f, amareloBrasil);
            
            //Aplicação em váriavel do sorteio de perguntas
            const char* textoPergunta = banco[sorteadas[pergunta_atual]].pergunta;

            //Formatação da pergunta
            int widthPergunta = MeasureTextEx(fonteCopa, textoPergunta, 13, 1).x;
            Vector2 posPergunta = { 500 - (widthPergunta / 2), 165 };
            if(widthPergunta > 760) posPergunta.x = 120; 
            
            //Texto da pergunta
            DrawTextEx(fonteCopa, textoPergunta, posPergunta, 13, 1, WHITE); 

            //Verificação do mouse nos botões
            for(int i = 0; i < 4; i++){
                bool mouseEmCima = CheckCollisionPointRec(mousePoint, btnOpcoes[i]);
                int offsetAnimacao = 0;
                Color corFundoBtn = amareloBrasil;
                Color corBordaBtn = azulBrasil;
                Color corTextoBtn = azulBrasil;

                if(mouseEmCima){
                    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) offsetAnimacao = 4; 
                    else{
                        offsetAnimacao = -4; 
                        corFundoBtn = azulBrasil;
                        corBordaBtn = amareloBrasil;
                        corTextoBtn = WHITE;
                        DrawRectangleRounded((Rectangle){ btnOpcoes[i].x + 6, btnOpcoes[i].y + 6, btnOpcoes[i].width, btnOpcoes[i].height }, 0.15f, 4, sombraUI);
                    }
                }else{
                    DrawRectangleRounded((Rectangle){ btnOpcoes[i].x + 3, btnOpcoes[i].y + 3, btnOpcoes[i].width, btnOpcoes[i].height }, 0.15f, 4, sombraUI);
                }
                
                //Retângulo dos botões
                Rectangle btnAnimado = { btnOpcoes[i].x, btnOpcoes[i].y + offsetAnimacao, btnOpcoes[i].width, btnOpcoes[i].height };

                //Desenhos dos botões 
                DrawRectangleRounded(btnAnimado, 0.15f, 4, corFundoBtn);
                DrawRectangleRoundedLinesEx(btnAnimado, 0.15f, 4, 2.0f, corBordaBtn);
                
                //Texto da opção atual
                const char* textoOpcao = banco[sorteadas[pergunta_atual]].opcoes[i];
                int widthOpcao = MeasureTextEx(fonteCopa, textoOpcao, 12, 1).x;

                //Desenho do texto
                DrawTextEx(fonteCopa, textoOpcao, (Vector2){ btnAnimado.x + (btnAnimado.width / 2) - (widthOpcao / 2), btnAnimado.y + 22 }, 12, 1, corTextoBtn);
            }
        }else if(estado == TELA_FEEDBACK){
            
            //Váriavel de acerto de pergunta
            bool acertou = (opcao_selecionada == banco[sorteadas[pergunta_atual]].correta);

            //Váriavel de cor do acerto da pergunta
            Color corDestaque = acertou ? GREEN : RED; 
            
            //Retângulo para resultados
            Rectangle cardFeedback = { 150, 180, 700, 360 };

            //Desenhos para os resultados
            DrawRectangleRounded((Rectangle){cardFeedback.x + 10, cardFeedback.y + 10, cardFeedback.width, cardFeedback.height}, 0.05f, 6, sombraUI); 
            DrawRectangleRounded(cardFeedback, 0.05f, 6, azulBrasil);
            DrawRectangleRoundedLinesEx(cardFeedback, 0.05f, 6, 3.0f, corDestaque); 

            //Mensagem de acerto
            const char* msg = acertou ? "CORRETO! VITORIA!" : "RESPOSTA INCORRETA";
            float textSize = 22 + sin(tempo * 6.0f) * 2; 
            int widthMsg = MeasureTextEx(fonteCopa, msg, textSize, 2).x;
            
            //Texto de acerto
            DrawTextEx(fonteCopa, msg, (Vector2){ 500 - (widthMsg / 2) + 3, 250 + 3 }, textSize, 2, Fade(BLACK, 0.6f)); 
            DrawTextEx(fonteCopa, msg, (Vector2){ 500 - (widthMsg / 2), 250 }, textSize, 2, corDestaque); 
            
            //Condição para acerto
            if(!acertou){
                int certa = banco[sorteadas[pergunta_atual]].correta;
                const char* textoCerta = TextFormat("RESPOSTA CERTA: %s", banco[sorteadas[pergunta_atual]].opcoes[certa]);
                int widthCerta = MeasureTextEx(fonteCopa, textoCerta, 12, 1).x;
                DrawTextEx(fonteCopa, textoCerta, (Vector2){ 500 - (widthCerta / 2), 360 }, 12, 1, WHITE);
            }else{
                const char* textoCerta = "+1 PONTO COMPUTADO";
                int widthCerta = MeasureTextEx(fonteCopa, textoCerta, 12, 1).x;
                DrawTextEx(fonteCopa, textoCerta, (Vector2){ 500 - (widthCerta / 2), 360 }, 12, 1, Fade(WHITE, 0.7f));
            }

            //Váriavel de checagem do mouse
            bool mouseEmCima = CheckCollisionPointRec(mousePoint, btnContinuar);
            int offsetAnimacao = 0; Color corFundoBtn = amareloBrasil;

            //Condição de mouse para animação dos botões
            if(mouseEmCima){
                if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) offsetAnimacao = 4;
                else{ offsetAnimacao = -4; corFundoBtn = WHITE; DrawRectangleRounded((Rectangle){ btnContinuar.x + 6, btnContinuar.y + 6, btnContinuar.width, btnContinuar.height }, 0.15f, 4, sombraUI); }
            }else{ DrawRectangleRounded((Rectangle){ btnContinuar.x + 3, btnContinuar.y + 3, btnContinuar.width, btnContinuar.height }, 0.15f, 4, sombraUI); }

            //Retângulo e Desenhos da animação do botão
            Rectangle btnAnimado = { btnContinuar.x, btnContinuar.y + offsetAnimacao, btnContinuar.width, btnContinuar.height };
            DrawRectangleRounded(btnAnimado, 0.15f, 4, corFundoBtn);
            DrawRectangleRoundedLinesEx(btnAnimado, 0.15f, 4, 2.0f, azulBrasil);
            
            //Texto para continuar no botão
            int widthCont = MeasureTextEx(fonteCopa, "CONTINUAR >>", 12, 1).x;
            DrawTextEx(fonteCopa, "CONTINUAR >>", (Vector2){ btnAnimado.x + (btnAnimado.width / 2) - (widthCont / 2), btnAnimado.y + 22 }, 12, 1, azulBrasil);
        
        }else if(estado == TELA_FIM){

            //Retângulo para FIM
            Rectangle cardFim = { 150, 120, 700, 440 };
            DrawRectangleRounded((Rectangle){cardFim.x + 10, cardFim.y + 10, cardFim.width, cardFim.height}, 0.05f, 6, sombraUI);
            DrawRectangleRounded(cardFim, 0.05f, 6, azulBrasil);
            DrawRectangleRoundedLinesEx(cardFim, 0.05f, 6, 3.0f, amareloBrasil);

            //Mensagem e Desenhos para fim
            const char* msgFim = "--- FIM DO QUIZ ---";
            int widthFim = MeasureTextEx(fonteCopa, msgFim, 24, 2).x;
            DrawTextEx(fonteCopa, msgFim, (Vector2){ 500 - (widthFim / 2) + 3, 160 + 3 }, 24, 2, BLACK);
            DrawTextEx(fonteCopa, msgFim, (Vector2){ 500 - (widthFim / 2), 160 }, 24, 2, WHITE);
            
            //Mensagem de pontuação
            const char* msgScore = TextFormat("VOCE ACERTOU %d DE %d!", score, num_perguntas);
            int widthScore = MeasureTextEx(fonteCopa, msgScore, 18, 1).x;
            DrawTextEx(fonteCopa, msgScore, (Vector2){ 500 - (widthScore / 2), 240 }, 18, 1, amareloBrasil);

            //Texto quantidade de pacotes ganhos
            Color corRecompensa = (pacotes_ganhos_rodada > 0) ? GREEN : LIGHTGRAY;
            const char* msgRodada = TextFormat("PACOTES GANHOS NESTA RODADA: %d", pacotes_ganhos_rodada);
            int widthRodada = MeasureTextEx(fonteCopa, msgRodada, 14, 1).x;
            DrawTextEx(fonteCopa, msgRodada, (Vector2){ 500 - (widthRodada / 2), 310 }, 14, 1, corRecompensa);

            //Texto total de pacotes geral
            int totalGuardado = pacotes_fechados;
            const char* msgAcumulado = TextFormat("SEU SALDO TOTAL GUARDADO: %d PACOTES", totalGuardado);
            int widthAcumulado = MeasureTextEx(fonteCopa, msgAcumulado, 12, 1).x;
            DrawTextEx(fonteCopa, msgAcumulado, (Vector2){ 500 - (widthAcumulado / 2), 360 }, 12, 1, Fade(WHITE, 0.8f));

            //Váriavel de checagem com o mouse no botão tentar, cor do fundo
            bool hoverTentar = CheckCollisionPointRec(mousePoint, btnTentar);
            int animTentar = 0; Color corFundoTentar = amareloBrasil;
            if(hoverTentar){
                if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) animTentar = 4;
                else{ animTentar = -4; corFundoTentar = WHITE; DrawRectangleRounded((Rectangle){ btnTentar.x + 6, btnTentar.y + 6, btnTentar.width, btnTentar.height }, 0.15f, 4, sombraUI); }
            }else{ DrawRectangleRounded((Rectangle){ btnTentar.x + 3, btnTentar.y + 3, btnTentar.width, btnTentar.height }, 0.15f, 4, sombraUI); }

            //Retângulo e desenhos do botão tentar novamente
            Rectangle rectTentar = { btnTentar.x, btnTentar.y + animTentar, btnTentar.width, btnTentar.height };
            DrawRectangleRounded(rectTentar, 0.15f, 4, corFundoTentar);
            DrawRectangleRoundedLinesEx(rectTentar, 0.15f, 4, 2.0f, azulBrasil);
            int wt = MeasureTextEx(fonteCopa, "TENTAR DE NOVO", 12, 1).x;
            DrawTextEx(fonteCopa, "TENTAR DE NOVO", (Vector2){ rectTentar.x + (rectTentar.width / 2) - (wt / 2), rectTentar.y + 22 }, 12, 1, azulBrasil);

            //Váriavel de checagem com o mouse no botão menu, cor do fundo
            bool hoverMenu = CheckCollisionPointRec(mousePoint, btnMenu);
            int animMenu = 0; Color corFundoMenu = amareloBrasil;
            if(hoverMenu) {
                if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)) animMenu = 4;
                else{ animMenu = -4; corFundoMenu = WHITE; DrawRectangleRounded((Rectangle){ btnMenu.x + 6, btnMenu.y + 6, btnMenu.width, btnMenu.height }, 0.15f, 4, sombraUI); }
            }else{ DrawRectangleRounded((Rectangle){ btnMenu.x + 3, btnMenu.y + 3, btnMenu.width, btnMenu.height }, 0.15f, 4, sombraUI); }

            //Retângulo e desenhos do botão menu
            Rectangle rectMenu = { btnMenu.x, btnMenu.y + animMenu, btnMenu.width, btnMenu.height };
            DrawRectangleRounded(rectMenu, 0.15f, 4, corFundoMenu);
            DrawRectangleRoundedLinesEx(rectMenu, 0.15f, 4, 2.0f, azulBrasil);
            int wm = MeasureTextEx(fonteCopa, "MENU PRINCIPAL", 12, 1).x;
            DrawTextEx(fonteCopa, "MENU PRINCIPAL", (Vector2){ rectMenu.x + (rectMenu.width / 2) - (wm / 2), rectMenu.y + 22 }, 12, 1, azulBrasil);
        }

        //Textura do cursor do mouse com a imagem de bola
        DrawTexture(cursorBola, (int)mousePoint.x - cursorBola.width/2, (int)mousePoint.y - cursorBola.height/2, WHITE);
        EndDrawing();
    }

    //Limpeza de variáveis
    UnloadMusicStream(musicaFundo);
    UnloadSound(somAcerto);
    UnloadSound(somErro);
    if (audioIniciadoAqui) CloseAudioDevice();
    UnloadTexture(cursorBola);
    UnloadFont(fonteCopa);
    UnloadImage(imagemBola); 

    // Fecha apenas a janela do minigame antes de voltar para o menu
    if(IsWindowReady()){
        CloseWindow();
    }

    // Se a saída foi pelo X, encerra o projeto por completo
    if(fecharProjeto){
        exit(0);
    }
}