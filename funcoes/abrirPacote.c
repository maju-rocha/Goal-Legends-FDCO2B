#include "raylib.h"
#include "raymath.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "biblioteca.h"
#include "global.h"
#include "abrirPacote.h"
#include "salvarPacotes.h"
#include "salvarRepetida.h"
#include "salvarAlbum.h"
#include "salvarMochila.h"

//=======================================================//
//==================== Paleta de Cores ==================//
//=======================================================//

#define COPA_AZUL_ESCURO (Color){5, 7, 15, 255}
#define COPA_AZUL_MEDIO  (Color){14, 20, 38, 255}
#define COPA_VERDE_NEON  (Color){0, 255, 140, 255}
#define COPA_VERDE_GRAMADO (Color){15, 55, 25, 255}
#define COPA_VERDE_GRAMA_L (Color){22, 75, 35, 255} // Verde claro para as faixas do campo
#define COPA_VERMELHO    (Color){240, 30, 70, 255}
#define COPA_OURO_PURO   (Color){255, 185, 0, 255}
#define COPA_OURO_BRILHO (Color){255, 240, 150, 255}
#define COPA_CINZA_CARD  (Color){22, 28, 48, 255}

#define QUANTIDADE_MAXIMA_PARTICULA 60
#define MAX_CARTAS_SORTEADAS 700 // Suporta até 100 pacotes simultâneos no Modo Turbo

//=======================================================//
//======================= Structs =======================//
//=======================================================//

typedef struct{ // Estrutura para representar partículas de confete
    Vector2 posicao;
    Vector2 velocidade;
    float tamanhoBase;
    float randomizadorPosicao;
    float transparenciaParticula;
    float velocidadeTransparenciaParticula;
    Color cor;
} Particula; 

typedef struct{ // Estrutura para gerenciar a tela de abertura de pacotes
    int quantidadeDesejada;
    int fase;
    int cartaAtualIndice;
    int totalCartasSorteioAtual; // Dinamico: 7 no normal, 7 * quantidade no turbo
    bool modoTurbo;              // false = abre um pacote por vez; true = mostra todos de uma vez

    int statusCartas[MAX_CARTAS_SORTEADAS];
    char statusMensagem[50];
    char codigoFotoCarregada[30];

    bool particulasInicializadas;
    Texture2D texturaCartaAtual;
    Figurinha pacoteSorteado[MAX_CARTAS_SORTEADAS];
    Particula particulas[QUANTIDADE_MAXIMA_PARTICULA];

    float tempoAnimacaoRasgar;
    float flashIntensidade;

    // Recursos do GIF Animado do Pacote
    Image gifImage;
    Texture2D gifTextura;
    int animFrames;
    int frameAtual;
    float tempoFrame;
    unsigned int decolagemMemoriaOffset;
    bool gifCarregado;
} AberturaPacotes;

static AberturaPacotes tela = {0}; // Inicializa a estrutura da tela de abertura de pacotes com valores padrão

//=======================================================//
//================== Funcoes Auxiliares =================//
//=======================================================//

void numeroParaTexto(int numero, char *destino){ 
    char invertido[20];
    int i = 0, j = 0;
    if (numero == 0) { destino[0] = '0'; destino[1] = '\0'; return; }
    if (numero < 0) { destino[j] = '-'; j++; numero *= -1; }
    while (numero > 0) { invertido[i] = (numero % 10) + '0'; numero /= 10; i++; }
    while (i > 0) { i--; destino[j] = invertido[i]; j++; }
    destino[j] = '\0'; 
}

void numeroParaTextoComDoisDigitos(int numero, char *destino) { 
    if (numero >= 0 && numero < 10) {
        destino[0] = '0';
        numeroParaTexto(numero, destino + 1);
    } else {
        numeroParaTexto(numero, destino);
    }
}

static void descarregarRecursosAbertura() { 
    if (tela.texturaCartaAtual.id > 0) {
        UnloadTexture(tela.texturaCartaAtual);
        tela.texturaCartaAtual.id = 0;
    }
    tela.codigoFotoCarregada[0] = '\0';
    
    if (tela.gifCarregado) {
        UnloadTexture(tela.gifTextura);
        UnloadImage(tela.gifImage);
        tela.gifCarregado = false;
    }
}

static void sortearFigurinhasDoPacote(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, int qtdPacotes) {
    tela.totalCartasSorteioAtual = qtdPacotes * 7;

    if (tela.totalCartasSorteioAtual > MAX_CARTAS_SORTEADAS) {
        tela.totalCartasSorteioAtual = MAX_CARTAS_SORTEADAS;
    }

    for (int f = 0; f < tela.totalCartasSorteioAtual; f++) {
        Figurinha sorteada = figurinhas[rand() % total];
        limparFigurinha(&sorteada);
        tela.pacoteSorteado[f] = sorteada;

        if (!estaNoAlbum(album, *total_album, sorteada.codigo)) {
            album[*total_album] = sorteada;
            (*total_album)++;

            salvarAlbum(album, *total_album);

            tela.statusCartas[f] = 1;
        } else {
            mochila[*total_mochila] = sorteada;
            (*total_mochila)++;

            figurinha_repetida++;
            salvarMochila(mochila, *total_mochila);
            salvarRepetida();

            tela.statusCartas[f] = 2;
        }
    }
}

