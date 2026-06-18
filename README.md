# OS DropZone (Cross-OS Note Sharing)

A modern, lightweight Qt 6 & QML cross-platform application designed for logging notes, links, and code snippets across different operating systems. It is particularly useful for developers using **dual-boot** configurations (e.g., Windows and Linux) sharing a common workspace or drive.

---

## 🎯 Objective
When working on dual-boot machines or multiple OS environments, sharing quick thoughts, terminal commands, snippets, or URLs between sessions is often cumbersome. 

**OS DropZone** solves this by writing to a shared file (`shared_notes.txt`) and automatically tagging every note with the current timestamp and the operating system it was saved from (e.g., `[2026-06-18 15:54 | Saved from Arch Linux]`). The history is instantly reloaded and displayed in a scrollable, monospaced feed when the app opens.

---

## ✨ Features
- **OS Autodetection:** Uses C++ preprocessor macros to detect the active OS (`Windows`, `Arch Linux`, or `Unknown System`) at build/compile time.
- **Timestamped Logs:** Automatically formats note entries with metadata showing exactly *when* and *where* it was written.
- **Persistent Local History:** Saves entries to a text file and reads it back to populate a read-only scrollable workspace history feed.
- **Material Dark UI:** Crafted with Qt Quick Controls 2 using a clean Material Dark theme and a Purple accent.
- **Monospaced Scrollable Feed:** Features a clean log layout using a monospaced font, ideal for rendering code snippets and terminal commands cleanly.

---

## 📂 Project Structure

```
cross-os/
├── CMakeLists.txt      # Qt 6 CMake build configuration (automatic MOC & RCC)
├── main.cpp            # Application entry point; exposes C++ Backend to QML
├── backend.h           # Active C++ backend logic (OS detection, file I/O operations)
├── backend.cpp         # Draft backend file (illustrating custom mount-point paths)
└── main.qml            # Modern QML interface layout (Material Dark design)
```

### File Details:
- **[CMakeLists.txt](file:///home/prakhar/Downloads/cross-os/CMakeLists.txt):** Sets up Qt 6 Gui, Core, Qml, and Quick packages, turning on automatic meta-object compiler (`AUTOMOC`) and resource compiler (`AUTORCC`).
- **[backend.h](file:///home/prakhar/Downloads/cross-os/backend.h):** Contains the `Backend` class declaration and inline methods. Saves files to your user home path (`~/shared_notes.txt` by default) and handles system detection logic.
- **[main.cpp](file:///home/prakhar/Downloads/cross-os/main.cpp):** Registers the C++ `Backend` instance as a global context property (`Backend`) inside the QML context.
- **[main.qml](file:///home/prakhar/Downloads/cross-os/main.qml):** Houses the user interface containing an input text area, save button, and a scrollable text container displaying history logs.
- **[backend.cpp](file:///home/prakhar/Downloads/cross-os/backend.cpp):** A draft implementation that demonstrates targeting a shared mount-point directory (such as `/mnt/shared_workspace/notes.txt`) instead of the user home path.

---

## 🛠️ How to Build and Run

### Prerequisites
Make sure you have the following installed on your system:
- **C++ Compiler** (supporting C++17)
- **CMake** (version >= 3.16)
- **Qt 6 SDK** (specifically Core, Gui, Qml, and Quick modules)

### Building the Project
Navigate to the root directory and run the following terminal commands:

```bash
# 1. Create and enter the build directory
mkdir build
cd build

# 2. Configure the project with CMake
cmake ..

# 3. Build the executable
cmake --build .

# 4. Run the application
./cross-os
```

---

## ⚙️ Customizing the Storage Location
By default, the active database/text file is saved in your user's home folder:
- **Linux:** `/home/<username>/shared_notes.txt`
- **Windows:** `C:\Users\<username>\shared_notes.txt`

To share data between operating systems on a dual-boot machine:
1. Locate your shared partition (e.g. an NTFS/FAT32 drive mounted at `/mnt/shared_workspace/` on Linux and mapped to `D:/` on Windows).
2. Edit the active file path variable inside [backend.h](file:///home/prakhar/Downloads/cross-os/backend.h) (or integrate the logic shown in [backend.cpp](file:///home/prakhar/Downloads/cross-os/backend.cpp)) using preprocessor directives to route to the correct partition paths dynamically:
   ```cpp
   #ifdef _WIN32
       QString sharedPath = "D:/shared_workspace/notes.txt";
   #elif __linux__
       QString sharedPath = "/mnt/shared_workspace/notes.txt";
   #endif
   ```
