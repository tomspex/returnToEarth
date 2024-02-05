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
        int posX,
            posY, 
            playerWidth = 60,
            playerHeight = 60,
            playerHealth = 3,
            playerSpeed = 5;
        bool flipSprite;
    public:
        void initPlayer() {
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
        void hurt() {

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
            cout << cubeRect.x << " - X " << cubeRect.y << " - Y " << now << " - now " << wait << " - wait " << "\n";
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
/*
class text {
    private:
        TTF_Font *comicMono = TTF_OpenFont("assets/fonts/comicMono.ttf", 24);
        SDL_Color White = {255, 255, 255};
        //Text contents
        SDL_Surface *objective = TTF_RenderText_Solid(comicMono, "NULL", White);
        SDL_Surface *score = TTF_RenderText_Solid(comicMono, "NULL", White);
        
    public:
        void updateObjective(const char *objectiveName) {
            objective = TTF_RenderText_Solid(comicMono, objectiveName, White);
        }
        void updateScore(const char *scoreInt) {
            score = TTF_RenderText_Solid(comicMono, scoreInt, White);
        }
        void drawText(SDL_Renderer *renderer) {
            SDL_Texture *objectiveTexture = SDL_CreateTextureFromSurface(renderer, objective);
            SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(renderer, score);
            SDL_RenderCopy(renderer, objectiveTexture, NULL, NULL);
            SDL_RenderCopy(renderer, scoreTexture, NULL, NULL);
        }
};
*/