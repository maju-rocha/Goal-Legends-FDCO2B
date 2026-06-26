#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <raylib.h>
#include "biblioteca.h"
#include "global.h"
#include "penalti.h"
#include "salvarPacotes.h"

#define MAX_RASTRO 12 // Define o tamanho máximo da "cauda" de efeito da bola

// Declarações prévias
void DesenharGoleiroPro(Vector2 pos, Color corUniforme);

// Estados internos do Minigame (Máquina de Estados para controlar as telas)
typedef enum { P_MENU, P_JOGANDO, P_GOL, P_DEFENDEU, P_FIM_DE_JOGO } EstadoPenalti;

void jogarPenalti(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, int *qtd_pacotes) {
    // Definição das dimensões da janela do minijogo
    const int larguraTela = 1000;
    const int alturaTela = 800;
    
    // Inicializa a janela caso ela já não tenha sido aberta no menu principal
    if (!IsWindowReady()) {
        InitWindow(larguraTela, alturaTela, "Minijogo: Penalty Strike");
    }

    SetExitKey(KEY_NULL); // Desativa o ESC padrão da Raylib para não fechar o jogo direto
    SetTargetFPS(60);     // Crava o jogo a 60 frames por segundo

    // --- INICIALIZAÇÃO DO ÁUDIO ---
    if (!IsAudioDeviceReady()) {
        InitAudioDevice(); // Inicializa o sistema de som se necessário
    }

    // Corrigido para "musicas/" com 's' no final (Carregamento dos arquivos de áudio)
    Music musicaFundo = LoadMusicStream("audio/musica_fundo.mp3"); 
    Sound somChute = LoadSound("audio/chute.mp3");
    Sound somGol = LoadSound("audio/gol.mp3");
    
    // Inicia a reprodução do som de fundo em loop e define volume baixo (10%)
    PlayMusicStream(musicaFundo);
    SetMusicVolume(musicaFundo, 0.1f);

    // Carrega a fonte retro/arcade usada na interface do jogo
    Font fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf");

    // --- CURSOR E TEXTURAS ---
    // Cria uma textura personalizada para o mouse a partir de uma imagem da bola
    Image imagemBola = LoadImage("extras/bola_cursor.png");
    ImageResize(&imagemBola, 40, 40); // Redimensiona o cursor para 40x40 pixels
    Texture2D cursorBola = LoadTextureFromImage(imagemBola);
    UnloadImage(imagemBola); // Libera o arquivo de imagem da RAM (já está na VRAM da GPU)

    // Carrega a textura oficial da bola de futebol de 2026
    Texture2D imagemBola2026 = LoadTexture("imagens/imagem_bola2026.png");

    // --- VARIÁVEIS DE CONTROLE DO JOGO ---
    Vector2 posicaoGoleiro = { 500, 290 }; // Posição (X, Y) inicial do goleiro
    Vector2 alvoGoleiro = { 500, 290 };    // Ponto para onde o goleiro está se deslocando
    Vector2 posicaoBola = { 500, 660 };    // Posição (X, Y) inicial da bola na marca do pênalti
    Vector2 velocidadeBola = { 0, 0 };     // Vetor de velocidade nos eixos X e Y
    bool bolaChutada = false;              // Flag para saber se a bola já está em movimento
    float raioBola = 16.0f;                // Raio base da bola para detecção de colisão
    float rotacaoBola = 0.0f;              // Ângulo de rotação visual da bola
    float escalaBola = 1.0f;               // Escala para criar o efeito de perspectiva 3D
    
    // Vetor para armazenar o histórico de posições da bola e fazer o efeito borrão/rastro
    Vector2 rastroBola[MAX_RASTRO];
    int contadorRastro = 0;

    // Variáveis do oscilador da linha de mira (ângulo e velocidade de oscilação)
    float anguloMira = 0.0f;
    float velocidadeMira = 0.04f;

    // Contadores de progresso e recompensas
    int gols = 0;
    int tentativas = 0;
    const int maxTentativas = 5;
    int pontuacaoCombo = 0;
    int pacotesGanhosRodada = 0;
    int totalFigurinhas = 0;
    bool pacotesComputados = false; // Garante que a recompensa só seja somada uma vez no fim do jogo

    EstadoPenalti estadoAtual = P_MENU; // O jogo começa na tela de menu/instruções
    int timerFrames = 0;                // Timer genérico baseado em frames para telas de transição
    bool sairDoJogo = false;            // Controle para encerrar o loop e voltar ao menu do projeto
    bool fecharProjeto = false;         // Controle para saber se clicou no 'X' e deve fechar o programa todo

    // Definição das cores da identidade visual (Brasil/Canarinho)
    Color verdeCanarinho = (Color){ 98, 209, 75, 255 }; 
    Color amareloBrasil = (Color){ 255, 215, 0, 255 };
    Color azulBrasil = (Color){ 0, 39, 118, 255 };
    Color sombraUI = Fade(BLACK, 0.4f); // Cor preta com 40% de opacidade para sombras

    // Definição geométrica (Retângulos) dos botões da tela final para cliques do mouse
    Rectangle btnTentar = { 170, 640, 310, 58 };
    Rectangle btnMenu = { 520, 640, 310, 58 };

    // --- LOOP PRINCIPAL DO MINIJOGO ---
    while (!sairDoJogo) {

        // Se clicar no X da janela, fecha o projeto inteiro
        if(WindowShouldClose()){
            fecharProjeto = true;
            break;
        }

        // Se apertar ESC, quebra o loop e volta para o menu principal
        if(IsKeyPressed(KEY_ESCAPE)){
            sairDoJogo = true;
            break;
        }

        float tempoGlobal = (float)GetTime(); // Tempo em segundos desde que o programa iniciou
        Vector2 mousePoint = GetMousePosition(); // Captura a posição atual (X, Y) do mouse

        // Atualiza o streaming da música de fundo obrigatoriamente a cada frame
        UpdateMusicStream(musicaFundo);

        // --- MÁQUINA DE ESTADOS (LÓGICA DO JOGO) ---
        switch (estadoAtual) {
            case P_MENU:
                // Se o jogador pressionar ESPAÇO no menu, reseta as estatísticas e começa a partida
                if (IsKeyPressed(KEY_SPACE)) {
                    gols = 0;
                    tentativas = 0;
                    pontuacaoCombo = 0;
                    pacotesComputados = false;
                    estadoAtual = P_JOGANDO;
                }
                break;

            case P_JOGANDO:
                if (!bolaChutada) {
                    // --- ANTES DO CHUTE (Fase de Mira) ---
                    escalaBola = 1.0f;
                    // Faz o ângulo da linha de mira oscilar de um lado para o outro
                    anguloMira += velocidadeMira;
                    if (anguloMira > 0.75f || anguloMira < -0.75f) velocidadeMira *= -1; // Inverte o sentido nas bordas

                    // Faz o goleiro se mover sozinho de um lado para o outro (Função Seno baseada no tempo)
                    alvoGoleiro.x = 500 + sinf(tempoGlobal * 4.0f) * 200.0f;
                    posicaoGoleiro.x += (alvoGoleiro.x - posicaoGoleiro.x) * 0.1f; // Interpolação suave (Efeito de atraso/mola)

                    // Quando pressiona ESPAÇO, realiza o chute
                    if (IsKeyPressed(KEY_SPACE)) {
                        // Converte o ângulo da mira em componentes de velocidade vetorial X e Y
                        velocidadeBola.x = sinf(anguloMira) * -16.0f;
                        velocidadeBola.y = -19.0f; // Bola sobe em direção ao gol (Y negativo)
                        bolaChutada = true;
                        contadorRastro = 0;

                        // TOCA O SOM DO CHUTE
                        PlaySound(somChute);
                        SetSoundVolume(somChute, 0.8f);
                    }
                } else {
                    // --- DEPOIS DO CHUTE (Bola em Movimento) ---
                    // Aplica o "efeito/curva" na bola usando as setas do teclado
                    if (IsKeyDown(KEY_LEFT)){
                        velocidadeBola.x -= 0.45f; // Altera a trajetória para a esquerda
                        rotacaoBola -= 15.0f;      // Gira a bola visualmente para a esquerda
                    } else if (IsKeyDown(KEY_RIGHT)) {
                        velocidadeBola.x += 0.45f; // Altera a trajetória para a direita
                        rotacaoBola += 15.0f;      // Gira a bola visualmente para a direita
                    } else {
                        rotacaoBola += velocidadeBola.x * 2.0f; // Rotação natural proporcional à velocidade lateral
                    }

                    // Sistema de rastro: desloca as posições anteriores da bola para trás no vetor
                    for (int i = MAX_RASTRO - 1; i > 0; i--) rastroBola[i] = rastroBola[i - 1];
                    rastroBola[0] = posicaoBola; // A posição atual vira o topo do rastro
                    if (contadorRastro < MAX_RASTRO) contadorRastro++;

                    // Atualiza fisicamente a posição da bola somando a velocidade
                    posicaoBola.x += velocidadeBola.x;
                    posicaoBola.y += velocidadeBola.y;

                    // Reduz o tamanho/escala da bola à medida que ela sobe (Simula profundidade/distância 3D)
                    escalaBola = 1.0f - ((660.0f - posicaoBola.y) / 440.0f) * 0.42f;

                    // Inteligência Artificial do Goleiro: Ele tenta adivinhar e seguir a trajetória futura da bola
                    alvoGoleiro.x = posicaoBola.x + (velocidadeBola.x * 1.1f);
                    if (alvoGoleiro.x < 260) alvoGoleiro.x = 260; // Limita o goleiro à trave esquerda
                    if (alvoGoleiro.x > 740) alvoGoleiro.x = 740; // Limita o goleiro à trave direita
                    
                    posicaoGoleiro.x += (alvoGoleiro.x - posicaoGoleiro.x) * 0.09f; // Suavização do movimento de defesa

                    // VERIFICAÇÃO DE RESULTADOS:
                    // 1. O Goleiro interceptou a bola? (Colisão de círculos entre Bola e Goleiro)
                    if (CheckCollisionCircles(posicaoBola, raioBola * escalaBola, posicaoGoleiro, 32.0f)) {
                        estadoAtual = P_DEFENDEU;
                        tentativas++;
                        pontuacaoCombo -= 80; // Penalidade por erro
                        if (pontuacaoCombo < 0) pontuacaoCombo = 0;
                        timerFrames = 0;
                    }
                    // 2. A bola passou da linha do gol? (Y <= 220)
                    else if (posicaoBola.y <= 220) {
                        // Verifica se a coordenada X está dentro dos limites das traves (252 a 748)
                        if (posicaoBola.x >= 252 && posicaoBola.x <= 748) {
                            estadoAtual = P_GOL;
                            gols++;
                            // Ganha pontos extras dependendo de quão rápido/com efeito a bola foi chutada
                            pontuacaoCombo += 100 + (int)(fabsf(velocidadeBola.x) * 10);
                            
                            // TOCA O SOM DO GOL
                            PlaySound(somGol);
                            SetSoundVolume(somGol, 0.06f);
                        } else {
                            // Bola foi para fora
                            estadoAtual = P_DEFENDEU;
                            pontuacaoCombo -= 80;
                            if (pontuacaoCombo < 0) pontuacaoCombo = 0;
                        }
                        tentativas++;
                        timerFrames = 0;
                    }
                }
                break;

            case P_GOL:
            case P_DEFENDEU:
                // Telas de comemoração/erro temporizadas (espera 90 frames = 1.5 segundos antes de resetar)
                timerFrames++;
                if (timerFrames > 90) {
                    // Reseta os elementos do campo para o próximo chute
                    posicaoBola = (Vector2){ 500, 660 };
                    velocidadeBola = (Vector2){ 0, 0 };
                    posicaoGoleiro = (Vector2){ 500, 290 };
                    bolaChutada = false;
                    anguloMira = 0.0f;
                    rotacaoBola = 0.0f;

                    // Se atingiu o limite de chutes (5), calcula as recompensas e encerra a partida
                    if (tentativas >= maxTentativas) {
                        // Distribuição de pacotes com base no Score final obtido
                        if (pontuacaoCombo <= 250) pacotesGanhosRodada = 1;
                        else if (pontuacaoCombo <= 500) pacotesGanhosRodada = 2;
                        else if (pontuacaoCombo <= 750) pacotesGanhosRodada = 3;
                        else pacotesGanhosRodada = 4;
                        
                        // Computação e salvamento dos pacotes adicionados às variáveis globais
                        if (!pacotesComputados) {
                            if (qtd_pacotes != NULL) {
                                *qtd_pacotes += pacotesGanhosRodada; // Atualiza o ponteiro recebido por parâmetro
                                pacotes_fechados++;                  // Variável global do álbum
                                salvarPacotes();                     // Grava os novos dados no arquivo local (.txt/.dat)
                            }
                            pacotesComputados = true;
                        }

                        totalFigurinhas = pacotesGanhosRodada * 7; // Cada pacote contém 7 figurinhas
                        estadoAtual = P_FIM_DE_JOGO;
                    } else {
                        estadoAtual = P_JOGANDO; // Continua para o próximo chute
                    }
                }
                break;

            case P_FIM_DE_JOGO:
                // Gerenciamento de cliques nos botões de reiniciar ou voltar
                if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                    // Se clicou em "TENTAR NOVAMENTE", limpa todas as variáveis e reinicia o fluxo
                    if (CheckCollisionPointRec(mousePoint, btnTentar)) {
                        gols = 0;
                        tentativas = 0;
                        pontuacaoCombo = 0;
                        pacotesGanhosRodada = 0;
                        totalFigurinhas = 0;
                        pacotesComputados = false;
                        posicaoBola = (Vector2){ 500, 660 };
                        velocidadeBola = (Vector2){ 0, 0 };
                        posicaoGoleiro = (Vector2){ 500, 290 };
                        bolaChutada = false;
                        estadoAtual = P_JOGANDO;
                    } 
                    // Se clicou em "MENU PRINCIPAL", ativa a flag para sair da função
                    else if (CheckCollisionPointRec(mousePoint, btnMenu)) {
                        sairDoJogo = true; 
                    }
                }
                break;
        }

        // Se o jogador pediu para voltar ao menu principal, interrompe o desenho imediatamente
        if (sairDoJogo) break;

        // --- RENDERIZAÇÃO (DESENHO DOS GRÁFICOS) ---
        BeginDrawing();
        ClearBackground(verdeCanarinho);

        // Desenha as listras horizontais do gramado do estádio
        for (int y = 200; y < alturaTela; y += 60) {
            Color corGramado = ( (y / 60) % 2 == 0 ) ? verdeCanarinho : (Color){ 85, 185, 65, 255 };
            DrawRectangle(0, y, larguraTela, 60, corGramado);
        }

        // Desenha a estrutura cinza de fundo da arquibancada
        DrawRectangle(0, 0, larguraTela, 180, (Color){ 45, 45, 50, 255 }); 
        for (int y = 20; y < 180; y += 30) {
            DrawRectangle(0, y, larguraTela, 4, (Color){ 75, 75, 80, 255 });      
            DrawRectangle(0, y + 4, larguraTela, 26, (Color){ 55, 55, 60, 255 });  
        }

        // --- GERAÇÃO DA TORCIDA ANIMADA ---
        srand(1337); // Fixa a semente do gerador aleatório para a torcida não mudar de cor a cada frame
        Color coresCamisa[] = { RED, azulBrasil, amareloBrasil, WHITE, ORANGE, GREEN };
        Color coresPele[] = { (Color){241,194,125,255}, (Color){141,85,36,255}, (Color){255,219,172,255} };

        // Varre a arquibancada gerando bonequinhos em formato pixel-art
        for (int y = 20; y < 180; y += 30) {
            for (int x = 8; x < larguraTela - 12; x += 24) {
                Color camisa = coresCamisa[rand() % 6];
                Color pele = coresPele[rand() % 3];
                // Cria o efeito de pulo senoidal individualizado com base na posição X do torcedor
                int puloAnima = (int)(sinf(tempoGlobal * 4.0f + (x * 0.08f)) * 4.0f);
                if (rand() % 4 != 0) puloAnima = 0; // Nem todo mundo pula ao mesmo tempo

                DrawRectangle(x, y + 10 + puloAnima, 18, 16, camisa); // Corpo/Camisa
                DrawRectangleLines(x, y + 10 + puloAnima, 18, 16, BLACK);
                DrawRectangle(x + 2, y + 2 + puloAnima, 14, 10, pele);   // Rosto/Pele
                DrawRectangleLines(x + 2, y + 2 + puloAnima, 14, 10, BLACK);
                DrawRectangle(x + 2, y + 2 + puloAnima, 14, 3, (rand() % 2 == 0) ? BLACK : (Color){100, 50, 20, 255}); // Cabelo
            }
        }
        srand((unsigned int)tempoGlobal); // Restaura o comportamento aleatório padrão do jogo
        DrawRectangle(0, 180, larguraTela, 20, BLACK); // Linha divisória da arquibancada

        // --- ELEMENTOS E MARCAÇÕES DO CAMPO DE FUTEBOL ---
        // Rede interna do gol (Efeito quadriculado com linhas translúcidas de opacidade 120)
        int divisorRede = 20;
        for (int x = 250; x <= 750; x += divisorRede) DrawRectangle(x, 200, 2, 130, (Color){ 255, 255, 255, 120 });
        for (int y = 200; y <= 330; y += 15) DrawRectangle(250, y, 500, 2, (Color){ 255, 255, 255, 120 });

        // Linhas brancas de marcação da grande área e a marca de cal do pênalti
        DrawRectangleLines(150, 200, 700, 420, WHITE);
        DrawLine(0, 330, larguraTela, 330, WHITE);
        DrawCircle(500, 560, 6, WHITE);

        // Estrutura física das Traves Brancas com contorno preto para dar destaque visual
        DrawRectangle(242, 196, 12, 134, WHITE); DrawRectangle(242, 196, 12, 134, BLACK); DrawRectangle(244, 196, 8, 134, WHITE); // Poste Esquerdo
        DrawRectangle(746, 196, 12, 134, WHITE); DrawRectangle(746, 196, 12, 134, BLACK); DrawRectangle(748, 196, 8, 134, WHITE); // Poste Direito
        DrawRectangle(242, 196, 516, 12, WHITE); DrawRectangle(242, 196, 516, 12, BLACK); DrawRectangle(244, 198, 512, 8, WHITE); // Travessão Superior

        // --- RENDERIZAÇÃO ESPECÍFICA DE CADA TELA (INTERFACE DO USUÁRIO) ---
        switch (estadoAtual) {
            case P_MENU:
                // Desenha o painel de instruções cinza escuro centralizado na tela com overlay escuro no fundo
                DrawRectangle(0, 0, larguraTela, alturaTela, (Color){ 0, 0, 0, 200 });
                DrawRectangle(194, 194, 612, 412, BLACK);
                DrawRectangle(200, 200, 600, 400, (Color){ 25, 25, 25, 255 });
                DrawTextEx(fonteCopa, "MODO PENALTI", (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, "MODO PENALTI", 24, 2).x/2, 250 }, 24, 2, amareloBrasil);
                DrawTextEx(fonteCopa, "CONTROLES:", (Vector2){ 240, 340 }, 14, 2, WHITE);
                DrawTextEx(fonteCopa, "- ESPACO: CHUTAR BOLA", (Vector2){ 240, 380 }, 12, 2, LIGHTGRAY);
                DrawTextEx(fonteCopa, "- SETAS: ADICIONAR EFEITO", (Vector2){ 240, 410 }, 12, 2, LIGHTGRAY);
                DrawTextEx(fonteCopa, "PRESSIONE ESPACO PARA JOGAR", (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, "PRESSIONE ESPACO PARA JOGAR", 12, 2).x/2, 510 }, 12, 2, amareloBrasil);
                break;

            case P_JOGANDO:
                // HUD Superior Esquerdo (Placar de Chutes e Gols realizados)
                DrawRectangle(34, 24, 262, 82, BLACK); DrawRectangle(40, 30, 250, 70, (Color){ 25, 25, 25, 255 });
                DrawTextEx(fonteCopa, TextFormat("CHUTES: %d/%d", tentativas, maxTentativas), (Vector2){ 55, 45 }, 12, 2, WHITE);
                DrawTextEx(fonteCopa, TextFormat("GOLS:   %d", gols), (Vector2){ 55, 70 }, 12, 2, amareloBrasil);

                // HUD Superior Direito (Pontuação / Score acumulado)
                DrawRectangle(704, 24, 262, 82, BLACK); DrawRectangle(710, 30, 250, 70, (Color){ 25, 25, 25, 255 });
                DrawTextEx(fonteCopa, "PONTOS", (Vector2){ 725, 45 }, 10, 2, LIGHTGRAY);
                DrawTextEx(fonteCopa, TextFormat("%04d", pontuacaoCombo), (Vector2){ 725, 68 }, 18, 2, WHITE);

                if (!bolaChutada){
                    // Desenha a linha amarela de vetor que indica para onde o jogador está apontando a mira
                    float miraX = posicaoBola.x + sinf(anguloMira) * -220.0f;
                    float miraY = posicaoBola.y - cosf(anguloMira) * 220.0f;
                    DrawLineEx(posicaoBola, (Vector2){ miraX, miraY }, 2.0f, amareloBrasil);
                } else {
                    // Desenha os círculos semitransparentes de rastro armazenados no histórico de posições da bola
                    for (int i = 0; i < contadorRastro; i++) {
                        DrawCircleV(rastroBola[i], (raioBola * escalaBola) * 0.4f, (Color){ 255, 255, 255, 80 });
                    }
                }

                // Desenha o boneco do goleiro na tela chamando a função personalizada
                DesenharGoleiroPro(posicaoGoleiro, amareloBrasil);

                // Configura as matrizes de origem e destino para renderizar a textura da bola rotacionada na tela
                Rectangle fonteBola = { 0.0f, 0.0f, (float)imagemBola2026.width, (float)imagemBola2026.height };
                Rectangle destinoBola = { posicaoBola.x, posicaoBola.y, 32.0f * escalaBola, 32.0f * escalaBola };
                Vector2 centroOrigem = { (32.0f * escalaBola) / 2.0f, (32.0f * escalaBola) / 2.0f };
                DrawTexturePro(imagemBola2026, fonteBola, destinoBola, centroOrigem, 0, WHITE);
                break;

            case P_GOL:
                // Texto gigante de Gol na tela
                DrawTextEx(fonteCopa, "GOOOOOOOOOOOOOL!", (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, "GOOOOOOOOOOOOOL!", 40, 2).x/2, 330 }, 40, 2, amareloBrasil);
                break;

            case P_DEFENDEU:
                // Textos alternados baseados no motivo do erro do chute
                if (posicaoBola.x < 252 || posicaoBola.x > 748 || posicaoBola.y < 220) {
                    DrawTextEx(fonteCopa, "PARA FORA!", (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, "PARA FORA!", 30, 2).x/2, 330 }, 30, 2, WHITE);
                } else {
                    DrawTextEx(fonteCopa, "QUE DEFESA INCRIVEL!", (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, "QUE DEFESA INCRIVEL!", 30, 2).x/2, 330 }, 30, 2, RED);
                }
                break;

            case P_FIM_DE_JOGO:
                // Painel preto translúcido exibindo estatísticas finais e o saldo total de pacotes ganhos
                DrawRectangle(0, 0, larguraTela, alturaTela, (Color){ 0, 0, 0, 220 });
                DrawTextEx(fonteCopa, "FIM DE JOGO", (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, "FIM DE JOGO", 32, 2).x/2, 100 }, 32, 2, RED);
                DrawTextEx(fonteCopa, TextFormat("GOLS:  %d/%d", gols, maxTentativas), (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, TextFormat("GOLS:  %d/%d", gols, maxTentativas), 14, 2).x/2, 160 }, 14, 2, WHITE);
                DrawTextEx(fonteCopa, TextFormat("SCORE: %d", pontuacaoCombo), (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, TextFormat("SCORE: %d", pontuacaoCombo), 14, 2).x/2, 190 }, 14, 2, WHITE);
                
                DrawRectangle(244, 244, 512, 282, BLACK);
                DrawRectangle(250, 250, 500, 272, (Color){ 25, 25, 25, 255 });
                DrawTextEx(fonteCopa, "RECOMPENSA:", (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, "RECOMPENSA:", 14, 2).x/2, 280 }, 14, 2, amareloBrasil);
                DrawTextEx(fonteCopa, TextFormat("%d PACOTES RECEBIDOS", pacotesGanhosRodada), (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, TextFormat("%d PACOTES RECEBIDOS", pacotesGanhosRodada), 12, 2).x/2, 320 }, 12, 2, WHITE);
                DrawTextEx(fonteCopa, TextFormat("(+%d FIGURINHAS)", totalFigurinhas), (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, TextFormat("(+%d FIGURINHAS)", totalFigurinhas), 10, 2).x/2, 350 }, 10, 2, LIGHTGRAY);
                
                int saldoTotal = (qtd_pacotes != NULL) ? *qtd_pacotes : 0;
                DrawTextEx(fonteCopa, TextFormat("SALDO TOTAL: %d PACOTES", saldoTotal), (Vector2){ larguraTela/2 - MeasureTextEx(fonteCopa, TextFormat("SALDO TOTAL: %d PACOTES", saldoTotal), 11, 2).x/2, 390 }, 11, 2, GREEN);

                // Representação visual gráfica dos pacotinhos ganhos (Desenha retângulos amarelos lado a lado)
                for(int f = 0; f < pacotesGanhosRodada; f++) {
                    int posXCard = 435 + (f * 40) - ((pacotesGanhosRodada - 1) * 20);
                    DrawRectangle(posXCard, 440, 30, 40, amareloBrasil);
                    DrawRectangleLines(posXCard, 440, 30, 40, WHITE);
                }

                // --- COMPORTAMENTO DO BOTÃO: JOGAR DE NOVO ---
                bool hoverTentar = CheckCollisionPointRec(mousePoint, btnTentar); // Detecta mouse em cima do retângulo do botão
                int animTentar = hoverTentar ? ((IsMouseButtonDown(MOUSE_LEFT_BUTTON)) ? 4 : -4) : 0; // Cria efeito visual de afundar/subir ao passar o mouse
                if(hoverTentar && !IsMouseButtonDown(MOUSE_LEFT_BUTTON)) DrawRectangleRounded((Rectangle){ btnTentar.x + 6, btnTentar.y + 6, btnTentar.width, btnTentar.height }, 0.15f, 4, sombraUI);
                Rectangle rTentar = { btnTentar.x, btnTentar.y + animTentar, btnTentar.width, btnTentar.height };
                DrawRectangleRounded(rTentar, 0.15f, 4, hoverTentar ? WHITE : amareloBrasil);
                DrawRectangleRoundedLinesEx(rTentar, 0.15f, 4, 2.0f, azulBrasil);
                DrawTextEx(fonteCopa, "JOGAR DE NOVO", (Vector2){ rTentar.x + (rTentar.width/2) - (MeasureTextEx(fonteCopa, "JOGAR DE NOVO", 12, 1).x/2), rTentar.y + 22 }, 12, 1, azulBrasil);

                // --- COMPORTAMENTO DO BOTÃO: MENU PRINCIPAL ---
                bool hoverMenu = CheckCollisionPointRec(mousePoint, btnMenu);
                int animMenu = hoverMenu ? ((IsMouseButtonDown(MOUSE_LEFT_BUTTON)) ? 4 : -4) : 0;
                if(hoverMenu && !IsMouseButtonDown(MOUSE_LEFT_BUTTON)) DrawRectangleRounded((Rectangle){ btnMenu.x + 6, btnMenu.y + 6, btnMenu.width, btnMenu.height }, 0.15f, 4, sombraUI);
                Rectangle rMenu = { btnMenu.x, btnMenu.y + animMenu, btnMenu.width, btnMenu.height };
                DrawRectangleRounded(rMenu, 0.15f, 4, hoverMenu ? WHITE : amareloBrasil);
                DrawRectangleRoundedLinesEx(rMenu, 0.15f, 4, 2.0f, azulBrasil);
                DrawTextEx(fonteCopa, "MENU PRINCIPAL", (Vector2){ rMenu.x + (rMenu.width/2) - (MeasureTextEx(fonteCopa, "MENU PRINCIPAL", 12, 1).x/2), rMenu.y + 22 }, 12, 1, azulBrasil);
                break;
        }

        // Desenha a textura da bola personalizada exatamente no lugar das coordenadas do ponteiro do mouse
        DrawTexture(cursorBola, (int)mousePoint.x - cursorBola.width/2, (int)mousePoint.y - cursorBola.height/2, WHITE);
        EndDrawing(); // Finaliza a renderização e envia o frame pronto para o monitor
    }

    // --- LIMPEZA E DESALOCAÇÃO DE MEMÓRIA (BOAS PRÁTICAS) ---
    // Encerra os streams de música e descarrega todos os arquivos da VRAM/RAM ao sair da função
    StopMusicStream(musicaFundo);
    UnloadMusicStream(musicaFundo);
    UnloadSound(somChute);
    UnloadSound(somGol);

    UnloadTexture(cursorBola);
    UnloadTexture(imagemBola2026); 
    UnloadFont(fonteCopa);
    
    SetMouseCursor(MOUSE_CURSOR_DEFAULT); // Restaura o cursor padrão do sistema operacional
    
    // Fecha apenas a janela criada para o minijogo antes de retornar para o escopo do menu
    if(IsWindowReady()){
        CloseWindow();
    }

    // Se o loop foi interrompido porque clicou no 'X' superior da barra de janelas, encerra a execução total do projeto
    if(fecharProjeto){
        exit(0);
    }
}

// --- FUNÇÃO AUXILIAR PARA RENDERIZAR O GOLEIRO EM FORMATO VETORIAL ---
void DesenharGoleiroPro(Vector2 pos, Color corUniforme) {
    // Desenha o corpo (Retângulo) e a cabeça (Círculo) com contornos pretos finos ao redor
    DrawRectangle(pos.x - 25, pos.y, 50, 35, corUniforme); DrawRectangleLines(pos.x - 25, pos.y, 50, 35, BLACK);
    DrawCircle((int)pos.x, (int)pos.y - 12, 12, (Color){ 241, 194, 125, 255 }); DrawCircleLines((int)pos.x, (int)pos.y - 12, 12, BLACK);
}