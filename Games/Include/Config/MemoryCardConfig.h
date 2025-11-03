// Copyright (c) 2025 Adel Hales

#pragma once

#include "Scene/Scene.h"

namespace MemoryCard
{
    const int CARDS_COUNT_INITIAL = 12;
    const int CARDS_COUNT_PER_LEVEL = 4;

    const float CARD_PAIR_VISIBLE_COOLDOWN_DURATION = 0.5f;

    const sf::Color CARD_OUTLINE_COLOR(sf::Color::White);

    const std::vector<sf::Color> CARDS_COLORS =
    {
        sf::Color::Red,     sf::Color::Blue,   sf::Color::Green,
        sf::Color::Magenta, sf::Color::Yellow, sf::Color::Cyan,
    };

    const std::string CARD_TEXTURE_FILENAME = "Card.png";
}