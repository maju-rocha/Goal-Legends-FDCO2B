#include "raylib.h"
#include "raymath.h"
#include "biblioteca.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//=======================================================//
//==================== Paleta de Cores ==================//
//=======================================================//

#define COPA_AZUL_ESCURO (Color){5, 7, 15, 255}
#define COPA_AZUL_MEDIO  (Color){14, 20, 38, 255}
#define COPA_VERDE_NEON  (Color){0, 255, 140, 255}
#define COPA_VERMELHO    (Color){240, 30, 70, 255}
#define COPA_OURO_PURO   (Color){255, 185, 0, 255}
#define COPA_OURO_BRILHO (Color){255, 240, 150, 255}
#define COPA_CINZA_CARD  (Color){22, 28, 48, 255}

#define QUANTIDADE_MAXIMA_PARTICULA 60
#define MAX_CARTAS_SORTEADAS 700 // Suporta até 100 pacotes simultâneos no Modo Turbo

//=======================================================//
//======================= Structs =======================//
//=======================================================//

//Struct das particulas do fundo da tela
typedef struct{
    Vector2 posicao;
    Vector2 velocidade;
    float tamanhoBase;
    float randomizadorPosicao;
    float transparenciaParticula;
    float velocidadeTransparenciaParticula;
    Color cor;
}Particula;

//Struct da tela de abertura de pacotes
typedef struct{
    int quantidadeDesejada;
    int fase;
    int cartaAtualIndice;
    int totalCartasSorteioAtual; // Dinâmico (ex: 7 no normal, 70 no turbo)
    bool modoTurbo;              // Controla se vai fazer loop no final ou sair

    int statusCartas[MAX_CARTAS_SORTEADAS];
    char statusMensagem[50];
    char codigoFotoCarregada[10];

    bool particulasInicializadas;
    Texture2D texturaCartaAtual;
    Figurinha pacoteSorteado[MAX_CARTAS_SORTEADAS];
    Particula particulas[QUANTIDADE_MAXIMA_PARTICULA];

    Image gifImage;
    Texture2D gifTextura;
    int animFrames;
    int contadorFrames;
    int frameMomento;
    bool gifCarregado;
}AberturaPacotes;

//Inicializa todos os valores da tela como zero
static AberturaPacotes tela = {0};

//=======================================================//
//================== Funcoes Auxiliares =================//
//=======================================================//

//Funcao para transformar numero inteiro em texto
void numeroParaTexto(int numero, char *destino){
    char invertido[20];
    int i = 0;
    int j = 0;

    if(numero == 0){
        destino[0] = '0';
        destino[1] = '\0';
        return;
    }

    if(numero < 0){
        destino[j] = '-';
        j++;
        numero = numero * -1;
    }

    while(numero > 0){
        invertido[i] = (numero % 10) + '0';
        numero = numero / 10;
        i++;
    }

    while(i > 0){
        i--;
        destino[j] = invertido[i];
        j++;
    }

    destino[j] = '\0';
}

//Funcao para transformar numero em texto com dois digitos, exemplo: 1 vira 01
void numeroParaTextoComDoisDigitos(int numero, char *destino){
    if(numero >= 0 && numero < 10){
        destino[0] = '0';
        numeroParaTexto(numero, destino + 1);
    }else{
        numeroParaTexto(numero, destino);
    }
}

//Funcao para descarregar texturas e gif da tela de abertura
static void descarregarRecursosAbertura(){
    if(tela.texturaCartaAtual.id > 0){
        UnloadTexture(tela.texturaCartaAtual);
        tela.texturaCartaAtual.id = 0;
    }
    tela.codigoFotoCarregada[0] = '\0';
    if(tela.gifCarregado){
        UnloadTexture(tela.gifTextura);
        UnloadImage(tela.gifImage);
        tela.gifCarregado = false;
    }
}

