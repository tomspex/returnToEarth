#include "keyInputs.hpp"
#include "constants.hpp"
#include <iostream>
using namespace std;

class bg {
    private:
        SDL_Surface *bgSurface = IMG_Load("assets/images/bg.png");
        SDL_Rect bgRect0;
        SDL_Rect bgRect1;
        int scroll;
    public:
        void initBG() {
            bgRect0.w = bgSurface->w;
            bgRect0.h = bgSurface->h;
            bgRect1.w = bgSurface->w;
            bgRect1.h = bgSurface->h;
            scroll = 0;
        }
        void scrollBG() {
            scroll -= 5;
            if (abs(scroll) > bgRect0.w) {
                scroll = 0;
            }
        }
        void drawBG(SDL_Renderer *renderer) {
            bgRect0.x = scroll;
            bgRect1.x = scroll + bgRect0.w;
            SDL_Texture *bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
            SDL_RenderCopy(renderer, bgTexture, NULL, &bgRect0);
            SDL_RenderCopy(renderer, bgTexture, NULL, &bgRect1);
        }
};
class player {
    private:
        SDL_Surface *playerSurface = IMG_Load("assets/images/player.png");
        SDL_Rect playerRect;
        const int initPlayerHealth = 3;
        int posX,
            posY, 
            playerWidth = 60,
            playerHeight = 60,
            playerHealth = initPlayerHealth,
            playerSpeed = 5;
        bool flipSprite;
    public:
        void initPlayer() {
            posY = HEIGHT/2;
            posX = 10;
            playerRect.h = playerHeight;
            playerRect.w = playerWidth;
        }
        void playerMovement() {
            flipSprite = getCurrentWASD(flipSprite);

            if(posX > WIDTH - playerWidth) {
                posX = WIDTH - playerWidth;
            }
            if(posY > HEIGHT - playerHeight){
                posY = HEIGHT - playerHeight;
            }
            if(posX < 0 and playerHealth != 0) {
                posX = 0;
            }
            if(posY < 0) {
                posY = 0;
            }
            posX += currentDirection.first * playerSpeed;
            posY += currentDirection.second * playerSpeed;
        }
        void drawPlayer(SDL_Renderer *renderer) {
            playerRect.x = posX;
            playerRect.y = posY;
            SDL_Texture *playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
            if(flipSprite) {
                SDL_RenderCopyEx(renderer, playerTexture, NULL, &playerRect, 0, 0, SDL_FLIP_HORIZONTAL);
            }else {
                SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);
            }
        }
        int getHealth() {
            return(playerHealth);
        }
        int hurt() {
            playerHealth -= 1;
            return(playerHealth);
        }
        void gameOver() {
            bool waiting = true;
            while(waiting) {
                posX -= 10;
                if(posX <= -playerWidth) {
                    posX = -playerWidth;
                }
                const Uint8 *keyState = SDL_GetKeyboardState(NULL);
                if(keyState[RESTART_KEY]) {
                    playerHealth = initPlayerHealth;
                    initPlayer();
                    score = 0;
                    
                    waiting = false;
                }
                SDL_Delay(30);

                SDL_Event e;
                while( SDL_PollEvent( &e ) != 0 ) {}
            }
        }
        SDL_Rect getPlayerRect() {
            return(playerRect);
        }
};
class cube {
    private:
        SDL_Surface *cubeSurface = IMG_Load("assets/images/storageCube/storageCubeSheet.png");
        SDL_Rect cubeRect;
        SDL_Rect cubeClip;
        const int initSpeed = -10,
            width = 64,
            height = 64;
        int currentSpeed = initSpeed,
            posX = -width,
            posY = 0,
            wait = 0,
            now = 0,
            animCount = 0;
    public:
        void initCube() {
            cubeRect.w = width;
            cubeRect.h = height;
            posX = -64;
            cubeClip.y = 0;
            cubeClip.w = 40;
            cubeClip.h = 40;
        }
        void scrollCube() {
            now++;
            if(now >= wait) {
                posX += currentSpeed;
            }
            if(posX <= -width) {
                posX = WIDTH;
                posY = rand() %HEIGHT;
                wait = rand() %250 + 10;
                now = 0;
            }
        }
        void drawCube(SDL_Renderer *renderer) {
            cubeRect.x = posX;
            cubeRect.y = posY;
            cubeClip.x = animCount*40;
            SDL_Texture *cubeTexture = SDL_CreateTextureFromSurface(renderer, cubeSurface);
            SDL_RenderCopy(renderer, cubeTexture, &cubeClip, &cubeRect);
            animCount++;
            if(animCount >= 23) {
                animCount = 0;
            }
        }
        bool detectCollision(SDL_Rect playerRect) {
            return(SDL_HasIntersection(&cubeRect, &playerRect));
        }
};
class cubeBunch {
    private:
        cube cube0;
        cube cube1;
        cube cube2;
        cube cube3;
        cube cube4;
    public:
        void initCubes() {
            cube0.initCube();
            cube1.initCube();
            cube2.initCube();
            cube3.initCube();
            cube4.initCube();
        }
        void scrollCubes() {
            cube0.scrollCube();
            cube1.scrollCube();
            cube2.scrollCube();
            cube3.scrollCube();
            cube4.scrollCube();
        }
        bool detectCollisions(SDL_Rect playerRect) {
            short checkAll;
            checkAll += cube0.detectCollision(playerRect);
            checkAll += cube1.detectCollision(playerRect);
            checkAll += cube2.detectCollision(playerRect);
            checkAll += cube3.detectCollision(playerRect);
            checkAll += cube4.detectCollision(playerRect);
            if(checkAll != 0) {
                return(1);
            }else{
                return(0);
            }
        }
        void drawCubes(SDL_Renderer *mainRenderer) {
            cube0.drawCube(mainRenderer);
            cube1.drawCube(mainRenderer);
            cube2.drawCube(mainRenderer);
            cube3.drawCube(mainRenderer);
            cube4.drawCube(mainRenderer);
        }
};
class audio {
    private:
        const char *bgSrc = "assets/music/bgUnused.wav";
        const char *hurtSrc = "assets/sounds/hit.wav";
        
