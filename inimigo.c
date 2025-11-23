#include <raylib.h>
#include <stdbool.h>
#include "inimigo.h"
#include <stdlib.h>
struct inimigo *CriarInimigo(int pos_x, int pos_y){
    struct inimigo *novo = (struct inimigo*)malloc(sizeof(struct inimigo));
    novo->pos_x = pos_x;
    novo->pos_y = pos_y;
    novo->next = NULL;
    return novo;
}

void AdicionarInimigo(struct inimigo **lista, int pos_x, int pos_y){
    struct inimigo *aux = *lista;
    struct inimigo *novo = CriarInimigo(pos_x, pos_y);
    if(aux == NULL){
        *lista = novo;
    } else {
        while(aux->next != NULL){
            aux = aux->next;
        }
        aux->next = novo;
    }
}

void LimparInimigos(struct inimigo **lista){
    struct inimigo *aux = *lista;
    while(aux != NULL){
        struct inimigo *temp = aux;
        aux = aux->next;
        free(temp);
    }
    *lista = NULL;
}

void AvancarInimigos(struct inimigo **lista, int direct, int largura){
    struct inimigo *aux = *lista;
    while(aux != NULL){
        if(direct){
            aux->pos_x += GetFrameTime() * 150;
        } else {
            aux->pos_x -= GetFrameTime() * 150;
        }
        aux = aux->next;
    }
}

void DesenhoInimigos(struct inimigo **lista, Texture2D nave_inimigo){
    struct inimigo *aux = *lista;
    while(aux != NULL){
        DrawTexture(nave_inimigo, aux->pos_x, aux->pos_y, WHITE);
        aux = aux->next;
    }
}



void ChecarColisaoComInimigos(struct tiro **tiros, struct inimigo **inimigos, int *pontos, Texture2D nave_inimigo){
    struct tiro *aux_tiro = *tiros;
    struct tiro *ant_tiro = NULL;
    
    while(aux_tiro != NULL){
        struct inimigo *aux_ini = *inimigos;
        struct inimigo *ant_ini = NULL;
        int acertou = 0;
        
        while(aux_ini != NULL){
            Rectangle rectTiro = {aux_tiro->laser.x, aux_tiro->laser.y, 5, 15};
            Rectangle rectInimigo = {aux_ini->pos_x, aux_ini->pos_y, 
                                     nave_inimigo.width, nave_inimigo.height};
            
            if(CheckCollisionRecs(rectInimigo, rectTiro)&& aux_tiro->tipo==0){
                struct inimigo *temp_ini = aux_ini;
                if(ant_ini == NULL){
                    *inimigos = (*inimigos)->next;
                    aux_ini = *inimigos;
                } else {
                    ant_ini->next = aux_ini->next;
                    aux_ini = aux_ini->next;
                }
                free(temp_ini);
                *pontos += 100;
                acertou = 1;
                break;
            } else {
                ant_ini = aux_ini;
                aux_ini = aux_ini->next;
            }
        }
        
        if(acertou){
            struct tiro *temp_tiro = aux_tiro;
            if(ant_tiro == NULL){
                *tiros = (*tiros)->next;
                aux_tiro = *tiros;
            } else {
                ant_tiro->next = aux_tiro->next;
                aux_tiro = aux_tiro->next;
            }
            free(temp_tiro);
        } else if(aux_tiro->laser.y < 0){
            struct tiro *temp_tiro = aux_tiro;
            if(ant_tiro == NULL){
                *tiros = (*tiros)->next;
                aux_tiro = *tiros;
            } else {
                ant_tiro->next = aux_tiro->next;
                aux_tiro = aux_tiro->next;
            }
            free(temp_tiro);
        } else {
            ant_tiro = aux_tiro;
            aux_tiro = aux_tiro->next;
        }
    }
}

void ChecarColisaoComPlayer(struct inimigo **inimigos, Rectangle rectJogador, struct nave_status *status, Texture2D nave_inimigo){
    struct inimigo *aux = *inimigos;
    struct inimigo *ant = NULL;
    
    while(aux != NULL){
        Rectangle rectInimigo = {aux->pos_x, aux->pos_y, 
                                 nave_inimigo.width, nave_inimigo.height};
        
        if(CheckCollisionRecs(rectInimigo, rectJogador)){
            status->vida -= 1;
            struct inimigo *temp = aux;
            if(ant == NULL){
                *inimigos = (*inimigos)->next;
                aux = *inimigos;
            } else {
                ant->next = aux->next;
                aux = aux->next;
            }
            free(temp);
        } else {
            ant = aux;
            aux = aux->next;
        }
    }
}


void Atirar_inimigo(struct tiro **n,struct inimigo **h){
    struct inimigo *aux=*h;
    while (aux!=NULL) {
        int atira=GetRandomValue(0, 999);
        if(atira==0){
            Vector2 pos={aux->pos_x,aux->pos_y};
                Atirar(n, pos,1); 
        }
        aux=aux->next;
    }
}