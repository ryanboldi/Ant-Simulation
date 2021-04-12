#include "LButton.h"

LButton::LButton()
{
    mPosition.x = 0;
    mPosition.y = 0;

    mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::setPosition(int x, int y)
{
    mPosition.x = x;
    mPosition.y = y;
}

void LButton::handleEvent(SDL_Event *e)
{
    //if the event was mouse related
    if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
    {
        //get the mouse position
        int x, y;
        SDL_GetMouseState(&x, &y);

        //check if the mouse is in this button
        bool inside = true;

        if (x < mPosition.x)
        {
            inside = false;
        }
        else if (x > mPosition.x + BUTTON_WIDTH)
        {
            inside = false;
        }
        else if (y < mPosition.y)
        {
            inside = false;
        }
        else if (y > mPosition.y + BUTTON_HEIGHT)
        {
            inside = false;
        }

        //mouse outside button
        if (!inside)
        {
            mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
        }
        //mouse inside button
        else
        {
            //set mouse over sprite
            switch (e->type)
            {
            case SDL_MOUSEMOTION:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_OVER_MOTION;
                break;

            case SDL_MOUSEBUTTONDOWN:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_DOWN;
                break;

            case SDL_MOUSEBUTTONUP:
                mCurrentSprite = BUTTON_SPRITE_MOUSE_UP;
                break;
            }
        }
    }
}

void LButton::render()
{
    //show the current button sprite
    gButtonSpriteSheetTexture.render(mPosition.x, mPosition.y, &gSpriteClips[mCurrentSprite]);
}