//Funcao para sortear as figurinhas baseado na quantidade de pacotes
static void sortearFigurinhasDoPacote(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, int qtdPacotes){
    
    tela.totalCartasSorteioAtual = qtdPacotes * 7;

    // Proteção de memória para não exceder o limite do array
    if (tela.totalCartasSorteioAtual > MAX_CARTAS_SORTEADAS) {
        tela.totalCartasSorteioAtual = MAX_CARTAS_SORTEADAS;
    }

    for(int f = 0; f < tela.totalCartasSorteioAtual; f++){
        Figurinha sorteada = figurinhas[rand() % total];
        limparFigurinha(&sorteada);
        tela.pacoteSorteado[f] = sorteada;

        if(!estaNoAlbum(album, *total_album, sorteada.codigo)){
            album[*total_album] = sorteada;
            (*total_album)++;

            FILE *totalFigurinhas = fopen("extras/album.csv", "a");
            if(totalFigurinhas){
                fprintf(totalFigurinhas, "%s,%s,%s,%s,%s\n", sorteada.codigo, sorteada.titulo, sorteada.secao, sorteada.grupo, sorteada.tipo);
                fclose(totalFigurinhas);
            }
            tela.statusCartas[f] = 1;
        }else{
            mochila[*total_mochila] = sorteada;
            (*total_mochila)++;

            FILE *totalFigurinhas = fopen("extras/mochila.csv", "a");
            if(totalFigurinhas){
                fprintf(totalFigurinhas, "%s,%s,%s,%s,%s\n", sorteada.codigo, sorteada.titulo, sorteada.secao, sorteada.grupo, sorteada.tipo);
                fclose(totalFigurinhas);
            }
            tela.statusCartas[f] = 2;
            salvarRepetida();
        }
    }
}

//=======================================================//
//==================== Particulas Fundo =================//
//=======================================================//

static void spawnParticulas(){
    for(int i = 0; i < QUANTIDADE_MAXIMA_PARTICULA; i++){
        tela.particulas[i].posicao = (Vector2){(float)GetRandomValue(0, 1000), (float)GetRandomValue(0, 800)};
        tela.particulas[i].velocidade = (Vector2){(float)GetRandomValue(-8, 8) / 10.0f, (float)GetRandomValue(-15, -5) / 10.0f};
        tela.particulas[i].tamanhoBase = (float)GetRandomValue(2, 5);
        tela.particulas[i].randomizadorPosicao = (float)GetRandomValue(0, 360);
        tela.particulas[i].transparenciaParticula = (float)GetRandomValue(30, 90) / 100.0f;
        tela.particulas[i].velocidadeTransparenciaParticula = (float)GetRandomValue(5, 15) / 1000.0f;

        if(GetRandomValue(0, 1) == 0){
            tela.particulas[i].cor = COPA_OURO_PURO;
        }else{
            tela.particulas[i].cor = COPA_VERDE_NEON;
        }
    }
    tela.particulasInicializadas = true;
}

static void efeitoParticulas(Vector2 mousePos, float tempoGlobal){
    if(!tela.particulasInicializadas) spawnParticulas();

    for(int i = 0; i < QUANTIDADE_MAXIMA_PARTICULA; i++){
        tela.particulas[i].posicao.x += tela.particulas[i].velocidade.x + sinf(tempoGlobal * 2.0f + tela.particulas[i].randomizadorPosicao) * 0.25f;
        tela.particulas[i].posicao.y += tela.particulas[i].velocidade.y;
        tela.particulas[i].transparenciaParticula -= tela.particulas[i].velocidadeTransparenciaParticula;

        float distAoMouse = Vector2Distance(mousePos, tela.particulas[i].posicao);
        if(distAoMouse < 100.0f){
            tela.particulas[i].posicao = Vector2Lerp(tela.particulas[i].posicao, mousePos, 0.01f);
        }

        if(tela.particulas[i].transparenciaParticula <= 0.0f || tela.particulas[i].posicao.y < -10 || tela.particulas[i].posicao.x < -10 || tela.particulas[i].posicao.x > 1010){
            tela.particulas[i].posicao = (Vector2){(float)GetRandomValue(0, 1000), 810.0f};
            tela.particulas[i].transparenciaParticula = (float)GetRandomValue(50, 95) / 100.0f;
        }

        float tamPulso = tela.particulas[i].tamanhoBase * (1.0f + sinf(tempoGlobal * 4.0f + tela.particulas[i].randomizadorPosicao) * 0.2f);
        DrawCircleV(tela.particulas[i].posicao, tamPulso, Fade(tela.particulas[i].cor, tela.particulas[i].transparenciaParticula * 0.5f));
    }
}

//=======================================================//
//=================== Textura da Carta ==================//
//=======================================================//

