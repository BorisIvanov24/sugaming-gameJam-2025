#pragma once
#include "raylib.h"
#include <iostream>
#include "ResourceManager.h"
#include "Player.h"
#include <string>
#include <deque>

enum class SolidBlockType
{
	BIG_ROCK,
	WOOD
};

struct SolidBLock
{
	Position pos;
	SolidBlockType type;
};

class Game
{
	double lastEventTime = 0.0;
	RenderTexture2D renderTexture;
	ResourceManager resourceManager;
	Camera2D camera;
	Player player;
	Position targetPosition;
	std::vector<SolidBLock> solidBlocks;
	std::deque<Position> circles;

	void input();
	void update();
	void draw();

	void updateCircles();
	void drawDebugGrid() const;
	void drawHoles() const;
	void drawHighlight() const;
	Rectangle getHighlightRec() const;
	bool isHighlighSolid() const;

	void generateSolidBlocks();
	std::string stringFromEnum(SolidBlockType type) const;
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

