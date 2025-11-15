#include <raylib.h>
#include <stdbool.h>
#include "tiro.h"
#include <stdlib.h>

struct tiro *CriarTiro(Vector2 pos){
    struct tiro *n=(struct tiro*)malloc(sizeof(struct tiro));
    n->laser=pos;
    n->next=NULL;
    return n;
}

void Atirar(struct tiro **lista,Vector2 pos){
    struct tiro *aux=*lista;
    struct tiro *novo=CriarTiro(pos);
    if(aux==NULL){
        *lista=novo;
    } else{
        while(aux->next!=NULL){
            aux=aux->next;
        }
        aux->next=novo;
    }
}
void Avancar_tiro(struct tiro **lista){
    struct tiro *aux=*lista;
    if(aux!=NULL){
        while (aux!=NULL)
        {
            aux->laser.y-=10;
            aux=aux->next;
        }

    }
}

void checar_tiro(struct tiro **lista,Rectangle rectNaves_de_Bonus,int *ponto,float *pos){
    struct tiro *aux=*lista;
    struct tiro *ant=NULL;
    if(aux!=NULL){
        while (aux!=NULL)
        {
            Rectangle rectTiro = {aux->laser.x,aux->laser.y, 5, 15};
            if(CheckCollisionRecs(rectNaves_de_Bonus,rectTiro)){
                struct tiro *temp = aux;
                if(ant == NULL){
                    (*lista) = (*lista)->next;
                    aux = (*lista);
                    *ponto += 100;
                    *pos=10;
                } else {
                    ant->next = aux->next;
                    aux = aux->next;
                    *ponto += 100;
                    *pos=10;
                }
                free(temp);
            } else if(aux->laser.y<0){
                struct tiro *temp = aux;
                if(ant == NULL){
                    (*lista)=(*lista)->next;
                    aux = (*lista);

                }else{
                    ant->next = aux->next;
                    aux = aux->next;
                }
                free(temp);

            }else{
                ant=aux;
                aux=aux->next;

            }
        }

    }
}

void desenho_tiro(struct tiro **lista,Texture2D piu){
    struct tiro *aux=*lista;
    while(aux!=NULL){
            DrawTexture(piu,aux->laser.x,aux->laser.y,WHITE);
            aux=aux->next;
        }

}

void LimparTiros(struct tiro **lista){
    struct tiro *aux = *lista;
    while(aux != NULL){
        struct tiro *temp = aux;
        aux = aux->next;
        free(temp);
    }
    *lista = NULL;
}