# Task 3 – To-Do List Manager

## Overview

This project is a graphical To-Do List Manager developed in C++ as part of the CodSoft C++ Programming Virtual Internship.

The application allows users to create and manage a list of tasks through a native Windows graphical interface. Users can add tasks, view their current status, mark tasks as completed, and remove tasks from the list.

## Features

- Add new tasks
- Display tasks in a graphical list
- Show task status as Pending or Completed
- Mark selected tasks as completed
- Remove selected tasks
- Display total task count
- Display pending task count
- Display completed task count
- Input validation
- Prevents adding empty or whitespace-only tasks
- Resizable application window
- Native Windows GUI

## User Interface

The application provides a graphical interface containing:

- Task input field
- Add Task button
- Task list
- Task status column
- Mark Completed button
- Remove Task button
- Task statistics/status display

## Technologies Used

- **Programming Language:** C++
- **GUI:** Windows API (Win32 API)
- **Common Controls:** Windows ListView
- **Platform:** Windows
- **Libraries:** `<windows.h>`, `<commctrl.h>`, `<sstream>`, `<string>`, `<vector>`

## Data Structure

Each task is represented using a `Task` structure:

```cpp
struct Task {
    std::string description;
    bool completed = false;
};
```


The tasks are stored in a vector:

`std::vector<Task> tasks;`

Each task contains:

- Description – The task entered by the user
- Completed – Boolean value representing whether the task has been completed

## Main Functions

`addTask()`

Adds a new task to the task list after validating the user input.

`selectedTask()`

Returns the index of the currently selected task in the ListView.

`completeSelectedTask()`

Marks the selected task as completed and updates its status in the task list.

`removeSelectedTask()`

Removes the selected task from both the internal task vector and the graphical task list.

`updateStatus()`

Updates the status information showing:

- Total number of tasks
- Number of pending tasks
- Number of completed tasks

`setTaskStatus()`

Updates the status column of a task between `Pending` and `Completed`.

`layoutControls()`

Adjusts the position and size of GUI controls when the application window is resized.

## How the Application Works

1. Launch the application.
2. Enter a task in the New Task input field.
3. Click Add Task.
4. The task is added to the task list with a Pending status.
5. Select a task from the list.
6. Click Mark Completed to change its status to Completed.
7. Select a task and click Remove Task to delete it.
8. The status bar continuously displays the total, pending, and completed task counts.

## Input Validation

The application validates task input before adding it.

If the input is:
- Empty
- Contains only spaces
- Contains only whitespace characters
the task is not added and the application displays an appropriate message.

## C++ Concepts Used
- Structures
- Classes and objects concepts
- Vectors
- Strings
- Functions
- Loops
- Conditional statements
- Boolean variables
- Input validation
- Event-driven programming
- Windows API programming
- Windows Common Controls
- Dynamic GUI layout

## How to Compile

The program requires a Windows environment and a C++ compiler with Windows API and Common Controls support.

Using MinGW:

`g++ main.cpp -o todo_list -lcomctl32`

Run the executable:

`todo_list.exe`

## Internship Details

Organization: CodSoft
Internship Domain: C++ Programming
Task: Task 3 – To-Do List Manager
