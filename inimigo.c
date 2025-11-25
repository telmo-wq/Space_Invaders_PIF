#include <raylib.h>
#include <stdbool.h>
#include "inimigo.h"
#include <stdlib.h>
struct inimigo *CriarInimigo(int pos_x, int pos_y){
    struct inimigo *novo = (struct inimigo*)malloc(sizeof(struct inimigo));
    novo->pos_x = pos_x;
    novo->pos_y = pos_y;
    novo->direcao = GetRandomValue(0, 1);
    int tipo=GetRandomValue(0, 3);
    switch (tipo) {
        case 0:
        novo->textura = ;
        break;
        case 1:
        novo->textura = ;
        break;
        case 2:
        novo->textura = ;
        break;
        case 3:
        novo->textura = ;
        break;

    }
    novo->textura = ;
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

void colisao_entre_inimigos(struct inimigo **lista,struct inimigo *atual, Texture2D nave_inimiga){
    struct inimigo *aux_ini = *lista;
    Rectangle rectInimigo_atual= {atual->pos_x, atual->pos_y, 
                                    (nave_inimiga.width)-50, (nave_inimiga.height)-50};
    while(aux_ini != NULL){
        Rectangle rectInimigo= {aux_ini->pos_x, aux_ini->pos_y, 
                                    (nave_inimiga.width)-50, (nave_inimiga.height)-50};
        if(CheckCollisionRecs(rectInimigo_atual, rectInimigo)){
            if(atual->direcao==0){
                atual->direcao=1;
                aux_ini->direcao=0;
            }else{
                atual->direcao=0;
                aux_ini->direcao=1;
            }
            break;
        }
        aux_ini=aux_ini->next;
        
    }

}

void AvancarInimigos(struct inimigo **lista, int largura,Texture2D nave_inimiga){
    struct inimigo *aux = *lista;
    while(aux != NULL){
        colisao_entre_inimigos(lista, aux, nave_inimiga);
        if(aux->pos_x<0){
            aux->pos_y+=50;
            aux->direcao=1;
        } else if(aux->pos_x>largura-100){
            aux->pos_y+=30;
            aux->direcao=0;

        }
        if(aux->direcao==1){
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



void ChecarColisaoComInimigos(struct tiro **tiros, struct inimigo **inimigos, int *pontos, Texture2D nave_inimiga){
    struct tiro *aux_tiro = *tiros;
    struct tiro *ant_tiro = NULL;
    
    while(aux_tiro != NULL){
        struct inimigo *aux_ini = *inimigos;
        struct inimigo *ant_ini = NULL;
        int acertou = 0;
        
        while(aux_ini != NULL){
            Rectangle rectTiro = {aux_tiro->laser.x, aux_tiro->laser.y, 5, 15};
            Rectangle rectInimigo = {aux_ini->pos_x, aux_ini->pos_y, 
                                     (nave_inimiga.width)-20, (nave_inimiga.height)-20};
            
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
        int atira=GetRandomValue(0, 99);
        if(atira==0){
            Vector2 pos={aux->pos_x,aux->pos_y};
                Atirar(n, pos,1); 
        }
        aux=aux->next;
    }
}