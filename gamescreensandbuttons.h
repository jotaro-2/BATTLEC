#ifndef GAMESCREENSANDBUTTONS_H
#define GAMESCREENSANDBUTTONS_H

#include "raylib.h"
#include <vector>
#include "globals.h"
#include <string>


//Create the buttons in the gameplay screen
class GameScreen {
private:
    // Button properties (Position and Size)

    Rectangle returnButton = {800, 10, 170, 40};
    Rectangle quitButton = {800, 50, 170, 40};

    // Colors for the buttons
    Color BUTTON_HOVER = {RED};
    Color BUTTON_DEFAULT = {BLACK};

    // Private helper functions to handle button clicks
    void ResetGame() {
        // Reset game variables to their initial state
        playerTurn = true;
        currentShipIndex = 0;
        playerShipsDestroyedCount = 0;
        opponentShipsDestroyedCount = 0;
        shotsHitCount = 0;
        shotsMissedCount = 0;
        playerbomb = 100;
        gameWon = false;
        gameLost = false;

        // Reinitialize ship placement
        for (auto& ship : playerShips) {
            ship.placed = false;
            ship.hits = 0;
            ship.startX = -1;
            ship.startY = -1;
        }
        for (auto& ship : opponentShips) {
            ship.placed = false;
            ship.hits = 0;
            ship.startX = -1;
            ship.startY = -1;
        }

        // Reinitialize shots tracking grids
        playerShots = std::vector<std::vector<char>>(GRID_SIZE, std::vector<char>(GRID_SIZE, ' '));
        opponentShots = std::vector<std::vector<char>>(GRID_SIZE, std::vector<char>(GRID_SIZE, ' '));

        // Reinitialize unshot cells
        InitializeUnshotCells();

        // Set game state to main menu
        currentState = MAIN_MENU;
    }

public:
    // Constructor (if needed to initialize any members)
    GameScreen() {}

    // Function to draw the Return to Main Menu button
    void DrawReturnToTitleScreen() {
        Vector2 mousePosition = GetMousePosition();
        bool hoverReturn = CheckCollisionPointRec(mousePosition, returnButton);  // Check if mouse is over button

        // Draw the button with hover effect
        if (hoverReturn) {
            DrawRectangleRec(returnButton, BUTTON_HOVER);
        } else {
            DrawRectangleRec(returnButton, BUTTON_DEFAULT);
        }
        DrawText("Main Menu", 820, 20, 20, WHITE);

        // Handle button click event
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (hoverReturn) {
                ResetGame();
                currentState = MAIN_MENU;  // Transition to main menu
            }
        }
    }

    // Function to draw the Quit button
    void DrawQuitButton() {
        Vector2 mousePosition = GetMousePosition();
        bool hoverQuit = CheckCollisionPointRec(mousePosition, quitButton);  // Check if mouse is over button

        // Draw the button with hover effect
        if (hoverQuit) {
            DrawRectangleRec(quitButton, BUTTON_HOVER);
        } else {
            DrawRectangleRec(quitButton, BUTTON_DEFAULT);
        }
        DrawText("Quit Game", 820, 60, 20, WHITE);

        // Handle button click event
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            if (hoverQuit) {
                CloseWindow();
                exit(0);  // Exit the game
            }
        }
    }
};