static void spawnParticulas() {
    for (int i = 0; i < QUANTIDADE_MAXIMA_PARTICULA; i++) {
        tela.particulas[i].posicao = (Vector2){(float)GetRandomValue(0, 1000), (float)GetRandomValue(0, 800)};
        tela.particulas[i].velocidade = (Vector2){(float)GetRandomValue(-8, 8) / 10.0f, (float)GetRandomValue(-15, -5) / 10.0f};
        tela.particulas[i].tamanhoBase = (float)GetRandomValue(2, 5);
        tela.particulas[i].randomizadorPosicao = (float)GetRandomValue(0, 360);
        tela.particulas[i].transparenciaParticula = (float)GetRandomValue(30, 90) / 100.0f;
        tela.particulas[i].velocidadeTransparenciaParticula = (float)GetRandomValue(5, 15) / 1000.0f;
        tela.particulas[i].cor = (GetRandomValue(0, 1) == 0) ? COPA_OURO_PURO : COPA_VERDE_NEON;
    }
    tela.particulasInicializadas = true;
}

static void efeitoParticulas(Vector2 mousePos, float tempoGlobal) {
    if (!tela.particulasInicializadas) spawnParticulas();

    for (int i = 0; i < QUANTIDADE_MAXIMA_PARTICULA; i++) {
        tela.particulas[i].posicao.x += tela.particulas[i].velocidade.x + sinf(tempoGlobal * 2.0f + tela.particulas[i].randomizadorPosicao) * 0.25f;
        tela.particulas[i].posicao.y += tela.particulas[i].velocidade.y;
        tela.particulas[i].transparenciaParticula -= tela.particulas[i].velocidadeTransparenciaParticula;

        float distAoMouse = Vector2Distance(mousePos, tela.particulas[i].posicao);
        if (distAoMouse < 100.0f) {
            tela.particulas[i].posicao = Vector2Lerp(tela.particulas[i].posicao, mousePos, 0.01f);
        }

        if (tela.particulas[i].transparenciaParticula <= 0.0f || tela.particulas[i].posicao.y < -10 || tela.particulas[i].posicao.x < -10 || tela.particulas[i].posicao.x > 1010) {
            tela.particulas[i].posicao = (Vector2){(float)GetRandomValue(0, 1000), 810.0f};
            tela.particulas[i].transparenciaParticula = (float)GetRandomValue(50, 95) / 100.0f;
        }

        float tamPulso = tela.particulas[i].tamanhoBase * (1.0f + sinf(tempoGlobal * 4.0f + tela.particulas[i].randomizadorPosicao) * 0.2f);
        DrawCircleV(tela.particulas[i].posicao, tamPulso, Fade(tela.particulas[i].cor, tela.particulas[i].transparenciaParticula * 0.5f));
    }
}


static void limparEspacosImagem(char *texto) {
    int inicio = 0;
    int fim = strlen(texto) - 1;

    while (texto[inicio] == ' ' || texto[inicio] == '\t') {
        inicio++;
    }

    while (fim >= inicio && (texto[fim] == ' ' || texto[fim] == '\t' || texto[fim] == '\n' || texto[fim] == '\r')) {
        texto[fim] = '\0';
        fim--;
    }

    if (inicio > 0) {
        int j = 0;

        for (int i = inicio; texto[i] != '\0'; i++) {
            texto[j] = texto[i];
            j++;
        }

        texto[j] = '\0';
    }
}

static void removerEspacosInternosImagem(char *texto) {
    int j = 0;

    for (int i = 0; texto[i] != '\0'; i++) {
        if (texto[i] != ' ' && texto[i] != '\t') {
            texto[j] = texto[i];
            j++;
        }
    }

    texto[j] = '\0';
}

static void trocarEspacoPorUnderlineImagem(char *texto) {
    for (int i = 0; texto[i] != '\0'; i++) {
        if (texto[i] == ' ') {
            texto[i] = '_';
        }
    }
}

static void copiarMaiusculoImagem(char *destino, const char *origem) {
    int i = 0;

    while (origem[i] != '\0') {
        char c = origem[i];

        if (c >= 'a' && c <= 'z') {
            c = c - 32;
        }

        destino[i] = c;
        i++;
    }

    destino[i] = '\0';
}

static void copiarMinusculoImagem(char *destino, const char *origem) {
    int i = 0;

    while (origem[i] != '\0') {
        char c = origem[i];

        if (c >= 'A' && c <= 'Z') {
            c = c + 32;
        }

        destino[i] = c;
        i++;
    }

    destino[i] = '\0';
}

static int comecaComImagem(const char *texto, const char *prefixo) {
    int i = 0;

    while (prefixo[i] != '\0') {
        if (texto[i] != prefixo[i]) {
            return 0;
        }

        i++;
    }

    return 1;
}

static void pegarSufixoNumericoImagem(const char *codigo, char *sufixo) {
    int i = 0;

    while (codigo[i] != '\0' && (codigo[i] < '0' || codigo[i] > '9')) {
        i++;
    }

    strcpy(sufixo, codigo + i);
}

static void montarCodigoComPrefixoImagem(char *destino, const char *prefixo, const char *sufixo) {
    strcpy(destino, prefixo);
    strcat(destino, sufixo);
}

static void adicionarPastaImagem(char pastas[][100], int *totalPastas, const char *pasta) {
    if (pasta[0] == '\0') {
        return;
    }

    for (int i = 0; i < *totalPastas; i++) {
        if (strcmp(pastas[i], pasta) == 0) {
            return;
        }
    }

    if (*totalPastas < 40) {
        strcpy(pastas[*totalPastas], pasta);
        (*totalPastas)++;
    }
}

