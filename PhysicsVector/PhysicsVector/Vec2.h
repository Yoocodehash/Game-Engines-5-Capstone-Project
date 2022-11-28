#pragma once

#include <cmath>
#include <iostream>

#ifdef VECTORLIBRARY_EXPORTS
#define VECTORLIBRARY_API __declspec(dllexport)
#else
#define VECTORLIBRARY_API __declspec(dllimport)
#endif

extern "C" VECTORLIBRARY_API float Vec2Init(float x_, float y_);
extern "C" VECTORLIBRARY_API float Vec2Addition(float x_, float y_, float vx_, float vy_);
extern "C" VECTORLIBRARY_API float Vec2Subtraction(float x_, float y_, float vx_, float vy_);
extern "C" VECTORLIBRARY_API float Vec2Division(float x_, float y_, float vx_, float vy_);
extern "C" VECTORLIBRARY_API float Vec2Multiplication(float x_, float y_, float vx_, float vy_);

extern "C" VECTORLIBRARY_API float Vec2Rotate(float x_, float y_, float degrees);
extern "C" VECTORLIBRARY_API float Vec2Normalize(float x_, float y_);

extern "C" VECTORLIBRARY_API float Vec2Magnitude(float x_, float y_);
extern "C" VECTORLIBRARY_API float Vec2Truncate(float x_, float y_, float length);

extern "C" VECTORLIBRARY_API float Vec2Dot(float x_, float y_, float vx_, float vy_);
extern "C" VECTORLIBRARY_API float Vec2Cross(float x_, float y_, float vx_, float vy_);