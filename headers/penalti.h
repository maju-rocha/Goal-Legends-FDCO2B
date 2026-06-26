#ifndef PENALTI_H
#define PENALTI_H

#include <raylib.h>
#include "biblioteca.h"

void DesenharGoleiroPro(Vector2 pos, Color corUniforme);
void jogarPenalti(Figurinha *figurinhas, Figurinha *mochila, Figurinha *album, int total, int *total_mochila, int *total_album, int *qtd_pacotes);

#endif