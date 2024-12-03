#include "globals.h"

using namespace std;

// Turn taking variables
bool playerTurn = true;
int currentShipIndex = 0;

//Grid variables (dimensions)
const int GRID_SIZE = 10;
const int CELL_SIZE = 42;
const int GRID_OFFSET_Y = 240;
const int GRID_OFFSET_X2 = 550; 
const int GRID_OFFSET_X1 = 50;

// Ship definitions (size, color, etc.)
vector<Ship> playerShips = {
    {5, "Ca", BLUE},
    {4, "Ba", PINK},
    {3, "Cr", GREEN},
    {3, "Su", ORANGE},
    {2, "De", PURPLE}
};

vector<Ship> opponentShips = {
    {5, "Ca", BLUE},
    {4, "Ba", PINK},
    {3, "Cr", GREEN},
    {3, "Su", ORANGE},
    {2, "De", PURPLE}
};


GameState currentState = MAIN_MENU;

// Positions for drawing the grids on screen
Vector2 grid1Position = {GRID_OFFSET_X1, GRID_OFFSET_Y};
Vector2 grid2Position = {GRID_OFFSET_X2, GRID_OFFSET_Y};

// Stores potential target cells
// Tracks the last hit's coordinates
vector<pair<int, int>> pendingTargets;  
pair<int, int> lastHit;               


// Counters to track the progress of the game
int playerShipsDestroyedCount = 0;
int opponentShipsDestroyedCount = 0;
int shotsHitCount = 0;
int shotsMissedCount = 0;
bool gameWon = false;
bool gameLost = false;

//player and opponent bomb counts
int playerbomb = 100;
int opponentbomb = 100;

vector<vector<bool>> playerShotsTracking(GRID_SIZE, vector<bool>(GRID_SIZE, false));

bool isTargeting = false;                           // Flag for targeting mode
vector<pair<int, int>> targetCells;           // Cells to target during targeting mode


// Arrays to track shots fired by the player and opponent / the list for the tracking the unshot cells
vector<vector<char>> playerShots(GRID_SIZE, vector<char>(GRID_SIZE, ' '));
vector<vector<char>> opponentShots(GRID_SIZE, vector<char>(GRID_SIZE, ' '));
vector<pair<int, int>> unshotCells;


