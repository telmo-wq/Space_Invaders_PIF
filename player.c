#include <raylib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "tiro.h"
#include <stdlib.h>
struct nave_status
{
    int vida;
};

int validador_nome(char *nome, int (*mat)[5]){
    for(int i=0;i<10;i++){
        int cont=0;
        for(int j=0;j<3;j++){
            if(mat[i][j]!=(int)nome[j]){
                break;
            }
            cont++;
        }
        if(cont==3){
            return 0;
        }
    }
    return 1;

}

void rankear(int (*mat)[5], int *nova) {
    
    for(int i=0;i<10;i++){
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

void listar_rank(int mat[11][5]) {
    int tableX = 50;
    int tableY = 50;
    int cellW = 200;  
    int cellH = 40;

    char texto[64];

    for (int r = 0; r < 11; r++) {

        Rectangle cell = (Rectangle) {
            tableX,
            tableY + r * cellH,
            cellW,
            cellH
        };

        DrawRectangleLinesEx(cell, 1, BLACK);

        snprintf(texto, sizeof(texto),"%c%c%c  |  %d   |  %d",
            (char)mat[r][0],(char)mat[r][1],(char)mat[r][2],mat[r][3],mat[r][4]);
        DrawText(texto, cell.x + 10, cell.y + 10, 20, BLACK);
    }
}

void levar_dano(struct tiro **tiros, Vector2 pos, struct nave_status *status, Texture2D nave){
    struct tiro *aux_tiro = *tiros;
    struct tiro *ant_tiro = NULL;
    while(aux_tiro != NULL){
        Rectangle rectTiro = {aux_tiro->laser.x, aux_tiro->laser.y, 5, 15};
        Rectangle rectjogador= {pos.x, pos.y, nave.width, nave.height};
            
        if(CheckCollisionRecs(rectjogador, rectTiro)&& aux_tiro->tipo!=0){
            status->vida=(status->vida)-1;
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