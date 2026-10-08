#include <windows.h>

#include <cerrno>
#include <cstdlib>
#include <random>
#include <string>

class NumberGuessGame {
public:
	NumberGuessGame(int minimum, int maximum)
		: minimum_(minimum), maximum_(maximum), attempts_(0), target_(0) {}

	void startNewRound() {
		std::random_device device;
		std::mt19937 generator(device());
		std::uniform_int_distribution<int> distribution(minimum_, maximum_);

		target_ = distribution(generator);
		attempts_ = 0;
	}

	bool isValidGuess(int guess) const {
		return guess >= minimum_ && guess <= maximum_;
	}

	bool isCorrect(int guess) const {
		return guess == target_;
	}

	bool isTooHigh(int guess) const {
		return guess > target_;
	}

	void recordAttempt() {
		++attempts_;
	}

	int attempts() const {
		return attempts_;
	}

	int minimum() const {
		return minimum_;
	}

	int maximum() const {
		return maximum_;
	}

private:
	int minimum_;
	int maximum_;
	int attempts_;
	int target_;
};

class GameUI {
public:
	GameUI()
		: window_(nullptr), guessInput_(nullptr), statusLabel_(nullptr),
		  attemptsLabel_(nullptr), game_(1, 100) {}

	int run(HINSTANCE instance, int showCommand) {
		const char* className = "NumberGuessGameWindow";

		WNDCLASSA windowClass{};
		windowClass.hInstance = instance;
		windowClass.lpfnWndProc = windowProcedure;
		windowClass.lpszClassName = className;
		windowClass.hCursor = LoadCursor(nullptr, IDC_ARROW);
		windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);

		if (!RegisterClassA(&windowClass)) {
			return 1;
		}

		window_ = CreateWindowExA(
			0, className, "Number Guess Challenge", WS_OVERLAPPED | WS_CAPTION |
				WS_SYSMENU | WS_MINIMIZEBOX,
			CW_USEDEFAULT, CW_USEDEFAULT, 500, 330, nullptr, nullptr, instance,
			this);

		if (window_ == nullptr) {
			return 1;
		}

		ShowWindow(window_, showCommand);
		UpdateWindow(window_);

		MSG message{};
		while (GetMessageA(&message, nullptr, 0, 0) > 0) {
			TranslateMessage(&message);
			DispatchMessageA(&message);
		}

		return static_cast<int>(message.wParam);
	}

private:
	static constexpr int guessInputId = 1001;
	static constexpr int guessButtonId = 1002;
	static constexpr int newGameButtonId = 1003;

	HWND window_;
	HWND guessInput_;
	HWND statusLabel_;
	HWND attemptsLabel_;
	NumberGuessGame game_;

	static LRESULT CALLBACK windowProcedure(HWND window, UINT message,
		WPARAM wParam, LPARAM lParam) {
		GameUI* userInterface = reinterpret_cast<GameUI*>(
			GetWindowLongPtrA(window, GWLP_USERDATA));

		if (message == WM_NCCREATE) {
			const CREATESTRUCTA* createData =
				reinterpret_cast<const CREATESTRUCTA*>(lParam);
			userInterface = static_cast<GameUI*>(createData->lpCreateParams);
			SetWindowLongPtrA(window, GWLP_USERDATA,
				reinterpret_cast<LONG_PTR>(userInterface));
			userInterface->window_ = window;
		}

		if (userInterface != nullptr) {
			return userInterface->handleMessage(message, wParam, lParam);
		}

		return DefWindowProcA(window, message, wParam, lParam);
	}

	LRESULT handleMessage(UINT message, WPARAM wParam, LPARAM lParam) {
		switch (message) {
		case WM_CREATE:
			createControls();
			startNewRound();
			return 0;

		case WM_COMMAND:
			if (LOWORD(wParam) == guessButtonId && HIWORD(wParam) == BN_CLICKED) {
				processGuess();
				return 0;
			}
			if (LOWORD(wParam) == newGameButtonId && HIWORD(wParam) == BN_CLICKED) {
				startNewRound();
				return 0;
			}
			return 0;

		case WM_CLOSE:
			DestroyWindow(window_);
			return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}

		return DefWindowProcA(window_, message, wParam, lParam);
	}

	void createControls() {
		CreateWindowA("STATIC", "NUMBER GUESS CHALLENGE", WS_CHILD | WS_VISIBLE,
			30, 25, 420, 30, window_, nullptr, nullptr, nullptr);
		CreateWindowA("STATIC", "Guess a number between 1 and 100:",
			WS_CHILD | WS_VISIBLE, 30, 80, 260, 25, window_, nullptr, nullptr,
			nullptr);

		guessInput_ = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "", WS_CHILD |
			WS_VISIBLE | ES_NUMBER | ES_AUTOHSCROLL, 30, 112, 180, 30, window_,
			reinterpret_cast<HMENU>(guessInputId), nullptr, nullptr);
		CreateWindowA("BUTTON", "Guess", WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON,
			225, 112, 100, 30, window_, reinterpret_cast<HMENU>(guessButtonId),
			nullptr, nullptr);
		CreateWindowA("BUTTON", "New Game", WS_CHILD | WS_VISIBLE,
			335, 112, 100, 30, window_, reinterpret_cast<HMENU>(newGameButtonId),
			nullptr, nullptr);

		statusLabel_ = CreateWindowA("STATIC", "", WS_CHILD | WS_VISIBLE,
			30, 170, 405, 30, window_, nullptr, nullptr, nullptr);
		attemptsLabel_ = CreateWindowA("STATIC", "Attempts: 0", WS_CHILD | WS_VISIBLE,
			30, 210, 300, 25, window_, nullptr, nullptr, nullptr);

		SetFocus(guessInput_);
	}

	void startNewRound() {
		game_.startNewRound();
		SetWindowTextA(statusLabel_, "A new number is ready. Make your guess!");
		SetWindowTextA(attemptsLabel_, "Attempts: 0");
		SetWindowTextA(guessInput_, "");
		EnableWindow(GetDlgItem(window_, guessButtonId), TRUE);
		SetFocus(guessInput_);
	}

	void processGuess() {
		char input[32]{};
		GetWindowTextA(guessInput_, input, sizeof(input));

		char* end = nullptr;
		errno = 0;
		long parsedGuess = std::strtol(input, &end, 10);
		if (input[0] == '\0' || *end != '\0' || errno == ERANGE ||
			parsedGuess < game_.minimum() || parsedGuess > game_.maximum()) {
			SetWindowTextA(statusLabel_, "Enter a whole number from 1 to 100.");
			return;
		}

		int guess = static_cast<int>(parsedGuess);
		game_.recordAttempt();
		std::string attemptsText = "Attempts: " + std::to_string(game_.attempts());
		SetWindowTextA(attemptsLabel_, attemptsText.c_str());

		if (game_.isCorrect(guess)) {
			std::string message = "Correct! You won in " +
				std::to_string(game_.attempts()) +
				(game_.attempts() == 1 ? " attempt!" : " attempts!");
			SetWindowTextA(statusLabel_, message.c_str());
			EnableWindow(GetDlgItem(window_, guessButtonId), FALSE);
			return;
		}

		SetWindowTextA(statusLabel_, game_.isTooHigh(guess)
			? "Too high! Try a smaller number."
			: "Too low! Try a larger number.");
		SetWindowTextA(guessInput_, "");
		SetFocus(guessInput_);
	}
};

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int showCommand) {
	GameUI userInterface;
	return userInterface.run(instance, showCommand);
}
