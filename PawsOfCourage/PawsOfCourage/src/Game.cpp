#include "Game.h"
#include "Constants.h"
#include <fstream>
#include <random>
#include <vector>

float Game::scale = 0.0f;  


Game::Game() : player({200, 200}, 20, 170.f)
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
    resourceManager.loadTexture("sniffLeft", "Assets/SniffLeft.png");
    resourceManager.loadTexture("sniffRight", "Assets/SniffRight.png");

    resourceManager.loadTexture("bigRock", "Assets/BigRock.png");
    resourceManager.loadTexture("wood", "Assets/Wood.png");
    resourceManager.loadTexture("hole", "Assets/Hole.png");
    resourceManager.loadTexture("rockGreen1", "Assets/rockGreen1.png");
    resourceManager.loadTexture("rockGreen2", "Assets/rockGreen2.png");
    resourceManager.loadTexture("miniMapCursor", "Assets/RockyHead.png");

    //Fonts
    resourceManager.setFont(LoadFont("Assets/Font.ttf"));

    Animation animationSniffLeft(resourceManager.getTexture("sniffLeft"),
        6, 32, 0.09f, REPEATING);
    Animation animationSniffRight(resourceManager.getTexture("sniffRight"),
        6, 32, 0.09f, REPEATING);
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
    player.addAnimation(animationSniffLeft, PlayerState::SNIFF_LEFT);
    player.addAnimation(animationSniffRight, PlayerState::SNIFF_RIGHT);

    generateSolidBlocks();
    player.setPosition(genPlayerStartPosition());
    targetPosition = genTargetPosition();
}

void Game::input()
{

}

void Game::update()
{
    checkWin();

    //update scale
    scale = std::min(
        (float)GetScreenWidth() / Constants::VIRTUAL_WIDTH,
        (float)GetScreenHeight() / Constants::VIRTUAL_HEIGHT
    );

    if (countdown > 0.0f)
    {
        countdown -= GetFrameTime();  // Decrease based on frame time
        if (countdown < 0.0f) countdown = 0.0f;  // Clamp to 0
    }

    Position oldPos = player.getPosition();
    player.update();

    if (player.isDigging)
    {
        circles.push_front(player.getPosition());
        lastEventTime = GetTime();
        player.isDigging = false;
    }
    
    updateCircles();

    if (checkPlayerCollision())
    {
        player.setPosition(oldPos);
    }

    camera.target = { (float)player.getPosition().x, (float)player.getPosition().y };
}

void Game::drawMinimap()
{
    Vector2 minimapPositionVirtual = { 1920 - Constants::MINIMAP_WIDTH - 10, 10 };

    const Texture2D& worldTexture = resourceManager.getTexture("map");

    // --- Set up scissor (clip) region ---
    BeginScissorMode((int)minimapPositionVirtual.x, (int)minimapPositionVirtual.y, Constants::MINIMAP_WIDTH, Constants::MINIMAP_HEIGHT);

    // --- Draw minimap texture ---
    DrawTexturePro(
        worldTexture,
        { 0, 0, (float)worldTexture.width, (float)-worldTexture.height }, // Flip Y
        { minimapPositionVirtual.x, minimapPositionVirtual.y, (float)Constants::MINIMAP_WIDTH, (float)Constants::MINIMAP_HEIGHT },
        { 0, 0 },
        0.0f,
        WHITE
    );

    float scaleX = (float)Constants::MINIMAP_WIDTH / worldTexture.width;
    float scaleY = (float)Constants::MINIMAP_HEIGHT / worldTexture.height;

    float playerMinimapX = minimapPositionVirtual.x + player.getPosition().x * scaleX;
    float playerMinimapY = minimapPositionVirtual.y + player.getPosition().y * scaleY;

    float targetMinimapX = minimapPositionVirtual.x + targetPosition.x * scaleX;
    float targetMinimapY = minimapPositionVirtual.y + targetPosition.y * scaleY;

   // if (player.circleDelay < 0.01)
    //{
        for (const auto& pos : circles)
        {
            float circleMinimapX = minimapPositionVirtual.x + pos.x * scaleX;
            float circleMinimapY = minimapPositionVirtual.y + pos.y * scaleY;

            int radius = sqrtf((circleMinimapX - targetMinimapX) * (circleMinimapX - targetMinimapX) +
                (circleMinimapY - targetMinimapY) * (circleMinimapY - targetMinimapY));

            DrawCircle(circleMinimapX, circleMinimapY, radius, { 255, 0, 0, 70 });
        }
    //}

    const Texture2D& texture = resourceManager.getTexture("miniMapCursor");
    DrawTexturePro(
        texture,
        { 0, 0, (float)texture.width, (float)texture.height }, // Flip Y
        { playerMinimapX -10, playerMinimapY -10, (float)texture.width * 3, (float)texture.width * 3 },
        { 0, 0 },
        0.0f,
        WHITE
    );    
    //DrawRectangle((int)playerMinimapX, (int)playerMinimapY, 4, 4, RED);

    // --- Disable scissor ---
    EndScissorMode();

    // Optional: draw a border around the minimap
    DrawRectangleLines((int)minimapPositionVirtual.x, (int)minimapPositionVirtual.y, Constants::MINIMAP_WIDTH, Constants::MINIMAP_HEIGHT, BLACK);
}

