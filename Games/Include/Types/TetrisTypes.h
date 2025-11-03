// Copyright (c) 2025 Adel Hales

#pragma once

#include "Config/TetrisConfig.h"

namespace Tetris
{
    enum Action
    {
        MoveRight, MoveLeft, Rotate, SoftDrop
    };

    using Board = std::array<std::array<std::optional<sf::Color>, GRID_WIDTH>, GRID_HEIGHT>;

    struct Piece
    {
        int type;
        int rotation;
        sf::Vector2i position;
        sf::Color color;
    };

    struct Stats
    {
        int score;
        sf::Text scoreText{GetDefaultFont()};
    };
}