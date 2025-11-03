// Copyright (c) 2025 Adel Hales

#pragma once

#include "Scene/Scene.h"

namespace Tetris
{
    const int GRID_WIDTH = 10;
    const int GRID_HEIGHT = 20;

    const float BLOCK_SIZE = 32;

    const int STATS_SCORE_PER_LINE = 100;

    const float ACTION_COOLDOWN_DURATION = 0.25f;
    const float PIECE_FALL_COOLDOWN_DURATION = 0.5f;
    const float SOFT_DROP_COOLDOWN_DURATION = 0.05f;

    const int PIECE_TYPE_COUNT = 7;
    const int PIECE_BLOCK_COUNT = 4;

    const sf::Color GRID_COLOR(125, 125, 125);

    const std::vector<std::vector<sf::Vector2i>> PIECE_SHAPES =
    {
        {{0, 1}, {1, 1}, {2, 1}, {3, 1}}, // I
        {{1, 0}, {2, 0}, {1, 1}, {2, 1}}, // O
        {{1, 0}, {0, 1}, {1, 1}, {2, 1}}, // T
        {{1, 0}, {2, 0}, {0, 1}, {1, 1}}, // S
        {{0, 0}, {1, 0}, {1, 1}, {2, 1}}, // Z
        {{0, 0}, {0, 1}, {1, 1}, {2, 1}}, // J
        {{2, 0}, {0, 1}, {1, 1}, {2, 1}}, // L
    };

    const std::vector<sf::Vector2f> PIECE_PIVOTS =
    {
        {1.5f, 1.5f}, // I
        {1.5f, 0.5f}, // O
        {1.0f, 1.0f}, // T
        {1.0f, 1.0f}, // S
        {1.0f, 1.0f}, // Z
        {1.0f, 1.0f}, // J
        {1.0f, 1.0f}, // L
    };
}