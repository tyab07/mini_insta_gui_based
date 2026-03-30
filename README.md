# 📸 Mini Instagram

A full-featured, high-performance C++ desktop application that replicates the core Instagram experience using Dear ImGui and OpenGL.

![Mini Instagram Preview](https://raw.githubusercontent.com/your-username/mini_insta/main/assets/preview.png)

## 🚀 Key Features

- **Authentication System**: Secure login and registration flows.
- **Dynamic Feed**: Real-time post loading with high-quality card layouts.
- **Rich User Profiles**: Display user stats, bio, and shared media.
- **Social Interactions**: Robust search, messaging system, and stories.
- **Premium UI**: Instagram-inspired light theme with Roboto typography and FontAwesome integration.

## 🛠️ Tech Stack

- **Core**: C++17
- **UI Framework**: [Dear ImGui](https://github.com/ocornut/imgui)
- **Graphics**: OpenGL 3.0+ / GLFW
- **Database**: SQLite3
- **Media**: `stb_image.h` for advanced image loading

## 🏗️ Architecture Overview

The project is structured for modularity and performance:

- `src/main.cpp`: Entry point, GLFW/OpenGL initialization, and main application loop.
- `src/gui/`: UI logic and state management (`UIManager`).
- `src/db/`: Persistence layer handling SQLite database operations.
- `thirdparty/`: External dependencies (ImGui, etc.).

## 📦 Getting Started

### Prerequisites

- **CMake** (3.10+)
- **GLFW3**, **OpenGL**, and **SQLite3** libraries

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/your-username/mini_insta.git
cd mini_insta

# Create build directory
mkdir build && cd build

# Configure and build
cmake ..
make -j$(nproc)

# Run the application
./Mini_Instagram
```

### Assets

Fonts and styles are located in the `assets/` directory. Ensure font paths in `src/main.cpp` are correct relative to your execution directory.

## 🛠️ Roadmap

- [ ] Mobile-responsive layout adjustments
- [ ] Real-time push notifications
- [ ] Advanced image filter support
- [ ] Direct cloud storage integration

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
