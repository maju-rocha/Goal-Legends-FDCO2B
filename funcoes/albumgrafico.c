#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <raylib.h>
#include <raymath.h>
#include "biblioteca.h"
#include "global.h"
#include "albumgrafico.h"

//Paleta de cores usada na tela do album grafico
#define COPA_AZUL_ESCURO (Color){ 8, 12, 24, 255 }
#define COPA_AZUL_MEDIO  (Color){ 20, 28, 48, 255 }
#define COPA_VERDE_NEON  (Color){ 0, 230, 150, 255 }
#define COPA_VERMELHO    (Color){ 210, 20, 52, 255 }
#define COPA_OURO_PURO   (Color){ 255, 190, 0, 255 }
#define COPA_OURO_BRILHO (Color){ 255, 230, 120, 255 }
#define COPA_CINZA_CARD  (Color){ 28, 36, 56, 255 }

#define quantidadeMaximaParticulas 50

//Estrutura usada para criar as particulas do fundo
typedef struct{

    Vector2 posicao;
    Vector2 velocidade;
    float tamanhoBase;
    float randomizadorPosicao;
    float transparenciaParticula;
    float velocidadetransparenciaParticula;

}ParticulaCopa;

//Funcoes para montar o caminho da imagem tentando varios nomes de pasta e codigo
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

