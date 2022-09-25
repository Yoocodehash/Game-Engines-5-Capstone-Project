#include "Vec2.h"

Vec2::Vec2()
{
	x = 0.0f;
	y = 0.0f;
}

Vec2::Vec2(float x_, float y_)
{
	x = x_;
	y = y_;
}

Vec2& Vec2::Addition(const Vec2& vec_)
{
	this->x += vec_.x;
	this->y += vec_.y;

	return *this;
}

Vec2& Vec2::Subtraction(const Vec2& vec_)
{
	this->x -= vec_.x;
	this->y -= vec_.y;

	return *this;
}

Vec2& Vec2::Multiplication(const Vec2& vec_)
{
	this->x *= vec_.x;
	this->y *= vec_.y;

	return *this;
}

Vec2& Vec2::Division(const Vec2& vec_)
{
	this->x /= vec_.x;
	this->y /= vec_.y;

	return *this;
}

Vec2& operator+(Vec2& v1, const Vec2& v2)
{
	return v1.Addition(v2);
}

Vec2& operator-(Vec2& v1, const Vec2& v2)
{
	return v1.Subtraction(v2);
}

Vec2& operator*(Vec2& v1, const Vec2& v2)
{
	return v1.Multiplication(v2);
}

Vec2& operator/(Vec2& v1, const Vec2& v2)
{
	return v1.Division(v2);
}

Vec2& Vec2::operator+=(const Vec2& v)
{
	return this->Addition(v);
}

Vec2& Vec2::operator-=(const Vec2& v)
{
	return this->Subtraction(v);
}

Vec2& Vec2::operator*=(const Vec2& v)
{
	return this->Multiplication(v);
}

Vec2& Vec2::operator/=(const Vec2& v)
{
	return this->Division(v);
}

std::ostream& operator<<(std::ostream& stream, const Vec2& v)
{
	stream << "(" << v.x << " , " << v.y << ")";
	return stream;
}