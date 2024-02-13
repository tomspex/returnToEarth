#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "keyInputs.hpp"
#include "constants.hpp"
#include <iostream>
#include <string>
using namespace std;

class bg {
    private:
        SDL_Surface *bgSurface = IMG_Load("assets/images/bg/bg.png");
        SDL_Surface *bg1Surface = IMG_Load("assets/images/bg/bg1.png");
        SDL_Surface *bg2Surface = IMG_Load("assets/images/bg/bg2.png");
        SDL_Texture *bgTexture, *bg1Texture, *bg2Texture;
        SDL_Rect bgRect0, bgRect1, bgRect2, bgRect3, bgRect4, bgRect5;
        int scroll0, scroll1, scroll2, shakeX;
        bool shake = 0;
    public:
        void setTextures(SDL_Renderer *renderer) {
            bgTexture = SDL_CreateTextureFromSurface(renderer, bgSurface);
            bg1Texture = SDL_CreateTextureFromSurface(renderer, bg1Surface);
            bg2Texture = SDL_CreateTextureFromSurface(renderer, bg2Surface);
        }
        void initBG() {
            bgRect0.w = bgSurface->w;
            bgRect1.w = bgSurface->w;
            bgRect2.w = bgSurface->w;
            bgRect3.w = bgSurface->w;
            bgRect4.w = bgSurface->w;
            bgRect5.w = bgSurface->w;
            bgRect0.h = bgSurface->h;
            bgRect1.h = bgSurface->h;
            bgRect2.h = bgSurface->h;
            bgRect3.h = bgSurface->h;
            bgRect4.h = bgSurface->h;
            bgRect5.h = bgSurface->h;

            bgRect0.y = 0;
            bgRect1.y = 0;
            bgRect2.y = 0;
            bgRect3.y = 0;
            bgRect4.y = 0;
            bgRect5.y = 0;
            scroll0 = 0;
            scroll1 = 0;
        }
        void scrollBG() {
            scroll0 -= 3;
            scroll1 -= 2;
            scroll2 -= 1;
            if (abs(scroll0) > bgRect0.w) {
                scroll0 = 0;
            }
            if (abs(scroll1) > bgRect2.w) {
                scroll1 = 0;
            }
            if (abs(scroll2) > bgRect4.w) {
                scroll2 = 0;
            }
            
            if(shake) {
                scroll0 -= shakeX;
                scroll1 -= shakeX;
                scroll2 -= shakeX;
                shakeX--;
                if(shakeX <= 0) {
                    shake = 0;
                }
            }
        }
        void shakeBG() {
            shake = 1;
            shakeX = 10;
        }
        void drawBG(SDL_Renderer *renderer) {
            bgRect0.x = scroll0;
            bgRect1.x = scroll0 + bgRect0.w;
            bgRect2.x = scroll1;
            bgRect3.x = scroll1 + bgRect2.w;
            bgRect4.x = scroll2;
            bgRect5.x = scroll2 + bgRect4.w;
            SDL_RenderCopy(renderer, bg2Texture, NULL, &bgRect4);
            SDL_RenderCopy(renderer, bg2Texture, NULL, &bgRect5);
            SDL_RenderCopy(renderer, bg1Texture, NULL, &bgRect2);
            SDL_RenderCopy(renderer, bg1Texture, NULL, &bgRect3);
            SDL_RenderCopy(renderer, bgTexture, NULL, &bgRect0);
            SDL_RenderCopy(renderer, bgTexture, NULL, &bgRect1);
        }
};
class player {
    private:
        SDL_Surface *playerSurface = IMG_Load("assets/images/player.png");
        SDL_Texture *playerTexture;
        SDL_Rect playerRect;
        const int initPlayerHealth = 3;
        int posX,
            posY, 
            playerWidth = 60,
            playerHeight = 60,
            playerHealth = initPlayerHealth,
            playerSpeed = 5,
            level = 0,
            score = 0,
            progress = 0;
        bool flipSprite;
    public:
        void setTextures(SDL_Renderer *renderer) {
            playerTexture = SDL_CreateTextureFromSurface(renderer, playerSurface);
        }
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
            if(flipSprite) {
                SDL_RenderCopyEx(renderer, playerTexture, NULL, &playerRect, 0, 0, SDL_FLIP_HORIZONTAL);
            }else {
                SDL_RenderCopy(renderer, playerTexture, NULL, &playerRect);
            }
        }
        int getLevel() {
            return(level);
        }
        void setLevel(int value) {
            level = value;
        }
        int getScore() {
            return(score);
        }
        void setScore(int newScore) {
            score = newScore;
        }
        int getProgress() {
            return(progress);
        }
        void setProgress(int newProgress) {
            progress = newProgress;
        }
        int getHealth() {
            return(playerHealth);
        }
        SDL_Rect getPlayerRect() {
            return(playerRect);
        }
        int hurt() {
            playerHealth -= 1;
            return(playerHealth);
        }
        int gameOver() {
            short waiting=0;
            while(waiting==0) {
                posX -= 10;
                if(posX <= -playerWidth) {
                    posX = -playerWidth;
                }
                const Uint8 *keyState = SDL_GetKeyboardState(NULL);
                if(keyState[RESTART_KEY]) {
                    playerHealth = initPlayerHealth;
                    initPlayer();
                    score = 0;
                    
                    waiting=1;
                }
                SDL_Delay(30);

                SDL_Event e;
                while( SDL_PollEvent( &e ) != 0 ) {
                    // checks for exit button pressed
                    if( e.type == SDL_QUIT )
                    {
                        waiting=2;
                    }
                }
            }
            return(waiting);
        }
};
class cube {
    private:
        SDL_Surface *cubeSurface = IMG_Load("assets/images/storageCube/storageCube.png");
        SDL_Texture *cubeTexture;
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
            animCount = rand() % 19;
    public:
        void setTextures(SDL_Renderer *renderer) {
            cubeTexture = SDL_CreateTextureFromSurface(renderer, cubeSurface);
        }
        void initCube() {
            cubeRect.w = width;
            cubeRect.h = height;
            posX = -64;
            cubeClip.y = 0;
            cubeClip.w = 32;
            cubeClip.h = 32;
        }
        void scrollCube() {
            now++;
            if(now >= wait) {
                posX += currentSpeed;
            }
            if(posX <= -width) {
                posX = WIDTH + 10;
                posY = rand() %HEIGHT;
                wait = rand() %250 + 10;
                now = 0;
            }
        }
        void drawCube(SDL_Renderer *renderer) {
            cubeRect.x = posX;
            cubeRect.y = posY;
            //cubeClip.x = animCount*32;
            SDL_RenderCopyEx(renderer, cubeTexture, NULL, &cubeRect, animCount*20, NULL, SDL_FLIP_NONE);
            animCount++;
            if(animCount >= 19) {
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
        void setTextures(SDL_Renderer *mainRenderer) {
            cube0.setTextures(mainRenderer);
            cube1.setTextures(mainRenderer);
            cube2.setTextures(mainRenderer);
            cube3.setTextures(mainRenderer);
            cube4.setTextures(mainRenderer);
        }
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
            short checkAll = 0;
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
        Mix_Music *bgMusic;

        Mix_Chunk *hurtChunk;
        Mix_Chunk *gameOverChunk;
    public:
        void initMusic() {
            Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024);

            bgMusic = Mix_LoadMUS("assets/music/bg.wav");

            hurtChunk = Mix_LoadWAV("assets/sounds/hit.wav");
            gameOverChunk = Mix_LoadWAV("assets/sounds/gameOver.wav");
        }
        void playBG() {
            Mix_PlayMusic(bgMusic, -1);
        }
        void stopBG() {
            Mix_HaltMusic();
        }
        void playHurt() {
            Mix_PlayChannel(0, hurtChunk, 0);
        }
        void playGameOver() {
            
            Mix_PlayChannel(0, gameOverChunk, 0);
        }
        void freeAudio() {
            Mix_FreeMusic(bgMusic);
            Mix_FreeChunk(hurtChunk);
            Mix_FreeChunk(gameOverChunk);
        }
};
class text {
    private:
        TTF_Font    *comicMono = TTF_OpenFont("assets/fonts/upHeav.ttf", 16),
                    *comicGameOver = TTF_OpenFont("assets/fonts/upHeav.ttf", 64);
        SDL_Color   White = {255, 255, 255};
        //Text contents
        SDL_Surface //Updated only once
                    *objective = TTF_RenderText_Blended(comicMono, "Objective: ", White),
                    *health = TTF_RenderText_Blended(comicMono, "Health: ", White),
                    *score = TTF_RenderText_Blended(comicMono, "Score: ", White),
                    *progress = TTF_RenderText_Blended(comicMono, "Progress:", White),
                    *level = TTF_RenderText_Blended(comicMono, "Level: ", White),
                    *gameOver = TTF_RenderText_Blended(comicGameOver, "Game Over!", White),
                    *gameOverSubtext = TTF_RenderText_Blended(comicMono, "Press T to Restart", White),
                    //Updated many times
                    *objectiveValue = TTF_RenderText_Blended(comicMono, "Objective Placeholder", White),
                    *healthValue = TTF_RenderText_Blended(comicMono, "Health Placeholder", White),
                    *scoreValue = TTF_RenderText_Blended(comicMono, "0", White),
                    *levelValue = TTF_RenderText_Blended(comicMono, "Level Placeholder", White);

