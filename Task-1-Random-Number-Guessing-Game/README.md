# Task 1 – Random Number Guessing Game

## Overview

This project is a graphical Random Number Guessing Game developed in C++ as part of the CodSoft C++ Programming Virtual Internship.

The application generates a random number between 1 and 100 and allows the user to guess the number through a graphical interface. After each guess, the application provides feedback indicating whether the guessed number is too high or too low.

## Features

- Generates a random number between 1 and 100
- Graphical user interface using the Windows API
- Accepts user guesses through an input field
- Provides "Too High" and "Too Low" feedback
- Tracks the number of attempts
- Displays the result when the correct number is guessed
- Allows the user to start a new game
- Validates user input

## Technologies Used

- **Programming Language:** C++
- **GUI:** Windows API (Win32 API)
- **Random Number Generation:** C++ `<random>` library
- **Platform:** Windows

## Program Structure

The application is divided into two main classes:

### `NumberGuessGame`

Responsible for the core game logic:

- Generating the target number
- Validating guesses
- Checking whether the guess is correct
- Determining whether the guess is too high
- Tracking the number of attempts

### `GameUI`

Responsible for the graphical user interface:

- Creating the application window
- Creating input fields and buttons
- Processing user interaction
- Displaying game status
- Starting a new game

## How the Game Works

1. Start the application.
2. A random number between 1 and 100 is generated.
3. Enter a guess in the input field.
4. Click the **Guess** button.
5. The application indicates whether the guess is:
   - Too high
   - Too low
   - Correct
6. The number of attempts is updated after every valid guess.
7. When the correct number is guessed, the game displays the result.
8. Click **New Game** to start another round.

## C++ Concepts Used

- Classes and objects
- Constructors
- Encapsulation
- Member functions
- Conditional statements
- Random number generation
- String handling
- Input validation
- Windows message handling
- Event-driven programming

## Internship Details

**Organization:** CodSoft  
**Internship Domain:** C++ Programming  
**Task:** Task 1 – Random Number Guessing Game
