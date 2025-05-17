#pragma once
#include "raylib.h"
#include <iostream>
#include "ResourceManager.h"
#include "Player.h"

class Game
{
	RenderTexture2D renderTexture;
	ResourceManager resourceManager;
	Camera2D camera;
	Player player;

	void input();
	void update();
	void draw();

public:

	static float scale;

	Game();
	void run();
};

