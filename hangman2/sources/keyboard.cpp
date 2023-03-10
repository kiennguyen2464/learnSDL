// keyboard.cpp#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "keyboard.h"

const std::string KEYBOARD_ROWS[3] = {"QWERTYUIOP", "ASDFGHJKL", "ZXCVBNM"};
const int KEYBOARD_POSITION_X = 500;
const int KEYBOARD_POSITION_Y = 400;
const int KEYBOARD_CHARACTER_FONT_SIZE = 48;

bool charButton::isUsed()
{
    return __isUsed;
}

void charButton::useKey()
{
    __isUsed = 1;
}

void charButton::resetKey()
{
    __isUsed = 0;
}

void charButton::updateSymbol(char newSym)
{
    symbol = newSym;
}

void charButton::handleEvent(SDL_Event* event, const word& key, guessWord& curGuessWord, bool& isTriggered, bool& isIn)
{
    SDL_Point pos = getPos();
    int w = getWidth();
    int h = getHeight();

    
    if (event->type == SDL_MOUSEMOTION || event->type == SDL_MOUSEBUTTONUP || event->type == SDL_MOUSEBUTTONDOWN)
    {
        int x = 0, y = 0;
        SDL_GetMouseState(&x, &y);

        bool inside = 1;

        if (x < pos.x || x > pos.x + w || y < pos.y || y > pos.y + h) inside = 0;

        if (!isUsed() && inside && event->type == SDL_MOUSEBUTTONDOWN) 
        {
            trigger(key, curGuessWord, isIn);
            isTriggered = 1;
        }
        
    }
}

void charButton::trigger(const word& key, guessWord& curGuessWord, bool& isIn)
{
    useKey();
    for (int i = 0; i < key.getLength(); i ++)
        if (key.getWord()[i] == symbol)
        {
            curGuessWord.updateChar(i, symbol);
            isIn = 1;
        } 
    return;
}

keyboard::keyboard()
{

}

keyboard::~keyboard()
{
    clear();
}

bool keyboard::loadCharTexture(SDL_Renderer* renderer)
{
    TTF_Font* mFont = NULL;
    mFont = TTF_OpenFont(&PATH_COMIC_FONT[0], KEYBOARD_CHARACTER_FONT_SIZE);
	if (mFont == NULL)
	{
		cout << "Failed to load the font. Error: " << TTF_GetError() << ".\n";
		return 0;
	}
    string charList = KEYBOARD_ROWS[0] + KEYBOARD_ROWS[1] + KEYBOARD_ROWS[2] + "_ ";

    for (char curChar : charList)
    {
        string curChar_str = "";
        curChar_str.push_back(curChar);
        if ((!(charTexture[int(curChar)].loadTexture(renderer, mFont, &curChar_str[0], SDL_COLOR_BLACK))) || 
            (!usedCharTexture[int(curChar)].loadTexture(renderer, mFont, &curChar_str[0], SDL_COLOR_GRAY)))
        {
            cout << "Failed to render text.\n";
            return 0;
        }
    }

	TTF_CloseFont(mFont);

    return 1;
}

void keyboard::set()
{
    int charHeight = charTexture['A'].getHeight();
    int curXPos = KEYBOARD_POSITION_X;
    int curYpos = KEYBOARD_POSITION_Y;

    for (int curRow = 0; curRow < 3; curRow ++)
    {
        curXPos = KEYBOARD_POSITION_X;
        for (auto ch : KEYBOARD_ROWS[curRow])
        {
            int ch_int = ch;
            keyboardButton[ch_int].set(curXPos, curYpos, charTexture[ch_int].getWidth(), charTexture[ch_int].getHeight());
            keyboardButton[ch_int].resetKey();
            curXPos += charTexture[ch_int].getWidth() + 2 * charTexture[int(' ')].getWidth();

            keyboardButton[ch_int].updateSymbol(ch);
        }
        curYpos += charHeight;
    }
}

void keyboard::handleEvent(SDL_Event& event, const word& key, guessWord& curGuessWord, bool& isTriggered, bool& isIn)
{
    for (int buttonId = 'A'; buttonId <= 'Z'; buttonId ++)
        keyboardButton[buttonId].handleEvent(&event, key, curGuessWord, isTriggered, isIn);
}

void keyboard::render(SDL_Renderer* renderer)
{
    for (int curRow = 0; curRow < 3; curRow ++)
    {
        for (auto ch : KEYBOARD_ROWS[curRow])
        {
            int ch_int = ch;
            if (!keyboardButton[ch_int].isUsed()) keyboardButton[ch_int].render(renderer, charTexture[ch_int]);
            else keyboardButton[ch_int].render(renderer, usedCharTexture[ch_int]);
        }
    }
}

void keyboard::clear()
{
    for (int i = 0; i < 128; i ++) charTexture[i].clear(), usedCharTexture[i].clear();
}