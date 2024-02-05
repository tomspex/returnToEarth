#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include "constants.hpp"
#include "SDLfunctions.hpp"
#include "classes.hpp"

int main() {
    // ============ Window Setup Start =============
    SDL_Window   *mainWindow = NULL;
    SDL_Renderer *mainRenderer = NULL;
    initSDL(mainWindow, mainRenderer);
    mainWindow = SDL_CreateWindow("SDL Image", 100, 100, WIDTH, HEIGHT, 0);
	mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
    //SDL_RenderSetLogicalSize(mainRenderer, 320, 240);
    // ============= Window Setup End ==============
    

    audio audio;
    audio.initMusic();
    audio.playBG();
    
    bg bg;
    player player;
    cubeBunch cubeBunch;
    
    bg.initBG();
    player.initPlayer();
    cubeBunch.initCubes();
    

    bool quit = false;
    while( !quit ) {
        float frameStart = SDL_GetTicks();
        SDL_RenderClear(mainRenderer);
        // ============ Game Code Start =============

        bg.scrollBG();
        player.playerMovement();
        cubeBunch.scrollCubes();

        if(cubeBunch.detectCollisions(player.getPlayerRect())) {
            //player.hurt();
            audio.playHurt();
        }

        bg.drawBG(mainRenderer);
        player.drawPlayer(mainRenderer);
        cubeBunch.drawCubes(mainRenderer);

        // ============  Game Code End  =============
        SDL_Event e;
        handleEvents(quit, e);

        SDL_RenderPresent(mainRenderer);
        int frameEnd = SDL_GetTicks();
        fpsCap(frameStart, frameEnd);
    }

    // ============ Window Close Start =============
    quitSDL(mainWindow, mainRenderer);
    return (0);
    // ============= Window Close End ==============
}