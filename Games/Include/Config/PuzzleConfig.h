// Copyright (c) 2025 Adel Hales

#pragma once

#include "Scene/Scene.h"

namespace Puzzle
{
    const int STATS_TARGET_VALUE = 2048;

    const int GRID_SIZE = 4;

    const float ANIMATION_DURATION = 0.05f;

    const int CELL_TEXT_SIZE = 80;

    const sf::Color CELL_COLOR_EMPTY(120, 110, 100);

    const std::vector<sf::Color> CELLS_COLORS =
    {
        sf::Color::Black,         
        {138, 128, 118}, // 2
        {137, 124, 100}, // 4
        {142, 77, 21},   // 8
        {145, 49, 0},    // 6
        {146, 24, 0},    // 32
        {146, 0, 0},     // 64
        {137, 107, 0},   // 128
        {137, 104, 0},   // 256
        {137, 100, 0},   // 512
        {137, 97, 0},    // 1024
        {137, 94, 0},    // 2048
    };

    const std::vector<sf::Color> CELLS_TEXT_COLORS =
    {
        {139, 136, 132}, {69, 59, 51},
    };
}