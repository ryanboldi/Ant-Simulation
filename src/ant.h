#ifndef ANT_H
#define ANT_H

#include <SDL.h>

class Ant
{
private:
    float x, y;
    float angle;
    unsigned short int scale = 1;

public:
    // constructor
    Ant(float x, float y, unsigned short int scale = 1, float angle = 0) : x(x), y(y), scale(scale), angle(angle){};
    ~Ant();

    //getters
    float get_x();
    float get_y();
    float get_angle();

    //render
    void render(SDL_Renderer *renderer);
};

#endif