// Copyright (c) 2025 Adel Hales

#pragma once

#include "Config/FlappyBirdConfig.h"

namespace FlappyBird
{
    struct Bird
    {
        sf::RectangleShape shape;
        std::array<float, BIRD_WEIGHT_COUNT> weights;
        sf::Vector2f velocity;
        int targetObstacleIndex;
        bool alive;
        float timeAlive;
    };

    struct Generation
    {
        std::array<Bird, BIRD_COUNT> birds;
        int index;
        int birdCount;
        int score;
        sf::Text indexText{GetDefaultFont()};
        sf::Text birdCountText{GetDefaultFont()};
        sf::Text scoreText{GetDefaultFont()};
    };

    struct Obstacle
    {
        sf::RectangleShape top;
        sf::RectangleShape bottom;
    };
}