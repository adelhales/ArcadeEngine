// Copyright (c) 2025 Adel Hales

#pragma once

#include "Config/SnakeConfig.h"

namespace Snake
{
    enum Action
    {
        MoveLeft, MoveRight, MoveUp, MoveDown
    };

    struct Map
    {
        sf::Vector2f tileSize;
        std::vector<sf::RectangleShape> grid;
    };

    enum class Direction
    {
        Up, Down, Left, Right, None
    };

    struct Part
    {
        sf::RectangleShape shape;
        sf::Vector2f lastPosition;
        sf::Vector2f nextPosition;
        Direction direction;
    };

    struct Corner
    {
        sf::RectangleShape shape;
        sf::RectangleShape background;
    };

    struct Stats
    {
        int score;
        sf::Text scoreText{GetDefaultFont()};
    };

    struct Player
    {
        Part head;
        std::vector<Part> body;
        std::vector<Corner> corners;
        Direction nextDirection;
        Stats stats;
        std::unordered_map<std::string, sf::IntRect> texturesRect;
        Cooldown moveCooldown;
    };

    struct Bonus
    {
        sf::RectangleShape shape;
    };
}