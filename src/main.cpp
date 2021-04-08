#include <SDL.h>
#include <SDL_image.h>
#include <SDl_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>

//screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

//texture wrapper class
class LTexture
{
public:
    //initializes variables
    LTexture();

    //Deallocates memory
    ~LTexture();

    //loads image at specified path
    bool loadFromFile(std::string path);

    //creates an image from font string
    bool loadFromRenderedText(std::string textureText, SDL_Color textColor);

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

//starts up SDL and create the window
bool init();

//Loads media
bool loadMedia();

//frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture *loadTexture(std::string path);

//the window we will be rendering to
SDL_Window *gWindow = NULL;

//the window renderer
SDL_Renderer *gRenderer = NULL;

//the global font
TTF_Font *gFont = NULL;

//Scene textures
LTexture gTextTexture;
LTexture gAntTexture;
LTexture gBackgroundTexture;

//scene sprites
const int antWalkFrames = 4;
SDL_Rect gSpriteClips[antWalkFrames];
LTexture gSpriteSheetTexture;

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

bool init()
{
    //Init flag
    bool success = true;

    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        //set texture filtering to linear
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Warning: Linear texture filtering not enabled!");
        }

        //create window
        gWindow = SDL_CreateWindow("Ant-Simulation", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {

            //create a vsync renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                //initialize renderer color
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                //initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                }

                //initialize SDL_TTF
                if (TTF_Init() == -1)
                {
                    printf("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

bool loadMedia()
{
    //loading success flag
    bool success = true;

    //load text
    gFont = TTF_OpenFont("assets/OpenSans-Regular.ttf", 28);
    if (gFont == NULL)
    {
        printf("Failed to load font! SDL_ttf Error: %s\n", TTF_GetError());
        success = false;
    }
    else
    {
        //render the text
        SDL_Color textColor = {0, 0, 0};
        if (!gTextTexture.loadFromRenderedText("Ant ant ant ant", textColor))
        {
            printf("Failed to render text texutre!\n Ryan_Error 0001");
            success = false;
        }
    }

    //load Ant Texture
    if (!gSpriteSheetTexture.loadFromFile("assets/ant/ant-walk.png"))
    {
        printf("Failed to load ant Image! \n");
        success = false;
    }
    else
    {
        //set ant 1
        gSpriteClips[0].x = 0;
        gSpriteClips[0].y = 0;
        gSpriteClips[0].w = 64;
        gSpriteClips[0].h = 64;

        //set ant 2
        gSpriteClips[1].x = 64;
        gSpriteClips[1].y = 0;
        gSpriteClips[1].w = 64;
        gSpriteClips[1].h = 64;

        //set ant 3
        gSpriteClips[2].x = 128;
        gSpriteClips[2].y = 0;
        gSpriteClips[2].w = 64;
        gSpriteClips[2].h = 64;

        //set ant 5
        gSpriteClips[3].x = 192;
        gSpriteClips[3].y = 0;
        gSpriteClips[3].w = 64;
        gSpriteClips[3].h = 64;

        gSpriteSheetTexture.setBlendMode(SDL_BLENDMODE_BLEND);
    }

    if (!gBackgroundTexture.loadFromFile("assets/background.png"))
    {
        printf("Failed to load background Image! \n");
        success = false;
    }

    return success;
}

void close()
{

    //free loaded images
    gTextTexture.free();
    gAntTexture.free();
    gBackgroundTexture.free();

    //free the global font
    TTF_CloseFont(gFont);
    gFont = NULL;

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //quit SDL Subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}

int main(int argc, char *args[])
{
    if (!init())
    {
        printf("Failed to initialize!\n");
    }
    else
    {
        //load media
        if (!loadMedia())
        {
            printf("Failed to load media!\n");
        }
        else
        {
            //Main loop flag
            bool quit = false;

            //SDL event handler
            SDL_Event e;

            //current animation frame
            int frame = 0;

            //Color modulation components
            Uint8 r = 255;
            Uint8 g = 255;
            Uint8 b = 255;

            //alpha modulation component
            Uint8 a = 255;

            //angle of rotation
            double degrees = 0;

            //flip type
            SDL_RendererFlip flipType = SDL_FLIP_NONE;

            //While application is running
            while (!quit)
            {
                //handle events on queue
                while (SDL_PollEvent(&e) != 0)
                {
                    //user requests quit
                    if (e.type == SDL_QUIT)
                    {
                        quit = true;
                    }
                    //on keypress change the rgb values
                    else if (e.type == SDL_KEYDOWN)
                    {
                        switch (e.key.keysym.sym)
                        {
                        case SDLK_a:
                            degrees -= 60;
                            break;

                        case SDLK_d:
                            degrees += 60;
                            break;

                        case SDLK_q:
                            flipType = SDL_FLIP_HORIZONTAL;
                            break;

                        case SDLK_w:
                            flipType = SDL_FLIP_NONE;
                            break;

                        case SDLK_e:
                            flipType = SDL_FLIP_VERTICAL;
                            break;
                        }
                    }
                }

                //Clear the screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                //modulate the texture's color a bit
                //gBackgroundTexture.setColor(0, 0, 0);
                gBackgroundTexture.render(0, 0);

                //set all the ants to have an alpha that we can modulate
                gSpriteSheetTexture.setAlpha(a);

                //render the current frame
                SDL_Rect *currentClip = &gSpriteClips[frame / 4];
                gSpriteSheetTexture.render((SCREEN_WIDTH - currentClip->w) / 2, (SCREEN_HEIGHT - currentClip->h) / 2, currentClip, degrees, NULL, flipType);

                //render text
                gTextTexture.render((SCREEN_WIDTH - gTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gTextTexture.getHeight()) / 2);

                //update screen
                SDL_RenderPresent(gRenderer);

                //go to next frame
                ++frame;

                //cycle the animation
                if (frame / 4 >= antWalkFrames)
                {
                    frame = 0;
                }
            }
        }
    }

    close();
    return 0;
}
