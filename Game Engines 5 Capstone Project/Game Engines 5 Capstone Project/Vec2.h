#pragma once
#include <iostream>

class Vec2
{
public:
	float x;
	float y;

	Vec2();
	Vec2(float x_, float y_);

	Vec2& Addition(const Vec2& vec_);
	Vec2& Subtraction(const Vec2& vec_);
	Vec2& Multiplication(const Vec2& vec_);
	Vec2& Division(const Vec2& vec_);

	friend Vec2& operator+(Vec2& v1, const Vec2& v2);
	friend Vec2& operator-(Vec2& v1, const Vec2& v2);
	friend Vec2& operator*(Vec2& v1, const Vec2& v2);
	friend Vec2& operator/(Vec2& v1, const Vec2& v2);

	Vec2& operator+=(const Vec2& v);
	Vec2& operator-=(const Vec2& v);
	Vec2& operator*=(const Vec2& v);
	Vec2& operator/=(const Vec2& v);

	Vec2& operator*(const int& i);

	friend std::ostream& operator<<(std::ostream& stream, const Vec2& v);
};