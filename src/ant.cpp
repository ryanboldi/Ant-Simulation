#include "ant.h"

Ant::~Ant()
{
}

static void loadSprites(SDL_Rect spriteClipArray[ANT_WALK_FRAMES])
{
    LTexture antSpriteSheetTexture;

    //load Ant Texture
    if (!antSpriteSheetTexture.loadFromFile("assets/ant/ant-walk.png"))
    {
        printf("Failed to load ant Image! \n");
    }
    else
    {
        //set ant 1
        spriteClipArray[0].x = 0;
        spriteClipArray[0].y = 0;
        spriteClipArray[0].w = 64;
        spriteClipArray[0].h = 64;

        //set ant 2
        spriteClipArray[1].x = 64;
        spriteClipArray[1].y = 0;
        spriteClipArray[1].w = 64;
        spriteClipArray[1].h = 64;

        //set ant 3
        spriteClipArray[2].x = 128;
        spriteClipArray[2].y = 0;
        spriteClipArray[2].w = 64;
        spriteClipArray[2].h = 64;

        //set ant 4
        spriteClipArray[3].x = 192;
        spriteClipArray[3].y = 0;
        spriteClipArray[3].w = 64;
        spriteClipArray[3].h = 64;

        antSpriteSheetTexture.setBlendMode(SDL_BLENDMODE_BLEND);
    }
}