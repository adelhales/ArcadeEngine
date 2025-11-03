// Copyright (c) 2025 Adel Hales

#pragma once

#include "Scene/Scene.h"

namespace Runner
{
    const sf::Vector2f PLAYER_SIZE(40, 40);
    const float PLAYER_JUMP_VELOCITY = -600;
    const float PLAYER_FALL_FACTOR = 2.5f;
    const float PLAYER_GRAVITY = 1800;

    const float OBSTACLE_WIDTH = 30;
    const float OBSTACLE_MIN_DISTANCE = 250;
    const float OBSTACLE_MAX_DISTANCE = 450;
    const float OBSTACLE_MIN_HEIGHT = 45;
    const float OBSTACLE_MAX_HEIGHT = 70;
    const float OBSTACLE_AIR_PROBABILITY = 0.2f;

    const float STATS_SPEED_START = 200;
    const float STATS_SPEED_INCREASE = 10;
    const float STATS_SPEED_MAX = 800;

    const float GROUND_HEIGHT = 10;

    const sf::Color PLAYER_COLOR(50, 220, 100);
    const sf::Color OBSTACLE_COLOR_GROUND(220, 80, 80);
    const sf::Color OBSTACLE_COLOR_HIGH(80, 160, 220);
    const sf::Color STATS_DISTANCE_TEXT_COLOR(sf::Color::White);
    const sf::Color GROUND_COLOR(100, 100, 100);
}