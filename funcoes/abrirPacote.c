#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include "../headers/biblioteca.h"

// Função auxiliar para comparar se está no album
int estaNoAlbum(Figurinha *album, int total_album, char codigo[]){
    for(int i = 0; i < total_album; i++) {
        if(strcmp(album[i].codigo, codigo) == 0) {
            return 1;
        }
    }
    return 0;
}

// FUNÇÃO FAXINEIRA: Limpa espaços vazios no início/fim da string e sujeiras como quebras de linha (\r \n)
void limparString(char *str) {
    if (!str) return;
    int len = strlen(str);
    while(len > 0 && (str[len-1] == ' ' || str[len-1] == '\r' || str[len-1] == '\n')) {
        str[len-1] = '\0';
        len--;
    }
    int start = 0;
    while(str[start] == ' ') start++;
    if (start > 0) memmove(str, str + start, len - start + 1);
}

// Os 4 ecrãs do processo de abrir pacote
typedef enum { ESCOLHER_QTD, ANIMACAO, REVELAR_CARTA, FIM_ABERTURA } EstadoAbertura;

void abrirPacote(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, int *pacotes_fechados){

    // Verifica se tem pacotes
    if(*pacotes_fechados <= 0){
        printf("OPSSS! Nao tens pacotes fechados no inventario!\n");
        return;
    }

    // Inicializa o Ecrã Gráfico do Raylib
    InitWindow(1000, 800, "Abrindo Pacotes Magicos...");
    SetTargetFPS(60);

    Font fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf");
    
    // ==========================================
    // CARREGAMENTO DA ANIMAÇÃO (GIF)
    // ==========================================
    int animFrames = 0;
    Image imAnim = LoadImageAnim("imagens/animacao.gif", &animFrames);
    Texture2D texAnim = LoadTextureFromImage(imAnim);
    
    int currentAnimFrame = 0;
    int frameDelay = 4; // Velocidade do GIF
    int frameCounter = 0;
    int nextFrameDataOffset = 0;

    EstadoAbertura estado = ESCOLHER_QTD;
    int qtd_selecionada = 1;
    int total_figurinhas = 0;
    int figurinha_atual = 0;
    
    // Arrays para guardar as figurinhas sorteadas
    Figurinha *sorteadas_array = NULL;
    int *status_sorteadas = NULL; // 1 = Album, 2 = Mochila
    
    // Variável para guardar a imagem da figurinha atual
    Texture2D textura_figurinha = { 0 };

    // Áreas de Botões
    Rectangle btnMenos = { 350, 400, 50, 50 };
    Rectangle btnMais = { 600, 400, 50, 50 };
    Rectangle btnAbrir = { 400, 500, 200, 60 };
    Rectangle btnProxima = { 400, 700, 200, 60 };

    Color verdeCampo = (Color){ 34, 139, 34, 255 };
    Color amareloBrasil = (Color){ 255, 215, 0, 255 };
    Color azulBrasil = (Color){ 0, 39, 118, 255 };

    while(!WindowShouldClose()) {
        Vector2 mousePoint = GetMousePosition();

        // ==========================================
        // LÓGICA
        // ==========================================
        if (estado == ESCOLHER_QTD) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (CheckCollisionPointRec(mousePoint, btnMenos) && qtd_selecionada > 1) qtd_selecionada--;
                if (CheckCollisionPointRec(mousePoint, btnMais) && qtd_selecionada < *pacotes_fechados) qtd_selecionada++;
                
                if (CheckCollisionPointRec(mousePoint, btnAbrir)) {
                    *pacotes_fechados -= qtd_selecionada;
                    // salvarPacotes(); // Descomenta se quiseres guardar o saldo permanentemente
                    total_figurinhas = qtd_selecionada * 7;
                    
                    sorteadas_array = malloc(total_figurinhas * sizeof(Figurinha));
                    status_sorteadas = malloc(total_figurinhas * sizeof(int));

                    FILE *arquivoalbum = fopen("extras/album.csv", "a");
                    FILE *arquivomochila = fopen("extras/mochila.csv", "a");

                    for(int i = 0; i < total_figurinhas; i++) {
                        int idx = rand() % total;
                        sorteadas_array[i] = figurinhas[idx];

                        if(!estaNoAlbum(album, *total_album, figurinhas[idx].codigo)) {
                            album[*total_album] = figurinhas[idx];
                            (*total_album)++;
                            if(arquivoalbum) fprintf(arquivoalbum,"%s,%s,%s,%s,%s\n", figurinhas[idx].codigo, figurinhas[idx].titulo, figurinhas[idx].secao, figurinhas[idx].grupo, figurinhas[idx].tipo);
                            status_sorteadas[i] = 1; // Foi para o Album
                        } else {
                            mochila[*total_mochila] = figurinhas[idx];
                            (*total_mochila)++;
                            if(arquivomochila) fprintf(arquivomochila,"%s,%s,%s,%s,%s\n", figurinhas[idx].codigo, figurinhas[idx].titulo, figurinhas[idx].secao, figurinhas[idx].grupo, figurinhas[idx].tipo);
                            status_sorteadas[i] = 2; // Repetida (Mochila)
                        }
                    }
                    if(arquivoalbum) fclose(arquivoalbum);
                    if(arquivomochila) fclose(arquivomochila);

                    estado = ANIMACAO; 
                }
            }
        } 
        else if (estado == ANIMACAO) {
            frameCounter++;
            if (frameCounter >= frameDelay) {
                currentAnimFrame++;
                if (currentAnimFrame >= animFrames) {
                    estado = REVELAR_CARTA;
                    
                    // ========================================================
                    // A MÁGICA DOS ESPAÇOS -> UNDERLINES E CAMINHO DUPLO AQUI!
                    // ========================================================
                    char pastaLimpa[100];
                    strcpy(pastaLimpa, sorteadas_array[figurinha_atual].secao);
                    limparString(pastaLimpa); 
                    
                    for(int j = 0; pastaLimpa[j] != '\0'; j++) {
                        if(pastaLimpa[j] == ' ') pastaLimpa[j] = '_';
                    }

                    char codigoLimpo[100];
                    strcpy(codigoLimpo, sorteadas_array[figurinha_atual].codigo);
                    limparString(codigoLimpo); 

                    // CAMINHO CORRIGIDO COM A PASTA "imagens" ANTES
                    const char *caminhoImagem = TextFormat("imagens/imagens_figurinhas/%s/%s.png", pastaLimpa, codigoLimpo);
                    printf("\nDEBUG 1: Tentando abrir a imagem -> [%s]\n", caminhoImagem);
                    textura_figurinha = LoadTexture(caminhoImagem);
                    
                } else {
                    nextFrameDataOffset = imAnim.width * imAnim.height * 4 * currentAnimFrame;
                    UpdateTexture(texAnim, ((unsigned char *)imAnim.data) + nextFrameDataOffset);
                }
                frameCounter = 0;
            }
        }
        else if (estado == REVELAR_CARTA) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePoint, btnProxima)) {
                figurinha_atual++;
                if (figurinha_atual >= total_figurinhas) {
                    estado = FIM_ABERTURA;
                } else {
                    // ========================================================
                    // A MÁGICA SE REPETE PARA AS OUTRAS CARTAS DO PACOTE
                    // ========================================================
                    UnloadTexture(textura_figurinha);

                    char pastaLimpa[100];
                    strcpy(pastaLimpa, sorteadas_array[figurinha_atual].secao);
                    limparString(pastaLimpa);
                    
                    for(int j = 0; pastaLimpa[j] != '\0'; j++) {
                        if(pastaLimpa[j] == ' ') pastaLimpa[j] = '_';
                    }

                    char codigoLimpo[100];
                    strcpy(codigoLimpo, sorteadas_array[figurinha_atual].codigo);
                    limparString(codigoLimpo);

                    // CAMINHO CORRIGIDO COM A PASTA "imagens" ANTES
                    const char *caminhoImagem = TextFormat("imagens/imagens_figurinhas/%s/%s.png", pastaLimpa, codigoLimpo);
                    printf("DEBUG 2: Tentando abrir a imagem -> [%s]\n", caminhoImagem);
                    textura_figurinha = LoadTexture(caminhoImagem);
                }
            }
        }
        else if (estado == FIM_ABERTURA) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mousePoint, btnProxima)) {
                break; 
            }
        }

        // ==========================================
        // DESENHO
        // ==========================================
        BeginDrawing();
        ClearBackground(verdeCampo);

        if (estado == ESCOLHER_QTD) {
            DrawTextEx(fonteCopa, "ABRIR PACOTES", (Vector2){ 320, 100 }, 30, 2, WHITE);
            DrawTextEx(fonteCopa, TextFormat("Saldo disponivel: %d", *pacotes_fechados), (Vector2){ 310, 200 }, 16, 2, amareloBrasil);
            
            DrawRectangleRec(btnMenos, CheckCollisionPointRec(mousePoint, btnMenos) ? LIGHTGRAY : DARKGRAY);
            DrawTextEx(fonteCopa, "-", (Vector2){ btnMenos.x + 15, btnMenos.y + 10 }, 30, 2, WHITE);
            
            DrawTextEx(fonteCopa, TextFormat("%d", qtd_selecionada), (Vector2){ 470, 410 }, 30, 2, WHITE);
            
            DrawRectangleRec(btnMais, CheckCollisionPointRec(mousePoint, btnMais) ? LIGHTGRAY : DARKGRAY);
            DrawTextEx(fonteCopa, "+", (Vector2){ btnMais.x + 15, btnMais.y + 10 }, 30, 2, WHITE);

            DrawRectangleRec(btnAbrir, CheckCollisionPointRec(mousePoint, btnAbrir) ? WHITE : amareloBrasil);
            DrawTextEx(fonteCopa, "ABRIR", (Vector2){ btnAbrir.x + 50, btnAbrir.y + 20 }, 20, 2, azulBrasil);
        }
        else if (estado == ANIMACAO) {
            if (texAnim.id != 0) {
                Vector2 pos = { 500 - (texAnim.width / 2), 400 - (texAnim.height / 2) };
                DrawTexture(texAnim, pos.x, pos.y, WHITE);
            } else {
                DrawTextEx(fonteCopa, "(Ficheiro animacao.gif nao encontrado na pasta imagens)", (Vector2){ 150, 400 }, 14, 2, WHITE);
                estado = REVELAR_CARTA; 
            }
        }
        else if (estado == REVELAR_CARTA) {
            DrawTextEx(fonteCopa, TextFormat("FIGURINHA %d DE %d", figurinha_atual + 1, total_figurinhas), (Vector2){ 300, 50 }, 20, 2, WHITE);

            Rectangle cardRec = { 300, 100, 400, 530 };
            DrawRectangleRounded(cardRec, 0.05f, 10, WHITE);
            DrawRectangleRoundedLinesEx(cardRec, 0.05f, 10, 4, azulBrasil);

            // SE A IMAGEM EXISTIR NA PASTA, MOSTRA A IMAGEM
            if (textura_figurinha.id != 0) {
                Rectangle rectOrigem = { 0, 0, textura_figurinha.width, textura_figurinha.height };
                Rectangle rectDestino = { 320, 120, 360, 400 }; 
                DrawTexturePro(textura_figurinha, rectOrigem, rectDestino, (Vector2){0, 0}, 0.0f, WHITE);
                
                int textWidth = MeasureTextEx(fonteCopa, sorteadas_array[figurinha_atual].titulo, 14, 1).x;
                DrawTextEx(fonteCopa, sorteadas_array[figurinha_atual].titulo, (Vector2){ 500 - (textWidth / 2), 540 }, 14, 1, BLACK);
            } 
            // SE NÃO EXISTIR, MOSTRA O TEXTO COMO ALTERNATIVA DE SEGURANÇA
            else {
                DrawTextEx(fonteCopa, "[IMAGEM NAO ENCONTRADA]", (Vector2){ 320, 150 }, 12, 1, RED);
                DrawTextEx(fonteCopa, sorteadas_array[figurinha_atual].codigo, (Vector2){ 320, 230 }, 20, 2, DARKGRAY);
                DrawTextEx(fonteCopa, sorteadas_array[figurinha_atual].titulo, (Vector2){ 320, 280 }, 16, 1, BLACK);
                DrawTextEx(fonteCopa, sorteadas_array[figurinha_atual].secao, (Vector2){ 320, 330 }, 14, 1, DARKGRAY);
                DrawTextEx(fonteCopa, sorteadas_array[figurinha_atual].grupo, (Vector2){ 320, 380 }, 14, 1, DARKGRAY);
                DrawTextEx(fonteCopa, sorteadas_array[figurinha_atual].tipo, (Vector2){ 320, 430 }, 14, 1, DARKGRAY);
            }

            // Barra de Estado (Nova ou Repetida)
            Rectangle statusRec = { 300, 600, 400, 30 };
            Color corStatus = (status_sorteadas[figurinha_atual] == 1) ? GREEN : ORANGE;
            DrawRectangleRec(statusRec, corStatus);
            
            if (status_sorteadas[figurinha_atual] == 1) {
                int width = MeasureTextEx(fonteCopa, "NOVA! -> FOI PARA O ALBUM", 12, 1).x;
                DrawTextEx(fonteCopa, "NOVA! -> FOI PARA O ALBUM", (Vector2){ 500 - (width / 2), 610 }, 12, 1, BLACK);
            } else {
                int width = MeasureTextEx(fonteCopa, "REPETIDA -> FOI PARA A MOCHILA", 12, 1).x;
                DrawTextEx(fonteCopa, "REPETIDA -> FOI PARA A MOCHILA", (Vector2){ 500 - (width / 2), 610 }, 12, 1, BLACK);
            }

            DrawRectangleRec(btnProxima, CheckCollisionPointRec(mousePoint, btnProxima) ? WHITE : amareloBrasil);
            DrawTextEx(fonteCopa, "PROXIMA", (Vector2){ btnProxima.x + 40, btnProxima.y + 20 }, 20, 2, azulBrasil);
        }
        else if (estado == FIM_ABERTURA) {
            DrawTextEx(fonteCopa, "TODAS AS FIGURINHAS ABERTAS!", (Vector2){ 200, 300 }, 24, 2, WHITE);
            
            DrawRectangleRec(btnProxima, CheckCollisionPointRec(mousePoint, btnProxima) ? WHITE : amareloBrasil);
            DrawTextEx(fonteCopa, "CONCLUIR", (Vector2){ btnProxima.x + 30, btnProxima.y + 20 }, 20, 2, azulBrasil);
        }

        EndDrawing();
    }

    // Liberta a memória usada
    if (sorteadas_array) free(sorteadas_array);
    if (status_sorteadas) free(status_sorteadas);
    if (textura_figurinha.id != 0) UnloadTexture(textura_figurinha);
    UnloadTexture(texAnim);
    UnloadImage(imAnim);
    UnloadFont(fonteCopa);
    CloseWindow();
}