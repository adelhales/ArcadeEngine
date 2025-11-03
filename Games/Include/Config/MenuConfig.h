// Copyright (c) 2025 Adel Hales

#pragma once

#include "Scene/Scene.h"

namespace Menu
{
    const std::array BUTTONS_NAMES =
    { 
        "Clicker",     "Memory Card",    "Tic Tac Toe",  "Mine Sweeper",
        "Runner",      "Tetris",         "Pong",         "Breakout",
        "Flappy Bird", "Space Invaders", "Puzzle",       "Snake",
        "Mine Storm",  "Tower Defense",  "Level Editor", "Adventure"
    };

    const int BUTTONS_PER_ROW = 4;
    const float BUTTONS_PADDING_RATIO = 0.5f;

    const int BUTTON_TEXT_SIZE = 24;

    const int CONTROLS_TEXT_SIZE = 30;

    const float MUSIC_VOLUME = 5;
    const float MUSIC_PITCH = 1;

    const sf::Color BUTTON_COLOR(150, 150, 225);
    const sf::Color BUTTON_HOVERED_COLOR(sf::Color::Cyan);
    const sf::Color BUTTON_TEXT_COLOR(sf::Color::Black);
    const sf::Color CONTROLS_TEXT_COLOR(175, 175, 175);
    const sf::Color CONTROLS_BACKGROUND_COLOR(0, 0, 0, 225);

    const std::string BUTTON_TEXTURE_FILENAME = "Button.png";
    const std::string MUSIC_FILENAME = "Music.mp3";

    const std::array CONTROLS_TEXTS =
    {
        R"(Clicker
            Hit: Mouse Left / Gamepad South)",

        R"(Memory Card
            Reveal: Mouse Button / Gamepad Button)",

        R"(Tic-Tac-Toe
            Select: Mouse Button / Gamepad Button)",

        R"(Minesweeper
            Reveal: Mouse Left / Gamepad South
            Toggle Flag: Mouse Right / Gamepad West)",

        R"(Runner
            Jump: Space / Gamepad South
            Crouch: Left Ctrl / Gamepad West
            Fast Fall: S / Stick Y)",

        R"(Tetris
            Move: Left / Right / A / D / Stick X
            Rotate: Up / W / Gamepad North
            Soft Drop: Down / S / Stick Y
            Hard Drop: Space / Gamepad South)",

        R"(Pong
            Move: W / S / Stick Y)",

        R"(Breakout
            Move: Mouse X / Stick X)",

        R"(Flappy Bird
            No controls - AI plays)",

        R"(Space Invaders
            Move: Left / Right / A / D / Stick X
            Shoot: Space / Gamepad South)",

        R"(Puzzle
            Keyboard: Arrow Keys
            Gamepad: D-Pad / Stick
            Each press shifts tiles)",

        R"(Snake
            Left: Left / A / Stick X- / POV X-
            Right: Right / D / Stick X+ / POV X+
            Up: Up / W / Stick Y- / POV Y+
            Down: Down / S / Stick Y+ / POV Y-)",

        R"(Mine Storm
            Rotate Left: Left / A / Stick X-
            Rotate Right: Right / D / Stick X+
            Thrust: Up / W / Gamepad R2
            Shoot: Space / Gamepad South)",

        R"(Tower Defense
            Place: Mouse Left / Gamepad South
            Upgrade: Mouse Left / Gamepad South
            Toggle Preview: P / Gamepad North)",

        R"(Level Editor
            Move: A / D / W / S / Axes Z / R
            Select Tile: 0..9 / Numpad +/-
            Save: Ctrl + C / Gamepad L1
            Load: Ctrl + V / Gamepad R1
            Toggle Tileset: T / Gamepad West
            Adventure: Q / Gamepad North
            Zoom In: Gamepad R3
            Zoom Out: Gamepad L3
            Click: Mouse Left / Gamepad South)",

        R"(Adventure
            Move: A / D / Stick X
            Sprint: Left Shift / Gamepad L2
            Jump: W / Gamepad R1
            Climb: W / Gamepad R1
            Shoot: Mouse Left / Gamepad R2
            Force Field: Mouse Middle / Gamepad R3)"
    };
}