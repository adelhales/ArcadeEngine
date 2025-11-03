// Copyright (c) 2025 Adel Hales

#pragma once

#include "Config/AdventureConfig.h"

#include "Graphics/AnimatedShape.h"
#include "Utils/Flags.h"

namespace Adventure
{
    enum Action
    {
        MoveLeft, MoveRight, Sprint, Jump, Climb, Shoot, ForceField
    };

    enum PlayerState
    {
        Grounded  = 1 << 0,
        Climbing  = 1 << 1,
        Sprinting = 1 << 2,
    };

    struct Player
    {
        AnimatedShape shape;
        sf::CircleShape forceField;
        sf::Vector2f direction;
        float verticalVelocity;
        Flags<PlayerState> state;
        sf::Vector2f lastPosition;
        int score;
        int lives;
        sf::Text scoreText{GetDefaultFont()};
        sf::Text livesText{GetDefaultFont()};
        Cooldown jumpCooldown;
        Cooldown shootCooldown;
        Cooldown forceFieldCooldown;
    };

    struct Camera
    {
        sf::View view;
        float smoothFactor;
    };

    struct Bullet
    {
        sf::CircleShape shape;
        sf::Vector2f direction;
        float speed;
        bool alive;
    };

    struct Enemy
    {
        sf::RectangleShape shape;
        sf::Vector2f velocity;
        Cooldown knockbackCooldown;
        int lives;
    };
}