void Game::checkWin()
{
    const std::vector<Position>& pos = player.getDigPositions();

    if (pos.empty())
        return;

    if (targetPosition.x == pos.back().x && targetPosition.y == pos.back().y)
    {
        win = true;
    }
}


void Game::draw()
{
    int scaledWidth = Constants::VIRTUAL_WIDTH * scale;
    int scaledHeight = Constants::VIRTUAL_HEIGHT * scale;
    int offsetX = (GetScreenWidth() - scaledWidth) / 2;
    int offsetY = (GetScreenHeight() - scaledHeight) / 2;

    // --- Draw to the offscreen buffer at virtual resolution ---
    BeginTextureMode(renderTexture);
    ClearBackground(SKYBLUE);

    BeginMode2D(camera);

    DrawTexture(resourceManager.getTexture("map"), 0, 0, RAYWHITE);
    drawSolidBlocks();

    if(IsKeyDown(KEY_T))
        DrawRectangle(targetPosition.x, targetPosition.y, 32, 32, YELLOW);

    drawHoles();
    drawCircles();
    //drawDebugGrid();
    player.draw();
    drawHighlight();

    EndMode2D();
    DrawTextEx(resourceManager.getFont(), TextFormat("Timer: %.2f", countdown),
        {20, 20}, 60, 4.f, BLACK);

    //DrawText(TextFormat("Timer: %.2f", countdown), 20, 20, 60, RED);
    if (win)
    {
        DrawText("YOU WIIIIIIIIIIINNN!", 200, 200, 60, YELLOW);
    }

    if (countdown <= 0.0f)
    {
        DrawText("YOU LOSTTTTTTT!", 200, 200, 60, RED);
    }

    drawMinimap();
    EndTextureMode();

    // --- Draw the render texture to the window, scaled ---
    BeginDrawing();
    ClearBackground(SKYBLUE);

    Rectangle src = { 0.0f, 0.0f, (float)renderTexture.texture.width, -(float)renderTexture.texture.height };
    Rectangle dest = { (float)offsetX, (float)offsetY, (float)scaledWidth, (float)scaledHeight };

    DrawTexturePro(renderTexture.texture, src, dest, { 0, 0 }, 0.0f, WHITE);

    EndDrawing();
}

