#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>
#define MAX_TIRO 100

struct nave_status
{
    int vida;
};

struct tiro
{
    Vector2 laser;
    struct tiro *next;
};

struct inimigo{
  int pos_x;
  int pos_y;
  struct tiro tiro_inimigo;
  struct inimigo *next;
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

typedef enum {
    MENU = 0,
    GAMEPLAY = 1,
    GAMEOVER = 2,
    GAMEWIN=3
} GameScreen;

void LimparTiros(struct tiro **lista){
    struct tiro *aux = *lista;
    while(aux != NULL){
        struct tiro *temp = aux;
        aux = aux->next;
        free(temp);
    }
    *lista = NULL;
}

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

void ChecarColisaoComInimigos(struct tiro **tiros, struct inimigo **inimigos, 
                               int *pontos, Texture2D nave_inimigo){
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
            
            if(CheckCollisionRecs(rectInimigo, rectTiro)){
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

void ChecarColisaoComPlayer(struct inimigo **inimigos, Rectangle rectJogador, 
                             struct nave_status *status, Texture2D nave_inimigo){
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

int main(){
    InitWindow(100, 100,"temp");
    int monitor=GetCurrentMonitor();
    int largura=GetMonitorWidth(monitor)/1.5;
    int altura=GetMonitorHeight(monitor)/1.5;
    CloseWindow();

    InitWindow(largura,altura,"Space Invaders");
    SetTargetFPS(40);
    InitAudioDevice(); 

    int x=largura/2;
    int y=altura-50;
    int pontos=0;
    struct nave_status status;
    status.vida=4;
    
    Music musica = LoadMusicStream("audios/musica.wav");
    Sound tiro= LoadSound("audios/tiro.wav");
    Texture2D nave=LoadTexture("sprites/nave_vida_cheia.png");
    Texture2D nave_levemente_danificada=LoadTexture("sprites/nave_levemente_danificada.png");
    Texture2D nave_danificada=LoadTexture("sprites/nave_ultima_vida.png");
    Texture2D nave_ultima_vida=LoadTexture("sprites/nave_muito_danificada.png");
    Texture2D piu=LoadTexture("sprites/Zapper.png");
    Texture2D espaco=LoadTexture("sprites/final.png");
    Texture2D Nave_de_bonus=LoadTexture("sprites/Nave_de_bonus.png");
    Texture2D nave_inimigo=LoadTexture("sprites/Nave_de_bonus.png"); 

    struct inimigo *inimigos = NULL;
    int direct_inimigo = 1;
    float tempo_spawn = 0;
    float intervalo_spawn = 2.0f;
    int onda_atual = 1;
    int inimigos_na_onda = 3;
    int inimigos_derrotados = 0;

    struct tiro *n=NULL;
    
    PlayMusicStream(musica);

    GameScreen currentScreen = MENU; 

    while(!WindowShouldClose()){
        UpdateMusicStream(musica);


        if(currentScreen == MENU){
            if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)){
                pontos = 0;
                status.vida = 4;
                x = largura/2;
                y = altura-50;
                LimparTiros(&n);
                LimparInimigos(&inimigos);
                currentScreen = GAMEPLAY;
            }
        } else if(currentScreen == GAMEOVER){
            if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)){
                pontos = 0;
                status.vida = 4;
                x = largura/2;
                y = altura-50;
                LimparTiros(&n);
                LimparInimigos(&inimigos);
                currentScreen = GAMEPLAY;
            } else if(IsKeyPressed(KEY_ESCAPE)){
                currentScreen = MENU;
            }
        } else if(currentScreen == GAMEPLAY){
            if(IsKeyDown(KEY_S) && y+3<=altura-50){
                y+=3;
            }
            if(IsKeyDown(KEY_D) && x+3<=largura-50){
                x+=3;
            }
            if(IsKeyDown(KEY_A) && x-3>=0){
                x-=3;
            }
            if(IsKeyDown(KEY_W) && y-3>=0){
                y-=3;
            }
            if(IsKeyPressed(KEY_E)){
                Vector2 pos={x,y};
                Atirar(&n, pos);
                PlaySound(tiro); 
            }
            if(IsKeyPressed(KEY_ESCAPE)){
                currentScreen = MENU;
                LimparTiros(&n);
                LimparInimigos(&inimigos);
            }

            tempo_spawn += GetFrameTime();
            if(tempo_spawn >= intervalo_spawn && inimigos_derrotados < inimigos_na_onda){
                int pos_x = 50 + rand() % (largura - 100);
                AdicionarInimigo(&inimigos, pos_x, 30);
                inimigos_derrotados++;
                tempo_spawn = 0;
            }

            if(inimigos_derrotados >= inimigos_na_onda && inimigos == NULL){
                onda_atual++;
                inimigos_na_onda = 6 + onda_atual;
                inimigos_derrotados = 0;
                tempo_spawn = 0;
            }

            if (onda_atual>10){
                currentScreen=GAMEWIN;
            }
        }

        Rectangle rectNaves_Jogador = { x, y, nave.width, nave.height };

        if(currentScreen == GAMEPLAY){
            ChecarColisaoComPlayer(&inimigos, rectNaves_Jogador, &status, nave_inimigo);
        }

        BeginDrawing();
        ClearBackground(WHITE);
        DrawTexture(espaco, 0, 0, WHITE);
        DrawText(TextFormat("SCORE: %i | ONDA: %i", pontos, onda_atual), 0, 0, 40, MAROON);

        switch(currentScreen){
            case MENU:
                DrawText("SPACE INVADERS", largura/2 - 200, altura/2 - 60, 50, BLACK);
                DrawText("Pressione ENTER ou SPACE para iniciar", largura/2 - 220, altura/2, 20, DARKGRAY);
                DrawText("Use WASD para mover, E para atirar, ESC para voltar", largura/2 - 300, altura/2 + 30, 18, DARKGRAY);
                break;

            case GAMEPLAY:
                switch (status.vida) {
                    case 4:
                        DrawTexture(nave,x,y,WHITE);
                        break;
                    case 3:
                        DrawTexture(nave_levemente_danificada,x,y,WHITE);
                        break;
                    case 2:
                        DrawTexture(nave_danificada,x,y,WHITE);
                        break;
                    case 1:
                        DrawTexture(nave_ultima_vida,x,y,WHITE);
                        break;
                    default:
                        currentScreen = GAMEOVER;
                        break;
                }

                DesenhoInimigos(&inimigos, nave_inimigo);
                desenho_tiro(&n, piu);
                break;

            case GAMEOVER:
                DrawText("GAME OVER", largura/2 - 150, altura/2 - 40, 60, RED);
                DrawText(TextFormat("SCORE: %i | ONDA: %i", pontos, onda_atual), largura/2 - 120, altura/2 + 30, 30, MAROON);
                DrawText("Pressione ENTER para reiniciar ou ESC para voltar ao menu", largura/2 - 280, altura/2 + 80, 20, DARKGRAY);
                break;
            case GAMEWIN:
                DrawText("GANHOU MISERAVI", largura/2 - 150, altura/2 - 40, 60, GREEN);
                DrawText(TextFormat("SCORE: %i", pontos), largura/2 - 120, altura/2 + 30, 30, MAROON);
                DrawText("Pressione ENTER para reiniciar ou ESC para voltar ao menu", largura/2 - 280, altura/2 + 80, 20, DARKGRAY);
        }

        EndDrawing();

        if(currentScreen == GAMEPLAY){
            if(direct_inimigo){
                AvancarInimigos(&inimigos, 1, largura);
            } else {
                AvancarInimigos(&inimigos, 0, largura);
            }

            struct inimigo *aux = inimigos;
            int min_x = largura, max_x = 0;
            while(aux != NULL){
                if(aux->pos_x < min_x) min_x = aux->pos_x;
                if(aux->pos_x > max_x) max_x = aux->pos_x;
                aux = aux->next;
            }
            if(min_x < 0) direct_inimigo = 1;
            else if(max_x > largura - 100) direct_inimigo = 0;

            Avancar_tiro(&n);
            ChecarColisaoComInimigos(&n, &inimigos, &pontos, nave_inimigo);
        }
    }

    LimparInimigos(&inimigos);
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