//Create the buttons and enables to switch between menus
class MainMenu {
public:
    // Function to display and handle Main Menu Screen
    void ShowMainMenu() {
        //Create the buttons for Start, Credits, Tutorial and Quit Buttons
        Rectangle startButton = {360, 250, 300, 40};
        Rectangle creditsButton = {360, 300, 300, 40};
        Rectangle quitButton = {360, 350, 300, 40};
        
        Color BUTTON_HOVER = {LIGHTGRAY};
        Color BUTTON_DEFAULT = {DARKGRAY};
        
        // Main loop for the menu screen
        while (!WindowShouldClose() && currentState == MAIN_MENU) {
            BeginDrawing();
            ClearBackground(BLACK);
            // Get current mouse position for hover effect
            Vector2 mousePosition = GetMousePosition();
            // Check if the mouse is hovering over each button
            bool hoverStart = CheckCollisionPointRec(mousePosition, startButton);
            bool hoverCredits = CheckCollisionPointRec(mousePosition, creditsButton);
            bool hoverQuit = CheckCollisionPointRec(mousePosition, quitButton);
            
            DrawText("Main Menu", 400, 150, 40, WHITE);
            DrawText("BATTLEC", 400, 100, 40, WHITE);

            // Drawing buttons and handling hover effects
            //Start Button effects
            if (hoverStart) {
                DrawRectangleRec(startButton, BUTTON_HOVER);
            } else {
                DrawRectangleRec(startButton, BUTTON_DEFAULT);
            }
                DrawText("Start", 480, 260, 20, WHITE);

            //Credits Button effects
            if (hoverCredits) {
                DrawRectangleRec(creditsButton, BUTTON_HOVER);
            } else {
                DrawRectangleRec(creditsButton, BUTTON_DEFAULT);
            }
                DrawText("Credits", 470, 310, 20, WHITE);
            
            //Quit Button effects
            if (hoverQuit) {
                DrawRectangleRec(quitButton, BUTTON_HOVER);
            } else {
                DrawRectangleRec(quitButton, BUTTON_DEFAULT);
            }
                DrawText("Quit", 490, 360, 20, WHITE); 
        
            // Handle button clicks
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                if (hoverStart) {
                    currentState = GAMEPLAY;
                    break;
                } else if (hoverCredits) {
                    CreditsScreen(); // Show Credits Screen
                } else if (hoverQuit) {
                    CloseWindow(); // Exit the game
                    exit(0);
                }
            }
        
            EndDrawing();
        }
    }

    // Function to show the Credits screen
    void CreditsScreen() {
        while (!WindowShouldClose()) {
            BeginDrawing();
            ClearBackground(BLACK);
        
            DrawText("Game created by: ", 200, 360, 30, WHITE); 
            DrawText("Emre UYSAL", 480, 350, 40, GREEN);

            DrawText("Press [ESC] to return to the main menu", 10, 10, 20, WHITE);
        
            if (IsKeyPressed(KEY_ESCAPE)) {
                return; // Exit to main menu
            }
        
            EndDrawing();
        }
    }
};

//Writes the information on the gameplay screen
class GameStats {
public:
    // Function to write game statistics on the screen
    void DrawGameStats() {
        // Construct text strings based on global variables
        string playerDestroyedText = "Player ships destroyed: " + to_string(playerShipsDestroyedCount);
        string opponentDestroyedText = "Opponent ships destroyed: " + to_string(opponentShipsDestroyedCount);
        string shotsHitText = "Shots hit: " + to_string(shotsHitCount);
        string shotsMissedText = "Shots missed: " + to_string(shotsMissedCount);
        string playerBombText1 = "Bombs left: " + to_string(playerbomb);

        // Draw game statistics on the screen
        DrawText(playerDestroyedText.c_str(), 10, 10, 20, BLACK);
        DrawText(opponentDestroyedText.c_str(), 10, 40, 20, BLACK);
        DrawText(shotsHitText.c_str(), 10, 70, 20, BLACK);
        DrawText(shotsMissedText.c_str(), 10, 100, 20, BLACK);
        DrawText(playerBombText1.c_str(),10, 130, 20, BLACK);

      

        // Display a win or lose message if applicable
        if (gameWon) {
            DisplayMessage("You Won!", GOLD);
        }

        if (gameLost) {
            DisplayMessage("You Lost!", RED);
        }
    }

private:
    // Helper function to display win or lose messages
    void DisplayMessage(const char* message, Color color) {
        int fontSize = 50;
        int boxWidth = 300;
        int boxHeight = fontSize + 20;
        int boxX = 350;
        int boxY = 200;

        DrawRectangle(boxX, boxY, boxWidth, boxHeight, BLACK);
        DrawText(message, 400, 215, fontSize, color);
    }
};
 

#endif
