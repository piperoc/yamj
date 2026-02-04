# YAMJ (Yet Another Mouse Jiggler)

> **Note:** This program is for **Windows only**. It uses Windows-specific APIs and will not run on macOS or Linux.

A simple Windows mouse jiggler application that prevents your computer from going idle by periodically moving the mouse cursor in a small pattern.

## Overview

This utility moves the mouse cursor in a tiny square pattern (configurable) every few seconds (configurable). The movement keeps your system awake and prevents:

- Screen lock activation
- Sleep mode
- "Away" status in communication apps

## Prerequisites

- **LLVM/Clang** - C++ compiler with MSVC compatibility
- **CMake** (3.20+)
- **Ninja** build system
- **Visual Studio Build Tools** - Required for Windows SDK and runtime libraries
- **VS Code** (recommended for development)

## Building (Manual)

The project must be configured from a Visual Studio Developer PowerShell to ensure the Windows SDK and MSVC runtime libraries are available.

### 1. Open Developer PowerShell

Either open "Developer PowerShell for VS 2026" from the Start menu, or run:

```powershell
& "C:\Program Files (x86)\Microsoft Visual Studio\18\BuildTools\Common7\Tools\Launch-VsDevShell.ps1" -Arch amd64
```

### 2. Configure and Build

```powershell
cd C:\Projects\mouse-jiggler

# Debug build
cmake --preset clang-debug
cmake --build --preset clang-debug

# Release build
cmake --preset clang-release
cmake --build --preset clang-release
```

The executable will be generated at:
- Debug: `build/debug/yamj.exe`
- Release: `build/release/yamj.exe`

## Building and Running from VS Code

This project includes an example VS Code build configuration in `.vscode/tasks.json`.

> **Note:** The provided tasks.json is tailored for a specific PC and Visual Studio installation. You may need to customize the paths and environment setup for your own system. This is why `.vscode/tasks.json` is usually listed in `.gitignore`.

### Steps:
1. Open the project folder in VS Code
2. Press **Ctrl+Shift+B** to run the default build task (Debug build)
3. Or open the Command Palette (**Ctrl+Shift+P**) and type `Tasks: Run Task` to:
    - Configure (Debug/Release)
    - Build (Debug/Release)
    - Clean build directory
    - Run YAMJ (builds and launches the app)

> **Tip:** To set up your own build tasks, see [VS Code documentation](https://code.visualstudio.com/docs/editor/tasks) and adjust the PowerShell/Visual Studio paths as needed for your environment.

> **Note:** The build tasks automatically launch the Visual Studio Developer PowerShell for proper environment setup.

## Usage

Simply run the executable:

```powershell
.\build\debug\yamj.exe
```

- Set your desired interval and movement in the GUI
- Click **Start** to begin jiggling
- Minimize to send to system tray
- Double-click tray icon to restore
- Right-click tray icon to toggle Start/Stop
- Use the Help and About menu for more info

## License

This project is licensed under the terms of the **GNU General Public License v3.0** (GPLv3).

Copyright (C) 2026 Costantino Pipero

See [LICENSE](LICENSE) for details.
