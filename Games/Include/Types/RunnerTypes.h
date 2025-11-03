// Copyright (c) 2025 Adel Hales

#pragma once

#include "Config/RunnerConfig.h"

namespace Runner
{
    enum Action
    {
        Jump, Crouch, Fall
    };

    struct Player
    {
        sf::RectangleShape shape;
        sf::Vector2f velocity;
        bool grounded;
        bool crouching;
    };

    struct Obstacle
    {
        sf::RectangleShape shape;
    };

    struct Stats
    {
        float speed;
        float obstacleNextX;
        float distance;
        sf::Text distanceText{GetDefaultFont()};
    };
}