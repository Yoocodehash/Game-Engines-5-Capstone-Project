#include "TextureManager.h"

SDL_Texture* TextureManager::LoadTexture(const char* file_)
{
	SDL_Surface* anySurface = IMG_Load(file_);
	SDL_Texture* anyTexture = SDL_CreateTextureFromSurface(Window::renderer, anySurface);
	SDL_FreeSurface(anySurface);

	return anyTexture;
}

void TextureManager::DrawTexture(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest)
{
	SDL_RenderCopy(Window::renderer, tex, &src, &dest);
}
