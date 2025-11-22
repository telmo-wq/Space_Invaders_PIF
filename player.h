#ifndef PLAYER_H
#define PLAYER_H

struct nave_status
{
    int vida;
};
void rankear(int (*mat)[5], int *nova);
void salvar_rank(int (*mat)[5]);
void recuperar_rank(int (*mat)[5]);
#endif
