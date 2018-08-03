/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL and standard IO
#include <SDL2/SDL.h>
#include <stdio.h>

#include "Texture.h"

//Screen dimension constants
const int LOGICAL_WIDTH = 360;
const int LOGICAL_HEIGHT = 180;

struct vector {
    int x;
    int y;
};

bool init(SDL_Window *&, SDL_Renderer *&, Texture *&);

void close(SDL_Window *, SDL_Renderer *);

int main(int argc, char* args[]) {
    SDL_Window *    window      = NULL;
    SDL_Renderer *  renderer    = NULL;
    Texture *       texture     = NULL;

    vector pos = {0, 0};

    Uint32 ticks = 0;
    Uint32 delta = 0;
    Uint32 accumulator = 0;

    Uint32 frametime = 250;

    if(!init(window, renderer, texture)) {
        return 1;
    }

    SDL_Event e;
    bool done = false;
    while(!done) {
        Uint32 newTicks = SDL_GetTicks();
        delta = newTicks - ticks;
        ticks = newTicks;

        accumulator += delta;

        while(SDL_PollEvent(&e) != 0) {
            switch(e.type) {
                case SDL_QUIT:
                    done = true;
                    break;
                case SDL_KEYDOWN:
                    if(e.key.keysym.sym == SDLK_UP)      pos.y--;
                    if(e.key.keysym.sym == SDLK_DOWN)    pos.y++;
                    if(e.key.keysym.sym == SDLK_LEFT)    pos.x--;
                    if(e.key.keysym.sym == SDLK_RIGHT)   pos.x++;
                    if(e.key.keysym.sym == SDLK_w)    frametime -= 10;
                    if(e.key.keysym.sym == SDLK_s)   frametime += 10;
                    break;
            }
        }

        int spriteWidth = 16;
        int frame;
        if(accumulator > frametime * 2) {
            frame = 0;
            accumulator = 0;
        } else if(accumulator > frametime) {
            frame = 16;
        } else {
            frame = 0;
        }

        SDL_Rect clip = {0, frame, 16, 16};

        SDL_RenderClear(renderer);

        texture->render(clip, pos.x*spriteWidth, pos.y*spriteWidth);

        SDL_RenderPresent(renderer);
    }

    close(window, renderer);
    texture->free();

    window = NULL;
    renderer = NULL;

    return 0;
}

bool init(SDL_Window *& window, SDL_Renderer *& renderer, Texture *& texture) {
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL failed to initialize: %s", SDL_GetError());
        return false;
    }

    window = SDL_CreateWindow(
        "SDL Tutorial",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1280,
        720,
        SDL_WINDOW_SHOWN
    );

    if(window == NULL) {
        printf("Failed to instaniate window: %s", SDL_GetError());
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if(renderer == NULL) {
        printf("Could not initialize renderer Error: %s\n", SDL_GetError());
        return false;
    }

    SDL_RenderSetLogicalSize(renderer, LOGICAL_WIDTH, LOGICAL_HEIGHT);
    //SDL_RenderSetIntegerScale(renderer, SDL_TRUE);

    if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
        printf("Failed to init SDL_Image Error: %s", SDL_GetError());
        return false;
    }

    texture = new Texture(renderer);
    texture->loadFromFile("./spriteSheet.png");

    return true;
}

void close(SDL_Window * window, SDL_Renderer * renderer) {
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}
