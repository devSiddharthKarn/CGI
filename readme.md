# CGI_PROJECT2082

### C++ Graphics Engine: No Bloatware, Easy Handling
**Simple. Effective. Elegant.**

---

**CGI_PROJECT2082** is a lightweight C++ graphics engine designed for creating interactive graphical applications on Windows. Built from the ground up to teach graphics programming fundamentals, it provides direct pixel manipulation, window management, and input handling without heavy dependencies or complex abstractions.

Perfect for learning graphics programming, building small games, or prototyping visual applications.

---

## 🎯 Overview

CGI (C++ Graphics Engine) offers a minimalist approach to graphics programming:

- **Direct Window Control** - Create and manage custom windows with full control over rendering
- **Pixel-Perfect Graphics** - Draw shapes, manipulate individual pixels, and render custom graphics
- **Responsive Input** - Handle keyboard and mouse events for interactive applications
- **Frame Management** - Built-in timing system for smooth animations and consistent FPS
- **Zero Dependencies** - Pure C++ with Win32 API, no external libraries required

**Current Demos:**
- **Flappy Rectangle Game** - A physics-based game demonstrating collision detection and game mechanics
- **Cursor Drawing Demo** - Real-time interactive graphics following mouse input

---

## ✨ Features

### Window Management
- Create, move, resize, and manage custom windows
- Dynamic background colors and window titles
- Full control over window properties and lifecycle

### Graphics Rendering
- Pixel-level manipulation for complete drawing control
- Rectangle primitives with plans for more shapes
- Custom color system with RGB support
- Direct frame buffer access for advanced rendering

### Input Handling
- Real-time keyboard input detection
- Mouse position tracking and button events
- Support for key press and release events
- Foundation for more complex input patterns

### Performance & Timing
- Built-in frame timer for smooth animations
- FPS monitoring and frame rate control
- Configurable update loops for optimal performance

### Additional Features
- Custom bitmap font system (`font.txt` included)
- Extensible architecture for adding new features
- Clean, readable codebase ideal for learning

---

## 🚀 Getting Started

### Prerequisites
- **Windows OS** (Win32 API required)
- **C++ Compiler** (MSVC recommended, MinGW compatible)
- **Visual Studio** or **VS Code** with C++ extension

### Installation

1. **Clone the repository:**
```bash
git clone https://github.com/yourusername/CGI_PROJECT2082.git
cd CGI_PROJECT2082
```

2. **Open in your IDE:**
   - Open the project folder in Visual Studio or VS Code
   - Ensure your C++ compiler is properly configured

3. **Build and run:**
   - Compile `main.cpp` for the Cursor Drawing Demo
   - Compile `cgi_collection.cpp` for the Flappy Rectangle Game

---

## 🎮 Demo Applications

### Flappy Rectangle Game
A simple physics-based game showcasing collision detection and game loop fundamentals.

**Controls:**
- `Space` - Jump
- `R` - Restart after game over

**Run:** Build and execute `cgi_collection.cpp`

### Cursor Drawing Demo
An interactive demo that draws a rectangle following your mouse cursor in real-time.

**Controls:**
- Move your mouse within the window

**Run:** Build and execute `main.cpp`

---

## 💻 Quick Start Example

```cpp
#include "cgi_window.hpp"

// Your update function called every frame
void update_function(cgi::window& win) {
    // Draw a rectangle at (100, 100) with size 50x50 in red
    win.draw_rectangle(100, 100, 50, 50, cgi::color::rgb(255, 0, 0));
    
    // Check for keyboard input
    if (win.is_key_pressed(VK_SPACE)) {
        // Handle space bar press
    }
}

int main() {
    // Create a window: title, x, y, width, height, background color
    cgi::window window("My Graphics App", 50, 50, 800, 600, 
                       cgi::color::rgb(30, 30, 30));
    
    window.create();
    window.show();
    window.run_as(update_function, 60); // Run at 60 FPS
    
    return 0;
}
```

---

