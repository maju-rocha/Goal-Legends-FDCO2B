#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include "biblioteca.h"

void menuPrincipal(Figurinha *figurinhas, Figurinha *album, Figurinha *mochila, int total, int *total_album, int *total_mochila){
    
    //==========================================//
    //============ Interface Gráfica ===========//
    //==========================================//
    InitWindow(1000, 800, "Album de Figurinhas - Copa do Mundo");
    Font fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf");
    
    // Configuração do Cursor
    HideCursor();
    Image imagemBola = LoadImage("imagens/bola_cursor.png");
    ImageResize(&imagemBola, 40, 40);
    Texture2D cursorBola = LoadTextureFromImage(imagemBola);
    UnloadImage(imagemBola);

    SetTargetFPS(60);

    //EstadoAtual
    EstadoMenu telaAtual = MENU_PRINCIPAL;

    //Textos do Menu Principal
    const char *textosPrincipal[] = {
        "1. Abrir Pacote",
        "2. Ver Inventario",
        "3. Excluir do Inventario",
        "4. Pesquisar Figurinha",
        "5. Alterar Figurinha",
        "6. Trocar Figurinhas",
        "7. Area de Minigames"
    };

    Rectangle botoesPrincipal[7];

    for(int i = 0; i < 7; i++){
        botoesPrincipal[i] = (Rectangle){250, 220 + (i * 70), 500, 50};
    }

    //Textos dos Submenus
    const char *textosInventario[] = {"1. Ver Album", "2. Ver Mochila", "3. Voltar ao Menu"};
    const char *textosExcluir[] = {"1. Excluir Album", "2. Excluir Mochila", "3. Voltar ao Menu"};
    const char *textosMinigames[] = {"1. Quiz da Copa", "2. Jogo do Goleiro", "3. Penalti", "4. Voltar ao Menu"};

    //Layout padrão para todos os submenus
    Rectangle botoesSubmenu[4];

    for(int i = 0; i < 4; i++){
        botoesSubmenu[i] = (Rectangle){250, 300 + (i * 80), 500, 55};
    }

    Color verdeCampo = (Color){34, 139, 34, 255};
    Color amareloBrasil = (Color){255, 215, 0, 255};
    Color azulBrasil = (Color){0, 39, 118, 255};

    while(!WindowShouldClose()){
        Vector2 mousePoint = GetMousePosition();
        int acaoTerminal = 0; // Controla se alguma ação ainda precisa ir para o prompt antigo

        //====================================================//
        //================== Cliques/Tela ====================//
        //====================================================//
        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
            if(telaAtual == MENU_PRINCIPAL){
                for(int i = 0; i < 7; i++){
                    if(CheckCollisionPointRec(mousePoint, botoesPrincipal[i])){
                        if(i == 0){
                            telaAtual = ESTADO_ABRIR_PACOTE; // Ativa a tela gráfica nova!
                        }else if(i == 1){
                            telaAtual = MENU_INVENTARIO;
                        }else if(i == 2){
                            telaAtual = MENU_EXCLUIR;
                        }else if(i == 3){
                            acaoTerminal = 6;  // Pesquisar
                        }else if(i == 4){
                            acaoTerminal = 7;  // Alterar
                        }else if(i == 5){
                            acaoTerminal = 11; // Trocar
                        }else if(i == 6){
                            telaAtual = MENU_MINIGAMES;
                        }
                    }
                }
            }else if(telaAtual == MENU_INVENTARIO){
                for(int i = 0; i < 3; i++){
                    if(CheckCollisionPointRec(mousePoint, botoesSubmenu[i])){
                        if(i == 0){
                            acaoTerminal = 2; // Ver Album
                        }else if(i == 1){
                            acaoTerminal = 3; // Ver Mochila
                        }else if(i == 2){
                            telaAtual = MENU_PRINCIPAL;
                        }
                    }
                }
            }else if(telaAtual == MENU_EXCLUIR){
                for(int i = 0; i < 3; i++){
                    if(CheckCollisionPointRec(mousePoint, botoesSubmenu[i])){
                        if(i == 0){
                            acaoTerminal = 4; // Excluir Album
                        }else if(i == 1){
                            acaoTerminal = 5; // Excluir Mochila
                        }else if(i == 2){
                            telaAtual = MENU_PRINCIPAL;
                        }
                    }
                }
            }else if(telaAtual == MENU_MINIGAMES){
                for(int i = 0; i < 4; i++){
                    if(CheckCollisionPointRec(mousePoint, botoesSubmenu[i])){
                        if(i == 0){
                            acaoTerminal = 8;  // Quiz
                        }else if(i == 1){
                            acaoTerminal = 9;  // Goleiro
                        }else if(i == 2){
                            acaoTerminal = 10; // Penalti
                        }else if(i == 3){
                            telaAtual = MENU_PRINCIPAL;
                        }
                    }
                }
            }
        }

        //==========================================//
        //================= Desenho ================//
        //==========================================//
        BeginDrawing();
        ClearBackground(verdeCampo);

        // SE TIVER NA TELA DE ABRIR PACOTE: Desenha o fluxo interativo gráfico
        if(telaAtual == ESTADO_ABRIR_PACOTE){
            abrirPacote(figurinhas, mochila, album, total, total_mochila, total_album, fonteCopa, azulBrasil, amareloBrasil, &telaAtual);
        }
        // CASO CONTRÁRIO: Desenha as telas normais de menu
        else if(telaAtual == MENU_PRINCIPAL){
            const char *title = "ALBUM DE FIGURINHAS DA COPA";
            int titleWidth = MeasureTextEx(fonteCopa, title, 28, 2).x;

            DrawTextEx(fonteCopa, title, (Vector2){500 - (titleWidth / 2) + 3, 60 + 3}, 28, 2, BLACK);
            DrawTextEx(fonteCopa, title, (Vector2){500 - (titleWidth / 2), 60}, 28, 2, WHITE);

            const char *subtitle = "Selecione uma opcao abaixo:";
            int subtitleWidth = MeasureTextEx(fonteCopa, subtitle, 16, 2).x;

            DrawTextEx(fonteCopa, subtitle, (Vector2){500 - (subtitleWidth / 2) + 2, 120 + 2}, 16, 2, BLACK);
            DrawTextEx(fonteCopa, subtitle, (Vector2){500 - (subtitleWidth / 2), 120}, 16, 2, amareloBrasil);

            for(int i = 0; i < 7; i++){
                bool mouseEmCima = CheckCollisionPointRec(mousePoint, botoesPrincipal[i]);
                int offsetAnimacao = 0;

                if(mouseEmCima){
                    if(IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                        offsetAnimacao = 2;
                    }else{
                        offsetAnimacao = -6;
                        DrawRectangle(botoesPrincipal[i].x + 4, botoesPrincipal[i].y + 4, botoesPrincipal[i].width, botoesPrincipal[i].height, Fade(BLACK, 0.4f));
                    }
                }

                Rectangle btnAnimado = {
                    botoesPrincipal[i].x,
                    botoesPrincipal[i].y + offsetAnimacao,
                    botoesPrincipal[i].width,
                    botoesPrincipal[i].height
                };

                DrawRectangleRec(btnAnimado, mouseEmCima ? LIGHTGRAY : amareloBrasil);
                DrawRectangleLinesEx(btnAnimado, 2.0f, mouseEmCima ? azulBrasil : DARKGRAY);
                
                char textoBotao[100];

                if(i == 0){
                    sprintf(textoBotao, "1. Abrir Pacote (%d disp.)", pacotes_fechados);
                }else{
                    strcpy(textoBotao, textosPrincipal[i]);
                }

                int textWidth = MeasureTextEx(fonteCopa, textoBotao, 14, 2).x;

                DrawTextEx(fonteCopa, textoBotao, (Vector2){btnAnimado.x + (btnAnimado.width / 2) - (textWidth / 2), btnAnimado.y + 18}, 14, 2, azulBrasil);
            }
        }else{
            //Submenus (Inventário, Exclusão, Minigames)
            const char *titulo;
            const char *subtitulo;
            const char **textosBotoes;
            int numBotoes = 3;

            if(telaAtual == MENU_INVENTARIO){
                titulo = "MEU INVENTARIO";
                subtitulo = "Qual colecao voce quer visualizar?";
                textosBotoes = textosInventario;
            }else if(telaAtual == MENU_EXCLUIR){
                titulo = "EXCLUIR FIGURINHAS";
                subtitulo = "De onde voce quer remover figurinhas?";
                textosBotoes = textosExcluir;
            }else{
                titulo = "AREA DE MINIGAMES";
                subtitulo = "Escolha seu desafio:";
                textosBotoes = textosMinigames;
                numBotoes = 4;
            }

            int titleWidth = MeasureTextEx(fonteCopa, titulo, 28, 2).x;

            DrawTextEx(fonteCopa, titulo, (Vector2){500 - (titleWidth / 2) + 3, 100 + 3}, 28, 2, BLACK);
            DrawTextEx(fonteCopa, titulo, (Vector2){500 - (titleWidth / 2), 100}, 28, 2, WHITE);
            
            int subtitleWidth = MeasureTextEx(fonteCopa, subtitulo, 16, 2).x;

            DrawTextEx(fonteCopa, subtitulo, (Vector2){500 - (subtitleWidth / 2) + 2, 180 + 2}, 16, 2, BLACK);
            DrawTextEx(fonteCopa, subtitulo, (Vector2){500 - (subtitleWidth / 2), 180}, 16, 2, amareloBrasil);

            for(int i = 0; i < numBotoes; i++){
                bool mouseEmCima = CheckCollisionPointRec(mousePoint, botoesSubmenu[i]);
                int offsetAnimacao = mouseEmCima ? (IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? 2 : -6) : 0;

                if(mouseEmCima && !IsMouseButtonDown(MOUSE_LEFT_BUTTON)){
                    DrawRectangle(botoesSubmenu[i].x + 4, botoesSubmenu[i].y + 4, botoesSubmenu[i].width, botoesSubmenu[i].height, Fade(BLACK, 0.4f));
                }

                Rectangle btnAnimado = {
                    botoesSubmenu[i].x,
                    botoesSubmenu[i].y + offsetAnimacao,
                    botoesSubmenu[i].width,
                    botoesSubmenu[i].height
                };

                DrawRectangleRec(btnAnimado, mouseEmCima ? LIGHTGRAY : amareloBrasil);
                DrawRectangleLinesEx(btnAnimado, 2, mouseEmCima ? azulBrasil : DARKGRAY);
                
                int textWidth = MeasureTextEx(fonteCopa, textosBotoes[i], 16, 2).x;

                DrawTextEx(fonteCopa, textosBotoes[i], (Vector2){btnAnimado.x + (btnAnimado.width / 2) - (textWidth / 2), btnAnimado.y + 20}, 16, 2, azulBrasil);
            }
        }

        //Texto Fechamento
        const char *footer = "Feche a janela (X) ou aperte ESC para sair";
        int footerWidth = MeasureTextEx(fonteCopa, footer, 12, 2).x;

        DrawTextEx(fonteCopa, footer, (Vector2){500 - (footerWidth / 2) + 2, 740 + 2}, 12, 2, BLACK);
        DrawTextEx(fonteCopa, footer, (Vector2){500 - (footerWidth / 2), 740}, 12, 2, LIGHTGRAY);

        //Cursor
        DrawTexture(cursorBola, (int)mousePoint.x - cursorBola.width / 2, (int)mousePoint.y - cursorBola.height / 2, WHITE);

        EndDrawing();

        //==========================================//
        //============= Reaparecer Menu ============//
        //==========================================//
        if(acaoTerminal != 0){
            // Fecha temporariamente a tela se a ação requisitada for de terminal antigo
            UnloadTexture(cursorBola);
            UnloadFont(fonteCopa);
            CloseWindow();
            
            if(acaoTerminal == 2){
                printf("\n=========================================\nTotal album: %d\n", *total_album);
                listarFigurinhasAlbum(album, *total_album);
                albumGrafico(figurinhas, total, album, *total_album);
            }else if(acaoTerminal == 3){
                printf("\n=========================================\nTotal mochila: %d\n", *total_mochila);
                listarFigurinhasMochila(mochila, *total_mochila);
            }else if(acaoTerminal == 4){
                excluirAlbum(figurinhas, album, total_album);
            }else if(acaoTerminal == 5){
                excluirMochila(figurinhas, mochila, total_mochila);
            }else if(acaoTerminal == 6){
                pesquisarFigurinha(figurinhas, total);
            }else if(acaoTerminal == 7){
                int opcao_alterar;

                do{
                    printf("\n--- MENU DE ALTERACAO ---\n1 - Alterar figurinha do album\n2 - Resetar a lista de figurinhas\n3 - Voltar ao menu principal\nEscolha: ");

                    if(scanf("%d", &opcao_alterar) != 1){
                        opcao_alterar = 0;
                    }

                    while(getchar() != '\n');

                    if(opcao_alterar == 1){
                        alterarFigurinha(figurinhas, total);
                    }else if(opcao_alterar == 2){
                        resetarLista(figurinhas, total);
                    }
                }while(opcao_alterar != 3);
            }else if(acaoTerminal == 11){
                trocarFigurinha(figurinhas, mochila, album, total_mochila, total_album);
            }else if(acaoTerminal == 8){
                jogarQuiz(figurinhas, mochila, album, total, total_mochila, total_album);
            }else if(acaoTerminal == 9){
                jogarGoleiro();
            }else if(acaoTerminal == 10){
                jogarPenalti(figurinhas, mochila, album, total, total_mochila, total_album, &pacotes_fechados);
            }

            //Menu de travamento pós terminal
            printf("\n=========================================\nPressione ENTER para voltar ao menu grafico...");

            int c;

            while((c = getchar()) != '\n' && c != EOF);
            getchar();

            telaAtual = MENU_PRINCIPAL;
            
            //Reinicializa os componentes gráficos da tela
            InitWindow(1000, 800, "Album de Figurinhas - Copa do Mundo");
            fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf");
            HideCursor();

            Image imgBolaTemp = LoadImage("imagens/bola_cursor.png");
            ImageResize(&imgBolaTemp, 40, 40);
            cursorBola = LoadTextureFromImage(imgBolaTemp);
            UnloadImage(imgBolaTemp);

            SetTargetFPS(60);
        }
    }

    //Fechamento e liberação de memória
    UnloadTexture(cursorBola);
    UnloadFont(fonteCopa);
    CloseWindow();

    free(figurinhas);
    free(album);
    free(mochila);
}