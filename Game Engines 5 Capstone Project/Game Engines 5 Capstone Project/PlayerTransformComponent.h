#pragma once

#include "Components.h"
#include"Vec2.h"

class PlayerTransformComponent : public Component
{
public:

	Vec2 position, velocity;

	float speed = 0.3f;

	PlayerTransformComponent()
	{
		position.x = 0.0f;
		position.y = 0.0f;
	}

	PlayerTransformComponent(float x_, float y_)
	{
		position.x = x_;
		position.y = y_;
	}

	void PlayerInit() override
	{
		velocity.x = 0;
		velocity.y = 0;
	}

	void UpdatePlayer() override
	{
		position.x += velocity.x * speed;
		position.y += velocity.y * speed;
	}
};