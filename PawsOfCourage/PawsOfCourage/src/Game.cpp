#include "Game.h"
#include "Constants.h"

float Game::scale = 0.0f;

Game::Game() : player({200, 200}, 32, 170.f)
{
    renderTexture = LoadRenderTexture(Constants::VIRTUAL_WIDTH, Constants::VIRTUAL_HEIGHT);
    SetTextureFilter(renderTexture.texture, TEXTURE_FILTER_BILINEAR);

    camera.target = { 400, 400 };
    camera.offset = {Constants::VIRTUAL_WIDTH / 2.0f, Constants::VIRTUAL_HEIGHT / 2.0f}; 
    camera.rotation = 0.0f;
    camera.zoom = 3.0f;

    resourceManager.loadTexture("map", "Assets/map.png");
    resourceManager.loadTexture("walkRight", "Assets/WalkRight.png");
    resourceManager.loadTexture("walkLeft", "Assets/WalkLeft.png");
    resourceManager.loadTexture("digLeft", "Assets/DigLeft.png");
    resourceManager.loadTexture("digRight", "Assets/DigRight.png");
    resourceManager.loadTexture("idleLeft", "Assets/IdleLeft.png");
    resourceManager.loadTexture("idleRight", "Assets/IdleRight.png");
    resourceManager.loadTexture("walkDown", "Assets/WalkDown.png");
    resourceManager.loadTexture("walkUp", "Assets/WalkUp.png");

    Animation animationLeft(resourceManager.getTexture("walkLeft"), 
                            8, 32, 0.09f, REPEATING);
    Animation animationRight(resourceManager.getTexture("walkRight"),
                             8, 32, 0.09f, REPEATING);
    Animation animationUp(resourceManager.getTexture("walkUp"), 
                          5, 32, 0.09f, REPEATING);
    Animation animationDown(resourceManager.getTexture("walkDown"), 
                            5, 32, 0.09f, REPEATING);
    Animation animationIdleLeft(resourceManager.getTexture("idleLeft"), 
                                5, 32, 0.07f, REPEATING);
    Animation animationIdleRight(resourceManager.getTexture("idleRight"), 
                                 5, 32, 0.07f, REPEATING);
    Animation animationDigLeft(resourceManager.getTexture("digLeft"),
        8, 32, 0.07f, REPEATING);
    Animation animationDigRight(resourceManager.getTexture("digRight"),
        8, 32, 0.07f, REPEATING);

    player.addAnimation(animationLeft, PlayerState::LEFT);
    player.addAnimation(animationRight, PlayerState::RIGHT);
    player.addAnimation(animationUp, PlayerState::UP);
    player.addAnimation(animationDown, PlayerState::DOWN);
    player.addAnimation(animationIdleLeft, PlayerState::IDLE_LEFT);
    player.addAnimation(animationIdleRight, PlayerState::IDLE_RIGHT);
    player.addAnimation(animationDigLeft, PlayerState::DIG_LEFT);
    player.addAnimation(animationDigRight, PlayerState::DIG_RIGHT);

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

    player.update();
    camera.target = { (float)player.getPosition().x, (float)player.getPosition().y };
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
    //drawDebugGrid();
    player.draw();
    drawHighlight();

    EndTextureMode();

    // --- Draw the render texture to the window, scaled ---
    BeginDrawing();
    ClearBackground(GREEN);

    Rectangle src = { 0.0f, 0.0f, (float)renderTexture.texture.width, -(float)renderTexture.texture.height };
    Rectangle dest = { (float)offsetX, (float)offsetY, (float)scaledWidth, (float)scaledHeight };

    DrawTexturePro(renderTexture.texture, src, dest, { 0, 0 }, 0.0f, WHITE);

    EndDrawing();
}

void Game::drawDebugGrid() const
{
    for (int i = 0; i < 45; i++)
    {
        for (int j = 0; j < 80; j++)
        {
            DrawRectangleLines(i * 32, j * 32, 32, 32, BLACK);
        }
    }
}

void Game::drawHighlight() const
{
    Position pawsPosition = player.getPawsPosition();

    pawsPosition.x = (pawsPosition.x / 32) * 32;
    pawsPosition.y = (pawsPosition.y / 32) * 32;

    DrawRectangle(pawsPosition.x, pawsPosition.y, 32, 32, {0,255, 0, 40});
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
