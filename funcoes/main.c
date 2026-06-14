#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <time.h> 
#include <raylib.h>
#include "biblioteca.h"

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

    // Leitura dos arquivos (mantida exatamente como a sua)
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

    fgets(linha, sizeof(linha), arquivo); // ignora cabecalho
    while (total < 981 && fscanf(arquivo, " %9[^,],%49[^,],%49[^,],%49[^,],%49[^\n]", figurinhas[total].codigo, figurinhas[total].titulo, figurinhas[total].secao, figurinhas[total].grupo, figurinhas[total].tipo) == 5){
        total++;
    }
    fclose(arquivo);
    
    srand(time(NULL));

    // ==========================================
    // CONFIGURAÇÃO DA INTERFACE GRÁFICA (RAYLIB)
    // ==========================================
    InitWindow(1000, 800, "Album de Figurinhas - Copa do Mundo");
    SetTargetFPS(60);

    const char *textosBotoes[] = {
        "1. Abrir Pacote",
        "2. Ver Album",
        "3. Ver Mochila",
        "4. Excluir do Album",
        "5. Excluir da Mochila",
        "6. Pesquisar Figurinha",
        "7. Alterar Figurinha (Menu Terminal)",
        "8. Jogar Quiz da Copa",
        "9. Jogo do Goleiro"
    };

    Rectangle botoes[9];
    for (int i = 0; i < 9; i++) {
        botoes[i] = (Rectangle){ 250, 180 + (i * 55), 500, 45 };
    }

    // Cores temáticas da Copa
    Color verdeCampo = (Color){ 34, 139, 34, 255 };
    Color amareloBrasil = (Color){ 255, 215, 0, 255 };
    Color azulBrasil = (Color){ 0, 39, 118, 255 };

    while (!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();
        int acaoEscolhida = 0;

        // Lógica de clique
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            for (int i = 0; i < 9; i++) {
                if (CheckCollisionPointRec(mousePoint, botoes[i])) {
                    acaoEscolhida = i + 1; // Guarda qual botão foi clicado (1 a 9)
                }
            }
        }

        // Desenho do Menu
        BeginDrawing();
        ClearBackground(verdeCampo);

        // Títulos
        DrawText("ALBUM DE FIGURINHAS DA COPA", 220, 50, 32, WHITE);
        DrawText("Selecione uma opcao abaixo:", 340, 110, 22, amareloBrasil);

        // Desenha os botões
        for (int i = 0; i < 9; i++) {
            bool mouseEmCima = CheckCollisionPointRec(mousePoint, botoes[i]);
            DrawRectangleRec(botoes[i], mouseEmCima ? LIGHTGRAY : amareloBrasil);
            DrawRectangleLinesEx(botoes[i], 2, mouseEmCima ? azulBrasil : DARKGRAY);
            
            int textWidth = MeasureText(textosBotoes[i], 20);
            DrawText(textosBotoes[i], botoes[i].x + (botoes[i].width / 2) - (textWidth / 2), botoes[i].y + 10, 20, azulBrasil);
        }

        DrawText("Feche a janela (X) ou aperte ESC para sair do programa", 180, 740, 20, LIGHTGRAY);

        EndDrawing();

        // Execução da Ação Escolhida
        if (acaoEscolhida != 0) {
            CloseWindow(); // Fecha a interface gráfica para liberar o terminal

            printf("\n=========================================\n");
            
            // Chama as funções originais do seu projeto
            if (acaoEscolhida == 1) {
                abrirPacote(figurinhas, mochila, album, total, &total_mochila, &total_album);
            } 
            else if (acaoEscolhida == 2){
                printf("Total album: %d\n", total_album);
                listarFigurinhasAlbum(album, total_album);
            } 
            else if (acaoEscolhida == 3){
                printf("Total mochila: %d\n", total_mochila);
                listarFigurinhasMochila(mochila, total_mochila);
            } 
            else if (acaoEscolhida == 4){
                excluirAlbum(figurinhas, album, &total_album);
            } 
            else if (acaoEscolhida == 5){
                excluirMochila(figurinhas, mochila, &total_mochila);
            } 
            else if (acaoEscolhida == 6){
                pesquisarFigurinha(figurinhas, total);
            } 
            else if (acaoEscolhida == 7){
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
            else if (acaoEscolhida == 8){
                jogarQuiz(figurinhas, mochila, album, total, &total_mochila, &total_album);
            }
            else if (acaoEscolhida == 9){
                jogarGoleiro();
            }

            printf("\n=========================================\n");
            printf("Pressione ENTER para voltar ao menu grafico...");
            
            // Pausa para o usuário ler o terminal antes de reabrir o gráfico
            int c;
            while ((c = getchar()) != '\n' && c != EOF); // Limpa o buffer caso tenha sobrado lixo
            getchar(); // Aguarda o ENTER real

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