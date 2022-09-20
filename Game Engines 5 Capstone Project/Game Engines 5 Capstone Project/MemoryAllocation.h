#pragma once

#include <stdio.h>
#include <iostream>
#include <queue>

struct Vec4
{
	float x, y, z, w;
};

struct Vec3
{
	float x, y, z;
};

struct Vec2
{
	float x, y;
};

struct VectorVariables
{
	Vec4 a;
	Vec4 b;
	Vec3 c;
	Vec3 d;
	Vec3 e;
	Vec2 f;
	Vec2 g;
};

class MemoryAllocation
{
public:

	MemoryAllocation();
	~MemoryAllocation();

	void ReturnMemoryAllocation();

};

