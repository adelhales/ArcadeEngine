// Copyright (c) 2025 Adel Hales

#pragma once

#include "Config/SpaceInvadersConfig.h"

namespace SpaceInvaders
{
    enum Action
    {
        MoveLeft,
        MoveRight,
        Shoot
    };

    struct Stats
    {
        int score;
        int lives;
        sf::Text scoreText{GetDefaultFont()};
        sf::Text livesText{GetDefaultFont()};
    };

    struct Player
    {
        sf::RectangleShape shape;
        float speed;
        Stats stats;
        Cooldown shootCooldown;
    };

    struct Enemy
    {
        sf::RectangleShape shape;
        int lives;
    };

    struct Wave
    {
        std::vector<std::vector<Enemy>> enemies;
        sf::Vector2f direction;
        Cooldown moveCooldown;
        Cooldown shootCooldown;
        std::array<sf::Texture*, 3> textures;
    };

    struct BunkerPart
    {
        sf::RectangleShape shape;
        bool alive;
    };

    struct Bunker
    {
        std::vector<BunkerPart> parts;
        sf::RectangleShape bounds;
    };

    struct Bullet
    {
        sf::CircleShape shape;
        sf::Vector2f direction;
        float speed;
        bool alive;
    };
}