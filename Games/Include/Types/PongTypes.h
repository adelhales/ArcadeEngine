// Copyright (c) 2025 Adel Hales

#pragma once

#include "Config/PongConfig.h"

namespace Pong
{
    enum Action
    {
        Player1MoveUp, Player1MoveDown,
        Player2MoveUp, Player2MoveDown
    };

    struct Stats
    {
        int score;
        sf::Text scoreText{GetDefaultFont()};
    };

    struct Player
    {
        int joystickId;
        sf::RectangleShape shape;
        float speed;
        Stats stats;
    };

    struct Ball
    {
        sf::CircleShape shape;
        sf::Vector2f direction;
        float speed;
    };
}