        Mix_Chunk *hurtChunk = Mix_LoadWAV(hurtSrc);
    public:
        void initMusic() {
            Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);
        }
        void playBG() {
            Mix_Music *bgMusic = Mix_LoadMUS(bgSrc);
            Mix_PlayMusic(bgMusic, -1);
        }
        void playHurt() {
            Mix_Chunk *hurtChunk = Mix_LoadWAV(hurtSrc);
            Mix_PlayChannel(0, hurtChunk, 0);
        }
};
class text {
    private:
        TTF_Font *comicMono = TTF_OpenFont("assets/fonts/comicMono.ttf", 16);
        TTF_Font *comicGameOver = TTF_OpenFont("assets/fonts/comicMono.ttf", 64);
        SDL_Color White = {255, 255, 255};
        //Text contents
        SDL_Surface *objective = TTF_RenderText_Solid(comicMono, "NULL", White);
        SDL_Surface *health = TTF_RenderText_Solid(comicMono, "NULL", White);
        SDL_Surface *score = TTF_RenderText_Solid(comicMono, "NULL", White);
        SDL_Surface *gameOver = TTF_RenderText_Blended(comicGameOver, "Game Over!", White);
        SDL_Surface *gameOverSubtext = TTF_RenderText_Blended(comicMono, "Press T to Restart", White);

        SDL_Rect objectiveRect;
        SDL_Rect healthRect;
        SDL_Rect scoreRect;
        SDL_Rect gameOverRect;
        SDL_Rect gameOverSubtextRect;
    public:
        void initText() {
            objectiveRect.x = 5;
            objectiveRect.y = 5;
            healthRect.x = 5;
            healthRect.y = 25;
            scoreRect.x = 5;
            scoreRect.y = 45;
            gameOverRect.x = WIDTH/2-gameOver->w/2;
            gameOverRect.y = HEIGHT/2-gameOver->h;
            gameOverRect.w = gameOver->w;
            gameOverRect.h = gameOver->h;
            gameOverSubtextRect.x = WIDTH/2-gameOverSubtext->w/2;
            gameOverSubtextRect.y = HEIGHT/2+10;
            gameOverSubtextRect.w = gameOverSubtext->w;
            gameOverSubtextRect.h = gameOverSubtext->h;
        }
        void updateObjective(const char *objectiveName) {
            objective = TTF_RenderText_Blended(comicMono, objectiveName, White);
            objectiveRect.w = objective->w;
            objectiveRect.h = objective->h; 
        }
        void updateHealth(int newHealth) {
            string intToString = "Health: ";
            intToString += to_string(newHealth);
            health = TTF_RenderText_Blended(comicMono, intToString.c_str(), White);
            healthRect.w = health->w;
            healthRect.h = health->h; 
        }
        void updateScore(int scoreInt) {
            string intToString = "Score: ";
            intToString += to_string(scoreInt);
            score = TTF_RenderText_Blended(comicMono, intToString.c_str(), White);
            scoreRect.w = score->w;
            scoreRect.h = score->h; 
        }
        void drawText(SDL_Renderer *renderer) {
            SDL_Texture *objectiveTexture = SDL_CreateTextureFromSurface(renderer, objective);
            SDL_Texture *healthTexture = SDL_CreateTextureFromSurface(renderer, health);
            SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(renderer, score);
            SDL_RenderCopy(renderer, objectiveTexture, NULL, &objectiveRect);
            SDL_RenderCopy(renderer, healthTexture, NULL, &healthRect);
            SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
        }
        void gameOverText(SDL_Renderer *renderer) {
            SDL_Texture *gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOver);
            SDL_Texture *gameOverSubtextTexture = SDL_CreateTextureFromSurface(renderer, gameOverSubtext);
            SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);
            SDL_RenderCopy(renderer, gameOverSubtextTexture, NULL, &gameOverSubtextRect);
        }
};