// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/Window.hpp>

#include <optional>
#include <variant>

enum class GamepadButton
{
    West,   South,  East,  North,
    L1,     R1,     L2,    R2,
    Select, Start,  L3,    R3
};

namespace Input
{
    struct Keyboard { sf::Keyboard::Scan key; };
    struct Mouse    { sf::Mouse::Button button; };
    struct Gamepad  { GamepadButton button; int id = 0; };
    struct Axis     { sf::Joystick::Axis axis; float threshold = 0.5f; int id = 0; };

    using Binding = std::variant<Keyboard, Mouse, Gamepad, Axis>;

    std::optional<int> LogicalToHardware(GamepadButton button, int id);
    std::optional<GamepadButton> HardwareToLogical(int button, int id);
}