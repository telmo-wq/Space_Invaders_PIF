#include <raylib.h>
#define MAX_TIRO 100

struct tiro
{
    Vector2 laser;
};

struct inimigo{
  int pos_x;
  int pos_y;  
  struct tiro tiro_inimigo;
};


int main(){
    
    int largura=1000;
    int altura=800;

    InitWindow(largura,altura,"Space Invaders");
    SetTargetFPS(40);
    InitAudioDevice(); 

    int x=largura/2;
    int y=altura-50;
    int a=0,pontos=0;
    struct tiro tiros[MAX_TIRO];
    
    Music musica = LoadMusicStream("audios/musica.wav");
    Sound tiro= LoadSound("audios/tiro.wav");
    Texture2D nave=LoadTexture("sprites/nave_vida_cheia.png");
    Texture2D piu=LoadTexture("sprites/Zapper.png");
    Texture2D espaco=LoadTexture("sprites/final.png");
    Texture2D Nave_de_bonus=LoadTexture("sprites/Nave_de_bonus.png");

    Vector2 posicao_inimigo={400,30};
    
    PlayMusicStream(musica);
    while(!WindowShouldClose()){
        UpdateMusicStream(musica);

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
            tiros[a].laser.x=x;
            tiros[a].laser.y=y;
            a++;
            if(a==MAX_TIRO){
                a=0;
            }
            PlaySound(tiro); 
            
        }
        Rectangle rectNaves_de_Bonus = { posicao_inimigo.x, posicao_inimigo.y, Nave_de_bonus.width, Nave_de_bonus.height };
        for (int i = 0; i < a; i++){
            tiros[i].laser.y-=10;
            Rectangle rectTiro = {tiros[i].laser.x,tiros[i].laser.y, 5, 15};
            if(CheckCollisionRecs(rectNaves_de_Bonus,rectTiro)){
                pontos+=100;
                tiros[i].laser.y=-10;

            }
        }
        
        


        BeginDrawing();
        ClearBackground(WHITE);
        DrawTexture(espaco, 0, 0, WHITE);
        DrawTexture(nave,x,y,WHITE);
        DrawText(TextFormat("SCORE: %i", pontos), 0, 0, 40, MAROON);
        DrawTexture(Nave_de_bonus,posicao_inimigo.x,posicao_inimigo.y,WHITE);
        for (int i = 0; i < a; i++){
            if(tiros[i].laser.y>0){
            DrawTexture(piu,tiros[i].laser.x,tiros[i].laser.y,WHITE);
            }
        }   
        
        EndDrawing();

        posicao_inimigo.y += GetFrameTime() * 200;

        if (posicao_inimigo.y > 700){
            posicao_inimigo.y = 30;
        }
    }
    StopMusicStream(musica);
    UnloadMusicStream(musica);
    UnloadSound(tiro);
    UnloadTexture(nave);
    UnloadTexture(espaco);
    UnloadTexture(piu);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}