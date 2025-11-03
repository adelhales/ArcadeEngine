// Copyright (c) 2025 Adel Hales

#pragma once

#include "Scene/Scene.h"

namespace Adventure
{
    const int PLAYER_LIVES = 10;
    const float PLAYER_GRAVITY = 9.81f;
    const float PLAYER_WALK_SPEED = 40;
    const float PLAYER_SPRINT_SPEED = 60;
    const float PLAYER_JUMP_SPEED = 125;
    const float PLAYER_JUMP_COOLDOWN_DURATION = 0.25f;
    const float PLAYER_FALL_SPEED = 45;
    const float PLAYER_CLIMB_SPEED = 50;
    const float PLAYER_SHOOT_COOLDOWN_DURATION = 0.4f;
    const float PLAYER_FORCE_FIELD_RADIUS = 100;
    const float PLAYER_FORCE_FIELD_FORCE = 200;
    const float PLAYER_FORCE_FIELD_COOLDOWN_DURATION = 20;
    const float PLAYER_ANIMATION_FRAME_DURATION = 0.1f;
    const std::vector<int> PLAYER_ANIMATION_FRAMES_PER_ROW = {9, 9, 9, 9};

    const float CAMERA_SMOOTH_FACTOR = 5;

    const float BULLET_RADIUS = 1;
    const float BULLET_SPEED = 150;

    const int ENEMY_LIVES = 3;
    const float ENEMY_SPEED = 20;
    const float ENEMY_KNOCKBACK_FORCE = 50;
    const float ENEMY_KNOCKBACK_COOLDOWN_DURATION = 0.5f;
    const float ENEMY_SPAWN_RADIUS = 300;
    const float ENEMY_SPAWN_COOLDOWN_DURATION = 5;

    const int BONUS_GOAL = 20;
    const float BONUS_SOUND_VOLUME = 20;

    const sf::Color PLAYER_COLOR(200, 200, 200);
    const sf::Color PLAYER_FORCE_FIELD_COLOR(70, 130, 180, 50);
    const sf::Color ENEMY_COLOR(160, 160, 160);
    const sf::Color BULLET_COLOR(25, 25, 25);
    const sf::Color STATS_TEXT_COLOR(200, 200, 200);
    const sf::Color BACKGROUND_COLOR(110, 110, 110);

    const std::string PLAYER_TEXTURE_FILENAME = "SpriteSheet.png";
    const std::string ENEMY_TEXTURE_FILENAME = "Enemy.png";
    const std::string TILEMAP_LEVEL_FILENAME = "Adventure.txt";
    const std::string TILEMAP_TEXTURE_FILENAME = "TileMap.png";
    const std::string BACKGROUND_TEXTURE_FILENAME = "LandscapeBackground.png";
    const std::string BONUS_SOUND_FILENAME = "Bonus.mp3";
}