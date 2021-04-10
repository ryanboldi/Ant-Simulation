#include "LTexture.h"
#include <string>
#include <cmath>

LTexture::LTexture()
{
    //initialize
    mTexture = NULL;
    mWidth = 0;
    mHeight = 0;
}

LTexture::~LTexture()
{
    //deallocate
    free();
}

bool LTexture::loadFromFile(std::string path)
{
    //get rid of preexisting texture
    free();

    //the final texture
    SDL_Texture *newTexture = NULL;

    //load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {

        //color key the image
        //SDL_SetColorKey(loadedSurface, SDL_TRUE, SDL_MapRGBA(loadedSurface->format, 0xFF, 0xFF, 0xFF));

        //create new texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        else
        {
            //get image dimensions
            mWidth = loadedSurface->w;
            mHeight = loadedSurface->h;
        }

        //get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }

    //return success
    mTexture = newTexture;
    return mTexture != NULL;
}

bool LTexture::loadFromRenderedText(std::string textureText, SDL_Color textColor)
{
    //get rid of preexisting texture
    free();

    //Render text surface
    SDL_Surface *textSurface = TTF_RenderText_Solid(gFont, textureText.c_str(), textColor);
    if (textSurface == NULL)
    {
        printf("unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
    }
    else
    {
        //create texture from surface pixels
        mTexture = SDL_CreateTextureFromSurface(gRenderer, textSurface);
        if (mTexture == NULL)
        {
            printf("unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
        }
        else
        {
            //get image dimensions
            mWidth = textSurface->w;
            mHeight = textSurface->h;
        }

        //get rid of old surface
        SDL_FreeSurface(textSurface);
    }

    //return success
    return mTexture != NULL;
}

void LTexture::setColor(Uint8 red, Uint8 green, Uint8 blue)
{
    //modulate texture
    SDL_SetTextureColorMod(mTexture, red, green, blue);
}

void LTexture::setBlendMode(SDL_BlendMode blending)
{
    SDL_SetTextureBlendMode(mTexture, blending);
}

void LTexture::setAlpha(Uint8 alpha)
{
    //modulate the texture's alpha
    SDL_SetTextureAlphaMod(mTexture, alpha);
}

void LTexture::free()
{
    //free texture if it exists
    if (mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        mWidth = 0;
        mHeight = 0;
    }
}

void LTexture::render(int x, int y, SDL_Rect *clip, double angle, SDL_Point *center, SDL_RendererFlip flip)
{

    //set rendering sapce and render to screen
    SDL_Rect renderQuad = {x, y, mWidth, mHeight};

    //set clip rendering dimensions
    if (clip != NULL)
    {
        renderQuad.w = clip->w;
        renderQuad.h = clip->h;
    }

    SDL_RenderCopyEx(gRenderer, mTexture, clip, &renderQuad, angle, center, flip);
}

int LTexture::getWidth()
{
    return mWidth;
}

int LTexture::getHeight()
{
    return mHeight;
}