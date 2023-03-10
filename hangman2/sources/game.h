//game.h
#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "playgamePage.h"

class game
{
    private:
        /// @brief The window
        SDL_Window* mWindow;

        /// @brief The renderer
        SDL_Renderer* mRenderer;
        
        /// @brief Class for the starting page of the game
        startPage mStartPage;

        /// @brief Class for the choosing difficulty page of the game
        chooseDifficultyPage mChooseDifficultyPage;

        /// @brief Class for the main page of the game
        playgamePage mPlaygamePage;
    
    public: 
        /// @brief Constructor
        game();

        /// @brief Destructor
        ~game();

        /// @brief Initialize SDL window, renderer, image
        /// @return 1 if successful, 0 otherwise
        bool initSDL();

        /// @brief Settings before playing the game
        void preset();

        /// @brief The main gameplay
        void play();

        /// @brief Cleanup
        void clear();
};