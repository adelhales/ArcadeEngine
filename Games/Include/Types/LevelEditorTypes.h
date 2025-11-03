// Copyright (c) 2025 Adel Hales

#pragma once

#include "Config/LevelEditorConfig.h"

#include "Graphics/TileMap.h"

namespace LevelEditor
{
    enum Action
    {
        MoveLeft, MoveRight, MoveUp, MoveDown, Click
    };

    struct Camera
    {
        sf::View view;
        float speed;
    };

    struct SelectedTile
    {
        std::string input;
        sf::Text text{GetDefaultFont()};
        Tile tile;
        bool used;
    };
}