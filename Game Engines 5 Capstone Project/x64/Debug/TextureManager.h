#pragma once
#include "Window.h"

class TextureManager
{
public:
	static SDL_Texture* LoadTexture(const char* file_);
	static void DrawTexture(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest);
};

