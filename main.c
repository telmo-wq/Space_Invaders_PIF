#include <raylib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#define MAX_TIRO 100
#define MAX_ENEMY 5

struct tiro
{
    Vector2 laser;
    struct tiro *next;
};

struct Jogador
{
    Vector2 pos; //posição do jogador
    int vida;
    int tipo;
    int pontos;

};

struct inimigo{
  int pos_x;
  int pos_y;  
  struct tiro *tiro_inimigo;
};

struct tiro *CriarTiro(Vector2 pos){
    struct tiro *n=(struct tiro*)malloc(sizeof(struct tiro));
    n->laser=pos;
    n->next=NULL;
    return n;
}

struct inimigo *inimigos = NULL;
int inimigocount = 8;
int inimigo_capacity = 8;


void Desenho_inimigo(Texture2D inimigo_texture, int y){
    for(int i=0;i<inimigocount;i++){
        DrawTexture(inimigo_texture,100,y, WHITE);
    }
}

struct inimigo* InimigoTryAdd(int pos_x, int pos_y){
    if (inimigocount >= inimigo_capacity){
        int newcap = (inimigo_capacity == 0) ? 4 : inimigo_capacity * 2; 
        struct inimigo *tmp = realloc(inimigos, sizeof(struct inimigo) * newcap);
        if (!tmp) return NULL;
        inimigos = tmp;
        inimigo_capacity = newcap;
    }
    struct inimigo *inimigo = &inimigos[inimigocount++];
    inimigo->pos_x = pos_x;
    inimigo->pos_y = pos_y;
    inimigo->tiro_inimigo = NULL;
    return inimigo;
}

void InimigoInit(){
    inimigo_capacity= 8;
    inimigos = malloc(sizeof(struct inimigo) * inimigo_capacity);
    inimigocount = 0;
    for (int i = 0; i < 5; i++){
        InimigoTryAdd(100, 30);
    }
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
    GAMEOVER = 2
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

void InitGame(){
    InimigoInit();
   // InimigoTryAdd(GetRandomValue(0, 100), 30);
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
    InitGame();

    struct Jogador jogador;

    jogador.pos.x=largura/2;
    jogador.pos.y=altura-50;
    jogador.pontos=0;
    jogador.vida=4;
    
    Music musica = LoadMusicStream("audios/musica.wav");
    Sound tiro= LoadSound("audios/tiro.wav");
    Texture2D nave=LoadTexture("sprites/nave_vida_cheia.png");
    Texture2D nave_levemente_danificada=LoadTexture("sprites/nave_levemente_danificada.png");
    Texture2D nave_danificada=LoadTexture("sprites/nave_ultima_vida.png");
    Texture2D nave_ultima_vida=LoadTexture("sprites/nave_muito_danificada.png");
    Texture2D piu=LoadTexture("sprites/Zapper.png");
    Texture2D espaco=LoadTexture("sprites/final.png");
    Texture2D Nave_de_bonus=LoadTexture("sprites/Nave_de_bonus.png");

    Vector2 posicao_inimigo={GetRandomValue(0, 100),30};

    int direct_inimigo=1;
    struct tiro *n=NULL;
    float spawnTimer = 0.0f;
    float spawnInterval = 1.5f;
    int y = 0;
    float temp = 0;

    PlayMusicStream(musica);

    GameScreen currentScreen = MENU; 

    while(!WindowShouldClose()){
        UpdateMusicStream(musica);
        temp++;

        if(currentScreen == MENU){
            if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)){
                jogador.pontos = 0;
                jogador.vida = 4;
                jogador.pos.x = largura/2;
                jogador.pos.y = altura-50;
                direct_inimigo=1;
                posicao_inimigo.x = GetRandomValue(0, 100); posicao_inimigo.y = 30;
                LimparTiros(&n);
                currentScreen = GAMEPLAY;
            }
        } else if(currentScreen == GAMEOVER){
            if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)){
                jogador.pontos = 0;
                jogador.vida = 4;
                jogador.pos.x = largura/2;
                jogador.pos.y = altura-50;
                posicao_inimigo.x = GetRandomValue(0, 100); posicao_inimigo.y = 30;
                LimparTiros(&n);
                currentScreen = GAMEPLAY;
            } else if(IsKeyPressed(KEY_ESCAPE)){
                currentScreen = MENU;
            }
        } else if(currentScreen == GAMEPLAY){
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
            if(IsKeyPressed(KEY_ESCAPE)){
                currentScreen = MENU;
                LimparTiros(&n);
            }


            if (posicao_inimigo.y > altura){
                posicao_inimigo.x = GetRandomValue(0, largura - Nave_de_bonus.width);
                posicao_inimigo.y = 30;
                direct_inimigo = 1;
            }
        }

        Rectangle rectNaves_Jogador = { jogador.pos.x, jogador.pos.y, nave.width, nave.height };
        Rectangle rectNaves_de_Bonus = { posicao_inimigo.x, posicao_inimigo.y, Nave_de_bonus.width, Nave_de_bonus.height };

        if(currentScreen == GAMEPLAY){
            if(CheckCollisionRecs(rectNaves_de_Bonus,rectNaves_Jogador)){
                jogador.vida-=1;
                posicao_inimigo.y = 0;
            }
        }

        BeginDrawing();
        ClearBackground(WHITE);
        DrawTexture(espaco, 0, 0, WHITE);
        DrawText(TextFormat("SCORE: %i", jogador.pontos), 0, 0, 40, MAROON);

        switch(currentScreen){
            case MENU:
                DrawText("SPACE INVADERS", largura/2 - 200, altura/2 - 60, 50, BLACK);
                DrawText("Pressione ENTER ou SPACE para iniciar", largura/2 - 220, altura/2, 20, DARKGRAY);
                DrawText("Use WASD para mover, E para atirar, ESC para voltar", largura/2 - 300, altura/2 + 30, 18, DARKGRAY);
                break;

            case GAMEPLAY:
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
                        currentScreen = GAMEOVER;
                        break;
                }
                spawnTimer += GetFrameTime();
                if (spawnTimer >= spawnInterval){
                    spawnTimer = 0.0f;
                    InimigoTryAdd(GetRandomValue(0, largura - Nave_de_bonus.width), 30);
                }
                Desenho_inimigo(Nave_de_bonus, y);
                DrawTexture(Nave_de_bonus,posicao_inimigo.x,posicao_inimigo.y,WHITE);
                desenho_tiro(&n,  piu);
                break;

            case GAMEOVER:
                DrawText("GAME OVER", largura/2 - 150, altura/2 - 40, 60, RED);
                DrawText(TextFormat("SCORE: %i", jogador.pontos), largura/2 - 80, altura/2 + 30, 30, MAROON);
                DrawText("Pressione ENTER para reiniciar ou ESC para voltar ao menu", largura/2 - 280, altura/2 + 80, 20, DARKGRAY);
                break;
        }

        EndDrawing();

        y += 3;

        if(currentScreen == GAMEPLAY){
            Desenho_inimigo(Nave_de_bonus, y);
            if(direct_inimigo){
                posicao_inimigo.y += GetFrameTime() * 200;
            } else{
                posicao_inimigo.y -= GetFrameTime() * 200;
            }
            if (posicao_inimigo.x < 0){
                direct_inimigo=1;
            } else if(posicao_inimigo.x>largura-100){
                direct_inimigo=0;
            }
            Avancar_tiro(&n);
            checar_tiro(&n, rectNaves_de_Bonus, &jogador.pontos,&posicao_inimigo.x);
        }
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
    if (inimigos) free(inimigos);
    CloseWindow();
    return 0;
}