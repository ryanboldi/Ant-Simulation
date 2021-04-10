#ifndef ANT_H
#define ANT_H

#include "LTexture.h"

#define ANT_WALK_FRAMES 4

class Ant
{
private:
    float x, y;
    float angle;
    unsigned short int scale = 1;
    static SDL_Rect mSpriteClips[ANT_WALK_FRAMES];
    static LTexture mSpriteSheetTexture;

public:
    // constructor
    Ant(float x, float y, unsigned short int scale = 1, float angle = 0) : x(x),
                                                                           y(y),
                                                                           scale(scale),
                                                                           angle(angle){};
    ~Ant();

    static bool loadSprites();

    // //getters
    // float get_x();
    // float get_y();
    // float get_angle();

    //render
    void render(SDL_Renderer *renderer, int frame);
};

#endif