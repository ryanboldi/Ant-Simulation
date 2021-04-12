#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>

//screen dimension constants
extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;

//the window we will be rendering to
extern SDL_Window *gWindow;

//the window renderer
extern SDL_Renderer *gRenderer;

//the global font
extern TTF_Font *gFont;

// texture wrapper class
class LTexture
{
public:
    //initializes variables
    LTexture();

    //Deallocates memory
    ~LTexture();

    //loads image at specified path
    bool loadFromFile(std::string path);

//only load ttf if we need it
#if defined(SDL_TTF_MAJOR_VERSION)
    //creates an image from font string
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);
#endif

    //set color modulation
    void setColor(Uint8 red, Uint8 green, Uint8 blue);

    //set blending
    void setBlendMode(SDL_BlendMode blending);

    //set alpha modulation
    void setAlpha(Uint8 alpha);

    //deallocate texture
    void free();

    //renders texture at given point
    void render(int x, int y, SDL_Rect *clip = NULL, double angle = 0.0, SDL_Point *center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

    //get image dimensions
    int getWidth();
    int getHeight();

private:
    //the actual hardware texture
    SDL_Texture *mTexture;

    //Image dimensions
    int mWidth;
    int mHeight;
};

#endif