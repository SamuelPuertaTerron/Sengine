#pragma once

namespace Sengine::Input
{
    enum class EKeyCode : uint16_t
    {
        // Special Keys
        Invalid = 0,
        Space = 32,
        Escape = 256,
        Enter = 257,
        Tab = 258,
        Backspace = 259,
        Insert = 260,
        Delete = 261,
        Right = 262,
        Left = 263,
        Down = 264,
        Up = 265,
        PageUp = 266,
        PageDown = 267,
        Home = 268,
        End = 269,

        // Modifier Keys
        LeftShift = 340,
        LeftControl = 341,
        LeftAlt = 342,
        RightShift = 344,
        RightControl = 345,
        RightAlt = 346,

        // Function Keys
        F1 = 290, F2 = 291, F3 = 292, F4 = 293, F5 = 294, F6 = 295,
        F7 = 296, F8 = 297, F9 = 298, F10 = 299, F11 = 300, F12 = 301,

        // Letters
        A = 65, B = 66, C = 67, D = 68, E = 69,
        F = 70, G = 71, H = 72, I = 73, J = 74,
        K = 75, L = 76, M = 77, N = 78, O = 79,
        P = 80, Q = 81, R = 82, S = 83, T = 84,
        U = 85, V = 86, W = 87, X = 88, Y = 89,
        Z = 90,

        // Numbers
        Num0 = 48, Num1 = 49, Num2 = 50, Num3 = 51, Num4 = 52,
        Num5 = 53, Num6 = 54, Num7 = 55, Num8 = 56, Num9 = 57
    };

    enum class EMouseButton : uint8_t
    {
        Invalid = 0xFF, // Not a real Raylib value

        Left = 0,
        Right = 1,
        Middle = 2,
        Side = 3,
        Extra = 4,
        Forward = 5,
        Back = 6
    };

    bool IsKeyPressed(EKeyCode keyCode);
    bool IsKeyDown(EKeyCode keyCode);

    bool IsMouseButtonPressed(EMouseButton button);
    bool IsMouseButtonDown(EMouseButton button);

    void ShouldShowMouseCursor(bool value);
    void ShouldLockMouseCursor(bool value);
}//namespace Sengine
