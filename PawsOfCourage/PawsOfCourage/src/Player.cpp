#include "Player.h"
#include <iostream>
#include "Constants.h"

std::string Player::enumToString(PlayerState playerState) const
{
	switch (playerState)
	{
	case PlayerState::IDLE_LEFT:
		return "idle_left";
	case PlayerState::IDLE_RIGHT:
		return "idle_right";
	case PlayerState::LEFT:
		return "left";
	case PlayerState::RIGHT:
		return "right";
	case PlayerState::UP:
		return "up";
	case PlayerState::DOWN:
		return "down";
	case PlayerState::DIG_LEFT:
		return "dig_left";
	case PlayerState::DIG_RIGHT:
		return "dig_right";
	case PlayerState::SNIFF_LEFT:
		return "sniff_left";
	case PlayerState::SNIFF_RIGHT:
		return "sniff_right";
	default:
		break;
	}
}

void Player::updateDiggingTimer()
{
	if (diggingTimer < 1.f)
		return;

	diggingTimer -= GetFrameTime();
}

void Player::checkDiggingEnded()
{
	if (diggingTimer < 1.f)
	{
		if (currentState == PlayerState::DIG_LEFT)
		{
			currentState = PlayerState::IDLE_LEFT;
			digPositions.push_back(getHighlightPos());
		}
		

		if (currentState == PlayerState::DIG_RIGHT)
		{
			digPositions.push_back(getHighlightPos());
			currentState = PlayerState::IDLE_RIGHT;
		}
	}
}

void Player::updateSniffingTimer()
{
	if (sniffingTimer < 1.f)
		return;

	sniffingTimer -= GetFrameTime();
}

void Player::checkSniffingEnded()
{
	if (sniffingTimer < 1.f)
	{
		if (currentState == PlayerState::SNIFF_LEFT)
		{
			isDigging = true;
			circleDelay = 30.0;
			arrowFlag = true;
			currentState = PlayerState::IDLE_LEFT;
		}


		if (currentState == PlayerState::SNIFF_RIGHT)
		{
			isDigging = true;
			circleDelay = 30.0;
			arrowFlag = true;
			currentState = PlayerState::IDLE_RIGHT;
		}
	}
}

Position Player::getPawsPosition() const
{
	Position pawsPosition;

	pawsPosition.x = position.x + (32 / 2);
	pawsPosition.y = position.y + 32;

	return pawsPosition;
}

void Player::setPosition(Position newPos)
{
	position = newPos;
}

int Player::getHitBoxSize() const
{
	return hitBoxSize;
}

Position Player::getHighlightPos() const
{
	Position pawsPosition = getPawsPosition();

	pawsPosition.x = (pawsPosition.x / 32) * 32;
	pawsPosition.y = (pawsPosition.y / 32) * 32;

	return { pawsPosition.x, pawsPosition.y };
}

const std::vector<Position>& Player::getDigPositions() const
{
	return digPositions;
}

void Player::popDigPosition()
{
	if (digPositions.empty())
		return;

	digPositions.pop_back();
}

void Player::clearDigPositions()
{
	digPositions.clear();
}

Player::Player(const Position& position, int hitBoxSize, float movementSpeed)
{
	this->position = position;
	this->hitBoxSize = hitBoxSize;
	this->movementSpeed = movementSpeed;
	currentState = PlayerState::IDLE_LEFT;
	isDigging = false;
}

PlayerState Player::update()
{
	float dt = GetFrameTime();
	int toAdd = dt * movementSpeed;
	
	std::string currentAnimStr = enumToString(currentState);
	anims[currentAnimStr].animationUpdate();

	updateDiggingTimer();
	checkDiggingEnded();

	updateSniffingTimer();
	checkSniffingEnded();

	if (IsKeyDown(KEY_A) && IsKeyDown(KEY_W))
	{
		position.x -= toAdd;
		position.y -= toAdd;
		currentState = PlayerState::LEFT;
	}
	else if (IsKeyDown(KEY_S) && IsKeyDown(KEY_A))
	{
		position.x -= toAdd;
		position.y += toAdd;
		currentState = PlayerState::LEFT;
	}
	else if (IsKeyDown(KEY_D) && IsKeyDown(KEY_S))
	{
		position.x += toAdd;
		position.y += toAdd;
		currentState = PlayerState::RIGHT;
	}
	else if (IsKeyDown(KEY_D) && IsKeyDown(KEY_W))
	{
		position.x += toAdd;
		position.y -= toAdd;
		currentState = PlayerState::RIGHT;
	}
	else if (IsKeyDown(KEY_D))
	{
		position.x += toAdd;
		currentState = PlayerState::RIGHT;
	}
	else if (IsKeyDown(KEY_A))
	{
		position.x -= toAdd;
		currentState = PlayerState::LEFT;
	}
	else if (IsKeyDown(KEY_W))
	{
		position.y -= toAdd;
		currentState = PlayerState::UP;

	}
	else if (IsKeyDown(KEY_S))
	{
		position.y += toAdd;
		currentState = PlayerState::DOWN;
	}
	else if (IsKeyPressed(KEY_ENTER))
	{
		diggingTimer = Constants::DIGGING_TIME;

		if (currentState == PlayerState::IDLE_LEFT)
			currentState = PlayerState::DIG_LEFT;
		else
			currentState = PlayerState::DIG_RIGHT;

	}
	else if(currentState == PlayerState::LEFT || currentState == PlayerState::DOWN)
	{
		currentState = PlayerState::IDLE_LEFT;
	}
	else if (currentState == PlayerState::RIGHT || currentState == PlayerState::UP)
	{
		currentState = PlayerState::IDLE_RIGHT;
	}
	else if (IsKeyPressed(KEY_SPACE))
	{
		sniffingTimer = Constants::SNIFFING_TIME;

		if (currentState == PlayerState::IDLE_LEFT)
			currentState = PlayerState::SNIFF_LEFT;
		else
			currentState = PlayerState::SNIFF_RIGHT;
	}

	return currentState;
}

void Player::addAnimation(const Animation& animation, PlayerState playerState)
{
	std::string animString = enumToString(playerState);

	anims[animString] = animation;
}

Rectangle Player::getHitBox() const
{
	int offsetX = 7;
	int offsetY = 7;
	return {(float)position.x + offsetX, (float)position.y + offsetY, (float)hitBoxSize, (float)hitBoxSize};
}

void Player::draw() const
{
	std::string currentAnimStr = enumToString(currentState);

	DrawTextureRec(anims.at(currentAnimStr).getTexture(), anims.at(currentAnimStr).animationFrame(),
		{ (float)position.x, (float)position.y }, RAYWHITE);

	//DrawRectangleLines(getHitBox().x, getHitBox().y, getHitBox().width, getHitBox().height, BLACK);
}

Position Player::getPosition() const
{
	return position;
}
