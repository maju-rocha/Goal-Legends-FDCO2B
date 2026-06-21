#include "raylib.h"
#include "biblioteca.h"
#include <stdlib.h>
#include <time.h>

//Struct atacantes
typedef struct{
    Rectangle rec;
    bool usado;
}Atacante;

void jogarGoleiro(){
    
    //Inicialização da tela
    const int screenWidth = 1000;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Minigame: Jogo do Goleiro");
    
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
    
    Font fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf");//Carrega a fonte
    
    bool jogarNovamente = true;

    //Opções fim de jogo
    Rectangle btnMenu = {300, 450, 400, 60};
    Rectangle btnReiniciar = {300, 350, 400, 60};
    
    srand(time(NULL));//inicia aleatoriedade para as direções da bola após defesa

    while (jogarNovamente){

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
        
        Vector2 boxPosition = { 500, 20 };//Posição inicial
        Vector2 boxSpeed = {((rand() % 11) - 5) * multiplicadorDificuldade, 7.0f * multiplicadorDificuldade};//Velocidade inicial da bola de acordo com a dificuldade

        //Colisões
        Rectangle obstacleRec = {(screenWidth - 200) / 2, screenHeight - 140, 200, 30}; // Posição e tamanho do obstáculo
        Rectangle golHitbox = {(screenWidth - 800) / 2, screenHeight - 100, 800, 100}; // Posição e tamanho do gol

        while (!WindowShouldClose() && !fimdejogo){
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
            
            Rectangle boxRec = { boxPosition.x -25, boxPosition.y -25, 50, 50 };
            
            bool collisionGoleiro = false;
            bool collisionGol = false;
            
            if(CheckCollisionRecs(boxRec, obstacleRec)){
                collisionGoleiro = true;
            }else{
                collisionGoleiro = false;
            }if(CheckCollisionRecs(boxRec, golHitbox)){
                collisionGol = true;
            }else{
                collisionGol = false;
            }
            
            if(collisionGoleiro){
                defesas++;
                boxPosition = (Vector2){ 500, 27 };//Reseta a posição da bola
                
                boxSpeed.y = (rand() % 5 + 7) * multiplicadorDificuldade;//Muda a velocidade da bola de acordo com a dificuldade
                boxSpeed.x = (rand() % 11) - 5;//Muda a angulação da bola
                
                qtdAtacantes = rand() % 3;//Spawna de 0 a 2 atacantes
                
                for(int i = 0; i < qtdAtacantes; i++){
                    atacantes[i].rec = (Rectangle){rand() % 750 + 50, rand() % 350 + 150, 150, 30};//Posição aleatória dos atacantes
                    
                    atacantes[i].usado = false;
                }//for
            }//if
            
            for(int i = 0; i < qtdAtacantes; i++){
                if(!atacantes[i].usado && CheckCollisionRecs(boxRec, atacantes[i].rec)){
                    
                    atacantes[i].usado = true;
                    
                    boxSpeed.x = (rand() % 11) - 5;
                    
                    if(boxSpeed.x == 0){
                        boxSpeed.x = 1;
                    }//if

                    
                }//if
            }//for
            
            if(collisionGol){

                vidas--;

                boxPosition = (Vector2){500, 27};

                boxSpeed.y = (rand() % 5 + 7) * multiplicadorDificuldade;
                boxSpeed.x = (rand() % 11) - 5;

                if(boxSpeed.x == 0){
                    boxSpeed.x = 1;
                }//if

                if(vidas <= 0){
                    fimdejogo = true;
                }

            }//if
            
            tempoJogo += GetFrameTime();
            
            if(tempoJogo >= proximaAceleracao){
                multiplicadorDificuldade += 0.1f;
                proximaAceleracao += 20.0f;
                velocidadeGoleiro += 0.5f;
            }//if

            if(tempoJogo >= proximoPacote){
                qntPacotes++;
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
            DrawRectangleRec((Rectangle){golHitbox.x + 50, golHitbox.y + 12, 2, 80}, BLACK); 
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
            DrawRectangleRec((Rectangle){golHitbox.x + 25, golHitbox.y + 12, 2, 80}, BLACK); 
            DrawRectangleRec((Rectangle){golHitbox.x + 75, golHitbox.y + 12, 2, 80}, BLACK); 
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

                //Texto de Tempo / Dificuldade
                DrawTexturePro(imagemFundoPontuacao,(Rectangle){0, 0, imagemFundoPontuacao.width, imagemFundoPontuacao.height}, fundoMenuRec,(Vector2){0, 0}, 0,Fade(WHITE, 0.8f));
                DrawTextEx(fonteCopa, TextFormat("Tempo: %.0f", tempoJogo), (Vector2){20, 50}, 14, 2, WHITE);//Texto do tempo de jogo
                DrawTextEx(fonteCopa, TextFormat("Dificuldade: %.1fx", multiplicadorDificuldade), (Vector2){20, 30}, 14, 2, WHITE);//Texto do nível de dificuldade

                //Desenha as vidas
                DrawTexturePro(imagemFundoPontuacao, (Rectangle){0, 0, imagemFundoPontuacao.width, imagemFundoPontuacao.height }, (Rectangle){screenWidth - 195, 0, 200, 100}, (Vector2){0, 0}, 0, Fade(WHITE, 0.8f));

                if(vidas >= 1){
                    DrawTexturePro(imagemVida,(Rectangle){0,0,imagemVida.width,imagemVida.height},(Rectangle){screenWidth - 170, 20, 50, 50},(Vector2){0,0},0,WHITE);
                }//if

                if(vidas >= 2){
                    DrawTexturePro(imagemVida,(Rectangle){0,0,imagemVida.width,imagemVida.height},(Rectangle){screenWidth - 120, 20, 50, 50},(Vector2){0,0},0,WHITE);
                }//if

                if(vidas >= 3){
                    DrawTexturePro(imagemVida,(Rectangle){0,0,imagemVida.width,imagemVida.height},(Rectangle){screenWidth - 70, 20, 50, 50},(Vector2){0,0},0,WHITE);
                }//if

                EndDrawing();
            }//fim do while

            if(fimdejogo){

                Menu_FimdeJogo opcao = MENU_FIM;

                while(!WindowShouldClose()){

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

                    BeginDrawing();
                    ClearBackground(BLACK);

                    DrawText("FIM DE JOGO", 300, 150, 40, RED);

                    DrawRectangleRec(btnReiniciar, GREEN);
                    DrawText("JOGAR NOVAMENTE", 330, 370, 20, );

                    DrawRectangleRec(btnMenu, BLUE);
                    DrawText("MENU", 450, 470, 20, WHITE);

                    EndDrawing();
                }//while

                if(opcao == JOGAR_NOVAMENTE){
                    jogarNovamente = true;
                }else{
                    jogarNovamente = false;
                    UnloadFont(fonteCopa);
                    CloseWindow();

                    // Volta para o menu principal
                    menuPrincipal();
                }//if

        }//if fim de jogo

    }//while (fimdejogo)
    
    
    UnloadFont(fonteCopa);
    CloseWindow();
    return;
}