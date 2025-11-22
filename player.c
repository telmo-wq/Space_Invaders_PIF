#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
struct nave_status
{
    int vida;
};


void rankear(int (*mat)[5]) {
    int temp[5];
    for(int i=0;i<5;i++){
        temp[i]=mat[10][i];
    }
    if(mat[0][4]<temp[4]){
        for (int i = 9; i >= 0; i--) {
            for(int j=0;j<5;j++){
                mat[i+1][j]=mat[i][j];
            }
        }
        for(int i=0;i<5;i++){
            mat[0][i]=temp[i];
        }
    } else{
        for(int i=1;i<10;i++){
            if(mat[i][4]<temp[4]){
                for (int j = 9; j >= i; j--) {
                    for(int x=0;x<5;x++){
                        mat[j+1][x]=mat[j][x];
                    }
                }
                for(int j=0;j<5;j++){
                    mat[i][j]=temp[j];
                }
                break;
            }
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
            fscanf(rank, "%d", mat[i][j]);
        }
    }
    fclose(rank);
}