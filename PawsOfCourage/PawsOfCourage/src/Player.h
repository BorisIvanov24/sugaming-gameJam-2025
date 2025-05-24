#pragma once
#include "raylib.h"
#include <string>
#include <unordered_map>
#include "Animation.h"
#include <vector>
#include "Constants.h"

enum class PlayerState
{
	IDLE_LEFT,
	IDLE_RIGHT,
	LEFT,
	RIGHT,
	UP,
	DOWN,
	DIG_LEFT,
	DIG_RIGHT,
	SNIFF_LEFT,
	SNIFF_RIGHT
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

	float diggingTimer = Constants::DIGGING_TIME;
	float sniffingTimer = Constants::SNIFFING_TIME;

	std::string enumToString(PlayerState playerState) const;
	
	void updateDiggingTimer();
	void checkDiggingEnded();

	void updateSniffingTimer();
	void checkSniffingEnded();

public:

	bool isDigging = false;
	double circleDelay = 2.0;
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
	bool arrowFlag = false;

	const std::vector<Position>& getDigPositions() const;
	void popDigPosition();
	void clearDigPositions();
};

