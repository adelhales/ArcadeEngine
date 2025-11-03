// Copyright (c) 2025 Adel Hales

#pragma once

#include "Config/BreakoutConfig.h"

namespace Breakout
{
    enum Action
    {
        MoveLeft, MoveRight
    };

    struct Stats
    {
        int score;
        int lives;
        sf::Text scoreText{GetDefaultFont()};
        sf::Text livesText{GetDefaultFont()};
    };

    struct Player
    {
        sf::RectangleShape shape;
        bool magnetic;
        Stats stats;
    };

    struct Ball
    {
        sf::CircleShape shape;
        sf::Vector2f direction;
        float speed;
        bool alive;
        bool fire;
        bool invincible;
    };

    struct Brick
    {
        sf::RectangleShape shape;
        int lives;
    };

    enum BonusType
    {
        ExpandPaddle,
        ShrinkPaddle,
        MagnetPaddle,
        AccelerateBall,
        DecelerateBall,
        MultiBall,
        FireBall,
        InvincibleBall,
        Count
    };

    struct Bonus
    {
        BonusType type;
        Cooldown endCooldown;
        sf::Text text{GetDefaultFont()};
    };
}