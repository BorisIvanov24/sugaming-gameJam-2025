#include "Game.h"
#include "Constants.h"

float Game::scale = 0.0f;

Game::Game() : anim(4, 16, 0.05f, AnimationType::REPEATING)
{
    renderTexture = LoadRenderTexture(Constants::VIRTUAL_WIDTH, Constants::VIRTUAL_HEIGHT);
    SetTextureFilter(renderTexture.texture, TEXTURE_FILTER_BILINEAR);

    camera.target = { 400, 400 };
    camera.offset = {Constants::VIRTUAL_WIDTH / 2.0f, Constants::VIRTUAL_HEIGHT / 2.0f}; 
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;

    resourceManager.loadTexture("map", "Sprites/map.png");
    resourceManager.loadTexture("chicken", "Sprites/chicken.png");

    anim.setTexture(resourceManager.getTexture("chicken"));

}

void Game::input()
{

}

void Game::update()
{
    //update scale
    scale = std::min(
        (float)GetScreenWidth() / Constants::VIRTUAL_WIDTH,
        (float)GetScreenHeight() / Constants::VIRTUAL_HEIGHT
    );

    if (IsKeyDown(KEY_D))
    {
        camera.target.x += Constants::KEY_BUTTON_MOVEMENT;
    }
    else if (IsKeyDown(KEY_A))
    {
        camera.target.x -= Constants::KEY_BUTTON_MOVEMENT;
    }
    else if (IsKeyDown(KEY_W))
    {
        camera.target.y -= Constants::KEY_BUTTON_MOVEMENT;
    }
    else if (IsKeyDown(KEY_S))
    {
        camera.target.y += Constants::KEY_BUTTON_MOVEMENT;
    }

    anim.animationUpdate();
}

void Game::draw()
{
    int scaledWidth = Constants::VIRTUAL_WIDTH * scale;
    int scaledHeight = Constants::VIRTUAL_HEIGHT * scale;
    int offsetX = (GetScreenWidth() - scaledWidth) / 2;
    int offsetY = (GetScreenHeight() - scaledHeight) / 2;

    // --- Draw to the offscreen buffer at virtual resolution ---
    BeginTextureMode(renderTexture);
    ClearBackground(RAYWHITE);

    BeginMode2D(camera);

    DrawTexture(resourceManager.getTexture("map"), 0, 0, RAYWHITE);
    
    DrawTextureRec(anim.getTexture(), anim.animationFrame(), camera.target, RAYWHITE);

    EndTextureMode();

    // --- Draw the render texture to the window, scaled ---
    BeginDrawing();
    ClearBackground(RED);

    Rectangle src = { 0.0f, 0.0f, (float)renderTexture.texture.width, -(float)renderTexture.texture.height };
    Rectangle dest = { (float)offsetX, (float)offsetY, (float)scaledWidth, (float)scaledHeight };

    DrawTexturePro(renderTexture.texture, src, dest, { 0, 0 }, 0.0f, WHITE);

    EndDrawing();
}

void Game::run()
{

    while (!WindowShouldClose())
    {
        input();
        update();
        draw();
    }

}
