#pragma once
#include "raylib.h"
#include <iostream>
#include "ResourceManager.h"
#include "Player.h"
#include <string>
#include <deque>
#include "Constants.h"

enum class SolidBlockType
{
	BIG_ROCK,
	WOOD,
	ROCK_GREEN1,
	ROCK_GREEN2
};

enum class ScreenState
{
	MAIN_MENU,
	COMICS,
	GAME,
	WIN,
	LOSE,
	BEFORE_MENU
};

enum class ComicsState
{
	SCENE1,
	SCENE2,
	SCENE3,
	SCENE4
};

struct SolidBLock
{
	Position pos;
	SolidBlockType type;
};

class Game
{	
	int sniffsLeft = Constants::SNIFFS_LEFT;
	ComicsState comicsState = ComicsState::SCENE1;
	bool loading = true;
	bool winMusic = true;
	ScreenState screenState = ScreenState::BEFORE_MENU;
	float countdown = Constants::COUNTDOWN;
	bool win = false;
	double lastEventTime = 0.0;
	RenderTexture2D renderTexture;
	ResourceManager resourceManager;
	Camera2D camera;
	Player player;
	Position targetPosition;
	std::vector<SolidBLock> solidBlocks;
	std::deque<Position> circles;

	void resetGameStats();

	void input();
	void update();
	void draw();

	void updateCircles();
	void drawDebugGrid() const;
	void drawHoles();
	void drawHighlight() const;
	Rectangle getHighlightRec() const;
	bool isHighlighSolid() const;

	void generateSolidBlocks();
	std::string stringFromEnum(SolidBlockType type) const;
	void drawSolidBlocks() const;

	void drawCircles();
	int getCircleRadius(Position pos) const;

	bool checkPlayerCollision() const;
	Position genPlayerStartPosition() const;
	Position genTargetPosition() const;

	int getRandomNumberInInterval(int left, int right) const;
	bool isSolidBlock(int x, int y) const;

	void drawMinimap();

	void checkWin();
public:

	static float scale;

	Game();
	void run();
};

