// Copyright (c) 2025 Adel Hales

#pragma once

#include "Scene/Scene.h"

namespace Pong
{
    const bool TWO_PLAYERS = false;

    const sf::Vector2f PLAYER_SIZE(24, 160);
    const float PLAYER_SPEED = 800;
    const float PLAYER_AI_SPEED_FACTOR = 7;

    const float BALL_RADIUS = 10;
    const float BALL_SPEED = 500;
    const float BALL_SPEED_MAX = 5000;
    const float BALL_SPEED_BONUS_FACTOR = 1.05f;
    const sf::Angle BALL_ANGLE_MAX = sf::degrees(45);

    const int STATS_SCORE_GOAL = 10;

    const sf::Vector2f MAP_LINE_SEGMENT_SIZE(5, 20);

    const sf::Color PLAYER_1_COLOR(sf::Color::Cyan);
    const sf::Color PLAYER_1_SCORE_COLOR(sf::Color::Cyan);
    const sf::Color PLAYER_2_COLOR(sf::Color::Red);
    const sf::Color PLAYER_2_SCORE_COLOR(sf::Color::Red);
    const sf::Color BALL_COLOR(sf::Color::White);
    const sf::Color MAP_LINE_SEGMENT_COLOR(sf::Color::White);
}