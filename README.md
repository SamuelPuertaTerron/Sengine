# Sengine  
*A lightweight 2D game engine written in C++20, built on top of [Raylib](https://www.raylib.com/).*

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue.svg)
![Platform](https://img.shields.io/badge/platform-Windows-lightgrey.svg)
![Build](https://img.shields.io/badge/build-Premake5-orange.svg)

Sengine is a small, readable engine for making 2D games. It pairs an ECS (EnTT) with Box2D physics, a pixel-perfect renderer, and JSON world serialization.

## Features

- **Virtual resolution canvas.** Write your game at a fixed resolution (e.g. 640×360) and let the engine scale it to any window size.

## Quick Start

This example below creates a world and Draws a ```Hello from Sengine``` text at the centre of the screen. 

```cpp
#include "Globals.h"
#include "Engine/Sengine.h"

namespace Game
{
    using namespace Sengine;

    class GameLayer : public ILayer
    {
    public:
        void OnCreate() override
        {
            Serialization::RegisterComponentSerializers();
            PhysicsSettings::Gravity = 9.81f; // meters/s², +Y is down

            // Every world gets these systems.
            m_Worlds.SetSharedSetup([](World& world)
            {
                world.AddSystem<PhysicsSystem>();
                world.AddSystem<RenderSystem>(Raylib::Camera2D{ .zoom = 1.0f });
            });

            m_Worlds.Register("Level1",
                [] { return std::make_unique<World>(); },
                "Resources/Worlds/Level1.json");

            m_Worlds.Request("Level1");
        }

        void OnTick(float deltaTime) override
        {
            Renderer2D::BeginFrame(Raylib::Color(30, 30, 40, 255));
            m_Worlds.Tick(deltaTime);
            Renderer2D::EndFrame();
        }

    private:
        Assets::AssetManager m_Assets;
        WorldManager m_Worlds{ m_Assets };
    };
{

int main()
{
    Sengine::EngineSpecification spec;
    spec.Width  = 1280;
    spec.Height = 720;
    spec.Title  = "My Game";
    spec.Render.VirtualWidth  = 640;
    spec.Render.VirtualHeight = 360;
    spec.Render.Scaling = Sengine::ScaleMode::Integer;

    std::vector<std::unique_ptr<Sengine::ILayer>> layers;
    layers.push_back(std::make_unique<Game::GameLayer>());

    Sengine::Engine::CreateAndRun(spec, std::move(layers));
}
```

### Creating an entity

```cpp
Entity player = world.CreateEntity("Player");

player.AddComponent<TransformComponent>().Position = { 320.0f, 100.0f };
player.AddComponent<TextureComponent>().Texture = assets.GetTexture("player.png");
player.AddComponent<RigidbodyComponent>().Type = RigidbodyType::DynamicBody;
player.AddComponent<BoxColliderComponent>(); // sized from the sprite automatically

auto& callbacks = player.AddComponent<CollisionCallbacksComponent>();
callbacks.OnTriggerEnter = [](Entity self, Entity other)
{
    Logging::Log(Logging::ELogType::Info, "Player entered a trigger!");
};
```

---

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
