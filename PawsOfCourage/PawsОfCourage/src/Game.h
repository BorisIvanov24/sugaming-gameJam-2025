#pragma once
#include "raylib.h"
#include <iostream>
#include "ResourceManager.h"
#include "Animation.h"

class Game
{
	RenderTexture2D renderTexture;
	ResourceManager resourceManager;
	Camera2D camera;
	Animation anim;

	void input();
	void update();
	void draw();

public:

	static float scale;

	Game();
	void run();
};

