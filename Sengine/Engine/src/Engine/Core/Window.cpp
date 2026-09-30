#include "Globals.h"
#include "Window.h"

#include "Engine/Core/Engine.h"

namespace Sengine
{
    Window::Window(const EngineSpecification& spec)
    {
        Raylib::SetTraceLogCallback(Logging::CustomTraceLog);
        Raylib::SetConfigFlags(Raylib::FLAG_WINDOW_RESIZABLE);

        Raylib::InitWindow(spec.Width, spec.Height, spec.Title.c_str());

        m_WindowSize = { static_cast<float>(spec.Width), static_cast<float>(spec.Height) };

        //Never let the window get smaller than the canvas, so integer scaling is always at least 1x.
        Raylib::SetWindowMinSize(spec.Render.VirtualWidth, spec.Render.VirtualHeight);

        Raylib::SetExitKey(Raylib::KEY_NULL);

        m_bIsWindowRunning = true;
    }

    Window::~Window()
    {
        Raylib::CloseWindow();
    }

    bool Window::GetIsWindowRunning() const
    {
        return m_bIsWindowRunning;
    }

    int Window::GetWidth() const 
    {
        return static_cast<int>(m_WindowSize.x);
    }

    int Window::GetHeight() const
    {
        return static_cast<int>(m_WindowSize.y);
    }

    void Window::SetIsWindowRunning(bool value)
    {
        m_bIsWindowRunning = value;
    }

    void Window::PollEvents()
    {
        if (Raylib::WindowShouldClose())
        {
            m_bIsWindowRunning = false;
        }
    }

    void Window::RequestClose()
    {
        SetIsWindowRunning(false);
    }
}//namespace Sengine