void Game::updateCircles()
{
    if (circles.empty())
        return;

    if (GetTime() - lastEventTime >= Constants::CIRCLE_REMOVAL_INTERVAL)
    {
        lastEventTime = GetTime();
        circles.pop_back();
    }
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

void Game::drawHoles()
{
    const std::vector<Position>& vec = player.getDigPositions();
    if (vec.empty())
        return;

    const Texture2D& texture = resourceManager.getTexture("hole");

    Position pos = vec.back();

    if (isSolidBlock(pos.x / 32, pos.y / 32))
    {
        player.popDigPosition();
    }
    
    for (int i = 0; i < vec.size(); i++)
    {
        int holeX = (vec[i].x / 32) * 32;
        int holeY = (vec[i].y / 32) * 32;
        DrawTexture(texture, holeX, holeY, RAYWHITE);
    }
}

void Game::drawHighlight() const
{
    Color color = { 0,255, 0, 40 };

    if (isHighlighSolid())
        color = { 255,0, 0, 70 };

    DrawRectangleRec(getHighlightRec(), color);
}

Rectangle Game::getHighlightRec() const
{
    Position pawsPosition = player.getPawsPosition();

    pawsPosition.x = (pawsPosition.x / 32) * 32;
    pawsPosition.y = (pawsPosition.y / 32) * 32;

    return { (float)pawsPosition.x, (float)pawsPosition.y, 32, 32 };
}

bool Game::isHighlighSolid() const
{
    Rectangle rec = getHighlightRec();

    return isSolidBlock((int)rec.x / 32, (int)rec.y / 32);
}

void Game::generateSolidBlocks()
{
    for (int i = 0; i < Constants::MAP_WIDTH_TILES; i++)
    {
        for (int j = 0; j < Constants::MAP_HEIGHT_TILES; j++)
        {
            int randomNumber = getRandomNumberInInterval(1, 100);

            if (randomNumber <= 5 || i == 0 || j == 0 || 
                i == Constants::MAP_WIDTH_TILES - 1 || 
                j == Constants::MAP_HEIGHT_TILES - 1)
            {
                randomNumber = getRandomNumberInInterval(0, 3);
                solidBlocks.push_back({ {i, j}, (SolidBlockType)randomNumber });
            }
        }
    }
}

std::string Game::stringFromEnum(SolidBlockType type) const
{
    switch (type)
    {
    case SolidBlockType::BIG_ROCK:
        return "bigRock";
    case SolidBlockType::WOOD:
        return "wood";
    case SolidBlockType::ROCK_GREEN1:
        return "rockGreen1";
    case SolidBlockType::ROCK_GREEN2:
        return "rockGreen2";
    }

    return "";
}

void Game::drawSolidBlocks() const
{
    for (int i = 0; i < solidBlocks.size(); i++)
    {
        SolidBlockType blockType = solidBlocks[i].type;

        DrawTexture(resourceManager.getTexture(stringFromEnum(blockType)),
            solidBlocks[i].pos.x * 32, solidBlocks[i].pos.y * 32, RAYWHITE);
    }
}

void Game::drawCircles()
{
    float var = 20.0 * GetFrameTime();
    player.circleDelay -= var;
    
   // if(player.circleDelay > 0.1)
    //lastEventTime += var;

   // if ((player.circleDelay - 0.0) < 0.001)
    //{
        for (Position pos : circles)
        {
            int radius = getCircleRadius(pos);
            DrawCircle(pos.x, pos.y, radius, { 0, 0, 255, 10 });
        }
    //}
}

int Game::getCircleRadius(Position pos) const
{
    int dist = sqrt((pos.x - targetPosition.x) * (pos.x - targetPosition.x) +
               (pos.y - targetPosition.y) * (pos.y - targetPosition.y));

    return dist + 64;
}

bool Game::checkPlayerCollision() const
{
    Position pos = player.getPosition();

    for (int i = 0; i < solidBlocks.size(); i++)
    {
        Rectangle rec1 = player.getHitBox();
        Rectangle rec = { (float)solidBlocks[i].pos.x * 32, (float)solidBlocks[i].pos.y * 32, 32, 32 };
        
        if (CheckCollisionRecs(rec1, rec))
            return true;
    }
    return false;
}

Position Game::genPlayerStartPosition() const
{
    Position pos;

    do
    {
        pos.x = getRandomNumberInInterval(1, 79);
        pos.y = getRandomNumberInInterval(1, 44);

    } while (isSolidBlock(pos.x, pos.y));

    return { pos.x * 32, pos.y * 32 };
}

Position Game::genTargetPosition() const
{
    Position pos;

    do
    {
        pos.x = getRandomNumberInInterval(1, 79);
        pos.y = getRandomNumberInInterval(1, 44);

    } while (isSolidBlock(pos.x, pos.y));

    return { pos.x * 32, pos.y * 32 };
}

int Game::getRandomNumberInInterval(int left, int right) const
{
    std::random_device rd;  // Seed
    std::mt19937 gen(rd()); // Mersenne Twister engine
    std::uniform_int_distribution<int> dist(left, right);

    return dist(gen);
}

bool Game::isSolidBlock(int x, int y) const
{
    for (int i = 0; i < solidBlocks.size(); i++)
    {
        Position pos = solidBlocks[i].pos;

        if (pos.x == x && pos.y == y)
            return true;
    }

    return false;
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
