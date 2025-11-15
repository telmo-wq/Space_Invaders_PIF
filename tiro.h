#ifndef TIRO_H
#define TIRO_H

#include <raylib.h>
#include <stdbool.h>

struct tiro {
    Vector2 laser;
    struct tiro *next;
};

struct tiro *CriarTiro(Vector2 pos);
void Atirar(struct tiro **lista, Vector2 pos);
void Avancar_tiro(struct tiro **lista);
void checar_tiro(struct tiro **lista, Rectangle rectNaves_de_Bonus, int *ponto, float *pos);
void desenho_tiro(struct tiro **lista, Texture2D piu);
void LimparTiros(struct tiro **lista);

#endif
