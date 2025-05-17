#include "Player.h"
#include <iostream>

std::string Player::enumToString(PlayerState playerState) const
{
	switch (playerState)
	{
	case PlayerState::IDLE:
		return "idle";
	case PlayerState::LEFT:
		return "left";
	case PlayerState::RIGHT:
		return "right";
	case PlayerState::UP:
		return "up";
	case PlayerState::DOWN:
		return "down";
	default:
		break;
	}
}

Player::Player(const Vector2& position, int hitBoxSize, float movementSpeed)
{
	this->position = position;
	this->hitBoxSize = hitBoxSize;
	this->movementSpeed = movementSpeed;
	currentState = PlayerState::IDLE;
}

void Player::update()
{
	float dt = GetFrameTime();
	float toAdd = dt * movementSpeed;
	
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
	else
	{
		currentState = PlayerState::IDLE;
	}
}

void Player::addAnimation(const Animation& animation, PlayerState playerState)
{
	std::string animString = enumToString(playerState);
	
	if (anims.count(animString))
	{
		std::cout<<"IMA GO"<<std::endl;
	}

	anims[animString] = animation;
}

Rectangle Player::getHitBox() const
{
	return {position.x, position.y, (float)hitBoxSize, (float)hitBoxSize};
}

void Player::draw() const
{
	std::string currentAnimStr = enumToString(currentState);

	DrawTextureRec(anims.at(currentAnimStr).getTexture(), anims.at(currentAnimStr).animationFrame(),
		position, RAYWHITE);
}

Vector2 Player::getPosition() const
{
	return position;
}
