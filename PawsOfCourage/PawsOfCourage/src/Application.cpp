#include "raylib.h"
#include "Constants.h"
#include "Game.h"

int main()
{
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(Constants::DEFAULT_WIDTH, Constants::DEFAULT_HEIGHT, "Rocky");
    InitAudioDevice();

    SetTargetFPS(60);              

    Game game;
    game.run();

    CloseWindow();      
}