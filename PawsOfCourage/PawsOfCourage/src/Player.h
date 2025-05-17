#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>
#include "Animation.h"

enum class PlayerState
{
	IDLE,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

class Player
{
	Vector2 position;
	int hitBoxSize;
	float movementSpeed;
	PlayerState currentState;
	std::unordered_map<std::string, Animation> anims;

	std::string enumToString(PlayerState playerState) const;


public:

	Player(const Vector2& position, int hitBoxSize, float movementSpeed);
	void update();
	void addAnimation(const Animation& animation, PlayerState playerState);
	Rectangle getHitBox() const;
	void draw() const;
	Vector2 getPosition() const;
};

