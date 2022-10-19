#include "Audio.h"

Audio::Audio()
{
    BirdSound = NULL;
    music = NULL;

    //Initialize SDL Mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
    {
        printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    }
}

void Audio::DestroyAudio()
{
    //Free the sound effects
    Mix_FreeChunk(BirdSound);
    BirdSound = NULL;

    std::cout << "\nBird sound effect has been freed!\n";

    //Free the music
    Mix_FreeMusic(music);
    music = NULL;
    
    std::cout << "The music has been freed!\n\n";

    Mix_Quit();
}

bool Audio::LoadAudio()
{
    //Load music
    music = Mix_LoadMUS("Audio/Free as a Bird.wav");
    if (music == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }

    //Load sound effects
    BirdSound = Mix_LoadWAV("Audio/Bird Sound.wav");
    if (BirdSound == NULL)
    {
        printf("Failed to load Bird Sound sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }

    return true;
}