# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

This is a **Student Contact Management System** built with Qt and C++, using a **Binary Search Tree (BST)** to efficiently store and manage student information.

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
│                 BinarySearchTree<T>                     │
│ (Generic BST implementation for efficient data         │
│  storage and retrieval by student ID)                  │
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
    QString studentID;      // Primary key for BST
    QString name;
    QDate birthDate;
    QString gender;
    QString addressName;
    int addressCoordX;
    int addressCoordY;
};
```

### BinarySearchTree<T> (binarysearchtree.h)
Key methods:
- `insert(const T& data)` - O(log n) average case
- `deleteStudent(const QString& id)` - O(log n) average case
- `search(const QString& id, T& result)` - O(log n) average case
- Traversal methods (preorder, inorder, postorder) - O(n)
- Multiple query methods (by name, age, address) - O(n)

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
| `mainwindow.cpp/h`                        | UI event handling and data layer bridge  |
| `binarysearchtree.h`                      | BST data structure implementation        |
| `student.h`                               | Student data structure definition        |
| `StudentMessageManagementSystem.pro`      | Qt project configuration                 |