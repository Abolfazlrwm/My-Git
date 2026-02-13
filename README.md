# MyGit - Minimalist Version Control System (v1.0)

MyGit is a lightweight, terminal-based version control system implemented in **Modern C++**. This project demonstrates clean architecture, SOLID principles, and low-level file manipulation without relying on external libraries.



## 🚀 Overview
MyGit mimics the core functionality of Git, allowing you to initialize repositories, track file changes, and navigate through history. It uses a **content-addressable storage** model, where files are identified by their unique hashes.

## ✨ Features (v1.0)
- **Repository Initialization**: Set up the necessary internal structure (`.mygit`).
- **Staging Area**: Snapshot files and track them before committing.
- **Content-Addressable Storage**: Files are stored as objects based on their content hash.
- **Commit History**: Create immutable snapshots of your project with timestamps and messages.
- **Log System**: Traverse and view the history of your project.
- **Checkout**: Restore your working directory to any previous commit state.

## 🏗 Architecture
The project follows a **Layered Architecture**:
1.  **CLI Layer**: Handles user input and command parsing (`main.cpp`).
2.  **Logic Layer**: Manages the orchestration of versioning operations (`MyGit.cpp`).
3.  **Storage Layer**: Handles direct I/O operations and directory management.

## 🛠 Installation & Build
Since this version is designed for maximum compatibility, it doesn't require CMake. You can compile it directly using `g++`.

### Prerequisites
- GCC/G++ Compiler (Supporting C++11 or higher)

### Build Command
Run the following command in the root directory:
```bash
g++ src/*.cpp -I include -o mygit