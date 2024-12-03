#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include "raylib.h"
#include "globals.h"

// Functions for grid operations
void HighlightCell(Vector2 gridPosition, Vector2 mousePosition);
bool CanPlaceShip(const Ship& ship, Vector2 gridPosition);
void DrawShipPreview(Ship& ship, Vector2 gridPosition);
bool AreAllShipsPlaced();

// Opponent functions
bool OpponentShoot();
void OpponentGrid(Vector2 gridPosition);
void DrawOpponentShots(Vector2 position);
void PlaceOpponentShips();
void HandleOpponentTurn();

// Player functions
void PlayerGrid(Vector2 position);
void PlacePlayerShips(Vector2 position);
void DrawPlayerShots(Vector2 position);
bool PlayerShoot(Vector2 gridPosition, Vector2 mousePosition);
void HandlePlayerTurn(Vector2 mousePosition);

// Function to handle ship preview
void HandleShipPlacementAndPreview(Vector2 mousePosition);

//Writing the important information
void DrawGameText();
void InitializeUnshotCells();
//showing the opponent ships at the end of the game
void ShowOpponentShips(Vector2 position);

//ai helper functions
void AddAdjacentCells(int row, int col);
void DetermineRotation(int row, int col, const Ship& ship);

#endif