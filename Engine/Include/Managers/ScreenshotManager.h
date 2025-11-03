// Copyright (c) 2025 Adel Hales

#pragma once

#include <SFML/Graphics/RenderWindow.hpp>

class ScreenshotManager
{
private:
    const sf::RenderWindow& window_;

public:
    ScreenshotManager(const sf::RenderWindow& window);

    void Take() const;
};