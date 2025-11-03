// Copyright (c) 2025 Adel Hales

#pragma once

#include "Scene/Scene.h"

namespace SpaceInvaders
{
    const int PLAYER_LIVES = 10;
    const float PLAYER_SPEED = 250;
    const float PLAYER_SHOOT_COOLDOWN_DURATION = 0.25f;
    const float PLAYER_BULLET_RADIUS = 4;
    const float PLAYER_BULLET_SPEED = 500;

    const sf::Vector2i WAVE_COUNT(11, 5);
    const float WAVE_SPEEDUP_FACTOR = 0.95f;
    const float WAVE_MOVE_COOLDOWN_DURATION = 0.8f;
    const float WAVE_SHOOT_COOLDOWN_DURATION = 0.2f;

    const int ENEMY_LIVES = 2;
    const float ENEMY_BULLET_RADIUS = 5;
    const float ENEMY_BULLET_SPEED = 500;

    const int BUNKER_COUNT = 4;
    const sf::Vector2f BUNKER_SIZE(50, 50);
    const sf::Vector2i BUNKER_PART_COUNT(10, 10);
    const float BUNKER_OFFSET_TOP_Y = 125;
    const float BUNKER_SPACING_X = 125;

    const sf::Color PLAYER_COLOR(200, 200, 200);
    const sf::Color PLAYER_BULLET_COLOR(sf::Color::White);
    const sf::Color PLAYER_SCORE_TEXT_COLOR(sf::Color::White);
    const sf::Color PLAYER_LIVES_TEXT_COLOR(sf::Color::White);
    const sf::Color ENEMY_FULL_COLOR(sf::Color::White);
    const sf::Color ENEMY_HALF_COLOR(125, 125, 125);
    const sf::Color ENEMY_BULLET_COLOR(255, 0, 50);
    const sf::Color BUNKER_COLOR(122, 138, 154);

    const std::string PLAYER_TEXTURE_FILENAME = "Spaceship.png";
    const std::string ENEMY_1_TEXTURE_FILENAME = "Alien_1.png";
    const std::string ENEMY_2_TEXTURE_FILENAME = "Alien_2.png";
    const std::string ENEMY_3_TEXTURE_FILENAME = "Alien_3.png";
}