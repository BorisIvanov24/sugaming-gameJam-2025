#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>
#include "Animation.h"
#include <vector>

enum class PlayerState
{
	IDLE_LEFT,
	IDLE_RIGHT,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	DIG_LEFT,
	DIG_RIGHT
};

struct Position
{
	int x;
	int y;
};

class Player
{
	Position position;
	int hitBoxSize;
	float movementSpeed;
	PlayerState currentState;
	std::unordered_map<std::string, Animation> anims;
	std::vector<Position> digPositions;


	std::string enumToString(PlayerState playerState) const;

public:

	bool isDigging = false;
	Player(const Position& position, int hitBoxSize, float movementSpeed);
	PlayerState update();
	void addAnimation(const Animation& animation, PlayerState playerState);
	Rectangle getHitBox() const;
	void draw() const;
	Position getPosition() const;
	Position getPawsPosition() const;
	void setPosition(Position newPos);
	int getHitBoxSize() const;
	Position getHighlightPos() const;

	const std::vector<Position>& getDigPositions() const;
};

