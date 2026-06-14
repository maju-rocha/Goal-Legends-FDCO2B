#include "raylib.h"
#include "biblioteca.h"


void jogarGoleiro(){
    // --- Inicialização ---
    const int screenWidth = 1000;
    const int screenHeight = 800;
    InitWindow(screenWidth, screenHeight, "Minigame: Jogo do Goleiro");
    SetTargetFPS(60);

    Color verdeCampo = (Color){ 98, 209, 75, 255 };

    Vector2 boxPosition = { 500, 20 }; // Posição inicial
    Vector2 boxSpeed = { 6.0f, 6.0f }; // Velocidade: 4 pixels/frame no eixo X, 0 no eixo Y

    // Colisões/Flags
    Rectangle obstacleRec = {(screenWidth - 200) / 2, screenHeight - 30 - 20, 200, 30}; // Posição e tamanho do obstáculo
    bool collision = false;

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

        Rectangle boxRec = { boxPosition.x, boxPosition.y, 25, 25 };

        if (CheckCollisionRecs(boxRec, obstacleRec)){
            collision = true;
        } else{
            collision = false;
        }

        if (collision){
            boxSpeed.y *= -1;
            if(boxPosition.y > (obstacleRec.x + obstacleRec.width/2))
                boxSpeed.x *= +1;
            else
                boxSpeed.x *= -1;
        
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

            

        EndDrawing();
    }

    CloseWindow();
    return;
}