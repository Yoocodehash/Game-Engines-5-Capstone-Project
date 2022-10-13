#pragma once

#include "Components.h"
#include"Vec2.h"

class PlayerTransformComponent : public Component
{
public:

	Vec2 position, velocity;

	int height = 69;
	int width = 100;
	int scale = 1;

	float speed = 0.3f;

	PlayerTransformComponent()
	{
		position.x = 0.0f;
		position.y = 0.0f;
	}

	PlayerTransformComponent(int scale_)
	{
		position.x = 0;
		position.y = 0;
		scale = scale_;
	}

	PlayerTransformComponent(float x_, float y_)
	{
		position.x = x_;
		position.y = y_;
	}

	PlayerTransformComponent(float x_, float y_, int h_, int w_, int scale_)
	{
		position.x = x_;
		position.y = y_;
		height = h_;
		width = w_;
		scale = scale_;
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