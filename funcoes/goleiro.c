#include "raylib.h"
#include "biblioteca.h"


void jogarGoleiro(){
    // --- Inicialização ---
    const int screenWidth = 1000;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Minigame: Jogo do Goleiro");
    Font fonteCopa = LoadFont("extras/PressStart2P-Regular.ttf"); // Carrega a fonte
    SetTargetFPS(60);

    int gols = 0;
    
    Color verdeCampo = (Color){ 98, 209, 75, 255 };
    Color verdeGol = (Color){ 64, 107, 56, 255 };
    
    Vector2 boxPosition = { 500, 20 }; // Posição inicial
    Vector2 boxSpeed = { 6.0f, 6.0f }; // Velocidade: 4 pixels/frame no eixo X, 0 no eixo Y

    // Colisões/Flags
    Rectangle obstacleRec = {(screenWidth - 200) / 2, screenHeight - 140, 200, 30}; // Posição e tamanho do obstáculo
    Rectangle golHitbox = {(screenWidth - 800) / 2, screenHeight - 100, 800, 100}; // Posição e tamanho do gol

    while (!WindowShouldClose())
    {
        // --- Lógica ---
        boxPosition.y += boxSpeed.y; // Move a caixa
        boxPosition.x += boxSpeed.x; // Move a caixa

        // Ricocheteia a bola no retangulo
        if ((boxPosition.y < 25) || (boxPosition.y > screenHeight -25)){
            boxSpeed.y *= -1;
        }
        if ((boxPosition.x < 25) || (boxPosition.x > screenWidth -25)){
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
            boxSpeed.y *= -1;
            if(boxPosition.x > (obstacleRec.x + obstacleRec.width/2))
                boxSpeed.x *= +1;
            else
                boxSpeed.x *= -1;
        }

        if(collisionGol){
            gols++;
            boxSpeed.y *= -1;
            boxPosition = (Vector2){ 500, 27 }; // Reseta a posição da bola
        }
            if (IsKeyDown(KEY_RIGHT)) obstacleRec.x += 6.0f;
            if (IsKeyDown(KEY_LEFT)) obstacleRec.x -= 6.0f;

        // --- Desenho ---
        BeginDrawing();
            ClearBackground(verdeCampo);
    
            // Se houve colisão
            DrawCircleV(boxPosition, 29, BLACK); // contorno
            DrawCircleV(boxPosition, 25, WHITE); // bola

            // Desenha o obstáculo
            DrawRectangleRec(obstacleRec, BLUE);
            
            DrawRectangleRec((Rectangle){golHitbox.x - 4, golHitbox.y - 4, golHitbox.width + 8, golHitbox.height + 8}, BLACK); // contorno do gol
            DrawRectangleRec(golHitbox, WHITE);
            
            DrawRectangleRec((Rectangle){golHitbox.x + 8, golHitbox.y + 8, golHitbox.width - 16, golHitbox.height - 16}, BLACK); // contorno do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 12, golHitbox.y + 12, golHitbox.width - 24, golHitbox.height - 24}, verdeGol); // gol

            DrawRectangleRec((Rectangle){golHitbox.x + 50, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 100, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 150, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 200, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 250, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 300, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 350, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 400, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 450, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 500, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 550, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 600, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 650, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 700, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 750, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol

            DrawRectangleRec((Rectangle){golHitbox.x + 25, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 75, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 125, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 175, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 225, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 275, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 325, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 375, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 425, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 475, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 525, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 575, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 625, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 675, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 725, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 775, golHitbox.y + 12, 2, 80}, BLACK); // linha dentro do gol

            DrawRectangleRec((Rectangle){golHitbox.x + 10, golHitbox.y + 35, 780, 2}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 10, golHitbox.y + 62, 780, 2}, BLACK); // linha dentro do gol
            DrawRectangleRec((Rectangle){golHitbox.x + 10, golHitbox.y + 90, 780, 2}, BLACK); // linha dentro do gol
            


            const char* textoGols = TextFormat("Gols: %i", gols);
            DrawTextEx(fonteCopa, textoGols, (Vector2){ 20, 20 }, 24, 2, BLACK);

        EndDrawing();
    }

    UnloadFont(fonteCopa);
    CloseWindow();
    return;
}