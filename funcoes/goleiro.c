#include "raylib.h"
#include "biblioteca.h"
#include <stdlib.h>
#include <time.h>

//Struct atacantes
typedef struct {
    Rectangle rec;
    bool usado;
} Atacante;

void jogarGoleiro(){
    
    //Inicialização da tela
    const int screenWidth = 1000;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Minigame: Jogo do Goleiro");
<<<<<<< HEAD
    Font fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf");//Carrega a fonte
    
    // ==========================================
    // CÓDIGO DO MOUSE (Carregamento)
    // ==========================================
    HideCursor(); 
    Image imagemBola = LoadImage("extras/bola_cursor.png"); 
    ImageResize(&imagemBola, 40, 40); 
    Texture2D cursorBola = LoadTextureFromImage(imagemBola); 
    UnloadImage(imagemBola); 
    // ==========================================

=======
    Font fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf"); // Carrega a fonte
>>>>>>> 0a5acdb (códigos atualizados)
    SetTargetFPS(60);

    //Variáveis
    int defesas = 0;

    Atacante atacantes[2];
    int qtdAtacantes = 0;

    float tempoJogo = 0.0f;
    float proximaAceleracao = 20.0f;
    float multiplicadorDificuldade = 1.0f;
    float velocidadeGoleiro = 8.0f;

    srand(time(NULL));//inicia aleatoriedade para as direções da bola após defesa
    
    Color verdeCampo = (Color){ 98, 209, 75, 255 };
    Color verdeGol = (Color){ 64, 107, 56, 255 };
    
    Vector2 boxPosition = { 500, 20 };//Posição inicial
    Vector2 boxSpeed = {((rand() % 11) - 5) * multiplicadorDificuldade, 7.0f * multiplicadorDificuldade};//Velocidade inicial da bola de acordo com a dificuldade

    //Colisões
    Rectangle obstacleRec = {(screenWidth - 200) / 2, screenHeight - 140, 200, 30}; // Posição e tamanho do obstáculo
    Rectangle golHitbox = {(screenWidth - 800) / 2, screenHeight - 100, 800, 100}; // Posição e tamanho do gol

    while (!WindowShouldClose())
    {
        // ==========================================
        // CÓDIGO DO MOUSE (Posição)
        // ==========================================
        Vector2 mousePoint = GetMousePosition();

        boxPosition.y += boxSpeed.y;//Move a bola verticalmente
        boxPosition.x += boxSpeed.x;//Move a bola horizontalmente

        //Ricocheteia a bola no retangulo
        if((boxPosition.y < 25) || (boxPosition.y > screenHeight -25)){
            boxSpeed.y *= -1;
        }
        if((boxPosition.x < 25) || (boxPosition.x > screenWidth -25)){
            boxSpeed.x *= -1;
        }

        Rectangle boxRec = { boxPosition.x -25, boxPosition.y -25, 50, 50 };

        bool collisionGoleiro = false;
        bool collisionGol = false;

        if (CheckCollisionRecs(boxRec, obstacleRec)){
            collisionGoleiro = true;
        }else{
            collisionGoleiro = false;
        }if (CheckCollisionRecs(boxRec, golHitbox)){
            collisionGol = true;
        }else{
            collisionGol = false;
        }

        if (collisionGoleiro){
            defesas++;
            boxPosition = (Vector2){ 500, 27 };//Reseta a posição da bola

            boxSpeed.y = (rand() % 5 + 7) * multiplicadorDificuldade;//Muda a velocidade da bola de acordo com a dificuldade
            boxSpeed.x = (rand() % 11) - 5;//Muda a angulação da bola

            qtdAtacantes = rand() % 3;//Spawna de 0 a 2 atacantes

            for (int i = 0; i < qtdAtacantes; i++){
                atacantes[i].rec = (Rectangle){rand() % 750 + 50, rand() % 350 + 150, 150, 30};//Posição aleatória dos atacantes

                atacantes[i].usado = false;
            }
        }

        for(int i = 0; i < qtdAtacantes; i++){
            if(!atacantes[i].usado && CheckCollisionRecs(boxRec, atacantes[i].rec)){
                
                atacantes[i].usado = true;

                boxSpeed.x = (rand() % 11) - 5;

                if (boxSpeed.x == 0){
                    boxSpeed.x = 1;
                }//if
            }//if
        }//for

        if(collisionGol){
            boxSpeed.y *= -1;
            boxPosition = (Vector2){ 500, 27 };//Reseta a posição da bola
        }//if

        tempoJogo += GetFrameTime();

        if (tempoJogo >= proximaAceleracao){
            multiplicadorDificuldade += 0.1f;
            proximaAceleracao += 20.0f;
            velocidadeGoleiro += 0.5f;
        }//if

            //Teclas para o goleiro
            if (IsKeyDown(KEY_RIGHT)) obstacleRec.x += velocidadeGoleiro;
            if (IsKeyDown(KEY_LEFT)) obstacleRec.x -= velocidadeGoleiro;
        

        BeginDrawing();
            ClearBackground(verdeCampo);

            //Se houve colisão
            DrawCircleV(boxPosition, 29, BLACK);//Contorno da bola
            DrawCircleV(boxPosition, 25, WHITE);//Cor da bola

            //Desenha o goleiro
            DrawRectangleRec(obstacleRec, BLUE);//Cor do goleiro
            DrawRectangleLinesEx(obstacleRec, 4, BLACK);//Contorno do goleiro
            
        
            //Formato do Gol (traves)
            DrawRectangleRec((Rectangle){golHitbox.x - 4, golHitbox.y - 4, golHitbox.width + 8, golHitbox.height + 8}, BLACK); // contorno do gol
            DrawRectangleRec(golHitbox, WHITE);
            
            //Fundo do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 8, golHitbox.y + 8, golHitbox.width - 16, golHitbox.height - 16}, BLACK); // contorno do gol
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
            
            //Desenha os atacantes
            for(int i = 0; i < qtdAtacantes; i++){
                if(!atacantes[i].usado){
                    DrawRectangleRec(atacantes[i].rec, RED);//Cor dos atancantes
                    DrawRectangleLinesEx(atacantes[i].rec, 4, BLACK);//Contorno dos atacantes
                }//if
            }//for

            //Texto de defesas
            const char* textoDefesas = TextFormat("Defesas: %i", defesas);
            DrawTextEx(fonteCopa, textoDefesas, (Vector2){ 20, 20 }, 18, 2, BLACK);

            DrawTextEx(fonteCopa, TextFormat("Tempo: %.0f", tempoJogo), (Vector2){20, 60}, 18, 2, BLACK);//Texto do tempo de jogo

            DrawTextEx(fonteCopa, TextFormat("Nivel: %.1fx", multiplicadorDificuldade), (Vector2){20, 100}, 18, 2, BLACK);//Texto do nível de dificuldade

            DrawText(TextFormat("Gols: %i", gols), 10, 10, 40, BLACK);

            const char* textoGols = TextFormat("Gols: %i", gols);
            DrawTextEx(fonteCopa, textoGols, (Vector2){ 20, 20 }, 24, 2, BLACK);

            // Se houve colisão
            DrawCircleV(boxPosition, 29, BLACK); // contorno
            DrawCircleV(boxPosition, 25, WHITE); // bola

            //Se houve colisão
            DrawCircleV(boxPosition, 29, BLACK);//Contorno da bola
            DrawCircleV(boxPosition, 25, WHITE);//Cor da bola

            //Desenha o goleiro
            DrawRectangleRec(obstacleRec, BLUE);//Cor do goleiro
            DrawRectangleLinesEx(obstacleRec, 4, BLACK);//Contorno do goleiro
            
        
            //Formato do Gol (traves)
            DrawRectangleRec((Rectangle){golHitbox.x - 4, golHitbox.y - 4, golHitbox.width + 8, golHitbox.height + 8}, BLACK); // contorno do gol
            DrawRectangleRec(golHitbox, WHITE);
            
            //Fundo do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 8, golHitbox.y + 8, golHitbox.width - 16, golHitbox.height - 16}, BLACK); // contorno do gol
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
            
            //Desenha os atacantes
            for(int i = 0; i < qtdAtacantes; i++){
                if(!atacantes[i].usado){
                    DrawRectangleRec(atacantes[i].rec, RED);//Cor dos atancantes
                    DrawRectangleLinesEx(atacantes[i].rec, 4, BLACK);//Contorno dos atacantes
                }//if
            }//for

            //Texto de defesas
            const char* textoDefesas = TextFormat("Defesas: %i", defesas);
            DrawTextEx(fonteCopa, textoDefesas, (Vector2){ 20, 20 }, 18, 2, BLACK);

<<<<<<< HEAD
            DrawTextEx(fonteCopa, TextFormat("Tempo: %.0f", tempoJogo), (Vector2){20, 60}, 18, 2, BLACK);//Texto do tempo de jogo

            DrawTextEx(fonteCopa, TextFormat("Nivel: %.1fx", multiplicadorDificuldade), (Vector2){20, 100}, 18, 2, BLACK);//Texto do nível de dificuldade

            const char* textoGols = TextFormat("Gols: %i", gols);
            DrawTextEx(fonteCopa, textoGols, (Vector2){ 20, 20 }, 24, 2, BLACK);
=======
<<<<<<< HEAD
            DrawText(TextFormat("Gols: %i", gols), 10, 10, 40, BLACK);
=======
            const char* textoGols = TextFormat("Gols: %i", gols);
            DrawTextEx(fonteCopa, textoGols, (Vector2){ 20, 20 }, 24, 2, BLACK);
>>>>>>> 2664b94 (códigos atualizados)
>>>>>>> 0a5acdb (códigos atualizados)

        EndDrawing();
    }

<<<<<<< HEAD
    // ==========================================
    // CÓDIGO DO MOUSE (Limpeza)
    // ==========================================
    UnloadTexture(cursorBola);

=======
>>>>>>> 0a5acdb (códigos atualizados)
    UnloadFont(fonteCopa);
    CloseWindow();
    return;
}
