# YAMJ (Yet Another Mouse Jiggler)

A simple Windows mouse jiggler application that prevents your computer from going idle by periodically moving the mouse cursor in a small pattern.

## Overview

This utility moves the mouse cursor in a tiny square pattern (1 pixel in each direction) every 30 seconds. The movement is imperceptible during normal use but keeps your system awake and prevents:

- Screen lock activation
- Sleep mode
- "Away" status in communication apps

## Prerequisites

- **LLVM/Clang** - C++ compiler with MSVC compatibility
- **CMake** (3.20+)
- **Ninja** build system
- **Visual Studio Build Tools** - Required for Windows SDK and runtime libraries

## Building

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

## Usage

Simply run the executable:

```powershell
.\build\debug\yamj.exe
```

The application will:
1. Print a startup message
2. Jiggle the mouse every 30 seconds
3. Print a dot (`.`) each time it jiggles

Press `Ctrl+C` to stop the application.

## License

MIT
