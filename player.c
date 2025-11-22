#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
struct nave_status
{
    int vida;
};


void rankear(int (*mat)[5], int *nova) {
    if(mat[0][4]<nova[4]){
        for (int i = 0; i <10; i++) {
            for(int j=0;j<5;j++){
                mat[i+1][j]=mat[i][j];
            }
        }
        for(int i=0;i<5;i++){
            mat[0][i]=nova[i];
        }
    } else{
        for(int i=1;i<10;i++){
            if(mat[i][4]<nova[4]){
                for (int j = 0;j <10; j++) {
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