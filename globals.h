#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "raylib.h"

using namespace std;

//Game difficulty levels and Game states
enum GameState { MAIN_MENU, GAMEPLAY };

extern GameState currentState;

// Ship structure
struct Ship {
    int length;
    const char* name;
    Color color;
    bool placed = false;
    bool horizontal = true;
    bool vertical = true;
    int startX = -1;
    int startY = -1;
    int hits = 0;
};

// Grid properties
extern const int GRID_SIZE;
extern const int CELL_SIZE;
extern const int GRID_OFFSET_X1;
extern const int GRID_OFFSET_Y;
extern const int GRID_OFFSET_X2;


// Declare global variables with 'extern' to avoid multiple definitions
extern bool playerTurn;
extern int currentShipIndex;


extern vector<Ship> playerShips;
extern vector<Ship> opponentShips;

extern Vector2 grid1Position;
extern Vector2 grid2Position;

extern int playerShipsDestroyedCount;
extern int opponentShipsDestroyedCount;
extern int shotsHitCount;
extern int shotsMissedCount;
extern bool gameWon;
extern bool gameLost;

//player and opponent bomb counts
extern int playerbomb;
extern int opponentbomb;

extern vector<vector<bool>> playerShotsTracking;


// Stores potential target cells
// Tracks the last hit's coordinates
extern vector<pair<int, int>> pendingTargets;  
extern pair<int, int> lastHit; 

extern bool isTargeting;                     
extern vector<pair<int, int>> targetCells;

extern vector<vector<char>> playerShots;
extern vector<vector<char>> opponentShots;
extern vector<pair<int, int>> unshotCells;

#endif
