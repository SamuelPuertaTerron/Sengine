# Fallen  
*A fantasy 2D game written in C++20 using [Raylib](https://www.raylib.com/).*  

## Build  

The project uses **Premake 5** as the build system and currently supports **Visual Studio 2022** only.  

1. Run **`Build.bat`**  
2. This will generate a Visual Studio solution.  
3. Open the solution and build the currently selected application – all dependencies will be built automatically.  

## Third-Party Libraries  

Fallen relies on the following libraries:  

- [Raylib](https://www.raylib.com/) – window creation & rendering  
- [EnTT](https://github.com/skypjack/entt) – entity-component system  
- [Sol2](https://github.com/ThePhD/sol2) – Lua scripting  
- [ImGui](https://github.com/ocornut/imgui) & [rlImGui](https://github.com/raylib-extras/rlImGui) – in-game tools GUI  
- [nlohmann/json](https://github.com/nlohmann/json) – JSON serialization  
- [Premake5](https://premake.github.io/) – build configuration  
