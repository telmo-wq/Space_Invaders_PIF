#ifndef INIMIGO_H
#define INIMIGO_H

#include <raylib.h>
#include <stdbool.h>
#include "tiro.h"
#include "player.h"

struct inimigo {
    int pos_x;
    int pos_y;
    int direcao;
    Texture2D textura;
    struct inimigo *next;
};


struct inimigo *CriarInimigo(int pos_x, int pos_y);
void AdicionarInimigo(struct inimigo **lista, int pos_x, int pos_y);
void LimparInimigos(struct inimigo **lista);
void AvancarInimigos(struct inimigo **lista, int largura, int altura, struct nave_status *vida);
void DesenhoInimigos(struct inimigo **lista);
void Atirar_inimigo(struct tiro **n,struct inimigo **h);

void ChecarColisaoComInimigos(struct tiro **tiros, struct inimigo **inimigos, int *pontos);
void ChecarColisaoComPlayer(struct inimigo **inimigos, Rectangle rectJogador, struct nave_status *status);
#endif
