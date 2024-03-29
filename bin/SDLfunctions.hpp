#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#pragma once

#include <iostream>
#include "constants.hpp"
using namespace std;

// ==============================================================

void initSDL(SDL_Window *win, SDL_Renderer *ren) {

    SDL_Init(SDL_INIT_EVERYTHING);
    Mix_Init(0);
    TTF_Init();

    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);
}

// ==============================================================

void quitSDL(SDL_Window *win, SDL_Renderer *ren) {
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    Mix_Quit();
    TTF_Quit();
    SDL_Quit();
}

// ==============================================================

void handleEvents(bool &quit, SDL_Event e) {
    while( SDL_PollEvent( &e ) != 0 ) {
        // checks for exit button pressed
        if( e.type == SDL_QUIT )
        {
            quit = true;
        }
    }
}

// ==============================================================

void fpsCap(Uint64 start, Uint64 end) {
    float frameTime = end - start;

    if (FRAME_DELAY > frameTime) {
        SDL_Delay(FRAME_DELAY - frameTime);
    }
}

// ==============================================================

int updateClock(int clockIn) {
    clockIn++;
    if(clockIn >= 60) {
        clockIn = 0;
    }
    return(clockIn);
}