static void carregarTexturaCarta(Figurinha fig){
    if(strcmp(tela.codigoFotoCarregada, fig.codigo) == 0) return;

    if(tela.texturaCartaAtual.id > 0){
        UnloadTexture(tela.texturaCartaAtual);
        tela.texturaCartaAtual.id = 0;
    }

    char nomePastaPais[100];
    strcpy(nomePastaPais, fig.secao);
    trocarEspacoPorUnderline(nomePastaPais);

    char caminhoFoto[256];
    strcpy(caminhoFoto, "imagens/imagens_figurinhas/");
    strcat(caminhoFoto, nomePastaPais);
    strcat(caminhoFoto, "/");
    strcat(caminhoFoto, fig.codigo);
    strcat(caminhoFoto, ".png");

    if(FileExists(caminhoFoto)){
        Image img = LoadImage(caminhoFoto);
        if (img.data != NULL) {
            ImageResize(&img, 210, 290);
            tela.texturaCartaAtual = LoadTextureFromImage(img);
            UnloadImage(img);
            SetTextureFilter(tela.texturaCartaAtual, TEXTURE_FILTER_BILINEAR);
            strcpy(tela.codigoFotoCarregada, fig.codigo);
        } else {
            TraceLog(LOG_ERROR, "!!! ARQUIVO CORROMPIDO OU FORMATO INVALIDO: %s !!!", caminhoFoto);
            tela.texturaCartaAtual.id = 0;
            tela.codigoFotoCarregada[0] = '\0';
        }
    }else{
        TraceLog(LOG_ERROR, "!!! IMAGEM NAO ENCONTRADA: %s !!!", caminhoFoto);
        tela.texturaCartaAtual.id = 0;
        tela.codigoFotoCarregada[0] = '\0';
    }
}

static void sombraTela(){
    DrawRectangleGradientV(0, 0, 1000, 120, Fade(BLACK, 0.7f), Fade(BLACK, 0.0f));
    DrawRectangleGradientV(0, 680, 1000, 120, Fade(BLACK, 0.0f), Fade(BLACK, 0.8f));
}

//=======================================================//
//================== Tela Abrir Pacote ==================//
//=======================================================//

