#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <iostream>

class Audio
{
public:
	Audio();

	void DestroyAudio();
	bool LoadAudio();

	//The music that will be played
	Mix_Music* music;
	Mix_Music* creditsMusic;

	//The sound effects that will be used
	Mix_Chunk* BirdSound;
};

