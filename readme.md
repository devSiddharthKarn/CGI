
---

````markdown
# CGI_PROJECT2082
# CGI : NO BLOATWARE , EASY HANDLING
# Simple Effective Elegant

# introducing CGI(C++ Graphics Ingine)


**CGI_PROJECT2082** is a lightweight C++ graphics engine and demo suite for creating interactive graphical applications on Windows. The project demonstrates basic graphics programming concepts, including window creation, pixel-level drawing, input handling, and frame management. It includes a simple graphics library (`cgi_window.hpp`) and sample applications to showcase its capabilities.

This project is designed as a learning tool for graphics programming and small interactive games, with future plans to expand its features.

---

## Table of Contents

- [Overview](#overview)
- [Features](#features)
- [Installation](#installation)
- [Usage](#usage)
- [Project Structure](#project-structure)
- [Missing Features & Coming Soon](#missing-features--coming-soon)
- [Disadvantages / Limitations](#disadvantages--limitations)
- [License](#license)
- [Contributing](#contributing)

---

## Overview

The CGI_PROJECT2082 library provides the following capabilities:

- Custom **window creation and management** for rendering graphics.
- **Pixel-level graphics** manipulation, including drawing shapes and setting individual pixel colors.
- **Basic input handling** for keyboard and mouse, allowing interactive applications.
- **Demo applications** to illustrate game mechanics and graphics techniques.
- High-level structure suitable for learning graphics programming in C++ on Windows.

Current demos include:

- **Flappy Rectangle Game:** A small physics-based game similar to Flappy Bird.
- **Cursor Drawing Demo:** Interactive rectangle drawing following the mouse cursor.

This project is primarily educational and experimental, targeting developers who want to learn graphics programming concepts in C++.

---

## Features

- **Window Management**
  - Create, move, resize, and close custom windows.
  - Set background color and title dynamically.
- **Pixel Manipulation**
  - Draw rectangles and individual pixels.
  - Prepare the foundation for future shape rendering and graphics primitives.
- **Input Handling**
  - Keyboard and mouse detection.
  - Supports key press, key release, and cursor position tracking.
- **Frame Timing & FPS**
  - Built-in frame timer for smooth animations.
  - Monitor frames per second.
- **Custom Font Support**
  - Bitmap font (`font.txt`) included, ready for text rendering.
- **Demo Applications**
  - **Flappy Rectangle Game**
    - Playable game demo with simple physics, obstacles, and restart logic.
  - **Cursor Drawing Demo**
    - Interactive rectangle follows the mouse cursor in real-time.

---

## Installation

1. **Clone the repository:**

```bash
git clone https://github.com/yourusername/CGI_PROJECT2082.git
cd CGI_PROJECT2082
````

2. **Open in Visual Studio or VS Code**

   * Make sure you have a C++ compiler installed (MSVC recommended for Windows).
3. **Build the project**

   * Open `main.cpp` or `cgi_collection.cpp` as your entry point.
   * Compile and run using your IDE or command line.

---

## Usage

### Running Demos

**Flappy Rectangle Game**

1. Build and run `cgi_collection.cpp`.
2. Controls:

   * `Space`: Jump
   * `R`: Restart after game over

**Cursor Drawing Demo**

1. Build and run `main.cpp`.
2. Move the mouse cursor within the window to draw a red rectangle at the cursor position.

---

### Window API Example

```cpp
#include "cgi_window.hpp"

// Create a new window
cgi::window window("Demo Window", 50, 50, 400, 400, cgi::color::rgb(90, 89, 78));
window.create();
window.show();
window.run_as(update_function, 60); // Run at 60 FPS
```

* `update_function` is your custom function that updates the graphics each frame.

---

## Project Structure

```
CGICopy/CGI/
├── main.cpp                # Cursor drawing demo
├── cgi_collection.cpp      # Flappy Rectangle demo
├── cgi_window.hpp          # Window and graphics API
├── cgi_system_utils.hpp    # Input and system utilities
├── cgi_data_types.hpp      # Core types (color, buffer, etc.)
├── cgi_console.hpp         # Console window support
├── cgi_values.hpp          # Enum values
├── cgi_includes.hpp        # Common includes
├── cgi_std_font_loader.hpp # Font loader stub
├── font.txt                # Bitmap font definition
├── font.fnt                # Reserved for future fonts
├── .gitignore              # Git ignore rules
├── README.md               # This file
└── *.exe                   # Compiled binaries (Windows)
```

---

## Missing Features & Coming Soon

While CGI_PROJECT2082 provides a basic graphics engine, several features are **not yet implemented**:

* **Text Rendering:** Integration of the font system to draw text on windows.
* **Image Loading:** Display images from files (e.g., PNG, BMP).
* **Sound & Music Support:** No audio playback currently.
* **Cross-Platform Support:** Only works on Windows using Win32 API.
* **Advanced Input Handling:** Multi-key detection, mouse drag, or gamepad input.
* **UI Widgets:** Buttons, sliders, menus, and other GUI controls.
* **Documentation:** API docs and tutorials are limited.

*All of these features are planned for future releases!*

---

## Disadvantages / Limitations

* **Windows Only:** Project depends on Win32 API, cannot run on Linux or macOS.
* **CPU-Based Rendering:** No GPU acceleration; complex graphics may run slowly.
* **Minimal Error Handling:** Limited feedback for invalid operations or failed system calls.
* **Limited Graphics Primitives:** Currently only supports rectangles and pixel-level drawing.
* **No Resource Management:** Textures, sprites, or cleanup routines are missing.
* **Single-Threaded:** All rendering and input handling occur in the main thread.

---

## Contributing

Contributions are welcome! You can help by:

* Implementing missing features.
* Improving the graphics engine performance.
* Writing sample demos or tutorials.
* Reporting bugs or suggesting enhancements.

To contribute:

1. Fork the repository.
2. Create a new branch: `git checkout -b feature-name`.
3. Commit your changes.
4. Open a pull request.

---

## License

This project is licensed under the **MIT License**. See `LICENSE` for details.

---

**Contact & Support**

For questions or feedback:

* Open an issue on GitHub.
* Contact the maintainer: `devSiddharthKarn` on GitHub.

---

*CGI_PROJECT2082 is a work-in-progress educational graphics library designed to teach and explore graphics programming in C++. More features and enhancements are coming soon!*

```

---
