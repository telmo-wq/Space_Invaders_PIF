#include <raylib.h>
#include <stdlib.h>
#include <string.h>

void titulo(void)
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



        EndDrawing();

        if (framesCounter/10 > messageLength+2) break;
        
    }

    free(message);
    CloseWindow(); 
    return;
}

