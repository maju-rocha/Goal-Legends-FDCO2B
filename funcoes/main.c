#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h> 
#include <raylib.h>
#include "../headers/biblioteca.h" 

// Define os estados do menu
typedef enum { MENU_PRINCIPAL, MENU_INVENTARIO, MENU_EXCLUIR, MENU_MINIGAMES } EstadoMenu;

int main(){

    char linha[300];
    int total = 0;
    int total_album = 0;
    int total_mochila = 0;
    
    Figurinha *figurinhas = malloc(981 * sizeof(Figurinha));
    Figurinha *album = malloc(981 * sizeof(Figurinha));
    Figurinha *mochila = malloc(981 * sizeof(Figurinha));
    
    if (figurinhas == NULL || album == NULL || mochila == NULL){
        printf("Erro de alocacao.\n"); 
        return 1;
    }

    FILE *arquivo = fopen("extras/figurinhas2026.csv","rb");
    if (arquivo == NULL){
        printf("Erro ao abrir o arquivo figurinhas2026.csv.\n"); 
        return 1; 
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
    SetTargetFPS(60);

    EstadoMenu estadoAtual = MENU_PRINCIPAL;

    // Textos do Menu Principal (agora reduzido para 6 opções)
    const char *textosPrincipal[] = {
        "1. Abrir Pacote",
        "2. Ver Inventario",
        "3. Excluir do Inventario",
        "4. Pesquisar Figurinha",
        "5. Alterar Figurinha (Menu Terminal)",
        "6. Area de Minigames"
    };
    Rectangle botoesPrincipal[6];
    for (int i = 0; i < 6; i++) {
        botoesPrincipal[i] = (Rectangle){ 250, 220 + (i * 70), 500, 50 };
    }

    // Textos dos Submenus
    const char *textosInventario[] = { "1. Ver Album", "2. Ver Mochila", "3. Voltar ao Menu" };
    const char *textosExcluir[] = { "1. Excluir do Album", "2. Excluir da Mochila", "3. Voltar ao Menu" };
    const char *textosMinigames[] = { "1. Jogar Quiz da Copa", "2. Jogo do Goleiro", "3. Voltar ao Menu" };

    // Layout padrão para todos os submenus (3 botões espaçados)
    Rectangle botoesSubmenu[3];
    for (int i = 0; i < 3; i++) {
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
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (estadoAtual == MENU_PRINCIPAL) {
                for (int i = 0; i < 6; i++) {
                    if (CheckCollisionPointRec(mousePoint, botoesPrincipal[i])) {
                        if (i == 0) acaoEscolhida = 1; // Abrir pacote
                        else if (i == 1) estadoAtual = MENU_INVENTARIO;
                        else if (i == 2) estadoAtual = MENU_EXCLUIR;
                        else if (i == 3) acaoEscolhida = 6; // Pesquisar
                        else if (i == 4) acaoEscolhida = 7; // Alterar
                        else if (i == 5) estadoAtual = MENU_MINIGAMES;
                    }
                }
            } 
            else if (estadoAtual == MENU_INVENTARIO) {
                for (int i = 0; i < 3; i++) {
                    if (CheckCollisionPointRec(mousePoint, botoesSubmenu[i])) {
                        if (i == 0) acaoEscolhida = 2; // Ver Album
                        else if (i == 1) acaoEscolhida = 3; // Ver Mochila
                        else if (i == 2) estadoAtual = MENU_PRINCIPAL;
                    }
                }
            }
            else if (estadoAtual == MENU_EXCLUIR) {
                for (int i = 0; i < 3; i++) {
                    if (CheckCollisionPointRec(mousePoint, botoesSubmenu[i])) {
                        if (i == 0) acaoEscolhida = 4; // Excluir Album
                        else if (i == 1) acaoEscolhida = 5; // Excluir Mochila
                        else if (i == 2) estadoAtual = MENU_PRINCIPAL;
                    }
                }
            }
            else if (estadoAtual == MENU_MINIGAMES) {
                for (int i = 0; i < 3; i++) {
                    if (CheckCollisionPointRec(mousePoint, botoesSubmenu[i])) {
                        if (i == 0) acaoEscolhida = 8; // Quiz da Copa
                        else if (i == 1) acaoEscolhida = 9; // Jogo do Goleiro
                        else if (i == 2) estadoAtual = MENU_PRINCIPAL;
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
            DrawText("ALBUM DE FIGURINHAS DA COPA", 220, 60, 32, WHITE);
            DrawText("Selecione uma opcao abaixo:", 340, 120, 22, amareloBrasil);

            for (int i = 0; i < 6; i++) {
                bool mouseEmCima = CheckCollisionPointRec(mousePoint, botoesPrincipal[i]);
                DrawRectangleRec(botoesPrincipal[i], mouseEmCima ? LIGHTGRAY : amareloBrasil);
                DrawRectangleLinesEx(botoesPrincipal[i], 2, mouseEmCima ? azulBrasil : DARKGRAY);
                
                int textWidth = MeasureText(textosPrincipal[i], 22);
                DrawText(textosPrincipal[i], botoesPrincipal[i].x + (botoesPrincipal[i].width / 2) - (textWidth / 2), botoesPrincipal[i].y + 14, 22, azulBrasil);
            }
        } 
        else { // Lógica de desenho reaproveitada para todos os submenus
            const char *titulo;
            const char *subtitulo;
            const char **textosBotoes;

            // Define os textos baseados no estado atual
            if (estadoAtual == MENU_INVENTARIO) {
                titulo = "MEU INVENTARIO";
                subtitulo = "Qual colecao voce quer visualizar?";
                textosBotoes = textosInventario;
            } else if (estadoAtual == MENU_EXCLUIR) {
                titulo = "EXCLUIR FIGURINHAS";
                subtitulo = "De onde voce quer remover figurinhas?";
                textosBotoes = textosExcluir;
            } else { // MENU_MINIGAMES
                titulo = "AREA DE MINIGAMES";
                subtitulo = "Escolha seu desafio:";
                textosBotoes = textosMinigames;
            }

            int titleWidth = MeasureText(titulo, 36);
            DrawText(titulo, 500 - (titleWidth / 2), 100, 36, WHITE);
            
            int subtitleWidth = MeasureText(subtitulo, 24);
            DrawText(subtitulo, 500 - (subtitleWidth / 2), 180, 24, amareloBrasil);

            for (int i = 0; i < 3; i++) {
                bool mouseEmCima = CheckCollisionPointRec(mousePoint, botoesSubmenu[i]);
                DrawRectangleRec(botoesSubmenu[i], mouseEmCima ? LIGHTGRAY : amareloBrasil);
                DrawRectangleLinesEx(botoesSubmenu[i], 2, mouseEmCima ? azulBrasil : DARKGRAY);
                
                int textWidth = MeasureText(textosBotoes[i], 22);
                DrawText(textosBotoes[i], botoesSubmenu[i].x + (botoesSubmenu[i].width / 2) - (textWidth / 2), botoesSubmenu[i].y + 16, 22, azulBrasil);
            }
        }

        DrawText("Feche a janela (X) ou aperte ESC para sair do programa", 180, 740, 20, LIGHTGRAY);

        EndDrawing();

        // ==========================================
        // Execução da Ação Escolhida
        // ==========================================
        if (acaoEscolhida != 0) {
            CloseWindow(); 

            printf("\n=========================================\n");
            
            if (acaoEscolhida == 1) abrirPacote(figurinhas, mochila, album, total, &total_mochila, &total_album);
            else if (acaoEscolhida == 2) {
                printf("Total album: %d\n", total_album);
                listarFigurinhasAlbum(album, total_album);
            } 
            else if (acaoEscolhida == 3) {
                printf("Total mochila: %d\n", total_mochila);
                listarFigurinhasMochila(mochila, total_mochila);
            } 
            else if (acaoEscolhida == 4) excluirAlbum(figurinhas, album, &total_album);
            else if (acaoEscolhida == 5) excluirMochila(figurinhas, mochila, &total_mochila);
            else if (acaoEscolhida == 6) pesquisarFigurinha(figurinhas, total);
            else if (acaoEscolhida == 7) {
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
            else if (acaoEscolhida == 8) jogarQuiz(figurinhas, mochila, album, total, &total_mochila, &total_album);
            else if (acaoEscolhida == 9) jogarGoleiro();

            printf("\n=========================================\n");
            printf("Pressione ENTER para voltar ao menu grafico...");
            
            int c;
            while ((c = getchar()) != '\n' && c != EOF);
            getchar(); 

            // Restaura o estado para o Menu Principal ao voltar
            estadoAtual = MENU_PRINCIPAL;
            
            // Reabre a janela gráfica
            InitWindow(1000, 800, "Album de Figurinhas - Copa do Mundo");
            SetTargetFPS(60);
        }
    }

    printf("Saindo do programa...\n");

    free(figurinhas);
    free(album);
    free(mochila);

    return 0; 
}