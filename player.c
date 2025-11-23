#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include "tiro.h"
#include <stdlib.h>
struct nave_status
{
    int vida;
};


void rankear(int (*mat)[5], int *nova) {
    
    for(int i=1;i<10;i++){
        if(mat[i][4]<nova[4]){
            for (int j = 9;j >=i; j--) {
                for(int x=0;x<5;x++){
                    mat[j+1][x]=mat[j][x];
                    }
                }
            for(int j=0;j<5;j++){
                mat[i][j]=nova[j];
            }
            return;
            }
        }

    }

void levar_dano(struct tiro **tiros, Vector2 pos, struct nave_status *status, Texture2D nave){
    struct tiro *aux_tiro = *tiros;
    struct tiro *ant_tiro = NULL;
    while(aux_tiro != NULL){
        Rectangle rectTiro = {aux_tiro->laser.x, aux_tiro->laser.y, 5, 15};
        Rectangle rectjogador= {pos.x, pos.y, nave.width, nave.height};
        int acertou=0;
            
        if(CheckCollisionRecs(rectjogador, rectTiro)&& aux_tiro->tipo!=0){
            status->vida=(status->vida)-1;
            acertou=1;
            struct tiro *temp_tiro = aux_tiro;
            if(ant_tiro == NULL){
                *tiros = (*tiros)->next;
                aux_tiro = *tiros;
            } else {
                ant_tiro->next = aux_tiro->next;
                aux_tiro = aux_tiro->next;
            }
            free(temp_tiro);
            }   else{
            ant_tiro = aux_tiro;
            aux_tiro = aux_tiro->next;

        }

    } 
        
}


void salvar_rank(int (*mat)[5]){
    FILE *rank=fopen("rank.txt", "w+");
    for(int i=0;i<11;i++){
        for(int j=0;j<5;j++){
            fprintf(rank, "%d ", mat[i][j]);
        }
        fprintf(rank, "\n");
    }
    fclose(rank);
}
void recuperar_rank(int (*mat)[5]){
    FILE *rank=fopen("rank.txt", "r");
    for(int i=0;i<11;i++){
        for(int j=0;j<5;j++){
            fscanf(rank, "%d", &mat[i][j]);
        }
    }
    fclose(rank);
}