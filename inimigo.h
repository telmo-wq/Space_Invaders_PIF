#ifndef INIMIGO_H
#define INIMIGO_H

#include <raylib.h>
#include <stdbool.h>
#include "tiro.h"
#include "player.h"

struct inimigo {
    int pos_x;
    int pos_y;
    struct tiro tiro_inimigo;
    struct inimigo *next;
};


struct inimigo *CriarInimigo(int pos_x, int pos_y);
void AdicionarInimigo(struct inimigo **lista, int pos_x, int pos_y);
void LimparInimigos(struct inimigo **lista);
void AvancarInimigos(struct inimigo **lista, int direct, int largura);
void DesenhoInimigos(struct inimigo **lista, Texture2D nave_inimigo);

void ChecarColisaoComInimigos(struct tiro **tiros, struct inimigo **inimigos, int *pontos, Texture2D nave_inimigo);
void ChecarColisaoComPlayer(struct inimigo **inimigos, Rectangle rectJogador, struct nave_status *status, Texture2D nave_inimigo);
#endif