static void adicionarCodigoImagem(char codigos[][30], int *totalCodigos, const char *codigo) {
    if (codigo[0] == '\0') {
        return;
    }

    for (int i = 0; i < *totalCodigos; i++) {
        if (strcmp(codigos[i], codigo) == 0) {
            return;
        }
    }

    if (*totalCodigos < 40) {
        strcpy(codigos[*totalCodigos], codigo);
        (*totalCodigos)++;
    }
}

static int tentarCaminhoImagemFigurinha(char *caminho, const char *pasta, const char *codigo) {
    strcpy(caminho, "imagens/imagens_figurinhas/");
    strcat(caminho, pasta);
    strcat(caminho, "/");
    strcat(caminho, codigo);
    strcat(caminho, ".png");

    if (FileExists(caminho)) {
        return 1;
    }

    return 0;
}

static int montarCaminhoImagemFigurinha(char *caminho, const char *secao, const char *codigo) {
    char pastaOriginal[100];
    char pastaUnderline[100];
    char pastaMaiuscula[100];
    char codigoOriginal[30];
    char codigoSemEspaco[30];
    char codigoMaiusculo[30];
    char codigoMinusculo[30];
    char sufixoNumero[30];
    char temporarioCodigo[30];
    char pastas[40][100];
    char codigos[40][30];
    int totalPastas = 0;
    int totalCodigos = 0;

    strcpy(pastaOriginal, secao);
    strcpy(codigoOriginal, codigo);

    limparEspacosImagem(pastaOriginal);
    limparEspacosImagem(codigoOriginal);

    strcpy(pastaUnderline, pastaOriginal);
    trocarEspacoPorUnderlineImagem(pastaUnderline);

    strcpy(codigoSemEspaco, codigoOriginal);
    removerEspacosInternosImagem(codigoSemEspaco);

    copiarMaiusculoImagem(pastaMaiuscula, pastaUnderline);
    copiarMaiusculoImagem(codigoMaiusculo, codigoSemEspaco);
    copiarMinusculoImagem(codigoMinusculo, codigoSemEspaco);
    pegarSufixoNumericoImagem(codigoMaiusculo, sufixoNumero);

    adicionarPastaImagem(pastas, &totalPastas, pastaOriginal);
    adicionarPastaImagem(pastas, &totalPastas, pastaUnderline);

    adicionarCodigoImagem(codigos, &totalCodigos, codigoOriginal);
    adicionarCodigoImagem(codigos, &totalCodigos, codigoSemEspaco);
    adicionarCodigoImagem(codigos, &totalCodigos, codigoMaiusculo);
    adicionarCodigoImagem(codigos, &totalCodigos, codigoMinusculo);

    int ehCongo = 0;
    int ehEstadosUnidos = 0;
    int ehQatar = 0;
    int ehSuica = 0;
    int ehFifa = 0;

    if (strcmp(pastaUnderline, "Congo_DR") == 0) {
        ehCongo = 1;
    }

    if (strcmp(pastaUnderline, "Estados_Unidos") == 0 ) {
        ehEstadosUnidos = 1;
    }

    if (strcmp(pastaUnderline, "Qatar") == 0 ) {
        ehQatar = 1;
    }

    if (strcmp(pastaUnderline, "Suíça") == 0 ) {
        ehSuica = 1;
    }

    if (strcmp(pastaMaiuscula, "FIFA") == 0 ) {
        ehFifa = 1;
    }

    if (ehCongo) {
        adicionarPastaImagem(pastas, &totalPastas, "Congo_DR");
    }

    if (ehEstadosUnidos) {
        adicionarPastaImagem(pastas, &totalPastas, "EUA");
    }

    if (ehQatar) {
        adicionarPastaImagem(pastas, &totalPastas, "Catar");
    }

    if (ehSuica) {
        adicionarPastaImagem(pastas, &totalPastas, "Suiça");
    }

    if (ehFifa) {
        adicionarPastaImagem(pastas, &totalPastas, "FIFA_World_Cup_2026");
    }

    for (int i = 0; i < totalPastas; i++) {
        for (int j = 0; j < totalCodigos; j++) {
            if (tentarCaminhoImagemFigurinha(caminho, pastas[i], codigos[j])) {
                return 1;
            }
        }
    }

    return 0;
}

static void carregarTexturaCarta(Figurinha fig) {
    if (strcmp(tela.codigoFotoCarregada, fig.codigo) == 0) return;

    if (tela.texturaCartaAtual.id > 0) {
        UnloadTexture(tela.texturaCartaAtual);
        tela.texturaCartaAtual.id = 0;
    }

    char caminhoFoto[256];

    if (montarCaminhoImagemFigurinha(caminhoFoto, fig.secao, fig.codigo)) {
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
    } else {
        TraceLog(LOG_ERROR, "!!! IMAGEM NAO ENCONTRADA: %s !!!", caminhoFoto);
        tela.texturaCartaAtual.id = 0;
        tela.codigoFotoCarregada[0] = '\0';
    }
}

static void sombraTela() {
    DrawRectangleGradientV(0, 0, 1000, 120, Fade(BLACK, 0.7f), Fade(BLACK, 0.0f));
    DrawRectangleGradientV(0, 680, 1000, 120, Fade(BLACK, 0.0f), Fade(BLACK, 0.8f));
}

//=======================================================//
//============= MELHORIA DE DECORAÇÃO FUTEBOL ============//
//=======================================================//

