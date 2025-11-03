// Copyright (c) 2025 Adel Hales

#pragma once

#include "Config/MenuConfig.h"

namespace Menu
{
    struct Button
    {
        sf::RectangleShape shape;
        std::string name;
        sf::Text text{GetDefaultFont()};
    };

    struct Controls
    {
        sf::RectangleShape background;
        std::vector<sf::Text> texts;
        std::optional<int> current;
    };
} 