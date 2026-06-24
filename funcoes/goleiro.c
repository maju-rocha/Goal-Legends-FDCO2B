#include "raylib.h"
#include "biblioteca.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#define JOGAR_NOVAMENTE 1

//Struct atacantes
typedef struct{
    Rectangle rec;
    bool usado;
}Atacante;

//Funcao para contorno de texto
void DrawTextOutLine(const char *text, int x, int y, int fontSize, Color textColor, Color outlineColor){
    
    int offset = 2;

    for (int dx = -offset; dx <= offset; dx++){
        for (int dy = -offset; dy <= offset; dy++){
            if (dx != 0 || dy != 0){
                DrawText(text, x + dx, y + dy, fontSize, outlineColor);
            }//if
        }//for
    }//for

    DrawText(text, x, y, fontSize, textColor);
}//void

void jogarGoleiro(){
    
    //Inicialização da tela
    const int screenWidth = 1000;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Minigame: Jogo do Goleiro");
    InitAudioDevice();
    SetExitKey(KEY_NULL);
    
    //Váriaveis das imagens
    Texture2D imagemCampo = LoadTexture("imagens/imagem_campogoleiro.png");
    Texture2D imagemAtacante = LoadTexture("imagens/imagem_atacante.png");
    Texture2D imagemGoleiro = LoadTexture("imagens/imagem_goleiro.png");
    Texture2D imagemVida = LoadTexture("imagens/imagem_vida.png");
    Texture2D imagemFundoPontuacao = LoadTexture("imagens/imagem_fundomenu.png");
    Texture2D imagemLuvaGoleiro = LoadTexture("imagens/imagem_goleiroluva.png");
    Texture2D imagemCabecaGoleiro = LoadTexture("imagens/imagem_goleirocabeca.png");
    Texture2D imagemCabecaAtacante = LoadTexture("imagens/imagem_atacantecabeca.png");
    Texture2D imagemBola2026 = LoadTexture("imagens/imagem_bola2026.png");

    //Váriaveis de audio
    Sound somDefesa = LoadSound ("audio/som_defesagoleiro.mp3");
    SetSoundVolume(somDefesa, 0.05f);
    Sound somToque = LoadSound ("audio/som_toqueatacante.mp3");
    SetSoundVolume(somToque, 0.3f);
    Sound somMusica = LoadSound ("audio/som_musicagoleiro.mp3");
    SetSoundVolume(somMusica, 0.03f);
    //Váriavel da fonte
    Font fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf");
    
    //Váriaveis goleiro.c
    bool jogarNovamente = true;
    bool fecharPrograma = false;
    bool mostrarExplicacao = true;

    //Opções fim de jogo
    Rectangle btnMenu = {300, 450, 400, 60};
    Rectangle btnReiniciar = {300, 350, 400, 60};
    
    srand(time(NULL));//inicia aleatoriedade para as direções da bola após defesa

    //=======================================================//
    //========== Estrutura e Desenho da explicação ==========//
    //=======================================================//

    while(!WindowShouldClose() && mostrarExplicacao){

        Color fundoexplicacao = (Color){98, 88, 237, 255};

        BeginDrawing();
        ClearBackground(fundoexplicacao);

        DrawTextOutLine("MINIGAME: GOLEIRO", 280, 100, 40, WHITE, BLACK);

        DrawTextOutLine("Defenda os chutes pressionando as setas do teclado | <- | -> |", 100, 220, 27, WHITE, BLACK);

        DrawTextOutLine("Voce tem 3 vidas", 100, 260, 27, WHITE, BLACK);

        DrawTextOutLine("Se receber um gol perde uma vida", 100, 300, 27, WHITE, BLACK);

        DrawTextOutLine("A cada 30 segundos percorridos = 1 pacote.", 100, 340, 27, WHITE, BLACK);

        DrawTextOutLine("Pressione qualquer tecla para comecar", 100, 450, 27, YELLOW, BLACK);

        EndDrawing();

        if(GetKeyPressed() != 0){
            mostrarExplicacao = false;

        }//if
    
    }//while

    while (jogarNovamente){
        
        PlaySound(somMusica);//Musica do jogo

        if(fecharPrograma){
            CloseWindow();
            exit(0);
        }//if

        SetTargetFPS(60);

        //Variáveis
        int defesas = 0;
        int vidas = 3;

        Atacante atacantes[2];
        int qtdAtacantes = 0;

        float tempoJogo = 0.0f;
        float proximaAceleracao = 20.0f;
        float multiplicadorDificuldade = 1.0f;
        float velocidadeGoleiro = 8.0f;
        float proximoPacote = 30.0f;

        bool fimdejogo = false;

        
        Color verdeCampo = (Color){ 98, 209, 75, 255 };
        Color verdeGol = (Color){ 64, 107, 56, 255 };
        Color azulmenu = (Color){65, 53, 171, 255};
        Color corfimdejogo = (Color){94, 92, 115, 255};
        
        Vector2 boxPosition = { 500, 20 };//Posição inicial
        Vector2 boxSpeed = {((rand() % 11) - 5) * multiplicadorDificuldade, 7.0f * multiplicadorDificuldade};//Velocidade inicial da bola de acordo com a dificuldade

        //Colisões
        Rectangle obstacleRec = {(screenWidth - 200) / 2, screenHeight - 140, 200, 30}; //Posição e tamanho do obstáculo
        Rectangle golHitbox = {(screenWidth - 800) / 2, screenHeight - 100, 800, 100}; //Posição e tamanho do gol



        while (!fimdejogo){
            //Verifica saida do jogo fechando a aba da janela
            if (WindowShouldClose()) {
                fecharPrograma = true;
                
                break;
            }
            //Verifica saido do jogo para o menu apertando ESC
            if(IsKeyPressed(KEY_ESCAPE)){
                jogarNovamente = false; 

                break;
            }
            //Mouse
            Vector2 mousePoint = GetMousePosition();
            
            boxPosition.y += boxSpeed.y;//Move a bola verticalmente
            boxPosition.x += boxSpeed.x;//Move a bola horizontalmente
            
            //Ricocheteia a bola no retangulo
            if((boxPosition.y < 25) || (boxPosition.y > screenHeight -25)){
                boxSpeed.y *= -1;
            }//if
            if((boxPosition.x < 25) || (boxPosition.x > screenWidth -25)){
                boxSpeed.x *= -1;
            }//if

            //Hitbox da bola
            Rectangle boxRec = { boxPosition.x -25, boxPosition.y -25, 50, 50 };
            
            //Váriaveis de colisão
            bool collisionGoleiro = false;
            bool collisionGol = false;
            
            //Verificação de colisões entre a bola e goleiro/ bola e gol
            if(CheckCollisionRecs(boxRec, obstacleRec)){
                collisionGoleiro = true;
            }else{
                collisionGoleiro = false;
            }if(CheckCollisionRecs(boxRec, golHitbox)){
                collisionGol = true;
            }else{
                collisionGol = false;
            }
            
            //Verificação da colisão e consequência da bola e gol
            if(collisionGoleiro){
                PlaySound(somDefesa);//Som de defesa

                boxPosition = (Vector2){ 500, 27 };//Reseta a posição da bola
                
                boxSpeed.y = (rand() % 5 + 7) * multiplicadorDificuldade;//Muda a velocidade da bola de acordo com a dificuldade
                boxSpeed.x = (rand() % 11) - 5;//Muda a angulação da bola

                
                //Spawna de 0 a 2 atacantes
                qtdAtacantes = rand() % 3;
                
                //Spawn randomizado dos atacantes
                for(int i = 0; i < qtdAtacantes; i++){
                    atacantes[i].rec = (Rectangle){rand() % 750 + 50, rand() % 350 + 150, 150, 30};//Posição aleatória dos atacantes
                    
                    atacantes[i].usado = false;
                }//for
            }//if
            
            //Verificação de colisão entre bola e atacantes
            for(int i = 0; i < qtdAtacantes; i++){
                if(!atacantes[i].usado && CheckCollisionRecs(boxRec, atacantes[i].rec)){
                    
                    PlaySound(somToque);//Som de toque

                    atacantes[i].usado = true;
                    
                    boxSpeed.x = (rand() % 11) - 5;
                    
                    if(boxSpeed.x == 0){
                        boxSpeed.x = 1;
                    }//if

                    
                }//if
            }//for
            
            //Resultado da colisão da bola com o gol
            if(collisionGol){

                vidas--;//Decrementa se bola acerta o gol

                boxPosition = (Vector2){500, 27};//Reseta a posição da bola

                boxSpeed.y = (rand() % 5 + 7) * multiplicadorDificuldade;//Aumenta a velocidade de 5 a 11 de acordo com o mutiplicador de dificuldade
                boxSpeed.x = (rand() % 11) - 5;//Ângulo randomizado

                //Verifica se a bola está parada
                if(boxSpeed.x == 0){
                    boxSpeed.x = 1;
                }//if

                //Verifica a quantidade de vidas
                if(vidas <= 0){
                    fimdejogo = true;
                }

            }//if
            
            //Váriavel de tempo atualiza a cada segundo (60 fps)
            tempoJogo += GetFrameTime();
            
            //Condição de aumento de dificuldade
            if(tempoJogo >= proximaAceleracao){
                multiplicadorDificuldade += 0.1f;//Aumenta em 0.1 de velocidade geral da bola
                proximaAceleracao += 20.0f;//A cada 20 segundos
                velocidadeGoleiro += 0.5f;//Aumento de velocidade do goleiro
            }//if

            //Condição verificada para recompensa em pacotes
            if(tempoJogo >= proximoPacote){
                pacotes_fechados++;
                salvarPacotes();
                proximoPacote += 30.0f;
            }
                            
            //Teclas para o goleiro
            if(IsKeyDown(KEY_RIGHT)) obstacleRec.x += velocidadeGoleiro;
            if(IsKeyDown(KEY_LEFT)) obstacleRec.x -= velocidadeGoleiro;

            //Retângulo da luva do goleiro
            Rectangle luvaRec = {obstacleRec.x - 5, obstacleRec.y - 30, obstacleRec.width + 10, obstacleRec.height};
            
            //Retângulo da cabeça do goleiro
            Rectangle cabRec = {obstacleRec.x, obstacleRec.y - 25, obstacleRec.width, obstacleRec.height + 35};
            
            //Retângulo do menu de tempo/dificuldade
            Rectangle fundoMenuRec = {0, 0, 310, 90};

            //=======================================================//
            //=============== Parte Gráfica do Jogo =================//
            //=======================================================//

            BeginDrawing();
            
            //Desenha o campo
            DrawTexture(imagemCampo, 0, 0, WHITE);
            
            //Desenha linha do campo
            DrawRectangleRec((Rectangle){golHitbox.x - 25, golHitbox.y + 200, golHitbox.width + 50, golHitbox.height + 50}, WHITE);
            
            //Formato do Gol (traves)
            DrawRectangleRec((Rectangle){golHitbox.x - 4, golHitbox.y - 4, golHitbox.width + 8, golHitbox.height + 8}, BLACK); //contorno do gol
            DrawRectangleRec(golHitbox, WHITE);
            
            //Fundo do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 8, golHitbox.y + 8, golHitbox.width - 16, golHitbox.height - 16}, BLACK); //contorno do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 12, golHitbox.y + 12, golHitbox.width - 24, golHitbox.height - 24}, verdeGol); // gol
            
            //Linhas verticais da rede
            DrawRectangleRec((Rectangle){golHitbox.x + 25, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 50, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 75, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 100, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 150, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 200, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 250, golHitbox.y + 12, 2, 80}, BLACK);
            DrawRectangleRec((Rectangle){golHitbox.x + 300, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 350, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 400, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 450, golHitbox.y + 12, 2, 80}, BLACK);
            DrawRectangleRec((Rectangle){golHitbox.x + 500, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 550, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 600, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 650, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 700, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 750, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 125, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 175, golHitbox.y + 12, 2, 80}, BLACK);
            DrawRectangleRec((Rectangle){golHitbox.x + 225, golHitbox.y + 12, 2, 80}, BLACK);
            DrawRectangleRec((Rectangle){golHitbox.x + 275, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 325, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 375, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 425, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 475, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 525, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 575, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 625, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 675, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 725, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 775, golHitbox.y + 12, 2, 80}, BLACK); 
            
            //Linhas horizontais da rede
            DrawRectangleRec((Rectangle){golHitbox.x + 10, golHitbox.y + 35, 780, 2}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 10, golHitbox.y + 62, 780, 2}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 10, golHitbox.y + 90, 780, 2}, BLACK); 

            //Desenha a bola
            DrawTexturePro(imagemBola2026, (Rectangle){0, 0, boxRec.width, boxRec.height},boxRec, (Vector2){0, 0}, 0, WHITE);
            
            //Desenha o goleiro
            DrawTexturePro(imagemGoleiro, (Rectangle){0, 0, obstacleRec.width, obstacleRec.height},obstacleRec, (Vector2){0, 0}, 0, WHITE);
            DrawTexturePro(imagemLuvaGoleiro, (Rectangle){0, 0, imagemLuvaGoleiro.width, imagemLuvaGoleiro.height}, luvaRec, (Vector2){0, 0}, 0, WHITE);
            DrawTexturePro(imagemCabecaGoleiro, (Rectangle){0, 0, imagemCabecaGoleiro.width, imagemCabecaGoleiro.height}, cabRec, (Vector2){0, 0}, 0, WHITE);
            
            //Desenha os atacantes
            for(int i = 0; i < qtdAtacantes; i++){
                if(!atacantes[i].usado){
                    Rectangle cabAtRec = {atacantes[i].rec.x -2, atacantes[i].rec.y - 10, atacantes[i].rec.width + 10, atacantes[i].rec.height + 35};
                    DrawTexturePro(imagemAtacante,(Rectangle){0, 0, imagemAtacante.width, imagemAtacante.height}, atacantes[i].rec, (Vector2){0, 0}, 0, WHITE);
                    DrawTexturePro(imagemCabecaAtacante,(Rectangle){0, 0, imagemCabecaAtacante.width, imagemCabecaAtacante.height}, cabAtRec,(Vector2){0, 0},0,WHITE);

                }//if
            }//for

            //Fundo dos textos
            DrawTexturePro(imagemFundoPontuacao,(Rectangle){0, 0, imagemFundoPontuacao.width, imagemFundoPontuacao.height}, fundoMenuRec,(Vector2){0, 0}, 0,Fade(WHITE, 0.8f));
            
            //Texto tempo
            const char *textoTempo = TextFormat("Tempo: %.0f", tempoJogo);

            //Contorno preto
            DrawTextEx(fonteCopa, textoTempo, (Vector2){18, 50}, 14, 2, BLACK);
            DrawTextEx(fonteCopa, textoTempo, (Vector2){22, 50}, 14, 2, BLACK);
            DrawTextEx(fonteCopa, textoTempo, (Vector2){20, 48}, 14, 2, BLACK);
            DrawTextEx(fonteCopa, textoTempo, (Vector2){20, 52}, 14, 2, BLACK);

            //Texto
            DrawTextEx(fonteCopa, textoTempo, (Vector2){20, 50}, 14, 2, WHITE);


            //Texto Dificuldade
            const char *textoDificuldade = TextFormat("Dificuldade: %.1fx", multiplicadorDificuldade);

            //Contorno preto
            DrawTextEx(fonteCopa, textoDificuldade, (Vector2){18, 30}, 14, 2, BLACK);
            DrawTextEx(fonteCopa, textoDificuldade, (Vector2){22, 30}, 14, 2, BLACK);
            DrawTextEx(fonteCopa, textoDificuldade, (Vector2){20, 28}, 14, 2, BLACK);
            DrawTextEx(fonteCopa, textoDificuldade, (Vector2){20, 32}, 14, 2, BLACK);

            //Texto branco
            DrawTextEx(fonteCopa, textoDificuldade, (Vector2){20, 30}, 14, 2, WHITE);
            
            //Desenha as vidas
            DrawTexturePro(imagemFundoPontuacao, (Rectangle){0, 0, imagemFundoPontuacao.width, imagemFundoPontuacao.height }, (Rectangle){screenWidth - 195, 0, 200, 100}, (Vector2){0, 0}, 0, Fade(WHITE, 0.8f));

            if(vidas >= 1){DrawTexturePro(imagemVida,(Rectangle){0,0,imagemVida.width,imagemVida.height},(Rectangle){screenWidth - 170, 20, 50, 50},(Vector2){0,0},0,WHITE);
            }//if

            if(vidas >= 2){DrawTexturePro(imagemVida,(Rectangle){0,0,imagemVida.width,imagemVida.height},(Rectangle){screenWidth - 120, 20, 50, 50},(Vector2){0,0},0,WHITE);
            }//if

            if(vidas >= 3){DrawTexturePro(imagemVida,(Rectangle){0,0,imagemVida.width,imagemVida.height},(Rectangle){screenWidth - 70, 20, 50, 50},(Vector2){0,0},0,WHITE);
            }//if

                EndDrawing();
            }//fim do while

            //=======================================================//
            //========= Estrutura e Desenho do Fim de Jogo ==========//
            //=======================================================//

            if(fimdejogo){

                Menu_FimdeJogo opcao = MENU_FIM;

                while(!WindowShouldClose()){

                    BeginDrawing();

                    Vector2 mouse = GetMousePosition();

                    bool clicouReiniciar = IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, btnReiniciar);

                    bool clicouMenu = IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(mouse, btnMenu);

                    if(clicouMenu){
                        opcao = MENU_FIM;
                        break;
                    }//if

                    if(clicouReiniciar){
                        opcao = JOGAR_NOVAMENTE;
                        break;
                    }//if

                    //Fundo Fim de Jogo
                    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(corfimdejogo, 0.6f));

                    //Janela Fim de Jogo
                    Rectangle janelaFim = {screenWidth/2 - 200, screenHeight/2 - 150, 400, 300};

                    DrawRectangleRec(janelaFim, DARKBLUE);
                    DrawRectangleLinesEx(janelaFim, 3, BLACK);

                    //Texto Fim de jogo
                    DrawTextOutLine("FIM DE JOGO", janelaFim.x + 75, janelaFim.y + 30, 40, RED,BLACK);

                    //Botões
                    Rectangle btnReiniciar = {janelaFim.x + 100, janelaFim.y + 120, 200, 50};
                    Rectangle btnMenu = {janelaFim.x + 100, janelaFim.y + 190, 200, 50};
                    
                    //Cor, contorno e texto do botão reiniciar
                    DrawRectangleRec(btnReiniciar, BLUE);
                    DrawRectangleLinesEx(btnReiniciar, 3, BLACK);
                    DrawTextOutLine("REINICIAR", btnReiniciar.x + 45, btnReiniciar.y + 15, 20, WHITE,BLACK);

                    //Cor, contorno e texto do botão menu
                    DrawRectangleRec(btnMenu, BLUE);
                    DrawRectangleLinesEx(btnMenu, 3, BLACK);
                    DrawTextOutLine("MENU", btnMenu.x + 70, btnMenu.y + 15, 20, WHITE,BLACK);

                    EndDrawing();
                }//while

                if(opcao == JOGAR_NOVAMENTE){
                    jogarNovamente = true;
                }else{
                    jogarNovamente = false;
                }//if

        }//if fimdejogo

    }//while (fimdejogo)
    
    //Liberações de memória do jogo
    UnloadTexture(imagemCampo);
    UnloadTexture(imagemAtacante);
    UnloadTexture(imagemGoleiro);
    UnloadTexture(imagemVida);
    UnloadTexture(imagemFundoPontuacao);
    UnloadTexture(imagemLuvaGoleiro);
    UnloadTexture(imagemCabecaGoleiro);
    UnloadTexture(imagemCabecaAtacante);
    UnloadTexture(imagemBola2026);
    UnloadFont(fonteCopa);
    UnloadSound(somDefesa);
    UnloadSound(somToque);
    UnloadSound(somMusica);
    CloseAudioDevice();
    
    //Fecha a janela do jogo
    CloseWindow();
   
    return;
}