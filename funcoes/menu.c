#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <raylib.h>
#include "../headers/biblioteca.h"

void menuPrincipal(void){

        char linha[300];
        int total = 0;
        int total_album = 0;
        int total_mochila = 0;
        
        // ==========================================
        // VARIÁVEL DE PACOTES ACUMULADOS
        // ==========================================
        Figurinha *figurinhas = malloc(981 * sizeof(Figurinha));
        Figurinha *album = malloc(981 * sizeof(Figurinha));
        Figurinha *mochila = malloc(981 * sizeof(Figurinha));
        
        if (figurinhas == NULL || album == NULL || mochila == NULL){
            printf("Erro de alocacao.\n"); 
            return;
        }

        FILE *arquivo = fopen("extras/figurinhas2026.csv","rb");
        if (arquivo == NULL){
            printf("Erro ao abrir o arquivo figurinhas2026.csv.\n"); 
            return; 
        }

        FILE *arquivo_album = fopen("extras/album.csv", "r");
        if(arquivo_album != NULL){
            while(fscanf(arquivo_album," %9[^,],%49[^,],%49[^,],%49[^,],%49[^\n]", album[total_album].codigo, album[total_album].titulo, album[total_album].secao, album[total_album].grupo,album[total_album].tipo) == 5){
                total_album++;
            }
            fclose(arquivo_album);
        }

        FILE *arquivo_mochila = fopen("extras/mochila.csv", "r");
        if(arquivo_mochila != NULL){
            while(fscanf(arquivo_mochila, " %9[^,],%49[^,],%49[^,],%49[^,],%49[^\n]", mochila[total_mochila].codigo, mochila[total_mochila].titulo, mochila[total_mochila].secao, mochila[total_mochila].grupo, mochila[total_mochila].tipo) == 5){
                total_mochila++;
            }
            fclose(arquivo_mochila);
        }

        fgets(linha, sizeof(linha), arquivo); 
        while (total < 981 && fscanf(arquivo, " %9[^,],%49[^,],%49[^,],%49[^,],%49[^\n]", figurinhas[total].codigo, figurinhas[total].titulo, figurinhas[total].secao, figurinhas[total].grupo, figurinhas[total].tipo) == 5){
            total++;
        }
        fclose(arquivo);
        
        srand(time(NULL));

        // ==========================================
        // CONFIGURAÇÃO DA INTERFACE GRÁFICA
        // ==========================================
        InitWindow(1000, 800, "Album de Figurinhas - Copa do Mundo");
        Font fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf");
        
        // Configuração do Cursor
        HideCursor(); 
        Image imagemBola = LoadImage("imagens/bola_cursor.png"); 
        ImageResize(&imagemBola, 40, 40); 
        Texture2D cursorBola = LoadTextureFromImage(imagemBola); 
        UnloadImage(imagemBola); 

        SetTargetFPS(60);

        EstadoMenu estadoAtual = MENU_PRINCIPAL;

        // Textos do Menu Principal
        const char *textosPrincipal[] = {
            "1. Abrir Pacote",
            "2. Ver Inventario",
            "3. Excluir do Inventario",
            "4. Pesquisar Figurinha",
            "5. Alterar Figurinha",
            "6. Area de Minigames"
        };
        Rectangle botoesPrincipal[6];
        for (int i = 0; i < 6; i++) {
            botoesPrincipal[i] = (Rectangle){ 250, 220 + (i * 70), 500, 50 };
        }

        // Textos dos Submenus
        const char *textosInventario[] = { "1. Ver Album", "2. Ver Mochila", "3. Voltar ao Menu" };
        const char *textosExcluir[] = { "1. Excluir Album", "2. Excluir Mochila", "3. Voltar ao Menu" };
        const char *textosMinigames[] = { "1. Quiz da Copa", "2. Jogo do Goleiro", "3. Penalti", "4. Voltar ao Menu" };

        // Layout padrão para todos os submenus
        Rectangle botoesSubmenu[4];
        for (int i = 0; i < 4; i++) {
            botoesSubmenu[i] = (Rectangle){ 250, 300 + (i * 80), 500, 55 };
        }

        Color verdeCampo = (Color){ 34, 139, 34, 255 };
        Color amareloBrasil = (Color){ 255, 215, 0, 255 };
        Color azulBrasil = (Color){ 0, 39, 118, 255 };

        while (!WindowShouldClose()) {
            Vector2 mousePoint = GetMousePosition();
            int acaoEscolhida = 0;

            // ==========================================
            // Lógica de Cliques
            // ==========================================
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                if (estadoAtual == MENU_PRINCIPAL) {
                    for (int i = 0; i < 6; i++) {
                        if (CheckCollisionPointRec(mousePoint, botoesPrincipal[i])) {
                            if (i == 0) acaoEscolhida = 1;
                            else if (i == 1) estadoAtual = MENU_INVENTARIO;
                            else if (i == 2) estadoAtual = MENU_EXCLUIR;
                            else if (i == 3) acaoEscolhida = 6;
                            else if (i == 4) acaoEscolhida = 7;
                            else if (i == 5) estadoAtual = MENU_MINIGAMES;
                        }
                    }
                } 
                else if (estadoAtual == MENU_INVENTARIO) {
                    for (int i = 0; i < 3; i++) {
                        if (CheckCollisionPointRec(mousePoint, botoesSubmenu[i])) {
                            if (i == 0) acaoEscolhida = 2;
                            else if (i == 1) acaoEscolhida = 3;
                            else if (i == 2) estadoAtual = MENU_PRINCIPAL;
                        }
                    }
                }
                else if (estadoAtual == MENU_EXCLUIR) {
                    for (int i = 0; i < 3; i++) {
                        if (CheckCollisionPointRec(mousePoint, botoesSubmenu[i])) {
                            if (i == 0) acaoEscolhida = 4;
                            else if (i == 1) acaoEscolhida = 5;
                            else if (i == 2) estadoAtual = MENU_PRINCIPAL;
                        }
                    }
                }
                else if (estadoAtual == MENU_MINIGAMES) {
                    for (int i = 0; i < 4; i++) {
                        if (CheckCollisionPointRec(mousePoint, botoesSubmenu[i])) {
                            if (i == 0) acaoEscolhida = 8;
                            else if (i == 1) acaoEscolhida = 9;
                            else if (i == 2) acaoEscolhida = 10;
                            else if (i == 3) estadoAtual = MENU_PRINCIPAL;
                        }
                    }
                }
            }

            // ==========================================
            // Lógica de Desenho
            // ==========================================
            BeginDrawing();
            ClearBackground(verdeCampo);

            if (estadoAtual == MENU_PRINCIPAL) {
                const char* title = "ALBUM DE FIGURINHAS DA COPA";
                int titleWidth = MeasureTextEx(fonteCopa, title, 28, 2).x;
                DrawTextEx(fonteCopa, title, (Vector2){ 500 - (titleWidth / 2) + 3, 60 + 3 }, 28, 2, BLACK); // Sombra
                DrawTextEx(fonteCopa, title, (Vector2){ 500 - (titleWidth / 2), 60 }, 28, 2, WHITE);

                const char* subtitle = "Selecione uma opcao abaixo:";
                int subtitleWidth = MeasureTextEx(fonteCopa, subtitle, 16, 2).x;
                DrawTextEx(fonteCopa, subtitle, (Vector2){ 500 - (subtitleWidth / 2) + 2, 120 + 2 }, 16, 2, BLACK); // Sombra
                DrawTextEx(fonteCopa, subtitle, (Vector2){ 500 - (subtitleWidth / 2), 120 }, 16, 2, amareloBrasil);

                for (int i = 0; i < 6; i++) {
                    bool mouseEmCima = CheckCollisionPointRec(mousePoint, botoesPrincipal[i]);
                    int offsetAnimacao = 0;

                    if (mouseEmCima) {
                        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                            offsetAnimacao = 2; // Afunda
                        } else {
                            offsetAnimacao = -6; // Flutua
                            DrawRectangle(botoesPrincipal[i].x + 4, botoesPrincipal[i].y + 4, botoesPrincipal[i].width, botoesPrincipal[i].height, Fade(BLACK, 0.4f)); // Sombra do botão
                        }
                    }

                    Rectangle btnAnimado = { botoesPrincipal[i].x, botoesPrincipal[i].y + offsetAnimacao, botoesPrincipal[i].width, botoesPrincipal[i].height };

                    DrawRectangleRec(btnAnimado, mouseEmCima ? LIGHTGRAY : amareloBrasil);
                    DrawRectangleLinesEx(btnAnimado, 2, mouseEmCima ? azulBrasil : DARKGRAY);
                    
                    // Mostra a quantidade de pacotes disponíveis dinamicamente no botão
                    char textoBotao[100];
                    if (i == 0) {
                        sprintf(textoBotao, "1. Abrir Pacote (%d disp.)", pacotes_fechados);
                    } else {
                        strcpy(textoBotao, textosPrincipal[i]);
                    }

                    int textWidth = MeasureTextEx(fonteCopa, textoBotao, 14, 2).x;
                    DrawTextEx(fonteCopa, textoBotao, (Vector2){ btnAnimado.x + (btnAnimado.width / 2) - (textWidth / 2), btnAnimado.y + 18 }, 14, 2, azulBrasil);
                }
            } 
            else { 
                const char *titulo;
                const char *subtitulo;
                const char **textosBotoes;
                int numBotoes = 3;

                if (estadoAtual == MENU_INVENTARIO) {
                    titulo = "MEU INVENTARIO";
                    subtitulo = "Qual colecao voce quer visualizar?";
                    textosBotoes = textosInventario;
                } else if (estadoAtual == MENU_EXCLUIR) {
                    titulo = "EXCLUIR FIGURINHAS";
                    subtitulo = "De onde voce quer remover figurinhas?";
                    textosBotoes = textosExcluir;
                } else { 
                    titulo = "AREA DE MINIGAMES";
                    subtitulo = "Escolha seu desafio:";
                    textosBotoes = textosMinigames;
                    numBotoes = 4;
                }

                int titleWidth = MeasureTextEx(fonteCopa, titulo, 28, 2).x;
                DrawTextEx(fonteCopa, titulo, (Vector2){ 500 - (titleWidth / 2) + 3, 100 + 3 }, 28, 2, BLACK); // Sombra
                DrawTextEx(fonteCopa, titulo, (Vector2){ 500 - (titleWidth / 2), 100 }, 28, 2, WHITE);
                
                int subtitleWidth = MeasureTextEx(fonteCopa, subtitulo, 16, 2).x;
                DrawTextEx(fonteCopa, subtitulo, (Vector2){ 500 - (subtitleWidth / 2) + 2, 180 + 2 }, 16, 2, BLACK); // Sombra
                DrawTextEx(fonteCopa, subtitulo, (Vector2){ 500 - (subtitleWidth / 2), 180 }, 16, 2, amareloBrasil);

                for (int i = 0; i < numBotoes; i++) {
                    bool mouseEmCima = CheckCollisionPointRec(mousePoint, botoesSubmenu[i]);
                    int offsetAnimacao = 0;

                    if (mouseEmCima) {
                        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
                            offsetAnimacao = 2; // Afunda
                        } else {
                            offsetAnimacao = -6; // Flutua
                            DrawRectangle(botoesSubmenu[i].x + 4, botoesSubmenu[i].y + 4, botoesSubmenu[i].width, botoesSubmenu[i].height, Fade(BLACK, 0.4f)); // Sombra do botão
                        }
                    }

                    Rectangle btnAnimado = { botoesSubmenu[i].x, botoesSubmenu[i].y + offsetAnimacao, botoesSubmenu[i].width, botoesSubmenu[i].height };

                    DrawRectangleRec(btnAnimado, mouseEmCima ? LIGHTGRAY : amareloBrasil);
                    DrawRectangleLinesEx(btnAnimado, 2, mouseEmCima ? azulBrasil : DARKGRAY);
                    
                    int textWidth = MeasureTextEx(fonteCopa, textosBotoes[i], 16, 2).x;
                    DrawTextEx(fonteCopa, textosBotoes[i], (Vector2){ btnAnimado.x + (btnAnimado.width / 2) - (textWidth / 2), btnAnimado.y + 20 }, 16, 2, azulBrasil);
                }
            }

            const char* footer = "Feche a janela (X) ou aperte ESC para sair";
            int footerWidth = MeasureTextEx(fonteCopa, footer, 12, 2).x;
            DrawTextEx(fonteCopa, footer, (Vector2){ 500 - (footerWidth / 2) + 2, 740 + 2 }, 12, 2, BLACK); // Sombra do rodapé
            DrawTextEx(fonteCopa, footer, (Vector2){ 500 - (footerWidth / 2), 740 }, 12, 2, LIGHTGRAY);

            // ==========================================
            // DESENHO DO CURSOR PERSONALIZADO
            // ==========================================
            DrawTexture(cursorBola, (int)mousePoint.x - cursorBola.width/2, (int)mousePoint.y - cursorBola.height/2, WHITE);

            EndDrawing();

            // ==========================================
            // Execução da Ação Escolhida
            // ==========================================
            if (acaoEscolhida != 0) {
                // Descarrega tudo antes de fechar a janela para o terminal
                UnloadTexture(cursorBola);
                UnloadFont(fonteCopa); 
                CloseWindow(); 
                
                // LÓGICA DE ABRIR PACOTE
                if (acaoEscolhida == 1) {
                    printf("\n=========================================\n");
                    abrirPacote(figurinhas, mochila, album, total, &total_mochila, &total_album, &pacotes_fechados);
                }
                else if (acaoEscolhida == 2) {
                    printf("\n=========================================\n");
                    printf("Total album: %d\n", total_album);
                    listarFigurinhasAlbum(album, total_album);
                } 
                else if (acaoEscolhida == 3) {
                    printf("\n=========================================\n");
                    printf("Total mochila: %d\n", total_mochila);
                    listarFigurinhasMochila(mochila, total_mochila);
                } 
                else if (acaoEscolhida == 4) {
                    printf("\n=========================================\n");
                    excluirAlbum(figurinhas, album, &total_album);
                }
                else if (acaoEscolhida == 5) {
                    printf("\n=========================================\n");
                    excluirMochila(figurinhas, mochila, &total_mochila);
                }
                else if (acaoEscolhida == 6) {
                    printf("\n=========================================\n");
                    pesquisarFigurinha(figurinhas, total);
                }
                else if (acaoEscolhida == 7) {
                    printf("\n=========================================\n");
                    int opcao_alterar;
                    do {
                        printf("\n--- MENU DE ALTERACAO ---\n");
                        printf("1 - Alterar figurinha do album\n");
                        printf("2 - Resetar a lista de figurinhas\n");
                        printf("3 - Voltar ao menu principal\n");
                        printf("Escolha: ");

                        if (scanf("%d", &opcao_alterar) != 1) opcao_alterar = 0;
                        while(getchar() != '\n'); 

                        if (opcao_alterar == 1) alterarFigurinha(figurinhas, total);
                        else if (opcao_alterar == 2) resetarLista(figurinhas, total);
                        else if (opcao_alterar != 3) printf("Opcao invalida.\n");
                        
                    } while (opcao_alterar != 3);
                } 
                // CHAMADA DOS MINIGAMES
                else if (acaoEscolhida == 8) jogarQuiz(figurinhas, mochila, album, total, &total_mochila, &total_album);
                else if (acaoEscolhida == 9) jogarGoleiro();
                else if (acaoEscolhida == 10) jogarPenalti(); 

                // ==========================================
                // CONTROLE DE RETORNO DO TERMINAL
                // ==========================================
                if (acaoEscolhida >= 1 && acaoEscolhida <= 7) {
                    printf("\n=========================================\n");
                    printf("Pressione ENTER para voltar ao menu grafico...");
                    
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF);
                    getchar(); 
                }

                estadoAtual = MENU_PRINCIPAL;
                
                // Reabre a janela gráfica e recarrega os assets
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

        printf("Saindo do programa...\n");

        UnloadTexture(cursorBola);
        UnloadFont(fonteCopa); 

        free(figurinhas);
        free(album);
        free(mochila);
        
        return;
    }