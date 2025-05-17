#include "Game.h"
#include "Constants.h"

float Game::scale = 0.0f;

Game::Game() : player({200, 200}, 32, 100.f)
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

    Animation animationLeft(8, 32, 0.09f, REPEATING);
    animationLeft.setTexture(resourceManager.getTexture("walkLeft"));
    Animation animationRight(8, 32, 0.09f, REPEATING);
    animationRight.setTexture(resourceManager.getTexture("walkRight"));
    Animation animationUp(8, 32, 0.09f, REPEATING);
    animationUp.setTexture(resourceManager.getTexture("walkLeft"));
    Animation animationDown(8, 32, 0.09f, REPEATING);
    animationDown.setTexture(resourceManager.getTexture("walkLeft"));

    Animation animationIdle(8, 32, 0.09f, REPEATING);
    animationIdle.setTexture(resourceManager.getTexture("walkRight"));

    player.addAnimation(animationLeft, PlayerState::LEFT);
    player.addAnimation(animationRight, PlayerState::RIGHT);
    player.addAnimation(animationUp, PlayerState::UP);
    player.addAnimation(animationDown, PlayerState::DOWN);
    player.addAnimation(animationIdle, PlayerState::IDLE);
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
    camera.target = { player.getPosition().x, player.getPosition().y };
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
    player.draw();

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
