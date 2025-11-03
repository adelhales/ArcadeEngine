// Copyright (c) 2025 Adel Hales

#pragma once

#include "Config/MineStormConfig.h"

#include <queue>

#include "Graphics/AnimatedShape.h"

namespace MineStorm
{
    enum Action
    {
        RotateLeft, RotateRight, Thrust, Shoot
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
        sf::RectangleShape shield;
        sf::Vector2f velocity;
        Stats stats;
        Cooldown shootCooldown;
        Cooldown shieldCooldown;
    };

    enum EnemyType
    {
        Floating = 1,
        Magnetic = 2,
        Fireball = 3,
        MagneticFireball = 4
    };

    struct Enemy
    {
        sf::ConvexShape shape;
        sf::Vector2f direction;
        EnemyType type;
        float speed;
        int lives;
        int size;
    };

    struct EnemySettings
    {
        EnemyType type;
        int size;
    };

    struct Wave
    {
        std::queue<EnemySettings> enemies;
        std::vector<sf::CircleShape> spawns;
    };

    enum class BulletType
    {
        Player, Enemy
    };

    struct Bullet
    {
        sf::CircleShape shape;
        sf::Vector2f direction;
        float speed;
        bool alive;
        BulletType type;
    };

    struct Explosion
    {
        AnimatedShape shape;
    };
}