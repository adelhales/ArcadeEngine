// Copyright (c) 2025 Adel Hales

#pragma once

#include "Scene/Scene.h"

namespace TowerDefense
{
    const sf::Vector2i MAP_SIZE(18, 18);

    const int STATS_BASE_MONEY = 25;
    const int STATS_BASE_LEVEL = 0;

    const int WAVE_MONEY_BONUS = 10;
    const float WAVE_SPAWN_COOLDOWN_DURATION = 3;

    const int TOWER_MAX_LEVEL = 5;
    const int TOWER_PLACE_COST = 15;
    const int TOWER_UPGRADE_COST = 10;
    const float TOWER_SPAWN_COOLDOWN_DURATION = 1;
    const float TOWER_SHOOT_COOLDOWN_DURATION = 1;

    const int CASTLE_HEALTH = 200;

    const float ENEMY_SPEED = 80;
    const int ENEMY_HEALTH = 90;
    const int ENEMY_DAMAGE = 25;
    const float ENEMY_SPAWN_COOLDOWN_DURATION = 2;

    const float BULLET_SPEED = 500;
    const int BULLET_DAMAGE = 5;
    
    const float HEALTH_BAR_HEIGHT = 5;

    const sf::Color TOWER_COLOR(100, 145, 220);
    const sf::Color TOWER_AREA_COLOR(100, 149, 237, 50);
    const sf::Color CASTLE_COLOR(175, 175, 230);
    const sf::Color BULLET_COLOR(229, 228, 226);
    const sf::Color PATH_COLOR(60, 80, 120);
    const sf::Color PREVIEW_UPGRADE_COLOR(100, 175, 100);
    const sf::Color PREVIEW_INVALID_COLOR(175, 0, 0);
    const sf::Color PREVIEW_INVALID_AREA_COLOR(175, 0, 0, 50);
    const sf::Color HEALTH_BACKGROUND_COLOR(sf::Color::Red);
    const sf::Color HEALTH_FOREGROUND_COLOR(sf::Color::Green);

    const std::vector<sf::Color> ENEMY_COLORS =
    {
        {255, 69, 0}, {138, 43, 226}, {0, 255, 64},
    };

    const std::string CASTLE_TEXTURE_FILENAME = "Castle.png";
    const std::string ENEMY_TEXTURE_FILENAME = "Spaceship.png";
}