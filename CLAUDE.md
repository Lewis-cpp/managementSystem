# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a **Student Contact Management System** built with Qt and C++, using **SQLite** for persistent storage and efficient management of student information.

## Key Features

- CRUD operations for student information
- Multiple query methods (by ID, name, age range, address)
- Various traversal display options (pre-order, in-order, post-order)
- File I/O for data import/export
- User-friendly Qt GUI with menu structure

## High-Level Architecture

```
┌─────────────────────────────────────────────────────────┐
│                     User Interface                      │
│ ┌─────────────────────────────────────────────────────┐ │
│ │ StudentMessageManagementSystem.ui (Qt Designer)      │ │
│ └─────────────────────────────────────────────────────┘ │
└─────────────────────────┬───────────────────────────────┘
                           │
┌─────────────────────────▼───────────────────────────────┐
│                     MainWindow                         │
│ (Handles UI events and connects to data layer)         │
└─────────────────────────┬───────────────────────────────┘
                           │
┌─────────────────────────▼───────────────────────────────┐
│                     SQLite Database                    │
│ (Persistent storage for student information using       │
│  SQL queries for efficient data operations)            │
└─────────────────────────┬───────────────────────────────┘
                           │
┌─────────────────────────▼───────────────────────────────┐
│                      Student                           │
│ (Data structure holding student information:           │
│  ID, name, birthdate, gender, address)                │
└─────────────────────────────────────────────────────────┘
```

## Core Data Structures

### Student (student.h)
```cpp
struct Student {
    QString studentID;      // Primary key for SQLite database
    QString name;
    QDate birthDate;
    QString gender;
    QString addressName;
    int addressCoordX;
    int addressCoordY;
};
```

### SQLite Database Integration
The application uses SQLite for persistent data storage:
- MainWindow::db (QSqlDatabase) handles database connections
- SQL queries are used for all CRUD operations
- Table "students" is automatically created if it doesn't exist
- Indexing on studentID provides efficient lookup

## Build and Run

### Prerequisites
- Qt 5.15+ or Qt 6.x
- C++14+ compiler
- Supported OS: Windows, macOS, Linux

### Build with Qt Creator
1. Open `StudentMessageManagementSystem.pro` in Qt Creator
2. Configure the project with appropriate Qt kit
3. Build → Build All (Ctrl+B)
4. Run → Start Debugging (F5)

### Build with Command Line (qmake)
```bash
qmake StudentMessageManagementSystem.pro
make
```

### Build with CMake
```bash
mkdir build && cd build
cmake ..
cmake --build . --config Release
./StudentMessageManagementSystem
```

## Commonly Used Commands

### Build Commands
```bash
# Build with qmake (single command)
qmake StudentMessageManagemantSystem.pro && make

# Build with CMake
mkdir -p build && cd build && cmake .. && cmake --build . --config Release

# Clean build directory
rm -rf build
```

### Linting (if using clang-tidy)
```bash
# Lint all cpp files
clang-tidy *.cpp

# Lint specific files
clang-tidy mainwindow.cpp student.h
```

### Testing
```bash
# Currently no tests implemented
# To run tests (if added in future), use:
ctest -C Release
```

## File Format

Data is imported/exported in **CSV format** with the following structure:
```
studentID,name,birthDate,gender,addressName,addressCoordX,addressCoordY
2021001,����,1999-05-15,��,����������,120,39
```

## Documentation Standards

The project follows **Doxygen** documentation standards for files, functions, and variables. See README.md for detailed Doxygen formatting examples.

## Critical Files

| File                                      | Purpose                                  |
|-------------------------------------------|------------------------------------------|
| `StudentMessageManagementSystem.ui`        | Qt UI layout                              |
| `mainwindow.cpp/h`                        | UI event handling and SQLite database integration |
| `student.h`                               | Student data structure definition        |
| `StudentMessageManagemantSystem.pro`      | Qt project configuration (includes SQL module) |