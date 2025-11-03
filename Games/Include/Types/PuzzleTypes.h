// Copyright (c) 2025 Adel Hales

#pragma once

#include "Config/PuzzleConfig.h"

namespace Puzzle
{
    enum Action
    {
        MoveLeft, MoveRight, MoveUp, MoveDown
    };

    struct Cell
    {
        sf::RectangleShape shape;
        int targetValue;
        int value;
        sf::Text text{GetDefaultFont()};
        sf::Vector2f startPosition;
        sf::Vector2f endPosition;
        bool animating;
    };

    struct Grid
    {
        std::array<std::array<Cell, GRID_SIZE>, GRID_SIZE> cells;
        std::array<std::array<sf::RectangleShape, GRID_SIZE>, GRID_SIZE> background;
    };

    struct Stats
    {
        float animationProgress;
        bool previousPressed;
    };
}