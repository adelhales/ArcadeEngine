// Copyright (c) 2025 Adel Hales

#include "Managers/ScreenshotManager.h"

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <chrono>
#include <format>
#include <string>

#include "Utils/Log.h"

ScreenshotManager::ScreenshotManager(const sf::RenderWindow& window) :
    window_(window)
{
}

void ScreenshotManager::Take() const
{
    std::string filename = std::format("Screenshot_{:%Y%m%d_%H%M%S}.png",
        floor<std::chrono::milliseconds>(std::chrono::system_clock::now())
    );

    filename.replace(filename.find('.'), 1, "_");

    sf::Texture screenshot(window_.getSize());
    screenshot.update(window_);

    if (screenshot.copyToImage().saveToFile("Content/Screenshots/" + filename))
    {
        LOG_INFO("Screenshot saved as {}", filename);
    }
    else
    {
        LOG_WARNING("Failed to save screenshot to {}", filename);
    }
}