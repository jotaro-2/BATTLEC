#include "raylib.h"
#include "Gameplay.h"
#include "globals.h"
#include "gamescreensandbuttons.h"

int main() {
    // Initialize window
    InitWindow(1000, 800, "BattleC");
    SetTargetFPS(60);

    GameStats gameStats;
    MainMenu mainMenus;
    GameScreen gameUI;

    // Game loop
    while (!WindowShouldClose()) {
        switch (currentState) {
            case MAIN_MENU: {
                mainMenus.ShowMainMenu(); 
                // Transition to gameplay screen
                InitializeUnshotCells();
                currentState = GAMEPLAY;
                break;
            }
            case GAMEPLAY: {
                BeginDrawing();
                ClearBackground(LIGHTGRAY);

                Vector2 mousePosition = GetMousePosition();
                // Makes the grids visible to the player
                PlayerGrid(grid1Position);
                OpponentGrid(grid2Position);

                // Highlight cells (yellow, opponent grid)
                HighlightCell(grid2Position, mousePosition);

                //Place the opponent's and player's ships
                PlaceOpponentShips();
                PlacePlayerShips(grid1Position);
                //Shows the opponent ship places after the game ends
                ShowOpponentShips(grid2Position);

                //Write the information on the grid
                DrawGameText();

                //Turn taking mechanism
                HandleOpponentTurn();
                HandlePlayerTurn(mousePosition);

                //Function call for the preview and placement of the player ships
                HandleShipPlacementAndPreview(mousePosition);
                
                //Show the hit markers for both player and opponent grid
                DrawPlayerShots(grid1Position);
                DrawOpponentShots(grid2Position);

                //Information about the shot ships and pop-up messages
                gameStats.DrawGameStats();

                //Reset the whole game and variables (Prevents player to quit and relaunching the game everytime)
                gameUI.DrawReturnToTitleScreen();
                gameUI.DrawQuitButton();
                

                EndDrawing();
                break;
            }
        }
    }

    // Clean up
    CloseWindow();
    return 0;
}

