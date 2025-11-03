// Copyright (c) 2025 Adel Hales

#pragma once

#include "Scene/Scene.h"

namespace MineStorm
{
    const sf::Vector2f PLAYER_SIZE(40, 40);
    const int PLAYER_LIVES = 5;
    const float PLAYER_SHOOT_COOLDOWN_DURATION = 0.25f;
    const float PLAYER_SHIELD_COOLDOWN_DURATION = 3;
    const float PLAYER_ACCELERATION_SPEED = 150;
    const float PLAYER_FRICTION = 0.993f;
    const sf::Angle PLAYER_TURN_ANGLE_SPEED = sf::degrees(200);
    const float PLAYER_BULLET_RADIUS = 3;
    const float PLAYER_BULLET_SPEED = 500;

    const int ENEMY_LIVES = 3;
    const int ENEMY_SIZE = 3;
    const float ENEMY_SCALE = 15;
    const float ENEMY_SPEED = 40;
    const int ENEMY_CHILD_COUNT = 2;
    const int ENEMY_COUNT = 4;
    const float ENEMY_BULLET_RADIUS = 5;
    const float ENEMY_BULLET_SPEED = 150;

    const float EXPLOSION_SCALE = 3;
    const float EXPLOSION_ANIMATION_DURATION = 1.5f / 48;
    const std::vector<int> EXPLOSION_ANIMATION_FRAMES_PER_ROW = {48};

    const sf::Color PLAYER_COLOR(190, 190, 190);
    const sf::Color PLAYER_BULLET_COLOR(sf::Color::White);
    const sf::Color PLAYER_SHIELD_COLOR(57, 255, 20);
    const sf::Color ENEMY_BULLET_COLOR(sf::Color::Red);
    const sf::Color EXPLOSION_COLOR_FACTOR(150, 150, 150);
    const sf::Color SPAWN_POINT_COLOR(sf::Color::Red);

    const std::vector<std::vector<sf::Vector2f>> ENEMY_SHAPES =
    {
        {{0, -1}, {0.2f, 0}, {1, 0.8f},
         {0, 0.4f}, {-1, 0.8f}, {-0.2f, 0}},

        {{-1, -1}, {0, -0.8f}, {1, -1}, {0.8f, 0},
         {1, 1}, {0, 0.8f}, {-1, 1}, {-0.8f, 0}},

        {{0, -1}, {0.2f, -0.2f}, {1, 0}, {0.2f, 0.2f},
         {0, 1}, {-0.2f, 0.2f}, {-1, 0}, {-0.2f, -0.2f}},

        {{-0.8f, -1}, {-0.6f, -0.8f}, {1, -0.8f}, {0.8f, -0.6f},
         {0.8f, 1}, {0.6f, 0.8f}, {-1, 0.8f}, {-0.8f, 0.6f}},
    };

    const std::vector<sf::Color> ENEMY_COLORS =
    {
        {255, 170, 120}, {255, 60, 225}, {0, 255, 255}, {150, 255, 30}
    };

    const std::string PLAYER_TEXTURE_FILENAME = "Spaceship.png";
    const std::string EXPLOSION_TEXTURE_FILENAME = "Explosion.png";
    const std::string BACKGROUND_TEXTURE_FILENAME = "PurpleBackground.png";
    const std::string FOREGROUND_TEXTURE_FILENAME = "MinestormForeground.png";
}