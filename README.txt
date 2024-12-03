# BattleC C++
BattleC is a turn-based naval combat game built using the Raylib framework. Players place their ships on a grid, take turns shooting at their opponent's grid, and try to sink all their opponent's ships before running out of shots. 
The game includes features such as ship placement mechanics, game state management, and an interactive UI.

## Features
Turn-Based Gameplay: Players and AI take alternating turns, with extra turns awarded for successful hits. Both player and the opponent will have their turn until they fully destroy the ship or miss their shot.
Shot Counter: Both player and the opponent have 100 shots. 

### AI Opponent:
AI opponent starts shooting at random places on the grid until it hits a player ship. After the ship is hit, it keeps targeting at the same ship until it is fully destroyed.
AI will wait until you place your ships on the grid. Both you and the AI cannot start shooting before you place your ships. After you place your ships, AI will wait for your turn to end.

### Grid Mechanics:
Players place ships manually, with visible initials indicating their positions.
Visual feedback for hits, misses, and destroyed ships.
The AI will assign its ship on the grid randomly.
The mouse cursor has a highlighting effect when hovered over the opponent grid.
The player can also see the ship previews before placing the ships on the grid.

### UI Enhancements:
Restart button for resetting the game.
Information panel tracking hits, misses, and ships destroyed.
Display of opponent's ships after game ends.

## Gameplay Instructions
Mouse clicks will either rotate or place the ship on the grid. After the ships are placed, you can choose where you want to shoot on the grid. DON'T FORGET, you can shoot at the same cell again. If you miss, it is opponent's turn.

### Ship Placement
Place ships manually on your grid. Each ship is represented by its first initials and colors.

### Turn Mechanics:
Players and opponents take turns shooting at each other's grids.
Successful hits award an extra turn until the player misses or destroys a ship.

### Victory Conditions:
Destroy all opponent ships before running out of shots.
Restarting the Game: Use Main Menu button from the gameplay screen to go back and start game again. The game will reset and you can start playing again.

## Acknowledgments
Built with the Raylib framework for its simplicity, interactivity and performance.