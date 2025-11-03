// Copyright (c) 2025 Adel Hales

#pragma once

#include "Scene/Scene.h"

namespace Snake
{
    const int PLAYER_BODY_SIZE_DEFAULT = 2;
    const float PLAYER_MOVE_COOLDOWN_DURATION = 0.15f;

    const int STATS_SCORE_GOAL = 50;

    const float BONUS_SOUND_VOLUME = 20;

    const sf::Vector2i MAP_SIZE(12, 12);

    const sf::Color PLAYER_COLOR(0, 175, 0);
    const sf::Color STATS_SCORE_TEXT_COLOR(225, 225, 225);
    const sf::Color BONUS_COLOR(175, 0, 0);
    const sf::Color MAP_COLOR(50, 100, 50);
    const sf::Color MAP_OUTLINE_COLOR(sf::Color::Black);

    const std::string TILEMAP_TEXTURE_FILENAME = "Snake.png";
    const std::string BONUS_SOUND_FILENAME = "Bonus.mp3";
}