// Copyright (c) 2025 Adel Hales

#pragma once

#include "Config/ClickerConfig.h"

namespace Clicker
{
    enum Action
    {
        Click
    };

    struct Target
    {
        sf::CircleShape shape;
        Cooldown teleportCooldown;
    };

    struct Stats
    {
        int score;
        Cooldown finalCooldown;
        float bestTime;
        sf::Text scoreText{GetDefaultFont()};
        sf::Text finalCooldownText{GetDefaultFont()};
        sf::Text bestTimeText{GetDefaultFont()};
    };
}