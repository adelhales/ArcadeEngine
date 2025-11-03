// Copyright (c) 2025 Adel Hales

#pragma once

#include "Config/BounceConfig.h"

namespace Bounce
{
    enum Action
    {
        MoveLeft, MoveRight
    };

    struct Paddle
    {
        sf::RectangleShape shape;
        float speed;
    };

    struct Ball
    {
        sf::CircleShape shape;
        sf::Vector2f direction;
        float speed;
    };

    struct Stats
    {
        int score;
        int highScore;
        int lives;
        sf::Text scoreText{GetDefaultFont()};
        sf::Text highScoreText{GetDefaultFont()};
        sf::Text livesText{GetDefaultFont()};
    };
}