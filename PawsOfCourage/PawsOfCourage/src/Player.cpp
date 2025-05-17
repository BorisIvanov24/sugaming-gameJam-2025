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
	default:
		break;
	}
}

Position Player::getPawsPosition() const
{
	Position pawsPosition;

	pawsPosition.x = position.x + (hitBoxSize / 2);
	pawsPosition.y = position.y + hitBoxSize;

	return pawsPosition;
}

Player::Player(const Position& position, int hitBoxSize, float movementSpeed)
{
	this->position = position;
	this->hitBoxSize = hitBoxSize;
	this->movementSpeed = movementSpeed;
	currentState = PlayerState::IDLE_LEFT;
}

void Player::update()
{
	float dt = GetFrameTime();
	int toAdd = dt * movementSpeed;
	
	std::string currentAnimStr = enumToString(currentState);
	anims[currentAnimStr].animationUpdate();

	if (IsKeyDown(KEY_D))
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
	else if (IsKeyPressed(KEY_SPACE))
	{
		if(currentState == PlayerState::IDLE_LEFT)
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

}

void Player::addAnimation(const Animation& animation, PlayerState playerState)
{
	std::string animString = enumToString(playerState);

	anims[animString] = animation;
}

Rectangle Player::getHitBox() const
{
	return {(float)position.x, (float)position.y, (float)hitBoxSize, (float)hitBoxSize};
}

void Player::draw() const
{
	std::string currentAnimStr = enumToString(currentState);

	DrawTextureRec(anims.at(currentAnimStr).getTexture(), anims.at(currentAnimStr).animationFrame(),
		{ (float)position.x, (float)position.y }, RAYWHITE);

	//DrawRectangleLines(position.x, position.y, (float)hitBoxSize, (float)hitBoxSize, BLACK);
}

Position Player::getPosition() const
{
	return position;
}
