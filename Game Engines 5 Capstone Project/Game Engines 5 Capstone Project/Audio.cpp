#include "Audio.h"

Audio::Audio()
{
    BirdSound = NULL;
    ButtonPressedSound = NULL;
    LevelCompletionSound = NULL;
    GameMusic = NULL;
    CreditsMusic = NULL;
    MainMenuMusic = NULL;

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

    Mix_FreeChunk(ButtonPressedSound);
    ButtonPressedSound = NULL;

    std::cout << "Button sound effect has been freed!\n";

    Mix_FreeChunk(LevelCompletionSound);
    LevelCompletionSound = NULL;

    std::cout << "Level completed sound effect has been freed!\n";

    //Free the music
    Mix_FreeMusic(GameMusic);
    GameMusic = NULL;
    
    std::cout << "The music has been freed!\n";

    Mix_FreeMusic(CreditsMusic);
    CreditsMusic = NULL;

    std::cout << "The credits music has been freed!\n";

    Mix_FreeMusic(MainMenuMusic);
    MainMenuMusic = NULL;

    std::cout << "The main menu music has been freed!\n\n";

    Mix_Quit();
}

bool Audio::LoadAudio()
{
    //Load music
    GameMusic = Mix_LoadMUS("Audio/Free as a Bird.wav");
    if (GameMusic == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }

    CreditsMusic = Mix_LoadMUS("Audio/Credits music.wav");
    if (CreditsMusic == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }

    MainMenuMusic = Mix_LoadMUS("Audio/Main Menu Music.wav");
    if (MainMenuMusic == NULL)
    {
        printf("Failed to load beat music! SDL_mixer Error: %s\n", Mix_GetError());
    }

    //Load sound effects
    BirdSound = Mix_LoadWAV("Audio/Bird Sound.wav");
    if (BirdSound == NULL)
    {
        printf("Failed to load Bird Sound sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }

    ButtonPressedSound = Mix_LoadWAV("Audio/Select button.wav");
    if (ButtonPressedSound == NULL)
    {
        printf("Failed to load Bird Sound sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }

    LevelCompletionSound = Mix_LoadWAV("Audio/Level completed.wav");
    if (LevelCompletionSound == NULL)
    {
        printf("Failed to load Bird Sound sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }

    return true;
}