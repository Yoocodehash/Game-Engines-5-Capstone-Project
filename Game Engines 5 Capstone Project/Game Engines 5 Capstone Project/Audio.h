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
	Mix_Music* GameMusic;
	Mix_Music* CreditsMusic;
	Mix_Music* MainMenuMusic;

	//The sound effects that will be used
	Mix_Chunk* BirdSound;
	Mix_Chunk* LevelCompletionSound;
	Mix_Chunk* ButtonPressedSound;

};

