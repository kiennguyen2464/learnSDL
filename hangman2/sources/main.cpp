#include <iostream>
#include <SDL.h>
#include "game.h"

int main(int argc, char* argv[])
{
    auto* __game = new game;
    
    __game->preset();

    __game->play();
    
}