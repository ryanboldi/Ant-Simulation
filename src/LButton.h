#ifndef LBUTTON_H
#define LBUTTON_H

#include <SDL.h>
#include "LTexture.h"

//button constants
const int BUTTON_WIDTH = 150;
const int BUTTON_HEIGHT = 100;
const int TOTAL_BUTTONS = 4;

enum LButtonSprite
{
    BUTTON_SPRITE_MOUSE_OUT = 0,
    BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
    BUTTON_SPRITE_MOUSE_DOWN = 2,
    BUTTON_SPRITE_MOUSE_UP = 3,
    BUTTON_SPRITE_TOTAL = 4
};

extern LTexture gButtonSpriteSheetTexture;
extern SDL_Rect gButtonClips[BUTTON_SPRITE_TOTAL];

class LButton
{
public:
    // intializes internal variables
    LButton();

    //sets top left position
    void setPosition(int x, int y);

    //handles mouse event
    void handleEvent(SDL_Event *e);

    // shows button sprite
    void render();

private:
    //top left position
    SDL_Point mPosition;

    //currently used global sprite
    LButtonSprite mCurrentSprite;
};

#endif