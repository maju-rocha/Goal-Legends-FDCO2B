#include <raylib.h>
#include "../headers/biblioteca.h" // Importante para conhecer o tipo EstadoAbertura

void desenharAnimacaoPacote(Texture2D texAnim, int *currentFrame, int totalFrames, EstadoAbertura *estado) {
    // Desenha o frame atual (textura já carregada no abrirPacote)
    DrawTexture(texAnim, 500 - texAnim.width/2, 400 - texAnim.height/2, WHITE);
    
    // Controla tempo
    static int contador = 0;
    if (++contador >= 4) {
        (*currentFrame)++;
        if (*currentFrame >= totalFrames) {
            *estado = REVELAR_CARTA;
            *currentFrame = 0;
        }
        contador = 0;
    }
}