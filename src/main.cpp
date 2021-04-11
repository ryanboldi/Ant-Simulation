#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>
#include <cmath>

#include "ant.h"

//starts up SDL and create the window
bool init();

//Loads media
bool loadMedia();

//frees media and shuts down SDL
void close();

//Loads individual image as texture
SDL_Texture *loadTexture(std::string path);

//Scene textures
LTexture gTextTexture;
LTexture gBackgroundTexture;

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

    if (!Ant::loadSprites())
    {
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
    gTextTexture.free();
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
                }

                //Clear the screen
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
                SDL_RenderClear(gRenderer);

                //modulate the texture's color a bit
                gBackgroundTexture.render(0, 0);

                //render text
                gTextTexture.render((SCREEN_WIDTH - gTextTexture.getWidth()) / 2, (SCREEN_HEIGHT - gTextTexture.getHeight()) / 2);

                //update screen
                SDL_RenderPresent(gRenderer);

                //go to next frame
                ++frame;

                //cycle the animation
                if (frame / 4 >= ANT_WALK_FRAMES)
                {
                    frame = 0;
                }
            }
        }
    }

    close();
    return 0;
}
