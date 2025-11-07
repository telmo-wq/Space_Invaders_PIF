#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAX_TIRO 100

struct tiro
{
    Vector2 laser;
    struct tiro *next;
};

struct Jogador
{
    Vector2 pos;
    int vida;
    int tipo;
    struct tiro tiro;

};


struct Inimigo{
  Vector2 pos;
  int tipo;
  struct tiro tiro_inimigo;
};

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
            Rectangle rectTiro = {aux->laser.x,aux->laser.y, 3, 4};
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

int main(){
    InitWindow(100, 100,"temp");
    int monitor=GetCurrentMonitor();
    int largura=GetMonitorWidth(monitor)/1.5;
    int altura=GetMonitorHeight(monitor)/1.5;
    CloseWindow();

    InitWindow(largura,altura,"Space Invaders");
    SetTargetFPS(40);
    InitAudioDevice(); 

    int pontos=0;
    struct Jogador jogador;
    struct Inimigo inimigo;
    jogador.vida=4;
    inimigo.pos.x=0;
    inimigo.pos.y=0;
    Music musica = LoadMusicStream("audios/musica.wav");
    Sound tiro= LoadSound("audios/tiro.wav");
    //Carregamento de Texturas
    Texture2D nave=LoadTexture("sprites/nave_vida_cheia.png");
    Texture2D nave_levemente_danificada=LoadTexture("sprites/nave_levemente_danificada.png");
    Texture2D nave_danificada=LoadTexture("sprites/nave_ultima_vida.png");
    Texture2D nave_ultima_vida=LoadTexture("sprites/nave_muito_danificada.png");
    Texture2D piu=LoadTexture("sprites/Zapper.png");
    Texture2D espaco=LoadTexture("sprites/final.png");
    Texture2D Nave_de_bonus=LoadTexture("sprites/Nave_de_bonus.png");


    int direct_inimigo=1;
    struct tiro *n=NULL;

    jogador.pos.x=altura-10;
    jogador.pos.y=largura/2;

    PlayMusicStream(musica);
    while(!WindowShouldClose()){
        UpdateMusicStream(musica);
        
        //Movimentação temporaria

        if(IsKeyDown(KEY_S) && jogador.pos.y+3<=altura-50){
            jogador.pos.y+=3;
        }
        if(IsKeyDown(KEY_D) && jogador.pos.x+3<=largura-50){
            jogador.pos.x+=3;
        }
        if(IsKeyDown(KEY_A) && jogador.pos.x-3>=0){
            jogador.pos.x-=3;
        }
        if(IsKeyDown(KEY_W) && jogador.pos.y-3>=0){
            jogador.pos.y-=3;
        }
        if(IsKeyPressed(KEY_E)){
            Atirar(&n, jogador.pos);
            PlaySound(tiro); 
            
        }
        //rectangle serve para criar a area de collisão
        Rectangle rectNaves_Jogador = { jogador.pos.x, jogador.pos.y, nave.width, nave.height };
        Rectangle rectNaves_de_Bonus = { inimigo.pos.x, inimigo.pos.y, Nave_de_bonus.width-20, Nave_de_bonus.height-10 };

        if(CheckCollisionRecs(rectNaves_de_Bonus,rectNaves_Jogador)){
            jogador.vida-=1;
            inimigo.pos.y = 0;
        }



        BeginDrawing();
        ClearBackground(WHITE);
        DrawTexture(espaco, 0, 0, WHITE);
        DrawText(TextFormat("SCORE: %i", pontos), 0, 0, 40, MAROON);
        
        //Vida do jogador
        switch (jogador.vida) {
            case 4:
                DrawTexture(nave,jogador.pos.x,jogador.pos.y,WHITE);
                break;
            case 3:
                DrawTexture(nave_levemente_danificada,jogador.pos.x,jogador.pos.y,WHITE);
                break;
            
            case 2:
                DrawTexture(nave_danificada,jogador.pos.x,jogador.pos.y,WHITE);
                break;

            case 1:
                DrawTexture(nave_ultima_vida,jogador.pos.x,jogador.pos.y,WHITE);
                break;

            default:
                jogador.vida=4;
                pontos=0;

        }
        DrawTexture(Nave_de_bonus,inimigo.pos.x,inimigo.pos.y,WHITE);
        
        desenho_tiro(&n,  piu);
        
        EndDrawing();
        //Movimentação do inimigo
        if(direct_inimigo){
            inimigo.pos.x += GetFrameTime() * 200;
        } else{
            inimigo.pos.x -= GetFrameTime() * 200;
        }

        //Direção para onde se move
        if (inimigo.pos.x < 0){
            direct_inimigo=1;
        } else if(inimigo.pos.x>largura-100){
            direct_inimigo=0;
        }
        Avancar_tiro(&n);
        checar_tiro(&n, rectNaves_de_Bonus, &pontos,&inimigo.pos.x);
    }
    StopMusicStream(musica);
    UnloadMusicStream(musica);
    UnloadSound(tiro);
    UnloadTexture(nave);
    UnloadTexture(nave_danificada);
    UnloadTexture(nave_levemente_danificada);
    UnloadTexture(nave_ultima_vida);
    UnloadTexture(espaco);
    UnloadTexture(piu);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}