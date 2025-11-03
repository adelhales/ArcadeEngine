// Copyright (c) 2025 Adel Hales

#pragma once

#include "Scene/Scene.h"

namespace Clicker
{
    const float CURSOR_SPEED = 1200;

    const float TARGET_RADIUS = 30;
    const float TARGET_TELEPORT_COOLDOWN_DURATION = 1;

    const int STATS_SCORE_GOAL = 30;
    const float STATS_FINAL_COOLDOWN_DURATION = 20;
    const int STATS_TEXT_SIZE = 25;

    const sf::Color TARGET_COLOR(sf::Color::White);
    const sf::Color STATS_SCORE_TEXT_COLOR(sf::Color::Green);
    const sf::Color STATS_FINAL_COOLDOWN_TEXT_COLOR(sf::Color::Red);
    const sf::Color STATS_BEST_TIME_TEXT_COLOR(sf::Color::Yellow);

    const std::string TARGET_TEXTURE_FILENAME = "Target.png";

    const std::string_view STATS_BEST_TIME_KEY = "Clicker:Best Time";
}