#include "Button.h"

Button::Button() {
    static SDL_Texture* buttonTexture = IMG_LoadTexture(ren, "Buttons.png");
    tex = buttonTexture;

    srect.h = 100;
    srect.w = 400;
    srect.x = 0;

    drect.h = 75;
    drect.w = 300;


}

Button::~Button() {
}

void Button::update(Mouse& mouse) {

    if (SDL_HasIntersection(&mouse.rect, &drect)) {
        isSelected = true;
        srect.x = 400;
    }
    else {
        isSelected = false;
        srect.x = 0;
    }
}

void Button::draw() {
    SDL_RenderCopy(ren, tex, &srect, &drect);
}