int montarCaminhoImagemFigurinha(char *caminho, char *secao, char *codigo) {
    char pastaOriginal[100];
    char pastaUnderline[100];
    char pastaMaiuscula[100];
    char codigoOriginal[30];
    char codigoSemEspaco[30];
    char pastas[40][100];
    char codigos[40][30];
    int totalPastas = 0;
    int totalCodigos = 0;

    strcpy(pastaOriginal, secao);
    strcpy(codigoOriginal, codigo);

    limparEspacos(pastaOriginal);
    limparEspacos(codigoOriginal);

    strcpy(pastaUnderline, pastaOriginal);
    trocarEspacoPorUnderlineImagem(pastaUnderline);
    copiarMaiusculoImagem(pastaMaiuscula, pastaUnderline);

    strcpy(codigoSemEspaco, codigoOriginal);
    removerEspacosInternosImagem(codigoSemEspaco);

    adicionarCodigoImagem(codigos, &totalCodigos, codigoOriginal);
    adicionarCodigoImagem(codigos, &totalCodigos, codigoSemEspaco);

    adicionarPastaImagem(pastas, &totalPastas, pastaOriginal);
    adicionarPastaImagem(pastas, &totalPastas, pastaUnderline);


    int ehCongo = 0;
    int ehEstadosUnidos = 0;
    int ehQatar = 0;
    int ehSuica = 0;
    int ehFifa = 0;

    if(strcmp(pastaUnderline, "Congo_DR") == 0 ){
        ehCongo = 1;
    }

    if(strcmp(pastaUnderline, "Estados_Unidos") == 0){
        ehEstadosUnidos = 1;
    }

    if( strcmp(pastaUnderline, "Catar") == 0 ){
        ehQatar = 1;
    }

    if(strcmp(pastaUnderline, "Suíça") == 0 ){
        ehSuica = 1;
    }

    if(strcmp(pastaMaiuscula, "FIFA_WORLD_CUP_2026") == 0){
        ehFifa = 1;
    }

    if(ehCongo){
        adicionarPastaImagem(pastas, &totalPastas, "Congo_DR");
    }

    if(ehEstadosUnidos){
        adicionarPastaImagem(pastas, &totalPastas, "EUA");
    }

    if(ehQatar){
        adicionarPastaImagem(pastas, &totalPastas, "Catar");
    }

    if(ehSuica){
        adicionarPastaImagem(pastas, &totalPastas, "Suiça");
    }

    if(ehFifa){
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


//Funcao para comparar codigo do album tambem ignorando espacos internos
int estaNoAlbumFlexivel(Figurinha *album, int total_album, char *codigo){

    char codigoBusca[30];
    char codigoAlbum[30];

    strcpy(codigoBusca, codigo);
    limparEspacos(codigoBusca);
    removerEspacosInternosImagem(codigoBusca);

    for(int i = 0; i < total_album; i++){
        strcpy(codigoAlbum, album[i].codigo);
        limparEspacos(codigoAlbum);
        removerEspacosInternosImagem(codigoAlbum);

        if(strcmp(codigoAlbum, codigoBusca) == 0){
            return 1;
        }//if
    }//for

    return 0;

}//int

//Funcao para desenhar o fundo com um leve movimento de parallax
void DesenharFundoParallax(Texture2D textura, Vector2 posicaoMouse, float transparenciaParticula){

    if(textura.id == 0){
        return;
    }//if

    float deslocamentoX = (posicaoMouse.x - 500.0f) * -0.02f;
    float deslocamentoY = (posicaoMouse.y - 400.0f) * -0.02f;

    Rectangle origem = { 0.0f, 0.0f, (float)textura.width, (float)textura.height };
    Rectangle destino = { deslocamentoX - 20.0f, deslocamentoY - 20.0f, 1040.0f, 840.0f };
    Vector2 centro = { 0.0f, 0.0f };

    DrawTexturePro(textura, origem, destino, centro, 0.0f, Fade(WHITE, transparenciaParticula));

}//void

//Funcao principal do album grafico
void albumGrafico(Figurinha *figurinhas, int total_figurinhas, Figurinha *album, int total_album){

    Texture2D imagens[total_figurinhas];
    int posicoesX[total_figurinhas];
    int posicoesY[total_figurinhas];
    int temImagem[total_figurinhas];
    int paginaDaFigurinha[total_figurinhas];
    char codigosFigurinha[total_figurinhas][20];
    char tiposFigurinha[total_figurinhas][30];

    char secoes[100][50];
    int totalSecoes = 0;
    int totalMostradas = 0;
    int contadorFigurinhasColadas = 0;
    char caminho[256];

    //Limpa os dados do album
    for(int j = 0; j < total_album; j++){
        limparFigurinha(&album[j]);
    }//for

    //Limpa os dados de todas as figurinhas
    for(int i = 0; i < total_figurinhas; i++){
        limparFigurinha(&figurinhas[i]);
    }//for

    //Cria a lista de secoes que serao usadas como paginas
    for(int i = 0; i < total_figurinhas; i++){

        int existe = 0;

        for(int j = 0; j < totalSecoes; j++){

            if(strcmp(secoes[j], figurinhas[i].secao) == 0){
                existe = 1;
                break;
            }//if

        }//for

        if(!existe && totalSecoes < 100){
            strcpy(secoes[totalSecoes], figurinhas[i].secao);
            totalSecoes++;
        }//if

    }//for

    SetConfigFlags(FLAG_MSAA_4X_HINT);
    InitWindow(1000, 800, "FIFA WORLD CUP 2026 - ALBUM");
    SetExitKey(KEY_NULL);
    SetTargetFPS(60);

    //Carrega as imagens de fundo
    Texture2D fundosCopa[3];

    fundosCopa[0] = LoadTexture("imagens/FIFA_2026_Verde.png");
    SetTextureFilter(fundosCopa[0], TEXTURE_FILTER_BILINEAR);

    fundosCopa[1] = LoadTexture("imagens/FIFA_2026_Vermelho.png");
    SetTextureFilter(fundosCopa[1], TEXTURE_FILTER_BILINEAR);

    fundosCopa[2] = LoadTexture("imagens/FIFA_2026_Azul.png");
    SetTextureFilter(fundosCopa[2], TEXTURE_FILTER_BILINEAR);

    int larguraFigurinha = 95;
    int alturaFigurinha = 130;
    int espacoX = 145;
    int espacoY = 150;
    int inicioX = 135;
    int inicioY = 110;

    //Organiza as figurinhas por secao e carrega as imagens que ja estao no album
    for(int pagina = 0; pagina < totalSecoes; pagina++){

        int coluna = 0;
        int linha = 0;

        for(int i = 0; i < total_figurinhas; i++){

            if(strcmp(figurinhas[i].secao, secoes[pagina]) == 0){

                posicoesX[totalMostradas] = inicioX + coluna * espacoX;
                posicoesY[totalMostradas] = inicioY + linha * espacoY;
                paginaDaFigurinha[totalMostradas] = pagina;
                temImagem[totalMostradas] = 0;

                strcpy(codigosFigurinha[totalMostradas], figurinhas[i].codigo);
                strcpy(tiposFigurinha[totalMostradas], figurinhas[i].tipo);

                if(estaNoAlbum(album, total_album, figurinhas[i].codigo) || estaNoAlbumFlexivel(album, total_album, figurinhas[i].codigo)){

                    if(montarCaminhoImagemFigurinha(caminho, figurinhas[i].secao, figurinhas[i].codigo)){

                        imagens[totalMostradas] = LoadTexture(caminho);

                        if(imagens[totalMostradas].id != 0){

                            SetTextureFilter(imagens[totalMostradas], TEXTURE_FILTER_BILINEAR);

                            temImagem[totalMostradas] = 1;
                            contadorFigurinhasColadas++;

                        }//if

                    }else{
                        TraceLog(LOG_WARNING, "IMAGEM NAO ENCONTRADA: secao=%s codigo=%s", figurinhas[i].secao, figurinhas[i].codigo);
                    }//if

                }//if

                totalMostradas++;
                coluna++;

                if(coluna == 5){
                    coluna = 0;
                    linha++;
                }//if

            }//if

        }//for

    }//for

    //Cria as particulas decorativas do fundo
    ParticulaCopa particulas[quantidadeMaximaParticulas];

    for(int i = 0; i < quantidadeMaximaParticulas; i++){

        particulas[i].posicao = (Vector2){ (float)GetRandomValue(-20, 1020), (float)GetRandomValue(0, 800) };
        particulas[i].velocidade = (Vector2){ (float)GetRandomValue(-8, 8) / 15.0f, (float)GetRandomValue(-15, -4) / 10.0f };
        particulas[i].tamanhoBase = (float)GetRandomValue(2, 4);
        particulas[i].randomizadorPosicao = (float)GetRandomValue(0, 360);
        particulas[i].transparenciaParticula = (float)GetRandomValue(20, 80) / 100.0f;
        particulas[i].velocidadetransparenciaParticula = (float)GetRandomValue(5, 15) / 1000.0f;

    }//for

    int paginaAtual = 0;
    int proximaPagina = 0;
    float tempoGlobal = 0.0f;
    float viradaProgresso = 1.0f;
    float velocidadeVirada = 0.07f;
    int estadoTransicao = 0;
    int direcaoTransicao = 1;

    //Laco principal da janela
    while(!WindowShouldClose()){

        if(IsKeyPressed(KEY_ESCAPE)){
            break;
        }//if

        tempoGlobal += GetFrameTime();

        Vector2 posicaoMouse = GetMousePosition();

        //Atualiza as particulas do fundo
        for(int i = 0; i < quantidadeMaximaParticulas; i++){

            particulas[i].posicao.x += particulas[i].velocidade.x + sinf(tempoGlobal + particulas[i].randomizadorPosicao) * 0.2f;
            particulas[i].posicao.y += particulas[i].velocidade.y;
            particulas[i].transparenciaParticula -= particulas[i].velocidadetransparenciaParticula;

            float distanciaMouse = Vector2Distance(posicaoMouse, particulas[i].posicao);

            if(distanciaMouse < 80.0f){
                particulas[i].posicao.x += (particulas[i].posicao.x - posicaoMouse.x) * 0.03f;
            }//if

            if(particulas[i].transparenciaParticula <= 0.0f || particulas[i].posicao.y < 70.0f){

                particulas[i].posicao = (Vector2){ (float)GetRandomValue(0, 1000), 760.0f };
                particulas[i].transparenciaParticula = (float)GetRandomValue(60, 100) / 100.0f;

            }//if

        }//for

        //Controla o efeito de virar pagina
        if(estadoTransicao == 1){

            viradaProgresso -= velocidadeVirada;

            if(viradaProgresso <= 0.0f){
                viradaProgresso = 0.0f;
                paginaAtual = proximaPagina;
                estadoTransicao = 2;
            }//if

        }else if(estadoTransicao == 2){

            viradaProgresso += velocidadeVirada;

            if(viradaProgresso >= 1.0f){
                viradaProgresso = 1.0f;
                estadoTransicao = 0;
            }//if

        }//if

        //Verifica os comandos para trocar de pagina
        if(estadoTransicao == 0){

            if(IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)){
                proximaPagina = (paginaAtual + 1) % totalSecoes;
                direcaoTransicao = 1;
                estadoTransicao = 1;
            }//if

            if(IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)){
                proximaPagina = (paginaAtual - 1 + totalSecoes) % totalSecoes;
                direcaoTransicao = -1;
                estadoTransicao = 1;
            }//if

            Rectangle botaoVoltar = { 5, 370, 50, 60 };
            Rectangle botaoAvancar = { 945, 370, 50, 60 };

            if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT)){

                if(CheckCollisionPointRec(posicaoMouse, botaoVoltar)){
                    proximaPagina = (paginaAtual - 1 + totalSecoes) % totalSecoes;
                    direcaoTransicao = -1;
                    estadoTransicao = 1;
                }//if

                if(CheckCollisionPointRec(posicaoMouse, botaoAvancar)){
                    proximaPagina = (paginaAtual + 1) % totalSecoes;
                    direcaoTransicao = 1;
                    estadoTransicao = 1;
                }//if

            }//if

        }//if

        BeginDrawing();
        ClearBackground(COPA_AZUL_ESCURO);

        int indiceFundo = paginaAtual % 3;

        //Desenha o fundo conforme a pagina atual
        DesenharFundoParallax(fundosCopa[indiceFundo], posicaoMouse, 1.0f);

        //Desenha as particulas
        for(int i = 0; i < quantidadeMaximaParticulas; i++){

            float tamanhoPulso = particulas[i].tamanhoBase * (1.0f + sinf(tempoGlobal * 3.0f + particulas[i].randomizadorPosicao) * 0.2f);

            DrawCircleV(particulas[i].posicao, tamanhoPulso, Fade(COPA_OURO_PURO, particulas[i].transparenciaParticula * 0.5f));

        }//for

        float s = viradaProgresso;
        float escalaHorizontal = (s * s * (3.0f - 2.0f * s));

        //Desenha as figurinhas da pagina atual
        for(int i = 0; i < totalMostradas; i++){

            if(paginaDaFigurinha[i] == paginaAtual){

                float originalX = (float)posicoesX[i];
                float x = originalX;
                float y = (float)posicoesY[i];

                if(estadoTransicao != 0){

                    if(direcaoTransicao == 1){
                        x = 500.0f - (500.0f - originalX) * escalaHorizontal;
                    }else{
                        x = 500.0f + (originalX - 500.0f) * escalaHorizontal;
                    }//if

                }//if

                Rectangle retanguloFigurinha = { x, y, (float)larguraFigurinha * escalaHorizontal, (float)alturaFigurinha };
                bool mouseEmCima = (estadoTransicao == 0) && CheckCollisionPointRec(posicaoMouse, retanguloFigurinha);

                if(mouseEmCima){

                    retanguloFigurinha.y -= 6.0f;
                    retanguloFigurinha.x -= 3.0f;
                    retanguloFigurinha.width += 6.0f;
                    retanguloFigurinha.height += 6.0f;

                }//if

                bool ehEspecial = (strstr(tiposFigurinha[i], "Especial") != NULL);

                if(temImagem[i]){

                    Rectangle origemImagem = { 0.0f, 0.0f, (float)imagens[i].width, (float)imagens[i].height };

                    DrawTexturePro(imagens[i], origemImagem, retanguloFigurinha, (Vector2){0.0f, 0.0f}, 0.0f, WHITE);

                    if(ehEspecial){

                        float pulso = (sinf(tempoGlobal * 5.0f) + 1.0f) / 2.0f;
                        Color corBorda = Fade(COPA_OURO_PURO, 0.5f + (pulso * 0.5f));

                        DrawRectangleLinesEx((Rectangle){retanguloFigurinha.x - 1.0f, retanguloFigurinha.y - 1.0f, retanguloFigurinha.width + 2.0f, retanguloFigurinha.height + 2.0f}, 2.0f, corBorda);
                        DrawRectangleLinesEx((Rectangle){retanguloFigurinha.x - 3.0f, retanguloFigurinha.y - 3.0f, retanguloFigurinha.width + 6.0f, retanguloFigurinha.height + 6.0f}, 1.0f, Fade(COPA_OURO_BRILHO, pulso * 0.3f));

                    }else{

                        Color corLinha = Fade(BLACK, 0.5f);

                        if(mouseEmCima){
                            corLinha = COPA_VERDE_NEON;
                        }//if

                        DrawRectangleLinesEx(retanguloFigurinha, 1.5f, corLinha);

                    }//if

                    if(mouseEmCima){

                        float reflexoX = retanguloFigurinha.x + ((float)sinf(tempoGlobal * 2.5f) + 1.0f) * 0.5f * retanguloFigurinha.width;

                        DrawLineEx((Vector2){reflexoX, retanguloFigurinha.y}, (Vector2){reflexoX + 15.0f, retanguloFigurinha.y + retanguloFigurinha.height}, 3.0f, Fade(WHITE, 0.25f));

                    }//if

                }else{

                    DrawRectangleRec(retanguloFigurinha, COPA_CINZA_CARD);

                    Color corLinha = Fade(WHITE, 0.08f);

                    if(mouseEmCima){
                        corLinha = COPA_VERDE_NEON;
                    }//if

                    DrawRectangleLinesEx(retanguloFigurinha, 1.5f, corLinha);

                    if(mouseEmCima){
                        DrawRectangleRec(retanguloFigurinha, Fade(COPA_AZUL_MEDIO, 0.4f));
                    }//if

                    if(escalaHorizontal > 0.25f){

                        DrawLineEx((Vector2){retanguloFigurinha.x + 4.0f, retanguloFigurinha.y + 4.0f}, (Vector2){retanguloFigurinha.x + retanguloFigurinha.width - 4.0f, retanguloFigurinha.y + retanguloFigurinha.height - 4.0f}, 1.0f, Fade(WHITE, 0.02f));

                        int larguraCodigo = MeasureText(codigosFigurinha[i], 16);
                        float posicaoCodigoX = retanguloFigurinha.x + (retanguloFigurinha.width - (larguraCodigo * escalaHorizontal)) / 2.0f;
                        Color corCodigo = Fade(WHITE, 0.35f);

                        if(mouseEmCima){
                            corCodigo = COPA_VERDE_NEON;
                        }//if

                        DrawText(codigosFigurinha[i], (int)posicaoCodigoX + 1, (int)(retanguloFigurinha.y + (retanguloFigurinha.height / 2.0f) - 7.0f), 16, Fade(BLACK, 0.6f));
                        DrawText(codigosFigurinha[i], (int)posicaoCodigoX, (int)(retanguloFigurinha.y + (retanguloFigurinha.height / 2.0f) - 8.0f), 16, corCodigo);

                    }//if

                }//if

            }//if

        }//for

        //Desenha a parte de cima da tela
        DrawRectangle(0, 0, 1000, 85, Fade(COPA_AZUL_MEDIO, 0.85f));
        DrawRectangle(0, 83, 1000, 2, COPA_OURO_PURO);

        DrawText(secoes[paginaAtual], 32, 27, 32, Fade(BLACK, 0.4f));
        DrawText(secoes[paginaAtual], 30, 25, 32, WHITE);

        float porcentagemConclusao = 0.0f;

        if(total_figurinhas > 0){
            porcentagemConclusao = ((float)contadorFigurinhasColadas / total_figurinhas) * 100.0f;
        }//if

        DrawRectangle(650, 47, 320, 10, Fade(BLACK, 0.5f));
        DrawRectangle(651, 48, (int)(318 * (porcentagemConclusao / 100.0f)), 8, COPA_VERDE_NEON);
        DrawText(TextFormat("ALBUM PROGRESSO: %.1f%% (%d/%d)", porcentagemConclusao, contadorFigurinhasColadas, total_figurinhas), 650, 24, 13, Fade(WHITE, 0.7f));

        DrawText(TextFormat("PAGINA %02d / %02d", paginaAtual + 1, totalSecoes), 455, 58, 15, COPA_OURO_PURO);

        Rectangle botaoVoltarReal = { 5, 370, 50, 60 };
        Rectangle botaoAvancarReal = { 945, 370, 50, 60 };

        bool emCimaVoltar = CheckCollisionPointRec(posicaoMouse, botaoVoltarReal) && (estadoTransicao == 0);
        bool emCimaAvancar = CheckCollisionPointRec(posicaoMouse, botaoAvancarReal) && (estadoTransicao == 0);

        float tamanhoBotaoVoltar = 22.0f;
        Color corBotaoVoltar = Fade(WHITE, 0.1f);
        Color corTextoVoltar = WHITE;

        if(emCimaVoltar){
            tamanhoBotaoVoltar = 26.0f;
            corBotaoVoltar = COPA_VERDE_NEON;
            corTextoVoltar = COPA_VERDE_NEON;
        }//if

        DrawCircleV((Vector2){30, 400}, tamanhoBotaoVoltar, corBotaoVoltar);
        DrawCircleV((Vector2){30, 400}, 20.0f, COPA_AZUL_MEDIO);
        DrawText("<", 24, 387, 24, corTextoVoltar);

        float tamanhoBotaoAvancar = 22.0f;
        Color corBotaoAvancar = Fade(WHITE, 0.1f);
        Color corTextoAvancar = WHITE;

        if(emCimaAvancar){
            tamanhoBotaoAvancar = 26.0f;
            corBotaoAvancar = COPA_VERDE_NEON;
            corTextoAvancar = COPA_VERDE_NEON;
        }//if

        DrawCircleV((Vector2){970, 400}, tamanhoBotaoAvancar, corBotaoAvancar);
        DrawCircleV((Vector2){970, 400}, 20.0f, COPA_AZUL_MEDIO);
        DrawText(">", 964, 387, 24, corTextoAvancar);

        //Desenha a parte de baixo com as instrucoes
        DrawRectangle(0, 765, 1000, 35, COPA_AZUL_MEDIO);
        DrawLine(0, 765, 1000, 765, Fade(WHITE, 0.08f));

        int larguraInstrucao = MeasureText("Seta Direita / Esquerda ou Cliques para mudar de pagina", 14);

        DrawText("Seta Direita / Esquerda ou Cliques para mudar de pagina", (1000 - larguraInstrucao) / 2, 775, 14, Fade(WHITE, 0.5f));

        EndDrawing();

    }//while

    //Descarrega as imagens das figurinhas
    for(int i = 0; i < totalMostradas; i++){

        if(temImagem[i]){
            UnloadTexture(imagens[i]);
        }//if

    }//for

    //Descarrega as imagens de fundo
    for(int i = 0; i < 3; i++){
        UnloadTexture(fundosCopa[i]);
    }//for

    CloseWindow();

}//void