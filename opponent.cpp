
#include <vector>
#include <cstdlib>
#include "raylib.h"
#include "Gameplay.h"
#include "globals.h"

using namespace std;

//Function to determine opponent shooting mechanism
bool OpponentShoot() {
    // Return early if no valid cells to shoot, or if game is over
    if (unshotCells.empty() || gameWon || gameLost) return false;

    int row, col;
    if (!targetCells.empty()) {
        // Targeting mode: Continue targeting based on hit cells
        row = targetCells.front().first;
        col = targetCells.front().second;
        targetCells.erase(targetCells.begin());
    } else {
        // Random mode: Choose a random unshot cell
        int index = rand() % unshotCells.size();
        row = unshotCells[index].first;
        col = unshotCells[index].second;
        unshotCells.erase(unshotCells.begin() + index);
    }

    bool hit = false;

    for (Ship& ship : playerShips) {
        if (ship.placed) {
            // Check if the current shot hits this ship
            for (int i = 0; i < ship.length; ++i) {
                int x = ship.horizontal ? ship.startX + i : ship.startX;
                int y = ship.horizontal ? ship.startY : ship.startY + i;

                if (x == col && y == row) {
                    playerShots[row][col] = 'H'; // Mark as a hit
                    ship.hits++;
                    hit = true;

                    if (ship.hits == 1) {
                        // Add all adjacent cells for the first hit
                        AddAdjacentCells(row, col);
                    } else if (ship.hits > 1) {
                        // Refine targeting after confirming the ship's orientation
                        DetermineRotation(row, col, ship);
                    }

                    // If the ship is destroyed, reset targeting for this ship
                    if (ship.hits == ship.length) {
                        playerShipsDestroyedCount++;
                        targetCells.clear();

                        // End game if all player ships are destroyed
                        if (playerShipsDestroyedCount == playerShips.size()) {
                            gameLost = true;
                        }
                    }
                    break;
                }
            }
        }
        if (hit) break;
    }
    if (!hit) {
        playerShots[row][col] = 'M'; // Mark as a miss
    }
    return hit;
}

//Add adjacent cells to targetCells
void AddAdjacentCells(int row, int col) {
    //This code checks and adds the cell above, below, right and left
    if (row > 0 && playerShots[row - 1][col] == ' ') 
        targetCells.push_back({row - 1, col});
    if (row < GRID_SIZE - 1 && playerShots[row + 1][col] == ' ') 
        targetCells.push_back({row + 1, col});
    if (col > 0 && playerShots[row][col - 1] == ' ') 
        targetCells.push_back({row, col - 1});
    if (col < GRID_SIZE - 1 && playerShots[row][col + 1] == ' ') 
        targetCells.push_back({row, col + 1});
}

//Determine orientation and prioritize targeting in-line cells
void DetermineRotation(int row, int col, const Ship& ship) {
    // Clear any previous targets and focus only on cells aligned with the ship
    targetCells.clear();

    if (ship.horizontal) {
        // If the ship is horizontal, add cells to the left and right
        for (int i = 1; i < ship.length; ++i) {
            if (col - i >= 0 && playerShots[row][col - i] == ' ') 
                targetCells.push_back({row, col - i});
            if (col + i < GRID_SIZE && playerShots[row][col + i] == ' ') 
                targetCells.push_back({row, col + i});
        }
    } else {
        // If the ship is vertical, add cells to the below and above
        for (int i = 1; i < ship.length; ++i) {
            if (row - i >= 0 && playerShots[row - i][col] == ' ') 
                targetCells.push_back({row - i, col});
            if (row + i < GRID_SIZE && playerShots[row + i][col] == ' ') 
                targetCells.push_back({row + i, col});
        }
    }
}
  
// Draw shots on the opponent's grid
void DrawOpponentShots(Vector2 position) {
    //Loop through each row and column
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            Rectangle cell = {position.x + col * CELL_SIZE, position.y + row * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            // Check for hits or misses
            if (opponentShots[row][col] == 'H') {
                //If it is hit, make the cell color black
                DrawRectangleRec(cell, BLACK);
                DrawText("H", cell.x + CELL_SIZE / 3, cell.y + CELL_SIZE / 4, 20, RED);
                //If it is miss, make the cell color blue
            } else if (opponentShots[row][col] == 'M') {
                DrawRectangleRec(cell, DARKBLUE);
                DrawText("M", cell.x + CELL_SIZE / 3, cell.y + CELL_SIZE / 4, 20, WHITE);
            }
        }
    }
}

// Function to place opponent ships on the grid
void PlaceOpponentShips() {
    srand((time(0)));  // Random seed for ship placement

    // Initialize a grid to mark occupied cells
    bool opponentGrid[GRID_SIZE][GRID_SIZE] = {false};
    // Skip if the ship is already placed
    for (Ship& ship : opponentShips) {
        if (ship.placed) continue; 
        
        bool placed = false;
        while (!placed) {
            // Randomly choose if the ship will be horizontal or vertical and the position of the ship inside the grid
            ship.horizontal = rand() % 2 == 0;

            if (ship.horizontal) {
                ship.startX = rand() % (GRID_SIZE - ship.length + 1); //for making sure that the ships are placed within the grids
                ship.startY = rand() % GRID_SIZE;
            } else {
                ship.startX = rand() % GRID_SIZE;   //for making sure that the ships are placed within the grids
                ship.startY = rand() % (GRID_SIZE - ship.length + 1);
            }


            // Check if all cells for this ship are free
            placed = true;
            for (int i = 0; i < ship.length; i++) {
                int x = ship.horizontal ? ship.startX + i : ship.startX;
                int y = ship.horizontal ? ship.startY : ship.startY + i;

                if (opponentGrid[x][y]) {
                    placed = false;
                    break;  // Overlap found, reattempt placement
                }
            }

            // If placement is successful, mark the grid cells as occupied
            if (placed) {
                for (int i = 0; i < ship.length; i++) {
                    int x = ship.horizontal ? ship.startX + i : ship.startX;
                    int y = ship.horizontal ? ship.startY : ship.startY + i;
                    opponentGrid[x][y] = true;
                }
                ship.placed = true; // Mark ship as placed
            }
        }
    }
}

// Handle opponent turn
void HandleOpponentTurn(){
    if (!AreAllShipsPlaced()) return;
      // Check if the opponent's shot was a hit and if the game ended
    if (!gameWon && !gameLost && !playerTurn) {
        bool hit = OpponentShoot();
        if (hit) { 
            //if hit continue shooting
        } else {
            playerTurn = true;  // Switch back to player's turn
        }
    }
}


// Function to draw the opponent's ships on the grid after the game ends
void ShowOpponentShips(Vector2 position) {
    if (!gameWon && !gameLost) return;
    // Iterate through each ship in the opponent's fleet
    for (const Ship &ship : opponentShips) {
        // Skip ships that are not yet placed
        if (!ship.placed) continue;

        // Draw each part of the ship
        for (int i = 0; i < ship.length; i++) {
            int x = ship.horizontal ? ship.startX + i : ship.startX;
            int y = ship.horizontal ? ship.startY : ship.startY + i;

            // Define the rectangle for the ship's position on the grid
            Rectangle cell = {position.x + x * CELL_SIZE, position.y + y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            DrawRectangleRec(cell, ship.color);
        }
    }
}
