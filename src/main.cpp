#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

//screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

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

//current displayed texture
SDL_Texture *gTexture = NULL;

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

    //load PNG Texture
    //gTexture = loadTexture("assets/ant.png");
    //if(gTexture == NULL)
    //{
    //   printf("Failed to load texture image!\n");
    //   success = false;
    //}
    return success;
}

void close()
{
    //Free the loaded image
    SDL_DestroyTexture(gTexture);
    gTexture = NULL;

    //Destroy window
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    //quit SDL Subsystems
    IMG_Quit();
    SDL_Quit();
}

SDL_Texture *loadTexture(std::string path)
{
    //the final texture
    SDL_Texture *newTexture = NULL;

    //load image at specified path
    SDL_Surface *loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        //create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }

        //get rid of the old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
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

                //Render red filled quad
                //struct
                SDL_Rect fillRect = {SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2};
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0x00, 0x00, 0xFF);
                SDL_RenderFillRect(gRenderer, &fillRect);

                //Render green outlined quad
                SDL_Rect outlineRect = {SCREEN_WIDTH / 6, SCREEN_HEIGHT / 6, SCREEN_WIDTH * 2 / 3, SCREEN_HEIGHT * 2 / 3};
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0xFF, 0x00, 0xFF);
                SDL_RenderDrawRect(gRenderer, &outlineRect);

                //Draw blue horizontal line
                SDL_SetRenderDrawColor(gRenderer, 0x00, 0x00, 0xFF, 0xFF);
                SDL_RenderDrawLine(gRenderer, 0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SCREEN_HEIGHT / 2);

                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0x00, 0xFF);
                for (int i = 0; i < SCREEN_HEIGHT; i += 4)
                {
                    SDL_RenderDrawPoint(gRenderer, SCREEN_WIDTH / 2, i);
                }

                //render texture to screen
                //SDL_RenderCopy(gRenderer, gTexture, NULL, NULL);

                //Update screen
                SDL_RenderPresent(gRenderer);
            }
        }
    }

    close();
    return 0;
}