#define _WIN32_IE 0x0600
#include <windows.h>
#include <commctrl.h>

#include <sstream>
#include <string>
#include <vector>

namespace {

struct Task {
	std::string description;
	bool completed = false;
};

enum ControlId {
	InputId = 1001,
	AddId,
	TaskListId,
	CompleteId,
	RemoveId,
	StatusId
};

const char* WindowClassName = "Win32TodoListWindow";
std::vector<Task> tasks;
HWND taskInput = nullptr;
HWND taskList = nullptr;
HWND statusLabel = nullptr;
HFONT controlFont = nullptr;

void updateStatus(const std::string& message = std::string()) {
	if (statusLabel == nullptr) {
		return;
	}

	if (!message.empty()) {
		SetWindowTextA(statusLabel, message.c_str());
		return;
	}

	std::size_t completedCount = 0;
	for (const Task& task : tasks) {
		if (task.completed) {
			++completedCount;
		}
	}

	std::ostringstream text;
	text << tasks.size() << " tasks | " << tasks.size() - completedCount
		 << " pending | " << completedCount << " completed";
	SetWindowTextA(statusLabel, text.str().c_str());
}

void setTaskStatus(int row) {
	LVITEMA item{};
	item.iSubItem = 1;
	item.pszText = const_cast<char*>(tasks[static_cast<std::size_t>(row)].completed
		? "Completed" : "Pending");
	SendMessageA(taskList, LVM_SETITEMTEXTA, static_cast<WPARAM>(row),
		reinterpret_cast<LPARAM>(&item));
}

void addTask() {
	const int textLength = GetWindowTextLengthA(taskInput);
	if (textLength <= 0) {
		updateStatus("Enter a task before adding it.");
		SetFocus(taskInput);
		return;
	}

	std::string description(static_cast<std::size_t>(textLength) + 1, '\0');
	GetWindowTextA(taskInput, &description[0], textLength + 1);
	description.resize(static_cast<std::size_t>(textLength));
	const std::size_t first = description.find_first_not_of(" \t\r\n");
	if (first == std::string::npos) {
		updateStatus("Enter a task before adding it.");
		SetFocus(taskInput);
		return;
	}
	description = description.substr(first, description.find_last_not_of(" \t\r\n") - first + 1);
	Task newTask;
	newTask.description = description;
	tasks.push_back(newTask);

	LVITEMA item{};
	item.mask = LVIF_TEXT;
	item.iItem = static_cast<int>(tasks.size() - 1);
	item.pszText = const_cast<char*>(tasks.back().description.c_str());
	const int row = static_cast<int>(SendMessageA(taskList, LVM_INSERTITEMA, 0,
		reinterpret_cast<LPARAM>(&item)));
	if (row >= 0) {
		setTaskStatus(row);
		ListView_SetItemState(taskList, row, LVIS_SELECTED | LVIS_FOCUSED,
			LVIS_SELECTED | LVIS_FOCUSED);
	}

	SetWindowTextA(taskInput, "");
	SetFocus(taskInput);
	updateStatus();
}

int selectedTask() {
	return ListView_GetNextItem(taskList, -1, LVNI_SELECTED);
}

void completeSelectedTask() {
	const int row = selectedTask();
	if (row < 0) {
		updateStatus("Select a task to mark it complete.");
		return;
	}

	Task& task = tasks[static_cast<std::size_t>(row)];
	if (task.completed) {
		updateStatus("That task is already completed.");
		return;
	}
	task.completed = true;
	setTaskStatus(row);
	updateStatus();
}

void removeSelectedTask() {
	const int row = selectedTask();
	if (row < 0) {
		updateStatus("Select a task to remove.");
		return;
	}

	tasks.erase(tasks.begin() + row);
	ListView_DeleteItem(taskList, row);
	updateStatus();
}

void createControls(HWND window) {
	controlFont = static_cast<HFONT>(GetStockObject(DEFAULT_GUI_FONT));
	HWND heading = CreateWindowExA(0, "STATIC", "TO-DO LIST",
		WS_CHILD | WS_VISIBLE, 18, 14, 220, 24, window, nullptr,
		GetModuleHandleA(nullptr), nullptr);
	HWND inputLabel = CreateWindowExA(0, "STATIC", "New task",
		WS_CHILD | WS_VISIBLE, 18, 48, 100, 20, window, nullptr,
		GetModuleHandleA(nullptr), nullptr);
	taskInput = CreateWindowExA(WS_EX_CLIENTEDGE, "EDIT", "",
		WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL | WS_TABSTOP,
		18, 70, 560, 30, window, reinterpret_cast<HMENU>(InputId),
		GetModuleHandleA(nullptr), nullptr);
	HWND addButton = CreateWindowExA(0, "BUTTON", "Add task",
		WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
		590, 70, 110, 30, window, reinterpret_cast<HMENU>(AddId),
		GetModuleHandleA(nullptr), nullptr);
	taskList = CreateWindowExA(WS_EX_CLIENTEDGE, WC_LISTVIEWA, "",
		WS_CHILD | WS_VISIBLE | LVS_REPORT | LVS_SINGLESEL | LVS_SHOWSELALWAYS | WS_TABSTOP,
		18, 112, 680, 300, window, reinterpret_cast<HMENU>(TaskListId),
		GetModuleHandleA(nullptr), nullptr);
	HWND completeButton = CreateWindowExA(0, "BUTTON", "Mark completed",
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		18, 430, 130, 30, window, reinterpret_cast<HMENU>(CompleteId),
		GetModuleHandleA(nullptr), nullptr);
	HWND removeButton = CreateWindowExA(0, "BUTTON", "Remove task",
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,
		158, 430, 110, 30, window, reinterpret_cast<HMENU>(RemoveId),
		GetModuleHandleA(nullptr), nullptr);
	statusLabel = CreateWindowExA(0, "STATIC", "0 tasks | 0 pending | 0 completed",
		WS_CHILD | WS_VISIBLE | SS_RIGHT,
		300, 435, 400, 22, window, reinterpret_cast<HMENU>(StatusId),
		GetModuleHandleA(nullptr), nullptr);

	HWND controls[] = {heading, inputLabel, taskInput, addButton, taskList,
		completeButton, removeButton, statusLabel};
	for (HWND control : controls) {
		SendMessageA(control, WM_SETFONT, reinterpret_cast<WPARAM>(controlFont), TRUE);
	}

	ListView_SetExtendedListViewStyle(taskList,
		LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	LVCOLUMNA column{};
	column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
	column.pszText = const_cast<char*>("Task");
	column.cx = 500;
	SendMessageA(taskList, LVM_INSERTCOLUMNA, 0, reinterpret_cast<LPARAM>(&column));
	column.pszText = const_cast<char*>("Status");
	column.cx = 150;
	column.iSubItem = 1;
	SendMessageA(taskList, LVM_INSERTCOLUMNA, 1, reinterpret_cast<LPARAM>(&column));
}

void layoutControls(HWND window) {
	RECT client{};
	GetClientRect(window, &client);
	const int width = client.right - client.left;
	const int height = client.bottom - client.top;
	const int margin = 18;
	const int inputWidth = width - margin * 2 - 120;
	const int listWidth = width - margin * 2;
	const int listHeight = height - 168;

	SetWindowPos(taskInput, nullptr, margin, 70, inputWidth, 30, SWP_NOZORDER);
	SetWindowPos(GetDlgItem(window, AddId), nullptr, width - margin - 110,
		70, 110, 30, SWP_NOZORDER);
	SetWindowPos(taskList, nullptr, margin, 112, listWidth, listHeight, SWP_NOZORDER);
	SetWindowPos(GetDlgItem(window, CompleteId), nullptr, margin, height - 48,
		130, 30, SWP_NOZORDER);
	SetWindowPos(GetDlgItem(window, RemoveId), nullptr, margin + 140, height - 48,
		110, 30, SWP_NOZORDER);
	SetWindowPos(statusLabel, nullptr, width - 420, height - 43,
		402, 22, SWP_NOZORDER);

	LVCOLUMNA column{};
	column.mask = LVCF_WIDTH;
	column.cx = listWidth - 150;
	SendMessageA(taskList, LVM_SETCOLUMNA, 0, reinterpret_cast<LPARAM>(&column));
	column.cx = 130;
	SendMessageA(taskList, LVM_SETCOLUMNA, 1, reinterpret_cast<LPARAM>(&column));
}

LRESULT CALLBACK windowProcedure(HWND window, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		createControls(window);
		return 0;
	case WM_SIZE:
		if (taskList != nullptr) {
			layoutControls(window);
		}
		return 0;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case AddId:
			if (HIWORD(wParam) == BN_CLICKED) {
				addTask();
			}
			return 0;
		case CompleteId:
			if (HIWORD(wParam) == BN_CLICKED) {
				completeSelectedTask();
			}
			return 0;
		case RemoveId:
			if (HIWORD(wParam) == BN_CLICKED) {
				removeSelectedTask();
			}
			return 0;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProcA(window, message, wParam, lParam);
}

}

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int showCommand) {
	INITCOMMONCONTROLSEX commonControls{};
	commonControls.dwSize = sizeof(commonControls);
	commonControls.dwICC = ICC_LISTVIEW_CLASSES;
	InitCommonControlsEx(&commonControls);

	WNDCLASSEXA windowClass{};
	windowClass.cbSize = sizeof(windowClass);
	windowClass.lpfnWndProc = windowProcedure;
	windowClass.hInstance = instance;
	windowClass.hCursor = LoadCursorA(nullptr, MAKEINTRESOURCEA(32512));
	windowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	windowClass.lpszClassName = WindowClassName;
	if (RegisterClassExA(&windowClass) == 0) {
		return 1;
	}

	HWND window = CreateWindowExA(0, WindowClassName, "To-Do List Manager",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 760, 560,
		nullptr, nullptr, instance, nullptr);
	if (window == nullptr) {
		return 1;
	}

	ShowWindow(window, showCommand);
	UpdateWindow(window);

	MSG message{};
	while (GetMessageA(&message, nullptr, 0, 0) > 0) {
		TranslateMessage(&message);
		DispatchMessageA(&message);
	}
	return static_cast<int>(message.wParam);
}