static void DesenharDecoracoesFutebol(float tempoGlobal) {
    // 1. Gramado listrado em perspectiva de estádio de fundo
    for (int i = 0; i < 16; i++) {
        Color corGrama = (i % 2 == 0) ? COPA_VERDE_GRAMADO : COPA_VERDE_GRAMA_L;
        DrawRectangle(0, i * 50, 1000, 50, corGrama);
    }

    Color corLinhas = Fade(WHITE, 0.15f);

    // 2. Linha sutil de grande área e rede de gol estilizada ao fundo
    for(int x = 100; x < 900; x += 40) {
        DrawLineEx((Vector2){(float)x, 150}, (Vector2){(float)x + 20, 650}, 1.0f, Fade(WHITE, 0.04f));
    }
    for(int y = 150; y < 650; y += 40) {
        DrawLineEx((Vector2){100, (float)y}, (Vector2){900, (float)y}, 1.0f, Fade(WHITE, 0.04f));
    }

    // Marcações do Gramado Clássicas
    DrawCircleLines(500, 400, 160, corLinhas);
    DrawCircle(500, 400, 5, corLinhas);
    DrawLineEx((Vector2){500, 0}, (Vector2){500, 800}, 2.5f, corLinhas);
    DrawRectangleLinesEx((Rectangle){150, 180, 700, 440}, 2.0f, corLinhas);

    // Confetes da torcida voando
    for (int i = 0; i < 30; i++) {
        float x = fmodf((float)(i * 45) + tempoGlobal * 20.0f, 1000.0f);
        float y = fmodf((float)(i * 35) + tempoGlobal * 50.0f, 800.0f);
        float rot = tempoGlobal * (15 + i);
        DrawRectanglePro((Rectangle){x, y, 5, 8}, (Vector2){2.5f, 4}, rot, Fade((i % 2 == 0) ? COPA_OURO_PURO : COPA_VERDE_NEON, 0.3f));
    }
}

static void DesenharMetadePacote(Texture2D textura, Rectangle dest, bool superior, float offsetRasgo) {
    Rectangle origem = { 0.0f, 0.0f, (float)textura.width, (float)textura.height / 2.0f };
    if (!superior) origem.y = (float)textura.height / 2.0f;

    Rectangle destino = dest;
    destino.height /= 2.0f;
    
    if (superior) destino.y -= offsetRasgo;
    else destino.y += dest.height / 2.0f + offsetRasgo;

    DrawTexturePro(textura, origem, destino, (Vector2){0,0}, 0.0f, WHITE);
}

//=======================================================//
//================== Tela Abrir Pacote ==================//
//=======================================================//

