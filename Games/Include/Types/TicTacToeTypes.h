// Copyright (c) 2025 Adel Hales

#pragma once

#include "Config/TicTacToeConfig.h"

namespace TicTacToe
{
    struct Cell
    {
        sf::RectangleShape shape;
        char symbol;
        sf::Text text{GetDefaultFont()};
    };

    struct Grid
    {
        std::array<std::array<Cell, GRID_SIZE>, GRID_SIZE> cells;
        sf::Vector2f cellSize;
    };
}