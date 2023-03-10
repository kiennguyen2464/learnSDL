// enemy.cpp

#include "enemy.h"

spike::spike()
{
    
}

spike::spike(const int& x, const int& y)
{
    mPosX = x;
    mPosY = y;
    mWidth = SPIKE_WIDTH;
    mHeight = SPIKE_HEIGHT + 10;
}

spike::~spike()
{

}

void spike::setRotateAngle(const int& angle)
{
    rotateAngle = angle;
}

void spike::render(SDL_Renderer* renderer, LTexture& spritesheet)
{
    setSpriteClip(spritesheet, SPIKE_SPRITE_POS_x, SPIKE_SPRITE_POS_Y, SPIKE_WIDTH, SPIKE_HEIGHT);
    SDL_Point center = {mPosX + SPIKE_WIDTH / 2, mPosY - SPIKE_HEIGHT / 2};

    spritesheet.render(renderer, mPosX, mPosY - SPIKE_HEIGHT, &mSpriteClip, rotateAngle, NULL);

}