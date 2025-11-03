// Copyright (c) 2025 Adel Hales

#pragma once

#include "Scene/Scene.h"

namespace LevelEditor
{
    const sf::Vector2u MAP_SIZE(50, 50);
    const sf::Vector2u TILE_SIZE(16, 16);

    const float CAMERA_SPEED = 200;

    const int INFOS_TEXT_SIZE = 18;

    const float PLACE_COOLDOWN_DURATION = 0.25f;

    const sf::Color GRID_COLOR(sf::Color::Black);
    const sf::Color INFOS_COLOR(200, 200, 200);
    const sf::Color BACKGROUND_COLOR(110, 110, 110);

    const std::string TILEMAP_LEVEL_FILENAME = "Adventure.txt";
    const std::string TILEMAP_TEXTURE_FILENAME = "TileMap.png";
    const std::string BACKGROUND_TEXTURE_FILENAME = "LandscapeBackground.png";
}