void abrirPacote(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, Font fonteCopa, Color azulBrasil, Color amareloBrasil, EstadoMenu *estadoAtual){
    (void)amareloBrasil;

    Vector2 mousePoint = GetMousePosition();
    float tempoGlobal = (float)GetTime();

    if(!tela.gifCarregado){
        tela.animFrames = 0;
        tela.gifImage = LoadImageAnim("imagens/animacao.gif", &tela.animFrames);
        tela.gifTextura = LoadTextureFromImage(tela.gifImage);
        tela.contadorFrames = 0;
        tela.frameMomento = 0;
        tela.gifCarregado = true;
    }

    DrawRectangleGradientV(0, 0, 1000, 800, COPA_AZUL_ESCURO, azulBrasil);
    efeitoParticulas(mousePoint, tempoGlobal);
    sombraTela();

    //=======================================================//
    //================ Fase 0: Escolher Pacotes =============//
    //=======================================================//

    if(tela.fase == 0){
        if(tela.quantidadeDesejada < 1) tela.quantidadeDesejada = 1;
        if(tela.quantidadeDesejada > pacotes_fechados && pacotes_fechados > 0) tela.quantidadeDesejada = pacotes_fechados;
        if(pacotes_fechados == 0) tela.quantidadeDesejada = 0;

        DrawRectangle(180, 160, 640, 440, Fade(COPA_AZUL_MEDIO, 0.85f));
        DrawRectangleLinesEx((Rectangle){180, 160, 640, 440}, 2.0f, Fade(COPA_OURO_PURO, 0.5f));

        DrawLineEx((Vector2){175, 155}, (Vector2){210, 155}, 3.0f, COPA_OURO_PURO);
        DrawLineEx((Vector2){175, 155}, (Vector2){175, 190}, 3.0f, COPA_OURO_PURO);
        DrawLineEx((Vector2){825, 605}, (Vector2){790, 605}, 3.0f, COPA_VERDE_NEON);
        DrawLineEx((Vector2){825, 605}, (Vector2){825, 570}, 3.0f, COPA_VERDE_NEON);

        char pacotesDisponiveis[50];
        char numeroPacotes[20];
        numeroParaTextoComDoisDigitos(pacotes_fechados, numeroPacotes);
        strcpy(pacotesDisponiveis, "PACOTES DISPONIVEIS: ");
        strcat(pacotesDisponiveis, numeroPacotes);

        DrawTextEx(fonteCopa, pacotesDisponiveis, (Vector2){500 - MeasureTextEx(fonteCopa, pacotesDisponiveis, 15, 1).x / 2, 210}, 15, 1, Fade(WHITE, 0.5f));
        DrawTextEx(fonteCopa, "SELECIONE A QUANTIDADE", (Vector2){500 - MeasureTextEx(fonteCopa, "SELECIONE A QUANTIDADE", 22, 2).x / 2, 255}, 22, 2, WHITE);

        // Layout redistribuído para caber o botão MAX
        Rectangle btnMenos = {290, 320, 60, 60};
        Rectangle boxQtd   = {370, 320, 160, 60};
        Rectangle btnMais  = {550, 320, 60, 60};
        Rectangle btnMax   = {630, 320, 80, 60};

        bool seMenos = CheckCollisionPointRec(mousePoint, btnMenos);
        bool seMais  = CheckCollisionPointRec(mousePoint, btnMais);
        bool seMax   = CheckCollisionPointRec(mousePoint, btnMax);

        // Botão Menos
        DrawRectangleRec(btnMenos, seMenos ? Fade(COPA_VERMELHO, 0.2f) : Fade(COPA_AZUL_ESCURO, 0.6f));
        DrawRectangleLinesEx(btnMenos, seMenos ? 2.0f : 1.0f, seMenos ? COPA_VERMELHO : COPA_OURO_PURO);
        DrawTextEx(fonteCopa, "-", (Vector2){btnMenos.x + 30 - MeasureTextEx(fonteCopa, "-", 24, 1).x / 2, btnMenos.y + 18}, 24, 1, WHITE);

        // Caixa de Quantidade
        DrawRectangleRec(boxQtd, Fade(BLACK, 0.4f));
        DrawRectangleLinesEx(boxQtd, 1.0f, Fade(WHITE, 0.2f));
        char txtQtd[10];
        numeroParaTextoComDoisDigitos(tela.quantidadeDesejada, txtQtd);
        DrawTextEx(fonteCopa, txtQtd, (Vector2){boxQtd.x + 80 - MeasureTextEx(fonteCopa, txtQtd, 32, 2).x / 2, boxQtd.y + 14}, 32, 2, COPA_VERDE_NEON);

        // Botão Mais
        DrawRectangleRec(btnMais, seMais ? Fade(COPA_VERDE_NEON, 0.2f) : Fade(COPA_AZUL_ESCURO, 0.6f));
        DrawRectangleLinesEx(btnMais, seMais ? 2.0f : 1.0f, seMais ? COPA_VERDE_NEON : COPA_OURO_PURO);
        DrawTextEx(fonteCopa, "+", (Vector2){btnMais.x + 30 - MeasureTextEx(fonteCopa, "+", 24, 1).x / 2, btnMais.y + 18}, 24, 1, WHITE);

        // Botão Max
        DrawRectangleRec(btnMax, seMax ? Fade(COPA_OURO_PURO, 0.2f) : Fade(COPA_AZUL_ESCURO, 0.6f));
        DrawRectangleLinesEx(btnMax, seMax ? 2.0f : 1.0f, COPA_OURO_PURO);
        DrawTextEx(fonteCopa, "MAX", (Vector2){btnMax.x + 40 - MeasureTextEx(fonteCopa, "MAX", 16, 1).x / 2, btnMax.y + 22}, 16, 1, seMax ? COPA_OURO_PURO : WHITE);

        if(IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
            if(seMenos && tela.quantidadeDesejada > 1) tela.quantidadeDesejada--;
            if(seMais && tela.quantidadeDesejada < pacotes_fechados) tela.quantidadeDesejada++;
            if(seMax && pacotes_fechados > 0) tela.quantidadeDesejada = pacotes_fechados;
        }

        // ====== Botão Abrir NORMAL ======
        const char *textoAbrirN = "ABRIR (1 POR VEZ)";
        float largN = MeasureTextEx(fonteCopa, textoAbrirN, 14, 1).x;
        Rectangle btnAbrirN = {500 - (largN / 2) - 20, 405, largN + 40, 14 + 24};
        bool seAbrirN = CheckCollisionPointRec(mousePoint, btnAbrirN);

        DrawRectangleRec(btnAbrirN, seAbrirN ? Fade(COPA_OURO_PURO, 0.2f) : Fade(COPA_OURO_PURO, 0.05f));
        DrawRectangleLinesEx(btnAbrirN, seAbrirN ? 2.0f : 1.0f, COPA_OURO_PURO);
        DrawTextEx(fonteCopa, textoAbrirN, (Vector2){btnAbrirN.x + 20, btnAbrirN.y + 12}, 14, 1, seAbrirN ? COPA_OURO_PURO : WHITE);

        if(seAbrirN && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
            if(pacotes_fechados > 0 && tela.quantidadeDesejada > 0){
                tela.modoTurbo = false;
                pacotes_fechados--;
                salvarPacotes();
                sortearFigurinhasDoPacote(figurinhas, mochila, album, total, total_mochila, total_album, 1);

                tela.cartaAtualIndice = 0;
                tela.fase = 1;
                strcpy(tela.statusMensagem, "");
            }else{
                strcpy(tela.statusMensagem, "PACOTES INSUFICIENTES!");
            }
        }

        // ====== Botão Abrir TURBO ======
        const char *textoAbrirT = "ABRIR MODO TURBO (TODOS)";
        float largT = MeasureTextEx(fonteCopa, textoAbrirT, 14, 1).x;
        Rectangle btnAbrirT = {500 - (largT / 2) - 20, 465, largT + 40, 14 + 24};
        bool seAbrirT = CheckCollisionPointRec(mousePoint, btnAbrirT);

        DrawRectangleRec(btnAbrirT, seAbrirT ? Fade(COPA_VERDE_NEON, 0.2f) : Fade(COPA_AZUL_ESCURO, 0.6f));
        DrawRectangleLinesEx(btnAbrirT, seAbrirT ? 2.0f : 1.0f, seAbrirT ? COPA_VERDE_NEON : COPA_OURO_PURO);
        DrawTextEx(fonteCopa, textoAbrirT, (Vector2){btnAbrirT.x + 20, btnAbrirT.y + 12}, 14, 1, seAbrirT ? COPA_VERDE_NEON : WHITE);

        if(seAbrirT && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
            if(pacotes_fechados > 0 && tela.quantidadeDesejada > 0){
                tela.modoTurbo = true;
                int pacotesParaAbrir = tela.quantidadeDesejada;
                
                pacotes_fechados -= pacotesParaAbrir;
                salvarPacotes();
                sortearFigurinhasDoPacote(figurinhas, mochila, album, total, total_mochila, total_album, pacotesParaAbrir);

                tela.quantidadeDesejada = 0; // Zera para encerrar após mostrar tudo
                tela.cartaAtualIndice = 0;
                tela.fase = 1;
                strcpy(tela.statusMensagem, "");
            }else{
                strcpy(tela.statusMensagem, "PACOTES INSUFICIENTES!");
            }
        }

        // ====== Botão Voltar ao Menu ======
        const char *textoVoltar = "VOLTAR AO MENU";
        float largV = MeasureTextEx(fonteCopa, textoVoltar, 12, 1).x;
        Rectangle btnVoltar = {500 - (largV / 2) - 20, 525, largV + 40, 12 + 20};
        bool seVoltar = CheckCollisionPointRec(mousePoint, btnVoltar);

        DrawRectangleRec(btnVoltar, seVoltar ? Fade(COPA_VERMELHO, 0.15f) : Fade(WHITE, 0.02f));
        DrawRectangleLinesEx(btnVoltar, 1.0f, seVoltar ? COPA_VERMELHO : Fade(WHITE, 0.2f));
        DrawTextEx(fonteCopa, textoVoltar, (Vector2){btnVoltar.x + 20, btnVoltar.y + 10}, 12, 1, seVoltar ? COPA_VERMELHO : Fade(WHITE, 0.6f));

        if(seVoltar && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)){
            descarregarRecursosAbertura();
            tela.quantidadeDesejada = 1;
            *estadoAtual = MENU_PRINCIPAL;
        }

        if(strlen(tela.statusMensagem) > 0){
            DrawTextEx(fonteCopa, tela.statusMensagem, (Vector2){500 - MeasureTextEx(fonteCopa, tela.statusMensagem, 14, 1).x / 2, 120}, 14, 1, COPA_VERMELHO);
        }

    //=======================================================//
    //================ Fase 1: Pacote Fechado ===============//
    //=======================================================//

    }else if(tela.fase == 1){

        float brilho = (sinf(tempoGlobal * 5.0f) + 1.0f) / 2.0f;
        Rectangle pacoteRec = {380, 130, 240, 450};

        DrawRectangleLinesEx((Rectangle){pacoteRec.x - 4, pacoteRec.y - 4, pacoteRec.width + 8, pacoteRec.height + 8}, 2.0f, Fade(COPA_OURO_PURO, 0.2f + (brilho * 0.4f)));
        DrawRectangleRec(pacoteRec, COPA_AZUL_MEDIO);
        DrawRectangleLinesEx(pacoteRec, 3.0f, COPA_OURO_PURO);
        DrawRectangle(395, 145, 210, 420, COPA_AZUL_ESCURO);

        float linhaMovimento = pacoteRec.y + 15 + ((float)sinf(tempoGlobal * 2.5f) + 1.0f) * 0.5f * 400.0f;
        DrawRectangleGradientH(pacoteRec.x + 15, linhaMovimento, 210, 3, Fade(COPA_VERDE_NEON, 0.0f), COPA_VERDE_NEON);

        DrawTextEx(fonteCopa, "FIFA OFICIAL", (Vector2){500 - MeasureTextEx(fonteCopa, "FIFA OFICIAL", 12, 1).x / 2, 200}, 12, 1, COPA_OURO_PURO);
        DrawTextEx(fonteCopa, "PACOTE\nFIGURINHA", (Vector2){500 - MeasureTextEx(fonteCopa, "PACOTE\nFIGURINHA", 22, 1).x / 2, 320}, 22, 1, WHITE);

        if (tela.modoTurbo) {
            DrawTextEx(fonteCopa, "!!! MODO TURBO ATIVADO !!!", (Vector2){500 - MeasureTextEx(fonteCopa, "!!! MODO TURBO ATIVADO !!!", 12, 1).x / 2, 400}, 12, 1, COPA_VERDE_NEON);
        }

        char apertar[50];
        strcpy(apertar, "APERTE [ ESPACO ] PARA ABRIR");
        DrawTextEx(fonteCopa, apertar, (Vector2){500 - MeasureTextEx(fonteCopa, apertar, 14, 1).x / 2, 620}, 14, 1, Fade(COPA_VERDE_NEON, 0.6f + (brilho * 0.4f)));

        if(IsKeyPressed(KEY_SPACE)){
            tela.fase = 2;
            tela.contadorFrames = 0;
        }

    //=======================================================//
    //================== Fase 2: Animacao GIF ===============//
    //=======================================================//

    }else if(tela.fase == 2){

        tela.frameMomento++;

        if(tela.frameMomento >= 6){
            tela.contadorFrames++;

            if(tela.contadorFrames >= tela.animFrames){
                tela.fase = 3;
                tela.contadorFrames = 0;
                tela.codigoFotoCarregada[0] = '\0';
            }else{
                UpdateTexture(tela.gifTextura, ((unsigned char *)tela.gifImage.data) + (tela.gifImage.width * tela.gifImage.height * 4 * tela.contadorFrames));
            }
            tela.frameMomento = 0;
        }

        float brilho = (sinf(tempoGlobal * 8.0f) + 1.0f) / 2.0f;
        DrawCircle(500, 400, 240.0f + (brilho * 15.0f), Fade(COPA_OURO_PURO, 0.15f));

        Rectangle origem = {0.0f, 0.0f, (float)tela.gifTextura.width, (float)tela.gifTextura.height};
        Rectangle destino = {375.0f, 190.0f, 250.0f, 420.0f};
        Vector2 centro = {0.0f, 0.0f};

        DrawTexturePro(tela.gifTextura, origem, destino, centro, 0.0f, WHITE);

    //=======================================================//
    //=============== Fase 3: Carrossel Cartas ==============//
    //=======================================================//

    }else if(tela.fase == 3){

        Figurinha fig = tela.pacoteSorteado[tela.cartaAtualIndice];
        carregarTexturaCarta(fig);

        Rectangle valoresFigurinha = {365, 100, 270, 520};

        float inclinacaoHorizontalMouse = ((mousePoint.x - 500) / 500.0f) * 15.0f;
        float inclinacaoVerticalMouse = ((mousePoint.y - 360) / 360.0f) * 12.0f;

        valoresFigurinha.x += inclinacaoHorizontalMouse;
        valoresFigurinha.y += inclinacaoVerticalMouse + (sinf(tempoGlobal * 2.0f) * 3.0f);

        DrawRectangleRec((Rectangle){valoresFigurinha.x + 12, valoresFigurinha.y + 12, valoresFigurinha.width, valoresFigurinha.height}, Fade(BLACK, 0.4f));
        DrawRectangleRec(valoresFigurinha, COPA_CINZA_CARD);

        bool ehEspecial = (strcmp(fig.tipo, "Especial") == 0 || strcmp(fig.tipo, "especial") == 0);
        Color corBorda = COPA_VERDE_NEON;

        if(tela.statusCartas[tela.cartaAtualIndice] == 1){
            if(ehEspecial){
                float brilhoEspecial = (sinf(tempoGlobal * 3.0f) + 1.0f) * 0.5f;
                corBorda = ColorLerp(COPA_OURO_PURO, COPA_OURO_BRILHO, brilhoEspecial);
                DrawRectangleLinesEx((Rectangle){valoresFigurinha.x - 2, valoresFigurinha.y - 2, valoresFigurinha.width + 4, valoresFigurinha.height + 4}, 2.0f, corBorda);
            }else{
                corBorda = Fade(COPA_VERDE_NEON, 0.7f);
                DrawRectangleLinesEx(valoresFigurinha, 2.0f, corBorda);
            }
        }else{
            corBorda = Fade(WHITE, 0.2f);
            DrawRectangleLinesEx(valoresFigurinha, 2.0f, corBorda);
        }

        if(tela.texturaCartaAtual.id > 0){
            DrawTexture(tela.texturaCartaAtual, valoresFigurinha.x + 30, valoresFigurinha.y + 60, WHITE);
            DrawRectangleLinesEx((Rectangle){valoresFigurinha.x + 29, valoresFigurinha.y + 59, 212, 292}, 1.5f, Fade(WHITE, 0.1f));

            float reflexoX = valoresFigurinha.x + 30 + ((mousePoint.x / 1000.0f) * 210.0f);
            DrawLineEx((Vector2){reflexoX, valoresFigurinha.y + 60}, (Vector2){reflexoX - 30, valoresFigurinha.y + 350}, 3.0f, Fade(WHITE, 0.15f));
        }else{
            DrawRectangle(valoresFigurinha.x + 30, valoresFigurinha.y + 60, 210, 290, COPA_AZUL_ESCURO);
            DrawRectangleLinesEx((Rectangle){valoresFigurinha.x + 30, valoresFigurinha.y + 60, 210, 290}, 1.0f, Fade(WHITE, 0.04f));
        }

        DrawRectangle(valoresFigurinha.x + 20, valoresFigurinha.y + 18, 80, 24, COPA_AZUL_MEDIO);
        DrawRectangleLinesEx((Rectangle){valoresFigurinha.x + 20, valoresFigurinha.y + 18, 80, 24}, 1.0f, corBorda);

        Color corTextoCodigo = ehEspecial ? COPA_OURO_PURO : WHITE;
        DrawTextEx(fonteCopa, fig.codigo, (Vector2){(valoresFigurinha.x + 60) - MeasureTextEx(fonteCopa, fig.codigo, 11, 1).x / 2, valoresFigurinha.y + 24}, 11, 1, corTextoCodigo);
        DrawTextEx(fonteCopa, fig.titulo, (Vector2){(valoresFigurinha.x + 135) - MeasureTextEx(fonteCopa, fig.titulo, 15, 1).x / 2, valoresFigurinha.y + 368}, 15, 1, WHITE);
        DrawTextEx(fonteCopa, fig.secao, (Vector2){(valoresFigurinha.x + 135) - MeasureTextEx(fonteCopa, fig.secao, 11, 1).x / 2, valoresFigurinha.y + 395}, 11, 1, Fade(WHITE, 0.4f));

        Color corTextoTipo = ehEspecial ? COPA_OURO_PURO : COPA_VERDE_NEON;
        DrawTextEx(fonteCopa, fig.tipo, (Vector2){(valoresFigurinha.x + 135) - MeasureTextEx(fonteCopa, fig.tipo, 11, 1).x / 2, valoresFigurinha.y + 418}, 11, 1, corTextoTipo);

        //Mensagem de figurinha nova ou repetida
        if(tela.statusCartas[tela.cartaAtualIndice] == 1){
            DrawRectangle(valoresFigurinha.x + 15, valoresFigurinha.y + 445, 240, 60, Fade(COPA_VERDE_NEON, 0.15f));
            DrawRectangleLinesEx((Rectangle){valoresFigurinha.x + 15, valoresFigurinha.y + 445, 240, 60}, 1.0f, COPA_VERDE_NEON);
            DrawTextEx(fonteCopa, "NOVA FIGURINHA!", (Vector2){(valoresFigurinha.x + 135) - MeasureTextEx(fonteCopa, "NOVA FIGURINHA!", 12, 1).x / 2, valoresFigurinha.y + 458}, 12, 1, COPA_VERDE_NEON);
            DrawTextEx(fonteCopa, "ADICIONADA AO ALBUM", (Vector2){(valoresFigurinha.x + 135) - MeasureTextEx(fonteCopa, "ADICIONADA AO ALBUM", 10, 1).x / 2, valoresFigurinha.y + 480}, 10, 1, WHITE);
        }else{
            DrawRectangle(valoresFigurinha.x + 15, valoresFigurinha.y + 445, 240, 60, Fade(COPA_AZUL_MEDIO, 0.6f));
            DrawRectangleLinesEx((Rectangle){valoresFigurinha.x + 15, valoresFigurinha.y + 445, 240, 60}, 1.0f, Fade(WHITE, 0.3f));
            DrawTextEx(fonteCopa, "REPETIDA", (Vector2){(valoresFigurinha.x + 135) - MeasureTextEx(fonteCopa, "REPETIDA", 12, 1).x / 2, valoresFigurinha.y + 458}, 12, 1, Fade(WHITE, 0.7f));
            DrawTextEx(fonteCopa, "ENVIADA PARA A MOCHILA", (Vector2){(valoresFigurinha.x + 135) - MeasureTextEx(fonteCopa, "ENVIADA PARA A MOCHILA", 10, 1).x / 2, valoresFigurinha.y + 480}, 10, 1, Fade(WHITE, 0.5f));
        }

        if(tela.cartaAtualIndice > 0){
            DrawTextEx(fonteCopa, "<", (Vector2){290, 350}, 26, 1, Fade(WHITE, 0.3f));
        }

        if(tela.cartaAtualIndice < tela.totalCartasSorteioAtual - 1){
            DrawTextEx(fonteCopa, ">", (Vector2){690, 350}, 26, 1, Fade(WHITE, 0.3f));
        }

        char indicador[50];
        sprintf(indicador, "FIGURINHA: %d / %d", tela.cartaAtualIndice + 1, tela.totalCartasSorteioAtual);
        DrawTextEx(fonteCopa, indicador, (Vector2){500 - MeasureTextEx(fonteCopa, indicador, 13, 1).x / 2, 640}, 13, 1, Fade(WHITE, 0.5f));

        if(IsKeyPressed(KEY_RIGHT) && tela.cartaAtualIndice < tela.totalCartasSorteioAtual - 1){
            tela.cartaAtualIndice++;
        }
        if(IsKeyPressed(KEY_LEFT) && tela.cartaAtualIndice > 0){
            tela.cartaAtualIndice--;
        }

        float efeitoEnter = (sinf(tempoGlobal * 6.0f) + 1.0f) / 2.0f;
        DrawTextEx(fonteCopa, "APERTE [ ENTER ] PARA CONTINUAR", (Vector2){500 - MeasureTextEx(fonteCopa, "APERTE [ ENTER ] PARA CONTINUAR", 13, 1).x / 2, 675}, 13, 1, Fade(COPA_OURO_PURO, 0.5f + efeitoEnter * 0.5f));

        if(IsKeyPressed(KEY_ENTER)){
            if (tela.modoTurbo == false) {
                // Modo Normal: Abre de 1 em 1 pacotinho até a quantidade zerar
                tela.quantidadeDesejada--;
                if(tela.quantidadeDesejada > 0 && pacotes_fechados > 0){
                    pacotes_fechados--;
                    salvarPacotes();
                    sortearFigurinhasDoPacote(figurinhas, mochila, album, total, total_mochila, total_album, 1);
                    tela.cartaAtualIndice = 0;
                    tela.fase = 1;
                }else{
                    descarregarRecursosAbertura();
                    tela.fase = 0;
                    tela.quantidadeDesejada = 1;
                    *estadoAtual = MENU_PRINCIPAL;
                }
            } else {
                // Modo Turbo: Todos os pacotes já foram subtraídos, apenas finaliza
                descarregarRecursosAbertura();
                tela.fase = 0;
                tela.quantidadeDesejada = 1;
                *estadoAtual = MENU_PRINCIPAL;
            }
        }
    }
}