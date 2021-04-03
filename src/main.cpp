#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

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

    //deallocate texture
    void free();

    //renders texture at given point
    void render(int x, int y, int scale);

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

//Scene textures
LTexture gAntTexture;
LTexture gBackgroundTexture;

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

void LTexture::render(int x, int y, int scale = 1)
{
    //set rendering sapce and render to screen
    SDL_Rect renderQuad = {x, y, mWidth / scale, mHeight / scale};
    SDL_RenderCopy(gRenderer, mTexture, NULL, &renderQuad);
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

            //create a renderer for window
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED);
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
            }
        }
    }
    return success;
}

bool loadMedia()
{
    //loading success flag
    bool success = true;

    //load Ant Texture
    if (!gAntTexture.loadFromFile("assets/ant/ant1.png"))
    {
        printf("Failed to load ant Image! \n");
        success = false;
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
    gAntTexture.free();
    gBackgroundTexture.free();

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //quit SDL Subsystems
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
                }

                //Clear the screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                gBackgroundTexture.render(0, 0);

                gAntTexture.render(240, 190, 2);

                //Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    close();
    return 0;
}
