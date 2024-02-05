#include <iostream>
#include "constants.hpp"
#include "global.hpp"
using namespace std;

bool getCurrentWASD(bool flipSprite) {
    const Uint8 *keyState = SDL_GetKeyboardState(NULL);
    //sets the current directions based on the input constants in constants.hpp
    currentDirection.first = keyState[RIGHT_KEY] - keyState[LEFT_KEY];
    currentDirection.second = keyState[DOWN_KEY] - keyState[UP_KEY];
    //sets the last direction if the current direction is nonzero
    if(currentDirection.first != 0) {
        lastDirection.first = currentDirection.first;
        flipSprite = (lastDirection.first+1)/2;
    }
    if(currentDirection.second != 0) {
        lastDirection.second = currentDirection.second;
    }

    return(flipSprite);
}