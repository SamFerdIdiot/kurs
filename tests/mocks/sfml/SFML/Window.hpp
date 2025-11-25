/**
 * @file Window.hpp
 * @brief Mock SFML Window types for testing without SFML
 * @date 2025-11-18
 * 
 * This file provides mock implementations of SFML Window types
 * to allow testing of code that depends on SFML without requiring
 * SFML to be installed.
 */

#ifndef MOCK_SFML_WINDOW_HPP
#define MOCK_SFML_WINDOW_HPP

#include "System.hpp"
#include <cstdint>

namespace sf {

/**
 * Mock Keyboard - Keyboard input
 */
class Keyboard {
public:
    enum Key {
        Unknown = -1,
        A = 0, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
        Escape, LControl, LShift, LAlt, LSystem,
        RControl, RShift, RAlt, RSystem,
        Menu, LBracket, RBracket, Semicolon, Comma, Period, Quote, Slash, Backslash,
        Tilde, Equal, Hyphen, Space, Enter, Backspace, Tab,
        PageUp, PageDown, End, Home, Insert, Delete,
        Add, Subtract, Multiply, Divide,
        Left, Right, Up, Down,
        Numpad0, Numpad1, Numpad2, Numpad3, Numpad4, Numpad5, Numpad6, Numpad7, Numpad8, Numpad9,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12, F13, F14, F15,
        Pause, KeyCount
    };
    
    static bool isKeyPressed(Key key) { return false; }
};

/**
 * Mock Mouse - Mouse input
 */
class Mouse {
public:
    enum Button {
        Left,
        Right,
        Middle,
        XButton1,
        XButton2,
        ButtonCount
    };
    
    static bool isButtonPressed(Button button) { return false; }
    static Vector2i getPosition() { return Vector2i(0, 0); }
};

/**
 * Mock Event - Input event
 */
class Event {
public:
    enum EventType {
        Closed,
        Resized,
        LostFocus,
        GainedFocus,
        TextEntered,
        KeyPressed,
        KeyReleased,
        MouseWheelMoved,
        MouseWheelScrolled,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseEntered,
        MouseLeft,
        JoystickButtonPressed,
        JoystickButtonReleased,
        JoystickMoved,
        JoystickConnected,
        JoystickDisconnected,
        TouchBegan,
        TouchMoved,
        TouchEnded,
        SensorChanged,
        Count
    };
    
    struct SizeEvent {
        unsigned int width;
        unsigned int height;
    };
    
    struct KeyEvent {
        Keyboard::Key code;
        bool alt;
        bool control;
        bool shift;
        bool system;
    };
    
    struct TextEvent {
        uint32_t unicode;
    };
    
    struct MouseMoveEvent {
        int x;
        int y;
    };
    
    struct MouseButtonEvent {
        Mouse::Button button;
        int x;
        int y;
    };
    
    struct MouseWheelEvent {
        int delta;
        int x;
        int y;
    };
    
    struct MouseWheelScrollEvent {
        Mouse::Button wheel;
        float delta;
        int x;
        int y;
    };
    
    EventType type;
    
    union {
        SizeEvent size;
        KeyEvent key;
        TextEvent text;
        MouseMoveEvent mouseMove;
        MouseButtonEvent mouseButton;
        MouseWheelEvent mouseWheel;
        MouseWheelScrollEvent mouseWheelScroll;
    };
};

} // namespace sf

#endif // MOCK_SFML_WINDOW_HPP
