#pragma once
#include "raylib.h"
#include <iostream>
#include "ResourceManager.h"
#include "Player.h"
#include <string>

enum class BlockType
{
	BIG_ROCK,
	WOOD
};

struct SolidBLock
{
	Position pos;
	BlockType type;
};

class Game
{
	RenderTexture2D renderTexture;
	ResourceManager resourceManager;
	Camera2D camera;
	Player player;
	Position targetPosition;
	std::vector<SolidBLock> solidBlocks;

	void input();
	void update();
	void draw();

	void drawDebugGrid() const;

	void drawHighlight() const;
	Rectangle getHighlightRec() const;
	bool isHighlighSolid() const;

	void generateSolidBlocks();
	std::string getStringFromEnum(BlockType type) const;
	void drawSolidBlocks() const;

	void drawCircles() const;
	int getCircleRadius(Position pos) const;

	bool checkPlayerCollision() const;
	Position genPlayerStartPosition() const;
	Position genTargetPosition() const;

	int getRandomNumberInInterval(int left, int right) const;
	bool isSolidBlock(int x, int y) const;

	void drawMinimap();
public:

	static float scale;

	Game();
	void run();
};

