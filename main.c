#include <raylib.h>
#include <stdbool.h>
#include "inimigo.h"
#include "tiro.h"
#include <stdlib.h>
#include "teste_titulo.h"
#define MAX_TIRO 100





typedef enum {
    MENU = 0,
    GAMEPLAY = 1,
    GAMEOVER = 2,
    GAMEWIN=3
} GameScreen;


int main(){
    InitWindow(100, 100,"temp");
    int monitor=GetCurrentMonitor();
    int largura=GetMonitorWidth(monitor)/1.5;
    int altura=GetMonitorHeight(monitor)/1.5;
    CloseWindow();

    titulo();

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
    int ranking[11][5]={0};
    int cont_caracter=0;
    char nome_usuario[4];
    int chave=1;

    recuperar_rank(ranking);
    struct tiro *n=NULL;
    
    PlayMusicStream(musica);

    GameScreen currentScreen = MENU; 

    while(!WindowShouldClose()){
        UpdateMusicStream(musica);


        if(currentScreen == MENU){
            int caractere=GetCharPressed();
            while (caractere>0)
            {
                if(caractere>=97 && caractere<=122){
                    caractere-=32;
                }
                if(caractere>=65 && caractere<=90 && cont_caracter<3){
                    nome_usuario[cont_caracter]=(char)caractere;
                    cont_caracter++;
                    nome_usuario[cont_caracter]='\0';
                }
                caractere = GetCharPressed();
            }
            
            if(IsKeyPressed(KEY_BACKSPACE) && cont_caracter >0){
                    cont_caracter--;
                    nome_usuario[cont_caracter]='\0';
            }

            if ((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) && cont_caracter==3) {
                pontos = 0;
                chave=1;
                status.vida = 4;
                x = largura/2;
                y = altura-50;
                LimparTiros(&n);
                LimparInimigos(&inimigos);
                currentScreen = GAMEPLAY;
            }

        } else if(currentScreen == GAMEOVER){
                int lista[5];
                lista[0] = nome_usuario[0];
                lista[1] = nome_usuario[1];
                lista[2] = nome_usuario[2];
                lista[4]=pontos;
                lista[3]=onda_atual;
                if(chave){
                    rankear(ranking,lista);
                    chave=0;
                }
            if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)){
                pontos = 0;
                status.vida = 4;
                x = largura/2;
                y = altura-50;
                LimparTiros(&n);
                LimparInimigos(&inimigos);
                currentScreen = GAMEPLAY;
            } else if(IsKeyPressed(KEY_BACKSPACE)){
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
                Atirar(&n, pos,0);
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
                int lista[5];
                lista[0] = nome_usuario[0];
                lista[1] = nome_usuario[1];
                lista[2] = nome_usuario[2];
                lista[4]=pontos;
                lista[3]=onda_atual;
                if(chave){
                    rankear(ranking,lista);
                    chave=0;
                }
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
                DrawText("SPACE INVADERS", largura/2 - 200, (altura/2) - 300, 50, BLACK);
                DrawRectangleLines((largura/2)-100, (altura/2) -50 ,70 , 30, BLACK);
                DrawText(nome_usuario, (largura/2)-95 , (altura/2) -50 , 30, BLACK);
                DrawText("Pressione ENTER ou SPACE para iniciar", largura/2 - 220, altura/2+ 30, 20, DARKGRAY);
                DrawText("Use WASD para mover, E para atirar, ESC para voltar", largura/2 - 300, altura/2 + 60, 18, DARKGRAY);
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

            Atirar_inimigo(&n, &inimigos);
            Avancar_tiro(&n);
            Vector2 pos={x,y};
            levar_dano(&n,  pos, &status, nave);
            ChecarColisaoComInimigos(&n, &inimigos, &pontos, nave_inimigo);
        }
    }
    salvar_rank(ranking);
    LimparInimigos(&inimigos);
    StopMusicStream(musica);
    UnloadMusicStream(musica);
    UnloadSound(tiro);
    UnloadTexture(nave);
    UnloadTexture(Nave_de_bonus);
    UnloadTexture(nave_inimigo);
    UnloadTexture(nave_danificada);
    UnloadTexture(nave_levemente_danificada);
    UnloadTexture(nave_ultima_vida);
    UnloadTexture(espaco);
    UnloadTexture(piu);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}