## 📁 Project Structure

```
CGI_PROJECT2082/
├── main.cpp                    # Cursor drawing demo entry point
├── cgi_collection.cpp          # Flappy Rectangle game demo
├── cgi_window.hpp              # Core window and graphics API
├── cgi_system_utils.hpp        # Input handling and system utilities
├── cgi_data_types.hpp          # Core data structures (color, buffer)
├── cgi_console.hpp             # Console window support
├── cgi_values.hpp              # Enumerations and constants
├── cgi_includes.hpp            # Common includes and dependencies
├── cgi_std_font_loader.hpp     # Font loading system (WIP)
├── font.txt                    # Bitmap font definition
├── font.fnt                    # Reserved for future font formats
├── .gitignore                  # Git ignore configuration
└── README.md                   # This documentation
```

---

## 🔮 Roadmap & Coming Soon

CGI is under active development. Planned features include:

- [ ] **Text Rendering** - Full integration of the font system for drawing text
- [ ] **Image Loading** - Support for PNG, BMP, and other image formats
- [ ] **Audio System** - Sound effects and music playback
- [ ] **Cross-Platform Support** - Linux and macOS compatibility
- [ ] **Advanced Input** - Multi-key detection, mouse dragging, gamepad support
- [ ] **Shape Primitives** - Circles, lines, polygons, and curves
- [ ] **UI Components** - Buttons, sliders, text boxes, and menus
- [ ] **Resource Management** - Texture loading, sprite sheets, asset management
- [ ] **3D Support** - Basic 3D rendering capabilities
- [ ] **Comprehensive Documentation** - Full API reference and tutorials

*Check the [Issues](https://github.com/yourusername/CGI_PROJECT2082/issues) page for the latest development status.*

---

## ⚠️ Current Limitations

Please be aware of these limitations:

- **Windows Only** - Uses Win32 API exclusively; no Linux/macOS support yet
- **CPU Rendering** - Software-based rendering without GPU acceleration
- **Limited Primitives** - Currently supports rectangles and pixel drawing only
- **Single-Threaded** - All operations run on the main thread
- **No Resource Cleanup** - Manual memory management required in some cases
- **Minimal Error Handling** - Limited validation and error feedback
- **Basic Input** - No support for complex input patterns or gamepads

Despite these limitations, CGI provides an excellent foundation for learning graphics programming and building simple applications.

---

## 🤝 Contributing

Contributions are highly welcome! Here's how you can help:

**Ways to Contribute:**
- Implement features from the roadmap
- Optimize rendering performance
- Add new demo applications or tutorials
- Improve documentation and examples
- Report bugs or suggest enhancements
- Write unit tests

**Contribution Process:**
1. Fork the repository
2. Create a feature branch: `git checkout -b feature/your-feature-name`
3. Commit your changes with clear messages
4. Push to your fork: `git push origin feature/your-feature-name`
5. Open a Pull Request with a detailed description

**Code Guidelines:**
- Follow the existing code style
- Comment complex logic
- Keep functions focused and modular
- Test your changes before submitting

---

## 📄 License

This project is licensed under the **MIT License** - see the [LICENSE](LICENSE) file for details.

You are free to use, modify, and distribute this software for any purpose, including commercial applications.

---

## 📧 Contact & Support

**Questions? Suggestions? Found a bug?**

- **GitHub Issues:** [Report a bug or request a feature](https://github.com/yourusername/CGI_PROJECT2082/issues)
- **Discussions:** [Join the conversation](https://github.com/yourusername/CGI_PROJECT2082/discussions)
- **Maintainer:** [@devSiddharthKarn](https://github.com/devSiddharthKarn)

---

## 🙏 Acknowledgments

Built with passion for graphics programming education. Special thanks to all contributors and the graphics programming community for inspiration and guidance.

---

<div align="center">

**CGI_PROJECT2082** - *Learning Graphics Programming, One Pixel at a Time*

⭐ Star this repo if you find it useful!

</div>