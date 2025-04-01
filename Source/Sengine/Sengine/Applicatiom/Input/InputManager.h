/**
 *	Filename: InputManager.h
 *	Date Created: 28/02/25
 *	Purpose: The main class that handles any input related code.
 ***/
#pragma once
#include "KeyCodes.h"

namespace Sengine
{
	enum class EInputAction : uint8_t;

    using AnyInputCode          = std::any;
    using KeyDelegate           = std::function<void()>;
    using KeyCallback           = std::function<void(EKeyCode, EInputAction)>;
    using MouseCallback         = std::function<void(EMouseButton, EInputAction)>;
    using MousePositionCallback = std::function<void(int, int)>;
    using MouseScrollCallback   = std::function<void(double, double)>;

    enum class EInputAction : uint8_t
    {
        Press,
        Release,
        Repeat,
        None
    };

    struct KeyState
    {
        bool IsPressed = false;
        bool IsHeld = false;
        bool IsReleased = false;
        float Duration = 0.0f; // How long the key has been held
    };

    template<typename T>
    struct TAction
    {
        KeyDelegate Callback;
        T Code;
        bool ConsumeInput = false; // Whether this action consumes the input
    };

    struct KeyCodeAction : TAction<EKeyCode> {};
    struct MouseButtonAction : TAction<EMouseButton> {};

    struct ActionMap
    {
        std::string Name;
        bool IsActive = true;
        std::unordered_map<std::string, TAction<AnyInputCode>> ActionList;

        bool operator==(const ActionMap& other) const
        {
            return Name == other.Name;
        }
    };

    class InputManager
    {
    public:
        static bool Init();
        static void Destroy();

        static void Tick(float deltaTime);

        // Key state queries
        static bool IsKeyPressed(EKeyCode code);
        static bool IsKeyHeld(EKeyCode code);
        static bool IsKeyReleased(EKeyCode code);
        static float GetKeyHeldDuration(EKeyCode code);

        // Mouse state queries
        static bool IsMouseButtonPressed(EMouseButton button);
        static bool IsMouseButtonHeld(EMouseButton button);
        static Vector2Int GetMousePosition();
        static Vector2Int GetMouseDelta(); // Movement since last frame

        // Action map management
        static ActionMap& CreateActionMap(const std::string& name);
        //static void RemoveActionMap(const std::string& name);
        //static void SetActionMapActive(const std::string& name, bool active);

        template<typename T>
        static void BindAction(ActionMap& map, const std::string& actionName,
            const T& code, const KeyDelegate& callback, bool consumeInput = false);

    public:
        // Callbacks
        static inline KeyCallback KeyCallback;
        static inline MouseCallback MouseCallback;
        static inline MousePositionCallback MousePositionCallback;
        static inline MouseScrollCallback MouseScrollCallback;
    private:
        static void ProcessKeyEvent(EKeyCode code, EInputAction action);
        static void ProcessMouseButtonEvent(EMouseButton button, EInputAction action);
        static void ProcessMousePositionEvent(int x, int y);
        static void ProcessMouseScrollEvent(double xOffset, double yOffset);

        static inline std::unordered_map<EKeyCode, KeyState> s_KeyStates;
        static inline std::unordered_map<EMouseButton, KeyState> s_MouseStates;
        static inline std::vector<ActionMap> s_ActionMaps;
        static inline Vector2Int s_MousePosition;
        static inline Vector2Int s_LastMousePosition;
        static inline Vector2 s_MouseScroll;
    };
}
