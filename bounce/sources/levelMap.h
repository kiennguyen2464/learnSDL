// levelMap.h

/**
 * @brief This file controls the map of the game and how it is rendered
 * Logic:
 * - Horizontally, the map will move along with the ball. While playing, the ball will "stand still"
 * at some position during the game. The balls position is fixed at 7 brick tiles (560px) from the 
 * leftmost position of the game area.
 * - The map shifts up 7 brick tiles when the ball moves vertically out.
 * - The collide checking function will check colliding objects based on the position of the render map
 */

#ifndef LEVELMAP_GUARD
#define LEVELMAP_GUARD

#include <iostream>
#include <vector>
#include <sdl.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>
#include "base.h"
#include "ball.h"
#include "brickTile.h"
#include "levelMap.h"
#include "checkpoint.h"
#include "enemy.h"
#include "portal.h"
#include "finishLine.h"

/**
 * @brief 
 * - B: bricks
 * - R: vertical large portals, together with "+"
 * - r: vertical small portals
 * - E: horizontal small portals, together with "+"
 * - e: horizontal small portals
 * - T: spikes
 * - C: checkpoints
 * - L: extra lives
 * - P: pumps
 * - D: shrinkers
 * - G: Finish line, together with "#"
 * 
 */
const std::vector <std::string> LEVEL_CHAR_MAP[3] = 
{
    {""},

    {
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB          ",
        "     B    BB   BBBBBB          BBB         BB         BBB                  BBB      BB  L   BB               BB         BB          ",
        "    BB    BBc  BBBBBB         tBBB         +          BB                    BB      BB      BB               BB         BB          ",
        "   B B    BB   BBBBBB   BB     BBB         r          BB         C          BB  BB  BB  BB  BB  BB           BB         BB          ",
        "  B  B    BB   BBBBBB   BB     BBB  BBBB  BBBB  BBBB  BB                    BB  BB  BB  BB  BB  BB       BBe-BBe-BB     BB          ",
        "     B    BB       +    BB          BB     BB     BB                            BB  +   BB  +   BB    BBTBB  C   BBTBB  ##          ",
        "  BBBBBBB BB       r    BB T        BB  T     T   BB        T  T   T   T        BB  r   BB  r   BB    BBBBB      BBBBB  G#          ",
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB          ",
    },

    {
        "                                                                                                                        BBBBBBBBBBBBBBBBBBBBBBBB"
        "                                                                                                                        BBC                   BB"
        "                                                                                                                        BB          +         BB"
        "                                                                                                                        BB          R         BB"
        "                                                                                                                        BBBBBBBBBBBBBB        BB"
        "                                                                                                                        BB                    ##"
        "                                                                                                                        BB                    G#"
        "                                                                                                                        BB   BBBBBBBBBBBBBBBBBBB"
        "                                                                                                                        BB                    BB"
        "                                                                                                                        BB                    BB"
        "                                                                                                                        BBBB      BB  BB      BB"
        "                                                                                                                        BBC      BB    BB     BB"
        "                                                                                                                        BB      BB      BB    BB"
        "                                                                                                                        BB     BB   T    BB   BB"
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBE-BB"
        "  BBBBBBB BB                    +   BB      BB     +                        BB        C           BB         BB         +   BB         BB     BB"
        "  T     B BB       c            R   BB      BB     R                        BB                    BB         BB         r   BB         BB     BB"
        "        B BB             BBBBBBBBB  BB  BB         BBBBB                    BB        XZ          BB         +          BB  BB    BB   BB  BBBBB"
        "  BBBBBBB BB             BB     BB  BB  BB         BBBBB                    BB       XBBZ         BB         R          BB  BB  BBBB          BB"
        "  B     T BB             BB L           BB         BBBBB                    +       XBBBBZ        +          BB         BB      L BB          BB"
        "  BBBBBBB BB           T BB             BBP        BBBBB                    R      XBBBBBBZ       R          BB         BB        BB        T BB"
        "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBDDDDDDDDDBBBBBBBBBBBBBBBBBBBBBBBB"
    }
};

const int TILE_WIDTH = 80;
const int TILE_HEIGHT = 80;

const char BRICK_CHAR_SYMBOL = 'B';
const char CHECKPOINT_CHAR_SYMBOL = 'C';
const char CHECKPOINT_START_CHAR_SYMBOL = 'c';
const char SPIKE_CHAR_SYMBOL = 'T';
const char SPIKE_HORIZONTAL_CHAR_SYMBOL = 't';
const char PORTAL_VERTICAL_SMALL_CHAR_SYMBOL = 'r';
const char PORTAL_VERTICAL_LARGE_CHAR_SYMBOL = 'R';
const char PORTAL_HORIZONTAL_SMALL_CHAR_SYMBOL = 'e';
const char PORTAL_HORIZONTAL_LARGE_CHAR_SYMBOL = 'E';
const char FINISH_LINE_CHAR_SYMBOL = 'G';

const int DIRX[] = {1, -1, 0, 0};
const int DIRY[] = {0, 0, 1, -1};


class levelMap
{
private:
    /// @brief The encoded map of the level
    std::vector <std::string> charMap;

    /// @brief List of brick tiles
    std::vector <brickTile> vBrickTiles;

    /// @brief List of spikes
    std::vector <spike> vSpikes;

    /// @brief List of checkpoints
    std::vector <checkpoint> vCheckpoints;

    /// @brief List of portals
    std::vector <portal> vPortals;

    /// @brief The finish line
    finishLine mFinishLine;

    /// @brief The bottom left position of the current frame, in comparison with the full level map
    double curFramePosX, curFramePosY;

    /// @brief Check if a brick tile is free when it is not surrounded by other bricks
    bool isFreeBrickTile(const int& brickTileCharPosX, const int& brickTileCharPosY);

public:
    levelMap(/* args */);
    ~levelMap();

    /// @brief Set up the map by the level
    /// @param id The index of the current level
    void setMap(const int& id);

    /// @brief Move the map horizontally
    /// @param v Velocity
    void moveX(const double& dist);

    /// @brief Set the bottom left position of the current frame, in comparison with the full level map
    void setFramePos(const double& framePosX, const double& framePosY);

    /// @brief Get the list of brick tiles rendered on the map
    std::vector <brickTile> brickTilesList() const;

    /// @brief Get the list of spikes rendered on the map
    std::vector <spike> spikesList() const;

    /// @brief Get the list of checkpoints in the map
    std::vector <checkpoint> checkpointsList() const;

    /// @brief Get the list of portals in the map
    std::vector <portal> portalsList() const;

    finishLine getFinishLine() const;

    /// @brief Update the checkpoints list
    void updateCheckpointsList(const std::vector <checkpoint>& newCheckpointsList);

    /// @brief Update the portals list
    void updatePortalsList(const std::vector <portal>& newPortalsList);

    /// @brief Update the finish line
    void updateFinishLine(const finishLine& newFinishLine);

    int getFramePosX() const;

    void render(SDL_Renderer* renderer, LTexture& spritesheet);

};

#endif