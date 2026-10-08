// Two-player Tic-Tac-Toe with both console and native Win32 interfaces.

#include <windows.h>
#include <iostream>
#include <string>

namespace {

char board[3][3];
char currentPlayer = 'X';
HWND cells[3][3]{};
HWND statusLabel = nullptr;
HWND resetButton = nullptr;

void resetBoard() {
	for (auto& row : board) {
		for (char& cell : row) {
			cell = ' ';
		}
	}
	currentPlayer = 'X';
}

bool hasWon(char player) {
	for (int i = 0; i < 3; ++i) {
		if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
			(board[0][i] == player && board[1][i] == player && board[2][i] == player)) {
			return true;
		}
	}

	return (board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
		   (board[0][2] == player && board[1][1] == player && board[2][0] == player);
}

bool isDraw() {
	for (const auto& row : board) {
		for (char cell : row) {
			if (cell == ' ') {
				return false;
			}
		}
	}
	return true;
}

void printBoard() {
	std::cout << "\n  " << board[0][0] << " | " << board[0][1] << " | " << board[0][2] << "\n"
			  << " ---+---+---\n"
			  << "  " << board[1][0] << " | " << board[1][1] << " | " << board[1][2] << "\n"
			  << " ---+---+---\n"
			  << "  " << board[2][0] << " | " << board[2][1] << " | " << board[2][2] << "\n\n";
}

void playConsoleGame() {
	resetBoard();

	while (true) {
		printBoard();
		std::cout << "Player " << currentPlayer << ", choose a position (1-9): ";

		int position;
		if (!(std::cin >> position)) {
			std::cin.clear();
			std::cin.ignore(10000, '\n');
			std::cout << "Please enter a number from 1 to 9.\n";
			continue;
		}

		if (position < 1 || position > 9 || board[(position - 1) / 3][(position - 1) % 3] != ' ') {
			std::cout << "That move is not available. Try again.\n";
			continue;
		}

		board[(position - 1) / 3][(position - 1) % 3] = currentPlayer;
		if (hasWon(currentPlayer)) {
			printBoard();
			std::cout << "Player " << currentPlayer << " wins!\n";
			break;
		}
		if (isDraw()) {
			printBoard();
			std::cout << "The game is a draw.\n";
			break;
		}
		currentPlayer = currentPlayer == 'X' ? 'O' : 'X';
	}
}

void updateGuiBoard() {
	for (int row = 0; row < 3; ++row) {
		for (int column = 0; column < 3; ++column) {
			SetWindowTextA(cells[row][column], std::string(1, board[row][column]).c_str());
			EnableWindow(cells[row][column], board[row][column] == ' ');
		}
	}
}

void startGuiGame() {
	resetBoard();
	updateGuiBoard();
	SetWindowTextA(statusLabel, "Player X's turn");
	EnableWindow(resetButton, TRUE);
}

LRESULT CALLBACK windowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_COMMAND: {
		int controlId = LOWORD(wParam);
		if (controlId == 100) {
			startGuiGame();
			return 0;
		}

		if (controlId >= 1 && controlId <= 9 && board[(controlId - 1) / 3][(controlId - 1) % 3] == ' ') {
			int row = (controlId - 1) / 3;
			int column = (controlId - 1) % 3;
			board[row][column] = currentPlayer;
			updateGuiBoard();

			if (hasWon(currentPlayer)) {
				std::string messageText = "Player ";
				messageText += currentPlayer;
				messageText += " wins!";
				SetWindowTextA(statusLabel, messageText.c_str());
				for (auto& cellRow : cells) {
					for (HWND cell : cellRow) {
						EnableWindow(cell, FALSE);
					}
				}
				return 0;
			}

			if (isDraw()) {
				SetWindowTextA(statusLabel, "The game is a draw.");
				return 0;
			}

			currentPlayer = currentPlayer == 'X' ? 'O' : 'X';
			std::string turnText = "Player ";
			turnText += currentPlayer;
			turnText += "'s turn";
			SetWindowTextA(statusLabel, turnText.c_str());
		}
		return 0;
	}
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		return DefWindowProcA(window, message, wParam, lParam);
	}
}

int runGui(HINSTANCE instance, int showCommand) {
	const char className[] = "TicTacToeWindow";
	WNDCLASSA windowClass{};
	windowClass.hInstance = instance;
	windowClass.lpfnWndProc = windowProcedure;
	windowClass.lpszClassName = className;
	windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);

	if (!RegisterClassA(&windowClass)) {
		return 1;
	}

	HWND window = CreateWindowA(
		className, "Tic-Tac-Toe", WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 360, 460, nullptr, nullptr, instance, nullptr);
	if (!window) {
		return 1;
	}

	statusLabel = CreateWindowA("STATIC", "Player X's turn", WS_VISIBLE | WS_CHILD | SS_CENTER,
							   30, 20, 280, 30, window, nullptr, instance, nullptr);

	for (int row = 0; row < 3; ++row) {
		for (int column = 0; column < 3; ++column) {
			cells[row][column] = CreateWindowA(
				"BUTTON", " ", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
				30 + column * 95, 65 + row * 85, 85, 75, window,
				reinterpret_cast<HMENU>(static_cast<INT_PTR>(row * 3 + column + 1)), instance, nullptr);
		}
	}

	resetButton = CreateWindowA("BUTTON", "Play Again", WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON,
								105, 335, 130, 35, window, reinterpret_cast<HMENU>(100), instance, nullptr);
	startGuiGame();
	ShowWindow(window, showCommand);
	UpdateWindow(window);

	MSG message{};
	while (GetMessageA(&message, nullptr, 0, 0) > 0) {
		TranslateMessage(&message);
		DispatchMessageA(&message);
	}
	return static_cast<int>(message.wParam);
}

} // namespace

int main() {
	std::cout << "Tic-Tac-Toe\n"
			  << "1. Console game\n"
			  << "2. Win32 GUI game\n"
			  << "Choose a mode: ";

	int choice;
	std::cin >> choice;
	if (choice == 1) {
		do {
			playConsoleGame();
			std::cout << "Play again? (y/n): ";
			char answer;
			std::cin >> answer;
			if (answer != 'y' && answer != 'Y') {
				break;
			}
		} while (true);
		return 0;
	}

	if (choice == 2) {
		HINSTANCE instance = GetModuleHandleA(nullptr);
		return runGui(instance, SW_SHOWNORMAL);
	}

	std::cout << "Invalid choice.\n";
	return 1;
}
