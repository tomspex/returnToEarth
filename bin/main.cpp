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
    mainWindow = SDL_CreateWindow("Return to Earth", 100, 100, WIDTH, HEIGHT, SDL_WINDOW_RESIZABLE);
	mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderSetLogicalSize(mainRenderer, 640, 480);
    // ============= Window Setup End ==============

    audio audio;
    audio.initMusic();
    audio.playBG();
    
    bg bg;
    player player;
    npc npc;
    cubeBunch cubeBunch;
    text text;
    progressBar progressBar;

    bg.setTextures(mainRenderer);
    player.setTextures(mainRenderer);
    npc.setTextures(mainRenderer);
    cubeBunch.setTextures(mainRenderer);
    
    bg.initBG();
    player.initPlayer();
    npc.initNPC();
    cubeBunch.initCubes();
    text.initText(mainRenderer);
    text.updateObjectiveValue("Get to Earth", mainRenderer);
    text.updateHealthValue(player.getHealth(), mainRenderer);
    text.updateScoreValue(0, mainRenderer);
    

    bool quit = false;
    while( !quit ) {
        float frameStart = SDL_GetTicks();
        SDL_RenderClear(mainRenderer);
        // ============ Game Code Start =============

        bg.scrollBG();
        player.playerMovement();
        npc.moveNPC();
        cubeBunch.scrollCubes();
        progressBar.updateProgressBar();

        bg.drawBG(mainRenderer);
        player.drawPlayer(mainRenderer);
        npc.drawNPC(mainRenderer);
        cubeBunch.drawCubes(mainRenderer);
        text.drawText(mainRenderer);
        progressBar.drawProgressBar(mainRenderer);


        if(cubeBunch.detectCollisions(player.getPlayerRect())) {
            if(!alreadyHit) {
                alreadyHit = 1;
                health = player.hurt();
                bg.shakeBG();
                text.updateHealthValue(health, mainRenderer);
                audio.playHurt();

                if(health == 0) {
                    text.gameOverText(mainRenderer);
                    SDL_RenderPresent(mainRenderer);
                    audio.playGameOver();
                    audio.stopBG();
                    progressBar.resetLevelProgressOffset();
                    player.gameOver();
                    audio.playBG();
                    npc.initNPC();
                    bg.initBG();
                    text.updateHealthValue(player.getHealth(), mainRenderer);
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
            text.updateScoreValue(score, mainRenderer);
            text.updateLevelValue(level, mainRenderer);
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
    audio.freeAudio();
    quitSDL(mainWindow, mainRenderer);
    return (0);
    // ============= Window Close End ==============
}