        SDL_Texture //Updated only once
                    *objectiveTexture, 
                    *healthTexture, 
                    *scoreTexture, 
                    *progressTexture, 
                    *levelTexture, 
                    *gameOverTexture, 
                    *gameOverSubtextTexture,
                    //Updated many times
                    *objectiveValueTexture,
                    *healthValueTexture,
                    *scoreValueTexture,
                    *levelValueTexture;

        SDL_Rect    //Updated only once
                    objectiveRect,
                    healthRect,
                    scoreRect,
                    progressRect,
                    levelRect,
                    gameOverRect,
                    gameOverSubtextRect,
                    //Updated many times
                    objectiveValueRect,
                    healthValueRect,
                    scoreValueRect,
                    levelValueRect;
    public:
        void initText(SDL_Renderer *renderer) {
            // =========================== START OF RECT SETTINGS ===========================
            // FIXED
            // objective rect
            objectiveRect.x = 5;
            objectiveRect.y = 5;
            objectiveRect.w = objective->w;
            objectiveRect.h = objective->h;
            // health rect
            healthRect.x = 5;
            healthRect.y = 25;
            healthRect.w = health->w;
            healthRect.h = health->h;
            /// score rect
            scoreRect.x = 5;
            scoreRect.y = 45;
            scoreRect.w = score->w;
            scoreRect.h = score->h;
            // progress rect
            progressRect.x = WIDTH-220;
            progressRect.y = 5;
            progressRect.w = progress->w;
            progressRect.h = progress->h;
            // level rect
            levelRect.x = WIDTH-220;
            levelRect.y = 45;
            levelRect.w = level->w;
            levelRect.h = level->h;
            // gameover rect
            gameOverRect.x = WIDTH/2-gameOver->w/2;
            gameOverRect.y = HEIGHT/2-gameOver->h;
            gameOverRect.w = gameOver->w;
            gameOverRect.h = gameOver->h;
            // gameover subtext rect
            gameOverSubtextRect.x = WIDTH/2-gameOverSubtext->w/2;
            gameOverSubtextRect.y = HEIGHT/2+10;
            gameOverSubtextRect.w = gameOverSubtext->w;
            gameOverSubtextRect.h = gameOverSubtext->h;

            // VALUES
            // objective value rect
            objectiveValueRect.x = objectiveRect.x + objectiveRect.w;
            objectiveValueRect.y = objectiveRect.y;
            objectiveValueRect.h = objectiveRect.h;
            // health value rect
            healthValueRect.x = healthRect.x + healthRect.w;
            healthValueRect.y = healthRect.y;
            healthValueRect.h = healthRect.h;
            // score value rect
            scoreValueRect.x = scoreRect.x + scoreRect.w;
            scoreValueRect.y = scoreRect.y;
            scoreValueRect.h = scoreRect.h;
            // level value rect
            levelValueRect.x = levelRect.x + levelRect.w;
            levelValueRect.y = levelRect.y;
            levelValueRect.h = levelRect.h;
            // ============================ END OF RECT SETTINGS ============================
            // FIXED
            scoreTexture = SDL_CreateTextureFromSurface(renderer, score);
            objectiveTexture = SDL_CreateTextureFromSurface(renderer, objective);
            healthTexture = SDL_CreateTextureFromSurface(renderer, health);
            progressTexture = SDL_CreateTextureFromSurface(renderer, progress);
            levelTexture = SDL_CreateTextureFromSurface(renderer, level);
            gameOverTexture = SDL_CreateTextureFromSurface(renderer, gameOver);
            gameOverSubtextTexture = SDL_CreateTextureFromSurface(renderer, gameOverSubtext);
            // VALUES
            objectiveValueTexture = SDL_CreateTextureFromSurface(renderer, objectiveValue);
            healthValueTexture = SDL_CreateTextureFromSurface(renderer, healthValue);
            scoreValueTexture = SDL_CreateTextureFromSurface(renderer, scoreValue);
            levelValueTexture = SDL_CreateTextureFromSurface(renderer, levelValue);
        }
        void updateObjectiveValue(const char *objectiveName, SDL_Renderer *renderer) {
            objectiveValue = TTF_RenderText_Blended(comicMono, objectiveName, White);
            objectiveValueTexture = SDL_CreateTextureFromSurface(renderer, objectiveValue);
            objectiveValueRect.w = objectiveValue->w;
        }
        void updateHealthValue(int newHealth, SDL_Renderer *renderer) {
            healthValue = TTF_RenderText_Blended(comicMono, to_string(newHealth).c_str(), White);
            healthValueTexture = SDL_CreateTextureFromSurface(renderer, healthValue);
            healthValueRect.w = healthValue->w;
        }
        void updateScoreValue(int scoreArg, SDL_Renderer *renderer) {
            scoreValue = TTF_RenderText_Blended(comicMono, to_string(scoreArg).c_str(), White);
            scoreValueTexture = SDL_CreateTextureFromSurface(renderer, scoreValue);
            scoreValueRect.w = scoreValue->w;
        }
        void updateLevelValue(int levelInt, SDL_Renderer *renderer) {
            levelValue = TTF_RenderText_Blended(comicMono, to_string(levelInt).c_str(), White);
            levelValueTexture = SDL_CreateTextureFromSurface(renderer, levelValue);
            levelValueRect.w = levelValue->w;
        }
        void drawText(SDL_Renderer *renderer) {
            // FIXED
            SDL_RenderCopy(renderer, objectiveTexture, NULL, &objectiveRect);
            SDL_RenderCopy(renderer, healthTexture, NULL, &healthRect);
            SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);
            SDL_RenderCopy(renderer, progressTexture, NULL, &progressRect);
            SDL_RenderCopy(renderer, levelTexture, NULL, &levelRect);
            // VALUES
            SDL_RenderCopy(renderer, objectiveValueTexture, NULL, &objectiveValueRect);
            SDL_RenderCopy(renderer, healthValueTexture, NULL, &healthValueRect);
            SDL_RenderCopy(renderer, scoreValueTexture, NULL, &scoreValueRect);
            SDL_RenderCopy(renderer, levelValueTexture, NULL, &levelValueRect);
        }
        void gameOverText(SDL_Renderer *renderer) {
            SDL_RenderCopy(renderer, gameOverTexture, NULL, &gameOverRect);
            SDL_RenderCopy(renderer, gameOverSubtextTexture, NULL, &gameOverSubtextRect);
        }
};
class progressBar {
    private:
        const int fullBar = 50, scale=200/fullBar;
        const SDL_Rect barBase = {WIDTH-220, 20, fullBar*scale, 16};
        SDL_Rect currentProgress = {WIDTH-220, 20, 0, 16};
    public:
        int updateProgressBar(int newLevel) {
            currentProgress.w = scale*newLevel;
            if(currentProgress.w >= fullBar*scale) {
                return(1);
            }
            return(0);
        }
        void drawProgressBar(SDL_Renderer *renderer) {
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &barBase);
            SDL_SetRenderDrawColor(renderer, 139, 155, 180, 255);
            SDL_RenderFillRect(renderer, &currentProgress);
            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        }
};
class npc {
    private:
    SDL_Surface *npcSurface = IMG_Load("assets/images/npc.png");
    SDL_Texture *npcTexture;
    SDL_Rect npcRect;
        int posX,
            posY,
            dirX,
            dirY,
            lastDirX,
            lastDirY,
            npcCount = 0,
            npcWait = 0;
        const int npcPauseTime = 40,
                npcSpeed = 5,
                npcMoveDistance = 30,
                width = 60,
                height = 60;
    public:
    void setTextures(SDL_Renderer *renderer) {
        npcTexture = SDL_CreateTextureFromSurface(renderer, npcSurface);
    }
    void initNPC() {
        npcRect.w = width;
        npcRect.h = height;
        posX = 320;
        posY = 240;
    }
    void moveNPC() {
        if(npcCount == 0) {
            dirX = rand() % 3 - 1;
            dirY = rand() % 3 - 1;
        }
        if(dirX != 0) {
            lastDirX = dirX;
        }
        if(dirY != 0) {
            lastDirY = dirY;
        }
        npcCount++;
        if(npcCount >= npcMoveDistance) {
            npcCount = 0;
            npcWait++;
        }
        if(npcWait != 0) {
            npcWait += 1;
        }
        if(npcWait >= npcPauseTime) {
            npcWait = 0;
        }

        if(posX > WIDTH - width) {
            posX = WIDTH - width;
        }
        if(posY > HEIGHT - height) {
            posY = HEIGHT - height;
        }
        if(posX < 0) {
            posX = 0;
        }
        if(posY < 0) {
            posY = 0;
        }
        if(npcWait == 0) {
            posX += dirX * npcSpeed;
            posY += dirY * npcSpeed;
        }
        npcRect.x = posX;
        npcRect.y = posY;
    }
    void drawNPC(SDL_Renderer *renderer) {
        if(lastDirX == 1) {
            SDL_RenderCopyEx(renderer, npcTexture, NULL, &npcRect, 0, NULL, SDL_FLIP_HORIZONTAL);
        }else {
            SDL_RenderCopy(renderer, npcTexture, NULL, &npcRect);
        }
    }
    
};