#include "ant.h"

SDL_Rect Ant::mSpriteClips[ANT_WALK_FRAMES];
LTexture Ant::mSpriteSheetTexture;

Ant::~Ant()
{
}

bool Ant::loadSprites()
{
    bool success = true;

    //load Ant Texture
    if (!mSpriteSheetTexture.loadFromFile("assets/ant/ant-walk.png"))
    {
        printf("Failed to load ant Image! \n");
        success = false;
    }
    else
    {
        //set ant 1
        mSpriteClips[0].x = 0;
        mSpriteClips[0].y = 0;
        mSpriteClips[0].w = 64;
        mSpriteClips[0].h = 64;

        //set ant 2
        mSpriteClips[1].x = 64;
        mSpriteClips[1].y = 0;
        mSpriteClips[1].w = 64;
        mSpriteClips[1].h = 64;

        //set ant 3
        mSpriteClips[2].x = 128;
        mSpriteClips[2].y = 0;
        mSpriteClips[2].w = 64;
        mSpriteClips[2].h = 64;

        //set ant 4
        mSpriteClips[3].x = 192;
        mSpriteClips[3].y = 0;
        mSpriteClips[3].w = 64;
        mSpriteClips[3].h = 64;

        mSpriteSheetTexture.setBlendMode(SDL_BLENDMODE_BLEND);
    }

    return success;
}

void Ant::render(SDL_Renderer *renderer, int frame)
{
    //render the current frame
    SDL_Rect *currentClip = &mSpriteClips[frame / 4];
    mSpriteSheetTexture.render(x,
                               y,
                               currentClip, angle, NULL, SDL_FLIP_NONE);
}