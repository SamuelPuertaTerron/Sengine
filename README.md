# Sengine  
*A lightweight 2D game engine written in C++20, built on top of [Raylib](https://www.raylib.com/).*

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)
![Build](https://img.shields.io/badge/build-Premake5-orange.svg)

Sengine is a small, readable engine for making 2D games. It pairs an ECS (EnTT) with Box2D physics, a pixel-perfect renderer, and JSON world serialization.

## Features

- **Virtual resolution canvas.** Write your game at a fixed resolution (e.g. 640×360) and let the engine scale it to any window size.

## Building

The project uses **Premake 5** and currently supports **Windows / Visual Studio 2022** only.

1. Clone the repository:
```bash
   git clone https://github.com/SamuelPuertaTerron/Sengine.git
```
2. Run **`Build.bat`** to generate the Visual Studio solution.
3. Open the solution and build the selected application. All dependencies are built automatically.

---

## Third-Party Libraries  

Sengine relies on the following libraries:  

- [Raylib](https://www.raylib.com/) – window creation & rendering  
- [EnTT](https://github.com/skypjack/entt) – entity-component system  
- [Sol2](https://github.com/ThePhD/sol2) – Lua scripting  
- [ImGui](https://github.com/ocornut/imgui) & [rlImGui](https://github.com/raylib-extras/rlImGui) – in-game tools GUI  
- [nlohmann/json](https://github.com/nlohmann/json) – JSON serialization  
- [Premake5](https://premake.github.io/) – build configuration  
