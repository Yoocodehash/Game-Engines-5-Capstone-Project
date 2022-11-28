#include "pch.h"
#include "Vec2.h"

float Vec2Init(float x_, float y_)
{
	std::cout << "Vec2 Init = (" << x_ << ", " << y_ << ")" << std::endl;

	return x_, y_;
}

float Vec2Addition(float x_, float y_, float vx_, float vy_)
{
	float x = x_ + vx_;
	float y = y_ + vy_;

	std::cout << "Vec2 Addition: (" << x_ << " + " << vx_ << ", "
		<< y_ << " + " << vy_ << ") = (" << x << ", " << y << ")" << std::endl;

	return x, y;
}

float Vec2Subtraction(float x_, float y_, float vx_, float vy_)
{
	float x = x_ - vx_;
	float y = y_ - vy_;

	std::cout << "Vec2 Subtraction: (" << x_ << " - " << vx_ << ", "
		<< y_ << " - " << vy_ << ") = (" << x << ", " << y << ")" << std::endl;

	return x, y;
}

float Vec2Division(float x_, float y_, float vx_, float vy_)
{
	float x = x_ / vx_;
	float y = y_ / vy_;

	std::cout << "Vec2 Division: (" << x_ << " / " << vx_ << ", "
		<< y_ << " / " << vy_ << ") = (" << x << ", " << y << ")" << std::endl;

	return x, y;
}

float Vec2Multiplication(float x_, float y_, float vx_, float vy_)
{
	float x = x_ * vx_;
	float y = y_ * vy_;

	std::cout << "Vec2 Multiplication: (" << x_ << " * " << vx_ << ", "
		<< y_ << " * " << vy_ << ") = (" << x << ", " << y << ")" << std::endl;

	return x, y;
}

float Vec2Rotate(float x_, float y_, float degrees)
{
	float theta = degrees / 180.0f * 3.14f;
	float c = cos(theta);
	float s = sin(theta);
	float tx = x_ * c - y_ * s;
	float ty = x_ * s + y_ * c;
	float x = tx;
	float y = ty;

	std::cout << "Rotation occurs at Vec2 = (" << x << ", " << y << ")" << std::endl;

	return x, y;
}

float Vec2Normalize(float x_, float y_)
{
	float x = x_ / Vec2Magnitude(x_, y_);
	float y = y_ / Vec2Magnitude(x_, y_);

	std::cout << "Normalized Vec2 = (" << x << ", " << y << ")" << std::endl;

	return x, y;
}

float Vec2Magnitude(float x_, float y_)
{
	float mag = sqrt(x_ * x_ + y_ * y_);

	std::cout << "Magnitude Vec2 = " << mag << std::endl;

	return mag;
}

float Vec2Truncate(float x_, float y_, float length)
{
	float angle = atan2f(y_, x_);
	float x = length * cos(angle);
	float y = length * sin(angle);

	std::cout << "Truncate Vec2 = (" << x << ", " << y << ")" << std::endl;
	return x, y;
}

float Vec2Dot(float x_, float y_, float vx_, float vy_)
{
	float dot = x_ * vx_ + y_ * vy_;

	std::cout << "Dot Product Vec2 = " << dot << std::endl;
	return dot;
}

float Vec2Cross(float x_, float y_, float vx_, float vy_)
{
	float cross = (x_ * vy_) - (y_ * vx_);

	std::cout << "Cross Product Vec2 = " << cross << std::endl;
	return cross;
}