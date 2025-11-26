#ifndef PLAYER_H
#define PLAYER_H
#include "tiro.h"

struct nave_status
{
    int vida;
};
void rankear(int (*mat)[5], int *nova);
void salvar_rank(int (*mat)[5]);
void recuperar_rank(int (*mat)[5]);
int validador_nome(char *nome, int (*mat)[5]);
void levar_dano(struct tiro **tiros, Vector2 pos, struct nave_status *status, Texture2D nave);
void listar_rank(int (*mat)[5]);
#endif
