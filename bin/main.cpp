#include "include/SDL2/SDL.h"
#include "include/SDL2/SDL_image.h"
#include "include/SDL2/SDL_mixer.h"
#include "include/SDL2/SDL_ttf.h"
#include "constants.hpp"
#include "SDLfunctions.hpp"
#include "classes.hpp"

int main() {
    // ============ Window Setup Start =============
    SDL_Window   *mainWindow = NULL;
    SDL_Renderer *mainRenderer = NULL;
    initSDL(mainWindow, mainRenderer);
    mainWindow = SDL_CreateWindow("Return to Earth", 100, 100, WIDTH, HEIGHT, 0);
	mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
    //SDL_RenderSetLogicalSize(mainRenderer, 320, 240);
    // ============= Window Setup End ==============

    audio audio;
    audio.initMusic();
    audio.playBG();
    
    bg bg;
    player player;
    cubeBunch cubeBunch;
    text text;
    
    bg.initBG();
    player.initPlayer();
    cubeBunch.initCubes();
    text.initText();
    text.updateObjective("Objective: Get to Earth");
    text.updateHealth(player.getHealth());
    text.updateScore(0);
    

    bool quit = false;
    while( !quit ) {
        float frameStart = SDL_GetTicks();
        SDL_RenderClear(mainRenderer);
        // ============ Game Code Start =============

        bg.scrollBG();
        player.playerMovement();
        cubeBunch.scrollCubes();

        bg.drawBG(mainRenderer);
        player.drawPlayer(mainRenderer);
        cubeBunch.drawCubes(mainRenderer);
        text.drawText(mainRenderer);

        if(cubeBunch.detectCollisions(player.getPlayerRect())) {
            if(!alreadyHit) {
                alreadyHit = 1;
                health = player.hurt();
                text.updateHealth(health);
                audio.playHurt();

                if(health == 0) {
                    text.gameOverText(mainRenderer);
                    SDL_RenderPresent(mainRenderer);
                    player.gameOver();
                    bg.initBG();
                    cubeBunch.initCubes();
                    bg.drawBG(mainRenderer);
                    cubeBunch.drawCubes(mainRenderer);
                    health = player.getHealth();
                }
            }
        }else {
            alreadyHit = 0;
        }


        if(clockI == 0) {
            score++;
            text.updateScore(score);
        }
        clockI = updateClock(clockI);

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