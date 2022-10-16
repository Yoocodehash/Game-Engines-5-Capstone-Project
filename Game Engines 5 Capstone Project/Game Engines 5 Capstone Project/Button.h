#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

extern SDL_Window* win;
extern SDL_Renderer* ren;
extern double delta;

class Mouse {
public:
    SDL_Texture* tex;
    SDL_Rect rect;
    SDL_Rect point;

    Mouse() {
        tex = IMG_LoadTexture(ren, "mouse.png");
        rect.w = 25;
        rect.h = 25;
        point.w = 1;
        point.h = 1;
        SDL_ShowCursor(true);
    }

    void update() {
        SDL_GetMouseState(&rect.x, &rect.y);
        point.x = rect.x;
        point.y = rect.y;

    }

    void draw() {
        SDL_RenderCopy(ren, tex, NULL, &rect);
    }
};

class Button
{

public:
    SDL_Texture* tex;
    SDL_Rect srect;
    SDL_Rect drect;
    bool isSelected = false;
    Button();
    virtual ~Button();

    void update(Mouse&);
    void draw();
};

