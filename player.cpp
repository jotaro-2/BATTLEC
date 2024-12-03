#include <vector>
#include "raylib.h"
#include "Gameplay.h"
#include "globals.h"

// Function to place the playerShips on the grid
void PlacePlayerShips(Vector2 position) {
    // Iterate through each ship in the player's fleet
  for (const Ship &ship : playerShips) {
    // continue if the ship is already placed
    if (ship.placed) {
        
      for (int i = 0; i < ship.length; i++) {
        int x = ship.horizontal ? ship.startX + i : ship.startX;
        int y = ship.horizontal ? ship.startY : ship.startY + i;
        // Define the rectangle for the ship's position on the grid
        Rectangle cell = {position.x + x * CELL_SIZE,position.y + y * CELL_SIZE, CELL_SIZE, CELL_SIZE};
        DrawRectangleRec(cell, ship.color);
        // Draw the initials of the ship in the cell
        char label[3] = {ship.name[0] , ship.name[1], '\0'};  // Get the first two letters of the ship name
        Vector2 textPosition = {cell.x + CELL_SIZE / 4, cell.y + CELL_SIZE / 4};  // Adjust position inside the cell
        DrawText(label, textPosition.x, textPosition.y, 20, WHITE);  // Wrtie the initials on the ship
      }
    }
  }
}

// Function to handle the player's shooting action
bool PlayerShoot(Vector2 gridPosition, Vector2 mousePosition) {
    if (gameWon || gameLost || playerbomb <= 0) return false;  // Do nothing if the game is already won or if the player has no bombs left

    // Ensure all player ships are placed
    for (const Ship& ship : playerShips) {
        if (!ship.placed) {
            return false; // Early return if not all ships are placed
        }
    }

    int col = (mousePosition.x - gridPosition.x) / CELL_SIZE;
    int row = (mousePosition.y - gridPosition.y) / CELL_SIZE;

    // Ensure click is within grid bounds
    if (col >= 0 && col < GRID_SIZE && row >= 0 && row < GRID_SIZE) {
        // Variable to track if a hit occurred
        bool hit = false;

        // Check each opponent ship to see if the shot hits
        for (Ship& ship : opponentShips) {
            if (ship.placed) {
                for (int i = 0; i < ship.length; i++) {
                    int x = ship.horizontal ? ship.startX + i : ship.startX;
                    int y = ship.horizontal ? ship.startY : ship.startY + i;

                    if (x == col && y == row) {
                        opponentShots[row][col] = 'H';  // Mark as hit
                        ship.hits++;
                        shotsHitCount++;  // Increment shots hit count
                        hit = true;

                        // Check if the ship is fully destroyed
                        if (ship.hits == ship.length) {
                            opponentShipsDestroyedCount++;  // Increment destroyed ship count

                            // Check if all ships are destroyed
                            if (opponentShipsDestroyedCount == 5) {
                                gameWon = true;
                            }
                        }
                        break;
                    }
                }
            }
            if (hit) break;  // Exit loop early if a hit is confirmed
        }
        if (!hit) {
            opponentShots[row][col] = 'M';  // Mark as miss
            shotsMissedCount++;  // Increment shots missed count
        }
        playerbomb--;  // Reduce the number of bombs after each shot
        
        // Check if the player has run out of bombs in HARD mode
        if (playerbomb <= 0) {
            gameLost = true;  // End the game if the player has no bombs left
        }
        return hit;  // Return true if it was a hit, false otherwise
    }
    return false;  // Return false if shot was out of bounds
}

// Draw shots on the player's grid
void DrawPlayerShots(Vector2 position) {
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            Rectangle cell = {position.x + col * CELL_SIZE, position.y + row * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            // Check for hits or misses
            if (playerShots[row][col] == 'H') {
                DrawRectangleRec(cell, BLACK);  // Black for hit
                DrawText("H", cell.x + CELL_SIZE / 3, cell.y + CELL_SIZE / 4, 20, RED);
            } else if (playerShots[row][col] == 'M') {
                DrawRectangleRec(cell, DARKBLUE); // Dark blue for miss
                DrawText("M", cell.x + CELL_SIZE / 3, cell.y + CELL_SIZE / 4, 20, WHITE);
            }
        }
    }
}

//Function to preview and place the player ships on the grid
void HandleShipPlacementAndPreview(Vector2 mousePosition) {
    // Ensure the currentShipIndex is valid
    if (currentShipIndex >= playerShips.size()) return;

    // Get the current ship from the playerShips vector
    Ship& currentShip = playerShips[currentShipIndex]; // Get the current ship

    // Calculate the grid position from mouse position
    int col = (mousePosition.x - grid1Position.x) / CELL_SIZE;
    int row = (mousePosition.y - grid1Position.y) / CELL_SIZE;

    // Ensure the grid position is valid
    if (col >= 0 && col < GRID_SIZE && row >= 0 && row < GRID_SIZE) {
        Vector2 gridPosition = {(col), (row)};

        // Draw the ship preview if the ship has not been placed yet
        if (!currentShip.placed) {
            DrawShipPreview(currentShip, gridPosition); // Draw preview
        }
        // Handle ship placement when the left mouse button is clicked
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !currentShip.placed) {
            if (CanPlaceShip(currentShip, gridPosition)) {currentShip.startX = gridPosition.x; currentShip.startY = gridPosition.y; currentShip.placed = true;}
        }
        // Rotate ship with the right mouse button
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            currentShip.horizontal = !currentShip.horizontal;
        }
    }
    // Move to the next ship if the current one is placed
    if (currentShipIndex < (playerShips.size()) && currentShip.placed) {
        currentShipIndex++; // Increment to next ship index
    }
}

// Function to handle the player's turn
void HandlePlayerTurn(Vector2 mousePosition){
    //continue if the game is not won or not lost, and it is the player's turn
    if (!gameWon && !gameLost && playerTurn) {
        //check for the mouse click
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            // Player shoots at opponent's grid
            if (mousePosition.x >= GRID_OFFSET_X2 && mousePosition.x < GRID_OFFSET_X2 + GRID_SIZE * CELL_SIZE && mousePosition.y >= GRID_OFFSET_Y && mousePosition.y < GRID_OFFSET_Y + GRID_SIZE * CELL_SIZE) {
                bool hit = PlayerShoot(grid2Position, mousePosition);  // Check if the shot was a hit
                
                if (hit) {
                    //if hit continue shooting
                } else {
                    // If it the player missed, switch turns
                    playerTurn = false;
                }
            }
        }
    }
}