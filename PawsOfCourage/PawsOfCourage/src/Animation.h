#pragma once
#include "raylib.h"

enum AnimationType
{
	REPEATING = 1,
	ONESHOT = 2,
};

class Animation {
	int first;
	int last;
	int cur;

	int step;
	int numFramesPerRow;

	int tilePixelSize;

	float speed;
	// how much time is left on the current frame
	float durationLeft;

	AnimationType type;

	const Texture2D* texture = nullptr;

public:

	Animation() = default;
	Animation(const Texture2D& texture, int numFramesPerRow, 
		      int tilePixelSize, float speed, AnimationType type);

	Rectangle animationFrame() const;
	void animationUpdate();

	const Texture2D& getTexture() const;
};