void abrirPacote(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, Font fonteCopa, Color azulBrasil, Color amareloBrasil, EstadoMenu *estadoAtual) {
    (void)azulBrasil;
    (void)amareloBrasil;
    Vector2 mousePoint = GetMousePosition();
    float tempoGlobal = (float)GetTime();

    // Lógica para carregar e atualizar o GIF Animado do pacote em tempo real
    if (!tela.gifCarregado) {
        tela.gifImage = LoadImageAnim("imagens/animacao.gif", &tela.animFrames);
        if (tela.gifImage.data != NULL) {
            tela.gifTextura = LoadTextureFromImage(tela.gifImage);
            tela.frameAtual = 0;
            tela.tempoFrame = 0.0f;
            tela.gifCarregado = true;
        }
    } else if (tela.fase == 1 && tela.animFrames > 0) {
        // Incrementa o tempo para rodar os frames do GIF na Fase 1
        tela.tempoFrame += GetFrameTime();
        if (tela.tempoFrame >= 0.08f) { // Velocidade da animação do pacote se mexendo
            tela.tempoFrame = 0.0f;
            tela.frameAtual = (tela.frameAtual + 1) % tela.animFrames;
            
            // Calcula o salto de memória para pegar o frame correto dentro do arquivo GIF original
            int pixelOffset = tela.frameAtual * tela.gifImage.width * tela.gifImage.height * 4;
            UpdateTexture(tela.gifTextura, ((unsigned char *)tela.gifImage.data) + pixelOffset);
        }
    }

    // Desenha o fundo temático de estádio de futebol reformulado
    DesenharDecoracoesFutebol(tempoGlobal);
    efeitoParticulas(mousePoint, tempoGlobal);
    sombraTela();

    //=======================================================//
    //================ Fase 0: Escolher Pacotes =============//
    //=======================================================//
    if (tela.fase == 0) {
        int limitePacotesTurbo = MAX_CARTAS_SORTEADAS / 7;

        if (tela.quantidadeDesejada < 1) tela.quantidadeDesejada = 1;
        if (tela.quantidadeDesejada > pacotes_fechados && pacotes_fechados > 0) tela.quantidadeDesejada = pacotes_fechados;
        if (tela.quantidadeDesejada > limitePacotesTurbo) tela.quantidadeDesejada = limitePacotesTurbo;
        if (pacotes_fechados == 0) tela.quantidadeDesejada = 0;

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

        // Layout redistribuido para caber o botao MAX
        Rectangle btnMenos = {290, 320, 60, 60};
        Rectangle boxQtd = {370, 320, 160, 60};
        Rectangle btnMais = {550, 320, 60, 60};
        Rectangle btnMax = {630, 320, 80, 60};

        bool seMenos = CheckCollisionPointRec(mousePoint, btnMenos);
        bool seMais = CheckCollisionPointRec(mousePoint, btnMais);
        bool seMax = CheckCollisionPointRec(mousePoint, btnMax);

        DrawRectangleRec(btnMenos, seMenos ? Fade(COPA_VERMELHO, 0.2f) : Fade(COPA_AZUL_ESCURO, 0.6f));
        DrawRectangleLinesEx(btnMenos, seMenos ? 2.0f : 1.0f, seMenos ? COPA_VERMELHO : COPA_OURO_PURO);
        DrawTextEx(fonteCopa, "-", (Vector2){btnMenos.x + 30 - MeasureTextEx(fonteCopa, "-", 24, 1).x / 2, btnMenos.y + 18}, 24, 1, WHITE);

        DrawRectangleRec(boxQtd, Fade(BLACK, 0.4f));
        DrawRectangleLinesEx(boxQtd, 1.0f, Fade(WHITE, 0.2f));

        char txtQtd[10];
        numeroParaTextoComDoisDigitos(tela.quantidadeDesejada, txtQtd);
        DrawTextEx(fonteCopa, txtQtd, (Vector2){boxQtd.x + 80 - MeasureTextEx(fonteCopa, txtQtd, 32, 2).x / 2, boxQtd.y + 14}, 32, 2, COPA_VERDE_NEON);

        DrawRectangleRec(btnMais, seMais ? Fade(COPA_VERDE_NEON, 0.2f) : Fade(COPA_AZUL_ESCURO, 0.6f));
        DrawRectangleLinesEx(btnMais, seMais ? 2.0f : 1.0f, seMais ? COPA_VERDE_NEON : COPA_OURO_PURO);
        DrawTextEx(fonteCopa, "+", (Vector2){btnMais.x + 30 - MeasureTextEx(fonteCopa, "+", 24, 1).x / 2, btnMais.y + 18}, 24, 1, WHITE);

        DrawRectangleRec(btnMax, seMax ? Fade(COPA_OURO_PURO, 0.2f) : Fade(COPA_AZUL_ESCURO, 0.6f));
        DrawRectangleLinesEx(btnMax, seMax ? 2.0f : 1.0f, COPA_OURO_PURO);
        DrawTextEx(fonteCopa, "MAX", (Vector2){btnMax.x + 40 - MeasureTextEx(fonteCopa, "MAX", 16, 1).x / 2, btnMax.y + 22}, 16, 1, seMax ? COPA_OURO_PURO : WHITE);

        if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            if (seMenos && tela.quantidadeDesejada > 1) tela.quantidadeDesejada--;
            if (seMais && tela.quantidadeDesejada < pacotes_fechados && tela.quantidadeDesejada < limitePacotesTurbo) tela.quantidadeDesejada++;

            if (seMax && pacotes_fechados > 0) {
                tela.quantidadeDesejada = pacotes_fechados;

                if (tela.quantidadeDesejada > limitePacotesTurbo) {
                    tela.quantidadeDesejada = limitePacotesTurbo;
                }
            }
        }

        // Botao Abrir Normal: abre um pacote, mostra as 7 figurinhas e depois continua de um em um
        const char *textoAbrirNormal = "ABRIR (1 POR VEZ)";
        float larguraTextoAbrirNormal = MeasureTextEx(fonteCopa, textoAbrirNormal, 14, 1).x;
        Rectangle btnAbrirNormal = {500 - (larguraTextoAbrirNormal / 2) - 20, 405, larguraTextoAbrirNormal + 40, 38};
        bool seAbrirNormal = CheckCollisionPointRec(mousePoint, btnAbrirNormal);

        DrawRectangleRec(btnAbrirNormal, seAbrirNormal ? Fade(COPA_OURO_PURO, 0.2f) : Fade(COPA_OURO_PURO, 0.05f));
        DrawRectangleLinesEx(btnAbrirNormal, seAbrirNormal ? 2.0f : 1.0f, COPA_OURO_PURO);
        DrawTextEx(fonteCopa, textoAbrirNormal, (Vector2){btnAbrirNormal.x + 20, btnAbrirNormal.y + 12}, 14, 1, seAbrirNormal ? COPA_OURO_PURO : WHITE);

        if (seAbrirNormal && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            if (pacotes_fechados > 0 && tela.quantidadeDesejada > 0) {
                tela.modoTurbo = false;
                pacotes_fechados--;
                salvarPacotes();
                sortearFigurinhasDoPacote(figurinhas, mochila, album, total, total_mochila, total_album, 1);

                tela.cartaAtualIndice = 0;
                tela.fase = 1;
                strcpy(tela.statusMensagem, "");
            } else {
                strcpy(tela.statusMensagem, "PACOTES INSUFICIENTES!");
            }
        }

        // Botao Abrir Turbo: sorteia todos os pacotes escolhidos e mostra tudo em um unico carrossel
        const char *textoAbrirTurbo = "ABRIR MODO TURBO (TODOS)";
        float larguraTextoAbrirTurbo = MeasureTextEx(fonteCopa, textoAbrirTurbo, 14, 1).x;
        Rectangle btnAbrirTurbo = {500 - (larguraTextoAbrirTurbo / 2) - 20, 465, larguraTextoAbrirTurbo + 40, 38};
        bool seAbrirTurbo = CheckCollisionPointRec(mousePoint, btnAbrirTurbo);

        DrawRectangleRec(btnAbrirTurbo, seAbrirTurbo ? Fade(COPA_VERDE_NEON, 0.2f) : Fade(COPA_AZUL_ESCURO, 0.6f));
        DrawRectangleLinesEx(btnAbrirTurbo, seAbrirTurbo ? 2.0f : 1.0f, seAbrirTurbo ? COPA_VERDE_NEON : COPA_OURO_PURO);
        DrawTextEx(fonteCopa, textoAbrirTurbo, (Vector2){btnAbrirTurbo.x + 20, btnAbrirTurbo.y + 12}, 14, 1, seAbrirTurbo ? COPA_VERDE_NEON : WHITE);

        if (seAbrirTurbo && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            if (pacotes_fechados > 0 && tela.quantidadeDesejada > 0) {
                int pacotesParaAbrir = tela.quantidadeDesejada;

                if (pacotesParaAbrir > limitePacotesTurbo) {
                    pacotesParaAbrir = limitePacotesTurbo;
                }

                tela.modoTurbo = true;
                pacotes_fechados -= pacotesParaAbrir;
                salvarPacotes();
                sortearFigurinhasDoPacote(figurinhas, mochila, album, total, total_mochila, total_album, pacotesParaAbrir);

                tela.quantidadeDesejada = 0;
                tela.cartaAtualIndice = 0;
                tela.fase = 1;
                strcpy(tela.statusMensagem, "");
            } else {
                strcpy(tela.statusMensagem, "PACOTES INSUFICIENTES!");
            }
        }

        const char *textoVoltar = "VOLTAR AO MENU";
        float larguraTextoVoltar = MeasureTextEx(fonteCopa, textoVoltar, 12, 1).x;
        Rectangle btnVoltar = {500 - (larguraTextoVoltar / 2) - 20, 525, larguraTextoVoltar + 40, 32};

        bool seVoltar = CheckCollisionPointRec(mousePoint, btnVoltar);
        DrawRectangleRec(btnVoltar, seVoltar ? Fade(COPA_VERMELHO, 0.15f) : Fade(WHITE, 0.02f));
        DrawRectangleLinesEx(btnVoltar, 1.0f, seVoltar ? COPA_VERMELHO : Fade(WHITE, 0.2f));
        DrawTextEx(fonteCopa, textoVoltar, (Vector2){btnVoltar.x + 20, btnVoltar.y + 10}, 12, 1, seVoltar ? COPA_VERMELHO : Fade(WHITE, 0.6f));

        if (seVoltar && IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
            descarregarRecursosAbertura();
            tela.quantidadeDesejada = 1;
            tela.fase = 0;
            *estadoAtual = MENU_PRINCIPAL;
        }

        if (strlen(tela.statusMensagem) > 0) {
            DrawTextEx(fonteCopa, tela.statusMensagem, (Vector2){500 - MeasureTextEx(fonteCopa, tela.statusMensagem, 14, 1).x / 2, 120}, 14, 1, COPA_VERMELHO);
        }

        //=======================================================//
    //======= Fase 1: Pacote Funcionando (GIF ANIMADO) ======//
    //=======================================================//
    } else if (tela.fase == 1) {
        float brilho = (sinf(tempoGlobal * 5.0f) + 1.0f) / 2.0f;
        Rectangle pacoteRec = {380, 130, 240, 450};

        DrawRectangleLinesEx((Rectangle){pacoteRec.x - 4, pacoteRec.y - 4, pacoteRec.width + 8, pacoteRec.height + 8}, 2.0f, Fade(COPA_OURO_PURO, 0.2f + (brilho * 0.4f)));

        if (tela.gifCarregado) {
            // Desenha a textura atualizada frame-a-frame do seu GIF original
            Rectangle origemTextura = {0.0f, 0.0f, (float)tela.gifTextura.width, (float)tela.gifTextura.height};
            DrawTexturePro(tela.gifTextura, origemTextura, pacoteRec, (Vector2){0,0}, 0.0f, WHITE);
        } else {
            DrawRectangleRec(pacoteRec, COPA_AZUL_MEDIO);
            DrawText("PACOTE", pacoteRec.x + 80, pacoteRec.y + 200, 20, WHITE);
        }

        if (tela.modoTurbo) {
            DrawTextEx(fonteCopa, "!!! MODO TURBO ATIVADO !!!", (Vector2){500 - MeasureTextEx(fonteCopa, "!!! MODO TURBO ATIVADO !!!", 12, 1).x / 2, 590}, 12, 1, COPA_VERDE_NEON);
        }

        DrawTextEx(fonteCopa, "APERTE [ ESPACO ] PARA ABRIR", (Vector2){500 - MeasureTextEx(fonteCopa, "APERTE [ ESPACO ] PARA ABRIR", 14, 1).x / 2, 620}, 14, 1, Fade(COPA_VERDE_NEON, 0.6f + (brilho * 0.4f)));

        if (IsKeyPressed(KEY_SPACE)) {
            tela.fase = 2;
            tela.tempoAnimacaoRasgar = 0.0f;
            tela.flashIntensidade = 1.0f;
        }

    //=======================================================//
    //======== FASE 2: ANIMAÇÃO DE RASGAR SEU PACOTE ========//
    //=======================================================//
    } else if (tela.fase == 2) {
        tela.tempoAnimacaoRasgar += GetFrameTime();
        float offsetRasgo = powf(tela.tempoAnimacaoRasgar * 3.5f, 2.0f) * 100.0f;
        Rectangle pacoteRec = {380, 130, 240, 450};

        DrawCircleGradient(500, 350, tela.tempoAnimacaoRasgar * 800.0f, Fade(COPA_OURO_BRILHO, tela.flashIntensidade * 0.5f), Fade(COPA_OURO_PURO, 0.0f));

        if (tela.gifCarregado) {
            DesenharMetadePacote(tela.gifTextura, pacoteRec, true, offsetRasgo);
            DesenharMetadePacote(tela.gifTextura, pacoteRec, false, offsetRasgo);
        }

        if (tela.flashIntensidade > 0.0f) {
            DrawRectangle(0, 0, 1000, 800, Fade(WHITE, tela.flashIntensidade));
            tela.flashIntensidade -= GetFrameTime() * 1.5f;
        }

        if (tela.tempoAnimacaoRasgar >= 1.0f) {
            tela.fase = 3;
            tela.codigoFotoCarregada[0] = '\0';
        }

    //=======================================================//
    //======== Fase 3: Carrossel com Estádio e Placar =======//
    //=======================================================//
    } else if (tela.fase == 3) {
        // Placar Decorativo Superior do Evento
        DrawRectangle(250, 20, 500, 45, Fade(COPA_AZUL_MEDIO, 0.9f));
        DrawRectangleLinesEx((Rectangle){250, 20, 500, 45}, 1.5f, COPA_VERDE_NEON);
        DrawTextEx(fonteCopa, "GOAL LEGENDS - COPA DO MUNDO", (Vector2){500 - MeasureTextEx(fonteCopa, "GOAL LEGENDS - COPA DO MUNDO", 13, 1).x / 2, 33}, 13, 1, WHITE);

        Figurinha fig = tela.pacoteSorteado[tela.cartaAtualIndice];
        carregarTexturaCarta(fig);

        Rectangle valoresFigurinha = {365, 110, 270, 510};
        float inclinacaoHorizontalMouse = ((mousePoint.x - 500) / 500.0f) * 15.0f;
        float inclinacaoVerticalMouse = ((mousePoint.y - 360) / 360.0f) * 12.0f;

        valoresFigurinha.x += inclinacaoHorizontalMouse;
        valoresFigurinha.y += inclinacaoVerticalMouse + (sinf(tempoGlobal * 2.0f) * 3.0f);

        DrawRectangleRec((Rectangle){valoresFigurinha.x + 12, valoresFigurinha.y + 12, valoresFigurinha.width, valoresFigurinha.height}, Fade(BLACK, 0.4f));
        DrawRectangleRec(valoresFigurinha, COPA_CINZA_CARD);

        bool ehEspecial = (strcmp(fig.tipo, "Especial") == 0 || strcmp(fig.tipo, "especial") == 0);
        Color corBorda = COPA_VERDE_NEON;

        if (tela.statusCartas[tela.cartaAtualIndice] == 1) {
            if (ehEspecial) {
                float brilhoEspecial = (sinf(tempoGlobal * 3.0f) + 1.0f) * 0.5f;
                corBorda = ColorLerp(COPA_OURO_PURO, COPA_OURO_BRILHO, brilhoEspecial);
                DrawRectangleLinesEx((Rectangle){valoresFigurinha.x - 2, valoresFigurinha.y - 2, valoresFigurinha.width + 4, valoresFigurinha.height + 4}, 2.0f, corBorda);
            } else {
                corBorda = Fade(COPA_VERDE_NEON, 0.7f);
                DrawRectangleLinesEx(valoresFigurinha, 2.0f, corBorda);
            }
        } else {
            corBorda = Fade(WHITE, 0.2f);
            DrawRectangleLinesEx(valoresFigurinha, 2.0f, corBorda);
        }

        if (tela.texturaCartaAtual.id > 0) {
            DrawTexture(tela.texturaCartaAtual, valoresFigurinha.x + 30, valoresFigurinha.y + 55, WHITE);
            DrawRectangleLinesEx((Rectangle){valoresFigurinha.x + 29, valoresFigurinha.y + 54, 212, 292}, 1.5f, Fade(WHITE, 0.1f));

            float reflexoX = valoresFigurinha.x + 30 + ((mousePoint.x / 1000.0f) * 210.0f);
            DrawLineEx((Vector2){reflexoX, valoresFigurinha.y + 55}, (Vector2){reflexoX - 30, valoresFigurinha.y + 345}, 3.0f, Fade(WHITE, 0.15f));
        } else {
            DrawRectangle(valoresFigurinha.x + 30, valoresFigurinha.y + 55, 210, 290, COPA_AZUL_ESCURO);
            DrawRectangleLinesEx((Rectangle){valoresFigurinha.x + 30, valoresFigurinha.y + 55, 210, 290}, 1.0f, Fade(WHITE, 0.04f));
        }

        DrawRectangle(valoresFigurinha.x + 20, valoresFigurinha.y + 18, 80, 24, COPA_AZUL_MEDIO);
        DrawRectangleLinesEx((Rectangle){valoresFigurinha.x + 20, valoresFigurinha.y + 18, 80, 24}, 1.0f, corBorda);

        DrawTextEx(fonteCopa, fig.codigo, (Vector2){(valoresFigurinha.x + 60) - MeasureTextEx(fonteCopa, fig.codigo, 11, 1).x / 2, valoresFigurinha.y + 24}, 11, 1, ehEspecial ? COPA_OURO_PURO : WHITE);
        DrawTextEx(fonteCopa, fig.titulo, (Vector2){(valoresFigurinha.x + 135) - MeasureTextEx(fonteCopa, fig.titulo, 15, 1).x / 2, valoresFigurinha.y + 360}, 15, 1, WHITE);
        DrawTextEx(fonteCopa, fig.secao, (Vector2){(valoresFigurinha.x + 135) - MeasureTextEx(fonteCopa, fig.secao, 11, 1).x / 2, valoresFigurinha.y + 385}, 11, 1, Fade(WHITE, 0.4f));
        DrawTextEx(fonteCopa, fig.tipo, (Vector2){(valoresFigurinha.x + 135) - MeasureTextEx(fonteCopa, fig.tipo, 11, 1).x / 2, valoresFigurinha.y + 408}, 11, 1, ehEspecial ? COPA_OURO_PURO : COPA_VERDE_NEON);

        if (tela.statusCartas[tela.cartaAtualIndice] == 1) {
            DrawRectangle(valoresFigurinha.x + 15, valoresFigurinha.y + 435, 240, 60, Fade(COPA_VERDE_NEON, 0.15f));
            DrawRectangleLinesEx((Rectangle){valoresFigurinha.x + 15, valoresFigurinha.y + 435, 240, 60}, 1.0f, COPA_VERDE_NEON);
            DrawTextEx(fonteCopa, "NOVA FIGURINHA!", (Vector2){(valoresFigurinha.x + 135) - MeasureTextEx(fonteCopa, "NOVA FIGURINHA!", 12, 1).x / 2, valoresFigurinha.y + 448}, 12, 1, COPA_VERDE_NEON);
            DrawTextEx(fonteCopa, "ADICIONADA AO ALBUM", (Vector2){(valoresFigurinha.x + 135) - MeasureTextEx(fonteCopa, "ADICIONADA AO ALBUM", 10, 1).x / 2, valoresFigurinha.y + 470}, 10, 1, WHITE);
        } else {
            DrawRectangle(valoresFigurinha.x + 15, valoresFigurinha.y + 435, 240, 60, Fade(COPA_AZUL_MEDIO, 0.6f));
            DrawRectangleLinesEx((Rectangle){valoresFigurinha.x + 15, valoresFigurinha.y + 435, 240, 60}, 1.0f, Fade(WHITE, 0.3f));
            DrawTextEx(fonteCopa, "REPETIDA", (Vector2){(valoresFigurinha.x + 135) - MeasureTextEx(fonteCopa, "REPETIDA", 12, 1).x / 2, valoresFigurinha.y + 448}, 12, 1, Fade(WHITE, 0.7f));
            DrawTextEx(fonteCopa, "ENVIADA PARA A MOCHILA", (Vector2){(valoresFigurinha.x + 135) - MeasureTextEx(fonteCopa, "ENVIADA PARA A MOCHILA", 10, 1).x / 2, valoresFigurinha.y + 470}, 10, 1, Fade(WHITE, 0.5f));
        }

        if (tela.cartaAtualIndice > 0) DrawTextEx(fonteCopa, "<", (Vector2){290, 350}, 26, 1, Fade(WHITE, 0.3f));
        if (tela.cartaAtualIndice < tela.totalCartasSorteioAtual - 1) DrawTextEx(fonteCopa, ">", (Vector2){690, 350}, 26, 1, Fade(WHITE, 0.3f));

        char indicador[60];
        char numeroIndicadorAtual[20];
        char numeroIndicadorTotal[20];

        numeroParaTexto(tela.cartaAtualIndice + 1, numeroIndicadorAtual);
        numeroParaTexto(tela.totalCartasSorteioAtual, numeroIndicadorTotal);

        strcpy(indicador, "FIGURINHA: ");
        strcat(indicador, numeroIndicadorAtual);
        strcat(indicador, " / ");
        strcat(indicador, numeroIndicadorTotal);

        DrawTextEx(fonteCopa, indicador, (Vector2){500 - MeasureTextEx(fonteCopa, indicador, 13, 1).x / 2, 645}, 13, 1, Fade(WHITE, 0.5f));

        if (IsKeyPressed(KEY_RIGHT) && tela.cartaAtualIndice < tela.totalCartasSorteioAtual - 1) tela.cartaAtualIndice++;
        if (IsKeyPressed(KEY_LEFT) && tela.cartaAtualIndice > 0) tela.cartaAtualIndice--;

        float efeitoEnter = (sinf(tempoGlobal * 6.0f) + 1.0f) / 2.0f;
        DrawTextEx(fonteCopa, "APERTE [ ENTER ] PARA CONTINUAR", (Vector2){500 - MeasureTextEx(fonteCopa, "APERTE [ ENTER ] PARA CONTINUAR", 13, 1).x / 2, 680}, 13, 1, Fade(COPA_OURO_PURO, 0.5f + efeitoEnter * 0.5f));

        if (IsKeyPressed(KEY_ENTER)) {
            if (!tela.modoTurbo) {
                // Modo normal: abre um pacote por vez ate acabar a quantidade escolhida
                tela.quantidadeDesejada--;

                if (tela.quantidadeDesejada > 0 && pacotes_fechados > 0) {
                    pacotes_fechados--;
                    salvarPacotes();
                    sortearFigurinhasDoPacote(figurinhas, mochila, album, total, total_mochila, total_album, 1);

                    tela.cartaAtualIndice = 0;
                    tela.fase = 1;
                } else {
                    descarregarRecursosAbertura();
                    tela.fase = 0;
                    tela.quantidadeDesejada = 1;
                    *estadoAtual = MENU_PRINCIPAL;
                }
            } else {
                // Modo turbo: todos os pacotes ja foram sorteados; apenas finaliza
                descarregarRecursosAbertura();
                tela.fase = 0;
                tela.quantidadeDesejada = 1;
                *estadoAtual = MENU_PRINCIPAL;
            }
        }
    }
}