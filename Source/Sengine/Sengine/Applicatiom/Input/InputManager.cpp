#include "Globals.h"
#include "InputManager.h"

#include "Applicatiom/InputManagerSerialization.h"

namespace Sengine
{
    bool InputManager::Init()
    {
        KeyCallback = ProcessKeyEvent;
        MouseCallback = ProcessMouseButtonEvent;
        MousePositionCallback = ProcessMousePositionEvent;

        // Create default engine action map
        auto& engineMap = CreateActionMap("Engine");
        BindAction(engineMap, "ToggleFullscreen", EKeyCode::F11, []()
            {
				//sApplication::GetInstance().ToggleFullscreen();
            });
        BindAction(engineMap, "Quit", EKeyCode::Escape, []() 
            {
				Application::GetInstance().Quit();
            });

        return true;
    }

    void InputManager::Destroy()
    {
        s_KeyStates.clear();
        s_MouseStates.clear();
        s_ActionMaps.clear();
    }

    void InputManager::Tick(float deltaTime)
    {
        // Update key durations
        for (auto& [key, state] : s_KeyStates)
        {
            if (state.IsHeld)
            {
                state.Duration += deltaTime;
                state.IsPressed = false; // Pressed only lasts one frame
            }
            else if (state.IsReleased)
            {
                state.Duration = 0.0f;
                state.IsReleased = false;
            }
        }

        // Update mouse button durations
        for (auto& [button, state] : s_MouseStates)
        {
            if (state.IsHeld)
            {
                state.Duration += deltaTime;
                state.IsPressed = false;
            }
            else if (state.IsReleased)
            {
                state.Duration = 0.0f;
                state.IsReleased = false;
            }
        }

        // Process action maps
        for (auto& map : s_ActionMaps)
        {
            if (!map.IsActive) continue;

            for (auto& [name, action] : map.ActionList)
            {
                if (action.Code.type() == typeid(EKeyCode))
                {
                    auto key = std::any_cast<EKeyCode>(action.Code);
                    if (IsKeyPressed(key) || IsKeyHeld(key))
                    {
                        if (action.Callback) action.Callback();
                        if (action.ConsumeInput) s_KeyStates[key].IsPressed = false;
                    }
                }
                else if (action.Code.type() == typeid(EMouseButton))
                {
                    auto button = std::any_cast<EMouseButton>(action.Code);
                    if (IsMouseButtonPressed(button) || IsMouseButtonHeld(button))
                    {
                        if (action.Callback) action.Callback();
                        if (action.ConsumeInput) s_MouseStates[button].IsPressed = false;
                    }
                }
            }
        }

        s_LastMousePosition = s_MousePosition;
    }

    bool InputManager::IsKeyPressed(EKeyCode code)
    {
	    return s_KeyStates[code].IsPressed;
    }

    bool InputManager::IsKeyHeld(EKeyCode code)
    {
	    return s_KeyStates[code].IsHeld;
    }

    bool InputManager::IsKeyReleased(EKeyCode code)
    {
	    return s_KeyStates[code].IsReleased;
    }

    float InputManager::GetKeyHeldDuration(EKeyCode code)
    {
	    return s_KeyStates[code].Duration;
    }

    // Mouse state queries
    bool InputManager::IsMouseButtonPressed(EMouseButton button)
    {
	    return s_MouseStates[button].IsPressed;
    }

    bool InputManager::IsMouseButtonHeld(EMouseButton button)
    {
	    return s_MouseStates[button].IsHeld;
    }

    Vector2Int InputManager::GetMousePosition()
    {
	    return s_MousePosition;
    }

    Vector2Int InputManager::GetMouseDelta()
    {
	    return s_MousePosition - s_LastMousePosition;
    }

    // Action map management
    ActionMap& InputManager::CreateActionMap(const std::string& name)
    {
        s_ActionMaps.emplace_back();
        s_ActionMaps.back().Name = name;
        return s_ActionMaps.back();
    }

    template<typename T>
    void InputManager::BindAction(ActionMap& map, const std::string& actionName,
        const T& code, const KeyDelegate& callback, bool consumeInput)
    {
        TAction<AnyInputCode> action;
        action.Code = code;
        action.Callback = callback;
        action.ConsumeInput = consumeInput;
        map.ActionList[actionName] = action;
    }

    // Event processing
    void InputManager::ProcessKeyEvent(EKeyCode code, EInputAction action)
    {
        auto& state = s_KeyStates[code];

        switch (action)
        {
        case EInputAction::Press:
            state.IsPressed = true;
            state.IsHeld = true;
            state.IsReleased = false;
            state.Duration = 0.0f;
            break;
        case EInputAction::Release:
            state.IsPressed = false;
            state.IsHeld = false;
            state.IsReleased = true;
            break;
        case EInputAction::Repeat:
            // Duration continues to accumulate in Update
            break;
        default:
            break;
        }
    }

    void InputManager::ProcessMouseButtonEvent(EMouseButton button, EInputAction action)
    {
        auto& state = s_MouseStates[button];

        switch (action)
        {
        case EInputAction::Press:
            state.IsPressed = true;
            state.IsHeld = true;
            state.IsReleased = false;
            state.Duration = 0.0f;
            break;
        case EInputAction::Release:
            state.IsPressed = false;
            state.IsHeld = false;
            state.IsReleased = true;
            break;
        default: 
            break;
        }
    }

    void InputManager::ProcessMousePositionEvent(int x, int y)
    {
        s_MousePosition = { x, y };
    }

    void InputManager::ProcessMouseScrollEvent(double xOffset, double yOffset)
    {
        s_MouseScroll = { xOffset, yOffset };
    }
}