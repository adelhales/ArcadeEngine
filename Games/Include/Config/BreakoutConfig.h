// Copyright (c) 2025 Adel Hales

#pragma once

#include "Scene/Scene.h"

namespace Breakout
{
    const int PLAYER_LIVES = 8;
    const int PLAYER_SPEED = 1000;

    const float BALL_RADIUS = 10;
    const float BALL_SPEED = 650;
    const sf::Angle BALL_ANGLE_MAX = sf::degrees(60);
    const float BALL_MAGNET_RANGE = 300;

    const sf::Vector2i BRICK_MAP_SIZE(10, 7);
    const int BRICK_LIVES_MAX = 3;

    const float BONUS_COOLDOWN_DURATION = 5;
    const int BONUS_TEXT_SIZE = 20;
    const float BONUS_TEXT_OFFSET_Y = 25;

    const sf::Color PLAYER_COLOR(sf::Color::White);
    const sf::Color BALL_COLOR(sf::Color::White);
    const sf::Color BALL_FIRE_COLOR(255, 92, 0);

    const std::vector<sf::Color> BRICK_COLORS =
    {
        {120, 220, 250}, {0, 128, 128}, {0, 71, 171}, {18, 10, 143}
    };

    const std::string PLAYER_TEXTURE_FILENAME = "Paddle.png";
}