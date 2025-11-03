// Copyright (c) 2025 Adel Hales

#pragma once

#include "Scene/Scene.h"

namespace FlappyBird
{
    const int BIRD_COUNT = 50;
    const int BIRD_WEIGHT_COUNT = 4;
    const float BIRD_GRAVITY = 500;
    const float BIRD_JUMP_FORCE = 250;
    const float BIRD_ANGLE_MIN = -35;
    const float BIRD_ANGLE_MAX = 45;
    const float BIRD_ROTATION_VELOCITY_OFFSET = 200;
    const float BIRD_ROTATION_VELOCITY_RANGE = 900;

    const int GENERATION_ELITE_COUNT = BIRD_COUNT / 5;

    const float OBSTACLE_SPAWN_DISTANCE = 300;
    const float OBSTACLE_WIDTH = 50;
    const float OBSTACLE_SPEED = 100;
    const float OBSTACLE_GAP_MIN = 125;
    const float OBSTACLE_GAP_MAX = 175;

    const sf::Color STATS_TEXT_COLOR(175, 175, 175);
    const sf::Color BACKGROUND_COLOR(110, 110, 110);

    const std::string BIRD_TEXTURE_FILENAME = "Bird.png";
    const std::string PIPE_TEXTURE_FILENAME = "Pipe.png";
    const std::string BACKGROUND_TEXTURE_FILENAME = "LandscapeBackground.png";
}