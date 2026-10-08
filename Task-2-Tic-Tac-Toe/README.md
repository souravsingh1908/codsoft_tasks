# Task 2 – Tic-Tac-Toe Game

## Overview

This project is a two-player Tic-Tac-Toe game developed in C++ as part of the CodSoft C++ Programming Virtual Internship.

The application provides both a console-based interface and a native Windows GUI interface. Two players take turns placing `X` and `O` on a 3×3 game board until one player wins or the game ends in a draw.

## Features

- Two-player gameplay
- 3×3 Tic-Tac-Toe board
- Player X and Player O
- Console-based game mode
- Native Windows GUI game mode
- Automatic player turn switching
- Win detection
- Draw detection
- Invalid move handling
- Play Again functionality
- Interactive GUI buttons

## Game Modes

### Console Mode

The user can select the console mode from the main menu.

Players enter a position from 1 to 9 to place their symbol on the board.

```text
  1 | 2 | 3
 ---+---+---
  4 | 5 | 6
 ---+---+---
  7 | 8 | 9
```

The selected position is replaced with the current player's symbol.

## 2. Windows GUI Mode

The application also provides a graphical interface using the Windows API (Win32 API).

Players can click the buttons representing the cells of the 3×3 board to make their moves.

## Technologies Used
- Programming Language: C++
- GUI: Windows API (Win32 API)
- Platform: Windows
- Libraries: <windows.h>, <iostream>, <string>

## Game Logic

The game uses a 3×3 character array to represent the board.

`char board[3][3];`

Each cell initially contains a blank space. When a player makes a valid move, the corresponding cell is updated with `X` or `O`.

## Main Functions

- `resetBoard()` – Clears the board and sets Player X as the starting player.
- `hasWon()` – Checks all rows, columns, and diagonals for a winning combination.
- `isDraw()` – Checks whether all cells are occupied without a winner.
- `printBoard()` – Displays the game board in the console.
- `playConsoleGame()` – Controls the console version of the game.
- `updateGuiBoard()` – Updates the GUI buttons according to the current board state.
- `startGuiGame()` – Initializes or resets the GUI game.
- `runGui()` – Creates and runs the Windows GUI application.

## How to Play
1. Run the program.
2. Select a game mode:
- `1` for Console Game
- `2` for Win32 GUI Game
3. Player X starts the game.
4. Players take turns placing their symbols.
5. The program checks for a winning combination after every move.
6. If all cells are occupied without a winner, the game ends in a draw.
7. In console mode, players can choose whether to play again.
8. In GUI mode, the Play Again button starts a new game.

## Winning Condition

A player wins when the same symbol occupies:
- All three cells of a row
- All three cells of a column
- Either diagonal

## C++ Concepts Used

- Two-dimensional arrays
- Functions
- Loops
- Conditional statements
- Character and string handling
- Input validation
- Game-state management
- Event-driven programming
- Windows API programming

## How to Compile

The program requires a Windows environment and a C++ compiler with Windows API support.

Using MinGW:

`g++ main.cpp -o tic_tac_toe`

Run the executable:

`tic_tac_toe.exe`

## Internship Details

- **Organization:** CodSoft
- **Internship Domain:** C++ Programming
- **Task:** Task 2 – Tic-Tac-Toe Game
