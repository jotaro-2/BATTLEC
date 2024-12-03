#include "raylib.h"
#include "Gameplay.h"
#include "globals.h"

// Highlight the cell under the mouse pointer (only for the opponent grid)
void HighlightCell(Vector2 gridPosition, Vector2 mousePosition) {
    int col = (mousePosition.x - gridPosition.x) / CELL_SIZE;
    int row = (mousePosition.y - gridPosition.y) / CELL_SIZE;

    if (col >= 0 && col < GRID_SIZE && row >= 0 && row < GRID_SIZE) {
        Rectangle highlightRect = {gridPosition.x + col * CELL_SIZE,gridPosition.y + row * CELL_SIZE,CELL_SIZE,CELL_SIZE};
        DrawRectangleRec(highlightRect, Color{0, 0, 255, 100});
    }
}

// OpponentGrid function to display ships and grid
void OpponentGrid(Vector2 position) {
    // Draw column and row labels (a,b,c,etc.)
    for (int col = 0; col < GRID_SIZE; col++) {
        char label[2] = {('A' + col), '\0'};
        Vector2 textPosition = {position.x + col * CELL_SIZE + CELL_SIZE / 3, position.y - 20};
        DrawText(label, textPosition.x, textPosition.y, 20, BLACK);
    }
    // Draw row labels (numbers)
    for (int row = 0; row < GRID_SIZE; row++) {
        char label[2];
        sprintf(label, "%d", row);
        Vector2 textPosition = {position.x - 20, position.y + row * CELL_SIZE + CELL_SIZE / 3};
        DrawText(label, textPosition.x, textPosition.y, 20, BLACK);
    }

    // Draw the grid cells
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            Rectangle cell = {position.x + col * CELL_SIZE, position.y + row * CELL_SIZE, CELL_SIZE, CELL_SIZE};
            DrawRectangleLinesEx(cell, 0.75, BLACK);
        }
    }
}

//Function for creating the grid for the player
void PlayerGrid(Vector2 position) {
    // Draw the column labels (A-J)
    for (int col = 0; col < GRID_SIZE; col++) {
        char label[2] = {static_cast<char>('A' + col), '\0'}; // Convert the char to a string (vscode suggested adding static_cast) 
        Vector2 textPosition = {position.x + col * CELL_SIZE + CELL_SIZE / 3, position.y - 20}; // Adjust position above the grid
        DrawText(label, textPosition.x, textPosition.y, 20, BLACK);
    }
    // Draw the row labels (0-9)
    for (int row = 0; row < GRID_SIZE; row++) {
        char label[2];
        sprintf(label, "%d", row);  // Convert row number to string
        Vector2 textPosition = {position.x - 20, position.y + row * CELL_SIZE + CELL_SIZE / 3}; // Adjust position to the left of the grid
        DrawText(label, textPosition.x, textPosition.y, 20, BLACK);
    }
    // Draw the grid cells
    for (int row = 0; row < GRID_SIZE; row++) {
        for (int col = 0; col < GRID_SIZE; col++) {
            // Define the size and position of each cell
            Rectangle cell = {position.x + col * CELL_SIZE,position.y + row * CELL_SIZE,CELL_SIZE,CELL_SIZE};
            // Draw the cell outlines
            DrawRectangleLinesEx(cell, 0.75, BLACK);  
        }
    }
}

// Display information for the player
void DrawGameText(){
    DrawText("Left Click: Place the Ship, Right Click: Rotate the Ship", 20, 770, 20, BLACK);
    DrawText("You can reset the game by click on the 'Main Menu' button", 20, 750, 20, BLACK);

    DrawText("Opponent Grid", 675, 670, 20, BLACK);
    DrawText("Player Grid", 190, 670, 20, BLACK);
    DrawText("BattleC", 350, 100, 80, BLACK);
    
}

//Function to check if the all of the ships are placed on the grid (this function is for preventing player start shooting before placing the ships)
bool AreAllShipsPlaced() {
    for (const Ship& ship : playerShips) {
        if (!ship.placed) return false; // Return false if any ship is not placed
    }
    return true; // All ships are placed
}
 
// Function to check if the ship can be placed on the grid
bool CanPlaceShip(const Ship& ship, Vector2 gridPosition) {
    for (int i = 0; i < ship.length; i++) {
        int x = ship.horizontal ? gridPosition.x + i : gridPosition.x;
        int y = ship.horizontal ? gridPosition.y : gridPosition.y + i;

        // Check if the ship goes out of bounds
        if (x < 0 || x >= GRID_SIZE || y < 0 || y >= GRID_SIZE) {
            return false;
        }

        // Check if the space is occupied by another ship
        for (const Ship& placedShip : playerShips) {
            if (placedShip.placed) {
                for (int j = 0; j < placedShip.length; j++) {
                    int placedX = placedShip.horizontal ? placedShip.startX + j : placedShip.startX;
                    int placedY = placedShip.horizontal ? placedShip.startY : placedShip.startY + j;
                    if (x == placedX && y == placedY) {
                        return false; // Space is already occupied
                    }
                }
            }
        }
    }
    return true;
}

// Function to draw the preview of the ship
void DrawShipPreview(Ship& ship, Vector2 gridPosition) {
    Color previewColor = CanPlaceShip(ship, gridPosition) ? Color{ship.color.r, ship.color.g, ship.color.b, 100} : RED; // Red if can't be placed

    for (int i = 0; i < ship.length; i++) {
        int x = ship.horizontal ? gridPosition.x + i : gridPosition.x;
        int y = ship.horizontal ? gridPosition.y : gridPosition.y + i;

        // Only draw the ship if it's within the grid bounds
        if (x >= 0 && x < GRID_SIZE && y >= 0 && y < GRID_SIZE) {
            Rectangle cell = {(GRID_OFFSET_X1 + x * CELL_SIZE),(GRID_OFFSET_Y + y * CELL_SIZE),(CELL_SIZE),(CELL_SIZE)};

            DrawRectangleRec(cell, previewColor);  // Draw ship with color (red if invalid)
            DrawRectangleLinesEx(cell, 1, BLACK);  // Outline for the preview
        }
    }
}

// Function to initialize unshot cells
void InitializeUnshotCells() {
    // Loop through each row of the grid
    for (int row = 0; row < GRID_SIZE; ++row) {
        // Loop through each column of the grid for the current row
        for (int col = 0; col < GRID_SIZE; ++col) {
            //Add the unshot cells into list
            unshotCells.push_back({row, col});
        }
    }
}
