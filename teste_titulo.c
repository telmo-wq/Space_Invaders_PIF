
#include <raylib.h>

int main(void)
{
    
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [text] example - writing anim");

    const char *fileName = "titulo.txt";
    char *message = LoadFileText(fileName);

    int messageLength = (int)strlen(message);
    int framesCounter = 0;

    SetTargetFPS(60);               

   
    while (!WindowShouldClose())    
    {
        
        if (IsKeyDown(KEY_SPACE)) framesCounter += 8;
        else framesCounter++;

        if (IsKeyPressed(KEY_ENTER)) framesCounter = 0;
        

        
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText(TextSubtext(message, 0, framesCounter/10), 210, 160, 20, MAROON);

            DrawText("PRESS [ENTER] to RESTART!", 240, 260, 20, LIGHTGRAY);
            DrawText("HOLD [SPACE] to SPEED UP!", 239, 300, 20, LIGHTGRAY);

        EndDrawing();

        if (framesCounter/10 > messageLength) break;
        
    }

    free(message);
    CloseWindow();        

    return 0;
}

